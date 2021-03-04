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
