## Abstract 
The problems aims to build a ac circuit simulator, given a textual information about the circuit the application should output the optimized circuit. As the AC circuit consist of many active and passive elements, there are many unknown parameters. The application should be able to find the unknown parameter of the circuit using the knowledge of phasor diagram and Kirchoff's laws.

## Approach
We first parsed the file containing the textual information about the circuit, then based on the information draw a svg image. After we used the nodal analysis, phasor diagram to find the unknown parameters.

## AC_Circuit_Solver
Given a netlist *display*(draw) an AC circuit and find current and voltage values at *t=0* and 

