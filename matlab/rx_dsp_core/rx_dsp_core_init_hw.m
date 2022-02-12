clear;
close all;
clc;
sim_time = 1e-3;
%%
fs = 500e6;
ts = 1/fs;
%% Hardware parameters
ddc_phase_width = 16;
ddc_data_width = 16;
lpf1_outpu_width = 16;
lpf1_outpu_point = 0;
lpf2_outpu_width = 16;
lpf2_outpu_point = 0;
%% write xilinx coef files 
hq = lpf1; 
set(hq,'arithmetic','fixed'); 
coewrite(hq,10,'coef_filter1');
hq = lpf2; 
set(hq,'arithmetic','fixed'); 
coewrite(hq,10,'coef_filter2');

