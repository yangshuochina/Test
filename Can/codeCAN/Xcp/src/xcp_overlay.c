/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: xcp_overlay.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   08.10.2010
 *  LAST REVISION:      $Date: 2010/10/08 16:02:49MESZ $
 *                      $Author: Opel Martin RGB (OPELM) $
 *
 * ***************************************************************************
 *
 *  $Log: xcp_overlay.c  $
 *  Revision 1.1 2010/10/08 16:02:49MESZ Opel Martin RGB (OPELM) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Can/codeCAN/Xcp/src/project.pj
 *
 * ***************************************************************************/


/* ========== Includes ===================================================== */
#include "Std_Types.h"
#include "regtc1767.sfr"
#include "xcp_overlay.h"           /* module header file     */



/* ========== Enumerations, Type Definitions, Local Defines ================ */



/* ========== Local Function Prototypes ==================================== */



/* ========== Variables ============================================== */
#define XCP_START_SEC_LINKER_LABEL
#include "MemMap.h"
extern uint32 _lc_us_cal_rom;       /* size of cal_rom */
extern uint32 _lc_ub_cal_rom;
extern uint32 _lc_ub_cal_ram;
#define XCP_STOP_SEC_LINKER_LABEL
#include "MemMap.h"



/* ========== Local Macros ================================================= */



/* ========== Local Functions ============================================== */



/* ========== Global Functions ============================================= */
/******************************************************************************/
/*!
 * @fn void RAM_OverlayRamReset(void)
 *
 * @brief This function enables the Tricore Overlay RAM at power on reset.
 *
 * @details This function enables the Tricore Overlay RAM for can XCP working
 *          page access. The complete 8k Overlay RAM contains calibration data
 *          (reference page), which is copied to the overlay ram (working page).
 *          Then the registers are prepared to enable/disable the redirection
 *          from flash (ref. page) to overlay ram (working page).
 *          The 8k Overlay RAM is partitioned to four 2k slices, because the
 *          Tricore overlay feature supports a maximum block size of 2k, if
 *          redirected to the Overlay RAM. If redirection target is ED-RAM, more
 *          than 2k are supported.
 *
 * @param none
 *
 * @return void
 */
/*****************************************************************************/
#define XCP_START_SEC_CODE_INIT
#include "MemMap.h"
void RAM_OverlayRamReset(void)
{
   uint32_least i;
   uint32 size;
   uint32 *target;
   uint32 *source;
   uint32 offset;
   const uint32 OTAR_MASK  = 0x0ffffff0;
   const uint32 BLOCK_SIZE = 0x0ffff800;
   const uint32 RABR_MASK  = 0x0fff1ff0;

   /* assign linker labels to local pointers (better debugging) */
   size = ((uint32)(&_lc_us_cal_rom)) / 4;
   source = &_lc_ub_cal_rom;
   target = &_lc_ub_cal_ram;

   /* copy calibration data from flash (reference page) */
   /* to ram mirror (working page)                      */
   for(i=0; i<size; i++)
   {
      *(target+i) = *(source+i);
   }

   /* ==================== 1st 2k calrom area 8010.0000...8010.0800 ================== */
   offset = 0x00000000;
   OVC_OTAR0.U  = ((uint32)(&_lc_ub_cal_rom) + offset) & OTAR_MASK;
   OVC_OMASK0.U = BLOCK_SIZE;
   OVC_RABR0.U  = ((uint32)(&_lc_ub_cal_ram) + offset) & RABR_MASK;

   /* ==================== 2nd 2k calrom area 8010.0800...8010.1000 ================== */
   offset += 0x0800;
   OVC_OTAR1.U  = ((uint32)(&_lc_ub_cal_rom) + offset) & OTAR_MASK;
   OVC_OMASK1.U = BLOCK_SIZE;
   OVC_RABR1.U  = ((uint32)(&_lc_ub_cal_ram) + offset) & RABR_MASK;

   /* ==================== 3rd 2k calrom area 8010.1000...8010.1800 ================== */
   offset += 0x0800;
   OVC_OTAR2.U  = ((uint32)(&_lc_ub_cal_rom) + offset) & OTAR_MASK;
   OVC_OMASK2.U = BLOCK_SIZE;
   OVC_RABR2.U  = ((uint32)(&_lc_ub_cal_ram) + offset) & RABR_MASK;

   /* ==================== 4th 2k calrom area 8010.1800...8010.2000 ================== */
   offset += 0x0800;
   OVC_OTAR3.U  = ((uint32)(&_lc_ub_cal_rom) + offset) & OTAR_MASK;
   OVC_OMASK3.U = BLOCK_SIZE;
   OVC_RABR3.U  = ((uint32)(&_lc_ub_cal_ram) + offset) & RABR_MASK;
}
#define XCP_STOP_SEC_CODE_INIT
#include "MemMap.h"



/******************************************************************************/
/*!
 * @fn void RAM_OverlayRamEnable(boolean enable)
 *
 * @brief This function enables/disables the redirection from flash to ram.
 *
 * @details This function enables/disables the recirection from flash to ram
 *          via Tricore Overlay RAM feature.
 *
 * @param enable Enables or disables the redirection from flash to ram.
 *
 * @return void
 */
#define XCP_START_SEC_CODE
#include "MemMap.h"
void RAM_OverlayRamEnable(boolean enable)
{
   if (enable)
   {
      /* enable redirection to OVRAM */
      OVC_RABR0.U |= (1 << 31);
      OVC_RABR1.U |= (1 << 31);
      OVC_RABR2.U |= (1 << 31);
      OVC_RABR3.U |= (1 << 31);

   }
   else
   {
      OVC_RABR0.U &= ~(1 << 31);
      OVC_RABR1.U &= ~(1 << 31);
      OVC_RABR2.U &= ~(1 << 31);
      OVC_RABR3.U &= ~(1 << 31);
   }
}
#define XCP_STOP_SEC_CODE
#include "MemMap.h"
