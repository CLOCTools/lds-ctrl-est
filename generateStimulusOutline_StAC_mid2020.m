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

% ?? SwitchGate 


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

len=15e3
t = len2t(len);





trf = @(tp) [1-tp,tp;tp,1-tp];
frf = @(f1,f2) [1-f1,f1;1-f2,f2];

hmmDwellTime = 2;%in seconds
ptr_expt = (1/hmmDwellTime)*dt;
f1_expt = 20*dt;
f2_expt = 50*dt;

TR = trf(ptr_expt);
FR = frf(f1_expt,f2_expt);

stateGenFun = @(len, pTR) hmmgenerate(len, trf(pTR) , FR); 

state2gain = @(states,mag1,mag2) ((states-1)*(mag2-mag1)) + mag1;
stateWhiskerFun = @(len, states, mag1, mag2) whiskerNoiseFun(len,1).*state2gain(states,mag1,mag2);




[spks_,states_]  = stateGenFun(len, .5*dt);

figure(1)
clf
hold on
plot(t,sineFun(len,5,1),'g')
plot(t,optoNoiseFun(len,3)-5,'b')
plot(t,whiskerNoiseFun(len,.5)-10,'r')

plot(t,spks_-15,'k','LineWidth',1)
plot(t,states_-14.5,'g','LineWidth',3)

plot(t, stateWhiskerFun(len, states_, .1,.5)-17,'r')


hold off


%% construct dictionaries
array2cell = @(ary) arrayfun(@(a) {a}, ary);
buildConditionCell = @(cNameAry) array2cell(struct('conditionName',cNameAry));

ChannelMap = containers.Map()
ChannelMap('whisk') = 5;
ChannelMap('opto') = 6;

ColorsMap = containers.Map()
ColorsMap('whisk') = [1,.5,.5];
ColorsMap('opto') = [.3,.5,1];

fillStruct = @(Obj) fillColorChannel(Obj, ColorsMap, ChannelMap);

%% construct condition-specific dictionaries
offStruct = struct('function',fOFF);

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
ConditionStructMap('OFF') = offStruct;
ConditionStructMap('whisk_ON') = whiskStruct;
ConditionStructMap('whiskSweep_ON') = whiskSweepStruct;
ConditionStructMap('optoNoise_ON') = optoNoiseStruct;


getColor = @(name) ColorsMap(ConditionStructMap(name).type);


%% Phase 1A: Whisker Calibration

cond_1a = {'whiskSweep_ON'}
w_mags_sweep = num2cell([1,2,3,4,5]/5); 

%binding conditions and params should be different than binding conditions?
sweep = struct('conditionName',cond_1a,'params',w_mags_sweep);
sweepConditions = array2cell(sweep);

%%
Phase1A.title = 'Phase 1A: Whisker Calibration Sweep';
Phase1A.times = [0.5, 5.0, 0.5];
Phase1A.times = [2, 2, 2];

Phase1A.trialLength = sum(Phase1A.times)
Phase1A.nreps = 10;
Phase1A.dt = dt;

Phase1A.conditions = sweepConditions;
Phase1A.nCond = length(sweepConditions);
[ Phase1A ] = generateStimuli( Phase1A, ConditionStructMap ,ChannelMap,ColorsMap);

figure(2)
clf
plotExptPhase(Phase1A)

set(gcf,'Position',[144   563   676   242])

%% Phase 1B: Identification stimulus
Phase1B.title = 'Phase 1B: Whisker & Opto Identification Stimulus';

whisk_cond = buildConditionCell({'whisk_OFF','whisk_ON'});
opto_noise_cond = buildConditionCell({'optoNoise_OFF','optoNoise_ON'});
Phase1B.options = {opto_noise_cond, whisk_cond};


TimeMap1B = containers.Map();
TimeMap1B('whisk') = [0.1, 5.8 , 0.1];
TimeMap1B('opto') = [1, 4, 1];

Phase1B.times = TimeMap1B;%[0.5, 5.0, 0.5];
Phase1B.trialLength = sum(TimeMap1B('whisk'));

Phase1B.nreps = 10;
Phase1B.dt = dt;

[ Phase1B.conditions, Phase1B.nCond ] = combineConditionsMulti(Phase1B.options);
[ Phase1B ] = generateStimuli( Phase1B, ConditionStructMap ,ChannelMap,ColorsMap);

figure(3)
clf
plotExptPhase(Phase1B)
set(gcf,'Position',[   144   171   676   318])


%% Phase 2A: Control


nReroll = 0
nTransition = 0
nTrExpected = 4
stateSeqLen = 7.5e3;
state_pTR = .5*dt;

while ((nTransition < nTrExpected) && (nReroll < 100))
    %"rerolls the dice" until you get a genuine HMM state sequence with
    %"enough" transitions
    nReroll = nReroll+1;

    [~,frozenStateSeq] = stateGenFun(stateSeqLen, state_pTR);
    nTransition = sum(abs(diff(frozenStateSeq)));
end

frozenWhiskerSeqFun = @(mags) stateWhiskerFun(stateSeqLen, frozenStateSeq, mags(1), mags(2));


figure(100)
clf
hold on
plot(t(1:stateSeqLen), frozenWhiskerSeqFun([1,2]/5),'r')
plot(t(1:stateSeqLen), frozenStateSeq-1,'k:')

title(nReroll)


























