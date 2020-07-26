function boundObj = bindNameParams(Obj)
    boundObj = cell(size(Obj,1), 1);
    
    for i = 1:size(Obj,1)
        boundObj{i} = struct('name',Obj{i}{1}, 'params',Obj{i}{2});
    end
end
