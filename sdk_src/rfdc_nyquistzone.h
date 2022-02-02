/*

 * rfdc_NyquistZone.h
 *
 *  Created on: Aug 12, 2019
 *      Author: jlantz
 */
#ifndef SRC_RFDC_NYQUISTZONE_H_
#define SRC_RFDC_NYQUISTZONE_H_

/***************************** Include Files *********************************/
#include "xil_types.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_rfdcNyquistzone_init(void);

void rfdcNyquistZone(u32 *cmdVals);
void dacSetNyquistZone(u32 *cmdVals);
void adcSetNyquistZone(u32 *cmdVals);

/************************** Variable Definitions *****************************/



#endif /* SRC_RFDC_NYQUISTZONE_H_ */

