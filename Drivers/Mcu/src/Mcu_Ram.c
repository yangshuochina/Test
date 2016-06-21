/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: Mcu_Ram.c $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2010/05/19 14:18:08MESZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * *************************************************************************** 
 *
 *  $Log: Mcu_Ram.c  $
 *  Revision 1.1 2010/05/19 14:18:08MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/MicrocontrollerAbstractionLayer/Mcu/AA000/src/project.pj
 *
 * ***************************************************************************/

#include "Mcu.h"
#include "Mcu_Priv.h"

#define MCU_START_SEC_CODE
#include "MemMap.h"


Std_ReturnType Mcu_InitRamSection(Mcu_RamSectionType num_of_ram_section)
{
	sint32 size;
	uint32 *addr;
	const uint32 *image;

	MCU_ASSERT_RETERR(mcu_cfg != NULL_PTR, MCU_INITRAMSECTION_API_ID, MCU_E_UNINIT);
	MCU_ASSERT_RETERR(num_of_ram_section < mcu_cfg->Mcu_NumberOfRamSections, MCU_INITRAMSECTION_API_ID, MCU_E_PARAM_RAMSECTION);

	/* get the relevant data from the configuration structure */
	addr = mcu_cfg->Mcu_RamConfig[num_of_ram_section].start_addr;
	image = mcu_cfg->Mcu_RamConfig[num_of_ram_section].start_image;
	size = mcu_cfg->Mcu_RamConfig[num_of_ram_section].size;

	if(image == NULL_PTR) {
		/* we need only to clear the memory range */
		while(size > 0) {
			*addr = 0;
			addr++;
			size-=4;
		}
	}
	else {
		/* copy from the image */
		while(size > 0) {
			*addr = *image;
			addr++;
			image++;
			size-=4;
		}
	}
	return E_OK;
}
#define MCU_STOP_SEC_CODE
#include "MemMap.h"

