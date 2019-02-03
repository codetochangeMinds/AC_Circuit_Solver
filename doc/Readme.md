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

