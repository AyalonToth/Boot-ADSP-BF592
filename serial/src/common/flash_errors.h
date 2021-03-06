/*******************************************************************************/
/*                                                                             */
/*   (C) Copyright 2012 - Analog Devices, Inc.  All rights reserved.           */
/*                                                                             */
/*    FILE:     flash_errors.h ( )        					                   */
/*                                                                             */
/*    CHANGES:  1.00.0  - initial release    								   */
/*																			   */
/*    PURPOSE:  This header file defines the possible errors				   */
/*                                                                             */
/*******************************************************************************/

#ifndef __FLASH_ERRORS_H__
#define __FLASH_ERRORS_H__

#if defined(ADI_FLASH_DRIVER_RELEASE_BUILD)
#warning "Flash Driver may not work in the Release build"
#endif

#ifdef _MISRA_RULES
#pragma diag(suppress:misra_rule_6_3:"ADI header allows use of basic types")
#pragma diag(suppress:misra_rule_19_15:"ADI header defines and/or undefines macros within a block")
#endif /* _MISRA_RULES */

/* enum of possible errors */
typedef enum
{
	NO_ERR,					/* No Error */
	POLL_TIMEOUT,			/* Polling toggle bit failed */
	VERIFY_WRITE,			/* Verifying write to flash failed */
	INVALID_SECTOR,			/* Invalid Sector */
	INVALID_BLOCK,			/* Invalid Block */
	UNKNOWN_COMMAND,		/* Unknown Command */
	PROCESS_COMMAND_ERR,	/* Processing command */
	NOT_READ_ERROR,			/* Could not read memory from target */
	DRV_NOTAT_BREAK,		/* The application was not at AFP_BreakReady */
	BUFFER_IS_NULL,			/* Could not allocate storage for the buffer */
	NO_ACCESS_SECTOR,		/* Cannot access the sector( could be locked or something is stored there that should not be touched ) */
	WRITE_ERROR,			/* Error writing to memory */
	NO_MODE_SUPPORT,		/* Not a supported operating mode */
	SETUP_ERROR,			/* Error in setup */
	NUM_ERROR_CODES
}ERROR_CODE;

#endif /* __FLASH_ERRORS_H__ */
