Fc = 2.4e9;               % Carrier frequency (Hz)
Rsym = 11e6;             % Symbol rate (symbols/second)
N = 512;                   % Number of samples per symbol
frameLength = 1000;       % Number of symbols in a frame
M = 16;                   % Modulation order (16-QAM)
EbNo = 100;               % Ratio of baseband bit energy
Fc1 = 90e6;               %frequency of local oscillator
a = 1;                    %amplitude of LO
Fs = Rsym * N;            %sampling frequency in Hz


% constellationdiagram specify the refrence point and axis limit%
constDiag = comm.ConstellationDiagram('SamplesPerSymbol', 1, ...
'XLimits', [-4.5 4.5], ...
'YLimits', [-4.5 4.5], ...
'Position', [70 560 640 460]);

% 16-QAM modulator.
qam16Mod = comm.RectangularQAMModulator(M);

% Set the expected constellation of the constellation diagram.
constDiag.ReferenceConstellation = constellation(qam16Mod);

% random data symbols.
b1 = randi([0 M-1], frameLength, 1);
b2= randi([0 M-1], frameLength, 1);
txSym1 = qam16Mod(b1);
txSym2 = qam16Mod(b2);
scatterplot(txSym1)
text(real(txSym1)+0.1, imag(txSym1), dec2bin(b1))
title('16-QAM, Binary Symbol Mapping')
axis([-4 4 -4 4])
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%design filter

Nsym=10;
fltDelay = Nsym / (2* Rsym);
beta=1; 
sampsPerSym=N;

% Design raised cosine filter with given order in symbols
rctFilt = comm.RaisedCosineTransmitFilter(...
  'Shape',                  'Square root', ...
  'RolloffFactor',          beta, ...
  'FilterSpanInSymbols',    Nsym, ...
  'OutputSamplesPerSymbol', sampsPerSym);

% Upsample and filter. MIMO1 and MIMO2 baseband
yb1 = rctFilt([txSym1; zeros(Nsym/2,1)]);
yb2 = rctFilt([txSym2; zeros(Nsym/2,1)]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%frequency upconvert, MIMO1 and MIMO2 
% Generating carrier. The sqrt(2) factor is to ensures that the power of the
t = (0:1/Fs:((frameLength+Nsym/2)/Rsym)-1/Fs).';
carrier = exp(1i*2*pi*Fc*t);
yb1u=yb1.*carrier;
yb2u=yb2.*carrier;
yb2ur= real(yb2u)
 pwelch(yb2ur,[],[],[],Fs);
 dlmwrite('upsignal.csv',yb2ur);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%local oscillator equation%
x4 = a*cos(2*pi*Fc1*t);
x5 = a*cos((2*pi*Fc1*t)+pi/4);
%multiply modulated upconverted signal1 with LO
yb2l=yb2ur.*x4;
% pwelch(yb2l,[],[],[],Fs);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%sum two signals
ys=yb1u+yb2l;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%bandpass filter design%
bpFilt = designfilt('bandpassfir','FilterOrder',20, ...
         'CutoffFrequency1',2.44e9,'CutoffFrequency2',2.46e9, ...
         'SampleRate',Fs);
fvtool(bpFilt)
filter_in=[ys; zeros(10, 1)];
filter_out = filter(bpFilt,filter_in);
mimo1=filter_out(11:end);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%frequency downconvert
fdmimo1=mimo1.*conj(carrier);
% Design and raise cosine normalize filter in receiver
rcrFilt = comm.RaisedCosineReceiveFilter(...
  'Shape',                  'Square root', ...
  'RolloffFactor',          beta, ...
  'FilterSpanInSymbols',    Nsym, ...
  'InputSamplesPerSymbol',  sampsPerSym, ...
  'DecimationFactor',       1);
% Filter at the receiver.
yr = rcrFilt([fdmimo1; zeros(Nsym*sampsPerSym/2, 1)]);

% Correct for propagation delay by removing filter transients

REC_data3 = yr(1:N:end);
afmimoDownA3 = REC_data3(11:end);


% demodulator.
qam16Demod = comm.RectangularQAMDemodulator(M);

xHatA3 = qam16Demod(afmimoDownA3);
numErr3 = sum(xHatA3~=b1)