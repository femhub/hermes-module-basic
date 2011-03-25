#!/usr/bin/env python

# If you have installed the Hermes wrappers and the module wrapper into
# directories which are not on default Python path, you must point to them here:
import sys
sys.path.append("/home/pavel/build/hermes/lib/python")

from hermes2d.hermes2d import Linearizer
from hermes2d.plot import sln2png

# If you ran 'make install' and appended 'sys.path' appropriately (see above),
# you may import the module from anywhere by the following line:
#from hermes_modules.basic import ModuleBasic

# In current module's directory, you may import the wrapper also locally:
from basic import ModuleBasic

mesh = """
a = 1.0         # size of the mesh
b = sqrt(2)/2

vertices =
{
  { 0, -a },    # vertex 0
  { a, -a },    # vertex 1
  { -a, 0 },    # vertex 2
  { 0, 0 },     # vertex 3
  { a, 0 },     # vertex 4
  { -a, a },    # vertex 5
  { 0, a },     # vertex 6
  { a*b, a*b }  # vertex 7
}

elements =
{
  { 0, 1, 4, 3, 0 },  # quad 0
  { 3, 4, 7, 0 },     # tri 1
  { 3, 7, 6, 0 },     # tri 2
  { 2, 3, 6, 5, 0 }   # quad 3
}

boundaries =
{
  { 0, 1, 1 },
  { 1, 4, 2 },
  { 3, 0, 4 },
  { 4, 7, 2 },
  { 7, 6, 2 },
  { 2, 3, 4 },
  { 6, 5, 2 },
  { 5, 2, 3 }
}

curves =
{
  { 4, 7, 45 },  # +45 degree circular arcs
  { 7, 6, 45 }
}
"""

def main():
    e = ModuleBasic()
    e.set_mesh_str(mesh)
    e.set_initial_mesh_refinement(2)
    e.set_initial_poly_degree(4)
    solver_name = "umfpack"   # choose from amesos, aztecoo, mumps, petsc, superlu, umfpack
    e.set_matrix_solver(solver_name)
    print "Matrix solver:", solver_name
    e.set_material_markers(["0"])
    e.set_c1_array([1])
    e.set_c2_array([0])
    e.set_c3_array([0])
    e.set_c4_array([0])
    e.set_c5_array([1])
    e.set_dirichlet_markers(["4"])
    e.set_dirichlet_values([4], [0])
    e.set_neumann_markers(["1", "3"])
    e.set_neumann_values([0, 0])
    e.set_newton_markers(["2"])
    e.set_newton_values( [(1, 1)] )

    success = e.calculate()
    assert success is True
    sln = e.get_solution()
    print "Assembly time:", e.get_assembly_time()
    print "Solver time:", e.get_solver_time()
    print "Saving solution to 'solution.png'"
    sln2png(sln, "solution.png")
    raw_input()

if __name__ == "__main__":
    main()

