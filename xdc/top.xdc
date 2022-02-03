####################################################################################
create_clock -period 2.000 -name CC_PL_CLK_clk -waveform {0.000 1.000} [get_ports {pl_clk_clk_p[0]}]
# Max Delays from CC_PL_CLK_clk (400MHz) to clk_out1_design_1_clk_wiz_0_2 (500MHz)
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_adc*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_mts_sysref_count_adc/sysref_fbrc_ff*/D"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_adc*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_mts_sysref_count_adc/sysref_freq_done_ff*/D"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_adc*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_mts_sysref_count_adc/sysref_freq_strt_ff*/D"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_adc*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_rf_conv_mt_mrk_counter_adc*/mrk_cntr_ff*/CE"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_adc*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_rf_conv_mt_mrk_counter_adc*/mrk_cntr_started_ff*/D"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_adc*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_rf_conv_mt_mrk_counter_adc*/sysref_sync_dly_ff*/D"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_dac*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_mts_sysref_count_dac/sysref_fbrc_ff*/D"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_dac*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_mts_sysref_count_dac/sysref_freq_done_ff*/D"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_dac*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*i_mts_sysref_count_dac/sysref_freq_strt_ff*/D"}] 2.000
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_dac*/C"}] -to [get_pins -hierarchical -filter {NAME =~ "*sysref_dac_ff*/D"}] 2.000
# Max Delays from CC_PL_CLK_clk (400MHz) to clk_out1_design_1_clk_wiz_0_2 (500MHz)
set_max_delay -datapath_only -from [get_pins -hierarchical -filter {NAME =~ "*user_sysref_adc*/C"}] -to [get_cells -hierarchical -filter {IS_SEQUENTIAL == TRUE && NAME =~ "*ila_core_inst*"}] 2.000
# Input Delay for PL SYSREF
set_input_delay -clock [get_clocks CC_PL_CLK_clk] -min -add_delay 2.000 [get_ports pl_sysref_clk_p]
set_input_delay -clock [get_clocks CC_PL_CLK_clk] -max -add_delay 2.031 [get_ports pl_sysref_clk_p]


set_property PACKAGE_PIN C11 [get_ports {clk104_clk_spi_mux_sel_tri_o[0]}]
set_property PACKAGE_PIN B12 [get_ports {clk104_clk_spi_mux_sel_tri_o[1]}]
set_property IOSTANDARD LVCMOS12 [get_ports {clk104_clk_spi_mux_sel_tri_o[1]}]
set_property IOSTANDARD LVCMOS12 [get_ports {clk104_clk_spi_mux_sel_tri_o[0]}]

set_property PACKAGE_PIN A9 [get_ports Send_Play]
set_property PACKAGE_PIN A6 [get_ports Send_Cap]
set_property PACKAGE_PIN E7 [get_ports Ext_Play]
set_property PACKAGE_PIN A5 [get_ports Ext_Capture]
set_property IOSTANDARD LVCMOS18 [get_ports Ext_Capture]
set_property IOSTANDARD LVCMOS18 [get_ports Ext_Play]
set_property IOSTANDARD LVCMOS18 [get_ports Send_Play]
set_property IOSTANDARD LVCMOS18 [get_ports Send_Cap]
set_property DRIVE 8 [get_ports Send_Cap]
set_property DRIVE 8 [get_ports Send_Play]
set_property SLEW SLOW [get_ports Send_Cap]
set_property SLEW SLOW [get_ports Send_Play]
set_property IOSTANDARD LVDS_25 [get_ports pl_sysref_clk_p]
set_property IOSTANDARD LVDS_25 [get_ports pl_sysref_clk_n]
set_property PACKAGE_PIN B10 [get_ports pl_sysref_clk_p]
set_property PACKAGE_PIN B8 [get_ports {pl_clk_clk_p[0]}]
set_property IOSTANDARD LVDS_25 [get_ports {pl_clk_clk_p[0]}]
set_property IOSTANDARD LVDS_25 [get_ports {pl_clk_clk_n[0]}]

# Control clocking related to PL SYSREF
set_property LOC BUFGCE_HDIO_X0Y2 [get_cells {design_1_i/util_ds_buf_1/U0/USE_BUFGCE.GEN_BUFGCE[0].BUFGCE_U}]
set_property LOC MMCM_X0Y3 [get_cells design_1_i/clk_wiz_0/inst/mmcme4_adv_inst]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {design_1_i/util_ds_buf_1/U0/BUFGCE_O[0]}]

# Lock PL SYSREF capture FF
set_property BEL BFF2 [get_cells design_1_i/pl_sysref_0/inst/pl_sysref_r_reg]
set_property LOC SLICE_X107Y216 [get_cells design_1_i/pl_sysref_0/inst/pl_sysref_r_reg]
# Lock routing from PL SYSREF input pin to capture FFs

####################################################################################
set_property PACKAGE_PIN F6 [get_ports {clk_sync_tri_io[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {clk_sync_tri_io[0]}]
set_property SLEW FAST [get_ports {clk_sync_tri_io[0]}]
set_property DRIVE 8 [get_ports {clk_sync_tri_io[0]}]
####################################################################################
