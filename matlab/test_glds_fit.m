%%%
%%% This script fits a GLDS model to simulated data using subspace
%%% identification, then refines the fit using expectation-maximization.
%%%

%% create a simple second order model.
dt = 1e-3;
A = [0.95 -0.25; 0 0.9];
B = [0; 0.01];
C = eye(size(A));
n_x = size(A,1);
n_u = size(B,2);
n_y = size(C,1);
D = zeros(n_y, n_u);%feedthrough
d = zeros(n_y,1);%output bias

% process/measurement noise covariances
Q = 1e-4 * eye(n_x);
R = 1e-2 * eye(n_y);

% initial conditions
x0 = zeros(n_x,1);%expected initial state
P0 = Q;%covariance about that state

% process disturbance, input gain, etc.
m = zeros(n_x,1);
g = ones(n_u,1);

%     GLDS(dt,A,B,g,m,Q,C,d,R,x0,P0)
sys = GLDS(dt,A,B,g,m,Q,C,d,R,x0,P0);

%% simulate forward and generate training data
rng(13)%for reproducibility

dt = 1e-3;
T = 10;
n_trials = 15;

t = 0:dt:5;
n_samp = length(t);

u = repmat({zeros(n_u, n_samp)}, [n_trials, 1]);

% making the input some random data.
for trial=1:n_trials
  for k=2:n_samp
    u{trial}(:,k) = 0.975*u{trial}(:,k-1) + 1e-1*randn(n_u,1);
  end
end

[y, x, z] = sys.simulate(u,true);

n_samp_imp = ceil(0.1/dt);
y_imp = sys.simulate_imp(n_samp_imp);

%% fit model
sys_hat = copy(sys);
n_x_fit = n_x;
tic()
sing_vals = glds_ssid_mex(sys_hat, u, z, dt, n_x_fit, 50, d, 2);
toc()

%% compare fit to original
% [y_hat, x_hat] = sys_hat.kfilter_dual(u,z,true,false);
[y_hat, x_hat] = sys_hat.simulate(u,false);
[y_imp_hat, t_imp] = sys_hat.simulate_imp(n_samp_imp);

figure;
subplot(121);
semilogy(sing_vals, 'color', 0.5+zeros(1,3)); hold on;
semilogy(sing_vals(1:n_x_fit), 'color', 'k', 'linewidth', 2);
ylabel('Singular Values')
xlabel('Singular Value Index')

subplot(122); hold on;
plot(t_imp, y_imp{1}, '-', 'color', [0 0 0], 'linewidth', 2);
plot(t_imp, y_imp_hat{1}, '-', 'color', [0 0 0]+0.5, 'linewidth', 2);
legend('ground truth', 'estimated')
ylabel('Impulse Response (a.u.)')
xlabel('Time (s)')

z_m = cell2mat(z');
y_hat_m = cell2mat(y_hat');
pve = 1 - var(z_m-y_hat_m,[],2)./var(z_m,[],2);

eg_trial = 1;

figure;
subplot(311); hold on;
h=plot(t, z{eg_trial}(1,:)', '-', 'color', zeros(1,3));
h(2)=plot(t, y_hat{eg_trial}(1,:)', '-', 'color', 0.5+zeros(1,3), 'linewidth', 2);
legend(h, 'measurement', 'fit')
ylabel('Output 1 (a.u.)')
xlabel('Time (s)')
title(['proportion var explained (training): ' num2str(pve(1))])

subplot(312); hold on;
plot(t, z{eg_trial}(2,:)', '-', 'color', zeros(1,3));
plot(t, y_hat{eg_trial}(2,:)', '-', 'color', 0.5+zeros(1,3), 'linewidth', 2);
ylabel('Output 2 (a.u.)')
xlabel('Time (s)')
title(['proportion var explained (training): ' num2str(pve(2))])

subplot(313)
plot(t, u{eg_trial}','color', zeros(1,3));
ylabel('Input (a.u.)')
xlabel('Time (s)')

%%Refit by E-M
do_refit = true;
if (do_refit)
  calcAB = true; %calculate dynamics (A, B mats)
  calcQ = true; %calculate process noise cov (Q)
  calcInitial = true; %calculate initial conditions
  calcOutput = true; %calculate output (C)
  calcMeasurement = true; %calculate output noise (R)
  n_iter = 100;
  tol = 1e-2;

  [y_em, x_em, sum_E_xu_tm1_xu_tm1, sum_E_xu_t_xu_tm1, sum_E_x_t_x_t, n_t] = glds_em_refit_mex(sys_hat, u, z, calcAB, calcQ, calcInitial, calcOutput, calcMeasurement, n_iter, tol);

  % [y_hat_em, x_hat_em] = sys_hat.kfilter_dual(u,z,true,false);
  [y_hat_em, x_hat_em] = sys_hat.simulate(u,false);
  y_imp_hat_em = sys_hat.simulate_imp(n_samp_imp);

  % E-M smoothed estimates
  figure;
  subplot(211); hold on;
  plot(t, z{eg_trial}(1,:),'color', zeros(1,3));
  plot(t, y_em{eg_trial}(1,:),'color', [0 0 0]+0.5, 'linewidth', 2);
  legend('measurement', 'smoothed estimate')
  ylabel('Output (a.u.)')

  subplot(212)
  plot(t, u{eg_trial}','color', zeros(1,3));
  ylabel('Input (a.u.)')
  xlabel('Time (s)')

  figure;
  subplot(122); hold on;
  plot(t_imp, y_imp{1}, '-', 'color', [0 0 0], 'linewidth', 2);
  plot(t_imp, y_imp_hat_em{1}, '-', 'color', [0 0 0]+0.5, 'linewidth', 2);
  legend('ground truth', 'EM re-estimated')
  ylabel('Impulse Response (a.u.)')
  xlabel('Time (s)')

  y_hat_em_m = cell2mat(y_hat_em');
  pve_em = 1 - var(z_m-y_hat_em_m,[],2)./var(z_m,[],2);

  figure;
  subplot(311); hold on;
  h=plot(t, z{eg_trial}(1,:)', '-', 'color', zeros(1,3));
  h(2)=plot(t, y_hat_em{eg_trial}(1,:)', '-', 'color', 0.5+zeros(1,3), 'linewidth', 2);
  legend(h, 'measurement', 'fit')
  ylabel('Output 1 (a.u.)')
  xlabel('Time (s)')
  title(['EM-refit proportion var explained (training): ' num2str(pve_em(1))])

  subplot(312); hold on;
  plot(t, z{eg_trial}(2,:)', '-', 'color', zeros(1,3));
  plot(t, y_hat_em{eg_trial}(2,:)', '-', 'color', 0.5+zeros(1,3), 'linewidth', 2);
  ylabel('Output 2 (a.u.)')
  xlabel('Time (s)')
  title(['EM-refit proportion var explained (training): ' num2str(pve_em(2))])

  subplot(313)
  plot(t, u{eg_trial}','color', zeros(1,3));
  ylabel('Input (a.u.)')
  xlabel('Time (s)')
end
