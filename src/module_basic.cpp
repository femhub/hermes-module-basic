#include "hermes2d.h"

#include "/home/fmach/hpfem.org/hermes/hermes2d/src/hermes_module.cpp"
#include "/home/fmach/hpfem.org/hermes/hermes2d/src/weakform_library/h1.h"

using namespace WeakFormsH1::VolumetricMatrixForms;
using namespace WeakFormsH1::VolumetricVectorForms;
using namespace WeakFormsH1::SurfaceMatrixForms;
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

/* ModuleBasic */

class ModuleBasic : public HermesModule {
public:
  virtual void set_boundary(BoundaryData *boundary) {
    BCTypeH1 bc_type_h1 = static_cast<BoundaryDataH1*>(boundary)->bc_type_h1;
    if(bc_type_h1 == HERMES_DIRICHLET)
      this->essential_boundaries.push_back(boundary);
    else
      this->natural_boundaries.push_back(boundary);
  }

  virtual void set_spaces() {
    for (int i = 0; i < this->properties()->solution()->num_sol; i++)
      this->spaces.push_back(new H1Space(this->meshes.at(i), &this->bcs, properties()->mesh()->init_deg));
  }

  virtual void set_boundary_conditions() {
    for (unsigned int i = 0; i < this->essential_boundaries.size(); i++)
    {
      BoundaryDataH1 *boundary = dynamic_cast<BoundaryDataH1*>(this->essential_boundaries.at(i));

      DefaultEssentialBCConst bc(boundary->markers, boundary->value1);
      this->bcs.add_boundary_condition(&bc);
    }
    for (unsigned int i = 0; i < this->natural_boundaries.size(); i++)
    {
      BoundaryDataH1 *boundary = dynamic_cast<BoundaryDataH1*>(this->essential_boundaries.at(i));

      /*
      FIXME
      if (boundary->bc_type_h1 == HERMES_NEUMANN)
        WeakFormModuleBasic::add_vector_form_surf(new DefaultVectorFormSurf(0, boundary->markers,
                                                                            boundary->value1));
      else
      {
        WeakFormModuleBasic::add_matrix_form_surf(new DefaultMatrixFormSurf(0, 0, boundary->markers,
                                                                            boundary->value1));
        WeakFormModuleBasic::add_vector_form_surf(new DefaultVectorFormSurf(0, boundary->markers,
                                                                            boundary->value2));
      }
      */
    }
  }

  virtual void set_weakforms() {
    for (unsigned int i = 0; i < this->materials.size(); i++)
    {
      BasicMaterialData *material = dynamic_cast<BasicMaterialData *>(materials.at(i));

      /*
      FIXME
      Hermes::vector<std::string> markers = material->markers;
      WeakFormModuleBasic::add_matrix_form(new DefaultLinearDiffusion(0, 0, markers, material->c1));
      WeakFormModuleBasic::add_matrix_form(new DefaultLinearMass(0, 0, markers, material->c2));
      WeakFormModuleBasic::add_matrix_form(new DefaultLinearAdvection(0, 0, markers, material->c2, material->c3));
      WeakFormModuleBasic::add_vector_form(new DefaultVectorFormConst(0, markers, material->c5));
      */
    }
  }
};
