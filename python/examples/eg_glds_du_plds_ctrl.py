"""Example GLDS Control of PLDS where change in control (du) is being
updated, rather than amplitude (u).
"""
# %%
import time

import numpy as np

import ldsctrlest
import ldsctrlest.gaussian as glds
import ldsctrlest.poisson as plds

print(" ********** Example Gaussian LDS du Control of PLDS ********** \n")

# Make 1st-order SISO system, sampled at 1kHz
dt = 1e-3
n_u = 1
n_x = 1
n_y = 1

# number of time steps for simulation.
n_t = int(5 / dt)

# construct ground truth system to be controlled...
# initializes to random walk model with top-most n_y state observed
controlled_system = plds.System(n_u, n_x, n_y, dt)

# Ground-truth parameters for the controlled system
# (stand-in for physical system to be controlled)
a_true = np.eye(n_x)
a_true[0] = np.exp(-dt / 0.01)
b_true = 2.5e-2 * np.ones((n_x, n_u))
# control signal to model input unit conversion e.g., V -> mW/mm2:
g_true = 10 * np.ones(n_y)

# Going to simulate a switching disturbance (m) acting on system
which_m = 0  # whether low or high disturbance (0, 1)
m_low = np.log(dt) * (1 - a_true[0])
# NOTE: disturbance disabled
pr_lo2hi = 0  # 1e-3  # probability of going from low to high disturb.
m_high = np.log(20 * dt) * (1 - a_true[0])
pr_hi2lo = pr_lo2hi

# initially let m be low
m0_true = m_low * np.ones(n_x)
x0_true = np.ones(n_x) * np.log(dt)

# Assign params.
controlled_system.A = a_true
controlled_system.B = b_true
controlled_system.m = m0_true
controlled_system.g = g_true
controlled_system.x0 = x0_true
controlled_system.Reset()

print(".....................................")
print("controlled_system:")
print(".....................................")
print(controlled_system)
print(".....................................")

# Create **incorrect** model used for control.
# (e.g., imperfect model fitting)
b_controller = b_true / 50

# let's assume zero process disturbance initially
# (will be re-estimating)
m_controller = np.zeros(n_x)

# process noise covariance
q_controller = np.eye(n_y) * 1e-8

# output noise covariance
r_controller = np.eye(n_y) * 1e-2

controller_system = glds.System(n_u, n_x, n_y, dt)
controller_system.A = a_true
controller_system.B = b_controller
controller_system.g = g_true
controller_system.m = m_controller
controller_system.Q = q_controller
controller_system.R = r_controller
controller_system.Reset()  # reset to new m

# going to adaptively re-estimate the disturbance
controller_system.do_adapt_m = True

# set adaptation rate by changing covariance of assumed process noise
# acting on random-walk evolution of m
q_m = np.eye(n_x) * 1e-8
controller_system.Q_m = q_m

# create controller
# lower and upper bounds on control signal (e.g., in Volts)
u_lb = 0.0  # [=] V
u_ub = 5.0  # [=] V
controller = glds.Controller(controller_system, u_lb, u_ub)

# Control variables:
# Reference/target output, controller gains
y_ref0 = np.ones(n_y) * 20 * dt

# to design for this example, augmented state with control and made the input
# du; cost on output q_y = 1, on integral output = 1e2, on u = 0, on du =
# 1e-2.
k_x = np.ones((n_u, n_x)) * 2.44     # gains on state error
k_inty = np.ones((n_u, n_y)) * 97.4  # gains on integrated error
k_u = np.ones((n_u, n_u)) * 5.23e-2  # gains on input amp

# set up controller type bit mask so controller knows how to proceed
control_type = 0
# use integral action to minimize DC error
control_type = control_type | ldsctrlest.kControlTypeIntY
# update change in control (LP filters control)
control_type = control_type | ldsctrlest.kControlTypeDeltaU

# set controller type
controller.control_type = control_type

# Let's say these controller gains were designed assuming g was 9 V/(mW/mm2):
g_design = np.ones(n_u) * 10

# Set params.
# **n.b. using arbitrary defaults for Q, R in this example. Really, these
# should be set by users, as they tune characteristics of Kalman filter.
# Users can also choose not to recursively calculate the estimator gain and
# supply it (setKe) instead of covariances.**
controller.y_ref = y_ref0
controller.Kc = k_x
controller.Kc_inty = k_inty
controller.Kc_u = k_u
controller.g_design = g_design

print(".....................................")
print("control system:")
print(".....................................")
print(controller)
print(".....................................")

# set up variables for simulation
# create Matrix to save outputs in...
y_ref = np.ones((n_y, n_t)) * y_ref0[0]

# Simulated measurements
z = np.zeros_like(y_ref)

# simulated control signal ([=] V)
u = np.zeros((n_u, n_t))

# outputs, states and gain/disturbance params
# *_hat indicates online estimates
y_hat = np.zeros((n_y, n_t))
x_hat = np.zeros((n_x, n_t))
m_hat = np.zeros((n_x, n_t))

# *_true indicates ground truth (system being controlled)
y_true = np.zeros((n_y, n_t))
x_true = np.zeros((n_x, n_t))
m_true = np.zeros((n_x, n_t))

# set initial val
y_hat[:, 0] = controller.sys.y
y_true[:, 0] = controlled_system.y

x_hat[:, 0] = controller.sys.x
x_true[:, 0] = controlled_system.x

m_hat[:, 0] = controller.sys.m
m_true[:, 0] = controlled_system.m

print(f"Starting {n_t * dt} sec simulation ... ")
start = time.perf_counter()
for t in range(1, n_t):
    # simulate a stochastically switched disturbance
    chance = np.random.rand()
    if which_m == 0:  # low disturbance
        if chance < pr_lo2hi:  # switches low -> high disturbance
            m0_true = np.ones(n_x) * m_high
            which_m = 1
    else:  # high disturbance
        if chance < pr_hi2lo:  # switches high -> low disturbance
            m0_true = np.ones(n_x) * m_low
            which_m = 0
    controlled_system.m = m0_true

    # input
    u_tm1 = u[:, t - 1]

    # Simulate the true system.
    z[:, t] = controlled_system.Simulate(u_tm1)

    # This method uses a steady-state solution to control problem to calculate
    # x_ref, u_ref from reference output y_ref. Therefore, it is only
    # applicable to regulation problems or cases where reference trajectory
    # changes slowly compared to system dynamics.
    u[:, t] = controller.ControlOutputReference(z[:, t])

    # save the signals
    y_true[:, t] = controlled_system.y
    x_true[:, t] = controlled_system.x
    m_true[:, t] = controlled_system.m

    y_hat[:, t] = controller.sys.y
    x_hat[:, t] = controller.sys.x
    m_hat[:, t] = controller.sys.m

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

fig, axs = plt.subplots(3, 1, figsize=(8, 8))
axs[0].plot(t, z.T, linewidth=0.5, c=c_data)
axs[0].plot(t, y_hat.T, linewidth=2, c=c_est)
axs[0].plot(t, y_ref.T, linewidth=2, c=c_ref)
axs[0].legend(["measurements", "estimated output", "reference"])
axs[0].set(ylabel="(a.u.)", ylim=[-5e-3, 50e-3])

axs[1].plot(t, x_hat.T, linewidth=2, c=c_est)
axs[1].plot(t, np.exp(x_true.T), linewidth=2, c=c_true)
axs[1].legend(["estimated", "ground truth"])
axs[1].set_ylabel("States\n(a.u.)")

# NOTE: disturbance disabled
# axs[2].plot(t, m_hat.T, linewidth=2, c=c_est)
# axs[2].plot(t, m_true.T, linewidth=2, c=c_true)
# axs[2].legend(["estimated", "ground truth"])
# axs[2].set_ylabel("Disturbance\n(a.u.)")

axs[2].plot(t, u.T, linewidth=2, c=c_data)
axs[2].set(ylabel="Input\n(V)", xlabel="Time (s)")

fig.suptitle("eg_glds_du_plds_ctrl output")
fig.savefig("eg_glds_du_plds_ctrl_output.png", bbox_inches='tight', facecolor='w')
print("eg_glds_du_plds_ctrl_output.png saved")
# %%
