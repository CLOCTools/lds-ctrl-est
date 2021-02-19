clear; clc;

dt = h5read('test_glds_ctrl.h5','/dt');
u = h5read('test_glds_ctrl.h5','/u');
z = h5read('test_glds_ctrl.h5','/z');

lambdaRef = h5read('test_glds_ctrl.h5','/lambdaRef');
lambdaTrue = h5read('test_glds_ctrl.h5','/lambdaTrue');
lambdaHat= h5read('test_glds_ctrl.h5','/lambdaHat');

xTrue = h5read('test_glds_ctrl.h5','/xTrue');
xHat= h5read('test_glds_ctrl.h5','/xHat');

mTrue = h5read('test_glds_ctrl.h5','/mTrue');
mHat= h5read('test_glds_ctrl.h5','/mHat');

K = size(lambdaHat,2);
t = ((1:K)-1)*dt;

cEst = [0.85, 0.5, 0.85];

figure;
subplot(611); hold on;
plot(t, lambdaHat'/dt, 'LineWidth', 2, 'color', cEst);
plot(t, lambdaTrue'/dt, 'LineWidth', 2, 'color', [.25, .75, 0]);
plot(t, lambdaRef'/dt, 'LineWidth', 2, 'color', [.25, .75, 0]*.5);
legend({'Estimated Rate'; 'True Rate'})
xlabel('Time (s)')
ylabel('Event Rate (events/s)')
% ylim([0 250])

subplot(612);
plot(t, z', 'LineWidth', 0.1, 'color', [.33, .33, .33]);
ylabel('Measurements')

subplot(613); hold on;
plot(t, xHat', 'LineWidth', 2, 'color', cEst);
plot(t, exp(xTrue)', 'LineWidth', 2, 'color', [.25, .75, 0]);
ylabel('x')

subplot(614); hold on;
plot(t, mHat', 'LineWidth', 2, 'color', cEst);
ylabel('disturbance')

subplot(615); hold on;
plot(t, mTrue', 'LineWidth', 2, 'color', [.25, .75, 0]);
ylabel('plds disturbance')

subplot(616); hold on;
plot(t, u', 'LineWidth', 2, 'color', [0.33, 0.33, 1])
ylabel('input')
