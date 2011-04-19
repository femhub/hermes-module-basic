#include "../../hermes/hermes2d/src/hermes_module.h"
#include "../../hermes/hermes2d/src/hermes2d.h"
#include "../../hermes/hermes2d/src/weakform_library/h1.h"

using namespace WeakFormsH1::VolumetricMatrixForms;
using namespace WeakFormsH1::VolumetricVectorForms;
using namespace WeakFormsH1::SurfaceVectorForms;

/* WeakFormModuleBasic */

class WeakFormModuleBasic : public WeakForm {
public:
  WeakFormModuleBasic() : WeakForm(1) { }
};

/* BasicMaterialData */

class BasicMaterialData : public MaterialData {
public:
  BasicMaterialData(std::string marker, double c1, double c2, double c3, double c4, double c5) :
    MaterialData(marker), c1(c1), c2(c2), c3(c3), c4(c4), c5(c5) { }

  BasicMaterialData(Hermes::vector<std::string> markers, double c1, double c2, double c3, double c4, double c5) :
    MaterialData(markers), c1(c1), c2(c2), c3(c3), c4(c4), c5(c5) { }

  double c1, c2, c3, c4, c5;
};

/* BasicBoundaryData */

class BasicBoundaryData : public BoundaryDataH1 {
public:
  BasicBoundaryData(std::string marker, BoundaryConditionType type, scalar value, BoundaryConditionTypeH1 h1_type = DIRICHLET, scalar value2 = 0.0) :
    BoundaryDataH1(marker, type, value, h1_type), value2(value2) { }

  BasicBoundaryData(Hermes::vector<std::string> markers, BoundaryConditionType type, scalar value, BoundaryConditionTypeH1 h1_type = DIRICHLET, scalar value2 = 0.0) :
    BoundaryDataH1(markers, type, value, h1_type), value2(value2) { }

  double value2;
};

/* ModuleBasic */

class ModuleBasic : public HermesModule {
public:
  virtual void set_mesh(const std::string &mesh_string) {
  }

  virtual void set_boundary(BasicBoundaryData *boundary) {
    if(boundary->type == ESSENTIAL)
      this->essential_boundaries.push_back(dynamic_cast<BasicBoundaryData *>(boundary));
    else if (boundary->type == NATURAL)
      this->natural_boundaries.push_back(dynamic_cast<BasicBoundaryData *>(boundary));
  }

  void set_material(BasicMaterialData *material) {
    this->materials.push_back(dynamic_cast<MaterialData *>(material));
  }

  virtual void set_weakform() {
    /* Boundary conditions */
    for (unsigned int i = 0; i < this->essential_boundaries.size(); i++)
    {
      BasicBoundaryData *boundary = dynamic_cast<BasicBoundaryData *>(this->essential_boundaries.at(i));

      if (boundary->type == ESSENTIAL)
        if (boundary->is_constant())
        {
          DefaultEssentialBCConst bc(boundary->markers, boundary->value);
          this->bcs.add_boundary_condition(&bc);
        }
      else
        if (boundary->h1_type == NEUMANN)
          WeakFormModuleBasic::add_vector_form_surf(new DefaultVectorFormSurf(0, boundary->markers,
                                                                              boundary->value));
        else if (boundary->h1_type == NEWTON)
        {
          WeakFormModuleBasic::add_matrix_form_surf(new DefaultMatrixFormSurf(0, 0, boundary->markers,
                                                                              boundary->value));
          WeakFormModuleBasic::add_vector_form_surf(new DefaultVectorFormSurf(0, boundary->markers,
                                                                              boundary->value2));
        }
    }

    /* Materials */
    for (int i = 0; i < this->materials.size(); i++)
    {
      BasicMaterialData *material = dynamic_cast<BasicMaterialData *>(materials.at(i));

      Hermes::vector<std::string> markers = material->markers;
      WeakFormModuleBasic::add_matrix_form(new DefaultLinearDiffusion(0, 0, markers, material->c1));
      WeakFormModuleBasic::add_matrix_form(new DefaultLinearMass(0, 0, markers, material->c2));
      WeakFormModuleBasic::add_matrix_form(new DefaultLinearAdvection(0, 0, markers, material->c2, material->c3));
      WeakFormModuleBasic::add_vector_form(new DefaultVectorFormConst(0, markers, material->c5));
    }
  }
};
