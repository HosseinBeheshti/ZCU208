clear;
close all;
clc;
sim_time = 1e-3;
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
dds1_phase_width = 16;
dds1_data_width = 12;
dds1_cm_width = 25;
dds1_cm_scale = -dds1_data_width+1;
dds1_convert_width = 14;
dds1_convert_point = 0;
demix_convert_width = 14;
demix_convert_point = 0;
dds2_phase_width = 16;
dds2_data_width = 12;
dds2_cm_width = 25;
dds2_cm_scale = -dds2_data_width+1;
dds2_convert_width = 14;
dds2_convert_point = 0;
duc_convert_width = 14;
duc_convert_point = 0;
dds3_phase_width = 16;
dds3_data_width = 12;
dds3_cm_width = 25;
dds3_cm_scale = -dds3_data_width+1;
dds3_convert_width = 14;
dds3_convert_point = 0;
gain_mult_width = 16;
gain_mult_point = 0;
