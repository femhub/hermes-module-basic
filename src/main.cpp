#define HERMES_REPORT_ALL
#include "module_basic.cpp"

int main(int argc, char* argv[]) {
  // Load and refine mesh
  Mesh mesh;
  H2DReader mloader;
  mloader.load("domain.mesh", &mesh);
  mesh.refine_all_elements();

  // Create the module.
  ModuleBasic Basic;

  // Pass mesh pointer.
  Basic.add_mesh(&mesh);

  // Define Dirichlet boundaries.
  Basic.add_boundary(new BoundaryDataH1("Bottom", HERMES_DIRICHLET, 0.0));
  Basic.add_boundary(new BoundaryDataH1("Left", HERMES_DIRICHLET, 1.0));

  // Define Neumann boundaries.
  Basic.add_boundary(new BoundaryDataH1(Hermes::vector<std::string>("Outer", "Inner"),
                                        HERMES_NEUMANN, 5.0));

  // Define materials.
  Basic.add_material(new BasicMaterialData("Material 1", 10.0, 4.0, 17.0, 9.0, -7.0));
  Basic.add_material(new BasicMaterialData("Material 2", 3.0, 6.0, 7.0, 2.0, 10.0));

  // Solve
  if (!Basic.solve())
    error("Computation failed.");

  // Views
  MeshView mview("Mesh", new WinGeom(0, 0, 450, 450));
  mview.show(Basic.get_mesh(0));

  BaseView bview("Space", new WinGeom(460, 0, 450, 450));
  bview.show(Basic.get_space(0));

  ScalarView view("Solution", new WinGeom(920, 0, 450, 450));
  view.show(Basic.get_solution(0), HERMES_EPS_HIGH);

  View::wait();

  return 0;
}
