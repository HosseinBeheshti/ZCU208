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
downsample_ratio = 8;
f_ddc1 = 70e6;
f_ddc2 = 100e6;
f_ddc3 = 130e6;
%% filter
% All frequency values are in MHz.
Fs = 250;  % Sampling Frequency

Fpass = 5;               % Passband Frequency
Fstop = 10;              % Stopband Frequency
Dpass = 0.057501127785;  % Passband Ripple
Dstop = 0.001;           % Stopband Attenuation
dens  = 20;              % Density Factor
% Calculate the order from the parameters using FIRPMORD.
[N, Fo, Ao, W] = firpmord([Fpass, Fstop]/(Fs/2), [1 0], [Dpass, Dstop]);
% Calculate the coefficients using the FIRPM function.
lpf1_coefficient = firpm(N, Fo, Ao, W, {dens});

