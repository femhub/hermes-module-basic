#include "module_basic.cpp"

int main(int argc, char* argv[]) {
  ModuleBasic Basic;

  Mesh mesh;
  H2DReader mloader;
  mloader.load("domain.mesh", &mesh);

  Basic.add_mesh(&mesh);

  Basic.add_boundary(new BoundaryDataH1(Hermes::vector<std::string>("Bottom", "Right", "Upper", "Left"), HERMES_DIRICHLET, 0));
  //Basic.add_boundary(new BoundaryDataH1(Hermes::vector<std::string>("Right", "Left"), HERMES_NEUMANN, 5));
  Basic.add_material(new BasicMaterialData("Area", 1, 4, 7, 2, 10));

  //MeshView mview("Mesh", new WinGeom(0, 0, 350, 350));
  //mview.show(Basic.get_mesh(0));
  //mview::wait();

  Basic.solve();

  //BaseView bview("Space", new WinGeom(0, 0, 350, 350));
  //bview.show(Basic.get_space(0));
  //bview.wait();

  //ScalarView view("Solution", new WinGeom(0, 0, 440, 350));
  //view.show(Basic.meshes.at(0), HERMES_EPS_HIGH);

  return 0;
}
