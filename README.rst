Hermes Module Basic
===================

This is a simple generic module for a linear second-order PDE based on the Hermes 
library. The purpose of modules like this is to provide a higher-level API where 
the user is not exposed to weak forms, spaces, solver initialization, and other 
technical details. The module Basic is an open source module whose purpose is to 
illustrate how modules should be created.

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

Build the Module
----------------

In the root directory of this module, type::

    cmake .
    make

Run the Module on C++ Level
---------------------------

C++ sources are located in the directory src/. Change dir to the directory 
src/ and run the module using::

    ./module-basic model.cfg

The file model.cfg is a text file that emulates input from a GUI. You can 
change the parameters there at your will.


Run the Module on Python Level
------------------------------

Python wrappers are located in the directory python/ and they allow you 
to call the module from Python as follows::

    python module-basic.py

The file module-basic.py contains a set of parameters analogous to those
which on C++ level are in the file model.cfg. The user can change these
parameters arbitrarily. 

Run the Module in the Online Lab SDK
------------------------------------

To be completed.
