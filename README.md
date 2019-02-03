## Abstract 
The problems aims to build a ac circuit simulator, given a textual information about the circuit the application should output the optimized circuit. As the AC circuit consist of many active and passive elements, there are many unknown parameters. The application should be able to find the unknown parameter of the circuit using the knowledge of phasor diagram and Kirchoff's laws.
We first parsed the file containing the textual information about the circuit, then based on the information draw a svg image. After we used the nodal analysis, phasor diagram to find the unknown parameters.

# AC_Circuit_Solver
Given a netlist *display*(draw) an AC circuit and find current and voltage values at *t=0* and 

**Instruction to run flex on terminal:**
1. sudo apt-get install flex,to install lex
1. filename.l
1. cc lex.yy.c -ll
1. ./a.out<in.txt

**Instruction to run  Bison**
1. sudo apt-get install Bison
1. yacc -d filename.y
1. cc lex.yy.c y.tab.c -o outputfilename
1. ./outputfilename

**Stages of Project**
1. Used lex,yacc for scanning and parsing respectively.
1. Draw a compact circuit from given netlist.
1. Solve circuit from ac circuit theory and find intial and final value of current and voltage.
1. Make circuit clickable.
