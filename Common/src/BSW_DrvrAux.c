/* ***************************************************************************
 *
 *  COPYRIGHT:          AVL Software and Functions GmbH
 *  FILENAME:           $Source: BSW_DrvrAux.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   13.04.2010
 *  LAST REVISION:      $Date: 2010/12/08 15:43:40MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: BSW_DrvrAux.c  $
 *  Revision 1.10 2010/12/08 15:43:40MEZ Cotor Vlad RGB (COTORV) 
 *  - Correction of daisy chain SPI communication of TLE8104E chip
 *  Revision 1.9 2010/12/01 16:32:12MEZ Cotor Vlad RGB (COTORV) 
 *  Implement and integrate complex device driver for electrical hazard protection
 *  Revision 1.8 2010/11/22 07:52:29MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.7 2010/11/16 13:10:30MEZ Cotor Vlad RGB (COTORV) 
 *  Rename all initialization function from <module>_RstInit to <module>_Ini due to initialization reasons (not only by reset)
 *  Revision 1.6 2010/10/14 18:06:11MESZ Cotor Vlad RGB (COTORV) 
 *  Update complex device driver for real time clock due to changed SPI configuration.
 *  Revision 1.5 2010/10/07 13:50:06MESZ Cotor Vlad RGB (COTORV) 
 *  Update SPI sequence's names due to integration of complex device driver for real time clock
 *  Revision 1.4 2010/09/24 18:02:28MESZ Cotor Vlad RGB (COTORV) 
 *  Reorganize the SPI Configuration switching to macros
 *  Revision 1.3 2010/09/09 18:31:17MESZ Cotor Vlad RGB (COTORV)
 *  Update of BSW_Aux due to erroneous behavior of PWM IN function (InPwmClcd) - period limitation
 *  Revision 1.2 2010/08/27 14:20:45MESZ Cotor Vlad RGB (COTORV)
 *  Check and update all BSW modules due to initialization and the scheduling functions
 *  Update BSW modules due to:
 *  - update Init.c - Update the initialization functions
 *  - update OsTask.c - Update the order of existing scheduling functions and add the new ones.
 *  Revision 1.1 2010/08/25 16:44:40MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Common/src/project.pj
 *  Revision 1.6 2010/08/25 14:40:52MESZ Cotor Vlad RGB (COTORV)
 *  Implement and integrate complex device driver for cooling fan control
 *  Revision 1.5 2010/08/24 10:19:42MESZ Cotor Vlad RGB (COTORV)
 *  Update BSW for xCU power control complex device driver
 *  Update due to calculation correction, resolution adjustment
 *  Revision 1.4 2010/07/30 14:27:26MESZ Cotor Vlad RGB (COTORV)
 *  Update of BSW_Aux due to erroneous behavior of PWM IN function (InPwmClcd)
 *  Revision 1.3 2010/07/20 17:55:38MESZ Cotor Vlad RGB (COTORV)
 *  Update of PT1 filter calculation due to erroneous behaviour
 *  Revision 1.2 2010/07/15 12:36:14MESZ Cotor Vlad RGB (COTORV)
 *  Update auxiliary SW functions and structure, include structure and clean the log files
 *  Revision 1.1 2010/05/20 13:55:32MESZ Cotor Vlad RGB (COTORV)
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Common/src/project.pj
 *
 * ***************************************************************************/

#define  MASTER_BSW_DRVRAUX_C_              /* Master define for this module    */
#define  MLIB_INLINE  0

/* ========== Includes ===================================================== */
#include "Mathlib.h"

#include "BSW_DrvrAux.h"           /* module header file     */

/* ========== Enumerations, Type Definitions, Local Defines ================ */



/* ========== Local Function Prototypes ==================================== */



/* ========== Local Variables ============================================== */



/* ========== Local Macros ================================================= */



/* ========== Local Functions ============================================== */

#define BSWAUX_START_SEC_CODE_1MS
#include "MemMap.h"
void InPwmClcd(Struct_InPwm *pwm_data)
{
   /* get pin values */
   boolean status_pin = Dio_ReadChannel(pwm_data->PwmInPin);

   /* count time */
   pwm_data->counter_time++;

   if (status_pin == 1)
   {
      pwm_data->counter_high++;
   }
   else
   {
      /* status_pin is definitively zero here */
      /* edge from 1 -> 0 is end of PWM, let's calculate all values */
      if (pwm_data->status_pin_old != status_pin)
      {
         pwm_data->InDuty = (uint16)(((uint32)pwm_data->counter_high << 16) / pwm_data->counter_time);
         pwm_data->InPerd = MLIB_Uint16MulUint16Uint16(pwm_data->counter_time, (uint16)100);
         pwm_data->counter_high = 0;
         pwm_data->counter_time = 0;
      }
   }
   pwm_data->status_pin_old = status_pin;
}
#define BSWAUX_STOP_SEC_CODE_1MS
#include "MemMap.h"
/* ========== END InPwmClcd ================================================ */


#define BSWAUX_START_SEC_CODE_1MS
#include "MemMap.h"
uint8 returnCfgGain(uint8 CfgGain)
{
   uint8 temp_CfgGain;
   switch(CfgGain)
   {
      case 1u:
         temp_CfgGain = 0u;
         break;
      case 5u:        /* 0: Gain =   5 */
         temp_CfgGain = 0u;
         break;
      case 25u:       /* 1: Gain =  25 */
         temp_CfgGain = 1u;
         break;
      case 40u:       /* 2: Gain =  40 */
         temp_CfgGain = 2u;
         break;
      case 100u:      /* 3: Gain = 100 */
         temp_CfgGain = 3u;
         break;
      default:       /* default value = Gain 5 */
         temp_CfgGain = 0u;
         break;
   }
   return temp_CfgGain;
}
#define BSWAUX_STOP_SEC_CODE_1MS
#include "MemMap.h"
/* ========== END returnCfgGain ============================================ */


#define BSWAUX_START_SEC_CODE_10MS
#include "MemMap.h"
void ClcdPT1Fil(Struct_PT1Fil *PT1Fil_data)
{
   if (PT1Fil_data->factor == 0xFFFF)
   {
      PT1Fil_data->v_ADC_out = PT1Fil_data->v_ADC_meas << 4;
   }
   else
   {
      PT1Fil_data->v_ADC_out = (uint16)(
                                        ((sint32)PT1Fil_data->v_ADC_out_old) +
                                        ((((PT1Fil_data->v_ADC_meas << 4) - PT1Fil_data->v_ADC_out_old) * PT1Fil_data->factor) >> 16)
                                       );
   }
   PT1Fil_data->v_ADC_out_old = PT1Fil_data->v_ADC_out;
}
#define BSWAUX_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END ClcdPT1Fil =============================================== */


#define BSWAUX_START_SEC_CODE_10MS
#include "MemMap.h"
uint8 returnPrmErrSym(uint8 CfgPrmErrSym)
{
   uint8 temp_CfgPrmErrSym = 0;
   switch(CfgPrmErrSym)
   {
      case 0:                       /* spi:  0x0 - short circuit to ground */
         temp_CfgPrmErrSym = 0x1;   /* diag: 0x1 - short circuit to ground */
         break;
      case 1:                       /* spi:  0x1 - open load */
         temp_CfgPrmErrSym = 0x2;   /* diag: 0x2 - open load */
         break;
      case 2:                       /* spi:  0x2 - over load / over temperature */
         temp_CfgPrmErrSym = 0x3;   /* diag: 0x3 - over load / over temperature */
         break;
      case 3:                       /* spi:  0x3 - normal operation / no error */
         temp_CfgPrmErrSym = 0x0;   /* diag: 0x0 - normal operation / no error */
         break;
      default :
         temp_CfgPrmErrSym = 0x0;   /* diag: 0x0 - normal operation / no error */
         break;
   }
   return temp_CfgPrmErrSym;
}
#define BSWAUX_STOP_SEC_CODE_10MS
#include "MemMap.h"
/* ========== END returnPrmErrSym ========================================== */


/* ========== Undefine ===================================================== */

/* undefine MLIB_INLINE */
#ifdef MLIB_INLINE
#undef MLIB_INLINE
#endif

/* undefine master define for this module    */
#ifdef MASTER_BSW_DRVRAUX_C_
#undef MASTER_BSW_DRVRAUX_C_
#endif

