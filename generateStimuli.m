function [ PhaseStruct ] = generateStimuli(PhaseStruct, ConditionMap, ChannelMap,ColorsMap)
    
    t2i = @(t) round(t/PhaseStruct.dt);
    trialLengths = zeros(PhaseStruct.nCond,1);

    isOff = @(str) contains(str,'OFF');
    stripEnd = @(str) erase(erase(str,'_ON'),'_OFF');
    segments2fulltrace = @(PhaseStruct) repmat([PhaseStruct.segments{:}], [1,PhaseStruct.nreps]);

    
    for i = 1:PhaseStruct.nCond
        
        %lengths = t2i(PhaseStruct.times);
        
        %trial lengths should be equal across channels
        trialLengths(i) = t2i(PhaseStruct.trialLength); %sum(lengths);
        
        t = (1:trialLengths(i));
        if i>1
            t = t+ sum(trialLengths(1:(i-1)));
        end
        
        condStruct = PhaseStruct.conditions{i};
        nChannels = numel(condStruct);
        
        if (nChannels==1)
            channelStruct = {condStruct};
        else
            channelStruct = condStruct;
        end
        
        segmentChannels = zeros(nChannels, trialLengths(i));
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
            
            if isobject(PhaseStruct.times)
                lengths = t2i(PhaseStruct.times(type));
            else
                lengths = t2i(PhaseStruct.times);
            end
            
            
            
            fOFF = @(len) zeros(1,len);
            preSeg = fOFF(lengths(1));
            onSeg = fON(lengths(2),params);
            postSeg = fOFF(lengths(3));
            segment = [preSeg,onSeg,postSeg];
            
            size(segmentChannels)
            size(segment)
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
    lastt = 0;
    for i = 1:PhaseStruct.nreps
        fullt = [fullt, [PhaseStruct.t{:}]+lastt];
        lastt = fullt(end);
    end
    
    PhaseStruct.fullt = fullt;
    PhaseStruct.fullSegment = segments2fulltrace(PhaseStruct);
end