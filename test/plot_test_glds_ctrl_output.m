clear; clc;

load('../test_glds_ctrl.mat')

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
