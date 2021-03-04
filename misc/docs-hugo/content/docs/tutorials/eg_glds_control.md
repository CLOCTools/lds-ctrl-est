---
title: GLDS Control
---
# GLDS Control Tutorial

This tutorial shows how to use this library to control a system with a Gaussian LDS controller (`lds::gaussian::ctrl_t`). In place of a physical system, another GLDS model (`lds::gaussian::sys_t`) receives control inputs and provides measurements for the simulated feedback control loop. The controller is assumed to have an imperfect model of the system being controlled (here, a gain mismatch), and there is a stochastic, unmeasured disturbance acting on the system. A combination of integral action and adaptive estimation of this process disturbance is used to perform control.

The full code for this can be found [here](/ldsctrlest/docs/api/examples/eg_glds_ctrl_8cpp-example/).

## Preamble
In addition to including the main `ldsCtrlEst` header, this tutorial will use some shorthand.
```cpp
#include <ldsCtrlEst>

using lds::armaMat;
using lds::armaVec;
using lds::data_t;
using std::cout;
using std::vector;
```

## Creating a simulated system

A first-order single-input/single-output system will be used for the purposes of this demonstration. The simulation will be run at 1 kHz for 5 seconds.
```cpp
  // Make 1st-order SISO system, sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  // no time steps for simulation.
  size_t n_t = static_cast<size_t>(5.0 / dt);
```

When a system is initialized, rather than requiring all parameters to be provided at construction, only the dimensions and sample period are required and a default system is created.
```cpp
  // construct ground truth system to be controlled...
  // initializes to random walk model with top-most n_y state observed
  lds::gaussian::sys_t controlled_system(n_u, n_x, n_y, dt);
```

This default system is a random walk, where the state matrix is identity, the input matrix is zeros, and the top `min(n_x, n_y)` states are observed at the output. i.e., for this example,
{{<katex display>}}
x_{t+1} = x_t + w_t
{{</katex>}}

{{<katex display>}}
y_{t} = x_t \; .
{{</katex>}}


Now, we'll create non-default parameters for this model.

```cpp
  // Ground-truth parameters for the controlled system
  // (stand-in for physical system to be controlled)
  armaMat a_true(n_x, n_x, arma::fill::eye);
  a_true[0] = exp(-dt / 0.01);
  armaMat b_true = armaMat(n_x, n_u).fill(2e-4);
  // control signal to model input unit conversion e.g., V -> mW/mm2:
  armaVec g_true = armaVec(n_y).fill(10.0);
  // output noise covariance
  armaMat r_true = armaMat(n_y, n_y, arma::fill::eye) * 1e-4;
```

As mentioned above, this example will feature a stochastic disturbance. More specifically, a process disturbance will randomly change between two values.
```cpp
  /// Going to simulate a switching disturbance (m) acting on system
  size_t which_m = 0;  // whether low or high disturbance (0, 1)
  data_t m_low = 5 * dt * (1 - a_true[0]);
  data_t pr_lo2hi = 1e-3;  // probability of going from low to high disturb.
  data_t m_high = 20 * dt * (1 - a_true[0]);
  data_t pr_hi2lo = pr_lo2hi;

  // initially let m be low
  armaVec m0_true = armaVec(n_y).fill(m_low);
```

Finally, assign the parameters using corresponding set-methods.
```cpp
  // Assign params.
  controlled_system.setA(a_true);
  controlled_system.setB(b_true);
  controlled_system.setM(m0_true);
  controlled_system.setG(g_true);
  controlled_system.setR(r_true);
```

## Creating the controller
Now, create the controller. Similar to creating a system, most parameters are set to defaults at object construction. In additioin to system dimensions and sample period, the controller needs to know the upper and lower bounds on the control signal, past which the control saturates. If your actuator does **not** saturate somehow, simply set the lower and upper bounds to `-lds::inf` and `lds::inf`, respectively. Simple saturation is currently the only actuator model in this library.

Here, the control signal is command voltage sent to a physical driver (e.g., for an LED). Its limits are 0 to 5 V.
```cpp
  // create controller
  // lower and upper bounds on control signal (e.g., in Volts)
  data_t u_lb = 0.0;  // [=] V
  data_t u_ub = 5.0;  // [=] V
  glds::ctrl_t controller(n_u, n_x, n_y, u_lb, u_ub, dt);
```

Now, assign non-default parameters to this controller. For the sake of this simulation, the input matrix is set to an incorrect value. We also assume that the controller feedback gains were designed with an actuator whose conversion factor from volts to physical units (e.g., mW/mm2 optical intensity) differed from the actuator being used in the current experiment.
```cpp
  // Create **incorrect** model used for control.
  // (e.g., imperfect model fitting)
  armaMat b_controller = armaMat(n_x, n_u).fill(1e-4);

  // Let's say these controller gains were designed assuming g was 9 V/(mW/mm2):
  armaVec g_design = armaVec(n_u).fill(9);

  // let's assume zero process disturbance initially
  // (will be re-estimating)
  armaVec m_controller = armaVec(n_x, arma::fill::zeros);

  // Control variables:

  // if following enabled, adapts set point with re-estimated process
  // disturbance n.b., should not need integral action if this is enabled as the
  // adaptive estimator minimizes DC error
  bool do_adaptive_set_point = false;

  // Reference/target output, controller gains
  // n.b., Can either use armaVec (arma::Col) or std::vector
  armaVec y_ref0 = armaVec(n_y).fill(20.0 * dt);
  // (Can either use arma::Mat or mats flattened into std::vector)
  armaMat k_x = armaMat(n_u, n_x).fill(100);     // gains on state error
  armaMat k_inty = armaMat(n_u, n_y).fill(1e3);  // gains on integrated err

  // setting initial state to target to avoid error at onset:
  armaVec x0 = armaVec(n_x).fill(y_ref0[0]);

  // set up controller type bit mask so controller knows how to proceed
  size_t control_type = 0;
  if (do_adaptive_set_point) {
    // adapt set point with estimated disturbance
    control_type = control_type | lds::CONTROL_TYPE_ADAPT_M;
  } else {
    // use integral action to minimize DC error
    control_type = control_type | lds::CONTROL_TYPE_INTY;
  }

  // Start configuring controller:
  // Adaptively re-estimate process disturance (m)
  controller.adaptM = true;

  // set controller type
  controller.setControlType(control_type);

  // Set params.
  // **n.b. using arbitrary defaults for Q, R in this example. Really, these
  // should be set by users, as they tune characteristics of Kalman filter.
  // Users can also choose not to recursively calculate the estimator gain and
  // supply it (setKe) instead of covariances.**
  controller.setYRef(y_ref0);
  controller.setKc_x(k_x);
  controller.setKc_inty(k_inty);
  controller.setA(a_true);
  controller.setB(b_controller);
  controller.setM(m_controller);
  controller.setG(g_true);
  controller.setGDesign(g_design);
  controller.setX0(x0);
  controller.reset();  // reset to new initial condition

  // set adaptation rate by changing covariance of assumed process noise acting
  // on random-walk evolution of m
  armaMat q_m = armaMat(n_x, n_x, arma::fill::eye) * 1e-6;
  controller.setQ_m(q_m);
```

## Simulating control
In this demonstration, we will use the `steadyState_fbCtrl` method which allows users to simply set the reference output and supply the current measurement `z_t`. It then calculates the steady-state solution for the reference state/input. In this case, the goal is to regulate the output about a constant reference, so it is appropriate here. This method can also be used for time-varying references as long as this variation is slow compared to the dynamics of the system.

The control loop is carried out here in a simple for-loop, where a the controlled system is simulated, a measurement taken, and the control signal updated.
```cpp
    // Simulate the true system.
    controlled_system.setU(u_tm1);
    controlled_system.simPredict();
    armaVec z_t(z.colptr(t), z.n_rows, false, true);
    controlled_system.simMeasurement(z_t);

    // This method uses a steady-state solution to control problem to calculate
    // xRef, uRef from reference output yRef. Therefore, it is only applicable
    // to regulation problems or cases where reference trajectory changes slowly
    // compared to system dynamics.
    controller.steadyState_fbCtrl(z_t);
```

## Example simulation result
Below are example results for this simulation, including outputs, latent states, process disturbance, and the control signal. The controller's online estimates of the output, state, and disturbance are given in purple.

![example control output](/ldsctrlest/eg_glds_ctrl_output.png)
