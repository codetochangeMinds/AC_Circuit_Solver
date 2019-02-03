## Abstract 
The problems aims to build a ac circuit simulator, given a textual information about the circuit the application should output the optimized circuit. As the AC circuit consist of many active and passive elements, there are many unknown parameters. The application should be able to find the unknown parameter of the circuit using the knowledge of phasor diagram and Kirchoff's laws.

## Approach
We first parsed the file containing the textual information about the circuit, then based on the information draw a svg image. After we used the nodal analysis, phasor diagram to find the unknown parameters.

## AC_Circuit_Solver
Given a netlist *display*(draw) an AC circuit and find current and voltage values at *t=0* and 

## Excecuting Program
1. Open Terminal
2. Cd Ac_Circuit_Solver/Code
3. make
4. ./ac_circuit_solver

>Svg image will be produced on executing following commands which can be viewed in any modern browser.

## Assumption
1. CURRENT,VOLTAGE START WITH V,I
2. RESISTANCE,CAPACITOR,INDUCTOR START WITH R/r,L/l,C/c.
3. K/k->kilo,N/n->nano,P/p->pico,M->mega,m->milli.
4. We had taken only these unit in consideration.
5. Assumed ground voltage to be zero while solving circuit.
6. We are assuming dc offset and damping factor to be zero.
7. Net0 is not allowed while 0 is always taken as ground.
8. Netx Net(int*) **int** can't take discrete values(i.e if we have net1,net3 then net2 must be there).  

