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

For this, install the Hermes library first. Change dir to your 
local Hermes repository, for example /home/pavel/repos/hermes/.
There, either type "sudo make install" to have the Hermes library 
installed system-wide (typically into /usr/local/), or add into your 
CMake.vars file a line::

   set(CMAKE_INSTALL_PREFIX    /home/pavel/build/hermes)

to have Hermes installed into /home/pavel/build/hermes (adjust
the directory name to your needs). If using the latter option,
you do not need to use sudo. More details about the installation 
process can be found in the INSTALLATION section of the file 
CMake.vars.example in the Hermes repository.

Next, change dir to the directory with your module (this directory),
and in the file CMake.vars set the location of the Hermes library
by adding the line::

    set(HERMES_ROOT /home/pavel/build/hermes)

Then type::

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

If you have uncommented the line 

#  set(PYTHON_INSTALL_PATH   USE_SYSTEM_PYTHON_DIRECTORY)

in the file CMake.vars in your local Hermes repository, 
then you may delete the lines
::

    import sys
    sys.path.append("/home/pavel/build/hermes/lib/python")

in the file module-basic.py. Otherwise, adjust the line to point to the 
location of Hermes Python wrappers which must be compatible with your 
definition of HERMES_ROOT above.

Then run the module by typing::

    python module-basic.py

The file module-basic.py contains a set of parameters analogous to those
which on C++ level are in the file model.cfg. These parameters can be 
changed in the same way as on the C++ level.

Run the Module in the Online Lab SDK
------------------------------------

To be completed.
