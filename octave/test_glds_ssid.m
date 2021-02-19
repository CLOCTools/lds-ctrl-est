pkg load statistics
pkg load control

%% create a simple second order model.
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

sys = struct(...
  'A', A, ...
  'B', B, ...
  'C', C, ...
  'D', D, ...
  'm', m, ...
  'g', g, ...
  'x0', x0, ...
  'P0', P0, ...
  'Q', Q, ...
  'R', R ...
  );

%% simulate forward and generate training data
randn("state", 13)%for reproducibility

dt = 1e-3;
T = 10;
n_trials = 15;

t = 0:dt:5;
n_samp = length(t);

u = repmat({zeros(n_u, n_samp)}, [n_trials, 1]);
x = repmat({zeros(n_x, n_samp)}, [n_trials, 1]);
z = repmat({zeros(n_y, n_samp)}, [n_trials, 1]);

% making the input some random data.
for trial=1:n_trials
  for k =2:n_samp
    u{trial}(:,k) = 0.975*u{trial}(:,k-1) + 1e-1*randn(n_u,1);
  end
end

x{trial}(:,1) = x0;
z{trial}(:,1) = C*x{trial}(:,1) + D*u{trial}(:,1) + d;
mu = zeros(1, n_x);
for trial = 1:n_trials
  noise_x = mvnrnd(mu, Q, n_samp)';
  noise_y = mvnrnd(mu, R, n_samp)';
  for k = 2:n_samp
    x{trial}(:,k) = A*x{trial}(:,k-1) + B*g*u{trial}(:,k-1) + m + noise_x(:,k-1);
    z{trial}(:,k) = C*x{trial}(:,k) + D*u{trial}(:,k) + d + noise_y(:,k-1);
  end
end

%% fit model
n_x_fit = 2; %n_x
% [fit, sing_vals] = glds_ssid_mex(u, z, dt, nX, [nH, d0, force_unitNorm_C, which_wt, wt_g0, t0, t_startSSID, t_stopSSID])
tic()
[sys_hat, sing_vals] = glds_ssid_mex(u, z, dt, n_x_fit, 25, d, false, 1, 0);
toc()

%% compare fit to original
sys_mat = ss(sys.A, sys.B, sys.C, sys.D, dt);
sys_hat_mat = ss(sys_hat.A, sys_hat.B, sys_hat.C, sys_hat.D, dt);

t_imp = 0:dt:0.1;
[y_imp] = impulse(sys_mat, t_imp);
[y_imp_hat] = impulse(sys_hat_mat, t_imp);

figure;
subplot(121); hold on;
semilogy(sing_vals, '..', 'color', 'k');
semilogy(sing_vals(1:n_x_fit), '-', 'color', 'k');
ylabel('Singular Values')
xlabel('Singular Value Index')

subplot(122); hold on;
plot(t_imp, y_imp, '-', 'color', [0 0 0]);
plot(t_imp, y_imp_hat, '-', 'color', [0 0 0]+0.5);
legend('ground truth', 'estimated')
ylabel('Impulse Response (a.u.)')
xlabel('Time (s)')

% simulate
x_hat = repmat({zeros(n_x, n_samp)}, [n_trials, 1]);
y_hat = repmat({zeros(n_y, n_samp)}, [n_trials, 1]);
x_hat{trial}(:,1) = x0;
y_hat{trial}(:,1) = C*x_hat{trial}(:,1) + D*u{trial}(:,1) + d;
mu = zeros(1, n_x);
for trial = 1:n_trials
  for k = 2:n_samp
    x_hat{trial}(:,k) = A*x_hat{trial}(:,k-1) + B*g*u{trial}(:,k-1) + m;
    y_hat{trial}(:,k) = C*x_hat{trial}(:,k) + D*u{trial}(:,k) + d;
  end
end

z_m = cell2mat(z');
y_hat_m = cell2mat(y_hat');

pve = 1 - var(z_m-y_hat_m,[],2)./var(z_m,[],2);

eg_trial = 1;

figure;
subplot(311); hold on;
h=plot(t, z{eg_trial}(1,:)', '-', 'color', zeros(1,3));
h(2)=plot(t, y_hat{eg_trial}(1,:)', '-', 'color', 0.5+zeros(1,3));
legend(h, 'measurement', 'fit')
ylabel('Output 1 (a.u.)')
xlabel('Time (s)')
title(['proportion var explained (training): ' num2str(pve(1))])

subplot(312); hold on;
plot(t, z{eg_trial}(2,:)', '-', 'color', zeros(1,3));
plot(t, y_hat{eg_trial}(2,:)', '-', 'color', 0.5+zeros(1,3));
ylabel('Output 2 (a.u.)')
xlabel('Time (s)')
title(['proportion var explained (training): ' num2str(pve(2))])

subplot(313)
plot(t, u{eg_trial}','color', zeros(1,3));
ylabel('Input (a.u.)')
xlabel('Time (s)')

