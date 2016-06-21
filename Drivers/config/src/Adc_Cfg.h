/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Adc_Cfg.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/07/12 09:42:35MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Adc_Cfg.h  $
 *  Revision 1.2 2010/07/12 09:42:35MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/20 11:28:59MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /Controls/29015/ARE9018/Bsw/Drivers/config/src/project.pj
 *
 * ***************************************************************************/

#ifndef ADC_CFG_H_
#define ADC_CFG_H_

#include "team-s.h"

#define ADC_INSTANCE_ID 				(0)
#define ADC_DEV_ERROR_DETECT			(STD_ON)
#define ADC_NUM_OF_GROUPS_CONFIGURED    (10)

/* This factor(ADC_PRESCALAR_VALUE) will divide the SYS Freq, Make sure the ADC Freq shall be with 500KHz to 10MHz*/
#define ADC_PRESCALAR_VALUE         	(10u)   /**< Setup of prescaler for f_ADCI; see description of GLOBCTR.DIVA in TC1767 Manual. */

/* Only DeInit and Notification API make sense to put it OFF,
 * Because START/STOP conversion and Read Group are the only API to do conversion and get results */
#define ADC_DEINIT_API                  STD_ON	/**< Enable/Disable the availability of Service Adc_DeInit().  */
#define ADC_ENABLE_START_STOP_GROUP_API STD_ON  /**< Enable/Disable the availability of Service Adc_Start()/Stop_GroupConversion().  */
#define ADC_GRP_NOTIF_CAPABILITY        STD_OFF /**< Enable/Disable the availability of Service Adc_Enable()/Disable_Notification().  */
#define ADC_READ_GROUP_API              STD_ON  /**< Enable/Disable the availability of Service Adc_ReadGroup().  */


/**
 * This configuration parameter defines whether the polling mode
 * is supported by the Adc.
 *
 * @li \ref STD_OFF disables polling mode.
 * @li \ref STD_ON enables polling mode.
 *
 */
#define ADC_USE_POLLING_MODE (STD_OFF)

/* --- nothing to configure here ---*/
#if defined(ADC_USE_POLLING_MODE) && (ADC_USE_POLLING_MODE == STD_ON)
#define ADC_POLLING_MODE
#endif


#endif /* ADC_CFG_H_ */


