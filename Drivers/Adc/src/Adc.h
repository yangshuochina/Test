/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Adc.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 12:58:43MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: Adc.h  $
 *  Revision 1.5 2011/03/22 12:58:43MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.4 2010/12/10 16:20:08MEZ Cotor Vlad RGB (COTORV) 
 *  Update of ADC Driver due to rough resolution
 *  Revision 1.3 2010/11/22 07:54:31MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.2 2010/07/12 09:41:53MESZ Cotor Vlad RGB (COTORV) 
 *  Integration of EcuM, Fee, NvM, RamTst
 *  Update of memory mapping due to EcuM, Fee, NvM, RamTst,
 *  Upfate of ARE9018.lsl file due to EcuM
 *  Revision 1.1 2010/05/19 14:14:52MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Adc/AA000/src/project.pj
 *
 * ***************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "team-s.h"
#include "Std_Types.h"
#include "Adc_Cfg.h"

/** @defgroup group_adc Adc
 *  @ingroup group_adc */
/*@{*/

#define ADC_MODULE_ID         (123)
#define ADC_VENDOR_ID         (TEAMS_VENDOR_ID)
#define ADC_AR_MAJOR_VERSION  (2)
#define ADC_AR_MINOR_VERSION  (2)
#define ADC_AR_PATCH_VERSION  (2)
#define ADC_SW_MAJOR_VERSION  (1)
#define ADC_SW_MINOR_VERSION  (0)
#define ADC_SW_PATCH_VERSION  (0)




/* Driver APIs Service ID */
#define ADC_INIT_API_ID             		((uint8)(0))
#define ADC_DEINIT_API_ID             		((uint8)(1))
#define ADC_STARTGROUPCONVERSION_API_ID 	((uint8)(2))
#define ADC_STOPGROUPCONVERSION_API_ID 		((uint8)(3))
#define ADC_READGROUP_API_ID            	((uint8)(4))
#define ADC_ENABLEGROUPNOTIFICATION_API_ID  ((uint8)(7))
#define ADC_DISABLEGROUPNOTIFICATION_API_ID ((uint8)(8))
#define ADC_GETGROUPSTATUS_API_ID	        ((uint8)(9))
#define ADC_GETVERSIONINFO_API_ID			((uint8)(10))
#define ADC_SETUPRESULTBUFFER_API_ID   		((uint8)(12))


/* Development Error Codes */

#define ADC_E_UNINIT				((uint8)(0x0A))	/**< A service was called without a prior usage of Adc_Init(). */
#define ADC_E_BUSY			 		((uint8)(0x0B))	/**< Adc_StartGroupConversion was called while another conversion is already running.  */
#define ADC_E_IDLE 					((uint8)(0x0C))	/**< Adc_StopGroupConversion was called while no conversion was running.   */
#define ADC_E_ALREADY_INITIALIZED	((uint8)(0x0D))	/**< Adc_Init was called while it is already initailzied  */
#define ADC_E_PARAM_CONFIG  		((uint8)(0x0E))	/**< Adc_Init was called with incorrect configuration pointer. */
#define ADC_E_PARAM_GROUP 			((uint8)(0x15))	/**< A service was called with invalid group ID.  */
#define ADC_E_WRONG_CONV_MODE 		((uint8)(0x16))	/**< This error cant occur because HW Trigger related interfaces are not implemented  */
#define ADC_E_WRONG_TRIGG_SRC 		((uint8)(0x17))	/**< This error cant occur because HW Trigger related trigger sources are not implemented  */
#define ADC_E_NOTIF_CAPABILITY 		((uint8)(0x18))	/**< Enable/Disable notification function for a group whose configuration set has no notification available.  */
#define ADC_E_BUFFER_UNINIT 		((uint8)(0x19))	/**< Conversion started and result buffer pointer is not initialized.  */

/* Macros for ADC Result Resolution */
#define ADC_RESOLUTION_8BIT			(2u)
#define ADC_RESOLUTION_10BIT		(0u)
#define ADC_RESOLUTION_12BIT		(1u)

/** @page ADC Adc
 *
 * @section ADC_intro Introduction
 * The Adc function implements the Analog to Digital conversion functionality as specified by
 * <a href="../external/autosar/AUTOSAR_SWS_ADC_Driver.pdf">AUTOSAR_SWS_ADC_Driver</a>.
 * The Adc driver currently supports only the Software trigger based OneShot and Continuous conversion modes with
 * single stream access mode. Now onwards OneShot mode will be referred as Software Trigger conversion and
 * Continuous mode will be referred as Continuous trigger conversion, even though both are realized using Software
 * trigger conversion.
 *
 *
 * @subsection adc_nomen Nomenclature
 * @subsubsection adc_names Groups, Nodes, ...
 * The Adc driver uses the names Groups, Nodes and Channels. The following
 * shall ease the usage of them.
 *
 * @subsubsection adc_group_id Group
 *
 * A Group represents of set of analog channels to be converted through the specified HW node with the
 * configured conversion mode. In Each group its corresponding analog channels are represented in bitwise manner.
 * The position of the bit stands for the corresponding analog channel. Maximum of 16 channels only can be
 * configured in one group as each node has physically only 16 channel.
 *
 *
 * @subsubsection adc_node_id Node
 *
 * The Node represents the HW ADC Kernel. Each Group must be associated with one Kernel.
 * The operating frequency of both kernel is same. In each node only one continuous conversion is supported.
 * But maximum of 16 Software trigger conversion is supported.
 *
 * @subsubsection adc_channel_id Channel
 *
 * The Channel represents the physical Analog channel. In TC1767 we have 32 analog channel,
 * 16 in each node. Each channel can have different conversion profiles (one out of four) and each one of
 * them can be configured either in Software trigger group or in Continuous trigger group but it must be
 * configured only in one group.
 *
 * @section ADC_init Initialization of Adc
 * Before any service of the Adc can be used the initialization function Adc_Init() with a pointer
 * proper configuration set (#Adc_ConfigType) must be called.
 *
 *
 * @section ADC_autosar AutoSAR Conformance
 * The following limitations apply
 * @li HW trigger functionality is not implemented.
 * @li Priorities are fixed in such a way that SW trigger has higher priority than Continuous.
 * 	   So no configurable parameter needed for Priority.
 * @li Analog channel/pins must be not shared due to the usage of DMA
 * @li Following parameters given in the Adc Configuration are not possible -
 * 	   AdcChannel Conversion time, Streaming Mode, Reference VoltageSource Low
 * @li The Frequency of the AD Converter is fixed(set to MAX) via the ADC_CFG.H and not through user configuration
 *
 *
 * @section adc_operation Operation
 * The two AD converter kernels are used to convert maximum of 32 analog channels. Both kernel
 * Supports both Continuous and SW trigger conversion modes. Overall 8 SW triggers and 2
 * Continuous groups are possible (1 in each node). DMA is used to copy the results from the result register to
 * the corresponding group result buffer. Due to the restriction in the DMA, each of the group's
 * channel results are moved to the group's last channel result register and DMA will read the
 * result from the same result register but copy to corresponding ram location. Due to this
 * Strategy no channels must be shared. For each conversion source one DMA channel is dedicated.
 * Software trigger conversion uses DMA 0 and 2 for node 0 and 1. Continuous trigger conversion
 * uses DMA 1 and 3 for node 0 and 1.
 *
 * @subsection adc_dma_connection ADC<->DMA Connection
 *
 * @image html external_images/dma_connection.pdf "DMA Connection" width=14cm
 * @image html external_images/adc_group.pdf "ADC Group" width=14cm
 * @image latex external_images/dma_connection.pdf "DMA Connection" width=14cm
 * @image latex external_images/adc_group.pdf "ADC Group" width=14cm
 *
 * @section ADC_cfg Configuration
 * @subsection ADC_cfg_step_by_step Step by Step
 * @li Configure #Adc_GroupDefType.
 * @li Configure #Adc_NodeType.
 * @li Configure #Adc_ChannelProfileType.
 * @li Configure #Adc_ConfigType.
 * @li Call the function Adc_Init() with the pointer to the configured structure #Adc_ConfigType
 *     in the startup phase.
 *
 *
 * @subsection ADC_cfg_by_example Example
 * Configuration of #Adc_GroupDefType:
 * @code
 * const Adc_GroupDefType AdcChannelGroup[] = {
 * {                Group-0
 *   0x5A00,                    Analog Channel Set belongs to Group_0 (Adc_GroupDefType::AdcChannelSet)
 *   ADC_NODE_0,                Adc Node: 0 or 1(Adc_GroupDefType::AdcHwNodeId)
 *   ADC_CONV_MODE_ONESHOT,     Adc Convesion trigger type(Adc_GroupDefType::AdcConversionMode)
 *   DMA_CHANNEL_0,             DMA channel to be used for Moving result(Adc_GroupDefType::DmaChannel)
 *   &IoHwAb_Adc_Notification,  Adc_GroupDefType::notification
 *   AdcGroup0_Res,             Pointer to the result buffer(Adc_GroupDefType::AdcResultBufferPointer)
 * },
 * {                Group-1
 *   0x5F0,                     Analog Channel Set belongs to Group_1 (Adc_GroupDefType::AdcChannelSet)
 *   ADC_NODE_1,                Adc Node: 0 or 1 (Adc_GroupDefType::AdcHwNodeId)
 *   ADC_CONV_MODE_CONTINUOUS,  Adc Convesion trigger type(Adc_GroupDefType::AdcConversionMode)
 *   DMA_CHANNEL_3,             DMA channel to be used for Moving result(Adc_GroupDefType::DmaChannel)
 *   &IoHwAb_Adc_Notification,  Adc_GroupDefType::notification
 *   AdcGroup1_Res,             Pointer to the result buffer(Adc_GroupDefType::AdcResultBufferPointer)
 * };
 * @endcode
 *
 *  Configuration of #Adc_NodeType:
 *  For each used (required) node configuration must be done.
 * @code
 * const Adc_NodeType AdcNode[]= {
 * {                Node-0
 *   ADC_NODE_0,                                                Adc Node: 0 or 1 (Adc_NodeType::AdcHwNodeId)
 *   sizeof(AdcChannelProfile)/sizeof(AdcChannelProfile[0]),	Number of Profiles in Node0(Adc_NodeType::number_of_profile)
 *   {0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3},                         Profile array values to be written to each analog channel register(Adc_NodeType::AdcProfileIndex)
 *   &AdcChannelProfile[0]                                      Pointer to profile configuraiton structure (Adc_NodeType::AdcChannelProfileDefinition)
 * },
 * {                Node-1
 *   ADC_NODE_1,                                                Adc Node: 0 or 1 (Adc_NodeType::AdcHwNodeId)
 *   sizeof(AdcChannelProfile)/sizeof(AdcChannelProfile[0]),    Number of Profiles in Node 1(Adc_NodeType::number_of_profile)
 *   {0,1,1,1,0,1,2,1,1,1,2,3,0,1,2,3},                         Profile array values to be written to each analog channel register(Adc_NodeType::AdcProfileIndex)
 *   &AdcChannelProfile[0]	                                    Pointer to profile configuraiton structure (Adc_NodeType::AdcChannelProfileDefinition)
 * };
 * @endcode
 *
 *
 *  Configuration of #Adc_ChannelProfileType:
 *  Two different profile structure can be created one for each node. Each Node can have 4 different profiles,
 *  i.e. analog channel must have one among the four profiles
 * @code
 * const Adc_ChannelProfileType AdcChannelProfile[]=
 * {Profile-0
 *     0, ADC_RESOLUTION_12BIT, Sample Time and Resolution for analog channel (Adc_ChannelProfileType)
 *     2, ADC_RESOLUTION_10BIT,
 *    24, ADC_RESOLUTION_8BIT,
 *    80, ADC_RESOLUTION_12BIT,
 * };
 *
 * @endcode
 *
 *  Configuration of #Adc_ConfigType:
 *  Main driver configuration passed the pointer of this structure as parameter to ADC Initialization function.
 * @code
 * const Adc_ConfigType adc_config[] =
 * {
 *		sizeof(AdcChannelGroup)/sizeof(AdcChannelGroup[0]), Number of Groups(Adc_ConfigType::number_of_groups)
 *		sizeof(AdcNode)/sizeof(AdcNode[0]),					Number of Nodes(Adc_ConfigType::number_of_nodes)
 *		AdcChannelGroup,									Pointer to GroupStructure(Adc_ConfigType::AdcGroupDefinition)
 *		AdcNode												Pointer to NodeStructure(Adc_ConfigType::AdcNodeDefinition)
 * };
 * @endcode
 *
 * Configuration of Event Trigger:
 *  The Events can be trigger by Hardware using Interrupt or by Polling method.
 *  If it is by Interrupt mechanism then take care about configuring the priority of the interrupt sources.
 *  If it is by Polling method, place the polling function "Adc_MainFunction_Handling" in a least possible timer task
 *
 * Usage of the above:
 * @code
 * void init(void)
 * {
 *   ...
 *   Adc_Init(&ADC_cfg);
 *   Adc_SetAsyncMode();  Only necessary if interrupt mechanism is supported
 *   ...
 * }
 * @endcode
 *
 */

/* @dot
 *
 *   digraph dma_connection {
 *     node [shape=record fontsize="10.0"];
 *     rankdir=LR;
 *     AdcGroup [label="<grp0>GROUP0\
 *     |<grp1>GROUP1\
 *     |<grp2>...\
 *     |<grp3>GROUPn"];
 *     Group -> AdcGroup [fontsize="8.0"];
 *     AdcGroup:grp2 -> ADCHW[label="conversion started" fontsize="8.0"]
 *     ADCHW -> DMA[label="conversion finished" fontsize="8.0"];
 *
 *     AdcGroupRAM [label="<grp_res0>GROUP_RES_BUFFER_0\
 *     |<grp_res1>GROUP_RES_BUFFER_1\
 *     |<grp_res2>...\
 *     |<grp_res3>GROUP_RES_BUFFER_n"];
 *     DMA -> AdcGroupRAM[label="Store Results" fontsize="8.0"];
 *   }
 * @enddot
 *
 * @dot
 *   digraph adc_group {
 *     node [shape=record fontsize="10.0"];
 *     rankdir=LR;
 *     channel [label="<ch0>AN Channel-0\
 * |<ch1>AN Channel-1\
 * |<ch2>AN Channel-2\
 * |<ch3>AN Channel-3\
 * |<ch4>AN Channel-4\
 * |<ch5>AN Channel-5\
 * |<ch6>AN Channel-6\
 * |<ch7>AN Channel-7\
 * |<ch8>AN Channel-8\
 * |<ch9>AN Channel-9\
 * |<ch10>AN Channel-10\
 * |<ch11>AN Channel-11\
 * |<ch12>AN Channel-12\
 * |<ch13>AN Channel-13\
 * |<ch14>AN Channel-14\
 * |<ch15>AN Channel-15\
 * "];
 *     node [shape=plaintext fontsize="10.0"];
 *     covered [label="ADC HW"]
 *     covered -> channel:ch0;
 *     covered -> channel:ch3;
 *     covered -> channel:ch5;
 *     covered -> channel:ch7;
 *     covered -> channel:ch9;
 *     covered -> channel:ch11;
 *     covered -> channel:ch13;
 *     covered -> channel:ch15;
 *  node [shape="box" fontsize="10.0"]
 *     channel:ch1 -> GROUP0 [ fontsize="8.0"];
 *     channel:ch5 -> GROUP0 [ fontsize="8.0"];
 *     channel:ch6 -> GROUP0 [ fontsize="8.0"];
 *     channel:ch3 -> GROUP0 [ fontsize="8.0"];
 *
 *     channel:ch2 -> GROUP1 [ fontsize="8.0"];
 *     channel:ch9 -> GROUP1 [ fontsize="8.0"];
 *     channel:ch11 -> GROUP1 [ fontsize="8.0"];
 *
 *     channel:ch10 -> GROUPn [fontsize="8.0"];
 *     channel:ch15 -> GROUPn [fontsize="8.0"];
 *
 *
 *   }
 *
 * @enddot
 */

/** Type for the AdcChannel - channels to be converted.
 *
 * Analog channels to be converted represented in bitwise manner belongs to a particular group.
 * Each bit position stands for the corresponding channel number.
 */
typedef uint16 Adc_ChannelSetType;

/** This is the type for the group number used as part of the ADC service interface.
 *
 * It is logical channel number of the ADC Group from the config array of type "Adc_GroupDefType".
 */
typedef uint8  Adc_GroupType;


/** This enumeration is the definition for the HW-unit number. */
typedef enum {
	ADC_NODE_0,		/**< NODE0 shall be used. */
	ADC_NODE_1,		/**< NODE1 shall be used. */
} Adc_HwUnitType;



/** This type defines the different conversion modes for a particular set of channel.
 *
 * This type has to configured in the config structure type "Adc_GroupDefType".
 * */
typedef enum {
	ADC_CONV_MODE_ONESHOT,		/**< For each call of start interface, one conversion happens. */
	ADC_CONV_MODE_CONTINUOUS,	/**< Call of Start interface, Starts the conversion and it keeps converting till stop is called. */
} Adc_GroupConvModeType;


/** It is the type used for the Adc Result Buffer Pointer(AdcResultBufferPointer).
 *
 * The AdcResultBufferPointer is the pointer to the array where the group channel results are stored.
 */
typedef uint16 Adc_ValueGroupType;


/** It is the type used for each analog channel sampling time which is part of "Adc_ChannelProfileType".
 *
 * For each HW node, only 4 different sampling time is possible. So the 16 analog channels must use one of the 4 values
 */
typedef uint16 Adc_SamplingTimeType;


/** It is the type used for each analog channel resolution which is part of "Adc_ChannelProfileType".
 *
 * For each HW node, only 4 different resolution s possible. So the 16 analog channels must use one of the 4 values
 */
typedef uint16 Adc_ResolutionType;


/** This type defines the current status of the conversion of the Adc channel group.
 *
 * The status can be obtained by Adc_GetGroupStatus().
 */
typedef enum {
	ADC_IDLE,				/**< The Adc driver didnt perform any conversion for the specified group. */
	ADC_BUSY,				/**< The Adc driver is converting this specified group channels. */
	ADC_COMPLETED,			/**< The Adc driver finished conversion for this specified group channels */
	ADC_STREAM_COMPLETED	/**< As Streaming mode is not supported, all groups comes directly to this state without ADC_COMPLETED. */
} Adc_StatusType;


/** This is the Adc channel profile configuration structure.
 *
 *  There shall be maximum of only 4 profile classes for each node. It implies each
 *  of the 16 channel must use one of the 4 profile classes. This profile class structure
 *  is part of the Node configuration structure.
 */

typedef struct
{
	Adc_SamplingTimeType AdcChannelSampTime;		/**< ADC Channel Sampling time.*/
	Adc_ResolutionType AdcChannelResolution;		/**< ADC Channel result Resolution. */
}Adc_ChannelProfileType;


/** This is the Adc Channel group configuration structure.
 *
 * This structure links the set of analog channels to be converted to the HW to be used and result location.
 */
typedef struct
{
	Adc_ChannelSetType AdcChannelSet;				/**< Analog Channels which are part of group, each bit represents corresponding analog channel. */
	Adc_HwUnitType AdcHwNodeId;						/**< Represents the HW Node 0 or 1 for 1767. */
	Adc_GroupConvModeType AdcConversionMode;   		/**< Represents the conversion trigger type - Oneshot or Continuous.  */
	uint8 DmaChannel;								/**< DMA channel to be used for transfer of result. DMA0,2 use for SWtrigger and DMA1,3 used for Continuous trigger*/
	void (*notification)(Adc_GroupType);			/**< Call back notification function. */
	Adc_ValueGroupType * AdcResultBufferPointer;	/**< Pointer where results are stored for this group. */
}Adc_GroupDefType;


/** This is the ADC HW node configuration structure.
 *
 * This structure is used to configure the HW ADC node with profile information and profile value for each analog channel.
 */

typedef struct
{
	Adc_HwUnitType AdcHwNodeId;							/**< Represents the HW Node 0 or 1 for 1767. */
	uint8 number_of_profile;							/**< Number of profile configuration[1..4]. */
	uint8 AdcProfileIndex[16];							/**< Array(16) of profile index to be written to each analog channel. */
	const Adc_ChannelProfileType *AdcChannelProfileDefinition;/**< Pointer to profile configuration structure. */
}Adc_NodeType;


/** This is the Adc driver configuration structure which is passed as parameter to the Adc_Init function.
 *
 *  This structure has the pointer to the other substructure node and group info.
 */

typedef struct
{
	uint8 number_of_groups;					/**< This elements defines the total number of groups configured. */
	uint8 number_of_nodes;					/**< This elements defines the number of configured nodes(1 or 2).*/
	const Adc_GroupDefType *AdcGroupDefinition; /**< Pointer to the adc group. */
	const Adc_NodeType *AdcNodeDefinition;	/**< Pointer to the node structure. */
}Adc_ConfigType;


#define ADC_START_SEC_CODE
#include "MemMap.h"


/** This service initializes the Adc driver.
 *
 * The function Adc_Init initializes the Adc module. After the call of this interface, all other
 * services return valid value.
 *
 * @note After the execution of the function Adc_Init(), the configuration data are accessible
 * and can be used by the Adc module functions. If the ADC Module is already in INIT State, then
 * DeInit has to called before Initializing the driver again.
 *
 * @param cfg   Pointer to the Adc configuration.
 *
 */
extern void Adc_Init(const Adc_ConfigType *cfg);


/** This service shuts down the Adc driver.
 *
 *  After the DeInit() call, the driver interfaces are not usable anymore.
 */
extern void Adc_DeInit(void);


/** This service starts the AD conversion for the passed group.
 *
 * This function starts the new conversion only if the group is in IDLE or STREAM_COMPLETED State.
 * Otherwise it return BUSY status and continous the old conversion group.
 *
 * This function has to be called every time when it is required to convert the Analog channel if it
 * is SwTrigger (ONE SHOT mode) group. In case of Continuous Mode this interface need not to be called each
 * time to convert the channels.
 *
 * Call of this interface resets the result buffer to the beginning of the result array.
 *
 * @param group  Identifier of the Adc group whose channel needs to be converted.
 */
extern void Adc_StartGroupConversion(Adc_GroupType group);


/** This service stops the AD conversion for the passed group.
 *
 * After the call of this interface, continuous conversion will not happen if the
 * group conversion mode is Continuous. The on-going conversions will be stopped
 * as soon as this interface is called.
 *
 * All the pending Sw Trigger request gets cancelled and group state is set to IDLE.
 *
 * @param group  Identifier of the Adc group whose channel needs not to be converted.
 */
extern void Adc_StopGroupConversion(Adc_GroupType group);


/** This interface enables the notification mechanism. The notofication function will
 * be called when the conversion is finished and results are available.
 *
 * As the DMA is used for the transfer of results from HW register to result array, the
 * DMA interrupt sources will be used to raise the notification.
 *
 * @param group  Identifier of the Adc group whose notification mechanism has to be enabled.
 */
extern void ADC_EnableGroupNotification(Adc_GroupType group);


/** This interface disable the notification mechanism.
 *
 * @param group  Identifier of the Adc group whose notification mechanism has to be disabled.
 */
extern void ADC_DisableGroupNotification(Adc_GroupType group);


/** This interface reads the ADC group results.
 *
 * The corresponding ADC group channel results are copied from ADC driver internal buffer ot
 * the user passed buffer.
 *
 * If the interface is called before the results are available, the interface return error and
 * doesnt copy any result values.
 *
 * @param group  Identifier of the Adc group whose channel results will be returned.
 * @param DataBufferPtr Pointer to the memory location where the results for the passed group must be copied.
 * @return #E_NOT_OK: if the results are not copied
 * 		   #E_OK: if the results arecopied
 */
extern Std_ReturnType Adc_ReadGroup(Adc_GroupType group, Adc_ValueGroupType * DataBufferPtr);


/** This service setsup the result buffer for the corresponding Adc Group.
 *
 * This function changes the result buffer location for any group.
 * This function takes care that when the conversion is going-on the group's result
 * array is not updated.
 *
 * @param group 		Identifier of the Adc group whose result buffer will be setup.
 * @param DataBufferPtr Pointer to the result buffer.
 * @return #E_NOT_OK: if the buffer is not set
 * 		   #E_OK: if the buffer is set
 */
extern Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType group, Adc_ValueGroupType * DataBufferPtr);


/** The function Adc_GetVersionInfo returns the version information of the Adc module.
 *
 * The version information includes: Module Id, Vendor Id, Vendor specific version numbers. \n
 *
 * @note The specification requests that the function Adc_GetVersionInfo() shall be pre-compile time
 * configurable On/Off by the configuration parameter AdcVersionInfoApi. This is not supported
 * by this implementation, since this implementation provides this interface as an inline function
 * therefore this configuration is futile.
 *
 * @param[out] versioninfo Pointer to where to store the version information of this module.
 */
inline void Adc_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	versioninfo->moduleID = ADC_MODULE_ID;
	versioninfo->sw_major_version = ADC_SW_MAJOR_VERSION;
	versioninfo->sw_minor_version = ADC_SW_MINOR_VERSION;
	versioninfo->sw_patch_version = ADC_SW_PATCH_VERSION;
	versioninfo->vendorID = ADC_VENDOR_ID;
}


/** This service returns the status of the ADC group.
 *
 *  The ADC group status which is maintained by SW will be returned at the call of this interface.
 *
 * @param group 		Identifier of the Adc group whose status has to be returned.
 * @return 	#ADC_IDLE:        		 There is no activity in this group\n
 *          #ADC_BUSY:    			 Conversion is going-on for this group \n
 *          #ADC_STREAM_COMPLETED:   Conversion is finished and results are available.
 */
extern Adc_StatusType Adc_GetGroupStatus(Adc_GroupType group);


/** The function Adc_MainFunction_Handling has to be used if the ADC driver is used in Polling Mode.
 *
 * This function has to be called in an timer tasks (periodic task) so that it detects
 * any interrupt is pending which needs to be served.
 *
 * @note This interface is not mentioned in the Autosar specification, but it has been
 * provided, so that user can use the ADC driver in polling mode.
 */
extern void Adc_MainFunction_Handling(void);


/** This function enables the polling mode.
 *
 * The polling mode works without the interrupts. Therefore the function Adc_MainFunction_Handling()
 * needs to be used to process the interrupts.
 *
 * @note This interface is not mentioned in the Autosar specification, but it has been
 * provided, so that user can use the ADC driver in polling mode.
 */
void Adc_SetPollingMode(void);



#define ADC_STOP_SEC_CODE
#include "MemMap.h"

/*@}*/

#endif /* ADC_H_ */
