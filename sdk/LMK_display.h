
#ifndef __LMK_DISPLAY_H_
#define __LMK_DISPLAY_H_


// PLL1 VCXO freq is fixed to external VCXO freq for zcu216
#define VCXO_FREQ 160000000

#define LMK_PLL2_NCLK_MUX_PLL2_P  0
#define LMK_FB_MUX_DCLKout6       0
#define LMK_FB_MUX_DCLKout8       1
#define LMK_FB_MUX_SYSREF_DIVIDER 2
#define LMK_FB_MUX_EXTERNAL       3
#define LMK_PLL1_NCLK_MUX_OSCIN   0

#define LMK_CLKin_SEL_MODE_CLKIN0_MANUAL 0
#define LMK_CLKin_SEL_MODE_CLKIN1_MANUAL 1
#define LMK_CLKin_SEL_MODE_CLKIN2_MANUAL 2
#define LMK_CLKin_SEL_MODE_PIN_SEL_MODE  3
#define LMK_CLKin_SEL_MODE_AUTO_MODE     4

#define SCLK_SRC_VC0_MUX 0
#define SCLK_SRC_SYSREF  1


typedef struct reg_def_struct {
	unsigned char mask;
	unsigned char shift;
	unsigned char offset;
	u32           addr;
} reg_def_t;

typedef enum {
	LMK_DCLKoutX_1_PD = 0 ,
	LMK_DCLKoutx_FMT  ,
	LMK_DCLKoutX_MUX  ,
	LMK_DCLKoutX_DIV  ,
	LMK_SDCLKoutY_PD ,
	LMK_SDCLKoutY_FMT,
	LMK_SDCLKoutY_MUX,
	LMK_SYSREF_DIV_h,
	LMK_SYSREF_DIV_l,
	LMK_PLL2_N_h,
	LMK_PLL2_N_m,
	LMK_PLL2_N_l,
	LMK_PLL2_NCLK_MUX,
	LMK_FB_MUX_EN,
	LMK_FB_MUX,
	LMK_PLL2_R_h,
	LMK_PLL2_R_l,
	LMK_PLL2_REF_2X_EN,
	LMK_PLL1_NCLK_MUX,
	LMK_PLL1_N_h,
	LMK_PLL1_N_l,
	LMK_CLKin_SEL_MODE,
	LMK_CLKin0_R_h,
	LMK_CLKin0_R_l,
	LMK_CLKin1_R_h,
	LMK_CLKin1_R_l,
	LMK_CLKin2_R_h,
	LMK_CLKin2_R_l,
	LMK_CLKin0_EN,
	LMK_CLKin1_EN,
	LMK_CLKin2_EN,
	LMK_VCO_MUX,
	LMK_PLL2_P,
} LMK_regNames_t;


typedef struct lmk_clkout_struct {
	unsigned char dclk_pwrdwn;
	unsigned char sclk_pwrdwn;
	unsigned char sclk_src;
	int           dclk_div;
	int           sclk_div;

	long int      dclk_freq;
	long int      sclk_freq;
} lmk_clkout_t;


typedef struct lmk_clkin_struct {
	unsigned char en;
	int           div;
	long int      freq;
} lmk_clkin_t;


typedef struct lmk_config_struct {
	int sysref_div;

	int pll1_n_divider;
	int pll2_n_prescaler;
	int pll2_n_divider;
	int pll2_r_divider;
	int pll2_doubler;

	lmk_clkin_t   clkin[3];
	lmk_clkout_t  clkout[7];
	int clkin_sel_mode;

	long int PLL2_Phase_Detector_Freq;
	long int PLL2_vco_fdbk_div;
	long int PLL1_Phase_Detector_Freq;
	long int PLL2_VCO_freq;
	long int sysref_freq;

} lmk_config_t;


void LMK_clkinAndPLLInit(const u32 *regdataSrc, lmk_config_t *lmkInstPtr);
void LMK_sysrefInit(const u32 *regdataSrc, lmk_config_t *lmkInstPtr);
void LMK_clkoutCfgInit(const u32 *regdataSrc, lmk_config_t *lmkInstPtr);
void LMK_intermediateDump(lmk_config_t *lmkInstPtr);
void LMK_init(const u32 *regdataSrc, lmk_config_t *lmkInstPtr);
void LMK_clkoutFreqInit(lmk_config_t *lmkInstPtr);


#endif
