

#include <stdio.h>
#include "xil_printf.h"
#include "xil_types.h"
#include "LMX_display.h"

// Offsets match exported txt. R112 downto R0
const lmx_reg_def_t LMX_reg_defs[] = {
	/*
		 * Note: mask is applied after shift
		 */
	/*                      mask  shift offset addr      reg         */
	[LMX_OSC_2X] = {0x0001, 12, 103, 0x09},		  /* R9 [12]  */
	[LMX_PLL_R_PRE] = {0x00FF, 0, 100, 0x0C},	  /* R12 [7:0]  */
	[LMX_MULT] = {0x001F, 7, 102, 0x0A},		  /* R10 [11:7]    */
	[LMX_PLL_R] = {0x00FF, 4, 101, 0x0B},		  /* R11 [11:4]    */
	[LMX_PLL_N_l] = {0xFFFF, 0, 76, 0x24},		  /* R36 [15:0]    */
	[LMX_PLL_N_h] = {0x0007, 0, 78, 0x22},		  /* R34 [2:0]  */
	[LMX_PLL_NUM_l] = {0xFFFF, 0, 69, 0x2B},	  /* R43 [15:0]    */
	[LMX_PLL_NUM_h] = {0xFFFF, 0, 70, 0x2A},	  /* R42 [15:0]    */
	[LMX_PLL_DEN_l] = {0xFFFF, 0, 73, 0x27},	  /* R39 [15:0]    */
	[LMX_PLL_DEN_h] = {0xFFFF, 0, 74, 0x26},	  /* R38 [15:0]    */
	[LMX_CHDIV] = {0x001F, 6, 37, 0x4B},		  /* R75 [10:6]    */
	[LMX_OUTA_MUX] = {0x0003, 11, 67, 0x2D},	  /* R45 [12:11]    */
	[LMX_OUTB_MUX] = {0x0003, 0, 66, 0x2E},		  /* R46 [1:0]    */
	[LMX_SYSREF_DIV_PRE] = {0x0007, 5, 41, 0x47}, /* R71 [7:5]    */
	[LMX_SYSREF_DIV] = {0x07FF, 0, 40, 0x48},	  /* R72 [11:0]    */
	[LMX_OUTA_PD] = {0x0001, 6, 68, 0x2C},		  /* R44 [6]    */
	[LMX_OUTB_PD] = {0x0001, 7, 68, 0x2C},		  /* R44 [7]    */

};

// offsets match order of read back data which is reverse of exported .txt
//const lmx_reg_def_t LMX_reg_defs[] = {
//        /*
//		 * Note: mask is applied after shift
//		 */
//
//		/* Following registers section repeats 8 times. So _DIV is at 0x100, 0x108, 0x110, etc */
//        /*                      mask  shift offset addr      reg         */
//		[LMX_OSC_2X]           = {0x0001,  12,   9,   0x09}, /* R9 [12]  */
//		[LMX_MULT]             = {0x001F,   7,  10,   0x0A}, /* R10 [11:7]    */
//		[LMX_PLL_R]            = {0x00FF,   4,  11,   0x0B}, /* R11 [11:4]    */
//		[LMX_PLL_R_PRE]        = {0x000F,   0,  12,   0x0C}, /* R12 [7:0]  */
//		[LMX_PLL_N_h]          = {0x0007,   0,  34,   0x22}, /* R34 [2:0]  */
//		[LMX_PLL_N_l]          = {0xFFFF,   0,  36,   0x24}, /* R36 [15:0]    */
//		[LMX_PLL_DEN_h]        = {0xFFFF,   0,  38,   0x26}, /* R38 [15:0]    */
//		[LMX_PLL_DEN_l]        = {0xFFFF,   0,  39,   0x27}, /* R39 [15:0]    */
//		[LMX_PLL_NUM_h]        = {0xFFFF,   0,  42,   0x2A}, /* R42 [15:0]    */
//		[LMX_PLL_NUM_l]        = {0xFFFF,   0,  43,   0x2B}, /* R43 [15:0]    */
//		[LMX_OUTA_PD]          = {0x0001,   6,  44,   0x2C}, /* R44 [6]    */
//		[LMX_OUTB_PD]          = {0x0001,   7,  44,   0x2C}, /* R44 [7]    */
//		[LMX_OUTA_MUX]         = {0x0003,  11,  45,   0x2D}, /* R45 [12:11]    */
//		[LMX_OUTB_MUX]         = {0x0003,   0,  46,   0x2E}, /* R46 [1:0]    */
//		[LMX_SYSREF_DIV_PRE]   = {0x0007,   5,  71,   0x47}, /* R71 [7:5]    */
//		[LMX_SYSREF_DIV]       = {0x07FF,   0,  72,   0x48}, /* R72 [11:0]    */
//		[LMX_CHDIV]            = {0x001F,   6,  75,   0x4B}, /* R75 [10:6]    */
//
//};

#define LMX_RegVal(index, regValArray) (((regValArray)[LMX_reg_defs[index].offset] >> LMX_reg_defs[index].shift) & LMX_reg_defs[index].mask)

void LMX_SettingsInit(long int inFreq, const u32 *regdataSrc, lmx_config_t *lmxInstPtr)
{

	lmxInstPtr->osc_doubler_multiplier = LMX_RegVal(LMX_OSC_2X, regdataSrc) + 1;

	lmxInstPtr->pll_preR_div = LMX_RegVal(LMX_PLL_R_PRE, regdataSrc);
	if (lmxInstPtr->pll_preR_div == 0)
		lmxInstPtr->pll_preR_div = 128;

	lmxInstPtr->pll_multiplier = LMX_RegVal(LMX_MULT, regdataSrc);
	if (lmxInstPtr->pll_multiplier == 0)
		lmxInstPtr->pll_multiplier = 1;

	lmxInstPtr->pll_r_divider = LMX_RegVal(LMX_PLL_R, regdataSrc);
	if (lmxInstPtr->pll_r_divider == 0)
		lmxInstPtr->pll_r_divider = 1;

	lmxInstPtr->pll_n_divider = LMX_RegVal(LMX_PLL_N_h, regdataSrc) << 16 | LMX_RegVal(LMX_PLL_N_l, regdataSrc);

	lmxInstPtr->pll_fraction_numerator = LMX_RegVal(LMX_PLL_NUM_h, regdataSrc) << 16 | LMX_RegVal(LMX_PLL_NUM_l, regdataSrc);
	lmxInstPtr->pll_fraction_denominator = LMX_RegVal(LMX_PLL_DEN_h, regdataSrc) << 16 | LMX_RegVal(LMX_PLL_DEN_l, regdataSrc);

	switch (LMX_RegVal(LMX_CHDIV, regdataSrc))
	{
	case 0:
		lmxInstPtr->channel_div = 2;
		break;
	case 1:
		lmxInstPtr->channel_div = 4;
		break;
	case 2:
		lmxInstPtr->channel_div = 6;
		break;
	case 3:
		lmxInstPtr->channel_div = 8;
		break;
	case 4:
		lmxInstPtr->channel_div = 12;
		break;
	case 5:
		lmxInstPtr->channel_div = 16;
		break;
	case 6:
		lmxInstPtr->channel_div = 24;
		break;
	case 7:
		lmxInstPtr->channel_div = 32;
		break;
	case 8:
		lmxInstPtr->channel_div = 48;
		break;
	case 9:
		lmxInstPtr->channel_div = 64;
		break;
	case 10:
		lmxInstPtr->channel_div = 72;
		break;
	case 11:
		lmxInstPtr->channel_div = 96;
		break;
	case 12:
		lmxInstPtr->channel_div = 128;
		break;
	case 13:
		lmxInstPtr->channel_div = 192;
		break;
	case 14:
		lmxInstPtr->channel_div = 256;
		break;
	case 15:
		lmxInstPtr->channel_div = 384;
		break;
	case 16:
		lmxInstPtr->channel_div = 512;
		break;
	default:
		lmxInstPtr->channel_div = 768;
		break;
	}

	lmxInstPtr->sysref_pre_div = LMX_RegVal(LMX_SYSREF_DIV_PRE, regdataSrc);
	lmxInstPtr->sysref_div = LMX_RegVal(LMX_SYSREF_DIV, regdataSrc) * 2 + 4;

	lmxInstPtr->Fpd_freq = inFreq * lmxInstPtr->osc_doubler_multiplier / lmxInstPtr->pll_preR_div * lmxInstPtr->pll_multiplier / lmxInstPtr->pll_r_divider;

	lmxInstPtr->VCO_freq = lmxInstPtr->Fpd_freq * lmxInstPtr->pll_fraction_numerator / lmxInstPtr->pll_fraction_denominator +
						   (lmxInstPtr->Fpd_freq * lmxInstPtr->pll_n_divider);

	lmxInstPtr->chandiv_freq = lmxInstPtr->VCO_freq / lmxInstPtr->channel_div;

	lmxInstPtr->sysref_freq = lmxInstPtr->VCO_freq / lmxInstPtr->sysref_pre_div / lmxInstPtr->sysref_div;

	if (LMX_RegVal(LMX_OUTA_PD, regdataSrc) == 1)
	{
		lmxInstPtr->RFoutA_freq = -1;
	}
	else
	{
		switch (LMX_RegVal(LMX_OUTA_MUX, regdataSrc))
		{
		case LMX_OUTA_MUX_CHAN_DIV:
			lmxInstPtr->RFoutA_freq = lmxInstPtr->chandiv_freq;
			break;
		case LMX_OUTA_MUX_VCO:
			lmxInstPtr->RFoutA_freq = lmxInstPtr->VCO_freq;
			break;
		default:
			lmxInstPtr->RFoutA_freq = -1;
			break;
		}
	}

	if (LMX_RegVal(LMX_OUTB_PD, regdataSrc) == 1)
	{
		lmxInstPtr->RFoutB_freq = -1;
	}
	else
	{
		switch (LMX_RegVal(LMX_OUTB_MUX, regdataSrc))
		{
		case LMX_OUTB_MUX_CHAN_DIV:
			lmxInstPtr->RFoutB_freq = lmxInstPtr->chandiv_freq;
			break;
		case LMX_OUTB_MUX_VCO:
			lmxInstPtr->RFoutB_freq = lmxInstPtr->VCO_freq;
			break;
		case LMX_OUTB_SYSREF:
			lmxInstPtr->RFoutB_freq = lmxInstPtr->sysref_freq;
			break;
		default:
			lmxInstPtr->RFoutB_freq = -1;
			break;
		}
	}
}

void LMX_intermediateDump(lmx_config_t *lmxInstPtr)
{

	// print mult/divr values
	xil_printf("osc_doubler_multiplier:%d\n\r", lmxInstPtr->osc_doubler_multiplier);
	xil_printf("pll_preR_div:%d\n\r", lmxInstPtr->pll_preR_div);
	xil_printf("pll_multiplier:%d\n\r", lmxInstPtr->pll_multiplier);
	xil_printf("pll_r_divider:%d\n\r", lmxInstPtr->pll_r_divider);
	xil_printf("pll_n_divider:%d\n\r", lmxInstPtr->pll_n_divider);
	xil_printf("pll_fraction_numerator:%ld\n\r", lmxInstPtr->pll_fraction_numerator);
	xil_printf("pll_fraction_denominator:%ld\n\r", lmxInstPtr->pll_fraction_denominator);
	xil_printf("channel_div:%d\n\r", lmxInstPtr->channel_div);
	xil_printf("sysref_pre_div:%d\n\r", lmxInstPtr->sysref_pre_div);
	xil_printf("sysref_div:%d\n\r\n\r", lmxInstPtr->sysref_div);

	// print intermediate calculated values
	xil_printf("VCO_freq:%ld\n\r", lmxInstPtr->VCO_freq);
	xil_printf("chandiv_freq:%ld\n\r", lmxInstPtr->chandiv_freq);
	xil_printf("sysref_freq:%ld\n\r", lmxInstPtr->sysref_freq);
}
