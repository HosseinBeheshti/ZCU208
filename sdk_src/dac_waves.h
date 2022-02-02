/*
 * dac_waves.h
 *
 *  Created on: 11may20
 *      Author: jmcd
 */

#ifndef SRC_DAC_WAVES_H_
#define SRC_DAC_WAVES_H_

/***************************** Include Files *********************************/
#include "xil_types.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_dac_waves_init(void);
void waveSetBuf (u32 *cmdVals);
void waveSawtooth (u32 *cmdVals);
void dacSinCos(u32 *cmdVals);
void waveSin(u32 *cmdVals);

/************************** Variable Definitions *****************************/

#endif /* SRC_DAC_WAVES_H_ */
