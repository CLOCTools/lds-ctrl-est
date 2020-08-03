classdef (Abstract) LDS < matlab.mixin.Copyable

properties
dt double = [];

% parameters
x0(:,1) double = [];
P0(:,:) double = [];

m(:,1) double = [];
g(:,1) double = [];

A(:,:) double = [];
B(:,:) double = [];
Q(:,:) double = [];
end

properties (Abstract, Access=protected)
nX uint64;
nU uint64;
end

methods (Static)
end

end%classdef

function passed = contentsAreChar(c)
  passed = cellfun(@(x) ischar(x), c);
  passed = sum(passed)==numel(c);
  if ~passed
    error('Must be type char.')
  end
end

function passed = contentsAreDouble(c)
  passed = cellfun(@(x) isa(x,'double'), c);
  passed = sum(passed)==numel(c);
  if ~passed
    error('Must be type double.')
  end
end
