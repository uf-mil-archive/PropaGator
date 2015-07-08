#include "solver.h"
#include <boost/python.hpp>
#include "boost/python/extract.hpp"
#include "boost/python/numeric.hpp"
#include <iostream>

Vars vars;
Params params;
Workspace work;
Settings settings;

char const* smook() {
  return "ss";
}

char const* test(boost::python::numeric::array data)
{
  std::cout << "First array item: " << boost::python::extract<double>(data[0]) << std::endl;
  return "test!";
}

boost::python::list cvxsolve(
  boost::python::numeric::array u_0,
  boost::python::numeric::array a_0,
  boost::python::numeric::array Ohm,
  boost::python::numeric::array Q,
  boost::python::numeric::array Ba,
  boost::python::numeric::array jBa_u,
  double u_max,
  double u_min,
  double a_min,
  double a_max,
  double da_max,
  boost::python::numeric::array tau
)
{
  /* Compute a thrust allocation solution */
  set_defaults();
  setup_indexing();
  settings.verbose = 0;
  // load_default_data();

  params.u_min[0] = u_min;
  params.u_max[0] = u_max;
  params.a_min[0] = a_min;
  params.a_max[0] = a_max;
  params.da_max[0] = da_max;

  params.u_min[1] = u_min;
  params.u_max[1] = u_max;
  params.a_min[1] = a_min;
  params.a_max[1] = a_max;
  params.da_max[1] = da_max;

  for (unsigned int ii=0; ii < boost::python::len(Q); ii++)
  {
    params.Q[ii] = boost::python::extract<double>(Q[ii]);
  }

  for (unsigned int ii=0; ii < boost::python::len(Ohm); ii++)
  {
    params.Ohm[ii] = boost::python::extract<double>(Ohm[ii]);
  }

  for (unsigned int ii=0; ii < boost::python::len(tau); ii++)
  {
    params.tau[ii] = boost::python::extract<double>(tau[ii]);
  }

  for (unsigned int ii=0; ii < boost::python::len(u_0); ii++)
  {
    params.u_0[ii] = boost::python::extract<double>(u_0[ii]);
    params.a_0[ii] = boost::python::extract<double>(a_0[ii]);
  }

  for (unsigned int ii=0; ii < boost::python::len(jBa_u); ii++)
  {
    params.jBa_u[ii] = boost::python::extract<double>(jBa_u[ii]);
    params.Ba[ii] = boost::python::extract<double>(Ba[ii]);

  }

  int num_iters = solve();

  if (work.converged == 1) {
    // std::cout << "Converged" << std::endl;
  }

  boost::python::list results;

  // boost::python::numeric::array()

  results.append(vars.du[0]);
  results.append(vars.du[1]);
  results.append(vars.da[0]);
  results.append(vars.da[1]);
  // results.append(vars.s[0]);
  // results.append(vars.s[1]);
  // results.append(vars.s[2]);

  // results.append(boost::python::numeric::array(vars.du));
  // results.append(boost::python::numeric::array(vars.da));
  // results.append(num_iters);

  return(results);
}

BOOST_PYTHON_MODULE(azi_cvxbind)
{                   
  using namespace boost::python;

  boost::python::numeric::array::set_module_and_type("numpy", "ndarray");
  def("bind_test", test);
  def("ksolve", cvxsolve);
}