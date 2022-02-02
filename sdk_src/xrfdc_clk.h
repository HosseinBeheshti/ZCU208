/******************************************************************************
 * *
 * * Copyright (C) 2018 Xilinx, Inc.  All rights reserved.
 * *
 * * Permission is hereby granted, free of charge, to any person obtaining a copy
 * * of this software and associated documentation files (the "Software"), to deal
 * * in the Software without restriction, including without limitation the rights
 * * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * * copies of the Software, and to permit persons to whom the Software is
 * * furnished to do so, subject to the following conditions:
 * *
 * * The above copyright notice and this permission notice shall be included in
 * * all copies or substantial portions of the Software.
 * *
 * * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * * XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * * SOFTWARE.
 * *
 * * Except as contained in this notice, the name of the Xilinx shall not be used
 * * in advertising or otherwise to promote the sale, use or other dealings in
 * * this Software without prior written authorization from Xilinx.
 * *
 * ******************************************************************************/
#ifndef RFDC_CLK_H_
#define RFDC_CLK_H_

/***************************** Include Files *********************************/


/******************** Constant Definitions **********************************/
#define RF1_ADC0_1_2594_A_SS 8
#define RF2_ADC2_3_2594_B_SS 4
#define LMK4208_SS           2
#define RF3_DAC0_1_2594_C_SS 1

#define LMK04208_count 26
#define LMX2594_A_count 113

/**************************** Type Definitions *******************************/
typedef struct {
	int XFrequency;
	unsigned int LMX2594_A[LMX2594_A_count];
} XClockingLmx;

typedef struct {
	int Fosc;			//Fosc freq (KHz)
	int axiFreq;		// axi clock freq (KHz)
	int sysrefFreq;     // sysref freq (KHz)
	int refClkFreq;		// refclk Frequency (KHz)
	int refClkSrc;		// 1=refclk external
	unsigned int data[LMK04208_count];
} XClockingLmk;


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

void LMX2594ClockConfig(int XIicBus, int DacXFrequency, int AdcXFrequency);
void LMK04208ClockConfig(int XIicBus, int LMK04208_configIndex);
void LMX2594DisplaySupportedFreq(void);
void LMK04208DisplaySettings(int LMK04208_configIndex);
void stringCnv_KHz_2_HZ(int valKhz, char *bufptr);

#endif /* RFDC_CLK_H_ */
