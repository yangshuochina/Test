/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: SchM_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   12.07.2010
 *  LAST REVISION:      $Date: 2010/11/22 09:01:06MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: SchM_Cfg.h  $
 *  Revision 1.3 2010/11/22 09:01:06MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 14:16:43MESZ Cotor Vlad RGB (COTORV) 
 *  - Update the file's header comment
 *
 * ***************************************************************************/

#ifndef SCHM_CFG_H_
#define SCHM_CFG_H_


#include "team-s.h"
#include "Std_Types.h"
#include "osek.h"


/** @defgroup group_schm_config SchM Configuration
 *  @ingroup group_schm
 *
 * This module contains the configuration of the SchM - Basic Software Scheduler.
 */
/*@{*/

/**
 * Enable or disable development error detection.
 * Selection STD_ON, STD_OFF.
 */
#define SCHM_DEV_ERROR_DETECT STD_ON

/* --- nothing to configure here ---*/
#if defined(SCHM_DEV_ERROR_DETECT) && (SCHM_DEV_ERROR_DETECT == STD_ON)
#define SCHM_DETECT_DEVELOPMENT_ERRORS
#endif
/*@}*/

/** @defgroup group_schm_activation_point_config SchM Activation Point Configuration
 *  @ingroup group_schm_config
 *
 * This section contains the activation point configuration of the SchM - Basic Software Scheduler.
 */
/*@{*/
#define SCHM_ADC_ACTIVATION_POINT_1 (0)  /**< activation point of \ref Adc_MainFunction_Handling.*/
#define SCHM_FEE_ACTIVATION_POINT_1 (1)  /**< activation point of \ref Fee_MainFunction.*/
#define SCHM_FLS_ACTIVATION_POINT_1 (2)  /**< activation point of \ref Fls_MainFunction.*/
#define SCHM_SPI_ACTIVATION_POINT_1 (3)  /**< activation point of \ref Spi_MainFunction.*/
#define SCHM_EEP_ACTIVATION_POINT_1 (4)
#define SCHM_EA_ACTIVATION_POINT_1	(5)

/*@}*/


/** @defgroup group_schm_os_config SchM Operating System Configuration
 *  @ingroup group_schm_config
 *
 * This section contains the operating system configuration of the SchM - Basic Software Scheduler.
 */
/*@{*/

#define SCHM_TASK_START_TIME_10MS     (0)
#define SCHM_TASK_CYCLE_TIME_10MS     (10)

#define SchM_Os_TerminateTask() TerminateTask()

#define SchM_Os_Task(taskName) void taskName##func(void)
/* void SCHM_TASK_CYCLE_10MSfunc(void)*/
/*TASK(taskName)*/

#define SchM_Os_SetRelAlarm(task_name, start_time, cycle_time)  SetRelAlarm(task_name, start_time, cycle_time)

/*@}*/


#endif /* SCHM_CFG_H_ */


