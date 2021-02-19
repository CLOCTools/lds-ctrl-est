clear; clc;

dt = h5read('test_plds_switched_ctrl.h5','/dt');
u = h5read('test_plds_switched_ctrl.h5','/u');
z = h5read('test_plds_switched_ctrl.h5','/z');

lambdaRef = h5read('test_plds_switched_ctrl.h5','/lambdaRef');
lambdaTrue = h5read('test_plds_switched_ctrl.h5','/lambdaTrue');
lambdaHat= h5read('test_plds_switched_ctrl.h5','/lambdaHat');

xTrue = h5read('test_plds_switched_ctrl.h5','/xTrue');
xHat= h5read('test_plds_switched_ctrl.h5','/xHat');

% mTrue = h5read('test_plds_switched_ctrl.h5','/mTrue');
% mHat= h5read('test_plds_switched_ctrl.h5','/mHat');

state = h5read('test_plds_switched_ctrl.h5', '/state');

K = size(lambdaHat,2);
t = ((1:K)-1)*dt;

cEst = [0.85, 0.5, 0.85];

figure;
subplot(511); hold on;
plot(t, lambdaHat'/dt, 'LineWidth', 2, 'color', cEst);
plot(t, lambdaTrue'/dt, 'LineWidth', 2, 'color', [.25, .75, 0]);
plot(t, lambdaRef'/dt, 'LineWidth', 2, 'color', [.25, .75, 0]*.5);
legend({'Estimated Rate'; 'True Rate'})
xlabel('Time (s)')
ylabel('Event Rate (events/s)')
ylim([0 250])

subplot(512);
plot(t, z', 'LineWidth', 0.1, 'color', [.33, .33, .33]);
ylabel('Measurements')

log_lambdaRef = log(lambdaRef);

subplot(513); hold on;
plot(t, xHat', 'LineWidth', 2, 'color', cEst);
plot(t, xTrue', 'LineWidth', 2, 'color', [.25, .75, 0]);
plot(t, log_lambdaRef', 'LineWidth', 2, 'color', [.25, .75, 0]*.5);
ylabel('x')

subplot(514); hold on;
% plot(t, mHat', 'LineWidth', 2, 'color', cEst);
% plot(t, mTrue', 'LineWidth', 2, 'color', [.25, .75, 0]);
plot(t, state', 'LineWidth', 2, 'color', [.25, .75, 0]);
ylabel('switch-state')

subplot(515); hold on;
% e = xHat - log_lambdaRef;
% plot(t, -e', 'LineWidth', 2, 'color', [1, 0.33, 0])
plot(t, u', 'LineWidth', 2, 'color', [0.33, 0.33, 1])
ylabel('input')
