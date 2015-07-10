/* Produced by CVXGEN, 2015-07-07 00:46:48 -0400.  */
/* CVXGEN is Copyright (C) 2006-2012 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2012 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */

/* Filename: testsolver.c. */
/* Description: Basic test harness for solver.c. */
#include "solver.h"
Vars vars;
Params params;
Workspace work;
Settings settings;
#define NUMTESTS 0
int main(int argc, char **argv) {
  int num_iters;
#if (NUMTESTS > 0)
  int i;
  double time;
  double time_per;
#endif
  set_defaults();
  setup_indexing();
  load_default_data();
  /* Solve problem instance for the record. */
  settings.verbose = 1;
  num_iters = solve();
#ifndef ZERO_LIBRARY_MODE
#if (NUMTESTS > 0)
  /* Now solve multiple problem instances for timing purposes. */
  settings.verbose = 0;
  tic();
  for (i = 0; i < NUMTESTS; i++) {
    solve();
  }
  time = tocq();
  printf("Timed %d solves over %.3f seconds.\n", NUMTESTS, time);
  time_per = time / NUMTESTS;
  if (time_per > 1) {
    printf("Actual time taken per solve: %.3g s.\n", time_per);
  } else if (time_per > 1e-3) {
    printf("Actual time taken per solve: %.3g ms.\n", 1e3*time_per);
  } else {
    printf("Actual time taken per solve: %.3g us.\n", 1e6*time_per);
  }
#endif
#endif
  return 0;
}
void load_default_data(void) {
  /* Make this a diagonal PSD matrix, even though it's not diagonal. */
  params.Ohm[0] = 1.5507979025745755;
  params.Ohm[2] = 0;
  params.Ohm[1] = 0;
  params.Ohm[3] = 1.7081478226181048;
  params.tau[0] = -0.8363810443482227;
  params.tau[1] = 0.04331042079065206;
  params.tau[2] = 1.5717878173906188;
  params.Ba[0] = 1.5851723557337523;
  params.Ba[1] = -1.497658758144655;
  params.Ba[2] = -1.171028487447253;
  params.Ba[3] = -1.7941311867966805;
  params.Ba[4] = -0.23676062539745413;
  params.Ba[5] = -1.8804951564857322;
  params.u_0[0] = -0.17266710242115568;
  params.u_0[1] = 0.596576190459043;
  params.jBa_u[0] = -0.8860508694080989;
  params.jBa_u[1] = 0.7050196079205251;
  params.jBa_u[2] = 0.3634512696654033;
  params.jBa_u[3] = -1.9040724704913385;
  params.jBa_u[4] = 0.23541635196352795;
  params.jBa_u[5] = -0.9629902123701384;
  /* Make this a diagonal PSD matrix, even though it's not diagonal. */
  params.Q[0] = 1.4151011970100695;
  params.Q[3] = 0;
  params.Q[6] = 0;
  params.Q[1] = 0;
  params.Q[4] = 1.2835250817713186;
  params.Q[7] = 0;
  params.Q[2] = 0;
  params.Q[5] = 0;
  params.Q[8] = 1.6931379183129964;
  params.u_min[0] = -0.23818512931704205;
  params.u_min[1] = -1.372529046100147;
  params.u_max[0] = 0.17859607212737894;
  params.u_max[1] = 1.1212590580454682;
  params.a_min[0] = -0.774545870495281;
  params.a_min[1] = -1.1121684642712744;
  params.a_0[0] = -0.44811496977740495;
  params.a_0[1] = 1.7455345994417217;
  params.a_max[0] = 1.9039816898917352;
  params.a_max[1] = 0.6895347036512547;
  params.da_max[0] = 1.6113364341535923;
  params.da_max[1] = 1.383003485172717;
}
