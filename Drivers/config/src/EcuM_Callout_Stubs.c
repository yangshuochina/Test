/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: EcuM_Callout_Stubs.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2011/03/04 11:00:24MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: EcuM_Callout_Stubs.c  $
 *  Revision 1.8 2011/03/04 11:00:24MEZ Cotor Vlad RGB (COTORV) 
 *  - Update NvM due to NvM scheduler
 *  Revision 1.7 2011/02/28 14:52:16MEZ Cotor Vlad RGB (COTORV)
 *  - Develop shutdown function for xCU power control
 *  Revision 1.6 2011/01/17 19:12:53MEZ Cotor Vlad RGB (COTORV)
 *  Correct the SW to clean up the LOG files
 *  Revision 1.5 2010/10/08 05:43:55PDT Opel Martin RGB (OPELM)
 *  37040: Implement changes to enable calibration using a two page concept
 *  Revision 1.4 2010/07/30 15:11:01MESZ Opel Martin RGB (OPELM)
 *  - added XCP init (appl.)
 *  Revision 1.3 2010/07/16 14:37:05MESZ Cotor Vlad RGB (COTORV)
 *  Update SchM and EcuM due to ADC driver update
 *  Revision 1.2 2010/07/12 13:59:20MESZ Cotor Vlad RGB (COTORV)
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#include "Std_Types.h"

#include "EcuM.h"
#include "EcuM_Cbk.h"
#include "EcuM_Cfg.h"
#include "Adc.h"
#include "Fee.h"
#include "Gpt.h"
#include "Mcu.h"
#include "NvM.h"
#include "MemIf.h"
#include "Port.h"
#include "Pwm.h"
#include "Spi.h"
#include "Wdg.h"
#include "team-s.h"
#include "Ea.h"
#include "Eep.h"

#include "SchM_Cfg.h"
#include "SchM_Adc.h"
#include "SchM_Fee.h"
#include "SchM_Fls.h"
#include "SchM_Spi.h"
#include "SchM_Ea.h"
#include "SchM_Eep.h"

#include "NvM_Scheduler.h"
#include "Init.h"
#include "os.h"
#include "OsInterface.h"
#include "v_inc.h"
#include "CAN_applCan.h"
#include "XcpProf.h"
#include "xcp_overlay.h"


#define ECUM_START_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

extern const EcuM_ConfigType ecum_cfg;

#define ECUM_STOP_SEC_CONFIG_DATA_32BIT
#include "MemMap.h"

#define ECUM_START_SEC_VAR_8BIT
#include "MemMap.h"
uint8 NvM_Eraseneeded = 0;
#define ECUM_STOP_SEC_VAR_8BIT
#include "MemMap.h"

#define ECUM_START_SEC_VAR_32BIT
#include "MemMap.h"

/**
 * The selected EcuM post-build configuration which contains pointer
 * to all other BSW module post build configurations.
 */
static EcuM_ConfigType ecum_selected_pb_cfg;

#define ECUM_STOP_SEC_VAR_32BIT
#include "MemMap.h"

/*@{*/


#define ECUM_START_SEC_CODE
#include "MemMap.h"

void EcuM_ErrorHook(Std_ReturnType reason) {
   switch(reason) {
      case ECUM_E_CONFIGURATION_DATA_INCONSISTENT:
         ECUM_ASSERT_RETVOID(1, ECUM_INIT_API_ID, reason);
         break;
      default:
         /**< @todo Map not known errors to an api. */
         break;
   }
   /* TODO: The function \ref EcuM_ErrorHook stays in an endless loop if called. */
   while(1);
}

void EcuM_AL_DriverInitZero(void) {
}

EcuM_ConfigType* EcuM_DeterminePbConfiguration(void) {
   ecum_selected_pb_cfg = ecum_cfg;
   return &ecum_selected_pb_cfg;
}

void EcuM_AL_DriverInitOne(const EcuM_ConfigType* configPtr) {

   /* provide a proper clock */
   Mcu_Init(configPtr->bsw_driver.init_one.mcu_cfg);
   Mcu_InitClock(0);
   Mcu_DistributePllClock();

   /* -----------------------------------------------------------------------
	      Interrupt System:
	      -----------------------------------------------------------------------
	      - four arbitration cycles (max. 255 interrupt sources)
	      - two clocks per arbitration cycle */

   __mtcr(0xFE2C,  0x00000000);     /* load CPU interrupt control register */
   __isync();

   /* -----------------------------------------------------------------------
	      Peripheral Control Processor (PCP):
	      -----------------------------------------------------------------------
	      - the PCP internal clock is always running

	      - use Full Context save area (R[0] - R[7])
	      - start progam counter as left by last invocation
	      - channel watchdog is disabled
	      - maximum channel number checking is disabled */

   /* - four arbitration cycles (max. 255 PCP channels) */
   /* - two clocks per arbitration cycle */
   PCP_ICR.U        =  0x00000000;  /* load PCP interrupt control register */

   /* - the PCP warning mechanism is disabled */
   PCP_ITR.U        =  0x00000000;  /* load PCP interrupt threshold register */

   /* - type of service of PCP node 4 is CPU interrupt */
   PCP_SRC4.U       =  0x00001000;  /* load service request control register 4 */

   /* - type of service of PCP node 5 is CPU interrupt */
   PCP_SRC5.U       =  0x00001000;  /* load service request control register 5 */

   /* - type of service of PCP node 6 is CPU interrupt */
   PCP_SRC6.U       =  0x00001000;  /* load service request control register 6 */

   /* - type of service of PCP node 7 is CPU interrupt */
   PCP_SRC7.U       =  0x00001000;  /* load service request control register 7 */

   /* - type of service of PCP node 8 is CPU interrupt */
   PCP_SRC8.U       =  0x00001000;  /* load service request control register 8 */


   ts_initGPTAInt();

   Port_Init(configPtr->bsw_driver.init_one.port_cfg);

   Adc_Init(configPtr->bsw_driver.init_one.adc_cfg);
   Fls_Init(configPtr->bsw_driver.init_one.fls_cfg);
   Gpt_Init(configPtr->bsw_driver.init_one.gpt_cfg);
   Pwm_Init(configPtr->bsw_driver.init_one.pwm_cfg);
   Spi_Init(configPtr->bsw_driver.init_one.spi_cfg);
   Wdg_Init(configPtr->bsw_driver.init_one.wdg_cfg);
#ifdef ECUM_WDGM_INCLUDED
   WdgM_Init(configPtr->bsw_driver.init_one.wdgm_cfg);
#endif

   /* setup end of init protected registers for OS */
   ts_endinit_clear();
   osInitProtected();
   ts_endinit_set();

   /* Overlay Ram:
    * Init registers and mem areas for switching from
    * working page (overlay ram) <-> reference page (flash)
    */
   RAM_OverlayRamReset();

   /* - the CPU interrupt system is globally disabled */
   __enable();


   Spi_SetAsyncMode(SPI_INTERRUPT_MODE);
   Adc_StartGroupConversion(0);
   Adc_StartGroupConversion(1);
   EcuM_SelectApplicationMode(OSDEFAULTAPPMODE);

}

void EcuM_AL_DriverInitTwo(const EcuM_ConfigType* configPtr) {
#if defined(ADC_POLLING_MODE) && (ADC_POLLING_MODE == STD_ON)
   Adc_SetPollingMode();
   SchM_ActMainFunction_Adc(SCHM_ADC_ACTIVATION_POINT_1);
#endif
   SchM_ActMainFunction_Fls(SCHM_FLS_ACTIVATION_POINT_1);
   SchM_ActMainFunction_Spi(SCHM_SPI_ACTIVATION_POINT_1);
   SchM_ActMainFunction_Ea(  SCHM_EA_ACTIVATION_POINT_1);
   SchM_ActMainFunction_Eep(SCHM_EEP_ACTIVATION_POINT_1);

#ifdef ECUM_NVMRAM_MGR_INCLUDED
   /* NvRAM related initialize */
   Ea_Init();
   Eep_Init();
   NvM_Init();
#endif

   /* CAN Part */
   ApplCanInitPowerOn();
   XcpInit();
   ApplXcpInit();
}

void EcuM_AL_DriverInitThree(const EcuM_ConfigType* configPtr) {

   /* initialize BSW interface and ASW */
   init_rst();

   EcuM_RequestRUN(ECUM_USER_ID_0);
}

void EcuM_OnRTEStartup(void){

}

void EcuM_OnEnterRun(void) {
   // EcuM_RequestRUN(0);
}

void EcuM_OnExitRun(void) {

}

void EcuM_OnExitPostRun(void) {

}

void EcuM_OnPrepShutdown(void) {

}

void EcuM_OnGoSleep(void) {

}

void EcuM_OnGoOffOne(void) {

}

void EcuM_OnGoOffTwo(void) {

}

void EcuM_EnableWakeupSources(EcuM_WakeupSourceType wakeupSource){

}

void EcuM_GenerateRamHash(void) {

}

void EcuM_AL_SwitchOff(void) {

}

uint8 EcuM_CheckRamHash(void) {
   return 1;
}

void EcuM_DisableWakeupSources(EcuM_WakeupSourceType wakeupSource) {

}

void EcuM_AL_DriverRestart(void) {

}

void EcuM_StartWakeupSources(EcuM_WakeupSourceType wakeupSource) {

}

void EcuM_CheckValidation(EcuM_WakeupSourceType wakeupSource) {

}

void EcuM_StopWakeupSources(EcuM_WakeupSourceType wakeupSource) {

}

EcuM_WakeupReactionType EcuM_OnWakeupReaction(EcuM_WakeupReactionType wact) {
   return ECUM_WKACT_RUN;
}

void EcuM_SleepActivity(void) {

}

void EcuM_CheckWakeup(EcuM_WakeupSourceType wakeupSource) {

}


#define ECUM_STOP_SEC_CODE
#include "MemMap.h"

/*@}*/
