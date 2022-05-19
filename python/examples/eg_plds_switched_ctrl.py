"""Example Switched PLDS Control"""
# %%
import time

import numpy as np

import ldsctrlest
import ldsctrlest.poisson as plds

print(" ********** Example Switched Poisson LDS Control ********** \n")

# whether to do switched control
do_switch_ctrl = True

# Make 1st-order SISO system, sampled at 1kHz
dt = 1e-3
n_u = 1
n_x = 1
n_y = 1

# number of time steps for simulation.
n_t = int(30 / dt)

# for simulating switching
which_mode = 1
pr_21 = 1e-3   # prob mode 1 -> 2
pr_12 = pr_21  # prob mode 2 -> 1

# simulated system being controlled
controlled_system = plds.System(n_u, n_x, n_y, dt)

# **Assume the system is not well characterized by one LDS, but is well
# characterized by two LDS models with different input matrices.**
scale_sys_b = 2

# Ground-truth parameters for the controlled system
# (stand-in for physical system to be controlled)
a = np.eye(n_x)
a[0] = 0.985
b1 = np.ones((n_x, n_u)) * 0.05
d = np.ones(n_y) * np.log(dt)

controlled_system.A = a
controlled_system.B = b1
controlled_system.d = d
controlled_system.Reset()  # reset to initial conditions

# reference
y_ref0 = np.ones(n_y) * 25 * dt

# Let underlying system 1 be more sensitive than system 2
b2 = np.ones((n_x, n_u)) * b1[0] / scale_sys_b

print(".....................................")
print("controlled_system:")
print(".....................................")
print(controlled_system)
print(".....................................")

# --------------- Set up controller -----------------
# ---------------------------------------------------

# create switched controller sub-systems
# system 1
sys1 = controlled_system.copy()

# set process noise covariance
q_controller = np.eye(n_x) * 5e-3
sys1.Q = q_controller

# adaptively estimate process disturbance (m)
# n.b. using arbitrary default value for process noise if enabled.
sys1.do_adapt_m = True

# setting initial mode to target to avoid large error at onset:
x0_controller = np.log(y_ref0) - d
sys1.x0 = x0_controller
sys1.Reset()  # reset to initial conditions

print(".....................................")
print("sys1:")
print(".....................................")
print(sys1)
print(".....................................")

# system 2
sys2 = sys1.copy()

# set parameters
sys2.B = b2

print(".....................................")
print("sys2:")
print(".....................................")
print(sys2)
print(".....................................")

systems = [sys1, sys2]
# systems = ldsctrlest.UniformSystemList([sys1, sys2])

# controller gains for underlying system s:
k_x1 = np.ones((n_u, n_x))
k_x2 = scale_sys_b * k_x1  # system2 is x-times less sensitive.
k_x = ldsctrlest.UniformMatrixList([k_x1, k_x2])

u_lb = 0.0
u_ub = 5.0
switched_controller = plds.SwitchedController(systems, u_lb, u_ub)

# Control variables
control_type = 0;  # no integral action, etc
switched_controller.control_type = control_type
switched_controller.Kc = k_x

switched_controller.y_ref = y_ref0

print(".....................................")
print("switched_controller:")
print(".....................................")
print(switched_controller)
print(".....................................")

# set up variables for simulation

# Fake measurements
z = np.zeros((n_y, n_t))

# simulated control signal ([=] V)
u = np.zeros((n_u, n_t))

# create Matrix to save outputs in...
y_hat = np.zeros((n_y, n_t))
y_true = np.zeros((n_y, n_t))
y_ref = np.ones((n_y, n_t)) * y_ref0[0]

# modes and gain/disturbance params
x_hat = np.zeros((n_x, n_t))
x_true = np.zeros((n_x, n_t))
mode = np.ones((1, n_t))

# set initial val
y_hat[:, 0] = switched_controller.sys.y
y_true[:, 0] = controlled_system.y
x_hat[:, 0] = switched_controller.sys.x
x_true[:, 0] = controlled_system.x

print(f"Starting {n_t * dt} sec simulation ... ")
start = time.perf_counter()
for t in range(1, n_t):
    # simulate a stochastically switched disturbance
    chance = np.random.rand()
    if which_mode == 1:  # mode1
        if chance < pr_21:
            which_mode = 2
            controlled_system.B = b2
            if do_switch_ctrl:
                switched_controller.Switch(1)
    else:  # mode2
        if chance < pr_12:
            which_mode = 1
            controlled_system.B = b1
            if do_switch_ctrl:
                switched_controller.Switch(0)

    # Simulate the true system.
    z[:, t] = controlled_system.Simulate(u[:, t - 1])

    # perform control
    u[:, t] = switched_controller.ControlOutputReference(z[:, t])

    mode[:, t] = which_mode
    y_ref[:, t] = y_ref0
    y_true[:, t] = controlled_system.y
    x_true[:, t] = controlled_system.x
    y_hat[:, t] = switched_controller.sys.y
    x_hat[:, t] = switched_controller.sys.x

finish = time.perf_counter()
sim_time_ms = (finish - start) * 1000
print(f"Finished simulation in {sim_time_ms} ms.")
print(f"(app. {sim_time_ms * 1000 / n_t} us/time-step)")

print("fin.")

# %%
# Plotting
import matplotlib.pyplot as plt

t = np.arange(0, n_t * dt, dt)
c_data = 0.25 + np.zeros((1, 3))
c_true = 0.5 + np.zeros((1, 3))
c_est = [0.85, 0.5, 0.85]
c_ref = [0.25, 0.75, 0]

fig, axs = plt.subplots(4, 1, figsize=(8, 8))
axs[0].plot(t, z.T, ".", markersize=0.5, c=c_data)
axs[0].plot(t, y_hat.T/dt, linewidth=2, c=c_est)
axs[0].plot(t, y_ref.T/dt, linewidth=2, c=c_ref)
axs[0].plot(t, y_true.T/dt, linewidth=2, c=c_true)
axs[0].legend(["measurements", "estimated output", "reference", "ground truth"])
axs[0].set_ylabel("(events/s)")

axs[1].plot(t, x_hat.T, linewidth=2, c=c_est)
axs[1].plot(t, x_true.T, linewidth=2, c=c_true)
axs[1].legend(["estimated", "ground truth"])
axs[1].set_ylabel("States\n(a.u.)")

axs[2].plot(t, mode.T, linewidth=2, c=c_true)
axs[2].set(ylabel="Mode", ylim=(0, 3))

axs[3].plot(t, u.T, linewidth=2, c=c_data)
axs[3].set(ylabel="Input\n(V)", xlabel="Time (s)")

fig.suptitle("eg_plds_switched_ctrl output")
fig.savefig("eg_plds_switched_ctrl.png", bbox_inches='tight', facecolor='w')
print("eg_plds_switched_ctrl.png saved")
# %%
