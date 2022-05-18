#include <stdlib.h>
#include <string.h>

#include "../common/spi.h"
#include "../common/flash.h"

/* Instruction Set */

#define READ_DATA					0x03
#define FAST_READ					0x0b
#define SECTOR_ERASE				0xd8
#define BULK_ERASE					0xc7
#define PAGE_PROGRAM				0x02
#define READ_STATUS_REGISTER		0x05
#define WRITE_STATUS_REGISTER		0x01
#define WRITE_ENABLE				0x06
#define WRITE_DISABLE				0x04
#define JEDEC_ID					0x9f
#define READ_SIGNATURE				0xab

/* STATUS bits */
#define STATUS_BUSY					0x01
#define STATUS_WEL					0x02


#define NUM_SECTORS 256 // pagina 11
#define SECTOR_SIZE 0x10000


static int m25p16_write_enable(const struct flash_info *fi);
static int m25p16_write_status(const struct flash_info *fi, uint8_t status, uint8_t dummy);

static int m25p16_open(struct flash_info *fi)
{
	fi->size = 0x1000000;
	fi->number_of_regions = 1;
	fi->erase_block_regions = malloc(fi->number_of_regions * sizeof (struct erase_block_region));

	if (fi->erase_block_regions == NULL)
		return -1;

	fi->erase_block_regions[0].block_size = SECTOR_SIZE;
	fi->erase_block_regions[0].number_of_blocks = NUM_SECTORS;

	/* Clear all block protection bits */
	m25p16_write_enable(fi);
	m25p16_write_status(fi, 0, 0);

	return 0;
}

static int m25p16_close(struct flash_info *fi)
{
	free(fi->erase_block_regions);
	return 0;
}

static int m25p16_write_enable(const struct flash_info *fi)
{
	return generic_write_enable(fi, WRITE_ENABLE);
}

static int m25p16_write_disable(const struct flash_info *fi)
{
	return generic_write_disable(fi, WRITE_DISABLE);
}

static int m25p16_read_mid_did(struct flash_info *fi, uint8_t *mid, uint8_t *did)
{
	/* M25P16 uses JEDEC_ID.  */
	return -1;
}

static int m25p16_read_uid(const struct flash_info *fi, uint64_t *uid)
{
	return -1;
}

static int m25p16_read_jedec_id(const struct flash_info *fi, uint8_t *mid,
		uint8_t *memory_type_id, uint8_t *capacity_id)
{
	return generic_read_jedec_id(fi, JEDEC_ID, mid, memory_type_id, capacity_id);
}

static int m25p16_read_status(const struct flash_info *fi, uint8_t *status)
{
	uint8_t tbuf[1];
	uint8_t rbuf[1];
	int count;

	select_flash();

	count = 0;
	assign_instruction(fi, tbuf, READ_STATUS_REGISTER, &count);
	spi_send(tbuf, count);

	spi_recv(rbuf, 1);
	*status = rbuf[0];

	unselect_flash();

	return 0;
}

static int m25p16_not_busy(const uint8_t status)
{
	return (status & STATUS_BUSY) ? 0 : 1;
}

static int m25p16_wait_ready(const struct flash_info *fi)
{
	uint8_t tbuf[1];
	int count;

	select_flash();

	count = 0;
	assign_instruction(fi, tbuf, READ_STATUS_REGISTER, &count);
	spi_send(tbuf, count);

	spi_recv_until(m25p16_not_busy);

	unselect_flash();

	return 0;
}

static int m25p16_write_status(const struct flash_info *fi, uint8_t status, uint8_t dummy)
{
	uint8_t tbuf[2];
	int count;
	uint8_t old_status;

	m25p16_write_enable(fi);

	m25p16_read_status(fi, &old_status);
	if (!(old_status & STATUS_WEL))
		return -1;

	select_flash();

	count = 0;
	assign_instruction(fi, tbuf, WRITE_STATUS_REGISTER, &count);
	tbuf[count++] = status;
	spi_send(tbuf, count);

	unselect_flash();

	m25p16_wait_ready(fi);

	return 0;
}

static int m25p16_read(const struct flash_info *fi, uint32_t addr, uint8_t *buf, int size)
{
	uint8_t read_insn;
	uint8_t tbuf[5];
	int count;

	/*  TODO frequency will limit instruction chosen.  */
	read_insn = READ_DATA;

	select_flash();

	count = 0;
	assign_instruction(fi, tbuf, read_insn, &count);
	assign_address(fi, tbuf, addr, &count);

	if (read_insn == FAST_READ)
		count++;

	spi_send(tbuf, count);

	spi_recv(buf, size);

	unselect_flash();

	return 0;
}

/* TODO make erase functions generic */

/* Internal function used by m25p16_erase and m25p16_erase_chip */

static int m25p16_erase_internal(const struct flash_info *fi,
		uint32_t addr, erase_type_t erase_type)
{
	uint8_t tbuf[4];
	int count;
	uint8_t status;
	uint8_t insn;
	uint32_t addr_mask;

	switch (erase_type)
	{
	case ERASE_4KB:
		insn = SECTOR_ERASE;
		addr_mask = 0xfffff000;
		break;

	case ERASE_CHIP:
		insn = BULK_ERASE;
		addr_mask = 0;
		break;

	default:
		/* bad erase type */
		return -1;
	}

	m25p16_write_enable(fi);

	m25p16_read_status(fi, &status);
	if (!(status & STATUS_WEL))
		return -1;

	select_flash();

	count = 0;
	assign_instruction(fi, tbuf, insn, &count);
	if (erase_type != ERASE_CHIP)
		assign_address(fi, tbuf, addr & addr_mask, &count);
	spi_send(tbuf, count);

	unselect_flash();

	m25p16_wait_ready(fi);

	return 0;
}

static int m25p16_erase(const struct flash_info *fi, uint32_t addr, int size)
{
#define ERASE_TYPE ERASE_4KB
#define ERASE_SIZE (4 * 1024)

	size += addr % ERASE_SIZE;
	addr -= addr % ERASE_SIZE;

	while (size > 0)
	{
		m25p16_erase_internal(fi, addr, ERASE_TYPE);
		addr += ERASE_SIZE;
		size -= ERASE_SIZE;
	}

	return 0;
}

static int m25p16_erase_chip(const struct flash_info *fi)
{
	return m25p16_erase_internal(fi, 0, ERASE_CHIP);
}

#define PAGE_SIZE 256

static int m25p16_page_program (struct flash_info *fi,
		uint32_t addr, const uint8_t *buf, int size)
{
	uint8_t tbuf[4 + size];
	int count;
	uint8_t old_status;

	if ((addr % PAGE_SIZE) + size > PAGE_SIZE)
		return -1;

	m25p16_write_enable (fi);

	m25p16_read_status (fi, &old_status);
	if (!(old_status & STATUS_WEL))
		return -1;

	select_flash ();

	count = 0;
	assign_instruction (fi, tbuf, PAGE_PROGRAM, &count);
	assign_address (fi, tbuf, addr, &count);
	memcpy (tbuf + count, buf, size);
	spi_send (tbuf, count + size);

	unselect_flash ();

	m25p16_wait_ready (fi);

	return 0;
}

/* Program arbitary size */
static int m25p16_program (struct flash_info *fi,
		uint32_t addr, const uint8_t *buf, int size)
{
	int program_size;

	while (size > 0)
	{
		if (addr % PAGE_SIZE != 0)
			program_size = ((PAGE_SIZE - addr % PAGE_SIZE) < size
					? (PAGE_SIZE - addr % PAGE_SIZE) : size);
		else if (size >= PAGE_SIZE)
			program_size = PAGE_SIZE;
		else
			program_size = size;

		m25p16_page_program (fi, addr, buf, program_size);

		addr += program_size;
		buf += program_size;
		size -= program_size;
	}

	return 0;
}

static int m25p16_reset(const struct flash_info *fi)
{
	/* TODO implement it */
	return -1;
}

struct flash_info m25p16_info =
{
	"M25P16",	/* name */
	"Micron",	/* mname */
	0x20,		/* manufacturer ID */
	/* device ID.  M25P16's DID is 16-bit and is "memory type ID, capacity Id".
	 * so we just check them instead of device ID.  */
	0xff,
	0xBA,		/* memory type ID */
	0x18,		/* capacity ID */
	STANDARD,	/* supported modes */

	/* The following three fields are better be
	   initialized in flash_open functions.  */
	0,			/* size in Byte */
	0,			/* number_of_regions */
	NULL,		/* erase_block_regions */

	25,		/* max_freq */
	0,		/* max_quad_freq */
	20,		/* max_read_data_freq */

	0,		/* cpha */
	0,		/* cpol */
	0,		/* lsb_first */
	0,		/* start_on_mosi */

	0,		/* frequency */
	STANDARD,	/* current_mode */

	m25p16_open,
	m25p16_close,

	m25p16_read_mid_did,
	m25p16_read_uid,
	m25p16_read_jedec_id,
	m25p16_read_status,
	NULL, /* read_status2 */
	m25p16_write_status,
	NULL, /* flash_enable_quad_mode */
	NULL, /* flash_disable_quad_mode */
	m25p16_read,
	m25p16_write_enable,
	m25p16_write_disable,
	m25p16_erase,
	m25p16_erase_chip,
	m25p16_program,
	m25p16_reset,
};
