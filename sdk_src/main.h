/*
 * main.h
 *
 *  Created on: Sep 17, 2017
 *      Author: johnmcd
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

/***************************** Include Files ********************************/
#include "xparameters.h"
#include "xil_types.h"
#include "xrfdc.h"

/******************** Constant Definitions **********************************/

// Necessary to use this define when using jtagterminal but not SDK jtaguart console
//#define STRIP_CHAR_CR

// RFDC defines
#define RFDC_DEVICE_ID XPAR_XRFDC_0_DEVICE_ID
#define RFDC_BASE XPAR_XRFDC_0_BASEADDR

// Number of Tiles and Blocks in device
#define NUM_TILES 4
#define NUM_BLOCKS 4

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes *****************************/

/************************** Variable Definitions ****************************/

extern XRFdc RFdcInst; /* RFdc driver instance */

#endif /* SRC_MAIN_H_ */
