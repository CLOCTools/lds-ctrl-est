---
title: PLDS Switched Control
---
# PLDS Switched Control Tutorial

This tutorial shows how to use this library to control a system with a switched PLDS controller (`lds::poisson::sctrl_t`). This type of controller is applicable in scenarios where a physical system is **not** accurately captured by a LDS but has multiple discrete operating modes where the dynamics **can** be well-approximated as linear.

In the example that follows, another PLDS model (`lds::poisson::sys_t`) is used in place of a physical system. It receives control inputs and provides measurements for the simulated feedback control loop. This system stochastically flips between two input gains. In this example, the controller is assumed to have a perfect model of the switching system being controlled. Note that in practice, users would need to have a decoder that estimates operating mode of the physical system being controlled. This library does not currently include operating mode estimation.

The full code for this can be found [here](/ldsctrlest/docs/api/examples/eg_plds_switched_log_lin_ctrl_8cpp-example/).

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

## Creating the simulated system

A first-order single-input/single-output system will be used for the purposes of this demonstration. The simulation will be run at 1 kHz for 30 seconds.
```cpp
  // Make SISO system sampled at 1kHz
  data_t dt = 1e-3;
  size_t n_u = 1;
  size_t n_x = 1;
  size_t n_y = 1;

  // no time steps for simulation.
  size_t n_t = static_cast<size_t>(30.0 / dt);
```

The system's input matrix (`B`) will be switched stochastically from one value (`b1`) to a less sensitive value (`b2`) according to the following probabilities.
```cpp
  // for simulating switching
  size_t which_mode = 1;
  data_t pr_21 = 1e-3; // prob mode 1 -> 2
  data_t pr_12 = pr_21; // prob mode 2 -> 1
```

Initially, the system will be in "mode" 1, where `B = b2`.

```cpp
  // simulated system being controlled
  lds::poisson::sys_t controlled_system(n_u, n_x, n_y, dt);

  // **Assume the system is not well characterized by one LDS, but is well
  // characterized by two LDS models with different input matrices.**

  armaMat a(n_x, n_x, arma::fill::eye);
  a[0] = 0.985;
  armaMat b1 = armaMat(n_x, n_u).fill(0.05);
  armaVec d = armaVec(n_y, arma::fill::zeros).fill(log(1 * dt));

  controlled_system.setA(a);
  controlled_system.setB(b1);
  controlled_system.setD(d);
  controlled_system.reset(); // reset to initial conditions
```

See the [GLDS Control]({{<relref "eg_glds_control">}}) and [PLDS State Estimation]({{<relref "eg_plds_state_estimation">}}) tutorials for more detail about `sys_t` creation.

## Creating the controller
Now, create the controller. A switched-system controller (`sctrl_t`) essentially toggles between the parameters of its subsystems when the controller is told a switch has occured. The first thing the user needs to do is define these subsystems. In this example, there are two Poisson systems (`sys1`, `sys2`), which are the same save for their input gains.

```cpp
  // create switched controller sub-systems
  // system 1
  data_t u_lb = 0.0;
  data_t u_ub = 5.0;
  lds::poisson::ctrl_t sys1(n_u, n_x, n_y, u_lb, u_ub, dt);

  // Control variables
  armaVec y_ref0 = armaVec(n_y, arma::fill::zeros).fill(25.0 * dt);
  // controller gains for underlying system 1:
  armaMat k_x1(n_u, n_x, arma::fill::ones);
  size_t control_type = 0;  // no integral action, etc.

  // setting initial mode to target to avoid large error at onset:
  armaVec x0_controller = arma::log(y_ref0) - d;

  armaMat q_controller = armaMat(n_x, n_x, arma::fill::eye) * 5e-3;

  sys1.setControlType(control_type);
  sys1.setA(a);
  sys1.setB(b1);
  sys1.setD(d);
  sys1.setX0(x0_controller);
  sys1.setKc_x(k_x1);
  sys1.setQ(q_controller);  // set process noise covariance

  // system 2
  lds::poisson::ctrl_t sys2 = sys1;

  // Let underlying system 1 be more sensitive than system 2
  data_t scale_sys_b = 2;
  armaMat b2 = armaMat(n_x, n_u).fill(b1[0] / scale_sys_b);
  armaMat k_x2 = scale_sys_b * k_x1;  // system2 is x-times less sensitive.

  // set parameters
  sys2.setB(b2);
  sys2.setKc_x(k_x2);
```

Now that the two subsystems are defined, instantiate the switched controller and set its members. Importantly, control references "belong" to the switched controller, so they need to be assigned to it, rather than its subsystems.
```cpp
  // create 2-system composite switched-mode controller.
  lds::poisson::sctrl_t switched_controller(2, n_u, n_x, n_y, u_lb, u_ub, dt);
  // assign the parameters of the sub-systems 1 and 2
  switched_controller.setSystem(0, sys1);
  switched_controller.setSystem(1, sys2);
  switched_controller.switchSystem(0);  // start with system 1
  switched_controller.reset();          // reset to initial conditions

  // reference belongs to super system. Set here.
  switched_controller.setYRef(y_ref0);

  // adaptively estimate process disturbance (m)
  // n.b. using arbitrary default value for process noise if enabled.
  switched_controller.adaptM = true;
```

## Simulating control
In this demonstration, we will use the `steadyState_logLin_fbCtrl` method which allows users to simply set the reference output event rate (`y_ref`) and supply the current measurement `z_t`. It then calculates the steady-state solution for the reference state/input. In this case, the goal is to regulate the output about a constant reference, so it is appropriate here. This method can also be used for time-varying references as long as this variation is slow compared to the dynamics of the system. Importantly, this method performs control in the linear state space (i.e., taking the logarithm of the output).

The control loop is carried out here in a simple for-loop, where a the controlled system is simulated, a measurement taken, and the control signal updated.
```cpp
    // Simulate the true system.
    controlled_system.setU(u_tm1);
    controlled_system.simPredict();
    armaVec z_k = armaVec(z.colptr(t), z.n_rows, false, true);
    controlled_system.simMeasurement(z_k);

    // perform control
    switched_controller.steadyState_logLin_fbCtrl(z_k);
```

Note that as the gain of the controlled system changes stochastically, the controller is informed of this change. In practice, a user must decode such changes in the system's operating mode and call the `switchSystem` method accordingly. Such a decoder is not currently included in this library.

## Example simulation result
Below are example results for this simulation, including outputs, latent states, mode switches, and the control signal. The controller's online estimates of the output and state are shown in purple.

![example control output](/ldsctrlest/eg_plds_switched_ctrl_output.png)

Note that every time the operating mode of the system (here, a gain change) the controller immediately adjusts its inputs. In contrast, a non-switched controller with integral action would also compensate but do so in a comparitively sluggish fashion.
