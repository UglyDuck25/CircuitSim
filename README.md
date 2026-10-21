# Circuit Simulation
## A parralel Circuit Simulation and Optimization engine

## Author
Samuel Olukuewu

## Overview
This is a C project that is supposed to act as a simplified eda tool, By taking in The design of the circuit as a formula and the parameters for said Circuit. It will then run a simulation an X number of times then produce potential optimizations that can be made to said parameters.

### Core Functionality
- Circuit simulation engine that accepts circuit designs as formulas and parameters.
- Multi-run simulation capability (X iterations).
- Optimization analysis and suggestions for circuit parameters.

### Technical Stack
- **Language**: C
- **Concurrency**: MultiThreading for parallel circuit simulations
- **Communication**: UDP and TCP sockets for inter-node communication

## Project Objectives

1. **Performance**: Leverage parallel processing to simulate multiple circuit configurations simultaneously
2. **Scalability**: Support distributed simulation across multiple modules via network communication
3. **Optimization**: Provide intelligent parameter optimization based on simulation results
4. **Simplicity**: Maintain a clean, extensible architecture for future enhancements

## Architecture Overview

### High-Level Design
```
┌─────────────────────────────────────────────────┐
│           CLI / Configuration Layer              │
└──────────────────┬──────────────────────────────┘
                   │
        ┌──────────┴──────────┐
        │                     │
┌───────▼────────┐  ┌────────▼─────────┐
│ Circuit Parser │  │ Simulation Engine │
└────────────────┘  └────────┬──────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
┌───────▼────────┐  ┌────────▼──────┐  ┌────────▼──────┐
│ Thread Pool    │  │ Optimizer     │  │ Network Layer │
│ (Parallel Sim) │  │ (Parameter    │  │ (UDP/TCP)     │
│                │  │  Analysis)    │  │               │
└────────────────┘  └───────────────┘  └────────────────┘
```

## Module Specifications

### 1. Circuit Module (`src/circuit/`)
**Responsibility**: Circuit representation, parsing, and validation

**Key Components**:
- **circuit.h/c**: Core data structures and operations
  - Circuit definition structure
  - Component models (resistors, capacitors, inductors, voltage/current sources)
  - Parameter validation and bounds checking
- **circuit_parser.h/c**: Formula parsing engine
  - String-to-AST conversion
  - Expression evaluation
  - Support for arithmetic and logical operations

**Data Structures**:
```c
typedef struct {
    char *name;
    double value;
    double min_bound;
    double max_bound;
} Parameter;

typedef struct {
    Parameter *parameters;
    int param_count;
    // Circuit topology and component definitions
} Circuit;
```

### 2. Simulation Module (`src/simulation/`)
**Responsibility**: Single and batched circuit simulation

**Key Components**:
- **simulator.h/c**: Core simulation engine
  - Transient analysis
  - Steady-state analysis
  - Parameter sweep simulation
  - Result collection and aggregation

**Simulation Methods**:
- Nodal analysis for DC/transient circuits
- Time-stepping integration (Runge-Kutta or backward Euler)
- Parameter variation for sensitivity analysis

### 3. Threading Module (`src/threading/`)
**Responsibility**: Parallel simulation management

**Key Components**:
- **thread_pool.h/c**: Work queue-based thread pool
  - Configurable worker thread count
  - Lock-free or mutex-based queue management
  - Dynamic task distribution
- **sync.h/c**: Synchronization primitives
  - Barriers for multi-phase operations
  - Atomic operations for result aggregation

**Thread Model**:
- Master thread: Coordinates simulation campaigns
- Worker threads: Execute individual simulations
- Aggregator thread: Collects and processes results

### 4. Networking Module (`src/networking/`)
**Responsibility**: Distributed simulation support

**Key Components**:
- **socket_utils.h/c**: Socket abstraction layer
  - UDP client/server implementation
  - TCP connection management
  - Message serialization/deserialization
- **message.h/c**: Protocol definition
  - Simulation task distribution messages
  - Result collection messages
  - Heartbeat and health checks

**Communication Protocol**:
- **UDP**: For lightweight task distribution and result broadcasting
- **TCP**: For reliable circuit definition and configuration exchange

**Message Format** (Binary):
```
[HEADER(4)] [MSG_TYPE(1)] [PAYLOAD_SIZE(4)] [PAYLOAD] [CHECKSUM(4)]
```

### 5. Optimization Module (`src/optimization/`)
**Responsibility**: Parameter optimization based on simulation results

**Key Components**:
- **optimizer.h/c**: Optimization algorithms
  - Grid search for coarse exploration
  - Gradient descent for fine-tuning
  - Genetic algorithms for complex landscapes
  - Particle swarm optimization (PSO)
- **fitness.h/c**: Objective function definitions
  - Performance metrics (power, speed, area)
  - Custom user-defined objectives
  - Multi-objective optimization support

**Optimization Workflow**:
1. Generate initial parameter set
2. Simulate all variants in parallel
3. Evaluate fitness for each configuration
4. Generate new candidates based on best performers
5. Repeat until convergence or iteration limit

### 6. Utility Module (`src/utils/`)
**Responsibility**: Common services and infrastructure

**Key Components**:
- **logger.h/c**: Structured logging
  - Debug, info, warning, error levels
  - File and console output
  - Performance profiling utilities
- **config.h/c**: Configuration management
  - INI/JSON configuration parsing
  - Runtime parameter validation
  - Default configuration sets
- **memory.h/c**: Memory management
  - Custom allocators for optimization
  - Memory pooling for simulation results

## Configuration & Usage

### Configuration File Format (YAML/JSON)
```yaml
project:
  name: "example_circuit"
  description: "Basic RC filter circuit"

circuit:
  definition: "V(out) = V(in) * 1/(1 + jwRC)"
  parameters:
    - name: R
      value: 1000
      min: 100
      max: 10000
      unit: "ohms"
    - name: C
      value: 1e-6
      min: 1e-9
      max: 1e-3
      unit: "farads"

simulation:
  type: "ac_analysis"
  iterations: 1000
  frequency_range: [1, 1e6]
  num_threads: 8

optimization:
  algorithm: "genetic"
  objective: "minimize_power"
  generations: 50
  population_size: 100
```

### Command-Line Interface
```bash
./circuit_sim --config config.yaml --output results.json --verbose
./circuit_sim --simulate --params "R=1000,C=1e-6"
./circuit_sim --optimize --algorithm genetic --iterations 100
```

## Building & Dependencies

### Requirements
- **C99 or later** compiler (GCC, Clang)
- **POSIX-compliant OS** (Linux, macOS, WSL on Windows)
- **pthreads** for multithreading
- Optional: **CMake 3.10+** for build configuration

### Build Instructions
```bash
mkdir build && cd build
cmake .. -DENABLE_OPTIMIZATION=ON -DENABLE_NETWORKING=ON
make
./circuit_sim
```

### Optional Dependencies
- **MPI**: For distributed computing across clusters
- **HDF5**: For large-scale result storage
- **BLAS/LAPACK**: For advanced linear algebra

## Development Roadmap

### Phase 1: Core Simulation (Weeks 1-4)
- [ ] Circuit data structure and representation
- [ ] Formula parsing engine
- [ ] Basic nodal analysis simulator
- [ ] Unit tests for circuit components

### Phase 2: Parallelization (Weeks 5-7)
- [ ] Thread pool implementation
- [ ] Parallel simulation execution
- [ ] Result aggregation
- [ ] Performance profiling

### Phase 3: Networking (Weeks 8-10)
- [ ] Socket abstraction layer
- [ ] UDP/TCP message protocol
- [ ] Distributed simulation coordination
- [ ] Network-based result collection

### Phase 4: Optimization (Weeks 11-13)
- [ ] Optimization algorithm implementations
- [ ] Fitness evaluation framework
- [ ] Parameter variation generation
- [ ] Result analysis and reporting

### Phase 5: Integration & Refinement (Weeks 14-16)
- [ ] Configuration system
- [ ] CLI interface
- [ ] Documentation and examples
- [ ] Performance optimization
- [ ] Stress testing and reliability improvements

## Performance Considerations

- **Thread Count**: Auto-detect CPU cores, allow manual override
- **Memory Usage**: Stream results to disk for large simulation batches
- **Network Bandwidth**: Implement compression for result transmission
- **Load Balancing**: Dynamic work distribution to prevent idle threads

## Testing Strategy

- **Unit Tests**: Individual module functionality (circuit, simulator, optimizer)
- **Integration Tests**: Multi-module interactions
- **Performance Tests**: Scaling benchmarks with varying thread counts
- **Regression Tests**: Automated test suite for ongoing development

## Project Outline
