function Obj = fillColorChannel(Obj,ColorMap, ChannelMap)
    Obj.color = ColorMap(Obj.type);
    Obj.channel = ChannelMap(Obj.type);
end