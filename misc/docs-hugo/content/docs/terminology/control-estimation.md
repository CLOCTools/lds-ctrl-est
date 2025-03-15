---
title: C&E
---

# Control & Estimation
The control system provided by this library is comprised of a state estimator and a controller. The estimator is responsible for estimating the latent state of the system, given measurements up to and including the current time (i.e., *filtering*). At each time step, the controller then uses the resulting state feedback and an internal model of the system to update the inputs to the process being manipulated.

## State estimation
In general, the filtering performed to estimate the underlying state proceeds recursively by first using the model dynamics to predict the state change at the next time step, followed by updating this prediction when a new measurement is available. For a LDS, this two-step process can be summarized by
{{<katex display>}}
\widehat{\mathbf{x}}_{t|t-1} = \mathbf{A}\widehat{\mathbf{x}}_{t-1|t-1} + \mathbf{B} u_{t-1} + \mathbf{m}_{t-1} \;,
{{</katex>}}

{{<katex display>}}
\widehat{\mathbf{x}}_{t|t} = \widehat{\mathbf{x}}_{t|t-1} + \mathbf{K}^{\rm e}_t \left(\mathbf{z}_t - \widehat{\mathbf{y}}_{t|t-1}\right)\;,
{{</katex>}}

where {{<katex>}} \hat{\left(\cdot\right)}_{t|j} {{</katex>}} indicates an estimate at time {{<katex>}} t {{</katex>}} given data up to time {{<katex>}} j {{</katex>}} inclusive, {{<katex>}} \mathbf{K}^{\rm e} {{</katex>}} is the estimator gain, and

{{<katex display>}} \widehat{\mathbf{y}}_{t|t-1} = h\left( \widehat{\mathbf{x}}_{t|t-1} \right) \; .{{</katex>}}

In the case of GLDS models, the estimator gain (called `Ke` in library) is calculated recursively by [Kalman filtering](https://en.wikipedia.org/wiki/Kalman_filter), which requires knowledge of the process noise and measurement noise covariances (`Q`, `R`) in addition to the system matrices. For time-invariant GLDS models, the infinite horizon solution is often used, so this gain need not be time-varying. Users may instead set its pre-determined value with the `lds::gaussian::System::set_Ke` mutator.

In the case of PLDS models, there is an analogue of the Kalman filter developed for dynamical systems with point-process observations ([Eden et al. 2004](http://www.stat.columbia.edu/~liam/teaching/neurostat-spr11/papers/brown-et-al/eden2004.pdf)). This nonlinear filter recursively updates `Ke` at each time step and requires an estimate of the process noise covariance (`Q`) as well.

## Adaptive estimation of process disturbance
Both the Kalman filter and point-process analogue are model-based; therefore, their performance can be sensitive to model mismatch, whether this be imperfect model fitting or true drifts in system behavior. A practical approach to improving robustness is parameter adaptation. To that end, this library provides dual state-parameter estimation. Specifically, an additive process disturbance (`m`) is adaptively re-estimated when the `lds::System::do_adapt_m` property is set to `true`. This effectively provides integral action on minimizing state estimation error that could either be due to model mismatch or a true disturbance.

When parameter adaptation is enabled, this process disturbance is assumed to vary stochastically on a random walk
{{<katex display>}}
\mathbf{m}_{t} = \mathbf{m}_{t-1} + \mathbf{w}^m_{t-1} \;,
{{</katex>}}
where {{<katex>}} \mathbf{w}^m \sim \mathcal{N}\left(0, \mathbf{Q}_m\right){{</katex>}}. Kalman filtering or the point-process analogue are then used to estimate this disturbance in parallel with the state.

## Control
Given the estimated state, the controller updates the inputs to the system according to the following law:
{{<katex display>}}
\mathbf{u}_{t} = \mathbf{u}^{\rm ref}_t - \mathbf{K}^c_x \left( \widehat{\mathbf{x}}_t - \mathbf{x}^{\rm ref}_t\right)\;,
{{</katex>}}

where {{<katex>}} \left( \cdot \right)^{\rm ref} {{</katex>}} correspond to reference/target signals and {{<katex>}} \mathbf{K}^c_x {{</katex>}} is the state feedback controller gain. Recall that these controller gains are assumed to have been designed before the experiment using, for example, LQR.

If users are employing integral action for more robust tracking at DC and did not use the approach of augmenting the state vector and system matrices accordingly, there is an option to include the integral term as

{{<katex display>}}
\mathbf{u}_{t} = \mathbf{u}^{\rm ref}_t - \mathbf{K}^c_x \left( \widehat{\mathbf{x}}_t - \mathbf{x}^{\rm ref}_t\right) - \mathbf{K}^c_{\rm inty} \sum_{j=1}^{t}\left( \widehat{\mathbf{y}}_j - \mathbf{y}^{\rm ref}_j \right) \;.
{{</katex>}}

An additional option available to users is a control law that updates the *change* in `u`,

{{<katex display>}}
\Delta\mathbf{u}_{t} = -\mathbf{K}^c_u \left(\mathbf{u}_{t-1} - \mathbf{u}^{\rm ref}_{t-1} \right) - \mathbf{K}^c_x \left( \widehat{\mathbf{x}}_t - \mathbf{x}^{\rm ref}_t\right)\;,
{{</katex>}}

{{<katex display>}}
\mathbf{u}_{t} = \mathbf{u}_{t-1} + \Delta\mathbf{u}_{t} \; .
{{</katex>}}

Notice that this takes the form of a first-order difference equation for updating control (*i.e.*, {{<katex>}} \Delta\mathbf{u}_{t} = -\mathbf{K}^c_u \mathbf{u}_{t-1} + \epsilon_{t-1} {{</katex>}}), effectively low-pass filtering the input depending on the characteristics of {{<katex>}} \mathbf{K}^c_u {{</katex>}}. This can be useful in cases where users have designed the controller gains by LQR to minimize *not* the amplitude of the input, but the *change* in input, by augmenting the state vector with the input during LQR design.

Integral action and the {{<katex>}} \Delta \mathbf{u} {{</katex>}} control law can be combined. The library keeps track of the controller type by way of [bit masks]({{<relref "../api/modules/group__control__masks/">}}) which can be bit-wise OR'd to use in combination.

## Calculating reference state-control from output
In cases where an output reference is supplied and the goal is to track either a static or slowly varying output, users do not have to produce {{<katex>}} \mathbf{x}^{\rm ref} {{</katex>}} and {{<katex>}} \mathbf{u}^{\rm ref} {{</katex>}}. Methods are provided for calculating the state and control that would be required to reach the reference output at steady state (`lds::Controller<System>::ControlOutputReference`). This is achieved by linearly-constrained least squares. For single-output systems, it results in an exact solution; however, for multi-output problems it provides a least squares comprimise across outputs.

## Model Predictive Control
Model Predictive Control (MPC) is an advanced control strategy that utilizes a dynamic model of the system to predict and optimize future behavior over a specified time horizon. At each control step, MPC solves an optimization problem to determine the control inputs that minimize a cost function, which typically includes terms for tracking desired reference trajectories and penalizing excessive control efforts. This approach allows MPC to handle multivariable systems with constraints effectively, making it suitable for complex industrial applications.

In the context of linear systems, the optimization problem within MPC can be formulated as a quadratic program. This involves defining a quadratic cost function over the prediction horizon, which balances the trade-off between tracking performance and control effort. The solution to this quadratic program yields the optimal control inputs that drive the system towards the desired state while respecting operational constraints. Tools like the Operator Splitting Quadratic Program (OSQP) solver are often employed to efficiently solve these optimization problems in real-time. 

