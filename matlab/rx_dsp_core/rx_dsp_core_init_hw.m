clear;
close all;
clc;
sim_time = 1e-7;
%%
fs = 500e6;
ts = 1/fs;
%% filter coefficient
lpf1_coefficient = lpf1;
lpf2_coefficient = lpf2;
ddc_phase_width = 16;
ddc_data_width = 16;
