/*
** ADSP-BF592-A CPLB table definitions generated on Sep 10, 2012 at 09:47:46.
*/
/*
** Copyright (C) 2000-2012 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically based upon the options selected
** in the System Configuration utility. Changes to the CPLB configuration
** should be made by modifying the appropriate options rather than editing
** this file. To access the System Configuration utility, double-click the
** system.svc file from a navigation view.
**
** Custom additions can be inserted within the user-modifiable sections,
** these are bounded by comments that start with "$VDSG". Only changes
** placed within these sections are preserved when this file is re-generated.
**
** Product      : CrossCore Embedded Studio 1.0.1.0
** Tool Version : 6.0.2.28
*/

#include <sys/platform.h>
#include <cplbtab.h>

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_2_2)
#pragma diag(suppress:misra_rule_8_10)
#pragma diag(suppress:misra_rule_10_1_a)
#endif /* _MISRA_RULES */

#define CACHE_MEM_MODE (CPLB_DNOCACHE)

#pragma section("cplb_data")

cplb_entry dcplbs_table[] = {


   // L1 Data SRAM (32KB), (set write-through bit to avoid 1st write exceptions)
   {0xFF800000, (PAGE_SIZE_4KB | CPLB_DNOCACHE | CPLB_LOCK | CPLB_WT)}, 
   {0xFF801000, (PAGE_SIZE_4KB | CPLB_DNOCACHE | CPLB_LOCK | CPLB_WT)}, 
   {0xFF802000, (PAGE_SIZE_4KB | CPLB_DNOCACHE | CPLB_LOCK | CPLB_WT)}, 
   {0xFF803000, (PAGE_SIZE_4KB | CPLB_DNOCACHE | CPLB_LOCK | CPLB_WT)}, 
   {0xFF804000, (PAGE_SIZE_4KB | CPLB_DNOCACHE | CPLB_LOCK | CPLB_WT)}, 
   {0xFF805000, (PAGE_SIZE_4KB | CPLB_DNOCACHE | CPLB_LOCK | CPLB_WT)}, 
   {0xFF806000, (PAGE_SIZE_4KB | CPLB_DNOCACHE | CPLB_LOCK | CPLB_WT)}, 
   {0xFF807000, (PAGE_SIZE_4KB | CPLB_DNOCACHE | CPLB_LOCK | CPLB_WT)}, 

   // Boot ROM (4KB)
   {0xEF000000, (PAGE_SIZE_4KB | CPLB_VALID | CPLB_USER_RD)}, 

   // end of section - termination
   {0xffffffff, 0}, 
}; /* dcplbs_table */

#pragma section("cplb_data")

cplb_entry icplbs_table[] = {


   // L1 Instruction SRAM (32KB)
   {0xFFA00000, (PAGE_SIZE_4KB | CPLB_I_PAGE_MGMT)}, 
   {0xFFA01000, (PAGE_SIZE_4KB | CPLB_I_PAGE_MGMT)}, 
   {0xFFA02000, (PAGE_SIZE_4KB | CPLB_I_PAGE_MGMT)}, 
   {0xFFA03000, (PAGE_SIZE_4KB | CPLB_I_PAGE_MGMT)}, 
   {0xFFA04000, (PAGE_SIZE_4KB | CPLB_I_PAGE_MGMT)}, 
   {0xFFA05000, (PAGE_SIZE_4KB | CPLB_I_PAGE_MGMT)}, 
   {0xFFA06000, (PAGE_SIZE_4KB | CPLB_I_PAGE_MGMT)}, 
   {0xFFA07000, (PAGE_SIZE_4KB | CPLB_I_PAGE_MGMT)}, 

   // Boot ROM (4KB)
   {0xEF000000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 

   // L1 Instruction ROM (64KB) 
   {0xFFA10000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA11000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA12000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA13000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA14000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA15000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA16000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA17000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA18000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA19000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA1A000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA1B000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA1C000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA1D000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA1E000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 
   {0xFFA1F000, (PAGE_SIZE_4KB | CPLB_INOCACHE)}, 

   // end of section - termination
   {0xffffffff, 0}, 
}; /* icplbs_table */


#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

