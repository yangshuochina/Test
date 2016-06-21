/* ***************************************************************************
 *
 *  COPYRIGHT:          $Company: AVL Software and Functions GmbH $
 *  FILENAME:           $Source: WdgIf_Types.h $
 *  COMPILER:           Tasking
 *  PROZESSOR:          Tricore
 *
 *  DATE OF CREATION:   19.05.2010
 *  LAST REVISION:      $Date: 2011/03/22 09:29:22MEZ $
 *                      $Author: Cotor Vlad RGB (COTORV) $
 *
 * ***************************************************************************
 *
 *  $Log: WdgIf_Types.h  $
 *  Revision 1.3 2011/03/22 09:29:22MEZ Cotor Vlad RGB (COTORV) 
 *  - Optimize BSW documentation
 *  Revision 1.2 2010/11/22 08:53:34MEZ Cotor Vlad RGB (COTORV) 
 *  Update BSW software modules with doxygen documentation comments
 *  Revision 1.1 2010/05/19 14:14:00MESZ Cotor Vlad RGB (COTORV) 
 *  Initial revision
 *  Member added to project /PSL/Bsw/EcuAbstractionLayer/WdgIf/AA000/src/project.pj
 *
 * ***************************************************************************/


#ifndef WDGIF_TYPES_H_
#define WDGIF_TYPES_H_

#include "team-s.h"
#include "Std_Types.h"

/** @defgroup group_wdgif_config WdgIf Configuration Parameters. 
 *  @ingroup group_wdgif */
/*@{*/

/**
 * This type defines the different status of the Watchdog Driver.
 * There is no interface to get this status.
 * It is used only inside the driver interfaces.
 * The following shows the states and the possible activities related to them.
 * @image latex external_images/wdgif_seq_states.pdf "WdgIf SEQ States"
 */
/* @dot
 *   digraph seq_states {
 *     node [style="rounded" shape="box" fontsize="10.0"];
 *     edge [fontsize="8.0"];
 *
 *     OK [label = "WDGIF_UNINIT(not running)"];
 *     PENDING_I [label = "WDGIF_IDLE (running/pre-warning)"];
 *     PENDING_B [label = "WDGIF_BUSY (pre-warning)"];
 *
 *     OK -> PENDING_B [label="Wdg_Init()"];
 *     PENDING_B -> PENDING_I [label="Wdg_SetMode() or Wdg_Trigger()"];
 *     PENDING_I -> PENDING_B [label="Mode/Trigger updated"];
 *
 *   }
 * @enddot
 *
 */
typedef enum {
  WDGIF_UNINIT = 0,    /**< The watchdog driver is in un-initialized state. */
  WDGIF_IDLE=1,        /**< The watchdog driver is idle(WDG timer is running/switched off). */
  WDGIF_BUSY =2        /**< The watchdog driver is being switched between modes or getting re-triggered. */
}WdgIf_StatusType;

/**
 * This type defines various mode in which WDG driver can work.
 * WDG timer is running  based on F<SUB>fpi</SUB>. \n
 * F<SUB>fpi</SUB> Clock is derived from F<SUB>pll</SUB> as below. \n
 * <B>F<SUB>fpi</SUB> = F<SUB>pll</SUB>/FPIDIV. </B>\n
 * Where as \n
 * @li F<SUB>fpi</SUB> : Frequency of FPI Bus.
 * @li F<SUB>pll</SUB> : Frequency of PLL output.
 * @li FPIDIV: Division factor of PLL output [1..16]. Refer @ref mcu . \n
 *
 * Time out period for WDG is calculated by the following formula \n
 * <B> TIMEOUT = ((2<SUP>16</SUP> &ndash; startvalue)*256*2<SUP>(1 &ndash; IR)*6</SUP>)/F<SUB>fpi</SUB> </B> <br>
 * Following table shows the one tick value and maximum timeout in case of
 * slow or fast watchdog configuration.
 *
 * <table>
 *   <tr><th>MODE -></th><th colspan="2">WDGIF_SLOW_MODE</th><th colspan="2">WDGIF_FAST_MODE</th></tr>
 *   <tr><th>F<SUB>fpi</SUB></th><th>Minimum</th><th>Maximum</th><th>Minimum</th><th>Maximum</th></tr>
 *   <tr><td>10Mhz</td><td> 16384us </td><td> 107.372s </td><td> 256us </td><td> 1.677s </td></tr>
 *   <tr><td>20Mhz</td><td> 8192us </td><td> 053.686s </td><td> 128us </td><td> 0.838s </td></tr>
 *   <tr><td>30Mhz</td><td> 546us  </td><td> 035.790s </td><td> 085us </td><td> 0.559s </td></tr>
 *   <tr><td>40Mhz</td><td> 409us  </td><td> 026.843s </td><td> 064us </td><td> 0.419s </td></tr>
 *   <tr><td>50Mhz</td><td> 327us  </td><td> 021.474s </td><td> 051us </td><td> 0.335s </td></tr>
 *   <tr><td>60Mhz</td><td> 273us  </td><td> 017.895s </td><td> 042us </td><td> 0.279s </td></tr>
 *   <tr><td>70Mhz</td><td> 234us  </td><td> 015.338s </td><td> 036us </td><td> 0.239s </td></tr>
 *   <tr><td>80Mhz</td><td> 204us  </td><td> 013.421s </td><td> 032us </td><td> 0.209s </td></tr>
 *   <tr><td>90Mhz</td><td> 182us  </td><td> 011.930s </td><td> 028us </td><td> 0.186s </td></tr>
 *   <tr><td>100Mhz</td><td> 163us  </td><td> 010.737s </td><td> 025us </td><td> 0.167s </td></tr>
 *   <tr><td>110Mhz</td><td> 148us  </td><td> 009.761s </td><td> 023us </td><td> 0.152s </td></tr>
 *   <tr><td>120Mhz</td><td> 137us  </td><td> 008.947s </td><td> 021us </td><td> 0.139s </td></tr>
 * </table>
 *
 */
typedef enum {
  WDGIF_OFF_MODE,  /**< Watch dog is disabled. */
  WDGIF_SLOW_MODE, /**< Watch dog is set up for a long timeout period F<SUB>fpi</SUB>/16384. */
  WDGIF_FAST_MODE /**< Watch dog is set up for a short timeout period F<SUB>fpi</SUB>/256. */
}WdgIf_ModeType;

/*@}*/
#endif /* WDGIF_TYPES_H_ */
