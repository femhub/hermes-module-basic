#include "hermes2d.h"

#include "/home/fmach/hpfem.org/hermes/hermes2d/src/hermes_module.cpp"
#include "/home/fmach/hpfem.org/hermes/hermes2d/src/weakform_library/h1.h"

using namespace WeakFormsH1::VolumetricMatrixForms;
using namespace WeakFormsH1::VolumetricVectorForms;
using namespace WeakFormsH1::SurfaceMatrixForms;
using namespace WeakFormsH1::SurfaceVectorForms;

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
    this->properties()->solver()->newton_max_iter = 100;
    this->properties()->solver()->newton_tol = 1e-6;
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

  ~ModuleBasic() {
    delete wf;
  }

  virtual void set_weakforms() {
    this->wf = new WeakForm(this->properties()->solution()->num_sol);

    /* BCs */
    for (unsigned int i = 0; i < this->boundaries.size(); i++)
    {
      BoundaryDataH1 *boundary = dynamic_cast<BoundaryDataH1*>(this->boundaries.at(i));

      if (boundary->bc_type == HERMES_ESSENTIAL)
      {
        //printf("Dirichlet BC (%d/%d); %f\n", i+1, this->boundaries.size(), boundary->value1);
        this->bcs.add_boundary_condition(new DefaultEssentialBCConst(boundary->markers, boundary->value1));
      }
      else
      {
        // FIXME
        if (boundary->bc_type_h1 == HERMES_NEUMANN)
        {
          //printf("NEUMANN BC (%d/%d); %f\n", i+1, this->boundaries.size(), boundary->value1);
          this->wf->add_vector_form_surf(new DefaultVectorFormSurf(0, boundary->markers, boundary->value1));
        }

        if (boundary->bc_type_h1 == HERMES_NEWTON)
        {
          //printf("NEWTON BC (%d/%d); %f, %f\n", i+1, this->boundaries.size(), boundary->value1, boundary->value2);
          this->wf->add_matrix_form_surf(new DefaultMatrixFormSurf(0, 0, boundary->markers, boundary->value1));
          this->wf->add_vector_form_surf(new DefaultVectorFormSurf(0, boundary->markers, boundary->value2));
        }
      }
    }

    /* Materials */
    for (unsigned int i = 0; i < this->materials.size(); i++)
    {
      BasicMaterialData *material = dynamic_cast<BasicMaterialData *>(materials.at(i));

      Hermes::vector<std::string> markers = material->markers;
      this->wf->add_matrix_form(new DefaultJacobianDiffusion(0, 0, markers, material->c1));
      this->wf->add_matrix_form(new DefaultMatrixFormVol(0, 0, markers, material->c2));
      this->wf->add_matrix_form(new DefaultJacobianAdvection(0, 0, markers, material->c2)); // FIXME (material->c3)
      this->wf->add_vector_form(new DefaultResidualDiffusion(0, markers, material->c1));
      this->wf->add_vector_form(new DefaultResidualVol(0, markers, material->c2));
      this->wf->add_vector_form(new DefaultResidualAdvection(0, markers, material->c2)); // FIXME (material->c3)
      this->wf->add_vector_form(new DefaultVectorFormVol(0, markers, material->c5));
    }
  }

  virtual void set_spaces() {
    this->spaces.push_back(new H1Space(this->get_mesh(0), &this->bcs, this->properties()->mesh()->init_deg));
  }
};
