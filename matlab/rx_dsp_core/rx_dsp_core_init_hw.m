clear;
close all;
clc;
sim_time = 1e-3;
%%
fs = 500e6;
ts = 1/fs;
%% filters
lpf1_coefficient = lpf_coefficient_gen(1);
lpf2_coefficient = lpf_coefficient_gen(0);
%% Hardware parameters
ddc_phase_width = 16;
ddc_data_width = 16;
lpf1_outpu_width = 16;
lpf1_outpu_point = 0;
lpf2_outpu_width = 16;
lpf2_outpu_point = 0;




