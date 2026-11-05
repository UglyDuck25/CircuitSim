# Project Roadmap: Simplified EDA Circuit Simulation and Optimization Tool

This roadmap describes the work required to build the project in
manageable stages. Complete and test each milestone before moving to the
next. The goal is to build a reliable core application first, then add
simulation, repeated analysis, and optimization.

## Development Rules

-   Work in small, testable increments.
-   Keep each module responsible for one job.
-   Compile frequently with warnings enabled.
-   Test valid and invalid inputs.
-   Use hand-calculated circuits as reference results.
-   Do not implement optimization until the simulator produces
    trustworthy results.
-   Document decisions and supported features as they are finalized.

Suggested compiler flags during development:

``` bash
gcc -Wall -Wextra -Wpedantic -Werror -g
```

You may temporarily remove `-Werror` while resolving existing warnings,
but aim to keep the project warning-free.

------------------------------------------------------------------------

## Milestone 1 --- Learn the Circuit and Netlist Fundamentals

### Learn

-   Voltage, current, resistance, and power.
-   Ohm's law.
-   Series and parallel connections.
-   Electrical terminals and nodes.
-   Ground as the reference node.
-   Kirchhoff's Current Law (KCL) and Kirchhoff's Voltage Law (KVL).
-   How a netlist describes components and node connections.

### Decide and document

-   Which component types the first version supports.
-   The exact line syntax for each component.
-   Component naming rules.
-   Node naming rules and the special ground node `0`.
-   Comment and end-of-file syntax.
-   How malformed lines and unsupported components are reported.
-   Whether values may use unit suffixes (for example, `k`, `u`, or
    `n`). For the first parser, plain numeric values are simpler.

### Deliverable

A short netlist specification with examples and rules. Do not start with
a graphical interface.

------------------------------------------------------------------------

## Milestone 2 --- Design the Internal C Data Model

### Learn

-   C structures and enumerations.
-   Pointers and pointer ownership.
-   Arrays and dynamic memory allocation.
-   `malloc`, `realloc`, and `free`.
-   Header files and separating declarations from implementations.

### Implement

Design structures for:

-   A component: name, type, terminal node references, value, and
    optimization metadata.
-   A node: name or identifier and ground/reference status.
-   A circuit: collections of components and nodes, plus counts and
    capacities.

For the initial version, resistors and independent sources can use a
common two-terminal component representation. Keep in mind that terminal
order matters for voltage and current sources.

Use dynamically allocated arrays with both a count and a capacity. Check
allocation failures and free all allocated memory when the circuit is
destroyed.

### Deliverable

A small C program that creates a circuit in memory, adds a few
components and nodes, prints them, and releases its memory cleanly.

------------------------------------------------------------------------

## Milestone 3 --- Implement the Netlist Parser

### Learn

-   File input with `fopen`, `fgets`, and `fclose`.
-   String handling and character classification.
-   Tokenization and numeric conversion with `strtod`.
-   Parsing errors and line-number reporting.
-   Safe handling of fixed-size buffers and long lines.

### Implement

1.  Open a netlist file and report a useful error if it cannot be
    opened.
2.  Read it line by line.
3.  Ignore blank lines and comment lines.
4.  Recognize the supported component line formats.
5.  Validate token counts and component identifiers.
6.  Convert numeric values and reject invalid or out-of-range numbers.
7.  Resolve node names to internal node records.
8.  Add components to the circuit model.
9.  Recognize `.END` and handle missing end markers according to the
    specification.

Avoid relying on `strtok` as the permanent parser design if it makes
error handling or future grammar extensions difficult. A small tokenizer
that tracks token boundaries and line numbers may be easier to extend.

### Test cases

-   A valid voltage-divider netlist.
-   A blank file.
-   A missing file.
-   A comment-only file.
-   A malformed component line.
-   A non-numeric component value.
-   A duplicate component name.
-   A line that references a node not otherwise connected (to be handled
    by validation rules).

### Deliverable

The program reads a netlist and prints the parsed components and nodes
without performing simulation.

------------------------------------------------------------------------

## Milestone 4 --- Build Circuit Validation

### Learn

-   Graph connectivity and connected components.
-   Circuit reference nodes and floating nodes.
-   Difference between syntax errors and electrical/model errors.
-   How to design useful error and warning messages.

### Implement

Check at least:

-   Exactly one designated reference node, represented by node `0`.
-   Unique component names.
-   Valid component types and terminal counts.
-   Valid numeric values and supported parameter ranges.
-   No invalid or unresolved node references.
-   No unsupported component types.
-   Whether the circuit has a usable connection to the reference node.
-   Whether the circuit is likely to contain floating or disconnected
    sections.

Do not assume every disconnected circuit is automatically invalid in
every possible analysis. Define what the first simulator supports and
report issues in that context.

### Deliverable

A validation module that returns clear errors and warnings and prevents
invalid models from reaching the solver.

------------------------------------------------------------------------

## Milestone 5 --- Learn and Implement DC Circuit Analysis

### Learn

-   KCL and Ohm's law in equation form.
-   Nodal analysis.
-   Linear systems of equations: `A x = b`.
-   Matrix indexing and memory layout in C.
-   Gaussian elimination and partial pivoting.
-   Singular and ill-conditioned systems.
-   Modified Nodal Analysis (MNA), especially how ideal voltage sources
    are handled.

### Implement

1.  Define the unknowns in the system (node voltages and the additional
    source currents required by MNA).
2.  Build the conductance contributions for resistors.
3.  Add the MNA contributions for independent voltage sources.
4.  Add current-source contributions to the right-hand-side vector using
    a clearly documented current direction convention.
5.  Solve the matrix system.
6.  Map the solution back to node voltages and source currents.
7.  Calculate resistor branch currents and power.
8.  Detect singular systems and report a useful diagnostic.

Start with resistors and independent DC sources. Be explicit about
current direction and sign conventions.

### Deliverable

A DC solver that correctly solves small, supported circuits.

------------------------------------------------------------------------

## Milestone 6 --- Verify the Simulator Thoroughly

### Build reference circuits

1.  A single resistor across a voltage source.
2.  A two-resistor voltage divider.
3.  Two resistors in series.
4.  Two resistors in parallel.
5.  A current source with a resistive path to ground.
6.  A circuit with a floating node or singular matrix, to verify error
    handling.

For each valid circuit, calculate expected voltages and currents by hand
or with an independent trusted method. Compare the program output with
an appropriate numerical tolerance rather than requiring exact
floating-point equality.

### Add tests

-   Unit tests for matrix operations.
-   Unit tests for component stamping.
-   Solver tests.
-   End-to-end netlist-to-results tests.
-   Regression tests for previously fixed bugs.

### Deliverable

A documented set of tests showing that the simulator produces correct
results for its supported circuit types.

------------------------------------------------------------------------

## Milestone 7 --- Add Repeated Simulation

### Decide

-   Which component parameters may vary.
-   Whether each parameter is fixed or adjustable.
-   Minimum and maximum permitted values.
-   Whether variations are random, systematic, or based on component
    tolerances.
-   How many runs the user can request.
-   What statistics should be reported.

### Implement

1.  Add parameter constraints to the circuit configuration.
2.  Preserve the original component values.
3.  Generate a candidate parameter set for each run.
4.  Apply the candidate values without changing circuit connectivity.
5.  Run the DC solver.
6.  Record successful results and failed runs.
7.  Calculate summary statistics.

Start with a simple parameter sweep or bounded random sampling. Make the
random seed configurable so a run can be reproduced.

### Deliverable

A simulation mode that runs a user-selected number of trials and reports
the results.

------------------------------------------------------------------------

## Milestone 8 --- Define the Optimization Problem

Optimization needs a precise objective. A tool cannot know what "better"
means unless the user defines it.

### Decide

-   What output or circuit measurement is the target?
-   Is the goal to minimize error, power, cost, or a combination?
-   What constraints must always be satisfied?
-   Which component values can change?
-   What component ranges and preferred standard values are allowed?
-   How should invalid or unsolvable candidates be scored?

For a first objective, use target-output matching. For example,
minimize:

`error = abs(simulated_output - target_output)`

A relative or squared error may be more appropriate for some objectives,
but document the choice.

### Implement

-   An objective-function module.
-   Constraint checks.
-   A consistent way to score each valid simulation.
-   A clear policy for failed simulations.

### Deliverable

A documented optimization objective and a testable scoring function.

------------------------------------------------------------------------

## Milestone 9 --- Implement the First Optimizer

### Recommended starting point

Implement random search or grid search before a genetic algorithm. A
simple optimizer is easier to test and provides a baseline for judging
more advanced methods.

### Implement

1.  Generate candidate component values within allowed bounds.
2.  Run the simulator for each candidate.
3.  Reject candidates that violate constraints or cannot be simulated.
4.  Score valid candidates.
5.  Keep the best candidate found.
6.  Compare the best candidate with the original circuit.
7.  Stop after the configured number of evaluations or another
    documented stopping condition.

Later, consider hill climbing, simulated annealing, or a genetic
algorithm.

### Deliverable

The program recommends a candidate set of component values and explains
how it compares with the original circuit.

------------------------------------------------------------------------

## Milestone 10 --- Results, Export, and Usability

### Implement

-   A readable summary of the original circuit.
-   Original simulation results.
-   Number of attempted, successful, and failed runs.
-   Best parameter values found.
-   Objective value before and after optimization.
-   Improvement expressed with a clearly defined metric.
-   CSV export of individual simulation runs.
-   Helpful error messages and command-line usage instructions.

Do not claim that a result is globally optimal unless the algorithm and
search method can guarantee that. For random or heuristic search, say
"best candidate found."

### Deliverable

A usable command-line application that produces repeatable,
understandable reports.

------------------------------------------------------------------------

## Milestone 11 --- Quality, Documentation, and Release

### Complete

-   Unit and integration tests.
-   Memory checks with tools such as Valgrind where available.
-   Compiler-warning cleanup.
-   Input and numerical edge-case testing.
-   README build and usage instructions.
-   Netlist format documentation.
-   Example circuit files.
-   Explanation of supported features and known limitations.
-   A small demonstration showing parsing, simulation, repeated runs,
    and optimization.

### Deliverable

A clean repository that another developer can build, run, test, and
understand.

------------------------------------------------------------------------

## Suggested Project Directory

``` text
eda-project/
├── README.md
├── ROADMAP.md
├── Makefile
├── include/
│   ├── circuit.h
│   ├── component.h
│   ├── parser.h
│   ├── validator.h
│   ├── matrix.h
│   ├── simulator.h
│   ├── optimizer.h
│   └── results.h
├── src/
│   ├── main.c
│   ├── circuit.c
│   ├── component.c
│   ├── parser.c
│   ├── validator.c
│   ├── matrix.c
│   ├── simulator.c
│   ├── optimizer.c
│   └── results.c
├── examples/
│   └── voltage_divider.cir
└── tests/
    ├── test_parser.c
    ├── test_validator.c
    ├── test_matrix.c
    └── test_simulator.c
```

You can adjust the layout as the implementation grows. Avoid creating
empty modules just to match the tree; add files when their
responsibilities are clear.

## Recommended First-Week Plan

**Session 1:** Review voltage, current, resistance, Ohm's law, nodes,
ground, and KCL.

**Session 2:** Study SPICE-style netlists and write three small examples
by hand.

**Session 3:** Finalize the first-version netlist grammar and document
valid and invalid examples.

**Session 4:** Design the C component, node, and circuit structures.

**Session 5:** Write a small program that creates, stores, prints, and
frees a circuit in memory.

**Session 6:** Add dynamic arrays and test adding different numbers of
components and nodes.

**Session 7:** Review the design, test memory handling, and prepare to
implement the parser.

## Completion Checklist

-   [ ] Netlist syntax documented.
-   [ ] Component and node data structures implemented.
-   [ ] Dynamic storage and cleanup implemented.
-   [ ] Netlist parser implemented.
-   [ ] Parser tests passing.
-   [ ] Circuit validation implemented.
-   [ ] DC MNA model implemented.
-   [ ] Linear solver implemented and tested.
-   [ ] Reference circuits produce expected results.
-   [ ] Repeated simulation implemented.
-   [ ] Optimization objective documented.
-   [ ] Baseline optimizer implemented.
-   [ ] Results report and CSV export implemented.
-   [ ] Build, usage, examples, and limitations documented.

## Immediate Next Step

Start with **Milestone 1: write the netlist specification**. Decide the
exact syntax and rules for resistor, voltage-source, and current-source
lines. Then create two or three example `.cir` files and check that a
person can understand the circuit from the file alone. Once the format
is stable, design the C structures and begin the parser.
