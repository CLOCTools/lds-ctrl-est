clc
clear
close all
set(0,'DefaultLineLineWidth',2)
%% TIMES ARE IN SECONDS BY DEFAULT
dt = 1e-3;
ms2samples = @(t_ms) floor((t_ms/dt)*(1e-3));
sec2samples = @(t_ms) floor((t_ms/dt));


%% Filler functions i'd want to construct stimulu file

%default params
preTimeDefault = 0.5;
postTimeDefault = 0.5;

%bufferSegmentTimes(preTime, segement, postTime);
%defualtBufferSegment = @(segment) bufferSegmentTimes(preTimeDefault, segment, postTimeDefault);


%% Channel definitions - pulled from 2020.01.15.MBm04_awake3

% ? trigger (002 trig0)
% ? trigger (003 trig1)
% ? trigger (004 trig2)

% uWhisk (005 ugalv)
% uOpto (006 uopto)

% FR target (010 yRef)

%Thesse are one 4D vector, {0,1}

% ? Feedback Ctrl Gate (012 gate 0)
% ? Feedback Estim. Gate (013 gate1)
% ? Lock Stim Gate (014 gate2)
% ? Add Noise Gate (015 gate3)


%opto_noise ON (implies)-> CL gate OFF
%%
i2t = @(i) (i-1)*dt;
len2t = @(len) 0:dt:((len-1)*dt);

fOFF = @(len,params) zeros(1,len);

wClip = [-5,5]
clipFun = @(x,xmin,xmax) min(max(x,xmin),xmax);

optoNoiseMag = 3;
optoNoiseFun = @(len,mag) mag*rand(1,len);
optoNoiseFunDefault = @(len, ~) optoNoiseFun(len, optoNoiseMag);

whiskerNoiseMag = .1;
whiskerNoiseFun = @(len,mag) clipFun(mag*randn(1,len), wClip(1),wClip(2));
whiskerNoiseFunDefault = @(len, ~) whiskerNoiseFun(len,whiskerNoiseMag);

sinRaised = @(t) (sin(t)+1)/2;
sineFun = @(len,mag,freq) mag*sinRaised(2*pi*freq*len2t(len));

len=500
t = len2t(len);

figure(1)
clf
hold on
plot(t,sineFun(len,5,1),'g')
plot(t,optoNoiseFun(len,3),'b')
plot(t,whiskerNoiseFun(len,.1),'r')

hold off

%% construct dictionaries
ChannelMap = containers.Map()
ChannelMap('whisk') = 5;
ChannelMap('opto') = 6;

ColorsMap = containers.Map()
ColorsMap('whisk') = [1,.5,.5];
ColorsMap('opto') = [.3,.5,1];

fillStruct = @(Obj) fillColorChannel(Obj, ColorsMap, ChannelMap);

%% construct condition-specific dictionaries
whiskSweepStruct.function = whiskerNoiseFun;
whiskSweepStruct.type = 'whisk';
whiskSweepStruct = fillStruct(whiskSweepStruct);

whiskStruct.function = whiskerNoiseFunDefault
whiskStruct.type = 'whisk'
whiskStruct = fillStruct(whiskStruct);

optoNoiseStruct.function = optoNoiseFunDefault;
optoNoiseStruct.type = 'opto';
optoNoiseStruct = fillStruct(optoNoiseStruct);



ConditionStructMap = containers.Map()
ConditionStructMap('whisk_ON') = whiskStruct;
ConditionStructMap('whiskSweep_ON') = whiskSweepStruct;
ConditionStructMap('optoNoise_ON') = optoNoiseStruct;



ConditionMap = containers.Map()
ConditionMap('OFF') = fOFF;
ConditionMap('whisk_ON') = whiskerNoiseFunDefault;
ConditionMap('whiskSweep_ON') = whiskerNoiseFun;
ConditionMap('optoNoise_ON') = optoNoiseFunDefault;


%{
ColorsMap = containers.Map()
ColorsMap('whisk') = [1,.5,.5]*.9;
ColorsMap('whiskSweep') = [1,.5,.5]*.9;
ColorsMap('optoNoise') = [.3,.5,1]*.9;
%}

%% Phase 1A: Whisker Calibration
array2cell = @(ary) arrayfun(@(a) {a}, ary);

cond_1a = {'whiskSweep_ON'}
w_mags_sweep = num2cell([1,2,3,4,5]/5); 

%binding conditions and params should be different than binding conditions?
sweep = struct('conditionName',cond_1a,'params',w_mags_sweep);
sweepConditions = array2cell(sweep);

%%


%Phase1A.params = w_mags_sweep;
%Phase1A.options = {cond_1a, w_mags_sweep}

Phase1A.times = [0.5, 5.0, 4. ];
Phase1A.nreps = 10;
Phase1A.dt = dt;

% end of user-entered options, code below is intended to be functionalized
% to work for lots of scenarios
%[ Phase1A.conditions, Phase1A.nCond ] = combineConditionsMulti(Phase1A.options)
Phase1A.conditions = sweepConditions;
Phase1A.nCond = length(sweepConditions);
[ Phase1A ] = generateStimuli( Phase1A, ConditionMap );


figure(2)
clf
subplot(2,1,1)
hold on
title('Phase 1A : Whisker Calibration - [single trial]')
for i = 1:Phase1A.nCond
    plot(Phase1A.t{i}, Phase1A.segments{i})
end

subplot(2,1,2)
plot(Phase1A.fullt, Phase1A.fullSegment,'LineWidth',1,'Color','r')
title('[all trials]')

%% Phase 1B: Identification stimulus

%how params are being used here is different than above 

buildConditionCell = @(cNameAry) array2cell(struct('conditionName',cNameAry));

whisk_cond = buildConditionCell({'whisk_OFF','whisk_ON'});
opto_noise_cond = buildConditionCell({'optoNoise_OFF','optoNoise_ON'});
Phase1B.options = {whisk_cond, opto_noise_cond}

Phase1B.times = [0.5, 5.0, 0.5];
Phase1B.nreps = 10;
Phase1B.dt = dt;

% end of user-entered options, code below is intended to be functionalized
% to work for lots of scenarios
[ Phase1B.conditions, Phase1B.nCond ] = combineConditionsMulti(Phase1B.options)
[ Phase1B ] = generateStimuli( Phase1B, ConditionMap );


figure(3)
clf
subplot(2,1,1)
hold on
for i = 1:Phase1B.nCond
    plot(Phase1B.t{i},Phase1B.segments{i})
end

subplot(2,1,2)
hold on
plot(Phase1B.fullt, Phase1B.fullSegment,'b')

for i = 1:Phase1B.nCond
    t_ = Phase1B.t{i};
    plot(t_(end),1+i/10,'ko')
end


%{


% {{'whisk',true},{'whisk',false} x {{'opto_noise',false},{'opto_noise',true}}
[WO,nWO] = combineConditions(whisk_cond, opto_noise_cond);

[conditions_phase1, nCond_phase1] = combineConditionsMulti({whisk_cond, opto_noise_cond})


TrialLength_min_phase1 = nCond_phase1 * reps_phase1 * sum(times_phase1)/60;

sprintf('Phase 1 is projected to take %.1f minutes, with %i trials of %i conditions',TrialLength_min_phase1, reps_phase1, nCond_phase1)
%}
