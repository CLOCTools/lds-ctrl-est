clear; clc;

dt = h5read('eg_plds_est.h5','/dt');
u = h5read('eg_plds_est.h5','/u');
z = h5read('eg_plds_est.h5','/z');

y_true = h5read('eg_plds_est.h5','/y_true');
y_hat= h5read('eg_plds_est.h5','/y_hat');

x_true = h5read('eg_plds_est.h5','/x_true');
x_hat= h5read('eg_plds_est.h5','/x_hat');

m_true = h5read('eg_plds_est.h5','/m_true');
m_hat= h5read('eg_plds_est.h5','/m_hat');

n_t = size(y_hat,2);
t = ((1:n_t)-1)*dt;

c_data = 0.25 + zeros(1,3);
c_true = 0.5 + zeros(1,3);
c_est = [0.85, 0.5, 0.85];
c_ref = [.25, .75, 0];

t_z = t(z>0);
z = z(z>0);

figure;
subplot(411); hold on;
plot(t_z, z'*10, '.', 'markersize', 2, 'color', c_data);
plot(t, y_hat'/dt, 'LineWidth', 2, 'color', c_est);
% plot(t, y_true'/dt, 'LineWidth', 2, 'color', c_true);
legend({'measurements'; 'estimated output'}, 'location', 'northwest')
ylabel({'(events/s)'})
ylim([0 100])

subplot(412); hold on;
plot(t, x_hat', 'LineWidth', 2, 'color', c_est);
plot(t, x_true', 'LineWidth', 2, 'color', c_true);
legend({'estimated'; 'ground truth'}, 'location', 'southeast')
ylabel({'States'; '(a.u.)'})

subplot(413); hold on;
plot(t, m_hat', 'LineWidth', 2, 'color', c_est);
plot(t, m_true', 'LineWidth', 2, 'color', c_true);
legend({'estimated'; 'ground truth'}, 'location', 'northwest')
ylabel({'Disturbance'; '(a.u.)'})

subplot(414); hold on;
plot(t, u', 'LineWidth', 2, 'color',  c_data)
ylabel({'Input';'(V)'})
xlabel('Time (s)')

printfig('eg_plds_est_output','png',gcf,[8 8]);

% look at error after "warmup"
warm = ceil(5/dt);
e_y = y_hat(:,warm:end) - y_true(:,warm:end);
e_x = x_hat(:,warm:end) - x_true(:,warm:end);

figure;
subplot(121)
histogram(e_y(:)/dt, 'FaceColor', 0.5+zeros(1,3), 'EdgeColor', 'none', 'normalization','probability'); hold on;
ax=gca;
plot(mean(e_y(:))/dt+[0;0], ax.YLim, '--', 'LineWidth', 1, 'color', 'k')
xlabel('error (events/s)')
ylabel('pr(error)')
title({'Output';'Estimation Error'})

subplot(122)
histogram(e_x(:), 'FaceColor', 0.5+zeros(1,3), 'EdgeColor', 'none', 'normalization','probability'); hold on;
ax=gca;
plot(mean(e_x(:))+[0;0], ax.YLim, '--', 'LineWidth', 1, 'color', 'k')
xlabel('error (a.u.)')
ylabel('pr(error)')
title({'State'; 'Estimation Error'})

printfig('eg_plds_est_output_hist','png',gcf,[5 4]);

