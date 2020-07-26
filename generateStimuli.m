function [ PhaseStruct ] = generateStimuli(PhaseStruct, ConditionMap, ChannelMap)
    
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
        
        cond = PhaseStruct.conditions{i}.conditionName;
        
        if isfield(PhaseStruct.conditions{i},'params')
            params = PhaseStruct.conditions{i}.params;
        else
            params = 'DEFAULT';
        end


        if isOff(cond)
            fON = ConditionMap('OFF');
        else
            fON = ConditionMap(cond);
        end
        fOFF = @(len) zeros(1,len);

        fON
        params
        lengths
        
        preSeg = fOFF(lengths(1));
        onSeg = fON(lengths(2),params);
        postSeg = fOFF(lengths(3));
        segment = [preSeg,onSeg,postSeg];
        
        
        PhaseStruct.t{i} = t;
        PhaseStruct.lenghts{i} = lengths;
        PhaseStruct.segments{i} = segment;

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