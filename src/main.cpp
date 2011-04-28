#define HERMES_REPORT_ALL
#include "module_basic.cpp"

int main(int argc, char* argv[]) {

  Mesh mesh;
  H2DReader mloader;
  mloader.load("domain.mesh", &mesh);

  // Create the module.
  ModuleBasic Basic;

  // Pass mesh pointer.
  Basic.add_mesh(&mesh);

  // Define Dirichlet boundaries.
  Basic.add_boundary(new BoundaryDataH1(Hermes::vector<std::string>("Bottom", "Upper"), 
                                        HERMES_DIRICHLET, 0.0));

  // Define Neumann boundaries.
  Basic.add_boundary(new BoundaryDataH1(Hermes::vector<std::string>("Right", "Left"), HERMES_NEUMANN, 5.0));

  // Define materials.
  Basic.add_material(new BasicMaterialData("Area", 1.0, 4.0, 7.0, 2.0, 10.0));

  MeshView mview("Mesh", new WinGeom(0, 0, 350, 350));
  mview.show(Basic.get_mesh(0));
  mview.wait();

  std::string message_out;
  bool success = Basic.solve(message_out);
  printf("%s", message_out);
  if (success == false) {
    error("Computation failed.");
  }

  BaseView bview("Space", new WinGeom(0, 0, 350, 350));
  bview.show(Basic.get_space(0));
  bview.wait();

  ScalarView view("Solution", new WinGeom(0, 0, 440, 350));
  view.show(Basic.get_solution(0), HERMES_EPS_HIGH);
  view.wait();

  return 0;
}
