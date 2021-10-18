# %%
import time

import numpy as np

import ldsctrlest
from ldsctrlest import poisson as plds

# for generating random input
def random_walk(n_t, Q, x0):
    n = Q.shape[0]

    if n != Q.shape[1]:
        raise RuntimeError("Q must be `n` x `n`.")

    x = np.zeros((n, n_t))
    x[:, 0] = x0
    for t in range(1, n_t):
        x[:, t] = np.random.multivariate_normal(x[:, t - 1], Q)

    return x


print(" ********** Example Poisson LDS Estimation ********** \n")

# Make SISO system sampled at 1kHz
dt = 1e-3
n_u = 1  # no. inputs
n_x = 1  # no. states
n_y = 1  # no. outputs
n_t = int(30 / dt)  # no. time steps for simulation.

# construct ground truth system...
system_true = plds.System(n_u, n_x, n_y, dt)

# Model parameters
a_true = np.eye(n_x)
a_true[0] = np.exp(-dt / 0.075)
b_true = np.ones((n_x, n_u)) * 1e-2
m0_true = np.ones(n_x) * -7e-2  # disturbance
x0_true = m0_true * np.linalg.inv(np.eye(n_x) - a_true)  # initial state

# Assign params.
system_true.A = a_true
system_true.B = b_true
system_true.x0 = x0_true
system_true.m = m0_true
system_true.Reset()

# Construct system for estimation
# e.g., will create a model with incorrect disturbance
# Can copy parameters from another system object
system_estimator = system_true.copy()

# wrong disturbance
m0_est = m0_true * 2
system_estimator.m = m0_est

# set new initial conditions
x0_est = m0_est * np.linalg.inv(np.eye(n_x) - a_true)  # initial state
system_estimator.x0 = x0_est
system_estimator.Reset()  # reset to initial condition.

# turn on adaptive disturbance estimation
system_estimator.do_adapt_m = True

# set adaptation rate by changing covariance of assumed process noise acting
# on random-walk evolution of m
q_m = np.eye(n_x) * 1e-6
system_estimator.Q_m = q_m

print(".....................................")
print("estimator:")
print(".....................................")
print(system_estimator)
print(".....................................")

# Set up simulation:
# Simulated measurements
z = np.zeros((n_y, n_t))

# Stimulus (generate random stimulus)
q_u = np.eye(n_u) * 1e-3  # cov of random walk
u = random_walk(n_t, q_u, np.zeros(n_x))

# create matrix to save outputs in...
y_hat = np.zeros((n_y, n_t))
y_true = np.zeros((n_y, n_t))

# states and disturbance params
x_hat = np.zeros((n_x, n_t))
m_hat = np.zeros((n_x, n_t))

x_true = np.zeros((n_x, n_t))
m_true = np.zeros((n_x, n_t))

# initial conditions
y_hat[:, 0] = system_estimator.y
y_true[:, 0] = system_true.y
x_hat[:, 0] = system_estimator.x
x_true[:, 0] = system_true.x
m_hat[:, 0] = system_estimator.m
m_true[:, 0] = system_true.m

print(f"Starting {n_t * dt} sec simulation ... ")
start = time.perf_counter()
for t in range(1, n_t):
    # Simlate the true system.
    z[:, t] = system_true.Simulate(u[:, t - 1])

    # Filter (predict -> update)
    system_estimator.Filter(u[:, t - 1], z[:, t])

    # save signals
    y_true[:, t] = system_true.y
    x_true[:, t] = system_true.x
    m_true[:, t] = system_true.m

    y_hat[:, t] = system_estimator.y
    x_hat[:, t] = system_estimator.x
    m_hat[:, t] = system_estimator.m

finish = time.perf_counter()
sim_time_ms = (finish - start) * 1000
print(f"Finished simulation in {sim_time_ms} ms.")
print(f"(app. {sim_time_ms * 1000 / n_t} us/time-step)")

print("fin.")

# %%
# Plotting
import matplotlib.pyplot as plt

t = np.arange(0, n_t * dt, dt)
t_z = t[z.flatten() > 0]
z_t = z.flatten()[z.flatten() > 0]

c_data = 0.25 + np.zeros((1, 3))
c_true = 0.5 + np.zeros((1, 3))
c_est = [0.85, 0.5, 0.85]
c_ref = [0.25, 0.75, 0]

fig, axs = plt.subplots(4, 1, figsize=(8, 8))
axs[0].plot(t_z, z_t * 10, ".", markersize=2, c=c_data)
axs[0].plot(t, y_hat.T / dt, linewidth=2, c=c_est)
# axs[0].plot(t, y_true.T/dt, linewidth=2, c=c_true)
axs[0].legend(["measurements", "estimated output"])  # , "ground truth"])
axs[0].set(ylabel="(events/s)", ylim=(0, 100))

axs[1].plot(t, x_hat.T, linewidth=2, c=c_est)
axs[1].plot(t, x_true.T, linewidth=2, c=c_true)
axs[1].legend(["estimated", "ground truth"])
axs[1].set_ylabel("States\n(a.u.)")

axs[2].plot(t, m_hat.T, linewidth=2, c=c_est)
axs[2].plot(t, m_true.T, linewidth=2, c=c_true)
axs[2].legend(["estimated", "ground truth"])
axs[2].set_ylabel("Disturbance\n(a.u.)")

axs[3].plot(t, u.T, linewidth=2, c=c_data)
axs[3].set(ylabel="Input\n(V)", xlabel="Time (s)")

fig.suptitle("eg_plds_est output")
fig.savefig("eg_plds_est_output.png", bbox_inches="tight", facecolor="w")
print("eg_plds_est_output.png saved")

# %%
# Plot hist
warm = int(np.ceil(5 / dt))
e_y = y_hat[:, warm:] - y_true[:, warm:]
e_x = x_hat[:, warm:] - x_true[:, warm:]
e_y = e_y.flatten()
e_x = e_x.flatten()

fig, axs = plt.subplots(1, 2, figsize=(5, 5))

axs[0].hist(
    e_y / dt, bins=100, edgecolor="none", facecolor=[0.5, 0.5, 0.5], density=True
)
ymax = axs[0].get_ylim()[1]
axs[0].vlines(np.mean(e_y), 0, ymax, linestyle="--", linewidth=1, color="black")
axs[0].set(
    ylim=(0, ymax),
    xlabel="error (events/s)",
    ylabel="pr(error)",
    title="Output\nEstimation Error",
)

axs[1].hist(e_x, bins=100, density=True, facecolor=[.5, .5, .5], edgecolor="none")
ymax = axs[1].get_ylim()[1]
axs[1].vlines(np.mean(e_x), 0, ymax, linestyle="--", linewidth=1, color="black")
axs[1].set(
    ylim=(0, ymax),
    xlabel="error (a.u.)",
    ylabel="pr(error)",
    title="State\nEstimation Error",
)

fig.suptitle("eg_plds_est histogram output")
fig.tight_layout()
fig.savefig("eg_plds_est_output_hist.png", bbox_inches="tight", facecolor="w")
print("eg_plds_est_output_hist.png saved")
