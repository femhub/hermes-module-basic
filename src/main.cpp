#include "module_basic.cpp"

int main(int argc, char* argv[]) {
  ModuleBasic Basic;

  Mesh mesh;
  H2DReader mloader;
  mloader.load("domain.mesh", &mesh);

  Basic.add_mesh(&mesh);

  BoundaryDataH1 dirichlet(Hermes::vector<std::string>("Bottom", "Upper"), HERMES_DIRICHLET, 1);
  Basic.add_boundary(&dirichlet);
  BoundaryDataH1 neumann(Hermes::vector<std::string>("Right", "Left"), HERMES_NEUMANN, 5);
  Basic.add_boundary(&neumann);

  BasicMaterialData material("Arrea", 1, 4, 7, 2, 10);
  Basic.add_material(&material);

  MeshView mview("Mesh", new WinGeom(0, 0, 350, 350));
  mview.show(Basic.get_mesh(0));
  View::wait();

  Basic.solve();

  //ScalarView view("Solution", new WinGeom(0, 0, 440, 350));
  //view.show(Basic.meshes.at(0), HERMES_EPS_HIGH);

  return 0;
}
