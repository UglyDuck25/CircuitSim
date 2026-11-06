# Simplified EDA Circuit Simulation and Optimization Tool

## Project Overview

This project is a command-line Electronic Design Automation (EDA)
application written in **C**. It is intended to provide a simplified
workflow for describing an electrical circuit, simulating its behaviour,
running repeated simulations, and identifying component-value changes
that may improve a chosen performance objective.

The application will use a **text-based netlist** as its circuit input
format. Rather than drawing a schematic, a user describes components and
the circuit nodes to which their terminals are connected. The program
parses that description and builds an internal representation of the
circuit.

The long-term goal is to create a useful, demonstrable circuit-analysis
and optimization tool while also applying core computer-science concepts
such as parsing, data structures, memory management, numerical
algorithms, testing, and modular software design.

## Main Goals

1.  **Describe circuits:** Accept a readable netlist containing
    component declarations, node connections, values, and (eventually)
    parameter constraints.
2.  **Parse circuit descriptions:** Convert netlist text into structured
    C data.
3.  **Validate circuits:** Detect malformed lines, duplicate component
    names, undefined or invalid values, missing references, and other
    unsupported or invalid configurations.
4.  **Simulate circuits:** Build and solve the mathematical model of
    supported circuits to calculate quantities such as node voltages and
    component currents.
5.  **Run repeated simulations:** Evaluate a circuit under different
    component values or operating conditions.
6.  **Optimize parameters:** Search for component values that best
    satisfy a user-defined objective while respecting constraints.
7.  **Report results:** Present the original and best-found
    configurations, performance measurements, and simulation data.

## Initial Scope

The first version will focus on **DC circuits containing resistors and
independent sources**.

Initial supported elements:

-   Resistors
-   Independent DC voltage sources
-   Independent DC current sources
-   Ground (node `0`, used as the reference node)

Capacitors, inductors, diodes, transistors, AC analysis, and time-domain
transient analysis are possible future extensions. They are not required
for the first working version.

## Example Netlist

A simple voltage divider can be described as:

``` text
* Simple voltage divider
V1 N1 0 DC 12
R1 N1 N2 1000
R2 N2 0 2000
.END
```

In this example:

-   `V1` is a 12-volt DC voltage source connected between nodes `N1` and
    `0`.
-   `R1` is a 1000-ohm resistor between `N1` and `N2`.
-   `R2` is a 2000-ohm resistor between `N2` and ground.
-   `0` is the reference (ground) node.
-   Lines beginning with `*` are comments.
-   `.END` marks the end of the netlist.

The exact grammar, accepted units, naming rules, and error behaviour
are currently limited to the following parser input:

- Resistors use `Rname node1 node2 integer-value`.
- Independent DC voltage and current sources use
  `Vname/Iname node1 node2 DC integer-value`.
- Component names must be unique; resistor values must be positive.


## Intended Application Workflow

1.  The user supplies a netlist file.
2.  The input module reads the file.
3.  The parser tokenizes and interprets each supported line.
4.  The circuit model stores components, nodes, values, and
    connectivity.
5.  The validator checks the model and reports errors or warnings.
6.  The simulator constructs the equations for the supported circuit
    model.
7.  A numerical solver calculates circuit results.
8.  The simulation runner repeats the analysis for selected parameter
    configurations.
9.  The optimizer compares results against the user's objective and
    constraints.
10. The results module displays and exports the findings.

## Planned Technical Approach

-   **Language:** C (C17 is the initial target; C11 is also acceptable
    if required by the development environment).
-   **Input:** A documented, text-based netlist format inspired by SPICE
    conventions.
-   **Core representation:** Component and node structures, stored in
    dynamically managed collections.
-   **Circuit analysis:** Modified Nodal Analysis (MNA) for supported DC
    circuits.
-   **Numerical solution:** Implement and test a suitable linear-system
    solver, such as Gaussian elimination with partial pivoting, before
    considering an external library.
-   **Repeated simulation:** Parameter sweeps and/or randomized
    sampling.
-   **Optimization:** Begin with a straightforward baseline such as
    random search or grid search; add more advanced algorithms only
    after the simulation results are trustworthy.
-   **Output:** Human-readable terminal reports and CSV export.
-   **Testing:** Small circuits with independently calculated expected
    results, plus parser, validator, solver, and optimizer tests.

## Important Design Principles

-   Keep parsing, circuit representation, validation, simulation,
    optimization, and reporting as separate modules.
-   Keep the circuit topology separate from component parameter values
    so that optimization can change values without rebuilding
    connectivity.
-   Validate input before attempting simulation.
-   Report unsupported circuit features clearly rather than silently
    producing misleading results.
-   Treat numerical failures (for example, singular matrices) as errors
    that need explanation.
-   Verify the simulator against hand-calculated examples before
    trusting optimization recommendations.
-   Record the simulation settings and objective alongside reported
    results so that results are reproducible.

## Non-Goals for the First Version

The first version is not intended to be a full commercial EDA suite. It
will not initially provide:

-   A graphical schematic editor
-   PCB layout or routing
-   Manufacturing-file generation
-   Full SPICE compatibility
-   Full semiconductor device models
-   AC frequency-response or transient analysis

These may be considered after the core DC workflow is stable.

## Definition of a Successful First Release

The first release should be able to:

-   Load a documented netlist file.
-   Build and validate an internal circuit representation.
-   Simulate supported DC resistor/source circuits.
-   Produce correct node voltages and branch currents for test circuits.
-   Run multiple simulations using bounded parameter variations.
-   Identify a better parameter configuration for a clearly defined
    objective.
-   Explain the result and export simulation data to CSV.
-   Build and run using documented instructions.

## Project Status

**Current stage:** Initial netlist parsing and data-model implementation.

Build with `make` and run `./init [netlist-file]`. If no file is supplied,
the program reads `input_sample.txt`.
