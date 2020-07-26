function [conList,n] = combineConditions(conOptions1, conOptions2)
    %"vectors" together conditions, such that in1={'high','low'}, in2={'left','right'}
    %results in {'high-left', 'high-right', 'low-left', 'low-right'}
    %intended to help build experiment protocols

    [conList1_,conList2_] = meshgrid(conOptions1,conOptions2);
    conList1 = conList1_(:);
    conList2 = conList2_(:);
    
    conList = [conList1,conList2];
    
    n=length(conList1);
end