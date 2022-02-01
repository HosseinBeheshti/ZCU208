/*
 * adcLoadCalCoefficients.h
 *
 *  Created on: Aug 10, 2019
 *      Author: jlantz
 */

#ifndef ADCLOADCALCOEFF_H_
#define ADCLOADCALCOEFF_H_


/***************************** Include Files *********************************/
#include "xil_types.h"
#include "adcSaveCalCoefficients.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/


void adcLoadBGCalCoeff(u32 *cmdVals);
void adcLoadFGCalCoeff(u32 *cmdVals);
void cli_adcLoadCalCoeff_init(void);


/************************** Variable Definitions *****************************/
extern perAdcCoeffType storedCoeffSets[][NUM_TILES][NUM_BLOCKS];
extern int selectedCoeffSet;

#endif /* ADCSAVECALCOEFF_H_ */
