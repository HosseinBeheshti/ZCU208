/******************************************************************************
*
* Copyright (C) 2017-2019 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xrfclk.h
* @addtogroup xrfclk_v1_1
* @{
*
* Contains the API of the XRFclk middleware.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ---    -------- -----------------------------------------------
* 1.0   dc     07/21/19 Initial version
* 1.1   dc     11/21/19 Remove xil dependencies from linux build
*       dc     11/25/19 update LMX and LMK configs
*       dc     12/05/19 adjust LMX and LMK configs to a rftool needs
*
* </pre>
*
******************************************************************************/
#ifndef __XRFCLK_H_
#define __XRFCLK_H_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(XPS_BOARD_ZCU111) && !defined(XPS_BOARD_ZCU216)
#define XPS_BOARD_ZCU216
#endif

#if defined __BAREMETAL__
#include "xil_types.h"
#else
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
#define XST_SUCCESS 0L
#define XST_FAILURE 1L
#endif

#define RFCLK_LMX2594_1 0 /* I0 on MUX and SS3 on Bridge */
#define RFCLK_LMX2594_2 1 /* I1 on MUX and SS2 on Bridge */
#define RFCLK_LMK 2 /* I2 on MUX and SS1 on Bridge */
#ifdef XPS_BOARD_ZCU111
#define RFCLK_LMX2594_3 3 /* I3 on MUX and SS0 on Bridge */
#define RFCLK_CHIP_NUM 4
#define LMK_COUNT 26
#define LMK_FREQ_NUM 3 /* Number of LMK freq. configs */
#define LMX_ADC_NUM 17 /* Number of LMX ADC configs */
#define LMX_DAC_NUM 26 /* Number of LMX DAC configs */
#else
#define RFCLK_CHIP_NUM 3
#define LMK_COUNT 128//// 136 //128
#define LMK_FREQ_NUM 7 /* Number of LMK freq. configs */
#define LMX_ADC_NUM 32 /* Number of LMX ADC configs */
#define LMX_DAC_NUM 32 /* Number of LMX DAC configs */
#endif

#define LMX2594_COUNT 116
#define FREQ_LIST_STR_SIZE 16 /* Frequency string size */

#ifndef XPS_BOARD_ZCU111
typedef enum {
	LMX2594_FREQ_204M80,
	LMX2594_FREQ_245M76,
	LMX2594_FREQ_491M52,
	LMX2594_FREQ_1024M00,
	LMX2594_FREQ_1966M08,
	LMX2594_FREQ_2000M00,
	LMX2594_FREQ_2211M84,
	LMX2594_FREQ_2457M60,
	LMX2594_FREQ_2500M00,
	LMX2594_FREQ_3072M00,
	LMX2594_FREQ_3932M16,
	LMX2594_FREQ_4000M00,
	LMX2594_FREQ_4096M00,
	LMX2594_FREQ_4423M68,
	LMX2594_FREQ_4915M20,
	LMX2594_FREQ_5000M00,
	LMX2594_FREQ_5400M00,
	LMX2594_FREQ_5898M24,
	LMX2594_FREQ_6144M00,
	LMX2594_FREQ_6881M28,
	LMX2594_FREQ_7000M00,
	LMX2594_FREQ_7372M80,
	LMX2594_FREQ_7864M32,
	LMX2594_FREQ_8000M00,
	LMX2594_FREQ_8192M00,
	LMX2594_FREQ_8847M36,
	LMX2594_FREQ_9830M40,
	LMX2594_FREQ_10000M00,
	LMX2594_FREQ_10800M00,
	LMX2594_FREQ_300M00_PD,
	LMX2594_FREQ_300M00,
	LMX2594_FREQ_4000M00B,
} LMK_freq_idx_t;
#endif

u32 XRFClk_WriteReg(u32 ChipId, u32 Data);
u32 XRFClk_ReadReg(u32 ChipId, u32 *Data);
#if defined __BAREMETAL__ || defined XPS_BOARD_ZCU111
u32 XRFClk_Init();
#else
u32 XRFClk_Init(int GpioId);
#endif
void XRFClk_Close();
u32 XRFClk_ResetChip(u32 ChipId);
u32 XRFClk_SetConfigOnOneChipFromConfigId(u32 ChipId, u32 ConfigId);
u32 XRFClk_SetConfigOnOneChip(u32 ChipId, u32 *cfgData, u32 len);
u32 XRFClk_GetConfigFromOneChip(u32 ChipId, u32 *cfgData);
u32 XRFClk_SetConfigOnAllChipsFromConfigId(u32 ConfigId_LMK, u32 ConfigId_RF1,
#ifdef XPS_BOARD_ZCU111
					   u32 ConfigId_RF2, u32 ConfigId_RF3);
#else
					   u32 ConfigId_RF2);
#endif
u32 XRFClk_ControlOutputPortLMK(u32 PortId, u32 state);
u32 XRFClk_ConfigOutputDividerAndMUXOnLMK(u32 PortId, u32 DCLKoutX_DIV,
					  u32 DCLKoutX_MUX, u32 SDCLKoutY_MUX,
					  u32 SYSREF_DIV);

#ifdef __cplusplus
}
#endif

#endif
/** @} */
