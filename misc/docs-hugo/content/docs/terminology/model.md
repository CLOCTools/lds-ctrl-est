---
title: Models
---

# Model Definitions
This library provides methods for control and estimation of linear dynamical systems (LDS) of the following form:
{{< katex display >}}
\mathbf{x}_{t+1} = f\left( \mathbf{x}_{t}, \mathbf{v}_{t} \right) = \mathbf{A} \mathbf{x}_{t} + \mathbf{B} \mathbf{v}_{t} + \mathbf{m}_{t} + \mathbf{w}_{t}
{{< /katex >}}

{{< katex display >}}
\mathbf{y}_{t} = h\left( \mathbf{x}_{t} \right)
{{< /katex >}}

    t           : time index
    x           : system state
    v = g%u     : input (e.g., in physical units used for model fit)
    u           : control signal sent to actuator (e.g., in Volts)
    y           : system output
    m           : process disturbance
    w ~ N(0, Q) : process noise/disturbance

    A           : state matrix
    B           : input coupling matrix
    g           : input gain (e.g., for converting to control signal actuator voltage)
                  n.b., assumes this conversion is linear
    Q           : process noise covariance

    %           : element-wise multiplication


## LDS with Gaussian Observations
For linear dynamical systems whose outputs are assumed to be corrupted by additive Gaussian noise before measurement (Gaussian LDS models), the output function takes the following form.

{{< katex display >}}
\mathbf{y}_{t} = \mathbf{C} \mathbf{x}_{t} + \mathbf{d}
{{< /katex >}}

{{< katex display >}}
\mathbf{z}_{t} \sim \mathcal{N}\left(\mathbf{y}_{t} , \mathbf{R} \right)
{{< /katex >}}

    z           : measurement

    C           : output matrix
    d           : output bias
    R           : measurement noise covariance

## LDS with Poisson Observations
For linear dynamical systems whose outputs are assumed to be rates underlying measured count data derived from a Poisson distribution (Poisson LDS models), the output function takes the following form. Note an element-wise exponentiation is used to rectify the linear dynamics for the rate of the Poisson process.

{{< katex display >}}
y_{t}^{i} = \exp \left(\mathbf{c}^i \mathbf{x}_{t} + d^i\right)
{{< /katex >}}

{{< katex display >}}
z_{t}^i \sim \rm{Poisson} \left(y_{t}^i \right)
{{< /katex >}}

    i           : output index

    z           : measurement (count data)

    c           : i^th row of output matrix (C)
    d           : output bias


## Model Predictive Control (MPC)
Model Predictive Control (MPC) is an advanced control strategy that utilizes a dynamic model of the system to predict and optimize future behavior over a specified time horizon. At each control step, MPC solves an optimization problem to determine the control inputs that minimize a cost function, which typically includes terms for tracking desired reference trajectories and penalizing excessive control efforts. This approach allows MPC to handle multivariable systems with constraints effectively, making it suitable for complex industrial applications.

In the context of linear systems, the optimization problem within MPC can be formulated as a quadratic program. This involves defining a quadratic cost function over the prediction horizon, which balances the trade-off between tracking performance and control effort. The solution to this quadratic program yields the optimal control inputs that drive the system towards the desired state while respecting operational constraints. Tools like the Operator Splitting Quadratic Program (OSQP) solver are often employed to efficiently solve these optimization problems in real-time.
