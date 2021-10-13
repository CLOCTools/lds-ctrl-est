import time

import numpy as np

import ldsctrlest
import ldsctrlest.gaussian as glds

print(" ********** Example Gaussian LDS Control ********** \n")

# Make 1st-order SISO system, sampled at 1kHz
dt = 1e-3
n_u = 1
n_x = 1
n_y = 1

# number of time steps for simulation.
n_t = 5 / dt

# construct ground truth system to be controlled...
# initializes to random walk model with top-most n_y state observed
controlled_system = glds.System(n_u, n_x, n_y, dt)

# Ground-truth parameters for the controlled system
# (stand-in for physical system to be controlled)
a_true = np.eye(n_x)
a_true[0] = np.exp(-dt / 0.01)
b_true = 2e-4 * np.ones((n_x, n_u))
# control signal to model input unit conversion e.g., V -> mW/mm2:
g_true = 10 * np.ones(n_y)

# output noise covariance
r_true = 1e-4 * np.eye(n_y)

# Going to simulate a switching disturbance (m) acting on system
which_m = 0  # whether low or high disturbance (0, 1)
m_low = 5 * dt * (1 - a_true[0])
pr_lo2hi = 1e-3  # probability of going from low to high disturb.
m_high = 20 * dt * (1 - a_true[0])
pr_hi2lo = pr_lo2hi

# initially let m be low
m0_true = m_low * np.ones(n_x)

# Assign params.
controlled_system.A = a_true
controlled_system.B = b_true
controlled_system.m = m0_true
controlled_system.g = g_true
controlled_system.R = r_true

print(".....................................")
print("controlled_system:")
print(".....................................")
print(controlled_system)
print(".....................................")

# Create **incorrect** model used for control.
# (e.g., imperfect model fitting)
b_controller = b_true / 2

# let's assume zero process disturbance initially
# (will be re-estimating)
m_controller = np.zeros(n_x)

# for this demo, just use arbitrary default R
r_controller = np.eye(n_y) * ldsctrlest.kDefaultR0

controller_system = controlled_system.copy()
controller_system.B = b_controller
assert controller_system.B != controlled_system.B
controller_system.m = m_controller
controller_system.R = r_controller
controller_system.Reset()  # reset to new m
print(controller_system)

# going to adaptively re-estimate the disturbance
controller_system.do_adapt_m = True

# set adaptation rate by changing covariance of assumed process noise
# acting on random-walk evolution of m
q_m = np.eye(n_x) * 1e-6
controller_system.Q_m = q_m

# create controller
# lower and upper bounds on control signal (e.g., in Volts)
u_lb = 0.0  # [=] V
u_ub = 5.0  # [=] V

# make a controller
controller = glds.Controller(controller_system, u_lb, u_ub)

# Control variables:
# if following enabled, adapts set point with re-estimated process
# disturbance n.b., should not need integral action if this is enabled as the
# adaptive estimator minimizes DC error
do_adaptive_set_point = False

# Reference/target output, controller gains
y_ref0 = np.ones(n_y) * 20 * dt
k_x = np.ones(n_u, n_x) * 100     # gains on state error
k_inty = np.ones(n_u, n_y) * 1e3  # gains on integrated error

# setting initial state to target to avoid error at onset:
x0 = np.ones(n_x) * y_ref0[0]

# set up controller type bit mask so controller knows how to proceed
control_type = 0
if (do_adaptive_set_point):
    # adapt set point with estimated disturbance
    control_type = control_type | ldsctrlest.kControlTypeAdaptM
else:
    # use integral action to minimize DC error
    control_type = control_type | ldsctrlest.kControlTypeIntY

# set controller type
controller.control_type = control_type

# Let's say these controller gains were designed assuming g was 9 V/(mW/mm2):
g_design = np.ones(n_u) * 9

# Set params.
# **n.b. using arbitrary defaults for Q, R in this example. Really, these
# should be set by users, as they tune characteristics of Kalman filter.
# Users can also choose not to recursively calculate the estimator gain and
# supply it (setKe) instead of covariances.**
controller.y_ref = y_ref0
controller.Kc = k_x
controller.Kc_inty = k_inty
controller.g_design = g_design

print(".....................................")
print("control system:")
print(".....................................")
print(controller_system)
print(".....................................")

# set up variables for simulation
# create Matrixrix to save outputs in...
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

# get initial val
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
    if (which_m == 0):  # low disturbance
        if (chance < pr_lo2hi):   # switches low -> high disturbance
            m0_true = np.ones(n_x) * m_high
            which_m = 1
    else:                         # high disturbance
        if (chance < pr_hi2lo):   # switches high -> low disturbance
            m0_true = np.ones(n_x) * m_high
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

    y_hat[:, t] = controller.y
    x_hat[:, t] = controller.x
    m_hat[:, t] = controller.m

finish = time.perf_counter()
sim_time_ms = (finish - start) * 1000
print(f"Finished simulation in {sim_time_ms} ms.")
print(f"(app. {sim_time_ms / n_t} us/time-step")

print("fin.")
