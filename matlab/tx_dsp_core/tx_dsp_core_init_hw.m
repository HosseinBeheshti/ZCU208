clear;
close all;
clc;
sim_time = 5e-7;
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
%% bit width
adc_width = 16;
arcsine_rom_value = asin(0:1/2^(adc_width-1):1)./asin(1);
dds_phase_width = 16;
dds_data_width = 16;