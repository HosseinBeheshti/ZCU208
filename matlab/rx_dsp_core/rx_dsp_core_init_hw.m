clear;
close all;
clc;
sim_time = 5e-6;
%%
fs = 500e6;
ts = 1/fs;
iladata = importfile("iladata.csv", [3, Inf]);
t = 0:ts:ts*1023;
adc_data_array = table2array(iladata(:,4:11));
dac_data_array = table2array(iladata(:,12:19));
adc_data = reshape(adc_data_array',1,[])';
t_ws = 0:ts/8:(ts/8*8191);
workspace_adc = [t_ws' adc_data];
dac_data = reshape(dac_data_array',1,[])';
%% filter coefficient for dsp core
lpf1_coefficient = lpf1;
lpf2_coefficient = lpf2;
%% filter coefficient for channelized receiver
lpf3_coefficient = lpf3;
lpf4_coefficient = lpf4;
%% bit width
adc_width = 16;
gain_mult_width = 16;
gain_mult_point = 15;
dds1_phase_width = 16;
dds1_data_width = 16;
dds1_cm_width = 33;
dds1_convert_width = 16;
dds1_convert_point = 15;
dds2_phase_width = 16;
dds2_data_width = 16;
dds2_cm_width = 33;
dds2_convert_width = 16;
dds2_convert_point = 15;
lpf3_convert_width = 16;
lpf3_convert_point = 15;
dds3_phase_width = 16;
dds3_data_width = 16;
lpf4_convert_width = 16;
lpf4_convert_point = 15;
dds3_cm_width = 33;
dds3_cm_scale = -dds3_data_width+1;
dds3_convert_width = 16;
dds3_convert_point = 15;
