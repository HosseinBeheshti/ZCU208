
#ifndef __LMX_DISPLAY_H_
#define __LMX_DISPLAY_H_


#define LMX_OUTA_MUX_CHAN_DIV 0
#define LMX_OUTA_MUX_VCO      1
#define LMX_OUTA_MUX_PD       3

#define LMX_OUTB_MUX_CHAN_DIV 0
#define LMX_OUTB_MUX_VCO      1
#define LMX_OUTB_SYSREF       2
#define LMX_OUTB_MUX_PD       3


typedef struct lmx_reg_def_struct {
	unsigned int  mask;
	unsigned char shift;
	unsigned char offset;
	u32           addr;
} lmx_reg_def_t;

typedef enum{
	LMX_OSC_2X = 0,
	LMX_PLL_R_PRE,
	LMX_MULT,
	LMX_PLL_R,
	LMX_PLL_N_l,
	LMX_PLL_N_h,
	LMX_PLL_NUM_l,
	LMX_PLL_NUM_h,
	LMX_PLL_DEN_l,
	LMX_PLL_DEN_h,
	LMX_CHDIV,
	LMX_OUTA_MUX,
	LMX_OUTB_MUX,
	LMX_SYSREF_DIV_PRE,
	LMX_SYSREF_DIV,
	LMX_OUTA_PD,
	LMX_OUTB_PD,
} LMX_regNames_t;

typedef struct lmx_config_struct {

	char osc_doubler_multiplier;
	int pll_preR_div;
	int pll_multiplier;
	int pll_r_divider;
	int pll_n_divider;
	long int pll_fraction_numerator;
	long int pll_fraction_denominator;
	int channel_div;
	int sysref_pre_div;
	int sysref_div;

	long int Fpd_freq;
	long int VCO_freq;
	long int chandiv_freq;
	long int sysref_freq;
	long int RFoutA_freq;
	long int RFoutB_freq;

} lmx_config_t;


void LMX_intermediateDump(lmx_config_t *lmxInstPtr);
void LMX_SettingsInit(long int inFreq, const u32 *regdataSrc, lmx_config_t *lmxInstPtr);


#endif
