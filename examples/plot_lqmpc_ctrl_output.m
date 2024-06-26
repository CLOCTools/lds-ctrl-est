clear; clc;

% Read data
        dt = h5read('eg_lqmpc_ctrl.h5','/dt');
u = h5read('eg_lqmpc_ctrl.h5','/u');
y_true = h5read('eg_lqmpc_ctrl.h5','/y_true');
y_ref = h5read('eg_lqmpc_ctrl.h5','/y_ref');
j = h5read('eg_lqmpc_ctrl.h5','/j');

% Time vector
        n_t = size(y_ref,2);
t = ((1:n_t)-1)*dt;

% Define colors
        c_true = [64, 121, 177; 241, 148, 74; 105, 172, 89] / 255;
c_ref = [235, 71, 58] / 255;
c_input = [64, 121, 177; 241, 148, 74] / 255;
c_cost = [64, 121, 177] / 255;

% Plot the data
        figure;

% First graph
        subplot(311); hold on;
for i = 1:3
    plot(t, y_true(i, :)', 'LineWidth', 2, 'color', c_true(i, :));
end
plot(t, y_ref', 'LineWidth', 2, 'color', c_ref, 'LineStyle', '--');
ylabel({'Output'})

% Second graph
subplot(312); hold on;
for i = 1:2
    stairs(t, u(i, :)', 'LineWidth', 2, 'color', c_input(i, :));
end
ylabel({'Input'})

% Third graph
subplot(313); hold on;
stairs(t, j', 'LineWidth', 2, 'color', c_cost);
ylabel({'Cost'})

% Save the figure
printfig('eg_glds_ctrl_output', 'png', gcf, [8 8]);
