function [Phase] = savePhase2MatStim(Phase)
    %% funneling variables for saving to be read by:
    %https://github.gatech.edu/StanleyLab/rtxi-matioStimLd
    %{
    dt : double, Sample period (e.g., 0.001)
    trig : Matrix{double} [nTrig x nSamps], Time-keeping triggers (e.g., trial markers, whisker videography frame triggers, pupil videography frame triggers)
    gate : Matrix{double} [nGate x nSamps], Logical gates for switching behavior (e.g., enable control, enable estimator, lock control, add noise)

    gate5 is 
    gate6 is 

    u0 : Matrix{double} [nU0 x nSamps], Analog input(s) 0 (e.g., sensory stimuli, 1-4 galvos)
    u1 : Matrix{double} [nU1 x nSamps], Analog input(s) 1 (e.g., optical stimuli)
    u2 : Matrix{double} [nU2 x nSamps], Analog input(s) 2 (currently unused)
    u3 : Matrix{double} [nU3 x nSamps], Analog input(s) 3 (currently unused)

    Additionally, for control applications where the objective is to track a time-varying reference trajectory, the following can be loaded from the mat file and sent to a separate controller plugin.

    yRef : Matrix{double} [nY x nSamps], Reference output signals for control applications
    %}
    blankTrial =  zeros(1,size(Phase.StimBlock,2));

    Phase.trig = Phase.StimBlock(Phase.ChannelMap('triggers'),:);

    Phase.gate(1,:) = Phase.StimBlock(Phase.ChannelMap('fbCtrl'),:);
    Phase.gate(2,:) = Phase.StimBlock(Phase.ChannelMap('fbEstim'),:);
    Phase.gate(3,:) = blankTrial;
    Phase.gate(4,:) = blankTrial;

    Phase.gate(5,:) = Phase.StimBlock(Phase.ChannelMap('doOverrideState'),:); %"DO OVERRIDE"
    Phase.gate(6,:) = Phase.StimBlock(Phase.ChannelMap('stateOverride'),:); %"OVERRIDDEN STATE"

    Phase.u0 = Phase.StimBlock(Phase.ChannelMap('whisk'),:);
    Phase.u1 = Phase.StimBlock(Phase.ChannelMap('opto'),:);
    Phase.u2 = blankTrial;
    Phase.u3 = blankTrial;

    Phase.yRef = Phase.StimBlock(Phase.ChannelMap('yRef'),:)
end