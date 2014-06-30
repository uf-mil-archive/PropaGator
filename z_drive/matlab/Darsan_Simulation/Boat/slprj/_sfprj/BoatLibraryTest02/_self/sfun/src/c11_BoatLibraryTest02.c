/* Include files */

#include <stddef.h>
#include "blas.h"
#include "BoatLibraryTest02_sfun.h"
#include "c11_BoatLibraryTest02.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "BoatLibraryTest02_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c11_debug_family_names[27] = { "rx1", "rx2", "ry1", "ry2",
  "C", "Fxref", "Fyref", "Mref", "gridNum", "grid", "u1", "k", "h", "alpha",
  "nargin", "nargout", "u0", "rx01", "rx02", "ry01", "ry02", "C0", "inputBound",
  "Fx", "Fy", "M", "output" };

static const char * c11_b_debug_family_names[13] = { "p", "theta1", "f1",
  "theta2", "f2", "i", "gridNum", "nargin", "nargout", "inputBound", "rx1",
  "ry1", "output" };

static const char * c11_c_debug_family_names[22] = { "theta1", "f1", "theta2",
  "f2", "pretheta1", "pretheta2", "Fx", "Fy", "M", "nargin", "nargout", "input",
  "preinput", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1", "ry2", "C",
  "output" };

static const char * c11_d_debug_family_names[22] = { "theta1", "f1", "theta2",
  "f2", "pretheta1", "pretheta2", "Fx", "Fy", "M", "nargin", "nargout", "input",
  "preinput", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1", "ry2", "C",
  "output" };

static const char * c11_e_debug_family_names[22] = { "ii", "iii", "v0", "i", "j",
  "k", "l", "input", "v1", "nargin", "nargout", "grid", "preinput", "Fxref",
  "Fyref", "Mref", "rx1", "rx2", "ry1", "ry2", "C", "output" };

static const char * c11_f_debug_family_names[23] = { "theta1", "f1", "theta2",
  "f2", "pretheta1", "pretheta2", "dVdtheta1", "dVdf1", "dVdtheta2", "dVdf2",
  "nargin", "nargout", "input", "preinput", "Fxref", "Fyref", "Mref", "rx1",
  "rx2", "ry1", "ry2", "C", "output" };

static const char * c11_g_debug_family_names[15] = { "hsd", "i", "nargin",
  "nargout", "input", "preinput", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1",
  "ry2", "C", "output" };

static const char * c11_h_debug_family_names[19] = { "alpha", "u0", "u1", "v0",
  "v1", "nargin", "nargout", "input", "preinput", "h", "Fxref", "Fyref", "Mref",
  "rx1", "rx2", "ry1", "ry2", "C", "output" };

static const char * c11_i_debug_family_names[5] = { "nargin", "nargout", "input",
  "bound", "output" };

static const char * c11_j_debug_family_names[7] = { "k", "i", "nargin",
  "nargout", "input", "inputBound", "output" };

/* Function Declarations */
static void initialize_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance);
static void initialize_params_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance);
static void enable_c11_BoatLibraryTest02(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void disable_c11_BoatLibraryTest02(SFc11_BoatLibraryTest02InstanceStruct *
  chartInstance);
static void c11_update_debugger_state_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance);
static const mxArray *get_sim_state_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance);
static void set_sim_state_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance, const mxArray *c11_st);
static void finalize_c11_BoatLibraryTest02(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void sf_c11_BoatLibraryTest02(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void c11_chartstep_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance);
static void initSimStructsc11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c11_machineNumber, uint32_T
  c11_chartNumber);
static const mxArray *c11_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_output, const char_T *c11_identifier,
  real_T c11_y[4]);
static void c11_b_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[4]);
static void c11_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static const mxArray *c11_b_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static const mxArray *c11_c_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static const mxArray *c11_d_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static real_T c11_c_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId);
static void c11_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static const mxArray *c11_e_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_d_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[32]);
static void c11_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static void c11_e_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[7]);
static void c11_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static void c11_f_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[8]);
static void c11_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static const mxArray *c11_f_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_g_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[5]);
static void c11_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static const mxArray *c11_g_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_h_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[4]);
static void c11_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static const mxArray *c11_h_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_i_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[2]);
static void c11_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static void c11_info_helper(const mxArray **c11_info);
static const mxArray *c11_emlrt_marshallOut(char * c11_u);
static const mxArray *c11_b_emlrt_marshallOut(uint32_T c11_u);
static void c11_eml_scalar_eg(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void c11_boundGrid(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_inputBound[8], real_T c11_rx1, real_T c11_ry1, real_T c11_output[32]);
static void c11_linspace(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_d1, real_T c11_d2, real_T c11_y[5]);
static real_T c11_costFunction(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c11_input[4], real_T c11_preinput[4], real_T c11_Fxref,
  real_T c11_Fyref, real_T c11_Mref, real_T c11_rx1, real_T c11_rx2, real_T
  c11_ry1, real_T c11_ry2, real_T c11_C[7]);
static real_T c11_sind(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_x);
static boolean_T c11_isfinite(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c11_x);
static real_T c11_cosd(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_x);
static real_T c11_mpower(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_a);
static real_T c11_b_costFunction(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c11_input[4], real_T c11_preinput[4], real_T c11_Fxref,
  real_T c11_Fyref, real_T c11_Mref, real_T c11_rx1, real_T c11_rx2, real_T
  c11_ry1, real_T c11_ry2, real_T c11_C[7]);
static void c11_SteepestDescent(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c11_input[4], real_T c11_preinput[4], real_T c11_Fxref,
  real_T c11_Fyref, real_T c11_Mref, real_T c11_rx1, real_T c11_rx2, real_T
  c11_ry1, real_T c11_ry2, real_T c11_C[7], real_T c11_output[4]);
static const mxArray *c11_i_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static int32_T c11_j_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId);
static void c11_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static uint8_T c11_k_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_b_is_active_c11_BoatLibraryTest02, const
  char_T *c11_identifier);
static uint8_T c11_l_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId);
static void c11_b_sind(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T *c11_x);
static void c11_b_cosd(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T *c11_x);
static void init_dsm_address_info(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance)
{
  chartInstance->c11_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c11_is_active_c11_BoatLibraryTest02 = 0U;
}

static void initialize_params_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance)
{
}

static void enable_c11_BoatLibraryTest02(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c11_BoatLibraryTest02(SFc11_BoatLibraryTest02InstanceStruct *
  chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c11_update_debugger_state_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance)
{
}

static const mxArray *get_sim_state_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance)
{
  const mxArray *c11_st;
  const mxArray *c11_y = NULL;
  int32_T c11_i0;
  real_T c11_u[4];
  const mxArray *c11_b_y = NULL;
  uint8_T c11_hoistedGlobal;
  uint8_T c11_b_u;
  const mxArray *c11_c_y = NULL;
  real_T (*c11_output)[4];
  c11_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c11_st = NULL;
  c11_st = NULL;
  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_createcellarray(2), FALSE);
  for (c11_i0 = 0; c11_i0 < 4; c11_i0++) {
    c11_u[c11_i0] = (*c11_output)[c11_i0];
  }

  c11_b_y = NULL;
  sf_mex_assign(&c11_b_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_setcell(c11_y, 0, c11_b_y);
  c11_hoistedGlobal = chartInstance->c11_is_active_c11_BoatLibraryTest02;
  c11_b_u = c11_hoistedGlobal;
  c11_c_y = NULL;
  sf_mex_assign(&c11_c_y, sf_mex_create("y", &c11_b_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c11_y, 1, c11_c_y);
  sf_mex_assign(&c11_st, c11_y, FALSE);
  return c11_st;
}

static void set_sim_state_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance, const mxArray *c11_st)
{
  const mxArray *c11_u;
  real_T c11_dv0[4];
  int32_T c11_i1;
  real_T (*c11_output)[4];
  c11_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c11_doneDoubleBufferReInit = TRUE;
  c11_u = sf_mex_dup(c11_st);
  c11_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c11_u, 0)),
                       "output", c11_dv0);
  for (c11_i1 = 0; c11_i1 < 4; c11_i1++) {
    (*c11_output)[c11_i1] = c11_dv0[c11_i1];
  }

  chartInstance->c11_is_active_c11_BoatLibraryTest02 = c11_k_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c11_u, 1)),
     "is_active_c11_BoatLibraryTest02");
  sf_mex_destroy(&c11_u);
  c11_update_debugger_state_c11_BoatLibraryTest02(chartInstance);
  sf_mex_destroy(&c11_st);
}

static void finalize_c11_BoatLibraryTest02(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
}

static void sf_c11_BoatLibraryTest02(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
  int32_T c11_i2;
  int32_T c11_i3;
  int32_T c11_i4;
  int32_T c11_i5;
  real_T *c11_rx01;
  real_T *c11_rx02;
  real_T *c11_ry01;
  real_T *c11_ry02;
  real_T *c11_Fx;
  real_T *c11_Fy;
  real_T *c11_M;
  real_T (*c11_inputBound)[8];
  real_T (*c11_C0)[7];
  real_T (*c11_output)[4];
  real_T (*c11_u0)[4];
  c11_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c11_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c11_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c11_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S, 6);
  c11_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
  c11_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c11_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c11_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c11_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c11_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c11_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 8U, chartInstance->c11_sfEvent);
  for (c11_i2 = 0; c11_i2 < 4; c11_i2++) {
    _SFD_DATA_RANGE_CHECK((*c11_u0)[c11_i2], 0U);
  }

  for (c11_i3 = 0; c11_i3 < 4; c11_i3++) {
    _SFD_DATA_RANGE_CHECK((*c11_output)[c11_i3], 1U);
  }

  _SFD_DATA_RANGE_CHECK(*c11_rx01, 2U);
  _SFD_DATA_RANGE_CHECK(*c11_rx02, 3U);
  _SFD_DATA_RANGE_CHECK(*c11_ry01, 4U);
  _SFD_DATA_RANGE_CHECK(*c11_ry02, 5U);
  for (c11_i4 = 0; c11_i4 < 7; c11_i4++) {
    _SFD_DATA_RANGE_CHECK((*c11_C0)[c11_i4], 6U);
  }

  for (c11_i5 = 0; c11_i5 < 8; c11_i5++) {
    _SFD_DATA_RANGE_CHECK((*c11_inputBound)[c11_i5], 7U);
  }

  _SFD_DATA_RANGE_CHECK(*c11_Fx, 8U);
  _SFD_DATA_RANGE_CHECK(*c11_Fy, 9U);
  _SFD_DATA_RANGE_CHECK(*c11_M, 10U);
  chartInstance->c11_sfEvent = CALL_EVENT;
  c11_chartstep_c11_BoatLibraryTest02(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_BoatLibraryTest02MachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c11_chartstep_c11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance)
{
  real_T c11_hoistedGlobal;
  real_T c11_b_hoistedGlobal;
  real_T c11_c_hoistedGlobal;
  real_T c11_d_hoistedGlobal;
  real_T c11_e_hoistedGlobal;
  real_T c11_f_hoistedGlobal;
  real_T c11_g_hoistedGlobal;
  int32_T c11_i6;
  real_T c11_u0[4];
  real_T c11_rx01;
  real_T c11_rx02;
  real_T c11_ry01;
  real_T c11_ry02;
  int32_T c11_i7;
  real_T c11_C0[7];
  int32_T c11_i8;
  real_T c11_inputBound[8];
  real_T c11_Fx;
  real_T c11_Fy;
  real_T c11_M;
  uint32_T c11_debug_family_var_map[27];
  real_T c11_rx1;
  real_T c11_rx2;
  real_T c11_ry1;
  real_T c11_ry2;
  real_T c11_C[7];
  real_T c11_Fxref;
  real_T c11_Fyref;
  real_T c11_Mref;
  real_T c11_gridNum;
  real_T c11_grid[32];
  real_T c11_u1[4];
  real_T c11_k;
  real_T c11_h[4];
  real_T c11_alpha;
  real_T c11_nargin = 10.0;
  real_T c11_nargout = 1.0;
  real_T c11_output[4];
  int32_T c11_i9;
  int32_T c11_i10;
  real_T c11_b_inputBound[8];
  real_T c11_dv1[32];
  int32_T c11_i11;
  int32_T c11_i12;
  real_T c11_b_grid[32];
  int32_T c11_i13;
  real_T c11_preinput[4];
  real_T c11_b_Fxref;
  real_T c11_b_Fyref;
  real_T c11_b_Mref;
  real_T c11_b_rx1;
  real_T c11_b_rx2;
  real_T c11_b_ry1;
  real_T c11_b_ry2;
  int32_T c11_i14;
  real_T c11_b_C[7];
  uint32_T c11_b_debug_family_var_map[22];
  real_T c11_ii;
  real_T c11_iii;
  real_T c11_v0;
  real_T c11_i;
  real_T c11_j;
  real_T c11_b_k;
  real_T c11_l;
  real_T c11_input[4];
  real_T c11_v1;
  real_T c11_b_nargin = 10.0;
  real_T c11_b_nargout = 1.0;
  int32_T c11_i15;
  int32_T c11_i16;
  real_T c11_c_grid[4];
  int32_T c11_i17;
  real_T c11_b_preinput[4];
  int32_T c11_i18;
  real_T c11_c_C[7];
  int32_T c11_b_i;
  int32_T c11_b_j;
  int32_T c11_c_k;
  int32_T c11_b_l;
  int32_T c11_i19;
  real_T c11_b_input[4];
  int32_T c11_i20;
  real_T c11_c_preinput[4];
  int32_T c11_i21;
  real_T c11_d_C[7];
  int32_T c11_i22;
  int32_T c11_d_k;
  int32_T c11_i23;
  real_T c11_c_input[4];
  int32_T c11_i24;
  real_T c11_d_preinput[4];
  real_T c11_c_Fxref;
  real_T c11_c_Fyref;
  real_T c11_c_Mref;
  real_T c11_c_rx1;
  real_T c11_c_rx2;
  real_T c11_c_ry1;
  real_T c11_c_ry2;
  int32_T c11_i25;
  real_T c11_e_C[7];
  uint32_T c11_c_debug_family_var_map[15];
  real_T c11_hsd[4];
  real_T c11_c_i;
  real_T c11_c_nargin = 10.0;
  real_T c11_c_nargout = 1.0;
  int32_T c11_i26;
  real_T c11_d_input[4];
  int32_T c11_i27;
  real_T c11_e_preinput[4];
  int32_T c11_i28;
  real_T c11_f_C[7];
  real_T c11_dv2[4];
  int32_T c11_i29;
  int32_T c11_i30;
  int32_T c11_d_i;
  int32_T c11_i31;
  real_T c11_e_input[4];
  int32_T c11_i32;
  real_T c11_f_preinput[4];
  int32_T c11_i33;
  real_T c11_b_h[4];
  real_T c11_d_Fxref;
  real_T c11_d_Fyref;
  real_T c11_d_Mref;
  real_T c11_d_rx1;
  real_T c11_d_rx2;
  real_T c11_d_ry1;
  real_T c11_d_ry2;
  int32_T c11_i34;
  real_T c11_g_C[7];
  uint32_T c11_d_debug_family_var_map[19];
  real_T c11_b_alpha;
  real_T c11_b_u0[4];
  real_T c11_b_u1[4];
  real_T c11_b_v0;
  real_T c11_b_v1;
  real_T c11_d_nargin = 11.0;
  real_T c11_d_nargout = 1.0;
  int32_T c11_i35;
  int32_T c11_i36;
  real_T c11_b[4];
  int32_T c11_i37;
  int32_T c11_i38;
  real_T c11_c_u0[4];
  int32_T c11_i39;
  real_T c11_g_preinput[4];
  int32_T c11_i40;
  real_T c11_h_C[7];
  int32_T c11_i41;
  real_T c11_c_u1[4];
  int32_T c11_i42;
  real_T c11_h_preinput[4];
  int32_T c11_i43;
  real_T c11_i_C[7];
  real_T c11_a;
  int32_T c11_i44;
  int32_T c11_i45;
  int32_T c11_i46;
  int32_T c11_i47;
  real_T c11_d_u1[4];
  int32_T c11_i48;
  real_T c11_i_preinput[4];
  int32_T c11_i49;
  real_T c11_j_C[7];
  real_T c11_b_a;
  int32_T c11_i50;
  int32_T c11_i51;
  int32_T c11_i52;
  int32_T c11_i53;
  real_T c11_e_u1[4];
  int32_T c11_i54;
  real_T c11_j_preinput[4];
  int32_T c11_i55;
  real_T c11_k_C[7];
  real_T c11_c_a;
  int32_T c11_i56;
  int32_T c11_i57;
  int32_T c11_i58;
  int32_T c11_i59;
  real_T c11_f_input[4];
  int32_T c11_i60;
  real_T c11_c_inputBound[8];
  uint32_T c11_e_debug_family_var_map[7];
  real_T c11_e_k[4];
  real_T c11_e_i;
  real_T c11_e_nargin = 2.0;
  real_T c11_e_nargout = 1.0;
  int32_T c11_i61;
  int32_T c11_f_i;
  real_T c11_g_input;
  int32_T c11_g_i;
  int32_T c11_i62;
  real_T c11_bound[2];
  uint32_T c11_f_debug_family_var_map[5];
  real_T c11_f_nargin = 2.0;
  real_T c11_f_nargout = 1.0;
  real_T c11_b_output;
  int32_T c11_i63;
  int32_T c11_i64;
  int32_T c11_i65;
  real_T *c11_b_rx01;
  real_T *c11_b_rx02;
  real_T *c11_b_ry01;
  real_T *c11_b_ry02;
  real_T *c11_b_Fx;
  real_T *c11_b_Fy;
  real_T *c11_b_M;
  real_T (*c11_c_output)[4];
  real_T (*c11_d_inputBound)[8];
  real_T (*c11_b_C0)[7];
  real_T (*c11_d_u0)[4];
  boolean_T exitg1;
  boolean_T exitg2;
  c11_b_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c11_b_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c11_b_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c11_d_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S, 6);
  c11_b_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
  c11_b_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c11_b_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c11_b_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c11_b_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c11_c_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c11_d_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 8U, chartInstance->c11_sfEvent);
  c11_hoistedGlobal = *c11_b_rx01;
  c11_b_hoistedGlobal = *c11_b_rx02;
  c11_c_hoistedGlobal = *c11_b_ry01;
  c11_d_hoistedGlobal = *c11_b_ry02;
  c11_e_hoistedGlobal = *c11_b_Fx;
  c11_f_hoistedGlobal = *c11_b_Fy;
  c11_g_hoistedGlobal = *c11_b_M;
  for (c11_i6 = 0; c11_i6 < 4; c11_i6++) {
    c11_u0[c11_i6] = (*c11_d_u0)[c11_i6];
  }

  c11_rx01 = c11_hoistedGlobal;
  c11_rx02 = c11_b_hoistedGlobal;
  c11_ry01 = c11_c_hoistedGlobal;
  c11_ry02 = c11_d_hoistedGlobal;
  for (c11_i7 = 0; c11_i7 < 7; c11_i7++) {
    c11_C0[c11_i7] = (*c11_b_C0)[c11_i7];
  }

  for (c11_i8 = 0; c11_i8 < 8; c11_i8++) {
    c11_inputBound[c11_i8] = (*c11_d_inputBound)[c11_i8];
  }

  c11_Fx = c11_e_hoistedGlobal;
  c11_Fy = c11_f_hoistedGlobal;
  c11_M = c11_g_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 27U, 27U, c11_debug_family_names,
    c11_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx1, 0U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx2, 1U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry1, 2U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry2, 3U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_C, 4U, c11_d_sf_marshallOut,
    c11_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fxref, 5U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fyref, 6U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Mref, 7U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_gridNum, 8U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_grid, 9U, c11_e_sf_marshallOut,
    c11_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_u1, 10U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_k, 11U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_h, 12U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_alpha, 13U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargin, 14U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargout, 15U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c11_u0, 16U, c11_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_rx01, 17U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_rx02, 18U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_ry01, 19U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_ry02, 20U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c11_C0, 21U, c11_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c11_inputBound, 22U, c11_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_Fx, 23U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_Fy, 24U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_M, 25U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_output, 26U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 3);
  c11_rx1 = c11_rx01;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 3);
  c11_rx2 = c11_rx02;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 4);
  c11_ry1 = c11_ry01;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 4);
  c11_ry2 = c11_ry02;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 6);
  for (c11_i9 = 0; c11_i9 < 7; c11_i9++) {
    c11_C[c11_i9] = c11_C0[c11_i9];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 8);
  c11_Fxref = c11_Fx;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 8);
  c11_Fyref = c11_Fy;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 8);
  c11_Mref = c11_M;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 10);
  c11_gridNum = 5.0;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 12);
  for (c11_i10 = 0; c11_i10 < 8; c11_i10++) {
    c11_b_inputBound[c11_i10] = c11_inputBound[c11_i10];
  }

  c11_boundGrid(chartInstance, c11_b_inputBound, c11_rx1, c11_ry1, c11_dv1);
  for (c11_i11 = 0; c11_i11 < 32; c11_i11++) {
    c11_grid[c11_i11] = c11_dv1[c11_i11];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 14);
  for (c11_i12 = 0; c11_i12 < 32; c11_i12++) {
    c11_b_grid[c11_i12] = c11_grid[c11_i12];
  }

  for (c11_i13 = 0; c11_i13 < 4; c11_i13++) {
    c11_preinput[c11_i13] = c11_u0[c11_i13];
  }

  c11_b_Fxref = c11_Fxref;
  c11_b_Fyref = c11_Fyref;
  c11_b_Mref = c11_Mref;
  c11_b_rx1 = c11_rx1;
  c11_b_rx2 = c11_rx2;
  c11_b_ry1 = c11_ry1;
  c11_b_ry2 = c11_ry2;
  for (c11_i14 = 0; c11_i14 < 7; c11_i14++) {
    c11_b_C[c11_i14] = c11_C[c11_i14];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 22U, 22U, c11_e_debug_family_names,
    c11_b_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_ii, 0U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_iii, 1U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_v0, 2U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_i, 3U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_j, 4U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_k, 5U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_l, 6U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_input, 7U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_v1, 8U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_nargin, 9U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_nargout, 10U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_b_grid, 11U, c11_e_sf_marshallOut,
    c11_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_preinput, 12U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_Fxref, 13U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_Fyref, 14U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_Mref, 15U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_rx1, 16U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_rx2, 17U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_ry1, 18U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_ry2, 19U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_b_C, 20U, c11_d_sf_marshallOut,
    c11_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_u1, 21U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  CV_EML_FCN(0, 1);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 31);
  c11_ii = 8.0;
  c11_iii = 4.0;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 32);
  c11_i15 = 0;
  for (c11_i16 = 0; c11_i16 < 4; c11_i16++) {
    c11_c_grid[c11_i16] = c11_b_grid[c11_i15];
    c11_i15 += 8;
  }

  for (c11_i17 = 0; c11_i17 < 4; c11_i17++) {
    c11_b_preinput[c11_i17] = c11_preinput[c11_i17];
  }

  for (c11_i18 = 0; c11_i18 < 7; c11_i18++) {
    c11_c_C[c11_i18] = c11_b_C[c11_i18];
  }

  c11_v0 = c11_costFunction(chartInstance, c11_c_grid, c11_b_preinput,
    c11_b_Fxref, c11_b_Fyref, c11_b_Mref, c11_b_rx1, c11_b_rx2, c11_b_ry1,
    c11_b_ry2, c11_c_C);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 34);
  c11_u1[0] = c11_b_grid[0];
  c11_u1[1] = c11_b_grid[8];
  c11_u1[2] = c11_b_grid[16];
  c11_u1[3] = c11_b_grid[24];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 36);
  c11_i = 1.0;
  c11_b_i = 0;
  while (c11_b_i < 8) {
    c11_i = 1.0 + (real_T)c11_b_i;
    CV_EML_FOR(0, 1, 1, 1);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 37);
    c11_j = 1.0;
    c11_b_j = 0;
    while (c11_b_j < 8) {
      c11_j = 1.0 + (real_T)c11_b_j;
      CV_EML_FOR(0, 1, 2, 1);
      _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 38);
      c11_b_k = 1.0;
      c11_c_k = 0;
      while (c11_c_k < 8) {
        c11_b_k = 1.0 + (real_T)c11_c_k;
        CV_EML_FOR(0, 1, 3, 1);
        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 39);
        c11_l = 1.0;
        c11_b_l = 0;
        while (c11_b_l < 8) {
          c11_l = 1.0 + (real_T)c11_b_l;
          CV_EML_FOR(0, 1, 4, 1);
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 40);
          c11_input[0] = c11_b_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK
            ("grid", (int32_T)_SFD_INTEGER_CHECK("i", c11_i), 1, 8, 1, 0) - 1];
          c11_input[1] = c11_b_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK
            ("grid", (int32_T)_SFD_INTEGER_CHECK("j", c11_j), 1, 8, 1, 0) + 7];
          c11_input[2] = c11_b_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK
            ("grid", (int32_T)_SFD_INTEGER_CHECK("k", c11_b_k), 1, 8, 1, 0) + 15];
          c11_input[3] = c11_b_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK
            ("grid", (int32_T)_SFD_INTEGER_CHECK("l", c11_l), 1, 8, 1, 0) + 23];
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 41);
          for (c11_i19 = 0; c11_i19 < 4; c11_i19++) {
            c11_b_input[c11_i19] = c11_input[c11_i19];
          }

          for (c11_i20 = 0; c11_i20 < 4; c11_i20++) {
            c11_c_preinput[c11_i20] = c11_preinput[c11_i20];
          }

          for (c11_i21 = 0; c11_i21 < 7; c11_i21++) {
            c11_d_C[c11_i21] = c11_b_C[c11_i21];
          }

          c11_v1 = c11_b_costFunction(chartInstance, c11_b_input, c11_c_preinput,
            c11_b_Fxref, c11_b_Fyref, c11_b_Mref, c11_b_rx1, c11_b_rx2,
            c11_b_ry1, c11_b_ry2, c11_d_C);
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 42);
          if (CV_EML_IF(0, 1, 0, c11_v0 > c11_v1)) {
            _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 43);
            c11_v0 = c11_v1;
            _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 44);
            for (c11_i22 = 0; c11_i22 < 4; c11_i22++) {
              c11_u1[c11_i22] = c11_input[c11_i22];
            }
          }

          c11_b_l++;
          _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
        }

        CV_EML_FOR(0, 1, 4, 0);
        c11_c_k++;
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      }

      CV_EML_FOR(0, 1, 3, 0);
      c11_b_j++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 2, 0);
    c11_b_i++;
    _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
  }

  CV_EML_FOR(0, 1, 1, 0);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -44);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 18);
  c11_k = 1.0;
  c11_d_k = 0;
  while (c11_d_k < 20) {
    c11_k = 1.0 + (real_T)c11_d_k;
    CV_EML_FOR(0, 1, 0, 1);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 19);
    for (c11_i23 = 0; c11_i23 < 4; c11_i23++) {
      c11_c_input[c11_i23] = c11_u1[c11_i23];
    }

    for (c11_i24 = 0; c11_i24 < 4; c11_i24++) {
      c11_d_preinput[c11_i24] = c11_u0[c11_i24];
    }

    c11_c_Fxref = c11_Fxref;
    c11_c_Fyref = c11_Fyref;
    c11_c_Mref = c11_Mref;
    c11_c_rx1 = c11_rx1;
    c11_c_rx2 = c11_rx2;
    c11_c_ry1 = c11_ry1;
    c11_c_ry2 = c11_ry2;
    for (c11_i25 = 0; c11_i25 < 7; c11_i25++) {
      c11_e_C[c11_i25] = c11_C[c11_i25];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 15U, 15U, c11_g_debug_family_names,
      c11_c_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_hsd, 0U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_i, 1U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_nargin, 2U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_nargout, 3U,
      c11_b_sf_marshallOut, c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_c_input, 4U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_d_preinput, 5U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_Fxref, 6U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_Fyref, 7U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_Mref, 8U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_rx1, 9U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_rx2, 10U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_ry1, 11U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_ry2, 12U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_e_C, 13U, c11_d_sf_marshallOut,
      c11_d_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_h, 14U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    CV_EML_FCN(0, 7);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 151U);
    for (c11_i26 = 0; c11_i26 < 4; c11_i26++) {
      c11_d_input[c11_i26] = c11_c_input[c11_i26];
    }

    for (c11_i27 = 0; c11_i27 < 4; c11_i27++) {
      c11_e_preinput[c11_i27] = c11_d_preinput[c11_i27];
    }

    for (c11_i28 = 0; c11_i28 < 7; c11_i28++) {
      c11_f_C[c11_i28] = c11_e_C[c11_i28];
    }

    c11_SteepestDescent(chartInstance, c11_d_input, c11_e_preinput, c11_c_Fxref,
                        c11_c_Fyref, c11_c_Mref, c11_c_rx1, c11_c_rx2, c11_c_ry1,
                        c11_c_ry2, c11_f_C, c11_dv2);
    for (c11_i29 = 0; c11_i29 < 4; c11_i29++) {
      c11_hsd[c11_i29] = c11_dv2[c11_i29];
    }

    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 153U);
    for (c11_i30 = 0; c11_i30 < 4; c11_i30++) {
      c11_h[c11_i30] = 0.0;
    }

    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 155U);
    c11_c_i = 1.0;
    c11_d_i = 0;
    while (c11_d_i < 4) {
      c11_c_i = 1.0 + (real_T)c11_d_i;
      CV_EML_FOR(0, 1, 6, 1);
      _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 156U);
      c11_h[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("output", (int32_T)
        _SFD_INTEGER_CHECK("i", c11_c_i), 1, 4, 1, 0) - 1] = c11_hsd[(int32_T)
        (real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("hsd", (int32_T)_SFD_INTEGER_CHECK(
        "i", c11_c_i), 1, 4, 1, 0) - 1];
      c11_d_i++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 6, 0);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -156);
    _SFD_SYMBOL_SCOPE_POP();
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 20);
    for (c11_i31 = 0; c11_i31 < 4; c11_i31++) {
      c11_e_input[c11_i31] = c11_u1[c11_i31];
    }

    for (c11_i32 = 0; c11_i32 < 4; c11_i32++) {
      c11_f_preinput[c11_i32] = c11_u0[c11_i32];
    }

    for (c11_i33 = 0; c11_i33 < 4; c11_i33++) {
      c11_b_h[c11_i33] = c11_h[c11_i33];
    }

    c11_d_Fxref = c11_Fxref;
    c11_d_Fyref = c11_Fyref;
    c11_d_Mref = c11_Mref;
    c11_d_rx1 = c11_rx1;
    c11_d_rx2 = c11_rx2;
    c11_d_ry1 = c11_ry1;
    c11_d_ry2 = c11_ry2;
    for (c11_i34 = 0; c11_i34 < 7; c11_i34++) {
      c11_g_C[c11_i34] = c11_C[c11_i34];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 19U, 19U, c11_h_debug_family_names,
      c11_d_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML(&c11_b_alpha, 0U, c11_b_sf_marshallOut);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_b_u0, 1U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_b_u1, 2U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_v0, 3U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_v1, 4U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_nargin, 5U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_nargout, 6U,
      c11_b_sf_marshallOut, c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_e_input, 7U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_f_preinput, 8U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_b_h, 9U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_Fxref, 10U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_Fyref, 11U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_Mref, 12U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_rx1, 13U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_rx2, 14U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_ry1, 15U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_d_ry2, 16U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_g_C, 17U, c11_d_sf_marshallOut,
      c11_d_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_alpha, 18U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    CV_EML_FCN(0, 5);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 94);
    c11_b_alpha = 1.0;
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 96);
    for (c11_i35 = 0; c11_i35 < 4; c11_i35++) {
      c11_b_u0[c11_i35] = c11_e_input[c11_i35];
    }

    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 97);
    for (c11_i36 = 0; c11_i36 < 4; c11_i36++) {
      c11_b[c11_i36] = c11_b_h[c11_i36];
    }

    for (c11_i37 = 0; c11_i37 < 4; c11_i37++) {
      c11_b_u1[c11_i37] = c11_e_input[c11_i37] + c11_b[c11_i37];
    }

    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 99);
    for (c11_i38 = 0; c11_i38 < 4; c11_i38++) {
      c11_c_u0[c11_i38] = c11_b_u0[c11_i38];
    }

    for (c11_i39 = 0; c11_i39 < 4; c11_i39++) {
      c11_g_preinput[c11_i39] = c11_f_preinput[c11_i39];
    }

    for (c11_i40 = 0; c11_i40 < 7; c11_i40++) {
      c11_h_C[c11_i40] = c11_g_C[c11_i40];
    }

    c11_b_v0 = c11_b_costFunction(chartInstance, c11_c_u0, c11_g_preinput,
      c11_d_Fxref, c11_d_Fyref, c11_d_Mref, c11_d_rx1, c11_d_rx2, c11_d_ry1,
      c11_d_ry2, c11_h_C);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 100);
    for (c11_i41 = 0; c11_i41 < 4; c11_i41++) {
      c11_c_u1[c11_i41] = c11_b_u1[c11_i41];
    }

    for (c11_i42 = 0; c11_i42 < 4; c11_i42++) {
      c11_h_preinput[c11_i42] = c11_f_preinput[c11_i42];
    }

    for (c11_i43 = 0; c11_i43 < 7; c11_i43++) {
      c11_i_C[c11_i43] = c11_g_C[c11_i43];
    }

    c11_b_v1 = c11_b_costFunction(chartInstance, c11_c_u1, c11_h_preinput,
      c11_d_Fxref, c11_d_Fyref, c11_d_Mref, c11_d_rx1, c11_d_rx2, c11_d_ry1,
      c11_d_ry2, c11_i_C);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 102);
    exitg2 = FALSE;
    while ((exitg2 == FALSE) && (c11_b_v1 <= c11_b_v0)) {
      if (c11_b_v1 != 0.0) {
        CV_EML_WHILE(0, 1, 0, TRUE);
        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 103);
        c11_b_alpha++;
        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 104);
        c11_a = c11_b_alpha;
        for (c11_i44 = 0; c11_i44 < 4; c11_i44++) {
          c11_b[c11_i44] = c11_b_h[c11_i44];
        }

        for (c11_i45 = 0; c11_i45 < 4; c11_i45++) {
          c11_b[c11_i45] *= c11_a;
        }

        for (c11_i46 = 0; c11_i46 < 4; c11_i46++) {
          c11_b_u1[c11_i46] = c11_e_input[c11_i46] + c11_b[c11_i46];
        }

        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 105);
        c11_b_v0 = c11_b_v1;
        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 106);
        for (c11_i47 = 0; c11_i47 < 4; c11_i47++) {
          c11_d_u1[c11_i47] = c11_b_u1[c11_i47];
        }

        for (c11_i48 = 0; c11_i48 < 4; c11_i48++) {
          c11_i_preinput[c11_i48] = c11_f_preinput[c11_i48];
        }

        for (c11_i49 = 0; c11_i49 < 7; c11_i49++) {
          c11_j_C[c11_i49] = c11_g_C[c11_i49];
        }

        c11_b_v1 = c11_b_costFunction(chartInstance, c11_d_u1, c11_i_preinput,
          c11_d_Fxref, c11_d_Fyref, c11_d_Mref, c11_d_rx1, c11_d_rx2, c11_d_ry1,
          c11_d_ry2, c11_j_C);
        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 102);
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      } else {
        exitg2 = TRUE;
      }
    }

    CV_EML_WHILE(0, 1, 0, FALSE);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 109);
    c11_b_v0 = c11_b_v1;
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 111);
    exitg1 = FALSE;
    while ((exitg1 == FALSE) && (c11_b_v1 <= c11_b_v0)) {
      if (c11_b_alpha >= 0.0) {
        if (c11_b_v1 != 0.0) {
          CV_EML_WHILE(0, 1, 1, TRUE);
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 112);
          c11_b_alpha -= 0.01;
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 113);
          c11_b_a = c11_b_alpha;
          for (c11_i50 = 0; c11_i50 < 4; c11_i50++) {
            c11_b[c11_i50] = c11_b_h[c11_i50];
          }

          for (c11_i51 = 0; c11_i51 < 4; c11_i51++) {
            c11_b[c11_i51] *= c11_b_a;
          }

          for (c11_i52 = 0; c11_i52 < 4; c11_i52++) {
            c11_b_u1[c11_i52] = c11_e_input[c11_i52] + c11_b[c11_i52];
          }

          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 114);
          c11_b_v0 = c11_b_v1;
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 115);
          for (c11_i53 = 0; c11_i53 < 4; c11_i53++) {
            c11_e_u1[c11_i53] = c11_b_u1[c11_i53];
          }

          for (c11_i54 = 0; c11_i54 < 4; c11_i54++) {
            c11_j_preinput[c11_i54] = c11_f_preinput[c11_i54];
          }

          for (c11_i55 = 0; c11_i55 < 7; c11_i55++) {
            c11_k_C[c11_i55] = c11_g_C[c11_i55];
          }

          c11_b_v1 = c11_b_costFunction(chartInstance, c11_e_u1, c11_j_preinput,
            c11_d_Fxref, c11_d_Fyref, c11_d_Mref, c11_d_rx1, c11_d_rx2,
            c11_d_ry1, c11_d_ry2, c11_k_C);
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 111);
          _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
        } else {
          exitg1 = TRUE;
        }
      } else {
        exitg1 = TRUE;
      }
    }

    CV_EML_WHILE(0, 1, 1, FALSE);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 118);
    c11_b_alpha += 0.01;
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 120);
    c11_alpha = c11_b_alpha;
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -120);
    _SFD_SYMBOL_SCOPE_POP();
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 21);
    c11_c_a = c11_alpha;
    for (c11_i56 = 0; c11_i56 < 4; c11_i56++) {
      c11_b[c11_i56] = c11_h[c11_i56];
    }

    for (c11_i57 = 0; c11_i57 < 4; c11_i57++) {
      c11_b[c11_i57] *= c11_c_a;
    }

    for (c11_i58 = 0; c11_i58 < 4; c11_i58++) {
      c11_u1[c11_i58] += c11_b[c11_i58];
    }

    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 22);
    for (c11_i59 = 0; c11_i59 < 4; c11_i59++) {
      c11_f_input[c11_i59] = c11_u1[c11_i59];
    }

    for (c11_i60 = 0; c11_i60 < 8; c11_i60++) {
      c11_c_inputBound[c11_i60] = c11_inputBound[c11_i60];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 7U, 7U, c11_j_debug_family_names,
      c11_e_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_e_k, 0U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_e_i, 1U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_e_nargin, 2U, c11_b_sf_marshallOut,
      c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_e_nargout, 3U,
      c11_b_sf_marshallOut, c11_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_f_input, 4U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_c_inputBound, 5U,
      c11_c_sf_marshallOut, c11_e_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_u1, 6U, c11_sf_marshallOut,
      c11_sf_marshallIn);
    CV_EML_FCN(0, 2);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 55);
    for (c11_i61 = 0; c11_i61 < 4; c11_i61++) {
      c11_e_k[c11_i61] = 0.0;
    }

    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 56);
    c11_e_i = 1.0;
    c11_f_i = 0;
    while (c11_f_i < 4) {
      c11_e_i = 1.0 + (real_T)c11_f_i;
      CV_EML_FOR(0, 1, 5, 1);
      _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 57);
      c11_g_input = c11_f_input[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
        "input", (int32_T)_SFD_INTEGER_CHECK("i", c11_e_i), 1, 4, 1, 0) - 1];
      c11_g_i = (int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("inputBound",
        (int32_T)_SFD_INTEGER_CHECK("i", c11_e_i), 1, 4, 1, 0) - 1;
      for (c11_i62 = 0; c11_i62 < 2; c11_i62++) {
        c11_bound[c11_i62] = c11_c_inputBound[c11_g_i + (c11_i62 << 2)];
      }

      _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 5U, 5U, c11_i_debug_family_names,
        c11_f_debug_family_var_map);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_f_nargin, 0U,
        c11_b_sf_marshallOut, c11_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_f_nargout, 1U,
        c11_b_sf_marshallOut, c11_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_g_input, 2U,
        c11_b_sf_marshallOut, c11_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_bound, 3U, c11_h_sf_marshallOut,
        c11_h_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_output, 4U,
        c11_b_sf_marshallOut, c11_b_sf_marshallIn);
      CV_EML_FCN(0, 4);
      _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 82);
      if (CV_EML_IF(0, 1, 1, c11_g_input < c11_bound[0])) {
        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 83);
        c11_b_output = c11_bound[0];
      } else {
        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 84);
        if (CV_EML_IF(0, 1, 2, c11_g_input > c11_bound[1])) {
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 85);
          c11_b_output = c11_bound[1];
        } else {
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 87);
          c11_b_output = c11_g_input;
        }
      }

      _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -87);
      _SFD_SYMBOL_SCOPE_POP();
      c11_e_k[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("k", (int32_T)
        _SFD_INTEGER_CHECK("i", c11_e_i), 1, 4, 1, 0) - 1] = c11_b_output;
      c11_f_i++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 5, 0);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 59);
    for (c11_i63 = 0; c11_i63 < 4; c11_i63++) {
      c11_u1[c11_i63] = c11_e_k[c11_i63];
    }

    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -59);
    _SFD_SYMBOL_SCOPE_POP();
    c11_d_k++;
    _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
  }

  CV_EML_FOR(0, 1, 0, 0);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 25);
  for (c11_i64 = 0; c11_i64 < 4; c11_i64++) {
    c11_output[c11_i64] = c11_u1[c11_i64];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -25);
  _SFD_SYMBOL_SCOPE_POP();
  for (c11_i65 = 0; c11_i65 < 4; c11_i65++) {
    (*c11_c_output)[c11_i65] = c11_output[c11_i65];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 8U, chartInstance->c11_sfEvent);
}

static void initSimStructsc11_BoatLibraryTest02
  (SFc11_BoatLibraryTest02InstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c11_machineNumber, uint32_T
  c11_chartNumber)
{
}

static const mxArray *c11_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i66;
  real_T c11_b_inData[4];
  int32_T c11_i67;
  real_T c11_u[4];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i66 = 0; c11_i66 < 4; c11_i66++) {
    c11_b_inData[c11_i66] = (*(real_T (*)[4])c11_inData)[c11_i66];
  }

  for (c11_i67 = 0; c11_i67 < 4; c11_i67++) {
    c11_u[c11_i67] = c11_b_inData[c11_i67];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_output, const char_T *c11_identifier,
  real_T c11_y[4])
{
  emlrtMsgIdentifier c11_thisId;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_output), &c11_thisId,
    c11_y);
  sf_mex_destroy(&c11_output);
}

static void c11_b_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[4])
{
  real_T c11_dv3[4];
  int32_T c11_i68;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv3, 1, 0, 0U, 1, 0U, 1, 4);
  for (c11_i68 = 0; c11_i68 < 4; c11_i68++) {
    c11_y[c11_i68] = c11_dv3[c11_i68];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_output;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[4];
  int32_T c11_i69;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_output = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_output), &c11_thisId,
    c11_y);
  sf_mex_destroy(&c11_output);
  for (c11_i69 = 0; c11_i69 < 4; c11_i69++) {
    (*(real_T (*)[4])c11_outData)[c11_i69] = c11_y[c11_i69];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static const mxArray *c11_b_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  real_T c11_u;
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  c11_u = *(real_T *)c11_inData;
  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", &c11_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static const mxArray *c11_c_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i70;
  int32_T c11_i71;
  int32_T c11_i72;
  real_T c11_b_inData[8];
  int32_T c11_i73;
  int32_T c11_i74;
  int32_T c11_i75;
  real_T c11_u[8];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  c11_i70 = 0;
  for (c11_i71 = 0; c11_i71 < 2; c11_i71++) {
    for (c11_i72 = 0; c11_i72 < 4; c11_i72++) {
      c11_b_inData[c11_i72 + c11_i70] = (*(real_T (*)[8])c11_inData)[c11_i72 +
        c11_i70];
    }

    c11_i70 += 4;
  }

  c11_i73 = 0;
  for (c11_i74 = 0; c11_i74 < 2; c11_i74++) {
    for (c11_i75 = 0; c11_i75 < 4; c11_i75++) {
      c11_u[c11_i75 + c11_i73] = c11_b_inData[c11_i75 + c11_i73];
    }

    c11_i73 += 4;
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 2, 4, 2), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static const mxArray *c11_d_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i76;
  real_T c11_b_inData[7];
  int32_T c11_i77;
  real_T c11_u[7];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i76 = 0; c11_i76 < 7; c11_i76++) {
    c11_b_inData[c11_i76] = (*(real_T (*)[7])c11_inData)[c11_i76];
  }

  for (c11_i77 = 0; c11_i77 < 7; c11_i77++) {
    c11_u[c11_i77] = c11_b_inData[c11_i77];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 1, 7), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static real_T c11_c_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId)
{
  real_T c11_y;
  real_T c11_d0;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), &c11_d0, 1, 0, 0U, 0, 0U, 0);
  c11_y = c11_d0;
  sf_mex_destroy(&c11_u);
  return c11_y;
}

static void c11_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_nargout;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_nargout = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_y = c11_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_nargout),
    &c11_thisId);
  sf_mex_destroy(&c11_nargout);
  *(real_T *)c11_outData = c11_y;
  sf_mex_destroy(&c11_mxArrayInData);
}

static const mxArray *c11_e_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i78;
  int32_T c11_i79;
  int32_T c11_i80;
  real_T c11_b_inData[32];
  int32_T c11_i81;
  int32_T c11_i82;
  int32_T c11_i83;
  real_T c11_u[32];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  c11_i78 = 0;
  for (c11_i79 = 0; c11_i79 < 4; c11_i79++) {
    for (c11_i80 = 0; c11_i80 < 8; c11_i80++) {
      c11_b_inData[c11_i80 + c11_i78] = (*(real_T (*)[32])c11_inData)[c11_i80 +
        c11_i78];
    }

    c11_i78 += 8;
  }

  c11_i81 = 0;
  for (c11_i82 = 0; c11_i82 < 4; c11_i82++) {
    for (c11_i83 = 0; c11_i83 < 8; c11_i83++) {
      c11_u[c11_i83 + c11_i81] = c11_b_inData[c11_i83 + c11_i81];
    }

    c11_i81 += 8;
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 2, 8, 4), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_d_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[32])
{
  real_T c11_dv4[32];
  int32_T c11_i84;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv4, 1, 0, 0U, 1, 0U, 2, 8,
                4);
  for (c11_i84 = 0; c11_i84 < 32; c11_i84++) {
    c11_y[c11_i84] = c11_dv4[c11_i84];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_grid;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[32];
  int32_T c11_i85;
  int32_T c11_i86;
  int32_T c11_i87;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_grid = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_grid), &c11_thisId, c11_y);
  sf_mex_destroy(&c11_grid);
  c11_i85 = 0;
  for (c11_i86 = 0; c11_i86 < 4; c11_i86++) {
    for (c11_i87 = 0; c11_i87 < 8; c11_i87++) {
      (*(real_T (*)[32])c11_outData)[c11_i87 + c11_i85] = c11_y[c11_i87 +
        c11_i85];
    }

    c11_i85 += 8;
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static void c11_e_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[7])
{
  real_T c11_dv5[7];
  int32_T c11_i88;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv5, 1, 0, 0U, 1, 0U, 1, 7);
  for (c11_i88 = 0; c11_i88 < 7; c11_i88++) {
    c11_y[c11_i88] = c11_dv5[c11_i88];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_C;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[7];
  int32_T c11_i89;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_C = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_C), &c11_thisId, c11_y);
  sf_mex_destroy(&c11_C);
  for (c11_i89 = 0; c11_i89 < 7; c11_i89++) {
    (*(real_T (*)[7])c11_outData)[c11_i89] = c11_y[c11_i89];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static void c11_f_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[8])
{
  real_T c11_dv6[8];
  int32_T c11_i90;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv6, 1, 0, 0U, 1, 0U, 2, 4,
                2);
  for (c11_i90 = 0; c11_i90 < 8; c11_i90++) {
    c11_y[c11_i90] = c11_dv6[c11_i90];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_inputBound;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[8];
  int32_T c11_i91;
  int32_T c11_i92;
  int32_T c11_i93;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_inputBound = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_inputBound), &c11_thisId,
    c11_y);
  sf_mex_destroy(&c11_inputBound);
  c11_i91 = 0;
  for (c11_i92 = 0; c11_i92 < 2; c11_i92++) {
    for (c11_i93 = 0; c11_i93 < 4; c11_i93++) {
      (*(real_T (*)[8])c11_outData)[c11_i93 + c11_i91] = c11_y[c11_i93 + c11_i91];
    }

    c11_i91 += 4;
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static const mxArray *c11_f_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i94;
  real_T c11_b_inData[5];
  int32_T c11_i95;
  real_T c11_u[5];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i94 = 0; c11_i94 < 5; c11_i94++) {
    c11_b_inData[c11_i94] = (*(real_T (*)[5])c11_inData)[c11_i94];
  }

  for (c11_i95 = 0; c11_i95 < 5; c11_i95++) {
    c11_u[c11_i95] = c11_b_inData[c11_i95];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 1, 5), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_g_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[5])
{
  real_T c11_dv7[5];
  int32_T c11_i96;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv7, 1, 0, 0U, 1, 0U, 1, 5);
  for (c11_i96 = 0; c11_i96 < 5; c11_i96++) {
    c11_y[c11_i96] = c11_dv7[c11_i96];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_f2;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[5];
  int32_T c11_i97;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_f2 = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_f2), &c11_thisId, c11_y);
  sf_mex_destroy(&c11_f2);
  for (c11_i97 = 0; c11_i97 < 5; c11_i97++) {
    (*(real_T (*)[5])c11_outData)[c11_i97] = c11_y[c11_i97];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static const mxArray *c11_g_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i98;
  real_T c11_b_inData[4];
  int32_T c11_i99;
  real_T c11_u[4];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i98 = 0; c11_i98 < 4; c11_i98++) {
    c11_b_inData[c11_i98] = (*(real_T (*)[4])c11_inData)[c11_i98];
  }

  for (c11_i99 = 0; c11_i99 < 4; c11_i99++) {
    c11_u[c11_i99] = c11_b_inData[c11_i99];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 2, 1, 4), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_h_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[4])
{
  real_T c11_dv8[4];
  int32_T c11_i100;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv8, 1, 0, 0U, 1, 0U, 2, 1,
                4);
  for (c11_i100 = 0; c11_i100 < 4; c11_i100++) {
    c11_y[c11_i100] = c11_dv8[c11_i100];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_input;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[4];
  int32_T c11_i101;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_input = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_input), &c11_thisId,
    c11_y);
  sf_mex_destroy(&c11_input);
  for (c11_i101 = 0; c11_i101 < 4; c11_i101++) {
    (*(real_T (*)[4])c11_outData)[c11_i101] = c11_y[c11_i101];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static const mxArray *c11_h_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i102;
  real_T c11_b_inData[2];
  int32_T c11_i103;
  real_T c11_u[2];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i102 = 0; c11_i102 < 2; c11_i102++) {
    c11_b_inData[c11_i102] = (*(real_T (*)[2])c11_inData)[c11_i102];
  }

  for (c11_i103 = 0; c11_i103 < 2; c11_i103++) {
    c11_u[c11_i103] = c11_b_inData[c11_i103];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 2, 1, 2), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_i_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[2])
{
  real_T c11_dv9[2];
  int32_T c11_i104;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv9, 1, 0, 0U, 1, 0U, 2, 1,
                2);
  for (c11_i104 = 0; c11_i104 < 2; c11_i104++) {
    c11_y[c11_i104] = c11_dv9[c11_i104];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_bound;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[2];
  int32_T c11_i105;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_bound = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_bound), &c11_thisId,
    c11_y);
  sf_mex_destroy(&c11_bound);
  for (c11_i105 = 0; c11_i105 < 2; c11_i105++) {
    (*(real_T (*)[2])c11_outData)[c11_i105] = c11_y[c11_i105];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

const mxArray *sf_c11_BoatLibraryTest02_get_eml_resolved_functions_info(void)
{
  const mxArray *c11_nameCaptureInfo = NULL;
  c11_nameCaptureInfo = NULL;
  sf_mex_assign(&c11_nameCaptureInfo, sf_mex_createstruct("structure", 2, 57, 1),
                FALSE);
  c11_info_helper(&c11_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c11_nameCaptureInfo);
  return c11_nameCaptureInfo;
}

static void c11_info_helper(const mxArray **c11_info)
{
  const mxArray *c11_rhs0 = NULL;
  const mxArray *c11_lhs0 = NULL;
  const mxArray *c11_rhs1 = NULL;
  const mxArray *c11_lhs1 = NULL;
  const mxArray *c11_rhs2 = NULL;
  const mxArray *c11_lhs2 = NULL;
  const mxArray *c11_rhs3 = NULL;
  const mxArray *c11_lhs3 = NULL;
  const mxArray *c11_rhs4 = NULL;
  const mxArray *c11_lhs4 = NULL;
  const mxArray *c11_rhs5 = NULL;
  const mxArray *c11_lhs5 = NULL;
  const mxArray *c11_rhs6 = NULL;
  const mxArray *c11_lhs6 = NULL;
  const mxArray *c11_rhs7 = NULL;
  const mxArray *c11_lhs7 = NULL;
  const mxArray *c11_rhs8 = NULL;
  const mxArray *c11_lhs8 = NULL;
  const mxArray *c11_rhs9 = NULL;
  const mxArray *c11_lhs9 = NULL;
  const mxArray *c11_rhs10 = NULL;
  const mxArray *c11_lhs10 = NULL;
  const mxArray *c11_rhs11 = NULL;
  const mxArray *c11_lhs11 = NULL;
  const mxArray *c11_rhs12 = NULL;
  const mxArray *c11_lhs12 = NULL;
  const mxArray *c11_rhs13 = NULL;
  const mxArray *c11_lhs13 = NULL;
  const mxArray *c11_rhs14 = NULL;
  const mxArray *c11_lhs14 = NULL;
  const mxArray *c11_rhs15 = NULL;
  const mxArray *c11_lhs15 = NULL;
  const mxArray *c11_rhs16 = NULL;
  const mxArray *c11_lhs16 = NULL;
  const mxArray *c11_rhs17 = NULL;
  const mxArray *c11_lhs17 = NULL;
  const mxArray *c11_rhs18 = NULL;
  const mxArray *c11_lhs18 = NULL;
  const mxArray *c11_rhs19 = NULL;
  const mxArray *c11_lhs19 = NULL;
  const mxArray *c11_rhs20 = NULL;
  const mxArray *c11_lhs20 = NULL;
  const mxArray *c11_rhs21 = NULL;
  const mxArray *c11_lhs21 = NULL;
  const mxArray *c11_rhs22 = NULL;
  const mxArray *c11_lhs22 = NULL;
  const mxArray *c11_rhs23 = NULL;
  const mxArray *c11_lhs23 = NULL;
  const mxArray *c11_rhs24 = NULL;
  const mxArray *c11_lhs24 = NULL;
  const mxArray *c11_rhs25 = NULL;
  const mxArray *c11_lhs25 = NULL;
  const mxArray *c11_rhs26 = NULL;
  const mxArray *c11_lhs26 = NULL;
  const mxArray *c11_rhs27 = NULL;
  const mxArray *c11_lhs27 = NULL;
  const mxArray *c11_rhs28 = NULL;
  const mxArray *c11_lhs28 = NULL;
  const mxArray *c11_rhs29 = NULL;
  const mxArray *c11_lhs29 = NULL;
  const mxArray *c11_rhs30 = NULL;
  const mxArray *c11_lhs30 = NULL;
  const mxArray *c11_rhs31 = NULL;
  const mxArray *c11_lhs31 = NULL;
  const mxArray *c11_rhs32 = NULL;
  const mxArray *c11_lhs32 = NULL;
  const mxArray *c11_rhs33 = NULL;
  const mxArray *c11_lhs33 = NULL;
  const mxArray *c11_rhs34 = NULL;
  const mxArray *c11_lhs34 = NULL;
  const mxArray *c11_rhs35 = NULL;
  const mxArray *c11_lhs35 = NULL;
  const mxArray *c11_rhs36 = NULL;
  const mxArray *c11_lhs36 = NULL;
  const mxArray *c11_rhs37 = NULL;
  const mxArray *c11_lhs37 = NULL;
  const mxArray *c11_rhs38 = NULL;
  const mxArray *c11_lhs38 = NULL;
  const mxArray *c11_rhs39 = NULL;
  const mxArray *c11_lhs39 = NULL;
  const mxArray *c11_rhs40 = NULL;
  const mxArray *c11_lhs40 = NULL;
  const mxArray *c11_rhs41 = NULL;
  const mxArray *c11_lhs41 = NULL;
  const mxArray *c11_rhs42 = NULL;
  const mxArray *c11_lhs42 = NULL;
  const mxArray *c11_rhs43 = NULL;
  const mxArray *c11_lhs43 = NULL;
  const mxArray *c11_rhs44 = NULL;
  const mxArray *c11_lhs44 = NULL;
  const mxArray *c11_rhs45 = NULL;
  const mxArray *c11_lhs45 = NULL;
  const mxArray *c11_rhs46 = NULL;
  const mxArray *c11_lhs46 = NULL;
  const mxArray *c11_rhs47 = NULL;
  const mxArray *c11_lhs47 = NULL;
  const mxArray *c11_rhs48 = NULL;
  const mxArray *c11_lhs48 = NULL;
  const mxArray *c11_rhs49 = NULL;
  const mxArray *c11_lhs49 = NULL;
  const mxArray *c11_rhs50 = NULL;
  const mxArray *c11_lhs50 = NULL;
  const mxArray *c11_rhs51 = NULL;
  const mxArray *c11_lhs51 = NULL;
  const mxArray *c11_rhs52 = NULL;
  const mxArray *c11_lhs52 = NULL;
  const mxArray *c11_rhs53 = NULL;
  const mxArray *c11_lhs53 = NULL;
  const mxArray *c11_rhs54 = NULL;
  const mxArray *c11_lhs54 = NULL;
  const mxArray *c11_rhs55 = NULL;
  const mxArray *c11_lhs55 = NULL;
  const mxArray *c11_rhs56 = NULL;
  const mxArray *c11_lhs56 = NULL;
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("linspace"), "name", "name",
                  0);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "resolved",
                  "resolved", 0);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731858U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c11_rhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs0), "rhs", "rhs",
                  0);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs0), "lhs", "lhs",
                  0);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 1);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 1);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 1);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c11_rhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs1), "rhs", "rhs",
                  1);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs1), "lhs", "lhs",
                  1);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 2);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 2);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 2);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c11_rhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs2), "rhs", "rhs",
                  2);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs2), "lhs", "lhs",
                  2);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 3);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_floor"), "name",
                  "name", 3);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m"),
                  "resolved", "resolved", 3);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840326U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c11_rhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs3), "rhs", "rhs",
                  3);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs3), "lhs", "lhs",
                  3);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 4);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 4);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 4);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1323192178U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c11_rhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs4), "rhs", "rhs",
                  4);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs4), "lhs", "lhs",
                  4);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 5);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("realmax"), "name", "name", 5);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmax.m"), "resolved",
                  "resolved", 5);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1307672842U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c11_rhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs5), "rhs", "rhs",
                  5);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs5), "lhs", "lhs",
                  5);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmax.m"), "context",
                  "context", 6);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_realmax"), "name",
                  "name", 6);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "resolved",
                  "resolved", 6);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1326749596U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c11_rhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs6), "rhs", "rhs",
                  6);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs6), "lhs", "lhs",
                  6);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "context",
                  "context", 7);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_float_model"), "name",
                  "name", 7);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_float_model.m"),
                  "resolved", "resolved", 7);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1326749596U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c11_rhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs7), "rhs", "rhs",
                  7);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs7), "lhs", "lhs",
                  7);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "context",
                  "context", 8);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mtimes"), "name", "name", 8);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 8);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c11_rhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs8), "rhs", "rhs",
                  8);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs8), "lhs", "lhs",
                  8);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m!common_checks"),
                  "context", "context", 9);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 9);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 9);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c11_rhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs9), "rhs", "rhs",
                  9);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs9), "lhs", "lhs",
                  9);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 10);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mrdivide"), "name", "name",
                  10);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 10);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c11_rhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "context",
                  "context", 11);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("rdivide"), "name", "name",
                  11);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "resolved",
                  "resolved", 11);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c11_rhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 12);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 12);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 12);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c11_rhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 13);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalexp_compatible"),
                  "name", "name", 13);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m"),
                  "resolved", "resolved", 13);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c11_rhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 14);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_div"), "name", "name",
                  14);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m"), "resolved",
                  "resolved", 14);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731866U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c11_rhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs14), "lhs", "lhs",
                  14);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 15);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mtimes"), "name", "name", 15);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 15);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 15);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 15);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 15);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 15);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 15);
  sf_mex_assign(&c11_rhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs15), "rhs", "rhs",
                  15);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs15), "lhs", "lhs",
                  15);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 16);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mrdivide"), "name", "name",
                  16);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 16);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 16);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 16);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 16);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 16);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 16);
  sf_mex_assign(&c11_rhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs16), "rhs", "rhs",
                  16);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs16), "lhs", "lhs",
                  16);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 17);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("atan"), "name", "name", 17);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 17);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/atan.m"), "resolved",
                  "resolved", 17);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1343851972U), "fileTimeLo",
                  "fileTimeLo", 17);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 17);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 17);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 17);
  sf_mex_assign(&c11_rhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs17), "rhs", "rhs",
                  17);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs17), "lhs", "lhs",
                  17);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/atan.m"), "context",
                  "context", 18);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_atan"), "name",
                  "name", 18);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 18);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_atan.m"),
                  "resolved", "resolved", 18);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840318U), "fileTimeLo",
                  "fileTimeLo", 18);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 18);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 18);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 18);
  sf_mex_assign(&c11_rhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs18), "rhs", "rhs",
                  18);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs18), "lhs", "lhs",
                  18);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 19);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mtimes"), "name", "name", 19);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 19);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 19);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 19);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 19);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 19);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 19);
  sf_mex_assign(&c11_rhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs19), "rhs", "rhs",
                  19);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs19), "lhs", "lhs",
                  19);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 20);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("sind"), "name", "name", 20);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 20);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "resolved",
                  "resolved", 20);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1343851986U), "fileTimeLo",
                  "fileTimeLo", 20);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 20);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 20);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 20);
  sf_mex_assign(&c11_rhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs20), "rhs", "rhs",
                  20);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs20), "lhs", "lhs",
                  20);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "context",
                  "context", 21);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 21);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 21);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 21);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 21);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 21);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 21);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 21);
  sf_mex_assign(&c11_rhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs21), "rhs", "rhs",
                  21);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs21), "lhs", "lhs",
                  21);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 22);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("isfinite"), "name", "name",
                  22);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 22);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "resolved",
                  "resolved", 22);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 22);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 22);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 22);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 22);
  sf_mex_assign(&c11_rhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs22), "rhs", "rhs",
                  22);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs22), "lhs", "lhs",
                  22);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 23);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 23);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 23);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 23);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 23);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 23);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 23);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 23);
  sf_mex_assign(&c11_rhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs23), "rhs", "rhs",
                  23);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs23), "lhs", "lhs",
                  23);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 24);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("isinf"), "name", "name", 24);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 24);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "resolved",
                  "resolved", 24);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 24);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 24);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 24);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 24);
  sf_mex_assign(&c11_rhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs24), "rhs", "rhs",
                  24);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs24), "lhs", "lhs",
                  24);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "context",
                  "context", 25);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 25);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 25);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 25);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 25);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 25);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 25);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 25);
  sf_mex_assign(&c11_rhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs25), "rhs", "rhs",
                  25);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs25), "lhs", "lhs",
                  25);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 26);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("isnan"), "name", "name", 26);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 26);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "resolved",
                  "resolved", 26);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731858U), "fileTimeLo",
                  "fileTimeLo", 26);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 26);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 26);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 26);
  sf_mex_assign(&c11_rhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs26), "rhs", "rhs",
                  26);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs26), "lhs", "lhs",
                  26);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "context",
                  "context", 27);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 27);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 27);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 27);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 27);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 27);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 27);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 27);
  sf_mex_assign(&c11_rhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs27), "rhs", "rhs",
                  27);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs27), "lhs", "lhs",
                  27);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 28);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_guarded_nan"), "name",
                  "name", 28);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 28);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "resolved", "resolved", 28);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840376U), "fileTimeLo",
                  "fileTimeLo", 28);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 28);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 28);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 28);
  sf_mex_assign(&c11_rhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs28), "rhs", "rhs",
                  28);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs28), "lhs", "lhs",
                  28);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "context", "context", 29);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_is_float_class"), "name",
                  "name", 29);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 29);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m"),
                  "resolved", "resolved", 29);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840382U), "fileTimeLo",
                  "fileTimeLo", 29);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 29);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 29);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 29);
  sf_mex_assign(&c11_rhs29, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs29, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs29), "rhs", "rhs",
                  29);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs29), "lhs", "lhs",
                  29);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 30);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_rem90"), "name",
                  "name", 30);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 30);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "resolved", "resolved", 30);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 30);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 30);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 30);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 30);
  sf_mex_assign(&c11_rhs30, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs30, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs30), "rhs", "rhs",
                  30);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs30), "lhs", "lhs",
                  30);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 31);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mrdivide"), "name", "name",
                  31);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 31);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 31);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 31);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 31);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 31);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 31);
  sf_mex_assign(&c11_rhs31, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs31, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs31), "rhs", "rhs",
                  31);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs31), "lhs", "lhs",
                  31);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 32);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("rem"), "name", "name", 32);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 32);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "resolved",
                  "resolved", 32);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731854U), "fileTimeLo",
                  "fileTimeLo", 32);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 32);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 32);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 32);
  sf_mex_assign(&c11_rhs32, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs32, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs32), "rhs", "rhs",
                  32);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs32), "lhs", "lhs",
                  32);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 33);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 33);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 33);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 33);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 33);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 33);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 33);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 33);
  sf_mex_assign(&c11_rhs33, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs33, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs33), "rhs", "rhs",
                  33);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs33), "lhs", "lhs",
                  33);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 34);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 34);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 34);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 34);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 34);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 34);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 34);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 34);
  sf_mex_assign(&c11_rhs34, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs34, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs34), "rhs", "rhs",
                  34);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs34), "lhs", "lhs",
                  34);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 35);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 35);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 35);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 35);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1358207740U), "fileTimeLo",
                  "fileTimeLo", 35);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 35);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 35);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 35);
  sf_mex_assign(&c11_rhs35, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs35, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs35), "rhs", "rhs",
                  35);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs35), "lhs", "lhs",
                  35);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 36);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("abs"), "name", "name", 36);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 36);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "resolved",
                  "resolved", 36);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731852U), "fileTimeLo",
                  "fileTimeLo", 36);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 36);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 36);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 36);
  sf_mex_assign(&c11_rhs36, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs36, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs36), "rhs", "rhs",
                  36);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs36), "lhs", "lhs",
                  36);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 37);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 37);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 37);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 37);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 37);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 37);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 37);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 37);
  sf_mex_assign(&c11_rhs37, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs37, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs37), "rhs", "rhs",
                  37);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs37), "lhs", "lhs",
                  37);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 38);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_abs"), "name",
                  "name", 38);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 38);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m"),
                  "resolved", "resolved", 38);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840312U), "fileTimeLo",
                  "fileTimeLo", 38);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 38);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 38);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 38);
  sf_mex_assign(&c11_rhs38, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs38, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs38), "rhs", "rhs",
                  38);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs38), "lhs", "lhs",
                  38);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 39);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mtimes"), "name", "name", 39);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 39);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 39);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 39);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 39);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 39);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 39);
  sf_mex_assign(&c11_rhs39, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs39, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs39), "rhs", "rhs",
                  39);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs39), "lhs", "lhs",
                  39);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 40);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("cosd"), "name", "name", 40);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 40);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "resolved",
                  "resolved", 40);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1343851972U), "fileTimeLo",
                  "fileTimeLo", 40);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 40);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 40);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 40);
  sf_mex_assign(&c11_rhs40, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs40, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs40), "rhs", "rhs",
                  40);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs40), "lhs", "lhs",
                  40);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "context",
                  "context", 41);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 41);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 41);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 41);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 41);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 41);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 41);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 41);
  sf_mex_assign(&c11_rhs41, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs41, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs41), "rhs", "rhs",
                  41);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs41), "lhs", "lhs",
                  41);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 42);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mpower"), "name", "name", 42);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 42);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "resolved",
                  "resolved", 42);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 42);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 42);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 42);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 42);
  sf_mex_assign(&c11_rhs42, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs42, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs42), "rhs", "rhs",
                  42);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs42), "lhs", "lhs",
                  42);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 43);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 43);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 43);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 43);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 43);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 43);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 43);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 43);
  sf_mex_assign(&c11_rhs43, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs43, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs43), "rhs", "rhs",
                  43);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs43), "lhs", "lhs",
                  43);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 44);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("ismatrix"), "name", "name",
                  44);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 44);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/ismatrix.m"), "resolved",
                  "resolved", 44);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1331326458U), "fileTimeLo",
                  "fileTimeLo", 44);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 44);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 44);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 44);
  sf_mex_assign(&c11_rhs44, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs44, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs44), "rhs", "rhs",
                  44);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs44), "lhs", "lhs",
                  44);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 45);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("power"), "name", "name", 45);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 45);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m"), "resolved",
                  "resolved", 45);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 45);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 45);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 45);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 45);
  sf_mex_assign(&c11_rhs45, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs45, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs45), "rhs", "rhs",
                  45);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs45), "lhs", "lhs",
                  45);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m"), "context",
                  "context", 46);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 46);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 46);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 46);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 46);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 46);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 46);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 46);
  sf_mex_assign(&c11_rhs46, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs46, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs46), "rhs", "rhs",
                  46);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs46), "lhs", "lhs",
                  46);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 47);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 47);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 47);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 47);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 47);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 47);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 47);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 47);
  sf_mex_assign(&c11_rhs47, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs47, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs47), "rhs", "rhs",
                  47);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs47), "lhs", "lhs",
                  47);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 48);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 48);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 48);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 48);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1358207740U), "fileTimeLo",
                  "fileTimeLo", 48);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 48);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 48);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 48);
  sf_mex_assign(&c11_rhs48, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs48, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs48), "rhs", "rhs",
                  48);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs48), "lhs", "lhs",
                  48);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 49);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("floor"), "name", "name", 49);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 49);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "resolved",
                  "resolved", 49);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731854U), "fileTimeLo",
                  "fileTimeLo", 49);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 49);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 49);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 49);
  sf_mex_assign(&c11_rhs49, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs49, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs49), "rhs", "rhs",
                  49);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs49), "lhs", "lhs",
                  49);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "context",
                  "context", 50);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 50);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 50);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 50);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 50);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 50);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 50);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 50);
  sf_mex_assign(&c11_rhs50, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs50, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs50), "rhs", "rhs",
                  50);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs50), "lhs", "lhs",
                  50);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "context",
                  "context", 51);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_floor"), "name",
                  "name", 51);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 51);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m"),
                  "resolved", "resolved", 51);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840326U), "fileTimeLo",
                  "fileTimeLo", 51);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 51);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 51);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 51);
  sf_mex_assign(&c11_rhs51, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs51, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs51), "rhs", "rhs",
                  51);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs51), "lhs", "lhs",
                  51);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!scalar_float_power"),
                  "context", "context", 52);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 52);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 52);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 52);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 52);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 52);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 52);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 52);
  sf_mex_assign(&c11_rhs52, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs52, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs52), "rhs", "rhs",
                  52);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs52), "lhs", "lhs",
                  52);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!scalar_float_power"),
                  "context", "context", 53);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("mtimes"), "name", "name", 53);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 53);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 53);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 53);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 53);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 53);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 53);
  sf_mex_assign(&c11_rhs53, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs53, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs53), "rhs", "rhs",
                  53);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs53), "lhs", "lhs",
                  53);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 54);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("sin"), "name", "name", 54);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 54);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m"), "resolved",
                  "resolved", 54);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1343851986U), "fileTimeLo",
                  "fileTimeLo", 54);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 54);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 54);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 54);
  sf_mex_assign(&c11_rhs54, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs54, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs54), "rhs", "rhs",
                  54);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs54), "lhs", "lhs",
                  54);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m"), "context",
                  "context", 55);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("eml_scalar_sin"), "name",
                  "name", 55);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 55);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sin.m"),
                  "resolved", "resolved", 55);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1286840336U), "fileTimeLo",
                  "fileTimeLo", 55);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 55);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 55);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 55);
  sf_mex_assign(&c11_rhs55, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs55, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs55), "rhs", "rhs",
                  55);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs55), "lhs", "lhs",
                  55);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(""), "context", "context", 56);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("length"), "name", "name", 56);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 56);
  sf_mex_addfield(*c11_info, c11_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/length.m"), "resolved",
                  "resolved", 56);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(1303167806U), "fileTimeLo",
                  "fileTimeLo", 56);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 56);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 56);
  sf_mex_addfield(*c11_info, c11_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 56);
  sf_mex_assign(&c11_rhs56, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c11_lhs56, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_rhs56), "rhs", "rhs",
                  56);
  sf_mex_addfield(*c11_info, sf_mex_duplicatearraysafe(&c11_lhs56), "lhs", "lhs",
                  56);
  sf_mex_destroy(&c11_rhs0);
  sf_mex_destroy(&c11_lhs0);
  sf_mex_destroy(&c11_rhs1);
  sf_mex_destroy(&c11_lhs1);
  sf_mex_destroy(&c11_rhs2);
  sf_mex_destroy(&c11_lhs2);
  sf_mex_destroy(&c11_rhs3);
  sf_mex_destroy(&c11_lhs3);
  sf_mex_destroy(&c11_rhs4);
  sf_mex_destroy(&c11_lhs4);
  sf_mex_destroy(&c11_rhs5);
  sf_mex_destroy(&c11_lhs5);
  sf_mex_destroy(&c11_rhs6);
  sf_mex_destroy(&c11_lhs6);
  sf_mex_destroy(&c11_rhs7);
  sf_mex_destroy(&c11_lhs7);
  sf_mex_destroy(&c11_rhs8);
  sf_mex_destroy(&c11_lhs8);
  sf_mex_destroy(&c11_rhs9);
  sf_mex_destroy(&c11_lhs9);
  sf_mex_destroy(&c11_rhs10);
  sf_mex_destroy(&c11_lhs10);
  sf_mex_destroy(&c11_rhs11);
  sf_mex_destroy(&c11_lhs11);
  sf_mex_destroy(&c11_rhs12);
  sf_mex_destroy(&c11_lhs12);
  sf_mex_destroy(&c11_rhs13);
  sf_mex_destroy(&c11_lhs13);
  sf_mex_destroy(&c11_rhs14);
  sf_mex_destroy(&c11_lhs14);
  sf_mex_destroy(&c11_rhs15);
  sf_mex_destroy(&c11_lhs15);
  sf_mex_destroy(&c11_rhs16);
  sf_mex_destroy(&c11_lhs16);
  sf_mex_destroy(&c11_rhs17);
  sf_mex_destroy(&c11_lhs17);
  sf_mex_destroy(&c11_rhs18);
  sf_mex_destroy(&c11_lhs18);
  sf_mex_destroy(&c11_rhs19);
  sf_mex_destroy(&c11_lhs19);
  sf_mex_destroy(&c11_rhs20);
  sf_mex_destroy(&c11_lhs20);
  sf_mex_destroy(&c11_rhs21);
  sf_mex_destroy(&c11_lhs21);
  sf_mex_destroy(&c11_rhs22);
  sf_mex_destroy(&c11_lhs22);
  sf_mex_destroy(&c11_rhs23);
  sf_mex_destroy(&c11_lhs23);
  sf_mex_destroy(&c11_rhs24);
  sf_mex_destroy(&c11_lhs24);
  sf_mex_destroy(&c11_rhs25);
  sf_mex_destroy(&c11_lhs25);
  sf_mex_destroy(&c11_rhs26);
  sf_mex_destroy(&c11_lhs26);
  sf_mex_destroy(&c11_rhs27);
  sf_mex_destroy(&c11_lhs27);
  sf_mex_destroy(&c11_rhs28);
  sf_mex_destroy(&c11_lhs28);
  sf_mex_destroy(&c11_rhs29);
  sf_mex_destroy(&c11_lhs29);
  sf_mex_destroy(&c11_rhs30);
  sf_mex_destroy(&c11_lhs30);
  sf_mex_destroy(&c11_rhs31);
  sf_mex_destroy(&c11_lhs31);
  sf_mex_destroy(&c11_rhs32);
  sf_mex_destroy(&c11_lhs32);
  sf_mex_destroy(&c11_rhs33);
  sf_mex_destroy(&c11_lhs33);
  sf_mex_destroy(&c11_rhs34);
  sf_mex_destroy(&c11_lhs34);
  sf_mex_destroy(&c11_rhs35);
  sf_mex_destroy(&c11_lhs35);
  sf_mex_destroy(&c11_rhs36);
  sf_mex_destroy(&c11_lhs36);
  sf_mex_destroy(&c11_rhs37);
  sf_mex_destroy(&c11_lhs37);
  sf_mex_destroy(&c11_rhs38);
  sf_mex_destroy(&c11_lhs38);
  sf_mex_destroy(&c11_rhs39);
  sf_mex_destroy(&c11_lhs39);
  sf_mex_destroy(&c11_rhs40);
  sf_mex_destroy(&c11_lhs40);
  sf_mex_destroy(&c11_rhs41);
  sf_mex_destroy(&c11_lhs41);
  sf_mex_destroy(&c11_rhs42);
  sf_mex_destroy(&c11_lhs42);
  sf_mex_destroy(&c11_rhs43);
  sf_mex_destroy(&c11_lhs43);
  sf_mex_destroy(&c11_rhs44);
  sf_mex_destroy(&c11_lhs44);
  sf_mex_destroy(&c11_rhs45);
  sf_mex_destroy(&c11_lhs45);
  sf_mex_destroy(&c11_rhs46);
  sf_mex_destroy(&c11_lhs46);
  sf_mex_destroy(&c11_rhs47);
  sf_mex_destroy(&c11_lhs47);
  sf_mex_destroy(&c11_rhs48);
  sf_mex_destroy(&c11_lhs48);
  sf_mex_destroy(&c11_rhs49);
  sf_mex_destroy(&c11_lhs49);
  sf_mex_destroy(&c11_rhs50);
  sf_mex_destroy(&c11_lhs50);
  sf_mex_destroy(&c11_rhs51);
  sf_mex_destroy(&c11_lhs51);
  sf_mex_destroy(&c11_rhs52);
  sf_mex_destroy(&c11_lhs52);
  sf_mex_destroy(&c11_rhs53);
  sf_mex_destroy(&c11_lhs53);
  sf_mex_destroy(&c11_rhs54);
  sf_mex_destroy(&c11_lhs54);
  sf_mex_destroy(&c11_rhs55);
  sf_mex_destroy(&c11_lhs55);
  sf_mex_destroy(&c11_rhs56);
  sf_mex_destroy(&c11_lhs56);
}

static const mxArray *c11_emlrt_marshallOut(char * c11_u)
{
  const mxArray *c11_y = NULL;
  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 15, 0U, 0U, 0U, 2, 1, strlen
    (c11_u)), FALSE);
  return c11_y;
}

static const mxArray *c11_b_emlrt_marshallOut(uint32_T c11_u)
{
  const mxArray *c11_y = NULL;
  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", &c11_u, 7, 0U, 0U, 0U, 0), FALSE);
  return c11_y;
}

static void c11_eml_scalar_eg(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
}

static void c11_boundGrid(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_inputBound[8], real_T c11_rx1, real_T c11_ry1, real_T c11_output[32])
{
  uint32_T c11_debug_family_var_map[13];
  real_T c11_p;
  real_T c11_theta1[5];
  real_T c11_f1[5];
  real_T c11_theta2[5];
  real_T c11_f2[5];
  real_T c11_i;
  real_T c11_gridNum;
  real_T c11_nargin = 4.0;
  real_T c11_nargout = 1.0;
  real_T c11_dv10[5];
  int32_T c11_i106;
  int32_T c11_i107;
  int32_T c11_i108;
  int32_T c11_i109;
  real_T c11_A;
  real_T c11_B;
  real_T c11_x;
  real_T c11_y;
  real_T c11_b_x;
  real_T c11_b_y;
  real_T c11_c_y;
  real_T c11_c_x;
  real_T c11_d_x;
  real_T c11_a;
  real_T c11_d_y;
  real_T c11_b_A;
  real_T c11_e_x;
  real_T c11_f_x;
  int32_T c11_i110;
  int32_T c11_i111;
  int32_T c11_i112;
  int32_T c11_i113;
  int32_T c11_i114;
  int32_T c11_i115;
  static real_T c11_dv11[4] = { 0.0, 1.0, 0.0, 1.0 };

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 13U, 13U, c11_b_debug_family_names,
    c11_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_p, 0U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_theta1, 1U, c11_f_sf_marshallOut,
    c11_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_f1, 2U, c11_f_sf_marshallOut,
    c11_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_theta2, 3U, c11_f_sf_marshallOut,
    c11_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_f2, 4U, c11_f_sf_marshallOut,
    c11_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_i, 5U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_gridNum, 6U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargin, 7U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargout, 8U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_inputBound, 9U, c11_c_sf_marshallOut,
    c11_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx1, 10U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry1, 11U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_output, 12U, c11_e_sf_marshallOut,
    c11_c_sf_marshallIn);
  c11_gridNum = 5.0;
  CV_EML_FCN(0, 3);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 64);
  c11_p = 5.0;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 66);
  c11_linspace(chartInstance, c11_inputBound[0], c11_inputBound[4], c11_dv10);
  for (c11_i106 = 0; c11_i106 < 5; c11_i106++) {
    c11_theta1[c11_i106] = c11_dv10[c11_i106];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 67);
  c11_linspace(chartInstance, c11_inputBound[1], c11_inputBound[5], c11_dv10);
  for (c11_i107 = 0; c11_i107 < 5; c11_i107++) {
    c11_f1[c11_i107] = c11_dv10[c11_i107];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 68);
  c11_linspace(chartInstance, c11_inputBound[2], c11_inputBound[6], c11_dv10);
  for (c11_i108 = 0; c11_i108 < 5; c11_i108++) {
    c11_theta2[c11_i108] = c11_dv10[c11_i108];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 69);
  c11_linspace(chartInstance, c11_inputBound[3], c11_inputBound[7], c11_dv10);
  for (c11_i109 = 0; c11_i109 < 5; c11_i109++) {
    c11_f2[c11_i109] = c11_dv10[c11_i109];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 71);
  c11_A = c11_rx1;
  c11_B = c11_ry1;
  c11_x = c11_A;
  c11_y = c11_B;
  c11_b_x = c11_x;
  c11_b_y = c11_y;
  c11_c_y = c11_b_x / c11_b_y;
  c11_c_x = c11_c_y;
  c11_d_x = c11_c_x;
  c11_d_x = muDoubleScalarAtan(c11_d_x);
  c11_a = c11_d_x;
  c11_d_y = c11_a * 180.0;
  c11_b_A = c11_d_y;
  c11_e_x = c11_b_A;
  c11_f_x = c11_e_x;
  c11_i = c11_f_x / 3.1415926535897931;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 73);
  for (c11_i110 = 0; c11_i110 < 5; c11_i110++) {
    c11_output[c11_i110] = c11_theta1[c11_i110];
  }

  for (c11_i111 = 0; c11_i111 < 5; c11_i111++) {
    c11_output[c11_i111 + 8] = c11_f1[c11_i111];
  }

  for (c11_i112 = 0; c11_i112 < 5; c11_i112++) {
    c11_output[c11_i112 + 16] = c11_theta2[c11_i112];
  }

  for (c11_i113 = 0; c11_i113 < 5; c11_i113++) {
    c11_output[c11_i113 + 24] = c11_f2[c11_i113];
  }

  c11_i114 = 0;
  for (c11_i115 = 0; c11_i115 < 4; c11_i115++) {
    c11_output[c11_i114 + 5] = c11_dv11[c11_i115];
    c11_i114 += 8;
  }

  c11_output[6] = -c11_i;
  c11_output[14] = -1.0;
  c11_output[22] = -c11_i;
  c11_output[30] = -1.0;
  c11_output[7] = c11_i;
  c11_output[15] = 0.0;
  c11_output[23] = c11_i;
  c11_output[31] = 0.0;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -73);
  _SFD_SYMBOL_SCOPE_POP();
}

static void c11_linspace(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_d1, real_T c11_d2, real_T c11_y[5])
{
  real_T c11_A;
  real_T c11_x;
  real_T c11_b_x;
  real_T c11_delta1;
  real_T c11_b_A;
  real_T c11_c_x;
  real_T c11_d_x;
  real_T c11_delta2;
  int32_T c11_k;
  real_T c11_b_k;
  real_T c11_a;
  real_T c11_b;
  real_T c11_b_y;
  real_T c11_b_a;
  real_T c11_b_b;
  real_T c11_c_y;
  real_T c11_c_A;
  real_T c11_e_x;
  real_T c11_f_x;
  int32_T c11_c_k;
  real_T c11_c_a;
  real_T c11_c_b;
  real_T c11_d_y;
  boolean_T guard1 = FALSE;
  boolean_T guard2 = FALSE;
  boolean_T guard3 = FALSE;
  c11_eml_scalar_eg(chartInstance);
  c11_y[4] = c11_d2;
  c11_y[0] = c11_d1;
  guard1 = FALSE;
  guard2 = FALSE;
  guard3 = FALSE;
  if (c11_d1 < 0.0 != c11_d2 < 0.0) {
    if (muDoubleScalarAbs(c11_d1) > 8.9884656743115785E+307) {
      guard2 = TRUE;
    } else {
      guard3 = TRUE;
    }
  } else {
    guard3 = TRUE;
  }

  if (guard3 == TRUE) {
    if (muDoubleScalarAbs(c11_d2) > 8.9884656743115785E+307) {
      guard2 = TRUE;
    } else if (muDoubleScalarAbs(0.0) > 8.9884656743115785E+307) {
      guard1 = TRUE;
    } else {
      c11_c_A = c11_d2 - c11_d1;
      c11_e_x = c11_c_A;
      c11_f_x = c11_e_x;
      c11_delta1 = c11_f_x / 4.0;
      for (c11_c_k = 0; c11_c_k < 3; c11_c_k++) {
        c11_b_k = 1.0 + (real_T)c11_c_k;
        c11_c_a = c11_b_k;
        c11_c_b = c11_delta1;
        c11_d_y = c11_c_a * c11_c_b;
        c11_y[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
          _SFD_INTEGER_CHECK("", c11_b_k + 1.0), 1, 5, 1, 0) - 1] = c11_d1 +
          c11_d_y;
      }
    }
  }

  if (guard2 == TRUE) {
    guard1 = TRUE;
  }

  if (guard1 == TRUE) {
    c11_A = c11_d1;
    c11_x = c11_A;
    c11_b_x = c11_x;
    c11_delta1 = c11_b_x / 4.0;
    c11_b_A = c11_d2;
    c11_c_x = c11_b_A;
    c11_d_x = c11_c_x;
    c11_delta2 = c11_d_x / 4.0;
    for (c11_k = 0; c11_k < 3; c11_k++) {
      c11_b_k = 1.0 + (real_T)c11_k;
      c11_a = c11_delta2;
      c11_b = c11_b_k;
      c11_b_y = c11_a * c11_b;
      c11_b_a = c11_delta1;
      c11_b_b = c11_b_k;
      c11_c_y = c11_b_a * c11_b_b;
      c11_y[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
        _SFD_INTEGER_CHECK("", c11_b_k + 1.0), 1, 5, 1, 0) - 1] = (c11_d1 +
        c11_b_y) - c11_c_y;
    }
  }
}

static real_T c11_costFunction(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c11_input[4], real_T c11_preinput[4], real_T c11_Fxref,
  real_T c11_Fyref, real_T c11_Mref, real_T c11_rx1, real_T c11_rx2, real_T
  c11_ry1, real_T c11_ry2, real_T c11_C[7])
{
  real_T c11_output;
  uint32_T c11_debug_family_var_map[22];
  real_T c11_theta1;
  real_T c11_f1;
  real_T c11_theta2;
  real_T c11_f2;
  real_T c11_pretheta1;
  real_T c11_pretheta2;
  real_T c11_Fx;
  real_T c11_Fy;
  real_T c11_M;
  real_T c11_nargin = 10.0;
  real_T c11_nargout = 1.0;
  real_T c11_a;
  real_T c11_b;
  real_T c11_y;
  real_T c11_b_a;
  real_T c11_b_b;
  real_T c11_b_y;
  real_T c11_c_a;
  real_T c11_c_b;
  real_T c11_c_y;
  real_T c11_d_a;
  real_T c11_d_b;
  real_T c11_d_y;
  real_T c11_e_a;
  real_T c11_e_b;
  real_T c11_e_y;
  real_T c11_f_a;
  real_T c11_f_b;
  real_T c11_f_y;
  real_T c11_g_a;
  real_T c11_g_b;
  real_T c11_g_y;
  real_T c11_h_a;
  real_T c11_h_b;
  real_T c11_h_y;
  real_T c11_i_a;
  real_T c11_i_b;
  real_T c11_i_y;
  real_T c11_j_a;
  real_T c11_j_b;
  real_T c11_j_y;
  real_T c11_k_a;
  real_T c11_k_b;
  real_T c11_k_y;
  real_T c11_l_a;
  real_T c11_l_b;
  real_T c11_l_y;
  real_T c11_m_a;
  real_T c11_m_b;
  real_T c11_m_y;
  real_T c11_n_a;
  real_T c11_n_b;
  real_T c11_n_y;
  real_T c11_o_a;
  real_T c11_o_b;
  real_T c11_o_y;
  real_T c11_p_a;
  real_T c11_p_b;
  real_T c11_p_y;
  real_T c11_q_a;
  real_T c11_q_b;
  real_T c11_q_y;
  real_T c11_r_a;
  real_T c11_r_b;
  real_T c11_r_y;
  real_T c11_s_a;
  real_T c11_s_b;
  real_T c11_s_y;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 22U, 22U, c11_c_debug_family_names,
    c11_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_theta1, 0U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_f1, 1U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_theta2, 2U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_f2, 3U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_pretheta1, 4U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_pretheta2, 5U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fx, 6U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fy, 7U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_M, 8U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargin, 9U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargout, 10U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_input, 11U, c11_g_sf_marshallOut,
    c11_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_preinput, 12U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fxref, 13U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fyref, 14U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Mref, 15U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx1, 16U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx2, 17U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry1, 18U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry2, 19U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_C, 20U, c11_d_sf_marshallOut,
    c11_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_output, 21U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  CV_EML_FCN(0, 6);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 126);
  c11_theta1 = c11_input[0];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, MAX_int8_T);
  c11_f1 = c11_input[1];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 128U);
  c11_theta2 = c11_input[2];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 129U);
  c11_f2 = c11_input[3];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 131U);
  c11_pretheta1 = c11_preinput[0];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 132U);
  c11_pretheta2 = c11_preinput[2];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 134U);
  c11_a = -c11_f1;
  c11_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_b);
  c11_y = c11_a * c11_b;
  c11_b_a = c11_f2;
  c11_b_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_b_b);
  c11_b_y = c11_b_a * c11_b_b;
  c11_Fx = c11_y - c11_b_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 135U);
  c11_c_a = c11_f1;
  c11_c_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_c_b);
  c11_c_y = c11_c_a * c11_c_b;
  c11_d_a = c11_f2;
  c11_d_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_d_b);
  c11_d_y = c11_d_a * c11_d_b;
  c11_Fy = c11_c_y + c11_d_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 136U);
  c11_e_a = c11_rx1;
  c11_e_b = c11_f1;
  c11_e_y = c11_e_a * c11_e_b;
  c11_f_a = c11_e_y;
  c11_f_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_f_b);
  c11_f_y = c11_f_a * c11_f_b;
  c11_g_a = c11_ry1;
  c11_g_b = c11_f1;
  c11_g_y = c11_g_a * c11_g_b;
  c11_h_a = c11_g_y;
  c11_h_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_h_b);
  c11_h_y = c11_h_a * c11_h_b;
  c11_i_a = c11_rx2;
  c11_i_b = c11_f2;
  c11_i_y = c11_i_a * c11_i_b;
  c11_j_a = c11_i_y;
  c11_j_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_j_b);
  c11_j_y = c11_j_a * c11_j_b;
  c11_k_a = c11_ry2;
  c11_k_b = c11_f2;
  c11_k_y = c11_k_a * c11_k_b;
  c11_l_a = c11_k_y;
  c11_l_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_l_b);
  c11_l_y = c11_l_a * c11_l_b;
  c11_M = ((c11_f_y + c11_h_y) + c11_j_y) + c11_l_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 139U);
  c11_m_a = c11_C[0];
  c11_m_b = c11_mpower(chartInstance, c11_Fx - c11_Fxref);
  c11_m_y = c11_m_a * c11_m_b;
  c11_n_a = c11_C[1];
  c11_n_b = c11_mpower(chartInstance, c11_Fy - c11_Fyref);
  c11_n_y = c11_n_a * c11_n_b;
  c11_o_a = c11_C[2];
  c11_o_b = c11_mpower(chartInstance, c11_M - c11_Mref);
  c11_o_y = c11_o_a * c11_o_b;
  c11_p_a = c11_C[3];
  c11_p_b = c11_mpower(chartInstance, c11_f1);
  c11_p_y = c11_p_a * c11_p_b;
  c11_q_a = c11_C[4];
  c11_q_b = c11_mpower(chartInstance, c11_f2);
  c11_q_y = c11_q_a * c11_q_b;
  c11_r_a = c11_C[5];
  c11_r_b = c11_mpower(chartInstance, c11_theta1 - c11_pretheta1);
  c11_r_y = c11_r_a * c11_r_b;
  c11_s_a = c11_C[6];
  c11_s_b = c11_mpower(chartInstance, c11_theta2 - c11_pretheta2);
  c11_s_y = c11_s_a * c11_s_b;
  c11_output = (((((c11_m_y + c11_n_y) + c11_o_y) + c11_p_y) + c11_q_y) +
                c11_r_y) + c11_s_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -139);
  _SFD_SYMBOL_SCOPE_POP();
  return c11_output;
}

static real_T c11_sind(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_x)
{
  real_T c11_b_x;
  c11_b_x = c11_x;
  c11_b_sind(chartInstance, &c11_b_x);
  return c11_b_x;
}

static boolean_T c11_isfinite(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c11_x)
{
  real_T c11_b_x;
  boolean_T c11_b_b;
  boolean_T c11_b0;
  real_T c11_c_x;
  boolean_T c11_c_b;
  boolean_T c11_b1;
  c11_b_x = c11_x;
  c11_b_b = muDoubleScalarIsInf(c11_b_x);
  c11_b0 = !c11_b_b;
  c11_c_x = c11_x;
  c11_c_b = muDoubleScalarIsNaN(c11_c_x);
  c11_b1 = !c11_c_b;
  return c11_b0 && c11_b1;
}

static real_T c11_cosd(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_x)
{
  real_T c11_b_x;
  c11_b_x = c11_x;
  c11_b_cosd(chartInstance, &c11_b_x);
  return c11_b_x;
}

static real_T c11_mpower(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c11_a)
{
  real_T c11_b_a;
  real_T c11_c_a;
  real_T c11_ak;
  real_T c11_d_a;
  real_T c11_e_a;
  real_T c11_b;
  c11_b_a = c11_a;
  c11_c_a = c11_b_a;
  c11_eml_scalar_eg(chartInstance);
  c11_ak = c11_c_a;
  c11_d_a = c11_ak;
  c11_eml_scalar_eg(chartInstance);
  c11_e_a = c11_d_a;
  c11_b = c11_d_a;
  return c11_e_a * c11_b;
}

static real_T c11_b_costFunction(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c11_input[4], real_T c11_preinput[4], real_T c11_Fxref,
  real_T c11_Fyref, real_T c11_Mref, real_T c11_rx1, real_T c11_rx2, real_T
  c11_ry1, real_T c11_ry2, real_T c11_C[7])
{
  real_T c11_output;
  uint32_T c11_debug_family_var_map[22];
  real_T c11_theta1;
  real_T c11_f1;
  real_T c11_theta2;
  real_T c11_f2;
  real_T c11_pretheta1;
  real_T c11_pretheta2;
  real_T c11_Fx;
  real_T c11_Fy;
  real_T c11_M;
  real_T c11_nargin = 10.0;
  real_T c11_nargout = 1.0;
  real_T c11_a;
  real_T c11_b;
  real_T c11_y;
  real_T c11_b_a;
  real_T c11_b_b;
  real_T c11_b_y;
  real_T c11_c_a;
  real_T c11_c_b;
  real_T c11_c_y;
  real_T c11_d_a;
  real_T c11_d_b;
  real_T c11_d_y;
  real_T c11_e_a;
  real_T c11_e_b;
  real_T c11_e_y;
  real_T c11_f_a;
  real_T c11_f_b;
  real_T c11_f_y;
  real_T c11_g_a;
  real_T c11_g_b;
  real_T c11_g_y;
  real_T c11_h_a;
  real_T c11_h_b;
  real_T c11_h_y;
  real_T c11_i_a;
  real_T c11_i_b;
  real_T c11_i_y;
  real_T c11_j_a;
  real_T c11_j_b;
  real_T c11_j_y;
  real_T c11_k_a;
  real_T c11_k_b;
  real_T c11_k_y;
  real_T c11_l_a;
  real_T c11_l_b;
  real_T c11_l_y;
  real_T c11_m_a;
  real_T c11_m_b;
  real_T c11_m_y;
  real_T c11_n_a;
  real_T c11_n_b;
  real_T c11_n_y;
  real_T c11_o_a;
  real_T c11_o_b;
  real_T c11_o_y;
  real_T c11_p_a;
  real_T c11_p_b;
  real_T c11_p_y;
  real_T c11_q_a;
  real_T c11_q_b;
  real_T c11_q_y;
  real_T c11_r_a;
  real_T c11_r_b;
  real_T c11_r_y;
  real_T c11_s_a;
  real_T c11_s_b;
  real_T c11_s_y;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 22U, 22U, c11_d_debug_family_names,
    c11_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_theta1, 0U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_f1, 1U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_theta2, 2U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_f2, 3U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_pretheta1, 4U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_pretheta2, 5U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fx, 6U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fy, 7U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_M, 8U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargin, 9U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargout, 10U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_input, 11U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_preinput, 12U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fxref, 13U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fyref, 14U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Mref, 15U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx1, 16U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx2, 17U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry1, 18U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry2, 19U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_C, 20U, c11_d_sf_marshallOut,
    c11_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_output, 21U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  CV_EML_FCN(0, 6);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 126);
  c11_theta1 = c11_input[0];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, MAX_int8_T);
  c11_f1 = c11_input[1];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 128U);
  c11_theta2 = c11_input[2];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 129U);
  c11_f2 = c11_input[3];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 131U);
  c11_pretheta1 = c11_preinput[0];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 132U);
  c11_pretheta2 = c11_preinput[2];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 134U);
  c11_a = -c11_f1;
  c11_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_b);
  c11_y = c11_a * c11_b;
  c11_b_a = c11_f2;
  c11_b_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_b_b);
  c11_b_y = c11_b_a * c11_b_b;
  c11_Fx = c11_y - c11_b_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 135U);
  c11_c_a = c11_f1;
  c11_c_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_c_b);
  c11_c_y = c11_c_a * c11_c_b;
  c11_d_a = c11_f2;
  c11_d_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_d_b);
  c11_d_y = c11_d_a * c11_d_b;
  c11_Fy = c11_c_y + c11_d_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 136U);
  c11_e_a = c11_rx1;
  c11_e_b = c11_f1;
  c11_e_y = c11_e_a * c11_e_b;
  c11_f_a = c11_e_y;
  c11_f_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_f_b);
  c11_f_y = c11_f_a * c11_f_b;
  c11_g_a = c11_ry1;
  c11_g_b = c11_f1;
  c11_g_y = c11_g_a * c11_g_b;
  c11_h_a = c11_g_y;
  c11_h_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_h_b);
  c11_h_y = c11_h_a * c11_h_b;
  c11_i_a = c11_rx2;
  c11_i_b = c11_f2;
  c11_i_y = c11_i_a * c11_i_b;
  c11_j_a = c11_i_y;
  c11_j_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_j_b);
  c11_j_y = c11_j_a * c11_j_b;
  c11_k_a = c11_ry2;
  c11_k_b = c11_f2;
  c11_k_y = c11_k_a * c11_k_b;
  c11_l_a = c11_k_y;
  c11_l_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_l_b);
  c11_l_y = c11_l_a * c11_l_b;
  c11_M = ((c11_f_y + c11_h_y) + c11_j_y) + c11_l_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 139U);
  c11_m_a = c11_C[0];
  c11_m_b = c11_mpower(chartInstance, c11_Fx - c11_Fxref);
  c11_m_y = c11_m_a * c11_m_b;
  c11_n_a = c11_C[1];
  c11_n_b = c11_mpower(chartInstance, c11_Fy - c11_Fyref);
  c11_n_y = c11_n_a * c11_n_b;
  c11_o_a = c11_C[2];
  c11_o_b = c11_mpower(chartInstance, c11_M - c11_Mref);
  c11_o_y = c11_o_a * c11_o_b;
  c11_p_a = c11_C[3];
  c11_p_b = c11_mpower(chartInstance, c11_f1);
  c11_p_y = c11_p_a * c11_p_b;
  c11_q_a = c11_C[4];
  c11_q_b = c11_mpower(chartInstance, c11_f2);
  c11_q_y = c11_q_a * c11_q_b;
  c11_r_a = c11_C[5];
  c11_r_b = c11_mpower(chartInstance, c11_theta1 - c11_pretheta1);
  c11_r_y = c11_r_a * c11_r_b;
  c11_s_a = c11_C[6];
  c11_s_b = c11_mpower(chartInstance, c11_theta2 - c11_pretheta2);
  c11_s_y = c11_s_a * c11_s_b;
  c11_output = (((((c11_m_y + c11_n_y) + c11_o_y) + c11_p_y) + c11_q_y) +
                c11_r_y) + c11_s_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -139);
  _SFD_SYMBOL_SCOPE_POP();
  return c11_output;
}

static void c11_SteepestDescent(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c11_input[4], real_T c11_preinput[4], real_T c11_Fxref,
  real_T c11_Fyref, real_T c11_Mref, real_T c11_rx1, real_T c11_rx2, real_T
  c11_ry1, real_T c11_ry2, real_T c11_C[7], real_T c11_output[4])
{
  uint32_T c11_debug_family_var_map[23];
  real_T c11_theta1;
  real_T c11_f1;
  real_T c11_theta2;
  real_T c11_f2;
  real_T c11_pretheta1;
  real_T c11_pretheta2;
  real_T c11_dVdtheta1;
  real_T c11_dVdf1;
  real_T c11_dVdtheta2;
  real_T c11_dVdf2;
  real_T c11_nargin = 10.0;
  real_T c11_nargout = 1.0;
  real_T c11_a;
  real_T c11_b;
  real_T c11_y;
  real_T c11_b_a;
  real_T c11_b_b;
  real_T c11_b_y;
  real_T c11_c_a;
  real_T c11_c_b;
  real_T c11_c_y;
  real_T c11_d_a;
  real_T c11_d_b;
  real_T c11_d_y;
  real_T c11_e_a;
  real_T c11_e_b;
  real_T c11_e_y;
  real_T c11_f_a;
  real_T c11_f_b;
  real_T c11_f_y;
  real_T c11_g_a;
  real_T c11_g_b;
  real_T c11_g_y;
  real_T c11_h_a;
  real_T c11_h_b;
  real_T c11_h_y;
  real_T c11_i_a;
  real_T c11_i_b;
  real_T c11_i_y;
  real_T c11_j_a;
  real_T c11_j_b;
  real_T c11_j_y;
  real_T c11_x;
  real_T c11_b_x;
  real_T c11_k_a;
  real_T c11_k_b;
  real_T c11_k_y;
  real_T c11_l_a;
  real_T c11_l_b;
  real_T c11_l_y;
  real_T c11_m_a;
  real_T c11_m_b;
  real_T c11_m_y;
  real_T c11_n_a;
  real_T c11_n_b;
  real_T c11_n_y;
  real_T c11_o_a;
  real_T c11_o_b;
  real_T c11_o_y;
  real_T c11_p_a;
  real_T c11_p_b;
  real_T c11_p_y;
  real_T c11_q_a;
  real_T c11_q_b;
  real_T c11_q_y;
  real_T c11_r_a;
  real_T c11_r_b;
  real_T c11_r_y;
  real_T c11_s_a;
  real_T c11_s_b;
  real_T c11_s_y;
  real_T c11_t_a;
  real_T c11_t_b;
  real_T c11_t_y;
  real_T c11_u_a;
  real_T c11_u_b;
  real_T c11_u_y;
  real_T c11_v_a;
  real_T c11_v_b;
  real_T c11_v_y;
  real_T c11_w_a;
  real_T c11_w_b;
  real_T c11_w_y;
  real_T c11_x_a;
  real_T c11_x_b;
  real_T c11_x_y;
  real_T c11_y_a;
  real_T c11_y_b;
  real_T c11_y_y;
  real_T c11_ab_a;
  real_T c11_ab_b;
  real_T c11_ab_y;
  real_T c11_bb_a;
  real_T c11_bb_b;
  real_T c11_bb_y;
  real_T c11_c_x;
  real_T c11_d_x;
  real_T c11_cb_a;
  real_T c11_cb_b;
  real_T c11_cb_y;
  real_T c11_db_a;
  real_T c11_db_b;
  real_T c11_db_y;
  real_T c11_eb_a;
  real_T c11_eb_b;
  real_T c11_eb_y;
  real_T c11_fb_a;
  real_T c11_fb_b;
  real_T c11_fb_y;
  real_T c11_gb_a;
  real_T c11_gb_b;
  real_T c11_gb_y;
  real_T c11_hb_a;
  real_T c11_hb_b;
  real_T c11_hb_y;
  real_T c11_ib_a;
  real_T c11_ib_b;
  real_T c11_ib_y;
  real_T c11_jb_a;
  real_T c11_jb_b;
  real_T c11_jb_y;
  real_T c11_kb_a;
  real_T c11_kb_b;
  real_T c11_kb_y;
  real_T c11_lb_a;
  real_T c11_lb_b;
  real_T c11_lb_y;
  real_T c11_mb_a;
  real_T c11_mb_b;
  real_T c11_mb_y;
  real_T c11_nb_a;
  real_T c11_nb_b;
  real_T c11_nb_y;
  real_T c11_ob_a;
  real_T c11_ob_b;
  real_T c11_ob_y;
  real_T c11_pb_a;
  real_T c11_pb_b;
  real_T c11_pb_y;
  real_T c11_qb_a;
  real_T c11_qb_b;
  real_T c11_qb_y;
  real_T c11_rb_a;
  real_T c11_rb_b;
  real_T c11_rb_y;
  real_T c11_sb_a;
  real_T c11_sb_b;
  real_T c11_sb_y;
  real_T c11_tb_a;
  real_T c11_tb_b;
  real_T c11_tb_y;
  real_T c11_ub_a;
  real_T c11_ub_b;
  real_T c11_ub_y;
  real_T c11_vb_a;
  real_T c11_vb_b;
  real_T c11_vb_y;
  real_T c11_wb_a;
  real_T c11_wb_b;
  real_T c11_wb_y;
  real_T c11_xb_a;
  real_T c11_xb_b;
  real_T c11_xb_y;
  real_T c11_yb_a;
  real_T c11_yb_b;
  real_T c11_yb_y;
  real_T c11_ac_a;
  real_T c11_ac_b;
  real_T c11_ac_y;
  real_T c11_bc_a;
  real_T c11_bc_b;
  real_T c11_bc_y;
  real_T c11_cc_a;
  real_T c11_cc_b;
  real_T c11_cc_y;
  real_T c11_dc_a;
  real_T c11_dc_b;
  real_T c11_dc_y;
  real_T c11_ec_a;
  real_T c11_ec_b;
  real_T c11_ec_y;
  real_T c11_fc_a;
  real_T c11_fc_b;
  real_T c11_fc_y;
  real_T c11_gc_a;
  real_T c11_gc_b;
  real_T c11_gc_y;
  real_T c11_hc_a;
  real_T c11_hc_b;
  real_T c11_hc_y;
  real_T c11_ic_a;
  real_T c11_ic_b;
  real_T c11_ic_y;
  real_T c11_jc_a;
  real_T c11_jc_b;
  real_T c11_jc_y;
  real_T c11_kc_a;
  real_T c11_kc_b;
  real_T c11_kc_y;
  real_T c11_lc_a;
  real_T c11_lc_b;
  real_T c11_lc_y;
  real_T c11_mc_a;
  real_T c11_mc_b;
  real_T c11_mc_y;
  real_T c11_nc_a;
  real_T c11_nc_b;
  real_T c11_nc_y;
  real_T c11_oc_a;
  real_T c11_oc_b;
  real_T c11_oc_y;
  real_T c11_pc_a;
  real_T c11_pc_b;
  real_T c11_pc_y;
  real_T c11_qc_a;
  real_T c11_qc_b;
  real_T c11_qc_y;
  real_T c11_rc_a;
  real_T c11_rc_b;
  real_T c11_rc_y;
  real_T c11_sc_a;
  real_T c11_sc_b;
  real_T c11_sc_y;
  real_T c11_tc_a;
  real_T c11_tc_b;
  real_T c11_tc_y;
  real_T c11_uc_a;
  real_T c11_uc_b;
  real_T c11_uc_y;
  real_T c11_vc_a;
  real_T c11_vc_b;
  real_T c11_vc_y;
  real_T c11_wc_a;
  real_T c11_wc_b;
  real_T c11_wc_y;
  real_T c11_xc_a;
  real_T c11_xc_b;
  real_T c11_xc_y;
  real_T c11_yc_a;
  real_T c11_yc_b;
  real_T c11_yc_y;
  real_T c11_ad_a;
  real_T c11_ad_b;
  real_T c11_ad_y;
  real_T c11_bd_a;
  real_T c11_bd_b;
  real_T c11_bd_y;
  real_T c11_cd_a;
  real_T c11_cd_b;
  real_T c11_cd_y;
  real_T c11_dd_a;
  real_T c11_dd_b;
  real_T c11_dd_y;
  real_T c11_ed_a;
  real_T c11_ed_b;
  real_T c11_ed_y;
  real_T c11_fd_a;
  real_T c11_fd_b;
  real_T c11_fd_y;
  real_T c11_gd_a;
  real_T c11_gd_b;
  real_T c11_gd_y;
  real_T c11_hd_a;
  real_T c11_hd_b;
  real_T c11_hd_y;
  real_T c11_id_a;
  real_T c11_id_b;
  real_T c11_id_y;
  real_T c11_jd_a;
  real_T c11_jd_b;
  real_T c11_jd_y;
  real_T c11_kd_a;
  real_T c11_kd_b;
  real_T c11_kd_y;
  real_T c11_ld_a;
  real_T c11_ld_b;
  real_T c11_ld_y;
  real_T c11_md_a;
  real_T c11_md_b;
  real_T c11_md_y;
  real_T c11_nd_a;
  real_T c11_nd_b;
  real_T c11_nd_y;
  real_T c11_od_a;
  real_T c11_od_b;
  real_T c11_od_y;
  real_T c11_pd_a;
  real_T c11_pd_b;
  real_T c11_pd_y;
  real_T c11_qd_a;
  real_T c11_qd_b;
  real_T c11_qd_y;
  real_T c11_b_dVdtheta1[4];
  int32_T c11_i116;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 23U, 23U, c11_f_debug_family_names,
    c11_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_theta1, 0U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_f1, 1U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_theta2, 2U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_f2, 3U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_pretheta1, 4U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_pretheta2, 5U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_dVdtheta1, 6U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_dVdf1, 7U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_dVdtheta2, 8U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_dVdf2, 9U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargin, 10U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargout, 11U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_input, 12U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_preinput, 13U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fxref, 14U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Fyref, 15U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_Mref, 16U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx1, 17U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_rx2, 18U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry1, 19U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_ry2, 20U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_C, 21U, c11_d_sf_marshallOut,
    c11_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_output, 22U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  CV_EML_FCN(0, 8);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 163U);
  c11_theta1 = c11_input[0];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 164U);
  c11_f1 = c11_input[1];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 165U);
  c11_theta2 = c11_input[2];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 166U);
  c11_f2 = c11_input[3];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 168U);
  c11_pretheta1 = c11_preinput[0];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 169U);
  c11_pretheta2 = c11_preinput[2];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 171U);
  c11_a = c11_f1;
  c11_b = c11_ry1;
  c11_y = c11_a * c11_b;
  c11_b_a = c11_y;
  c11_b_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_b_b);
  c11_b_y = c11_b_a * c11_b_b;
  c11_c_a = c11_f1;
  c11_c_b = c11_rx1;
  c11_c_y = c11_c_a * c11_c_b;
  c11_d_a = c11_c_y;
  c11_d_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_d_b);
  c11_d_y = c11_d_a * c11_d_b;
  c11_e_a = c11_C[2];
  c11_e_b = c11_b_y - c11_d_y;
  c11_e_y = c11_e_a * c11_e_b;
  c11_f_a = c11_f1;
  c11_f_b = c11_rx1;
  c11_f_y = c11_f_a * c11_f_b;
  c11_g_a = c11_f_y;
  c11_g_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_g_b);
  c11_g_y = c11_g_a * c11_g_b;
  c11_h_a = c11_f2;
  c11_h_b = c11_rx2;
  c11_h_y = c11_h_a * c11_h_b;
  c11_i_a = c11_h_y;
  c11_i_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_i_b);
  c11_i_y = c11_i_a * c11_i_b;
  c11_j_a = c11_f1;
  c11_j_b = c11_ry1;
  c11_j_y = c11_j_a * c11_j_b;
  c11_x = c11_theta1;
  c11_b_x = c11_x;
  c11_b_x = muDoubleScalarSin(c11_b_x);
  c11_k_a = c11_j_y;
  c11_k_b = c11_b_x;
  c11_k_y = c11_k_a * c11_k_b;
  c11_l_a = c11_f2;
  c11_l_b = c11_ry2;
  c11_l_y = c11_l_a * c11_l_b;
  c11_m_a = c11_l_y;
  c11_m_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_m_b);
  c11_m_y = c11_m_a * c11_m_b;
  c11_n_a = c11_e_y;
  c11_n_b = (((c11_g_y - c11_Mref) + c11_i_y) + c11_k_y) + c11_m_y;
  c11_n_y = c11_n_a * c11_n_b;
  c11_o_a = c11_C[5];
  c11_o_b = c11_pretheta1 - c11_theta1;
  c11_o_y = c11_o_a * c11_o_b;
  c11_p_a = c11_C[1];
  c11_p_b = c11_f1;
  c11_p_y = c11_p_a * c11_p_b;
  c11_q_a = c11_p_y;
  c11_q_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_q_b);
  c11_q_y = c11_q_a * c11_q_b;
  c11_r_a = c11_f1;
  c11_r_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_r_b);
  c11_r_y = c11_r_a * c11_r_b;
  c11_s_a = c11_f2;
  c11_s_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_s_b);
  c11_s_y = c11_s_a * c11_s_b;
  c11_t_a = c11_q_y;
  c11_t_b = (c11_r_y - c11_Fyref) + c11_s_y;
  c11_t_y = c11_t_a * c11_t_b;
  c11_u_a = c11_C[0];
  c11_u_b = c11_f1;
  c11_u_y = c11_u_a * c11_u_b;
  c11_v_a = c11_u_y;
  c11_v_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_v_b);
  c11_v_y = c11_v_a * c11_v_b;
  c11_w_a = c11_f1;
  c11_w_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_w_b);
  c11_w_y = c11_w_a * c11_w_b;
  c11_x_a = c11_f2;
  c11_x_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_x_b);
  c11_x_y = c11_x_a * c11_x_b;
  c11_y_a = c11_v_y;
  c11_y_b = (c11_Fxref + c11_w_y) + c11_x_y;
  c11_y_y = c11_y_a * c11_y_b;
  c11_dVdtheta1 = ((c11_n_y - c11_o_y) - c11_t_y) + c11_y_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 178U);
  c11_ab_a = c11_C[3];
  c11_ab_b = c11_f1;
  c11_ab_y = c11_ab_a * c11_ab_b;
  c11_bb_a = c11_C[0];
  c11_bb_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_bb_b);
  c11_bb_y = c11_bb_a * c11_bb_b;
  c11_c_x = c11_theta1;
  c11_d_x = c11_c_x;
  c11_d_x = muDoubleScalarSin(c11_d_x);
  c11_cb_a = c11_f1;
  c11_cb_b = c11_d_x;
  c11_cb_y = c11_cb_a * c11_cb_b;
  c11_db_a = c11_f2;
  c11_db_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_db_b);
  c11_db_y = c11_db_a * c11_db_b;
  c11_eb_a = c11_bb_y;
  c11_eb_b = (c11_Fxref + c11_cb_y) + c11_db_y;
  c11_eb_y = c11_eb_a * c11_eb_b;
  c11_fb_a = c11_C[1];
  c11_fb_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_fb_b);
  c11_fb_y = c11_fb_a * c11_fb_b;
  c11_gb_a = c11_f1;
  c11_gb_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_gb_b);
  c11_gb_y = c11_gb_a * c11_gb_b;
  c11_hb_a = c11_f2;
  c11_hb_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_hb_b);
  c11_hb_y = c11_hb_a * c11_hb_b;
  c11_ib_a = c11_fb_y;
  c11_ib_b = (c11_gb_y - c11_Fyref) + c11_hb_y;
  c11_ib_y = c11_ib_a * c11_ib_b;
  c11_jb_a = c11_rx1;
  c11_jb_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_jb_b);
  c11_jb_y = c11_jb_a * c11_jb_b;
  c11_kb_a = c11_ry1;
  c11_kb_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_kb_b);
  c11_kb_y = c11_kb_a * c11_kb_b;
  c11_lb_a = c11_C[2];
  c11_lb_b = c11_jb_y + c11_kb_y;
  c11_lb_y = c11_lb_a * c11_lb_b;
  c11_mb_a = c11_f1;
  c11_mb_b = c11_rx1;
  c11_mb_y = c11_mb_a * c11_mb_b;
  c11_nb_a = c11_mb_y;
  c11_nb_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_nb_b);
  c11_nb_y = c11_nb_a * c11_nb_b;
  c11_ob_a = c11_f2;
  c11_ob_b = c11_rx2;
  c11_ob_y = c11_ob_a * c11_ob_b;
  c11_pb_a = c11_ob_y;
  c11_pb_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_pb_b);
  c11_pb_y = c11_pb_a * c11_pb_b;
  c11_qb_a = c11_f1;
  c11_qb_b = c11_ry1;
  c11_qb_y = c11_qb_a * c11_qb_b;
  c11_rb_a = c11_qb_y;
  c11_rb_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_rb_b);
  c11_rb_y = c11_rb_a * c11_rb_b;
  c11_sb_a = c11_f2;
  c11_sb_b = c11_ry2;
  c11_sb_y = c11_sb_a * c11_sb_b;
  c11_tb_a = c11_sb_y;
  c11_tb_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_tb_b);
  c11_tb_y = c11_tb_a * c11_tb_b;
  c11_ub_a = c11_lb_y;
  c11_ub_b = (((c11_nb_y - c11_Mref) + c11_pb_y) + c11_rb_y) + c11_tb_y;
  c11_ub_y = c11_ub_a * c11_ub_b;
  c11_dVdf1 = ((c11_ab_y + c11_eb_y) + c11_ib_y) + c11_ub_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 184U);
  c11_vb_a = c11_f2;
  c11_vb_b = c11_ry2;
  c11_vb_y = c11_vb_a * c11_vb_b;
  c11_wb_a = c11_vb_y;
  c11_wb_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_wb_b);
  c11_wb_y = c11_wb_a * c11_wb_b;
  c11_xb_a = c11_f2;
  c11_xb_b = c11_rx2;
  c11_xb_y = c11_xb_a * c11_xb_b;
  c11_yb_a = c11_xb_y;
  c11_yb_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_yb_b);
  c11_yb_y = c11_yb_a * c11_yb_b;
  c11_ac_a = c11_C[2];
  c11_ac_b = c11_wb_y - c11_yb_y;
  c11_ac_y = c11_ac_a * c11_ac_b;
  c11_bc_a = c11_f1;
  c11_bc_b = c11_rx1;
  c11_bc_y = c11_bc_a * c11_bc_b;
  c11_cc_a = c11_bc_y;
  c11_cc_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_cc_b);
  c11_cc_y = c11_cc_a * c11_cc_b;
  c11_dc_a = c11_f2;
  c11_dc_b = c11_rx2;
  c11_dc_y = c11_dc_a * c11_dc_b;
  c11_ec_a = c11_dc_y;
  c11_ec_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_ec_b);
  c11_ec_y = c11_ec_a * c11_ec_b;
  c11_fc_a = c11_f1;
  c11_fc_b = c11_ry1;
  c11_fc_y = c11_fc_a * c11_fc_b;
  c11_gc_a = c11_fc_y;
  c11_gc_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_gc_b);
  c11_gc_y = c11_gc_a * c11_gc_b;
  c11_hc_a = c11_f2;
  c11_hc_b = c11_ry2;
  c11_hc_y = c11_hc_a * c11_hc_b;
  c11_ic_a = c11_hc_y;
  c11_ic_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_ic_b);
  c11_ic_y = c11_ic_a * c11_ic_b;
  c11_jc_a = c11_ac_y;
  c11_jc_b = (((c11_cc_y - c11_Mref) + c11_ec_y) + c11_gc_y) + c11_ic_y;
  c11_jc_y = c11_jc_a * c11_jc_b;
  c11_kc_a = c11_C[6];
  c11_kc_b = c11_pretheta2 - c11_theta2;
  c11_kc_y = c11_kc_a * c11_kc_b;
  c11_lc_a = c11_C[1];
  c11_lc_b = c11_f2;
  c11_lc_y = c11_lc_a * c11_lc_b;
  c11_mc_a = c11_lc_y;
  c11_mc_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_mc_b);
  c11_mc_y = c11_mc_a * c11_mc_b;
  c11_nc_a = c11_f1;
  c11_nc_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_nc_b);
  c11_nc_y = c11_nc_a * c11_nc_b;
  c11_oc_a = c11_f2;
  c11_oc_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_oc_b);
  c11_oc_y = c11_oc_a * c11_oc_b;
  c11_pc_a = c11_mc_y;
  c11_pc_b = (c11_nc_y - c11_Fyref) + c11_oc_y;
  c11_pc_y = c11_pc_a * c11_pc_b;
  c11_qc_a = c11_C[0];
  c11_qc_b = c11_f2;
  c11_qc_y = c11_qc_a * c11_qc_b;
  c11_rc_a = c11_qc_y;
  c11_rc_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_rc_b);
  c11_rc_y = c11_rc_a * c11_rc_b;
  c11_sc_a = c11_f1;
  c11_sc_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_sc_b);
  c11_sc_y = c11_sc_a * c11_sc_b;
  c11_tc_a = c11_f2;
  c11_tc_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_tc_b);
  c11_tc_y = c11_tc_a * c11_tc_b;
  c11_uc_a = c11_rc_y;
  c11_uc_b = (c11_Fxref + c11_sc_y) + c11_tc_y;
  c11_uc_y = c11_uc_a * c11_uc_b;
  c11_dVdtheta2 = ((c11_jc_y - c11_kc_y) - c11_pc_y) + c11_uc_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 191U);
  c11_vc_a = c11_C[4];
  c11_vc_b = c11_f2;
  c11_vc_y = c11_vc_a * c11_vc_b;
  c11_wc_a = c11_C[0];
  c11_wc_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_wc_b);
  c11_wc_y = c11_wc_a * c11_wc_b;
  c11_xc_a = c11_f1;
  c11_xc_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_xc_b);
  c11_xc_y = c11_xc_a * c11_xc_b;
  c11_yc_a = c11_f2;
  c11_yc_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_yc_b);
  c11_yc_y = c11_yc_a * c11_yc_b;
  c11_ad_a = c11_wc_y;
  c11_ad_b = (c11_Fxref + c11_xc_y) + c11_yc_y;
  c11_ad_y = c11_ad_a * c11_ad_b;
  c11_bd_a = c11_C[1];
  c11_bd_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_bd_b);
  c11_bd_y = c11_bd_a * c11_bd_b;
  c11_cd_a = c11_f1;
  c11_cd_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_cd_b);
  c11_cd_y = c11_cd_a * c11_cd_b;
  c11_dd_a = c11_f2;
  c11_dd_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_dd_b);
  c11_dd_y = c11_dd_a * c11_dd_b;
  c11_ed_a = c11_bd_y;
  c11_ed_b = (c11_cd_y - c11_Fyref) + c11_dd_y;
  c11_ed_y = c11_ed_a * c11_ed_b;
  c11_fd_a = c11_rx2;
  c11_fd_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_fd_b);
  c11_fd_y = c11_fd_a * c11_fd_b;
  c11_gd_a = c11_ry2;
  c11_gd_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_gd_b);
  c11_gd_y = c11_gd_a * c11_gd_b;
  c11_hd_a = c11_C[2];
  c11_hd_b = c11_fd_y + c11_gd_y;
  c11_hd_y = c11_hd_a * c11_hd_b;
  c11_id_a = c11_f1;
  c11_id_b = c11_rx1;
  c11_id_y = c11_id_a * c11_id_b;
  c11_jd_a = c11_id_y;
  c11_jd_b = c11_theta1;
  c11_b_cosd(chartInstance, &c11_jd_b);
  c11_jd_y = c11_jd_a * c11_jd_b;
  c11_kd_a = c11_f2;
  c11_kd_b = c11_rx2;
  c11_kd_y = c11_kd_a * c11_kd_b;
  c11_ld_a = c11_kd_y;
  c11_ld_b = c11_theta2;
  c11_b_cosd(chartInstance, &c11_ld_b);
  c11_ld_y = c11_ld_a * c11_ld_b;
  c11_md_a = c11_f1;
  c11_md_b = c11_ry1;
  c11_md_y = c11_md_a * c11_md_b;
  c11_nd_a = c11_md_y;
  c11_nd_b = c11_theta1;
  c11_b_sind(chartInstance, &c11_nd_b);
  c11_nd_y = c11_nd_a * c11_nd_b;
  c11_od_a = c11_f2;
  c11_od_b = c11_ry2;
  c11_od_y = c11_od_a * c11_od_b;
  c11_pd_a = c11_od_y;
  c11_pd_b = c11_theta2;
  c11_b_sind(chartInstance, &c11_pd_b);
  c11_pd_y = c11_pd_a * c11_pd_b;
  c11_qd_a = c11_hd_y;
  c11_qd_b = (((c11_jd_y - c11_Mref) + c11_ld_y) + c11_nd_y) + c11_pd_y;
  c11_qd_y = c11_qd_a * c11_qd_b;
  c11_dVdf2 = ((c11_vc_y + c11_ad_y) + c11_ed_y) + c11_qd_y;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 197U);
  c11_b_dVdtheta1[0] = c11_dVdtheta1;
  c11_b_dVdtheta1[1] = c11_dVdf1;
  c11_b_dVdtheta1[2] = c11_dVdtheta2;
  c11_b_dVdtheta1[3] = c11_dVdf2;
  for (c11_i116 = 0; c11_i116 < 4; c11_i116++) {
    c11_output[c11_i116] = -c11_b_dVdtheta1[c11_i116];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -197);
  _SFD_SYMBOL_SCOPE_POP();
}

static const mxArray *c11_i_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_u;
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  c11_u = *(int32_T *)c11_inData;
  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", &c11_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static int32_T c11_j_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId)
{
  int32_T c11_y;
  int32_T c11_i117;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), &c11_i117, 1, 6, 0U, 0, 0U, 0);
  c11_y = c11_i117;
  sf_mex_destroy(&c11_u);
  return c11_y;
}

static void c11_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_b_sfEvent;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  int32_T c11_y;
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c11_b_sfEvent = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_y = c11_j_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_b_sfEvent),
    &c11_thisId);
  sf_mex_destroy(&c11_b_sfEvent);
  *(int32_T *)c11_outData = c11_y;
  sf_mex_destroy(&c11_mxArrayInData);
}

static uint8_T c11_k_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_b_is_active_c11_BoatLibraryTest02, const
  char_T *c11_identifier)
{
  uint8_T c11_y;
  emlrtMsgIdentifier c11_thisId;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_y = c11_l_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c11_b_is_active_c11_BoatLibraryTest02), &c11_thisId);
  sf_mex_destroy(&c11_b_is_active_c11_BoatLibraryTest02);
  return c11_y;
}

static uint8_T c11_l_emlrt_marshallIn(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId)
{
  uint8_T c11_y;
  uint8_T c11_u0;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), &c11_u0, 1, 3, 0U, 0, 0U, 0);
  c11_y = c11_u0;
  sf_mex_destroy(&c11_u);
  return c11_y;
}

static void c11_b_sind(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T *c11_x)
{
  real_T c11_b_x;
  real_T c11_d1;
  real_T c11_c_x;
  real_T c11_d_x;
  real_T c11_e_x;
  real_T c11_xk;
  real_T c11_f_x;
  real_T c11_g_x;
  real_T c11_absx;
  real_T c11_h_x;
  real_T c11_i_x;
  real_T c11_b;
  int8_T c11_n;
  real_T c11_b_b;
  real_T c11_c_b;
  real_T c11_d_b;
  real_T c11_e_b;
  int8_T c11_b_n;
  c11_b_x = *c11_x;
  if (!c11_isfinite(chartInstance, c11_b_x)) {
    c11_d1 = rtNaN;
  } else {
    c11_c_x = c11_b_x;
    c11_d_x = c11_c_x;
    c11_e_x = c11_d_x;
    c11_eml_scalar_eg(chartInstance);
    c11_xk = c11_e_x;
    c11_d_x = muDoubleScalarRem(c11_xk, 360.0);
    c11_f_x = c11_d_x;
    c11_g_x = c11_f_x;
    c11_absx = muDoubleScalarAbs(c11_g_x);
    if (c11_absx > 180.0) {
      if (c11_d_x > 0.0) {
        c11_d_x -= 360.0;
      } else {
        c11_d_x += 360.0;
      }

      c11_h_x = c11_d_x;
      c11_i_x = c11_h_x;
      c11_absx = muDoubleScalarAbs(c11_i_x);
    }

    if (c11_absx <= 45.0) {
      c11_b = c11_d_x;
      c11_d_x = 0.017453292519943295 * c11_b;
      c11_n = 0;
    } else if (c11_absx <= 135.0) {
      if (c11_d_x > 0.0) {
        c11_b_b = c11_d_x - 90.0;
        c11_d_x = 0.017453292519943295 * c11_b_b;
        c11_n = 1;
      } else {
        c11_c_b = c11_d_x + 90.0;
        c11_d_x = 0.017453292519943295 * c11_c_b;
        c11_n = -1;
      }
    } else if (c11_d_x > 0.0) {
      c11_d_b = c11_d_x - 180.0;
      c11_d_x = 0.017453292519943295 * c11_d_b;
      c11_n = 2;
    } else {
      c11_e_b = c11_d_x + 180.0;
      c11_d_x = 0.017453292519943295 * c11_e_b;
      c11_n = -2;
    }

    c11_b_n = c11_n;
    c11_b_x = c11_d_x;
    if ((real_T)c11_b_n == 0.0) {
      c11_d1 = muDoubleScalarSin(c11_b_x);
    } else if ((real_T)c11_b_n == 1.0) {
      c11_d1 = muDoubleScalarCos(c11_b_x);
    } else if ((real_T)c11_b_n == -1.0) {
      c11_d1 = -muDoubleScalarCos(c11_b_x);
    } else {
      c11_d1 = -muDoubleScalarSin(c11_b_x);
    }
  }

  *c11_x = c11_d1;
}

static void c11_b_cosd(SFc11_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T *c11_x)
{
  real_T c11_b_x;
  real_T c11_c_x;
  real_T c11_d_x;
  real_T c11_e_x;
  real_T c11_xk;
  real_T c11_f_x;
  real_T c11_g_x;
  real_T c11_absx;
  real_T c11_h_x;
  real_T c11_i_x;
  real_T c11_b;
  int8_T c11_n;
  real_T c11_b_b;
  real_T c11_c_b;
  real_T c11_d_b;
  real_T c11_e_b;
  int8_T c11_b_n;
  c11_b_x = *c11_x;
  if (!c11_isfinite(chartInstance, c11_b_x)) {
    *c11_x = rtNaN;
  } else {
    c11_c_x = c11_b_x;
    c11_d_x = c11_c_x;
    c11_e_x = c11_d_x;
    c11_eml_scalar_eg(chartInstance);
    c11_xk = c11_e_x;
    c11_d_x = muDoubleScalarRem(c11_xk, 360.0);
    c11_f_x = c11_d_x;
    c11_g_x = c11_f_x;
    c11_absx = muDoubleScalarAbs(c11_g_x);
    if (c11_absx > 180.0) {
      if (c11_d_x > 0.0) {
        c11_d_x -= 360.0;
      } else {
        c11_d_x += 360.0;
      }

      c11_h_x = c11_d_x;
      c11_i_x = c11_h_x;
      c11_absx = muDoubleScalarAbs(c11_i_x);
    }

    if (c11_absx <= 45.0) {
      c11_b = c11_d_x;
      c11_d_x = 0.017453292519943295 * c11_b;
      c11_n = 0;
    } else if (c11_absx <= 135.0) {
      if (c11_d_x > 0.0) {
        c11_b_b = c11_d_x - 90.0;
        c11_d_x = 0.017453292519943295 * c11_b_b;
        c11_n = 1;
      } else {
        c11_c_b = c11_d_x + 90.0;
        c11_d_x = 0.017453292519943295 * c11_c_b;
        c11_n = -1;
      }
    } else if (c11_d_x > 0.0) {
      c11_d_b = c11_d_x - 180.0;
      c11_d_x = 0.017453292519943295 * c11_d_b;
      c11_n = 2;
    } else {
      c11_e_b = c11_d_x + 180.0;
      c11_d_x = 0.017453292519943295 * c11_e_b;
      c11_n = -2;
    }

    c11_b_n = c11_n;
    c11_b_x = c11_d_x;
    if ((real_T)c11_b_n == 0.0) {
      *c11_x = muDoubleScalarCos(c11_b_x);
    } else if ((real_T)c11_b_n == 1.0) {
      *c11_x = -muDoubleScalarSin(c11_b_x);
    } else if ((real_T)c11_b_n == -1.0) {
      *c11_x = muDoubleScalarSin(c11_b_x);
    } else {
      *c11_x = -muDoubleScalarCos(c11_b_x);
    }
  }
}

static void init_dsm_address_info(SFc11_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c11_BoatLibraryTest02_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(651523784U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2254198150U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1567592274U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2509071756U);
}

mxArray *sf_c11_BoatLibraryTest02_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("qeAlU9t0OJulndQpkm8bbH");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,10,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(4);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(7);
      pr[1] = (double)(1);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(4);
      pr[1] = (double)(2);
      mxSetField(mxData,6,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,6,"type",mxType);
    }

    mxSetField(mxData,6,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,7,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,7,"type",mxType);
    }

    mxSetField(mxData,7,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,8,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,8,"type",mxType);
    }

    mxSetField(mxData,8,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,9,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,9,"type",mxType);
    }

    mxSetField(mxData,9,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(4);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c11_BoatLibraryTest02_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c11_BoatLibraryTest02_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c11_BoatLibraryTest02(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"output\",},{M[8],M[0],T\"is_active_c11_BoatLibraryTest02\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c11_BoatLibraryTest02_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
    chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _BoatLibraryTest02MachineNumber_,
           11,
           1,
           1,
           11,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           ssGetPath(S),
           (void *)S);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          init_script_number_translation(_BoatLibraryTest02MachineNumber_,
            chartInstance->chartNumber);
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_BoatLibraryTest02MachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _BoatLibraryTest02MachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"u0");
          _SFD_SET_DATA_PROPS(1,2,0,1,"output");
          _SFD_SET_DATA_PROPS(2,1,1,0,"rx01");
          _SFD_SET_DATA_PROPS(3,1,1,0,"rx02");
          _SFD_SET_DATA_PROPS(4,1,1,0,"ry01");
          _SFD_SET_DATA_PROPS(5,1,1,0,"ry02");
          _SFD_SET_DATA_PROPS(6,1,1,0,"C0");
          _SFD_SET_DATA_PROPS(7,1,1,0,"inputBound");
          _SFD_SET_DATA_PROPS(8,1,1,0,"Fx");
          _SFD_SET_DATA_PROPS(9,1,1,0,"Fy");
          _SFD_SET_DATA_PROPS(10,1,1,0,"M");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,9,3,0,0,0,7,2,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,611);
        _SFD_CV_INIT_EML_FCN(0,1,"getMinFromGrid",613,-1,1281);
        _SFD_CV_INIT_EML_FCN(0,2,"checkInputBound",1283,-1,1451);
        _SFD_CV_INIT_EML_FCN(0,3,"boundGrid",1453,-1,1933);
        _SFD_CV_INIT_EML_FCN(0,4,"sat",1935,-1,2123);
        _SFD_CV_INIT_EML_FCN(0,5,"getStepSize",2125,-1,2939);
        _SFD_CV_INIT_EML_FCN(0,6,"costFunction",2941,-1,3618);
        _SFD_CV_INIT_EML_FCN(0,7,"getDirection",3620,-1,3888);
        _SFD_CV_INIT_EML_FCN(0,8,"SteepestDescent",3890,-1,5510);
        _SFD_CV_INIT_EML_IF(0,1,0,1120,1130,1212,1236);
        _SFD_CV_INIT_EML_IF(0,1,1,1976,1995,2027,2118);
        _SFD_CV_INIT_EML_IF(0,1,2,2027,2050,2082,2118);
        _SFD_CV_INIT_EML_FOR(0,1,0,332,345,584);
        _SFD_CV_INIT_EML_FOR(0,1,1,867,880,1276);
        _SFD_CV_INIT_EML_FOR(0,1,2,884,897,1272);
        _SFD_CV_INIT_EML_FOR(0,1,3,905,918,1264);
        _SFD_CV_INIT_EML_FOR(0,1,4,930,943,1252);
        _SFD_CV_INIT_EML_FOR(0,1,5,1354,1378,1431);
        _SFD_CV_INIT_EML_FOR(0,1,6,3822,3844,3879);
        _SFD_CV_INIT_EML_WHILE(0,1,0,2455,2481,2648);
        _SFD_CV_INIT_EML_WHILE(0,1,1,2672,2712,2882);

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c11_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c11_sf_marshallOut,(MexInFcnForType)
            c11_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c11_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c11_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c11_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c11_b_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 7;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c11_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 4;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c11_c_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c11_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c11_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c11_b_sf_marshallOut,(MexInFcnForType)NULL);

        {
          real_T *c11_rx01;
          real_T *c11_rx02;
          real_T *c11_ry01;
          real_T *c11_ry02;
          real_T *c11_Fx;
          real_T *c11_Fy;
          real_T *c11_M;
          real_T (*c11_u0)[4];
          real_T (*c11_output)[4];
          real_T (*c11_C0)[7];
          real_T (*c11_inputBound)[8];
          c11_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
          c11_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
          c11_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
          c11_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S,
            6);
          c11_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
          c11_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
          c11_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c11_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c11_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c11_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
          c11_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, *c11_u0);
          _SFD_SET_DATA_VALUE_PTR(1U, *c11_output);
          _SFD_SET_DATA_VALUE_PTR(2U, c11_rx01);
          _SFD_SET_DATA_VALUE_PTR(3U, c11_rx02);
          _SFD_SET_DATA_VALUE_PTR(4U, c11_ry01);
          _SFD_SET_DATA_VALUE_PTR(5U, c11_ry02);
          _SFD_SET_DATA_VALUE_PTR(6U, *c11_C0);
          _SFD_SET_DATA_VALUE_PTR(7U, *c11_inputBound);
          _SFD_SET_DATA_VALUE_PTR(8U, c11_Fx);
          _SFD_SET_DATA_VALUE_PTR(9U, c11_Fy);
          _SFD_SET_DATA_VALUE_PTR(10U, c11_M);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _BoatLibraryTest02MachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "JTqQg77sfMcNeodvYtBbS";
}

static void sf_opaque_initialize_c11_BoatLibraryTest02(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc11_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c11_BoatLibraryTest02((SFc11_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
  initialize_c11_BoatLibraryTest02((SFc11_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c11_BoatLibraryTest02(void *chartInstanceVar)
{
  enable_c11_BoatLibraryTest02((SFc11_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c11_BoatLibraryTest02(void *chartInstanceVar)
{
  disable_c11_BoatLibraryTest02((SFc11_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c11_BoatLibraryTest02(void *chartInstanceVar)
{
  sf_c11_BoatLibraryTest02((SFc11_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c11_BoatLibraryTest02(SimStruct*
  S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c11_BoatLibraryTest02
    ((SFc11_BoatLibraryTest02InstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c11_BoatLibraryTest02();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_raw2high'.\n");
  }

  return plhs[0];
}

extern void sf_internal_set_sim_state_c11_BoatLibraryTest02(SimStruct* S, const
  mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c11_BoatLibraryTest02();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c11_BoatLibraryTest02((SFc11_BoatLibraryTest02InstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c11_BoatLibraryTest02(SimStruct* S)
{
  return sf_internal_get_sim_state_c11_BoatLibraryTest02(S);
}

static void sf_opaque_set_sim_state_c11_BoatLibraryTest02(SimStruct* S, const
  mxArray *st)
{
  sf_internal_set_sim_state_c11_BoatLibraryTest02(S, st);
}

static void sf_opaque_terminate_c11_BoatLibraryTest02(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc11_BoatLibraryTest02InstanceStruct*) chartInstanceVar)
      ->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_BoatLibraryTest02_optimization_info();
    }

    finalize_c11_BoatLibraryTest02((SFc11_BoatLibraryTest02InstanceStruct*)
      chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc11_BoatLibraryTest02((SFc11_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c11_BoatLibraryTest02(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c11_BoatLibraryTest02
      ((SFc11_BoatLibraryTest02InstanceStruct*)(((ChartInfoStruct *)
         ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c11_BoatLibraryTest02(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_BoatLibraryTest02_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      11);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,11,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,11,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,11);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 5, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 6, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 7, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 8, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 9, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,11,10);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,11,1);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=1; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 10; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,11);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(480124130U));
  ssSetChecksum1(S,(3603559042U));
  ssSetChecksum2(S,(1944895128U));
  ssSetChecksum3(S,(3768704622U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c11_BoatLibraryTest02(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c11_BoatLibraryTest02(SimStruct *S)
{
  SFc11_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest02InstanceStruct *)utMalloc(sizeof
    (SFc11_BoatLibraryTest02InstanceStruct));
  memset(chartInstance, 0, sizeof(SFc11_BoatLibraryTest02InstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c11_BoatLibraryTest02;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c11_BoatLibraryTest02;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c11_BoatLibraryTest02;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c11_BoatLibraryTest02;
  chartInstance->chartInfo.disableChart =
    sf_opaque_disable_c11_BoatLibraryTest02;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c11_BoatLibraryTest02;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c11_BoatLibraryTest02;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c11_BoatLibraryTest02;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c11_BoatLibraryTest02;
  chartInstance->chartInfo.mdlStart = mdlStart_c11_BoatLibraryTest02;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c11_BoatLibraryTest02;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->S = S;
  ssSetUserData(S,(void *)(&(chartInstance->chartInfo)));/* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  sf_opaque_init_subchart_simstructs(chartInstance->chartInfo.chartInstance);
  chart_debug_initialization(S,1);
}

void c11_BoatLibraryTest02_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c11_BoatLibraryTest02(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c11_BoatLibraryTest02(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c11_BoatLibraryTest02(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c11_BoatLibraryTest02_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
