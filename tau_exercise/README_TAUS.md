# Tau README

Next steps:
1. Re-train BDT in ROOTTMVA with five highest-significance variables.
1. Decide on appropriate bins for these variables.
1. Generate an array in C++ with all possible combinations of the bins, evaluate the BDT for each variables' values, and save the discriminant (converted from float  to integer) into another column in the array.
1. Decide on a series of bitwise operations that can transform the variable values/ranges into x bits (decide what x is) that can be interpreted as a memory address.

## Plotting tau variables
Use `tau_exercise/plot_tau_features/runValidationPlots.C`.

## Cut-based approach efficiencies
This has all been moved to [https://github.com/skkwan/phase2L1validation](the phase2L1validation GitHub), which should be run from a SL7 machine. My current setup is on UW Wisc servers.