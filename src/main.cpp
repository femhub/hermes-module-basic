#include "module_basic.cpp"

int main(int argc, char* argv[]) {
  ModuleBasic Basic;
  Basic.set_meshes();

  BoundaryDataH1 dirichlet("Bottom", HERMES_DIRICHLET, 1);
  Basic.set_boundary(&dirichlet);
  BoundaryDataH1 neumann(Hermes::vector<std::string>("Right, Upper, Left"), HERMES_NEUMANN, 5);
  Basic.set_boundary(&neumann);

  BasicMaterialData material("Arrea", 1, 4, 7, 2, 45);
  Basic.set_material(&material);

  MeshView mview("Mesh", new WinGeom(0, 0, 350, 350));
  mview.show(Basic.meshes.at(0));

  //Basic.solve();

  //ScalarView view("Solution", new WinGeom(0, 0, 440, 350));
  //view.show(Basic.meshes.at(0), HERMES_EPS_HIGH);
  //View::wait();
}
