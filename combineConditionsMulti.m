function [conList,nCond,conListBlock] = combineConditionsMulti(conOptions)
    %"vectors" together conditions, such that in1={'high','low'}, in2={'left','right'}
    %results in {'high-left', 'high-right', 'low-left', 'low-right'}
    %intended to help build experiment protocols
    
    nOptions = length(conOptions);
    conList_ = cell(1,nOptions);

    [conList_{:}] = ndgrid(conOptions{:});
    %{
    [conList1_,conList2_] = meshgrid(conOptions{:});
    conList1 = conList1_(:);
    conList2 = conList2_(:);
    
    conList = [conList1,conList2];
    %}
    
    %flatten the space of options which was say 2x2 to 4x1
    conListFlat = cellfun(@(c) {c{:}}',conList_, 'UniformOutput',false);
    conListBlock = [conListFlat{:}]; %this is now nConditions  x nChannels
    
    nCond=size(conListBlock,1);

    conList = cell(nCond,1);
    for i = 1:size(conListBlock,1)
        conList{i} = conListBlock(i,:);
    end
    
    %conListCat = cellfun(@(c) strjoin(c, ','),conList,'UniformOutput',false);
end