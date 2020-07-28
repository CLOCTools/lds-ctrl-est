function plotStimBlock(Phase)
    dt = Phase.dt;
    i2t = @(i) (i-1)*dt;

    ChannelMap = Phase.ChannelMap;
    channelVals = ChannelMap.values;
    channelKeys = ChannelMap.keys;

    [channelVals,sortIdx_] = sort([channelVals{:}]);
    channelKeys = {channelKeys{sortIdx_}};
    nChannels = max(channelVals);
    
    trialLen = size(Phase.StimBlock,2);
    Phase.trialT = i2t(1:trialLen);
    XL = i2t([round(trialLen/5), 2*round(trialLen/5)]);
    
    subplot(2,2,1)
    hold on
    for i = 1:nChannels
        vec = Phase.StimBlock(i,:);
        if all(vec==0)
        else
            if ((max(vec)<.1))
                plot(Phase.trialT , (vec./max(vec)+i))
            else
                plot(Phase.trialT  , (vec./1+i))
            end
        end
    end
    xlim(XL);
    set(gca,'YTick',channelVals,'YTickLabels',channelKeys)
    title(Phase.title)

    subplot(2,1,2)
    hold on

    for i = 1:nChannels
        vec = Phase.StimBlock(i,:);
        if all(vec==0)
        else
            if ((max(vec)<.1))
                plot(Phase.trialT , (vec./max(vec)+i))
            else
                plot(Phase.trialT  , (vec./1+i))
            end
        end
    end



    set(gca,'YTick',channelVals,'YTickLabels',channelKeys)
    xlabel('time [seconds]')
    set(gcf,'Position',[         155         169        1136         629])

    end