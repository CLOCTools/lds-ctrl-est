function [ PhaseStruct ] = generateStimuli(PhaseStruct, ConditionMap, ChannelMap,ColorsMap)
    
    t2i = @(t) floor(t/PhaseStruct.dt);
    trialLengths = zeros(PhaseStruct.nCond,1);

    isOff = @(str) contains(str,'OFF');
    stripEnd = @(str) erase(erase(str,'_ON'),'_OFF');
    segments2fulltrace = @(PhaseStruct) repmat([PhaseStruct.segments{:}], [1,PhaseStruct.nreps]);

    
    for i = 1:PhaseStruct.nCond
        lengths = t2i(PhaseStruct.times);
        trialLengths(i) = sum(lengths);
        t = (1:trialLengths(i))+sum(trialLengths(1:i));

       % cond = PhaseStruct.conditions{i}{1};
       % condPre = stripEnd(cond); %get prefix
        %params = PhaseStruct.conditions{i}{2};
        
        
        %inner for loop based on parallel conditions
        
        condStruct = PhaseStruct.conditions{i};
        nChannels = numel(condStruct);
        
        if (nChannels==1)
            channelStruct = {condStruct}
        else
            channelStruct = condStruct;
        end
        
        segmentChannels = zeros(nChannels, sum(lengths));
        channelIDs = zeros(nChannels,1);
        plotColors = cell(nChannels,1);
        
        for j = 1:nChannels
            
            thisChannelStruct = channelStruct{j};
            cond = thisChannelStruct.conditionName;
            
            if isfield(thisChannelStruct,'params')
                params = thisChannelStruct.params;
            else
                params = 'DEFAULT';
            end

            if isOff(cond)
                fON = ConditionMap('OFF').function;
                nameOn = [stripEnd(cond),'_ON']; %if ends in x_OFF, use x_ON to check type
                type = ConditionMap(nameOn).type;

            else
                fON = ConditionMap(cond).function;
                type = ConditionMap(cond).type;

            end
            fOFF = @(len) zeros(1,len);

            fON
            params
            lengths

            preSeg = fOFF(lengths(1));
            onSeg = fON(lengths(2),params);
            postSeg = fOFF(lengths(3));
            segment = [preSeg,onSeg,postSeg];
            
            segmentChannels(j,:) = segment;
            channelIDs(j) = ChannelMap(type);
            plotColors{j} = ColorsMap(type);
        end
        
        
        
        
        PhaseStruct.t{i} = t;
        PhaseStruct.lenghts{i} = lengths;
        PhaseStruct.segments{i} = segmentChannels;
        PhaseStruct.channelIDs{i} = channelIDs;
        PhaseStruct.plotColors{i} = plotColors;

    end
    
    fullt = [];
    lastt = 0
    for i = 1:PhaseStruct.nreps
        fullt = [fullt, [PhaseStruct.t{:}]+lastt];
        lastt = fullt(end);
    end
    
    PhaseStruct.fullt = fullt;
    PhaseStruct.fullSegment = segments2fulltrace(PhaseStruct);
end