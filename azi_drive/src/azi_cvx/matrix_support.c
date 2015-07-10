/* Produced by CVXGEN, 2015-07-07 00:46:47 -0400.  */
/* CVXGEN is Copyright (C) 2006-2012 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2012 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */

/* Filename: matrix_support.c. */
/* Description: Support functions for matrix multiplication and vector filling. */
#include "solver.h"
void multbymA(double *lhs, double *rhs) {
  lhs[0] = -rhs[0]*(-params.jBa_u[0])-rhs[1]*(-params.jBa_u[3])-rhs[2]*(-params.Ba[0])-rhs[3]*(-params.Ba[3])-rhs[4]*(-1);
  lhs[1] = -rhs[0]*(-params.jBa_u[1])-rhs[1]*(-params.jBa_u[4])-rhs[2]*(-params.Ba[1])-rhs[3]*(-params.Ba[4])-rhs[5]*(-1);
  lhs[2] = -rhs[0]*(-params.jBa_u[2])-rhs[1]*(-params.jBa_u[5])-rhs[2]*(-params.Ba[2])-rhs[3]*(-params.Ba[5])-rhs[6]*(-1);
}
void multbymAT(double *lhs, double *rhs) {
  lhs[0] = -rhs[0]*(-params.jBa_u[0])-rhs[1]*(-params.jBa_u[1])-rhs[2]*(-params.jBa_u[2]);
  lhs[1] = -rhs[0]*(-params.jBa_u[3])-rhs[1]*(-params.jBa_u[4])-rhs[2]*(-params.jBa_u[5]);
  lhs[2] = -rhs[0]*(-params.Ba[0])-rhs[1]*(-params.Ba[1])-rhs[2]*(-params.Ba[2]);
  lhs[3] = -rhs[0]*(-params.Ba[3])-rhs[1]*(-params.Ba[4])-rhs[2]*(-params.Ba[5]);
  lhs[4] = -rhs[0]*(-1);
  lhs[5] = -rhs[1]*(-1);
  lhs[6] = -rhs[2]*(-1);
}
void multbymG(double *lhs, double *rhs) {
  lhs[0] = -rhs[2]*(-1);
  lhs[1] = -rhs[3]*(-1);
  lhs[2] = -rhs[2]*(1);
  lhs[3] = -rhs[3]*(1);
  lhs[4] = -rhs[0]*(-1);
  lhs[5] = -rhs[1]*(-1);
  lhs[6] = -rhs[0]*(1);
  lhs[7] = -rhs[1]*(1);
  lhs[8] = -rhs[0]*(-1);
  lhs[9] = -rhs[1]*(-1);
  lhs[10] = -rhs[0]*(1);
  lhs[11] = -rhs[1]*(1);
}
void multbymGT(double *lhs, double *rhs) {
  lhs[0] = -rhs[4]*(-1)-rhs[6]*(1)-rhs[8]*(-1)-rhs[10]*(1);
  lhs[1] = -rhs[5]*(-1)-rhs[7]*(1)-rhs[9]*(-1)-rhs[11]*(1);
  lhs[2] = -rhs[0]*(-1)-rhs[2]*(1);
  lhs[3] = -rhs[1]*(-1)-rhs[3]*(1);
  lhs[4] = 0;
  lhs[5] = 0;
  lhs[6] = 0;
}
void multbyP(double *lhs, double *rhs) {
  /* TODO use the fact that P is symmetric? */
  /* TODO check doubling / half factor etc. */
  lhs[0] = rhs[0]*(2*params.Ohm[0])+rhs[1]*(2*params.Ohm[2]);
  lhs[1] = rhs[0]*(2*params.Ohm[1])+rhs[1]*(2*params.Ohm[3]);
  lhs[2] = 0;
  lhs[3] = 0;
  lhs[4] = rhs[4]*(2*params.Q[0])+rhs[5]*(2*params.Q[3])+rhs[6]*(2*params.Q[6]);
  lhs[5] = rhs[4]*(2*params.Q[1])+rhs[5]*(2*params.Q[4])+rhs[6]*(2*params.Q[7]);
  lhs[6] = rhs[4]*(2*params.Q[2])+rhs[5]*(2*params.Q[5])+rhs[6]*(2*params.Q[8]);
}
void fillq(void) {
  work.q[0] = 0;
  work.q[1] = 0;
  work.q[2] = 0;
  work.q[3] = 0;
  work.q[4] = 0;
  work.q[5] = 0;
  work.q[6] = 0;
}
void fillh(void) {
  work.h[0] = -(params.u_min[0]-params.u_0[0]);
  work.h[1] = -(params.u_min[1]-params.u_0[1]);
  work.h[2] = params.u_max[0]-params.u_0[0];
  work.h[3] = params.u_max[1]-params.u_0[1];
  work.h[4] = -(params.a_min[0]-params.a_0[0]);
  work.h[5] = -(params.a_min[1]-params.a_0[1]);
  work.h[6] = params.a_max[0]-params.a_0[0];
  work.h[7] = params.a_max[1]-params.a_0[1];
  work.h[8] = params.da_max[0];
  work.h[9] = params.da_max[1];
  work.h[10] = params.da_max[0];
  work.h[11] = params.da_max[1];
}
void fillb(void) {
  work.b[0] = -(params.tau[0]-(params.Ba[0]*params.u_0[0]+params.Ba[3]*params.u_0[1]));
  work.b[1] = -(params.tau[1]-(params.Ba[1]*params.u_0[0]+params.Ba[4]*params.u_0[1]));
  work.b[2] = -(params.tau[2]-(params.Ba[2]*params.u_0[0]+params.Ba[5]*params.u_0[1]));
}
void pre_ops(void) {
}
