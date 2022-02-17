clear;
close all;
clc;
sim_time = 1e-7;
%%
fs = 500e6;
ts = 1/fs;
%% filter coefficient for dsp core
lpf1_coefficient = lpf1;
lpf2_coefficient = lpf2;
%% filter coefficient for channelized receiver
lpf3_coefficient = lpf3;
lpf4_coefficient = lpf4;
%% bit width
ddc_phase_width = 16;
ddc_data_width = 16;
