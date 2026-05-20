# Double Pendulum Simulation in C++ with SDL3

## Goals

This project started with a dual motivation. First, a deep curiosity for physical systems and the desire to build a pure, deterministic chaotic simulator from scratch. Second, a perfect opportunity to experiment with **WebAssembly (WASM) compilation**. The goal was to write raw, high-performance C++ code using **SDL3** and deploy it directly into modern web browsers with near-native performance, leaving all graphics computation to the GPU via WebGL.

---

## Features

* **Pure C++ Physics Engine:** Exact Lagrangian mechanics solved in real-time.
* **The Butterfly Effect in Action:** Simulates 3 independent pendulums simultaneously to witness chaotic divergence.
* **Persistent Visual Trails:** Renders thousands of past frames into a non-fading geometric fractal without dragging down the CPU.
* **WASM & Native Ready:** Compiles into standard desktop executables or cross-platform web bundles using Emscripten.

---

## Under the Hood: The Physics & The "Cheat"

### 1. The Equations of Motion (Lagrangian Mechanics)
A double pendulum cannot be easily solved with standard Newtonian forces ($F=ma$) because of the constraints between the two bars. Instead, this engine uses **Lagrangian Mechanics**, focusing on the total Kinetic Energy ($T$) and Potential Energy ($V$) of the system:

$$L = T - V$$

By applying the Euler-Lagrange equations, we obtain two highly non-linear, coupled second-order differential equations that calculate the angular accelerations ($\alpha_1$ and $\alpha_2$) at any given frame based on the current angles ($\theta_1, \theta_2$) and angular velocities ($\omega_1, \omega_2$). 

### 2. Numerical Integration & The Damping Problem
To compute the next position, the engine relies on a **Semi-Implicit Euler Integrator**:

* $\omega_{next} = \omega + \alpha \cdot dt$
* $\theta_{next} = \theta + \omega_{next} \cdot dt$

While incredibly fast and perfect for real-time video games, this numerical approximation introduces tiny floating-point rounding errors at every single millisecond. In a closed loop, these errors act as an artificial, invisible friction. Over time, the system bleeds its initial kinetic energy, forcing the pendulums to slow down and eventually drop into a repetitive, stable orbit (a predictable "sink hole" loop).

### 3. The Anti-Dissipation Trick (The "Cheat")
To counteract this artificial numerical damping and keep the simulation chaotic forever, a tiny "kick" of energy is injected back into the angular velocities at the end of every physics step:

```cpp
omega1 *= 1.0001;
omega2 *= 1.0001;
```

---
## Compilation

### Prerequisites
You need 
* g++ compatible for C++11
* SDL3
* Cmake
* emscripten


### compile the native application

#### Create the build native folder

```bash
mkdir build_native && cd build_native
```

#### Run Standard Cmake and build

```bash
cmake ..
cmake --build .
```

#### Run the executable 

```bash
./pendulum_native
```

### compile the web application

#### Create your web build folder

```bash
mkdir build_web && cd build_web

```

#### Configure CMake with your local Emscripten toolchain path 
example for Mac Intel (adjust the path of your Emscripten.cmake ):

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=/usr/local/Cellar/emscripten/4.0.17/libexec/cmake/Modules/Platform/Emscripten.cmake
```

#### Compile the project

```bash
cmake --build .
```

#### Fire up a local server to bypass browser CORS security:

   ```bash
   python3 -m http.server 8080