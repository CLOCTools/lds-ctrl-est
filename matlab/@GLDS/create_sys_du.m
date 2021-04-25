function [sys_du] = create_sys_du(this)
% [sys_du] = create_sys_du(this)
%
% Create a version of this system, where the input is the time-change in u (du),
% which entails augmenting the state with input v=g*u.
%

% get dimensions
[n_x,n_u] = size(this.B);
n_y = size(this.C,1);

% make a copy
sys_du = copy(this);

% now perform necessary augmentations
sys_du.A = [this.A this.B; zeros(n_u,n_x) ones(n_u,n_u)];
sys_du.B = [zeros(n_x,n_u); ones(n_u,n_u)];
sys_du.C = [this.C zeros(n_y,n_u)];
sys_du.Q = [this.Q zeros(n_x,n_u); zeros(n_u,n_x) zeros(n_u,n_u)];
sys_du.m = [this.m; zeros(n_u,1)];
sys_du.x0 = [this.x0; zeros(n_u,1)];
sys_du.P0 = [this.P0 zeros(n_x,n_u); zeros(n_u,n_x) zeros(n_u,n_u)];

% just in case Kalman gain changes
sys_du.calcK_steadyState();

end
