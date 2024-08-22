"""Based on matlab/test_glds_fit.m

This script fits a GLDS model to simulated data using subspace
identification, then refines the fit using expectation-maximization.
"""
# %%
# Fit SSID
import time

import numpy as np

import ldsctrlest
from ldsctrlest import gaussian as glds

# create a simple second order model.
dt = 1e-3
A = np.array([[0.95, -0.25], [0, 0.9]])
n_x = A.shape[0]
B = np.array([[0], [0.01]])
C = np.eye(n_x)
n_u = B.shape[1]
n_y = C.shape[0]
sys = glds.System(n_u, n_x, n_y, dt)
sys.A = A
sys.B = B
sys.C = C
d = np.zeros(n_y)

# sys.D = np.zeros((n_y, n_u))  # feedthrough
sys.d = np.zeros((n_y, 1))  # output bias

# process/measurement noise covariances
sys.Q = 1e-4 * np.eye(n_x)
sys.R = 1e-2 * np.eye(n_y)

# initial conditions
sys.x0 = np.zeros((n_x, 1))  # expected initial state
sys.P0 = sys.Q.copy()  # covariance about that state

# process disturbance, input gain, etc.
sys.m = np.zeros((n_x, 1))
sys.g = np.ones((n_u, 1))

# simulate forward and generate training data
np.random.seed(12)  # for reproducibility

n_trials = 15

t = np.arange(0, 5, dt)
n_samp = len(t)

u = []  # will be list of arrays

# making the input some random data.
for trial in range(n_trials):
    u.append(np.zeros((n_u, n_samp)))
    for k in range(1, n_samp):
        u_t = 0.975 * u[trial][:, k - 1] + 1e-1 * np.random.normal(size=n_u)
        u[trial][:, k] = u_t

y, x, z = sys.simulate_block(u)

n_samp_imp = int(np.ceil(0.1 / dt))
y_imp = sys.simulate_imp(n_samp_imp)
t_imp = np.arange(0, n_samp_imp * dt, dt)

# fit model
n_x_fit = n_x
n_h = 50
u_train = ldsctrlest.UniformMatrixList(u, free_dim=2)
z_train = ldsctrlest.UniformMatrixList(z, free_dim=2)
ssid = glds.FitSSID(n_x_fit, n_h, dt, u_train, z_train, d)

start = time.perf_counter()
fit, sing_vals = ssid.Run(ldsctrlest.SSIDWt.kMOESP)
stop = time.perf_counter()
print(f"Finished SSID in {(stop-start)*1000} ms.")

# compare fit to original without state noise
sys_hat = glds.System(fit)
y_hat, x_hat, _ = sys_hat.simulate_block(u)
y_imp_hat = sys_hat.simulate_imp(n_samp_imp)

# %%
# SSID plot singular values & impulse response
import matplotlib.pyplot as plt

fig, axs = plt.subplots(1, 2)
axs[0].semilogy(sing_vals[:n_h], "-o", color=[0.5, 0.5, 0.5])
axs[0].semilogy(sing_vals[:n_h], color="k", linewidth=2)
axs[0].set(ylabel="Singular Values", xlabel="Singular Value Index")

l1 = axs[1].plot(t_imp, y_imp[0].T, "-", c="k", linewidth=2)
l2 = axs[1].plot(t_imp, y_imp_hat[0].T, "-", c=[0.5, 0.5, 0.5], linewidth=2)
axs[1].legend([l1[0], l2[0]], ["ground truth", "estimated"])
axs[1].set(ylabel="Impulse Response (a.u.)", xlabel="Time (s)")
fig.tight_layout()

# %%
# SSID plot var explained
z_a = np.dstack(z)
y_hat_a = np.dstack(y_hat)
# var over dims 1 and 2, time and trial
pve = 1 - np.var(z_a - y_hat_a, axis=(1, 2)) / np.var(z_a, axis=(1, 2))

eg_trial = 0
fig, axs = plt.subplots(3, 1, figsize=(6, 5))

axs[0].plot(t, z[eg_trial][0, :], "k-")
axs[0].plot(t, y_hat[eg_trial][0, :], "-", c="gray", linewidth=2)
axs[0].legend(["measurement", "fit"])
axs[0].set(
    ylabel="Output 1 (a.u.)",
    xlabel="Time (s)",
    title=f"proportion var explained (training): {pve[0]:0.3f}",
)

axs[1].plot(t, z[eg_trial][1, :], "k-")
axs[1].plot(t, y_hat[eg_trial][1, :], "-", c="gray", linewidth=2)
axs[1].set(
    ylabel="Output 2 (a.u.)",
    xlabel="Time (s)",
    title=f"proportion var explained (training): {pve[1]:0.3f}",
)

axs[2].plot(t, u[eg_trial].T, "k")
axs[2].set(ylabel="Input (a.u.)", xlabel="Time (s)")

fig.tight_layout()
fig

# %%
# exit before EM for the sake of a fast test suite
# (EM takes 30 seconds)
quit()

# %%
# Refit by E-M
calc_dynamics = True  # calculate dynamics (A, B mats)
calc_Q = True  # calculate process noise cov (Q)
calc_init = True  # calculate initial conditions
calc_output = True  # calculate output (C)
calc_measurement = True  # calculate output noise (R)
max_iter = 100
tol = 1e-2

em = glds.FitEM(fit, u_train, z_train)

start = time.perf_counter()
fit_em = em.Run(
    calc_dynamics, calc_Q, calc_init, calc_output, calc_measurement, max_iter, tol
)
stop = time.perf_counter()
print(f"Finished EM fit in {(stop-start)*1000} ms.")

# %%
# compare EM fit to original without state noise
sys_hat_em = glds.System(fit_em)
sys_hat_em.Q = np.zeros_like(sys_hat_em.Q)
y_hat_em, x_hat_em, _ = sys_hat_em.simulate_block(u)
y_imp_hat_em = sys_hat_em.simulate_imp(n_samp_imp)

# %%
# Plot E-M smoothed estimates
fig, axs = plt.subplots(2, 1)

axs[0].plot(t, z[eg_trial][0, :], "k-")
axs[0].plot(t, y_hat_em[eg_trial][0, :], "-", c="gray", linewidth=2)
axs[0].legend(["measurement", "EM re-estimated"])
axs[0].set(ylabel="Output (a.u.)", xlabel="Time (s)")

axs[1].plot(t, u[eg_trial].T, "k")
axs[1].set(ylabel="Input (a.u.)", xlabel="Time (s)")

fig.tight_layout()
fig

# %%
# Plot EM-refit impulse response
fig, axs = plt.subplots(1, 2)

l1 = axs[1].plot(t_imp, y_imp[0].T, "-", c="k", linewidth=2)
l2 = axs[1].plot(t_imp, y_imp_hat_em[0].T, "-", c="gray", linewidth=2)
axs[1].legend([l1[0], l2[0]], ["ground truth", "EM re-estimated"])
axs[1].set(ylabel="Impulse Response (a.u.)", xlabel="Time (s)")
fig.tight_layout()
fig

# %%
# Plot E-M var explained
y_hat_em_a = np.dstack(y_hat_em)
# var over dims 1 and 2, time and trial
pve_em = 1 - np.var(z_a - y_hat_em_a, axis=(1, 2)) / np.var(z_a, axis=(1, 2))

fig, axs = plt.subplots(3, 1, figsize=(6, 5))

axs[0].plot(t, z[eg_trial][0, :], "k-")
axs[0].plot(t, y_hat_em[eg_trial][0, :], "-", c="gray", linewidth=2)
axs[0].legend(["measurement", "EM re-estimated"])
axs[0].set(
    ylabel="Output 1 (a.u.)",
    xlabel="Time (s)",
    title=f"EM-refit proportion var explained (training): {pve_em[0]:0.3f}",
)

axs[1].plot(t, z[eg_trial][1, :], "k-")
axs[1].plot(t, y_hat_em[eg_trial][1, :], "-", c="gray", linewidth=2)
axs[1].set(
    ylabel="Output 2 (a.u.)",
    xlabel="Time (s)",
    title=f"EM-refit proportion var explained (training): {pve_em[1]:0.3f}",
)

axs[2].plot(t, u[eg_trial].T, "k")
axs[2].set(ylabel="Input (a.u.)", xlabel="Time (s)")

fig.tight_layout()
fig
