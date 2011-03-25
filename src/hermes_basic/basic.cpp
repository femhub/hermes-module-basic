#include "basic.h"

// FIXME: Because of callbacks that depend on global variables
// we need to define a few global arrays below, but this is 
// a temporary solution. Like this, one cannot have two instances 
// of the basic module at the same time -- their global variables 
// would interfere with each other.

// Weak forms.
#include "definitions.cpp"

// Look up an integer number in an array.
bool find_index(const std::vector<int> &array, int x, int &i_out)
{
  for (unsigned int i=0; i < array.size(); i++)
    if (array[i] == x) {
      i_out = i;
      return true;
    }
  return false;
}

// Constructor.
ModuleBasic::ModuleBasic()
{
  this->init_ref_num = -1;
  this->init_p = -1;
  this->mesh = new Mesh();
  this->space = NULL;
  this->sln = new Solution();
  this->wf = new CustomWeakFormLinearConst();
  this->essential_bc_constant = NULL;
  this->essential_bcs = NULL;
  this->assembly_time = 0;
  this->solver_time = 0;
}

// Destructor.
ModuleBasic::~ModuleBasic()
{
  delete this->mesh;
  delete this->space;
}

// Set mesh via a string. 
// See basic.h for an example of such a string.
// TODO: Add more tests for correctness of the mesh.
bool ModuleBasic::set_mesh_str(const std::string &mesh_str)
{
  this->mesh_str = mesh_str;

  // Load the mesh.
  H2DReader mloader;
  mloader.load_str(this->get_mesh_string(), this->mesh);

  // Check whether the number of base elements is greater than zero.
  if (this->get_num_base_elements() <= 0) {
    info("No elements found in mesh");
    return false;
  };

  // Clear the mesh string.
  this->clear_mesh_string();

  // Debug.
  /*
  MeshView m("", 0, 0, 400, 400);
  m.show(this->mesh);
  View::wait();
  */
  
  return true;
}

// Set the number of initial uniform mesh refinements.
void ModuleBasic::set_initial_mesh_refinement(int init_ref_num) 
{
  this->init_ref_num = init_ref_num;
}

// Set initial poly degree in elements.
void ModuleBasic::set_initial_poly_degree(int p) 
{
  this->init_p = p;
}

// Set material markers, and check compatibility with mesh file.
void ModuleBasic::set_material_markers(const std::vector<std_string> &m_markers)
{  
  this->wf->set_material_markers(m_markers);
}

// Set c1 array.
void ModuleBasic::set_c1_array(const std::vector<double> &c1_array)
{
  int n = c1_array.size();
  for (int i = 0; i < n; i++) {
    if (c1_array[i] <= 1e-10) error("The c1 array needs to be positive.");
  }
  this->wf->set_c1_array(c1_array);
}

// Set c2 array.
void ModuleBasic::set_c2_array(const std::vector<double> &c2_array)
{
  this->wf->set_c2_array(c2_array);
}

// Set c3 array.
void ModuleBasic::set_c3_array(const std::vector<double> &c3_array)
{
  this->wf->set_c3_array(c3_array);
}

// Set c4 array.
void ModuleBasic::set_c4_array(const std::vector<double> &c4_array)
{
  this->wf->set_c4_array(c4_array);
}

// Set c5 array.
void ModuleBasic::set_c5_array(const std::vector<double> &c5_array)
{
  this->wf->set_c5_array(c5_array);
}

// Set Dirichlet boundary markers.
void ModuleBasic::set_dirichlet_markers(const std::vector<std_string> &bdy_markers_dirichlet)
{
  //this->bdy_markers_dirichlet = bdy_markers_dirichlet;
  Hermes::vector<int> t;
  for (unsigned int i = 0; i < bdy_markers_dirichlet.size(); i++) {
    t.push_back(bdy_markers_dirichlet[i]);
  }
  this->bc_types.add_bc_dirichlet(t);
}

// Set Dirichlet boundary values.
void ModuleBasic::set_dirichlet_values(const std::vector<int> &bdy_markers_dirichlet,
                                       const std::vector<double> &bdy_values_dirichlet)
{
  Hermes::vector<int> tm;
  for (unsigned int i = 0; i < bdy_markers_dirichlet.size(); i++) {
    tm.push_back(bdy_markers_dirichlet[i]);
  }
  Hermes::vector<double> tv;
  for (unsigned int i = 0; i < bdy_values_dirichlet.size(); i++) {
    tv.push_back(bdy_values_dirichlet[i]);
  }
  if (tm.size() != tv.size()) error("Mismatched numbers of Dirichlet boundary markers and values.");
  for (unsigned int i = 0; i < tm.size(); i++) this->bc_values.add_const(tm[i], tv[i]);
}

// Set Neumann boundary markers.
void ModuleBasic::set_neumann_markers(const std::vector<std_string> &bdy_markers_neumann)
{
  this->wf->set_neumann_markers(bdy_markers_neumann);
}

// Set Neumann boundary values.
void ModuleBasic::set_neumann_values(const std::vector<double> &bdy_values_neumann)
{
  this->wf->set_neumann_values(bdy_values_neumann);
}

// Set Newton boundary markers.
void ModuleBasic::set_newton_markers(const std::vector<std_string> &bdy_markers_newton)
{
  this->wf->set_newton_markers(bdy_markers_newton);
}

// Set Newton boundary values.
void ModuleBasic::set_newton_values(const std::vector<double_pair> &bdy_values_newton)
{
  this->wf->set_newton_values(bdy_values_newton);
}

// Sanity check of material markers and material constants.
void ModuleBasic::materials_sanity_check()
{
  if (this->mat_markers.size() != this->c1_array.size()) error("Wrong length of c1 array.");
  if (this->mat_markers.size() != this->c2_array.size()) error("Wrong length of c2 array.");
  if (this->mat_markers.size() != this->c3_array.size()) error("Wrong length of c3 array.");
  if (this->mat_markers.size() != this->c4_array.size()) error("Wrong length of c4 array.");
  if (this->mat_markers.size() != this->c5_array.size()) error("Wrong length of c5 array.");
}

// Get mesh string.
const char* ModuleBasic::get_mesh_string() 
{
  return this->mesh_str.c_str();
}

// Clear mesh string.
void ModuleBasic::clear_mesh_string() 
{
  this->mesh_str.clear();
}

void ModuleBasic::get_solution(Solution* s)
{
  s->copy(this->sln);
}

// Set mesh.
void ModuleBasic::set_mesh(Mesh* m) 
{
  this->mesh = m;
}

// Get mesh.
Mesh* ModuleBasic::get_mesh() 
{
  return this->mesh;
}

// Get space.
void ModuleBasic::get_space(H1Space* s) 
{
  s->dup(this->space->get_mesh());
}

// Get number of elements in base mesh.
int ModuleBasic::get_num_base_elements() 
{
  return this->mesh->get_num_base_elements();
}


// Set matrix solver.
void ModuleBasic::set_matrix_solver(std::string solver_name)
{
  bool found = false;
  if (solver_name == "amesos" ) {this->matrix_solver = SOLVER_AMESOS;  found = true;}
  if (solver_name == "aztecoo") {this->matrix_solver = SOLVER_AZTECOO; found = true;}
  if (solver_name == "mumps"  ) {this->matrix_solver = SOLVER_MUMPS;   found = true;}
  if (solver_name == "petsc"  ) {this->matrix_solver = SOLVER_PETSC;   found = true;}
  if (solver_name == "superlu") {this->matrix_solver = SOLVER_SUPERLU; found = true;}
  if (solver_name == "umfpack") {this->matrix_solver = SOLVER_UMFPACK; found = true;}
  if (!found) {
    warn("Possible matrix solvers: amesos, aztecoo, mumps, petsc, superlu, umfpack.");
    error("Unknown matrix solver %s.", solver_name.c_str());
  }
}

MatrixSolverType ModuleBasic::get_matrix_solver()
{
  return this->matrix_solver;
}

double ModuleBasic::get_assembly_time()
{
  return this->assembly_time;
}

double ModuleBasic::get_solver_time()
{
  return this->solver_time;
}

bool ModuleBasic::create_space_and_forms() 
{
  /* SANITY CHECKS */

  // Sanity check of material markers and material constants.
  this->wf->materials_sanity_check();

  // Check whether the number of base elements is greater than zero.
  if (this->get_num_base_elements() <= 0) {
    info("No elements found in mesh");
    return false;
  };

  /* BEGIN THE COMPUTATION */

  // Perform initial uniform mesh refinements.
  for (int i = 0; i < this->init_ref_num; i++) this->mesh->refine_all_elements();

  // Initialize boundary conditions.
  Hermes::vector<DefaultEssentialBCConst*> bc_essential_array = ();
  for (int i=0; i<bdy_markers_dirichlet.size(); i++) {
    bc_essential_array.push_back(new DefaultEssentialBCConst(bdy_markers_dirichlet[i], bdy_values_dirichlet[i]));
  }
  EssentialBCs* bcs = new EssentialBCs(bc_essential_array);

  // Create an H1 space with default shapeset.
  this->space = new H1Space(this->mesh, this->bcs, this->init_p);
  int ndof = Space::get_num_dofs(this->space);
  info("ndof = %d", ndof);
  if (ndof <= 0) return false;

  // Debug.
  /*
  BaseView b("", new WinGeom(0, 0, 400, 400));
  b.show(this->space);
  View::wait();
  */

  // Initialize the weak formulation.
  this->wf->create();

  return true;
}

// Solve the problem.
bool ModuleBasic::calculate() 
{
  // Begin assembly time measurement.
  TimePeriod cpu_time_assembly;
  cpu_time_assembly.tick();

  // Perform basic sanity checks, create mesh, perform 
  // uniform refinements, create space, register weak forms.
  bool mesh_ok = this->create_space_and_forms();
  if (!mesh_ok) return false;

  // Initialize the FE problem.
  bool is_linear = true;
  DiscreteProblem dp(this->wf, this->space, is_linear);

  // Set up the solver, matrix, and rhs according to the solver selection.
  SparseMatrix* matrix = create_matrix(matrix_solver);
  Vector* rhs = create_vector(matrix_solver);
  Solver* solver = create_linear_solver(matrix_solver, matrix, rhs);

  // Assemble the stiffness matrix and right-hand side vector.
  info("Assembling the stiffness matrix and right-hand side vector.");
  dp.assemble(matrix, rhs);

  // End assembly time measurement.
  cpu_time_assembly.tick();
  this->assembly_time = cpu_time_assembly.accumulated();

  // Begin solver time measurement.
  TimePeriod cpu_time_solver;
  cpu_time_solver.tick();

  // Solve the linear system and if successful, obtain the solution.
  info("Solving the matrix problem.");
  if(solver->solve()) Solution::vector_to_solution(solver->get_solution(), this->space, this->sln);
  else error ("Matrix solver failed.\n");

  // End solver time measurement.
  cpu_time_solver.tick();
  this->solver_time = cpu_time_solver.accumulated();

  // Clean up.
  delete solver;
  delete matrix;
  delete rhs;

  return true;
}

