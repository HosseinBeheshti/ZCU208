/*
 * adcSaveCalCoefficients.h
 *
 *  Created on: Aug 10, 2019
 *      Author: jlantz
 */

#ifndef ADCSAVECALCOEFF_H_
#define ADCSAVECALCOEFF_H_

/***************************** Include Files *********************************/
#include "xil_types.h"

/************************** Constant Definitions *****************************/
#define COEFF_OCB2 1
#define COEFF_TSCB 2
#define COEFF_GCB 3
#define COEFF_OCB1 0

//  NUM_TILES and NUM_BLOCKS set in main.h
#define NUM_SETS 4
//#define NUM_TILES 4
//#define NUM_BLOCKS 4

/**************************** Type Definitions *******************************/
typedef struct
{
	u32 written;
	u32 Coeff[8];
} singleCoeffType;

typedef struct
{
	singleCoeffType type[4];
} perAdcCoeffType;

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void adcSaveCalCoeff(u32 *cmdVals);
void adcCoeffClr(u32 *cmdVals);
void adcCoeffSet(u32 *cmdVals);
void adcCoeffSetDump(u32 *cmdVals);

/************************** Variable Definitions *****************************/

#endif /* ADCSAVECALCOEFF_H_ */
