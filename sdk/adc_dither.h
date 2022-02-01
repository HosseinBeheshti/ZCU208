/*
 *  adc_dither.h
 *
 *  Created on: Sept 15, 2020
 *      Author: jlantz
 */
#ifndef SRC_ADC_DITHER_H_
#define SRC_ADC_DITHER_H_

/***************************** Include Files *********************************/
#include "xil_types.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_adc_dither_init(void);

void adcGetDither(u32 *cmdVals);
void adcSetDither(u32 *cmdVals);

/************************** Variable Definitions *****************************/



#endif /* SRC_ADC_DITHER_H_ */

