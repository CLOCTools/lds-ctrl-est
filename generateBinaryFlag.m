function [FLAG,idIncr] = generateBinaryFlag(id)
    FLAG = dec2bin(2.^id);
    idIncr = id+1;
end