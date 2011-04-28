# include "/home/pavel/repos/hermes/hermes2d/src/weakform_library/h1.h"

#include <map>
#include <string>

using namespace Laplace::VolumetricMatrixForms;
using namespace Laplace::VolumetricVectorForms;
using namespace Laplace::SurfaceMatrixForms;
using namespace Laplace::SurfaceVectorForms;
using namespace Laplace::RightHandSides;

/* Weak forms */

// Custom surface vector form for the Neumann condition.
class CustomVectorFormSurfNeumann : public WeakForm::VectorFormSurf
{
public:
  CustomVectorFormSurfNeumann(int i, double neumenn_coeff, materials_map c1_pairs) 
         : WeakForm::VectorFormSurf(i), neumann_coeff(neumann_coeff), c1_pairs(c1_pairs) { }
  CustomVectorFormSurfNeumann(int i, std::string area, double neumann_coeff, materials_map c1_pairs) 
         : WeakForm::VectorFormSurf(i, area), neumann_coeff(neumann_coeff), c1_pairs(c1_pairs) { }

  template<typename Real, typename Scalar>
  Scalar vector_form_surf(int n, double *wt, Func<Scalar> *u_ext[], 
                          Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
    double c1;
    if (e->elem_marker == -9999) c1 = 1.0;         // Automatic order determination.
    else {
      std::string marker = get_elem_marker(e);
      c1 = c1_pairs[marker];
    }
    return c1 * neumann_coeff * int_v<Real, Scalar>(n, wt, v);
  }

  scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v, 
               Geom<double> *e, ExtData<scalar> *ext) {
    return vector_form_surf<scalar, scalar>(n, wt, u_ext, v, e, ext);
  }

  Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
    return vector_form_surf<Ord, Ord>(n, wt, u_ext, v, e, ext);
  }

private:
  double neumann_coeff;
  materials_map c1_pairs;
};

class CustomMatrixFormSurfNewton : public WeakForm::MatrixFormSurf
{
public:
  CustomMatrixFormSurfNewton(int i, int j, double newton_coeff, materials_map c1_pairs
) 
        : WeakForm::MatrixFormSurf(i, j), newton_coeff(newton_coeff), c1_pairs(c1_pairs) { }
  CustomMatrixFormSurfNewton(int i, int j, std::string area, double newton_coeff, materials_map c1_pairs) 
        : WeakForm::MatrixFormSurf(i, j, area), newton_coeff(newton_coeff), c1_pairs(c1_pairs) { }

  template<typename Real, typename Scalar>
  Scalar matrix_form_surf(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, 
                          Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
    double c1;
    if (e->elem_marker == -9999) c1 = 1.0;         // Automatic order determination.
    else {
      std::string marker = get_elem_marker(e);
      c1 = c1_pairs[marker];
    }
    return c1 * newton_coeff * int_u_v<Real, Scalar>(n, wt, u, v);
  }

  scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, 
               Geom<double> *e, ExtData<scalar> *ext) {
    return matrix_form_surf<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
  }

  Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, 
          Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
    return matrix_form_surf<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
  }

private:
  double newton_coeff;
  materials_map c1_pairs;
};

class CustomVectorFormSurfNewton : public WeakForm::VectorFormSurf
{
public:
  CustomVectorFormSurfNewton(int i, double newton_coeff, materials_map c1_pairs) 
    : WeakForm::VectorFormSurf(i), newton_coeff(newton_coeff), c1_pairs(c1_pairs) { }
  CustomVectorFormSurfNewton(int i, std::string area, double newton_coeff, materials_map c1_pairs) 
         : WeakForm::VectorFormSurf(i, area), newton_coeff(newton_coeff), c1_pairs(c1_pairs) { }

  template<typename Real, typename Scalar>
  Scalar vector_form_surf(int n, double *wt, Func<Scalar> *u_ext[], 
                          Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
    double c1;
    if (e->elem_marker == -9999) c1 = 1.0;         // Automatic order determination.
    else {
      std::string marker = get_elem_marker(e);
      c1 = c1_pairs[marker];
    }
    return c1 * newton_coeff * int_v<Real, Scalar>(n, wt, v);
  }

  scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v, 
               Geom<double> *e, ExtData<scalar> *ext) {
    return vector_form_surf<scalar, scalar>(n, wt, u_ext, v, e, ext);
  }

  Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
    return vector_form_surf<Ord, Ord>(n, wt, u_ext, v, e, ext);
  }

private:
  double newton_coeff;
  materials_map c1_pairs;
};

// Weak formulation with all matrix and surface forms.
class CustomWeakFormModuleBasic : public WeakForm
{
public:
  CustomWeakFormModuleBasic() : WeakForm(1) { 
    // Material markers.
    this->mat_markers = std::vector<std::string>();
    // Equation constants.
    this->c1_array = std::vector<double>();
    this->c2_array = std::vector<double>();
    this->c3_array = std::vector<double>();
    this->c4_array = std::vector<double>();
    this->c5_array = std::vector<double>();
    // Natural boundary conditions.
    this->bdy_markers_neumann = std::vector<std::string>();
    this->bdy_values_neumann = std::vector<double>();
    this->bdy_markers_newton = std::vector<std::string>();
    this->bdy_values_newton = std::vector<double_pair>();
    // Map to link material markers with c1_array constants. 
    this->c1_pairs = std::map<std::string, double>();
  };

  // Set material markers.
  void set_material_markers(const std::vector<std::string> &mat_markers)
  {
    for (unsigned int i = 0; i < mat_markers.size(); i++) {
      this->mat_markers.push_back(mat_markers[i]);
    }
  }

  // Set c1 array.
  void set_c1_array(const std::vector<double> &c1_array)
  {
    for (unsigned int i = 0; i < c1_array.size(); i++) {
      this->c1_array.push_back(c1_array[i]);
    }
  }

  // Set c2 array.
  void set_c2_array(const std::vector<double> &c2_array)
  {
    for (unsigned int i = 0; i < c2_array.size(); i++) {
      this->c2_array.push_back(c2_array[i]);
    }
  }

  // Set c3 array.
  void set_c3_array(const std::vector<double> &c3_array)
  {
    for (unsigned int i = 0; i < c3_array.size(); i++) {
      this->c3_array.push_back(c3_array[i]);
    }
  }

  // Set c4 array.
  void set_c4_array(const std::vector<double> &c4_array)
  {
    for (unsigned int i = 0; i < c4_array.size(); i++) {
      this->c4_array.push_back(c4_array[i]);
    }
  }

  // Set c5 array.
  void set_c5_array(const std::vector<double> &c5_array)
  {
    for (unsigned int i = 0; i < c5_array.size(); i++) {
      this->c5_array.push_back(c5_array[i]);
    }
  }

  // Sanity check of material markers and material constants.
  void sanity_check()
  {
    // Number of material markers and the material constants must be the same.
    if (this->mat_markers.size() != this->c1_array.size()) error("Wrong length of c1 array.");
    if (this->mat_markers.size() != this->c2_array.size()) error("Wrong length of c2 array.");
    if (this->mat_markers.size() != this->c3_array.size()) error("Wrong length of c3 array.");
    if (this->mat_markers.size() != this->c4_array.size()) error("Wrong length of c4 array.");
    if (this->mat_markers.size() != this->c5_array.size()) error("Wrong length of c5 array.");

    // Number of Neumann values and markers must be the same
    if (this->bdy_markers_neumann.size() != this->bdy_values_neumann.size()) 
      error("Wrong number of Neumann constants or markers.");

    // Number of Newton values and markers must be the same
    if (this->bdy_markers_newton.size() != this->bdy_values_newton.size()) 
      error("Wrong number of Newton constants or markers.");
  }

  // Set Neumann boundary markers.
  void set_neumann_markers(const std::vector<std::string> &bdy_markers_neumann)
  {
    for (unsigned int i = 0; i < bdy_markers_neumann.size(); i++) {
      this->bdy_markers_neumann.push_back(bdy_markers_neumann[i]);
    }
  }

  // Set Neumann boundary values.
  void set_neumann_values(const std::vector<double> &bdy_values_neumann)
  {
    for (unsigned int i = 0; i < bdy_values_neumann.size(); i++) {
      this->bdy_values_neumann.push_back(bdy_values_neumann[i]);
    }
  }

  // Set Newton boundary markers.
  void set_newton_markers(const std::vector<std::string> &bdy_markers_newton)
  {
    for (unsigned int i = 0; i < bdy_markers_newton.size(); i++) {
      this->bdy_markers_newton.push_back(bdy_markers_newton[i]);
    }
  }

  // Set Newton boundary values.
  void set_newton_values(const std::vector<double_pair> &bdy_values_newton)
  {
    for (unsigned int i = 0; i < bdy_values_newton.size(); i++) {
      this->bdy_values_newton.push_back(bdy_values_newton[i]);
    }
  }

  // Register all volumetric and surface forms.
  void create() {
    // Merge material markers and c1_array into a map (needed for surface forms).
    for (unsigned int i=0; i < mat_markers.size(); i++) {
      c1_pairs[mat_markers[i]] = c1_array[i];
    }

    // First add volumetric forms for all material markers.
    for (unsigned int i=0; i<this->mat_markers.size(); i++) {
      add_matrix_form(new DefaultMatrixFormStiffness(0, 0, this->mat_markers[i], this->c1_array[i]));
      add_matrix_form(new DefaultMatrixFormMass(0, 0, this->mat_markers[i], this->c4_array[i]));
      add_matrix_form(new DefaultMatrixFormAdvection(0, 0, this->mat_markers[i], 
						     this->c2_array[i], this->c3_array[i]));
      add_vector_form(new DefaultVectorFormConst(0, this->mat_markers[i], this->c5_array[i]));
    }

    // Add surface vector forms for all Neumann markers.
    for (unsigned int i=0; i<this->bdy_markers_neumann.size(); i++) {
      add_vector_form_surf(new CustomVectorFormSurfNeumann(0, this->bdy_markers_neumann[i], 
                                                           this->bdy_values_neumann[i], this->c1_pairs));
    }

    // Add surface and matrix forms for all Newton markers.
    for (unsigned int i=0; i<this->bdy_markers_newton.size(); i++) {
      add_matrix_form_surf(new CustomMatrixFormSurfNewton(0, 0, this->bdy_markers_newton[i], 
                           this->bdy_values_newton[i].first, this->c1_pairs));
      add_vector_form_surf(new CustomVectorFormSurfNewton(0, this->bdy_markers_newton[i], 
                           this->bdy_values_newton[i].second, this->c1_pairs));
    }
  }

  private:
    // Material markers.
    std::vector<std::string> mat_markers;        // List of material markers.
    // Equation constants.
    std::vector<double> c1_array;                // Equation parameter c1 (array for all material subdomains).
    std::vector<double> c2_array;                // Equation parameter c2 (array for all material subdomains).
    std::vector<double> c3_array;                // Equation parameter c3 (array for all material subdomains).
    std::vector<double> c4_array;                // Equation parameter c4 (array for all material subdomains).
    std::vector<double> c5_array;                // Equation parameter c5 (array for all material subdomains).
    // Natural boundary conditions.
    std::vector<std::string> bdy_markers_neumann;     // List of Neumann boundary markers.
    std::vector<double> bdy_values_neumann;           // List of Neumann boundary values.
    std::vector<std::string> bdy_markers_newton;      // List of Newton boundary markers.
    std::vector<double_pair> bdy_values_newton;       // List of Newton boundary value pairs.
    // Map to link material markers with c1_array constants. 
    materials_map c1_pairs;
};

