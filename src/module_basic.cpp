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
  ModuleBasic() {
    this->properties()->geometry = HERMES_PLANAR;
    this->properties()->analysis = HERMES_STEADY_STATE;
    this->properties()->mesh()->init_deg = 1;
    this->properties()->solver()->mat_solver = SOLVER_UMFPACK;
    this->properties()->solution()->num_sol = 1;
    this->properties()->adaptivity()->cand_list = H2D_NONE;
    this->properties()->adaptivity()->conv_exp = 1.0;
    this->properties()->adaptivity()->max_dofs = 6e4;
    this->properties()->adaptivity()->max_steps = 100;
    this->properties()->adaptivity()->regularize = -1;
    this->properties()->adaptivity()->strategy = 0;
    this->properties()->adaptivity()->threshold = 0.2;
    this->properties()->adaptivity()->tolerance = 1.0;
  }

  virtual void add_boundary(BoundaryData *boundary) {
    BCTypeH1 bc_type_h1 = static_cast<BoundaryDataH1*>(boundary)->bc_type_h1;
    if(bc_type_h1 == HERMES_DIRICHLET)
      this->essential_boundaries.push_back(boundary);
    else
      this->natural_boundaries.push_back(boundary);
  }

  virtual void set_weakforms() {
    WeakFormModuleBasic *wf;

    /* Essential BCs */
    for (unsigned int i = 0; i < this->essential_boundaries.size(); i++)
    {
      BoundaryDataH1 *boundary = dynamic_cast<BoundaryDataH1*>(this->essential_boundaries.at(i));

      DefaultEssentialBCConst bc(boundary->markers, boundary->value1);
      this->bcs.add_boundary_condition(&bc);
    }

    /* Natural BCs */
    for (unsigned int i = 0; i < this->natural_boundaries.size(); i++)
    {
      BoundaryDataH1 *boundary = dynamic_cast<BoundaryDataH1*>(this->essential_boundaries.at(i));

      if (boundary->bc_type_h1 == HERMES_NEUMANN)
        wf->add_vector_form_surf(new DefaultVectorFormSurf(0, boundary->markers, boundary->value1));
      else
      {
        wf->add_matrix_form_surf(new DefaultMatrixFormSurf(0, 0, boundary->markers, boundary->value1));
        wf->add_vector_form_surf(new DefaultVectorFormSurf(0, boundary->markers, boundary->value2));
      }
    }


    /* Materials */
    for (unsigned int i = 0; i < this->materials.size(); i++)
    {
      BasicMaterialData *material = dynamic_cast<BasicMaterialData *>(materials.at(i));

      Hermes::vector<std::string> markers = material->markers;
      wf->add_matrix_form(new DefaultLinearDiffusion(0, 0, markers, material->c1));
      wf->add_matrix_form(new DefaultLinearMass(0, 0, markers, material->c2));
      wf->add_matrix_form(new DefaultLinearAdvection(0, 0, markers, material->c2, material->c3));
      wf->add_vector_form(new DefaultVectorFormConst(0, markers, material->c5));
    }

    this->add_weakform(wf);
  }

  virtual void set_spaces() {
    H1Space space(this->meshes.at(0), &this->bcs, properties()->mesh()->init_deg);
    this->add_space(&space);
  }
};
