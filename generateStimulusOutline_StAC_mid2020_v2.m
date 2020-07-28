clc
clear
close all
set(0,'DefaultLineLineWidth',2)
set(0,'DefaultAxesFontSize',15)
%% TIMES ARE IN SECONDS BY DEFAULT
rng_seed = randi([1,1e3]); %   990,878,848

rng(rng_seed);
    
doSaveAllData = false; %whether to save more data than is sent to rtxi

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



%opto_noise ON (implies)-> CL gate OFF
%%
i2t = @(i) (i-1)*dt;
len2t = @(len) 0:dt:((len-1)*dt);
t2i = @(t) round(t/dt);


fOFF = @(len,params) zeros(1,len);
shuffle = @(vec) vec(randperm(numel(vec)));

wClip = [-5,5]
clipFun = @(x,xmin,xmax) min(max(x,xmin),xmax);


optoNoiseMag = 1;
optoNoiseFun = @(len,mag) mag*rand(1,len);
optoNoiseFunDefault = @(len, ~) optoNoiseFun(len, optoNoiseMag);

whiskerNoiseMag = .5;
whiskerNoiseLowPassFreq = 200;
lowpassFun = @(vec) lowpass(vec, whiskerNoiseLowPassFreq, 1/dt);

whiskerNoiseFun = @(len,mag) clipFun(mag*randn(1,len), wClip(1),wClip(2));
whiskerNoiseFunDefault = @(len, ~) whiskerNoiseFun(len,whiskerNoiseMag);

sinRaised = @(t) (-cos(t)+1)/2;
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
ChannelMap('triggers') = 2;

ChannelMap('whisk') = 5;
ChannelMap('opto') = 7;

ChannelMap('yRef') = 10;

ChannelMap('fbCtrl') = 12;%12
ChannelMap('fbEstim') = 13+1;%13

%Fake:
ChannelMap('stateOverride') = 21;

ChannelMap('doOverrideState') = 25;

ChannelMap('decodedState') = 17;
ChannelMap('trueState') = 19;



channelVals = ChannelMap.values;
channelKeys = ChannelMap.keys;

[channelVals,sortIdx_] = sort([channelVals{:}]);
channelKeys = {channelKeys{sortIdx_}};

nChannels = max(channelVals);



ColorsMap = containers.Map()
ColorsMap('whisk') = [1,.5,.5];
ColorsMap('opto') = [.3,.5,1];
ColorsMap('triggers') = [1,1,1]*.3;


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
%{
Phase1A.title = 'Phase 1A: Whisker Calibration Sweep';
%Phase1A.times = [0.5, 5.0, 0.5];
Phase1A.times = [.9,1,.1];

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

return
%}



Phase1A.title = 'Phase 1A: Whisker & Opto Calibration Stimulus';

whisk_cond = buildConditionCell({'whisk_OFF','whisk_ON'});
opto_noise_cond = buildConditionCell({'optoNoise_OFF','optoNoise_ON'});
Phase1A.options = {opto_noise_cond, whisk_cond};

TimeMap1A = containers.Map();
TimeMap1A('whisk') = [1.0, 1.0 , 1.0];
TimeMap1A('opto') = [1.0, 1.0 , 1.0];

Phase1A.times = TimeMap1A;%[0.5, 5.0, 0.5];
Phase1A.trialLength = sum(TimeMap1A('whisk'));

Phase1A.nreps = 25;
Phase1A.dt = dt;
Phase1A.ChannelMap = ChannelMap;

[ Phase1A.conditions, Phase1A.nCond ] = combineConditionsMulti(Phase1A.options);
[ Phase1A ] = generateStimuli( Phase1A, ConditionStructMap ,ChannelMap,ColorsMap);

P1A_whisk = Phase1A.fullSegment(2,:);
P1A_whisk_lowpass = lowpassFun(P1A_whisk);
Phase1A.fullSegment(2,:) = P1A_whisk_lowpass;



P1A_whiskerRescale = shuffle(linspace(whiskerNoiseMag/3, whiskerNoiseMag*3 , Phase1A.nreps));
onesSub = ones(1,t2i(Phase1A.trialLength)*Phase1A.nCond);

rescaleTime = [];
for i =1:Phase1A.nreps
    rescaleTime = [rescaleTime, onesSub*P1A_whiskerRescale(i)];
end

P1A_trigSeg = 0*onesSub;
P1A_trigSeg(1) = 1;
P1A_triggers = repmat(P1A_trigSeg,[1,Phase1A.nreps]);


Phase1A.fullSegment(2,:) = Phase1A.fullSegment(2,:).*rescaleTime;
Phase1A.whiskerScale = rescaleTime;



Phase1A.StimBlock = zeros(nChannels,size(Phase1A.fullSegment,2));

Phase1A.StimBlock(ChannelMap('triggers'),:) = P1A_triggers;
Phase1A.StimBlock(ChannelMap('opto'),:) = Phase1A.fullSegment(1,:);
Phase1A.StimBlock(ChannelMap('whisk'),:) = Phase1A.fullSegment(2,:);


figure(2)
clf
plotExptPhase(Phase1A)
hold on
plot(Phase1A.fullt*dt,Phase1A.whiskerScale+5,'k:')
set(gcf,'Position',[   144   171   676   318])

figure(22)
clf
plotStimBlock(Phase1A)

Phase1A = savePhase2MatStim(Phase1A);
save('Phase1A_stimProtocol.mat','-struct','Phase1A')

%% Phase 1B: Identification stimulus (add probe stim to end of this?)

Phase1B.title = 'Phase 1B: Whisker & Opto Identification Stimulus';

whisk_cond = buildConditionCell({'whisk_OFF','whisk_ON'});
opto_noise_cond = buildConditionCell({'optoNoise_OFF','optoNoise_ON'});
Phase1B.options = {opto_noise_cond, whisk_cond};

TimeMap1B = containers.Map();
TimeMap1B('whisk') = [4.0, 5 , 1.0];
TimeMap1B('opto') = [4.0, 5, 1.0];

Phase1B.times = TimeMap1B;%[0.5, 5.0, 0.5];
Phase1B.trialLength = sum(TimeMap1B('whisk'));

Phase1B.nreps = 25;
Phase1B.dt = dt;
Phase1B.ChannelMap = ChannelMap;

[ Phase1B.conditions, Phase1B.nCond ] = combineConditionsMulti(Phase1B.options);
[ Phase1B ] = generateStimuli( Phase1B, ConditionStructMap ,ChannelMap,ColorsMap);

P1B_whisk = Phase1B.fullSegment(2,:);
P1B_whisk_lowpass = lowpassFun(P1B_whisk);
Phase1B.fullSegment(2,:) = P1B_whisk_lowpass;


P1B_trigSeg = zeros(1,t2i(Phase1B.trialLength)*Phase1B.nCond);
P1B_trigSeg(1) = 1;
P1B_triggers = repmat(P1B_trigSeg,[1,Phase1B.nreps]);

Phase1B.StimBlock = zeros(nChannels,size(Phase1B.fullSegment,2));
Phase1B.StimBlock(ChannelMap('opto'),:) = Phase1B.fullSegment(1,:);
Phase1B.StimBlock(ChannelMap('whisk'),:) = Phase1B.fullSegment(2,:);
Phase1B.StimBlock(ChannelMap('triggers'),:) = P1B_triggers;



figure(121)
clf
hold on
plot(P1B_whisk,'r')
plot(P1B_whisk_lowpass,'k')



figure(3)
clf
plotExptPhase(Phase1B)
set(gcf,'Position',[   144   171   676   318])

figure(33)
clf
plotStimBlock(Phase1B)
Phase1B = savePhase2MatStim(Phase1B);
save('Phase1B_stimProtocol.mat','-struct','Phase1B')












%% Phase 2A: Opto & Whisker probes
Phase2A.dt=dt;
Phase2A.ChannelMap = ChannelMap;
Phase2A.title = 'Phase2A: probe stim';

P2A_prePad = 1;
P2A_postPad = .5;

ProbeSubLen = 1;

ProbeFreq = 10;%Hz
ProbePer = 1/ProbeFreq;
ProbePerI = t2i(ProbePer);

optoOnI = 1;
optoTimes = [optoOnI, ProbePerI-optoOnI];
optoMag = [1,0];

padSegment = @(t) zeros(1,t2i(t));

optoSegment = [];
for i = 1:length(optoTimes)
    optoSegment = [optoSegment, optoMag(i)*ones(1,optoTimes(i) )];
end
optoStack = [padSegment(P2A_prePad), repmat(optoSegment,[1,ProbeFreq]), padSegment(P2A_postPad)]
    

%val = 1-exp(-k*t);
%1-val = exp(-k*t)
%log(1-val) = -K*t
%-log(1-Val) = K*t = -log(1-.99) = K*(5ms) = 4.6052


ExpVal = 0.99; 
ExpTime = 5*dt;
ExpK = -log(1-ExpVal) / (ExpTime); %this time constant ensures it reaches ExpVal of the way to steady state by ExpTime;

V2deg = @(V) 2*V;
AvgRotaVelocity = @(Vsnip) V2deg( mean(abs(diff( Vsnip )))) /dt;
targVelocity = 100;%degrees/sec

t_half = i2t(1:t2i(6*dt));
t_sawStack = {t_half, t_half+t_half(end)}
t_saw = [t_sawStack{:}]

expRiseF = @(t) 1-exp(-ExpK*t);
expFallF = @(t) (1-exp(-ExpK*5*dt))*exp(-ExpK*t);
expSawF = @(tStack) [expRiseF(tStack{1}), expFallF(tStack{1}+dt) ];


VelocityScale = targVelocity / AvgRotaVelocity(expRiseF(t_half));

expSawSnip = VelocityScale*expSawF(t_sawStack);
sprintf('Average rotational velocity is %.1f deg/sec',AvgRotaVelocity(expSawSnip(1:length(expSawSnip)/2)))

whiskWaveSegment = [expSawSnip, zeros(1, ProbePerI-length(expSawSnip)) ];

whiskWaveStack = [padSegment(P2A_postPad), repmat(whiskWaveSegment,[1,ProbeFreq]), padSegment(P2A_prePad)];

%stagger opto from whisker

optoFull = [optoStack,zeros(1,length(whiskWaveStack))];
whiskWaveFull = [zeros(1,length(optoStack)),whiskWaveStack];
P2A_trig = zeros(size(optoFull));
P2A_trig(1)=1;


Phase2A.fullt = i2t(1:length(optoFull));

Phase2A.StimBlock = zeros(nChannels,length(optoFull));

Phase2A.StimBlock(ChannelMap('whisk'),:) = whiskWaveFull;
Phase2A.StimBlock(ChannelMap('opto'),:) = optoFull;
Phase2A.StimBlock(ChannelMap('triggers'),:) = P2A_trig;

%{
figure(91)
clf
hold on
plot(t_saw/dt, expSawSnip )
plot(t_saw/dt,.99*ones(size(t_saw)),'r:')
hold off
xlabel('time [ms]')
title('exponential wave')
legend('exponential wave','0.99')
%}


figure(93)
clf
subplot(2,2,1)
hold on
plot(Phase2A.fullt, optoFull, 'Color',ColorsMap('opto'))
plot(Phase2A.fullt-2, whiskWaveFull, 'Color',ColorsMap('whisk'))
title(Phase2A.title)

xlim([1-dt,1+10*dt]);
subplot(2,1,2)
hold on
plot(Phase2A.fullt, P2A_trig+1, 'Color',ColorsMap('triggers'))
plot(Phase2A.fullt, optoFull, 'Color',ColorsMap('opto'))
plot(Phase2A.fullt, whiskWaveFull, 'Color',ColorsMap('whisk'))
xlabel('time [sec]')
%return

%Phase2A.dt ; %already set
%Phase2A.ChannelMap = ChannelMap;
%Phase2A.StimBlock;

figure(94)
clf
plotStimBlock(Phase2A)

Phase2A = savePhase2MatStim(Phase2A);
save('Phase2A_stimProtocol.mat','-struct','Phase2A')


%% Phase 2B: Control

Phase2B.dt = dt;
Phase2B.nreps = 6;
Phase2B.title = 'Phase 2B: Control type comparison';


P2B_numSubs = 5;
P2B_subLen = 30e3;
P2B_waitLen = 30e3;

P2B_trialLen = [P2B_subLen+P2B_waitLen] *P2B_numSubs;
P2B_tvec = i2t(1:P2B_trialLen);

nReroll = 0
nTransition = 0
nTrExpected = 10
stateSeqLen = P2B_subLen;
state_pTR = .5*dt;

%padFun = @(vec) [vec,NaN*ones(size(vec,1),P2B_waitLen)];
padFun = @(vec) [vec,zeros(size(vec,1),P2B_waitLen)];


while ((nTransition < nTrExpected) && (nReroll < 100))
    %"rerolls the dice" until you get a genuine HMM state sequence with
    %"enough" transitions
    nReroll = nReroll+1;

    [frozenSpikes,frozenStateSeq] = stateGenFun(P2B_subLen, state_pTR);
    nTransition = sum(abs(diff(frozenStateSeq)));
end

frozenDecode = hmmviterbi(frozenSpikes, trf(state_pTR),FR);
frozenWhiskerSeqFun = @(mags) stateWhiskerFun(stateSeqLen, frozenStateSeq, mags(1), mags(2));

STATE_NAIVE = max(frozenStateSeq(:))+1;

target = sineFun(P2B_subLen, 20*dt, 5);
olSine = sineFun(P2B_subLen, 5, 5);

figure(100)
clf
hold on
plot(i2t(1:stateSeqLen), frozenWhiskerSeqFun([0,.5]),'r')
plot(i2t(1:stateSeqLen), frozenStateSeq-1,'k:')
title(nReroll)

BigStimBlock = zeros(nChannels, P2B_trialLen);


olOn = [0,1,0,0,0];
ctrlOn = [0,1,1,1,1];
estOn = [0,0,1,1,1];
%open loop ff control  = control ON, estimation OFF

stateTrueOn = [0,0,0,0,1];
stateNaiveOn = [0,1,1,0,0];
stateOverrideOn = double([stateTrueOn | stateNaiveOn]);

stateDecodeOn = [0,0,0,1,0];

trueStateSeq = repmat(padFun(frozenStateSeq), [1,P2B_numSubs]);
trueNaiveStateSeq = [];


triggers = [];
targetStack=[];
stateGate = [] ;
optoStack = [];
fbGate = [];
estGate = [];

subOnes = ones(1,P2B_subLen);
triggerSegment = zeros(1,P2B_subLen);
triggerSegment(1)=1;


for i = 1:length(ctrlOn)
    
    
    triggers = [triggers, padFun(triggerSegment)];
    %Needs trial markers, needs 30seconds of silence inserted
    
    targetStack = [targetStack, padFun(ctrlOn(i)*target)];
    optoStack = [optoStack, padFun(olOn(i)*olSine)];
    fbGate = [fbGate, padFun(subOnes*ctrlOn(i))];
    estGate = [estGate, padFun(subOnes*estOn(i))];
    
    stateGate = [stateGate, padFun(subOnes*stateOverrideOn(i)) ];

    
    tnsSegment = 0*subOnes;
    if (stateNaiveOn(i)==1)
        tnsSegment = subOnes*STATE_NAIVE;
    end
    if (stateTrueOn(i)==1)
        tnsSegment = frozenStateSeq;
    end
    trueNaiveStateSeq = [trueNaiveStateSeq, padFun(tnsSegment)];

    
end



BigStimBlock(ChannelMap('whisk'),:) = repmat(padFun(frozenWhiskerSeqFun([0,.3])), [1,P2B_numSubs]);
BigStimBlock(ChannelMap('decodedState'),:) = repmat(padFun(frozenDecode), [1,P2B_numSubs]);
BigStimBlock(ChannelMap('trueState'),:) = trueStateSeq;

BigStimBlock(ChannelMap('triggers'),:) = triggers;
BigStimBlock(ChannelMap('stateOverride'),:) = trueNaiveStateSeq;
BigStimBlock(ChannelMap('doOverrideState'),:) = stateGate;

%BigStimBlock(ChannelMap('opto'),:) = optoStack;
BigStimBlock(ChannelMap('yRef'),:) = targetStack;

BigStimBlock(ChannelMap('fbCtrl'),:) = fbGate;
BigStimBlock(ChannelMap('fbEstim'),:) = estGate;



figure(103)
clf

Phase2B.dt ; %already set
Phase2B.ChannelMap = ChannelMap;
Phase2B.StimBlock = BigStimBlock;

Phase2B.trialT = i2t(1:size(Phase2B.StimBlock,2));

plotStimBlock(Phase2B)
Phase2B = savePhase2MatStim(Phase2B);
save('Phase2B_stimProtocol.mat','-struct','Phase2B')


%%




















