import time
import numpy as np

import ldsctrlest
import ldsctrlest.poisson as plds

print(" ********** Example Poisson LDS Control ********** \n")

# Make SISO system sampled at 1kHz
dt = 1e-3
n_u = 1
n_x = 1
n_y = 1

# no time steps for simulation.
n_t = int(10 / dt)

# Control variables: _reference/target output, controller gains
y_ref0 = np.ones(n_y) * 30 * dt
k_x = np.ones((n_u, n_x)) + 1       # gains on state error
k_inty = np.zeros((n_u, n_y)) + 10  # gains on integrated output error

# Set control type bit mask, so controller knows what to do
control_type = ldsctrlest.kControlTypeIntY  # integral action

# Ground-truth parameters for the controlled system
# (stand-in for physical system to be controlled)
a_true = np.eye(n_x) * 0.986
b_true = np.zeros((n_x, n_u))
b_true[0] = 0.054
x0_true = np.ones(n_x) * np.log(1 * dt)

# Going to simulate a switching disturbance (m) acting on system
which_m = 0  # whether low or high disturbance (0, 1)
m_low = np.log(5 * dt) * (1 - a_true[0])
pr_lo2hi = 1e-3  # probability of going from low to high disturb.
m_high = np.log(20 * dt) * (1 - a_true[0])
pr_hi2lo = pr_lo2hi

m0_true = m_low * np.ones(n_x)
# construct ground truth system to be controlled...
controlled_system = plds.System(n_u, n_x, n_y, dt)

# Assign params.
controlled_system.A = a_true
controlled_system.B = b_true
controlled_system.m = m0_true
controlled_system.x0 = x0_true
# reset to initial conditions
controlled_system.Reset()

print(".....................................")
print("controlled_system:")
print(".....................................")
print(controlled_system)
print(".....................................")

# Create model used for control.
controller_system = controlled_system.copy()

# for this example, assume model correct, except disturbance
m0_controller = np.ones(n_x) * m_low
x0_controller = np.log(y_ref0)
controller_system.m = m0_controller
controller_system.x0 = x0_controller
controller_system.Reset() #reset to new init condition

# adaptively re-estimate process disturbance (m)
controller_system.do_adapt_m = True

# set adaptation rate by changing covariance of assumed process noise
# acting on random-walk evolution of m
q_m = np.eye(n_x) * 1e-5
controller_system.Q_m =q_m

u_lb = 0.0
u_ub = 5.0

# Create the controller
controller = plds.Controller(controlled_system, u_lb, u_ub)

# set controller type
controller.control_type = control_type

# set controller gains
controller.Kc = k_x
controller.Kc_inty = k_inty

# to protect against integral windup when output is consistently above
# target:
tau_awu = 0.1
controller.tau_awu = tau_awu

print(".....................................")
print("controller:")
print(".....................................")
print(controller)
print(".....................................")

# create Matrixrix to save outputs in...
y_ref = np.repeat(y_ref0.reshape((-1, 1)), n_t, axis=1)

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
    if (which_m == 0):  # low disturbance
        if (chance < pr_lo2hi):   # switches low -> high disturbance
            m0_true = np.ones(n_x) * m_high
            which_m = 1
    else:                         # high disturbance
        if (chance < pr_hi2lo):   # switches high -> low disturbance
            m0_true = np.ones(n_x) * m_high
            which_m = 0
    controlled_system.m = m0_true

    # e.g., use sinusoidal reference
    f = 0.5  # freq [=] Hz
    t_vec = np.ones(n_y) * t
    y_ref[:, t] += y_ref0 * np.sin(2*np.pi*f * dt*t - np.pi//4)

    # Simulate the true system.
    z[:, t] = controlled_system.Simulate(u[:, t-1])

    # This method uses a steady-state solution to control problem to calculate
    # x_ref, u_ref from reference output y_ref. Notably, it does this in the
    # log-linear space (i.e., log(y)).
    #
    # Therefore, it is only applicable to regulation problems or cases where
    # reference trajectory changes slowly compared to system dynamics.
    controller.y_ref = y_ref[:, t]
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
