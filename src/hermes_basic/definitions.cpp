// Weak forms for module Basic.

class CustomWeakFormModuleBasic : public WeakForm
{
public:
  CustomWeakFormModuleBasic() : WeakForm(1) { };

  // Set material markers.
  void CustomWeakFormModuleBasic::set_material_markers(const std::vector<std::string> &m_markers)
  {
    for (unsigned int i = 0; i < m_markers.size(); i++) {
      this->m_markers.push_back(m_markers[i]);
    }
  }

  // Set c1 array.
  void CustomWeakFormModuleBasic::set_c1_array(const std::vector<double> &c1_array)
  {
    for (unsigned int i = 0; i < c1_array.size(); i++) {
      this->c1_array.push_back(c1_array[i]);
    }
  }

  // Set c2 array.
  void CustomWeakFormModuleBasic::set_c2_array(const std::vector<double> &c2_array)
  {
    for (unsigned int i = 0; i < c2_array.size(); i++) {
      this->c2_array.push_back(c2_array[i]);
    }
  }

  // Set c3 array.
  void CustomWeakFormModuleBasic::set_c3_array(const std::vector<double> &c3_array)
  {
    for (unsigned int i = 0; i < c3_array.size(); i++) {
      this->c3_array.push_back(c3_array[i]);
    }
  }

  // Set c4 array.
  void CustomWeakFormModuleBasic::set_c4_array(const std::vector<double> &c4_array)
  {
    for (unsigned int i = 0; i < c4_array.size(); i++) {
      this->c4_array.push_back(c4_array[i]);
    }
  }

  // Set c5 array.
  void CustomWeakFormModuleBasic::set_c5_array(const std::vector<double> &c5_array)
  {
    for (unsigned int i = 0; i < c5_array.size(); i++) {
      this->c5_array.push_back(c5_array[i]);
    }
  }

  // Sanity check of material markers and material constants.
  void CustomWeakModuleBasic::materials_sanity_check()
  {
    if (this->mat_markers.size() != this->c1_array.size()) error("Wrong length of c1 array.");
    if (this->mat_markers.size() != this->c2_array.size()) error("Wrong length of c2 array.");
    if (this->mat_markers.size() != this->c3_array.size()) error("Wrong length of c3 array.");
    if (this->mat_markers.size() != this->c4_array.size()) error("Wrong length of c4 array.");
    if (this->mat_markers.size() != this->c5_array.size()) error("Wrong length of c5 array.");
  }

  // Set Neumann boundary markers.
  void CustomWeakFormModuleBasic::set_neumann_markers(const std::vector<int> &bdy_markers_neumann)
  {
    for (unsigned int i = 0; i < bdy_markers_neumann.size(); i++) {
      this->bdy_markers_neumann.push_back(bdy_markers_neumann[i]);
    }
  }

  // Set Neumann boundary values.
  void CustomWeakFormModuleBasic::set_neumann_values(const std::vector<double> &bdy_values_neumann)
  {
    for (unsigned int i = 0; i < bdy_values_neumann.size(); i++) {
      this->bdy_values_neumann.push_back(bdy_values_neumann[i]);
    }
  }

  // Set Newton boundary markers.
  void CustomWeakFormModuleBasic::set_newton_markers(const std::vector<int> &bdy_markers_newton)
  {
    for (unsigned int i = 0; i < bdy_markers_newton.size(); i++) {
      this->bdy_markers_newton.push_back(bdy_markers_newton[i]);
    }
  }

  // Set Newton boundary values.
  void CustomWeakFormModuleBasic::set_newton_values(const std::vector<double_pair> &bdy_values_newton)
  {
    for (unsigned int i = 0; i < bdy_values_newton.size(); i++) {
      this->bdy_values_newton.push_back(bdy_values_newton[i]);
    }
  }

  // Register all volumetric and surface forms.
  void CustomWeakFormModuleBasic::create() {
    // First add volumetric forms.
    for (int i=0; i<this->m_markers.size(); i++) {
      add_matrix_form(new CustomMatrixFormVolConstSym(this->c1_array[i], this->c4_array[i], this->m_markers[i]));
      add_matrix_form(new CustomMatrixFormVolConstNonym(this->c2_array[i], this->c3_array[i], this->m_markers[i]));
      add_vector_form(new DefaultVectorFormVolConst(0, this->c5_array[i]));
    }

    // Surface forms.
    // FIXME: WHAT ABOUT THE PERMUT ARRAY.
    add_vector_form_surf(new DefaultVectorFormSurfConst(0, bdy_bottom, const_gamma_bottom));
    add_vector_form_surf(new DefaultVectorFormSurfConst(0, bdy_outer, const_gamma_outer));
    add_vector_form_surf(new DefaultVectorFormSurfConst(0, bdy_left, const_gamma_left));
  }

  private:
    // Material markers.
    std::vector<std::string> m_markers;          // List of material markers.
    // Equation constants.
    std::vector<double> c1_array;                // Equation parameter c1 (array for all material subdomains).
    std::vector<double> c2_array;                // Equation parameter c2 (array for all material subdomains).
    std::vector<double> c3_array;                // Equation parameter c3 (array for all material subdomains).
    std::vector<double> c4_array;                // Equation parameter c4 (array for all material subdomains).
    std::vector<double> c5_array;                // Equation parameter c5 (array for all material subdomains).
    // Natural boundary conditions.
    std::vector<std:string> bdy_markers_neumann;      // List of Neumann boundary markers.
    std::vector<double> bdy_values_neumann;           // List of Neumann boundary values.
    std::vector<std::string> bdy_markers_newton;      // List of Newton boundary markers.
    std::vector<double_pair> bdy_values_newton;       // List of Newton boundary value pairs.
};

/* Matrix and vector forms */

class CustomMatrixFormVolConstSym : public WeakForm::MatrixFormVol
{
public:
  CustomMatrixFormVolConstSym(double c1, double c4, std::string area) 
    : WeakForm::MatrixFormVol(0, 0, HERMES_SYM, area), c1(c1), c4(c4) { }

  template<typename Real, typename Scalar>
  Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, 
                     Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
    return   c1 * int_grad_u_grad_v<Real, Scalar>(n, wt, u, v)
           + c4 * int_u_v<Real, Scalar>(n, wt, u, v);
  }

  scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, 
               Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
    return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
  }

  Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, 
          Geom<Ord> *e, ExtData<Ord> *ext) {
    return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
  }

  scalar c1, c4;
};

class CustomMatrixFormVolConstNonsym : public WeakForm::MatrixFormVol
{
public:
  CustomMatrixFormVolConstNonsym(double c2, double c3, std::string area) 
    : WeakForm::MatrixFormVol(0, 0, HERMES_NONSYM, area), c2(c2), c3(c3) { }

  template<typename Real, typename Scalar>
  Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, 
                     Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
    return   c2 * int_dudx_v<Real, Scalar>(n, wt, u, v)
           + c3 * int_dudy_v<Real, Scalar>(n, wt, u, v);
  }

  scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, 
               Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
    return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
  }

  Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, 
          Geom<Ord> *e, ExtData<Ord> *ext) {
    return matrix_form<Ord, Ord>(n, wt, u_ext, u, v, e, ext);
  }
  
  scalar c2, c3;
};






















// Weak form (volumetric, left-hand side).
template<typename Real, typename Scalar>
Scalar bilinear_form_vol(int n, double *wt, Func<Real> *u_ext[], Func<Real> *u, Func<Real> *v, 
                         Geom<Real> *e, ExtData<Scalar> *ext)
{
  int elem_marker = e->elem_marker;
  double c1, c2, c3, c4;
  if (elem_marker < 0) {
    // This is for Order calculation only:
    c1 = c2 = c3 = c4 = -5555.0;
  } else {
    // FIXME: these global arrays need to be removed.
    int index = _global_mat_markers.find_index(elem_marker);
    c1 = _global_c1_array[index];
    c2 = _global_c2_array[index];
    c3 = _global_c3_array[index];
    c4 = _global_c4_array[index];
  }
  return  
      c1 * int_grad_u_grad_v<Real, Scalar>(n, wt, u, v)
    + c2 * int_dudx_v<Real, Scalar>(n, wt, u, v)
    + c3 * int_dudy_v<Real, Scalar>(n, wt, u, v)
    + c4 * int_u_v<Real, Scalar>(n, wt, u, v);
}

// Weak form (volumetric, right-hand side).
template<typename Real, typename Scalar>
Scalar linear_form_vol(int n, double *wt, Func<Real> *u_ext[], Func<Real> *v, 
                       Geom<Real> *e, ExtData<Scalar> *ext)
{
  int elem_marker = e->elem_marker;
  double c5;
  if (elem_marker < 0) {
    // This is for Order calculation only:
    c5 = -5555.0;
  } else {
    // FIXME: these global arrays need to be removed.
    int index = _global_mat_markers.find_index(elem_marker);
    c5 = _global_c5_array[index];
  }
  return c5 * int_v<Real, Scalar>(n, wt, v);
}

// Weak form (surface, left-hand side).
template<typename Real, typename Scalar>
Scalar bilinear_form_surf_newton(int n, double *wt, Func<Real> *u_ext[], Func<Real> *u, Func<Real> *v, 
                          Geom<Real> *e, ExtData<Scalar> *ext)
{
  int edge_marker = e->edge_marker;
  int elem_marker = e->elem_marker;
  double const_newton_1;
  double c1;
  if (edge_marker < 0) {
    // This is for Order calculation only:
    const_newton_1 = -5555.0;
    c1 = -5555.0;
  } else {
    // FIXME: these global arrays need to be removed.
    if (_global_bdy_values_newton.size() > 0) {
      double_pair newton_pair = _global_bdy_values_newton[_global_bc_types->find_index_newton(edge_marker)];
      const_newton_1 = newton_pair.first;
    }
    else error("Internal in ModuleBasic: bilinear_form_surf_newton() should not have been called.");
    // FIXME: these global arrays need to be removed.
    c1 = _global_c1_array[_global_mat_markers.find_index(elem_marker)];
  }
  return c1 * const_newton_1 * int_u_v<Real, Scalar>(n, wt, u, v);
}

// Weak form (surface, neumann, right-hand side).
template<typename Real, typename Scalar>
Scalar linear_form_surf_neumann(int n, double *wt, Func<Real> *u_ext[], Func<Real> *v, 
                        Geom<Real> *e, ExtData<Scalar> *ext)
{
  int edge_marker = e->edge_marker;
  int elem_marker = e->elem_marker;
  double const_neumann;
  double c1;
  double result = 0;
  if (edge_marker < 0) {
    // This is for Order calculation only:
    const_neumann = -5555.0;
    c1 = -5555.0;
  } else {
    // FIXME: these global arrays need to be removed.
    if (_global_bdy_values_neumann.size() > 0) {
      int index = _global_bc_types->find_index_neumann(edge_marker);
      const_neumann = _global_bdy_values_neumann[index];
    }
    else error("Internal in ModuleBasic: linear_form_surf_neumann() should not have been called.");
    // FIXME: these global arrays need to be removed.
    c1 = _global_c1_array[_global_mat_markers.find_index(elem_marker)];  
  }
  return c1 * const_neumann * int_v<Real, Scalar>(n, wt, v);
}

// Weak form (surface, newton, right-hand side).
template<typename Real, typename Scalar>
Scalar linear_form_surf_newton(int n, double *wt, Func<Real> *u_ext[], Func<Real> *v, 
                        Geom<Real> *e, ExtData<Scalar> *ext)
{
  int edge_marker = e->edge_marker;
  int elem_marker = e->elem_marker;
  double const_newton_2;
  double c1;
  double result = 0;
  if (edge_marker < 0) {
    // This is for Order calculation only:
    const_newton_2 = -5555.0;
    c1 = -5555.0;
  } else {
    // FIXME: these global arrays need to be removed.
    if (_global_bdy_values_newton.size() > 0) {
      int index = _global_bc_types->find_index_newton(edge_marker);
      double_pair newton_pair = _global_bdy_values_newton[index];
      const_newton_2 = newton_pair.second;
    }
    else error("Internal in ModuleBasic: linear_form_surf_newton() should not have been called.");
    // FIXME: these global arrays need to be removed.
    c1 = _global_c1_array[_global_mat_markers.find_index(elem_marker)];  
  }
  return c1 * const_newton_2 * int_v<Real, Scalar>(n, wt, v);
}
