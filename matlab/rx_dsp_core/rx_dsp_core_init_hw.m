clear;
close all;
clc;
sim_time = 1e-3;
%%
fs = 4e9;
ts = 1/fs;
fc1 = 2.37e9;
fc2 = 2.4e9;
fc3 = 2.43e9;
%% DDC
downsample_ratio = 8;
f_ddc1 = 70e6;
f_ddc2 = 100e6;
f_ddc3 = 130e6;
%% filters
lpf1_coefficient = lpf_coefficient_gen(1);
lpf2_coefficient = lpf_coefficient_gen(0);
%% DUC
f_duc1 = 2.43e9;
f_duc2 = 2.4e9;
f_duc3 = 2.37e9;


