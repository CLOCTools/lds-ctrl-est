clear; clc;

dt = h5read('test_plds_est.h5','/dt');
u = h5read('test_plds_est.h5','/u');
z = h5read('test_plds_est.h5','/z');

lambdaTrue = h5read('test_plds_est.h5','/lambdaTrue');
lambdaHat= h5read('test_plds_est.h5','/lambdaHat');

xTrue = h5read('test_plds_est.h5','/xTrue');
xHat= h5read('test_plds_est.h5','/xHat');

mTrue = h5read('test_plds_est.h5','/mTrue');
mHat= h5read('test_plds_est.h5','/mHat');

K = size(lambdaHat,2);
t = ((1:K)-1)*dt;

cEst = [0.85, 0.5, 0.85];

figure;
subplot(511); hold on;
plot(t, lambdaHat'/dt, 'LineWidth', 2, 'color', cEst);
plot(t, lambdaTrue'/dt, 'LineWidth', 2, 'color', [.25, .75, 0]);
legend({'Estimated Rate'; 'True Rate'})
xlabel('Time (s)')
ylabel('Event Rate (events/s)')
ylim([0 250])

subplot(512);
plot(t, z', 'LineWidth', 0.1, 'color', [.33, .33, .33]);
ylabel('Measurements')

subplot(513); hold on;
plot(t, xHat', 'LineWidth', 2, 'color', cEst);
plot(t, xTrue', 'LineWidth', 2, 'color', [.25, .75, 0]);
ylabel('x')

subplot(514); hold on;
plot(t, mHat', 'LineWidth', 2, 'color', cEst);
plot(t, mTrue', 'LineWidth', 2, 'color', [.25, .75, 0]);
ylabel('disturbance')

subplot(616); hold on;
plot(t, u', 'LineWidth', 2, 'color', [0.33, 0.33, 1])
ylabel('input')


% look at s-s error
eY = lambdaHat(:,100:end) - lambdaTrue(:,100:end);
eX = xHat(:,100:end) - xTrue(:,100:end);

figure;
subplot(121)
histogram(eY(:)/dt, 'FaceColor', 0.5+zeros(1,3), 'EdgeColor', 'none'); hold on;
ax=gca;
plot(mean(eY(:))/dt+[0;0], ax.YLim, '--', 'LineWidth', 2, 'color', 'k')
title('Output Estimation Error')

subplot(122)
histogram(eX(:), 'FaceColor', 0.5+zeros(1,3), 'EdgeColor', 'none'); hold on;
ax=gca;
plot(mean(eX(:))+[0;0], ax.YLim, '--', 'LineWidth', 2, 'color', 'k')
title('State Estimation Error')
