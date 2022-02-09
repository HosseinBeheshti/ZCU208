function lpf_coefficient = lpf_coefficient_gen(i)
% Returns a discrete-time filter object.
% All frequency values are normalized to 1.
if i == 1
Fpass = 0.08;            % Passband Frequency
Fstop = 0.125;           % Stopband Frequency
Dpass = 0.057501127785;  % Passband Ripple
Dstop = 0.01;            % Stopband Attenuation
dens  = 20;              % Density Factor

% Calculate the order from the parameters using FIRPMORD.
[N, Fo, Ao, W] = firpmord([Fpass, Fstop], [1 0], [Dpass, Dstop]);

% Calculate the coefficients using the FIRPM function.
lpf_coefficient  = firpm(N, Fo, Ao, W, {dens});
else
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
lpf_coefficient = firpm(N, Fo, Ao, W, {dens});
end

% [EOF]
