function plotExptPhase(Phase)
    
    subplot(2,2,1)
    hold on
    for i = 1:Phase.nCond
        seg = Phase.segments{i};
        for j = 1:size(seg,1)
            plot(Phase.t{i},seg(j,:)+Phase.channelIDs{i}(j), 'Color',Phase.plotColors{i}{j})
        end
    end
    title([Phase.title,'[single trial]'] )


    subplot(2,1,2)
    hold on
    for j = 1:size(Phase.fullSegment,1)
        plot(Phase.fullt, Phase.fullSegment(j,:)+Phase.channelIDs{1}(j), 'Color',Phase.plotColors{1}{j})
    end
    title([Phase.title,'[all trials]'] )
end