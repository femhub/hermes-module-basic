Hermes Module Basic
===================

This is a simple generic module for a linear second-order PDE based on the Hermes 
library. The purpose of modules like this is to provide a higher-level API where 
the user is not exposed to wesk forms, spaces, solver initialization, and other 
technical details. 

Equation
--------

 -div(c1 \nabla u) + (c2, c3) \cdot \nabla u + c4 u = c5 

Here:

 * c1 ... equation parameter, element-wise constant
 * c2 ... equation parameter, element-wise constant
 * c3 ... equation parameter, element-wise constant
 * c4 ... equation parameter, element-wise constant
 * c5 ... equation parameter, element-wise constant

Boundary Conditions
-------------------

Dirichlet with piecewise-constant values (u = const)
Neumann with piecewise-constant normal derivatives (du/dn = const)
Newton with piecewise-constant parameters (const_1 u + du/dn = const_2)
