

#include <stdio.h>
#include "xil_printf.h"
#include "xil_types.h"
#include "LMK_display.h"


const reg_def_t LMK_reg_defs[] = {
        /*
		 * Note: mask is applied after shift
		 */

		/* Following registers section repeats 8 times. So _DIV is at 0x100, 0x108, 0x110, etc */
        /*                      mask  shift offset addr      reg         */
		[LMK_DCLKoutX_DIV]     = {0x1F,   0,   9,   0x100}, /* R256 [4:0]  */
		[LMK_DCLKoutX_MUX]     = {0x03,   0,  12,   0x103}, /* R259 [1:0]  */
		[LMK_SDCLKoutY_MUX]    = {0x01,   5,  13,   0x104}, /* R260 [5]    */
		[LMK_DCLKoutX_1_PD]    = {0x01,   3,  15,   0x106}, /* R262 [3]    */
		[LMK_SDCLKoutY_PD]     = {0x01,   0,  15,   0x106}, /* R262 [0]    */
		[LMK_DCLKoutx_FMT]     = {0x07,   0,  16,   0x107}, /* R263 [2:0]  */
		[LMK_SDCLKoutY_FMT]    = {0x07,   4,  16,   0x107}, /* R263 [6:4]  */

        /*                      mask  shift offset addr      reg         */
		[LMK_SYSREF_DIV_h]     = {0x1F,   0,  67,   0x13A}, /* R314 [4:0]  */
		[LMK_SYSREF_DIV_l]     = {0xFF,   0,  68,   0x13B}, /* R315 [7:0]  */

		[LMK_PLL2_N_h]         = {0x03,   0, 115,   0x166}, /* R358 [1:0]  */
		[LMK_PLL2_N_m]         = {0xFF,   0, 116,   0x167}, /* R359 [7:0]  */
		[LMK_PLL2_N_l]         = {0xFF,   0, 117,   0x168}, /* R360 [7:0]  */
		[LMK_PLL2_R_h]         = {0x0F,   0, 105,   0x160}, /* R352 {3:0]  */
		[LMK_PLL2_R_l]         = {0xFF,   0, 106,   0x161}, /* R352 {7:0]  */
		[LMK_PLL2_REF_2X_EN]   = {0x01,   0, 107,   0x162}, /* R354 {1]  */
		[LMK_PLL2_P]           = {0x07,   5, 107,   0x162}, /* R354 {7:5]  */
		[LMK_PLL1_N_h]         = {0x3F,   0,  98,   0x159}, /* R345 [5:0]  */
		[LMK_PLL1_N_l]         = {0xFF,   0,  99,   0x15A}, /* R346 [7:0]  */
		[LMK_CLKin0_R_h]       = {0x3F,   0,  92,   0x153}, /* R339 [5:0]  */
		[LMK_CLKin0_R_l]       = {0xFF,   0,  93,   0x154}, /* R340 [7:0]  */
		[LMK_CLKin1_R_h]       = {0x3F,   0,  94,   0x155}, /* R341 [5:0]  */
		[LMK_CLKin1_R_l]       = {0xFF,   0,  95,   0x156}, /* R342 [7:0]  */
		[LMK_CLKin2_R_h]       = {0x3F,   0,  96,   0x157}, /* R343 [5:0]  */
		[LMK_CLKin2_R_l]       = {0xFF,   0,  97,   0x158}, /* R344 [7:0]  */

		[LMK_PLL2_NCLK_MUX]    = {0x01,   4,  72,   0x13F}, /* R319 [4]  */
		[LMK_FB_MUX_EN]        = {0x01,   0,  72,   0x13F}, /* R319 [4]  */
		[LMK_FB_MUX]           = {0x03,   1,  72,   0x13F}, /* R319 [2:1]  */
		[LMK_PLL1_NCLK_MUX]    = {0x01,   3,  72,   0x13F}, /* R319 {3]  */
		[LMK_CLKin_SEL_MODE]   = {0x07,   4,  80,   0x147}, /* R327 [6:4]  */
		[LMK_CLKin0_EN]        = {0x01,   3,  79,   0x146}, /* R326 [3]  */
		[LMK_CLKin1_EN]        = {0x01,   4,  79,   0x146}, /* R326 [4]  */
		[LMK_CLKin2_EN]        = {0x01,   5,  79,   0x146}, /* R326 [5]  */
		[LMK_VCO_MUX]          = {0x03,   5,  65,   0x138}, /* R312 [6:5]]  */
};


#define LMK_RegVal(index, regValArray) ( ( (regValArray)[LMK_reg_defs[index].offset] >> LMK_reg_defs[index].shift) & LMK_reg_defs[index].mask )


void LMK_init(const u32 *regdataSrc, lmk_config_t *lmkInstPtr)
{
    LMK_sysrefInit(regdataSrc, lmkInstPtr);
    LMK_clkoutCfgInit(regdataSrc, lmkInstPtr);
    LMK_clkinAndPLLInit(regdataSrc, lmkInstPtr);
    LMK_clkoutFreqInit(lmkInstPtr);

}


void LMK_clkinAndPLLInit(const u32 *regdataSrc, lmk_config_t *lmkInstPtr)
{
	lmkInstPtr->pll2_n_prescaler = ( !(LMK_RegVal(LMK_PLL2_P, regdataSrc)) ) ? 8 : LMK_RegVal(LMK_PLL2_P, regdataSrc);

	lmkInstPtr->pll2_n_divider = LMK_RegVal(LMK_PLL2_N_h, regdataSrc) << 16 | LMK_RegVal(LMK_PLL2_N_m, regdataSrc) << 8 | LMK_RegVal(LMK_PLL2_N_l, regdataSrc);
	lmkInstPtr->pll2_r_divider = LMK_RegVal(LMK_PLL2_R_h, regdataSrc) << 8 | LMK_RegVal(LMK_PLL2_R_l, regdataSrc);
	lmkInstPtr->pll2_doubler = LMK_RegVal(LMK_PLL2_REF_2X_EN, regdataSrc) + 1;

	lmkInstPtr->pll1_n_divider = LMK_RegVal(LMK_PLL1_N_h, regdataSrc) << 8 | LMK_RegVal(LMK_PLL1_N_l, regdataSrc);

	lmkInstPtr->clkin[0].div = LMK_RegVal(LMK_CLKin0_R_h, regdataSrc) << 8 | LMK_RegVal(LMK_CLKin0_R_l, regdataSrc);
	lmkInstPtr->clkin[1].div = LMK_RegVal(LMK_CLKin1_R_h, regdataSrc) << 8 | LMK_RegVal(LMK_CLKin1_R_l, regdataSrc);
	lmkInstPtr->clkin[2].div = LMK_RegVal(LMK_CLKin2_R_h, regdataSrc) << 8 | LMK_RegVal(LMK_CLKin2_R_l, regdataSrc);

	lmkInstPtr->clkin[0].en = LMK_RegVal(LMK_CLKin0_EN, regdataSrc);
	lmkInstPtr->clkin[1].en = LMK_RegVal(LMK_CLKin1_EN, regdataSrc);
	lmkInstPtr->clkin[2].en = LMK_RegVal(LMK_CLKin2_EN, regdataSrc);


	lmkInstPtr->PLL2_Phase_Detector_Freq = VCXO_FREQ * lmkInstPtr->pll2_doubler / lmkInstPtr->pll2_r_divider;


    if (LMK_RegVal(LMK_PLL2_NCLK_MUX, regdataSrc) == LMK_PLL2_NCLK_MUX_PLL2_P) {
    	lmkInstPtr->PLL2_vco_fdbk_div = lmkInstPtr->pll2_n_divider * lmkInstPtr->pll2_n_prescaler;
    } else {
    	switch( LMK_RegVal(LMK_FB_MUX, regdataSrc) ) {
			case LMK_FB_MUX_DCLKout6:
				lmkInstPtr->PLL2_vco_fdbk_div = lmkInstPtr->clkout[6/2].dclk_div;
				break;
			case LMK_FB_MUX_DCLKout8:
				lmkInstPtr->PLL2_vco_fdbk_div = lmkInstPtr->clkout[8/2].dclk_div;
				break;
			case LMK_FB_MUX_SYSREF_DIVIDER:
				lmkInstPtr->PLL2_vco_fdbk_div = -1;
				break;
			case LMK_FB_MUX_EXTERNAL:
				lmkInstPtr->PLL2_vco_fdbk_div = -1;
				break;
			default:
				lmkInstPtr->PLL2_vco_fdbk_div = -1;
				break;
    	}
    }


    lmkInstPtr->PLL2_VCO_freq = lmkInstPtr->PLL2_Phase_Detector_Freq * lmkInstPtr->PLL2_vco_fdbk_div;
    lmkInstPtr->sysref_freq   = lmkInstPtr->PLL2_VCO_freq / lmkInstPtr->sysref_div;


    if (LMK_RegVal(LMK_PLL1_NCLK_MUX, regdataSrc) == LMK_PLL1_NCLK_MUX_OSCIN) {
    	lmkInstPtr->PLL1_Phase_Detector_Freq = VCXO_FREQ / lmkInstPtr->pll1_n_divider;
    } else {
    	switch( LMK_RegVal(LMK_FB_MUX, regdataSrc) ) {
			case LMK_FB_MUX_DCLKout6:
				lmkInstPtr->PLL1_Phase_Detector_Freq = lmkInstPtr->PLL2_VCO_freq / lmkInstPtr->clkout[6/2].dclk_div / lmkInstPtr->pll1_n_divider;
				break;
			case LMK_FB_MUX_DCLKout8:
				lmkInstPtr->PLL1_Phase_Detector_Freq = lmkInstPtr->PLL2_VCO_freq / lmkInstPtr->clkout[8/2].dclk_div / lmkInstPtr->pll1_n_divider;
				break;
			case LMK_FB_MUX_SYSREF_DIVIDER:
				lmkInstPtr->PLL1_Phase_Detector_Freq = lmkInstPtr->sysref_freq / lmkInstPtr->pll1_n_divider;
				break;
			case LMK_FB_MUX_EXTERNAL:
				lmkInstPtr->PLL1_Phase_Detector_Freq = -1;
				break;
			default:
				lmkInstPtr->PLL1_Phase_Detector_Freq = -1;
				break;
    	}
    }


    lmkInstPtr->clkin_sel_mode = LMK_RegVal(LMK_CLKin_SEL_MODE, regdataSrc);


	// If clkin0 selected or clkin0 is enabled for auto mode, set freq
    if(lmkInstPtr->clkin_sel_mode == LMK_CLKin_SEL_MODE_CLKIN0_MANUAL ||
    		(lmkInstPtr->clkin_sel_mode == LMK_CLKin_SEL_MODE_AUTO_MODE && lmkInstPtr->clkin[0].en) ) {
    	lmkInstPtr->clkin[0].freq = lmkInstPtr->PLL1_Phase_Detector_Freq * lmkInstPtr->clkin[0].div;
    } else {
    	lmkInstPtr->clkin[0].freq = -1;
    }

	// If clkin1 selected or clkin1 is enabled for auto mode, set freq
    if(lmkInstPtr->clkin_sel_mode == LMK_CLKin_SEL_MODE_CLKIN1_MANUAL ||
    		(lmkInstPtr->clkin_sel_mode == LMK_CLKin_SEL_MODE_AUTO_MODE && lmkInstPtr->clkin[1].en) ) {
    	lmkInstPtr->clkin[1].freq = lmkInstPtr->PLL1_Phase_Detector_Freq * lmkInstPtr->clkin[1].div;
    } else {
    	lmkInstPtr->clkin[1].freq = -1;
    }

	// If clkin2 selected or clkin2 is enabled for auto mode, set freq
    if(lmkInstPtr->clkin_sel_mode == LMK_CLKin_SEL_MODE_CLKIN2_MANUAL ||
    		(lmkInstPtr->clkin_sel_mode == LMK_CLKin_SEL_MODE_AUTO_MODE && lmkInstPtr->clkin[2].en) ) {
    	lmkInstPtr->clkin[2].freq = lmkInstPtr->PLL1_Phase_Detector_Freq * lmkInstPtr->clkin[2].div;
    } else {
    	lmkInstPtr->clkin[2].freq = -1;
    }


}


void LMK_sysrefInit(const u32 *regdataSrc, lmk_config_t *lmkInstPtr)
{
	lmkInstPtr->sysref_div = LMK_RegVal(LMK_SYSREF_DIV_h, regdataSrc) << 8 | LMK_RegVal(LMK_SYSREF_DIV_l, regdataSrc);
}


void LMK_clkoutCfgInit(const u32 *regdataSrc, lmk_config_t *lmkInstPtr)
{

	for(int i=0; i<7; i++) {
		/*********************************************
		 * Set s/dclkout power down flags
		 */
		lmkInstPtr->clkout[i].dclk_pwrdwn = 0;
		lmkInstPtr->clkout[i].sclk_pwrdwn = 0;

		// Is clkout powered down
		if( LMK_RegVal(LMK_DCLKoutX_1_PD, regdataSrc + (i*8)) ||
			(LMK_RegVal(LMK_DCLKoutx_FMT, regdataSrc + (i*8) ) == 0)) {
			lmkInstPtr->clkout[i].dclk_pwrdwn = 1;
		}

		// Is sclkout powered down
		if( LMK_RegVal(LMK_SDCLKoutY_PD, regdataSrc + (i*8)) ||
			(LMK_RegVal(LMK_SDCLKoutY_FMT, regdataSrc + (i*8) ) == 0)) {
			lmkInstPtr->clkout[i].sclk_pwrdwn = 1;
		}

		/*********************************************
		 * Set s/dclkout div and sclk src flags
		 */

		// determine div value for s/dclk output clk
		u32 clk_div;
		if( !(clk_div = LMK_RegVal(LMK_DCLKoutX_DIV, regdataSrc + (i*8))) ) {
			clk_div = 32;
		}

		// Set DCLK div dependent on output select
		// If bypass, set div to 1
		if( LMK_RegVal(LMK_DCLKoutX_MUX, regdataSrc + (i*8)) == 2) {
			lmkInstPtr->clkout[i].dclk_div = 1;
		} else {
			lmkInstPtr->clkout[i].dclk_div = clk_div;
		}

		// Set SCLK div dependent on output select
		// If selection is SYSREF, set div to 1
		if( LMK_RegVal(LMK_SDCLKoutY_MUX, regdataSrc + (i*8)) == SCLK_SRC_SYSREF) {
			lmkInstPtr->clkout[i].sclk_div = 1;
			lmkInstPtr->clkout[i].sclk_src = SCLK_SRC_SYSREF;
		} else {
			lmkInstPtr->clkout[i].sclk_div = clk_div;
			lmkInstPtr->clkout[i].sclk_src = SCLK_SRC_VC0_MUX;
		}
	}
}


void LMK_clkoutFreqInit(lmk_config_t *lmkInstPtr)
{

	for(int i=0; i<7; i++) {
		if(lmkInstPtr->clkout[i].dclk_pwrdwn == 1) {
			lmkInstPtr->clkout[i].dclk_freq = -1;
		} else {
			lmkInstPtr->clkout[i].dclk_freq = lmkInstPtr->PLL2_VCO_freq / lmkInstPtr->clkout[i].dclk_div;
		}

		if(lmkInstPtr->clkout[i].sclk_pwrdwn == 1) {
			lmkInstPtr->clkout[i].sclk_freq = -1;
		} else {
			if(lmkInstPtr->clkout[i].sclk_src == SCLK_SRC_SYSREF) {
				lmkInstPtr->clkout[i].sclk_freq = lmkInstPtr->sysref_freq / lmkInstPtr->clkout[i].sclk_div;
			} else {
				lmkInstPtr->clkout[i].sclk_freq = lmkInstPtr->PLL2_VCO_freq / lmkInstPtr->clkout[i].sclk_div;
			}
		}
	}

}


void LMK_intermediateDump(lmk_config_t *lmkInstPtr)
{

    // print parsed divider values
    xil_printf("pll1_clkin0_r_divider:%d\n\r", lmkInstPtr->clkin[0].div);
    xil_printf("pll1_clkin1_r_divider:%d\n\r", lmkInstPtr->clkin[1].div);
    xil_printf("pll1_clkin2_r_divider:%d\n\r", lmkInstPtr->clkin[2].div);
    xil_printf("pll1_n_divider:       %d\n\r", lmkInstPtr->pll1_n_divider);
    xil_printf("pll2_doubler:         %d\n\r", lmkInstPtr->pll2_doubler);
    xil_printf("pll2_r_divider:       %d\n\r", lmkInstPtr->pll2_r_divider);
    xil_printf("pll2_n_divider:       %d\n\r", lmkInstPtr->pll2_n_divider);
    xil_printf("SYSREF_div:           %d\n\r", lmkInstPtr->sysref_div);

    // print intermediate calculated values
    xil_printf("PLL2_Phase_Detector_Freq: %ld\n\r", lmkInstPtr->PLL2_Phase_Detector_Freq);
    xil_printf("PLL2_vco_fdbk_div: %ld\n\r", lmkInstPtr->PLL2_vco_fdbk_div);
    xil_printf("PLL1_Phase_Detector_Freq: %ld\n\r", lmkInstPtr->PLL1_Phase_Detector_Freq);
    xil_printf("PLL2_VCO_freq: %ld\n\r", lmkInstPtr->PLL2_VCO_freq);


    // print CLKout settings
	for(int i=0; i<7; i++) {
		xil_printf("DCLKout%02d: PD:%d DIV:%02d SCLKout%02d:PD:%d DIV:%02d SRC:%s\n\r",
				i * 2,
				lmkInstPtr->clkout[i].dclk_pwrdwn,
				lmkInstPtr->clkout[i].dclk_div,
				i * 2 + 1,
				lmkInstPtr->clkout[i].sclk_pwrdwn,
				lmkInstPtr->clkout[i].sclk_div,
				(lmkInstPtr->clkout[i].sclk_src == SCLK_SRC_VC0_MUX) ? "VCO_MUX" : "SYSREF" );
	}

}

