/* Include files */

#include <stddef.h>
#include "blas.h"
#include "BoatLibraryTest01_sfun.h"
#include "c4_BoatLibraryTest01.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "BoatLibraryTest01_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c4_debug_family_names[27] = { "rx1", "rx2", "ry1", "ry2",
  "C", "Fxref", "Fyref", "Mref", "gridNum", "grid", "u1", "k", "h", "alpha",
  "nargin", "nargout", "u0", "rx01", "rx02", "ry01", "ry02", "C0", "inputBound",
  "Fx", "Fy", "M", "output" };

static const char * c4_b_debug_family_names[10] = { "p", "theta1", "f1",
  "theta2", "f2", "gridNum", "nargin", "nargout", "inputBound", "output" };

static const char * c4_c_debug_family_names[22] = { "theta1", "f1", "theta2",
  "f2", "pretheta1", "pretheta2", "Fx", "Fy", "M", "nargin", "nargout", "input",
  "preinput", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1", "ry2", "C",
  "output" };

static const char * c4_d_debug_family_names[22] = { "theta1", "f1", "theta2",
  "f2", "pretheta1", "pretheta2", "Fx", "Fy", "M", "nargin", "nargout", "input",
  "preinput", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1", "ry2", "C",
  "output" };

static const char * c4_e_debug_family_names[22] = { "ii", "iii", "v0", "i", "j",
  "k", "l", "input", "v1", "nargin", "nargout", "grid", "preinput", "Fxref",
  "Fyref", "Mref", "rx1", "rx2", "ry1", "ry2", "C", "output" };

static const char * c4_f_debug_family_names[23] = { "theta1", "f1", "theta2",
  "f2", "pretheta1", "pretheta2", "dVdtheta1", "dVdf1", "dVdtheta2", "dVdf2",
  "nargin", "nargout", "input", "preinput", "Fxref", "Fyref", "Mref", "rx1",
  "rx2", "ry1", "ry2", "C", "output" };

static const char * c4_g_debug_family_names[20] = { "theta1", "f1", "theta2",
  "f2", "ddVddtheta1", "ddVddf1", "ddVddtheta2", "ddVddf2", "nargin", "nargout",
  "input", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1", "ry2", "C", "output" };

static const char * c4_h_debug_family_names[16] = { "hsd", "hn", "i", "nargin",
  "nargout", "input", "preinput", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1",
  "ry2", "C", "output" };

static const char * c4_i_debug_family_names[19] = { "alpha", "u0", "u1", "v0",
  "v1", "nargin", "nargout", "input", "preinput", "h", "Fxref", "Fyref", "Mref",
  "rx1", "rx2", "ry1", "ry2", "C", "output" };

static const char * c4_j_debug_family_names[5] = { "nargin", "nargout", "input",
  "bound", "output" };

static const char * c4_k_debug_family_names[7] = { "k", "i", "nargin", "nargout",
  "input", "inputBound", "output" };

/* Function Declarations */
static void initialize_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance);
static void initialize_params_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance);
static void enable_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance);
static void disable_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance);
static void c4_update_debugger_state_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance);
static const mxArray *get_sim_state_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance);
static void set_sim_state_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance, const mxArray *c4_st);
static void finalize_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance);
static void sf_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance);
static void c4_chartstep_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance);
static void initSimStructsc4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c4_machineNumber, uint32_T
  c4_chartNumber);
static const mxArray *c4_sf_marshallOut(void *chartInstanceVoid, void *c4_inData);
static void c4_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_output, const char_T *c4_identifier, real_T
  c4_y[4]);
static void c4_b_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[4]);
static void c4_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static const mxArray *c4_b_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData);
static const mxArray *c4_c_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData);
static const mxArray *c4_d_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData);
static real_T c4_c_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId);
static void c4_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static const mxArray *c4_e_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData);
static void c4_d_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[20]);
static void c4_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static void c4_e_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[7]);
static void c4_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static void c4_f_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[8]);
static void c4_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static const mxArray *c4_f_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData);
static void c4_g_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[5]);
static void c4_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static const mxArray *c4_g_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData);
static void c4_h_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[4]);
static void c4_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static const mxArray *c4_h_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData);
static void c4_i_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[2]);
static void c4_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static void c4_info_helper(const mxArray **c4_info);
static const mxArray *c4_emlrt_marshallOut(char * c4_u);
static const mxArray *c4_b_emlrt_marshallOut(uint32_T c4_u);
static void c4_eml_scalar_eg(SFc4_BoatLibraryTest01InstanceStruct *chartInstance);
static void c4_linspace(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
  real_T c4_d1, real_T c4_d2, real_T c4_y[5]);
static void c4_getMinFromGrid(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, real_T c4_grid[20], real_T c4_preinput[4], real_T c4_Fxref,
  real_T c4_Fyref, real_T c4_Mref, real_T c4_rx1, real_T c4_rx2, real_T c4_ry1,
  real_T c4_ry2, real_T c4_C[7], real_T c4_output[4]);
static real_T c4_costFunction(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, real_T c4_input[4], real_T c4_preinput[4], real_T c4_Fxref,
  real_T c4_Fyref, real_T c4_Mref, real_T c4_rx1, real_T c4_rx2, real_T c4_ry1,
  real_T c4_ry2, real_T c4_C[7]);
static real_T c4_sind(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
                      real_T c4_x);
static boolean_T c4_isfinite(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
  real_T c4_x);
static real_T c4_cosd(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
                      real_T c4_x);
static real_T c4_mpower(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
  real_T c4_a);
static real_T c4_b_costFunction(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, real_T c4_input[4], real_T c4_preinput[4], real_T c4_Fxref,
  real_T c4_Fyref, real_T c4_Mref, real_T c4_rx1, real_T c4_rx2, real_T c4_ry1,
  real_T c4_ry2, real_T c4_C[7]);
static void c4_SteepestDescent(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, real_T c4_input[4], real_T c4_preinput[4], real_T c4_Fxref,
  real_T c4_Fyref, real_T c4_Mref, real_T c4_rx1, real_T c4_rx2, real_T c4_ry1,
  real_T c4_ry2, real_T c4_C[7], real_T c4_output[4]);
static void c4_Newtons(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
  real_T c4_input[4], real_T c4_Fxref, real_T c4_Fyref, real_T c4_Mref, real_T
  c4_rx1, real_T c4_rx2, real_T c4_ry1, real_T c4_ry2, real_T c4_C[7], real_T
  c4_output[4]);
static const mxArray *c4_i_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData);
static int32_T c4_j_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId);
static void c4_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData);
static uint8_T c4_k_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_b_is_active_c4_BoatLibraryTest01, const
  char_T *c4_identifier);
static uint8_T c4_l_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId);
static void c4_b_sind(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
                      real_T *c4_x);
static void c4_b_cosd(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
                      real_T *c4_x);
static void init_dsm_address_info(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
  chartInstance->c4_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c4_is_active_c4_BoatLibraryTest01 = 0U;
}

static void initialize_params_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance)
{
}

static void enable_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c4_update_debugger_state_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance)
{
}

static const mxArray *get_sim_state_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance)
{
  const mxArray *c4_st;
  const mxArray *c4_y = NULL;
  int32_T c4_i0;
  real_T c4_u[4];
  const mxArray *c4_b_y = NULL;
  uint8_T c4_hoistedGlobal;
  uint8_T c4_b_u;
  const mxArray *c4_c_y = NULL;
  real_T (*c4_output)[4];
  c4_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c4_st = NULL;
  c4_st = NULL;
  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_createcellarray(2), FALSE);
  for (c4_i0 = 0; c4_i0 < 4; c4_i0++) {
    c4_u[c4_i0] = (*c4_output)[c4_i0];
  }

  c4_b_y = NULL;
  sf_mex_assign(&c4_b_y, sf_mex_create("y", c4_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_setcell(c4_y, 0, c4_b_y);
  c4_hoistedGlobal = chartInstance->c4_is_active_c4_BoatLibraryTest01;
  c4_b_u = c4_hoistedGlobal;
  c4_c_y = NULL;
  sf_mex_assign(&c4_c_y, sf_mex_create("y", &c4_b_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c4_y, 1, c4_c_y);
  sf_mex_assign(&c4_st, c4_y, FALSE);
  return c4_st;
}

static void set_sim_state_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance, const mxArray *c4_st)
{
  const mxArray *c4_u;
  real_T c4_dv0[4];
  int32_T c4_i1;
  real_T (*c4_output)[4];
  c4_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c4_doneDoubleBufferReInit = TRUE;
  c4_u = sf_mex_dup(c4_st);
  c4_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c4_u, 0)),
                      "output", c4_dv0);
  for (c4_i1 = 0; c4_i1 < 4; c4_i1++) {
    (*c4_output)[c4_i1] = c4_dv0[c4_i1];
  }

  chartInstance->c4_is_active_c4_BoatLibraryTest01 = c4_k_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c4_u, 1)),
     "is_active_c4_BoatLibraryTest01");
  sf_mex_destroy(&c4_u);
  c4_update_debugger_state_c4_BoatLibraryTest01(chartInstance);
  sf_mex_destroy(&c4_st);
}

static void finalize_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
}

static void sf_c4_BoatLibraryTest01(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
  int32_T c4_i2;
  int32_T c4_i3;
  int32_T c4_i4;
  int32_T c4_i5;
  real_T *c4_rx01;
  real_T *c4_rx02;
  real_T *c4_ry01;
  real_T *c4_ry02;
  real_T *c4_Fx;
  real_T *c4_Fy;
  real_T *c4_M;
  real_T (*c4_inputBound)[8];
  real_T (*c4_C0)[7];
  real_T (*c4_output)[4];
  real_T (*c4_u0)[4];
  c4_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c4_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c4_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c4_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S, 6);
  c4_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
  c4_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c4_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c4_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c4_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c4_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c4_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 3U, chartInstance->c4_sfEvent);
  for (c4_i2 = 0; c4_i2 < 4; c4_i2++) {
    _SFD_DATA_RANGE_CHECK((*c4_u0)[c4_i2], 0U);
  }

  for (c4_i3 = 0; c4_i3 < 4; c4_i3++) {
    _SFD_DATA_RANGE_CHECK((*c4_output)[c4_i3], 1U);
  }

  _SFD_DATA_RANGE_CHECK(*c4_rx01, 2U);
  _SFD_DATA_RANGE_CHECK(*c4_rx02, 3U);
  _SFD_DATA_RANGE_CHECK(*c4_ry01, 4U);
  _SFD_DATA_RANGE_CHECK(*c4_ry02, 5U);
  for (c4_i4 = 0; c4_i4 < 7; c4_i4++) {
    _SFD_DATA_RANGE_CHECK((*c4_C0)[c4_i4], 6U);
  }

  for (c4_i5 = 0; c4_i5 < 8; c4_i5++) {
    _SFD_DATA_RANGE_CHECK((*c4_inputBound)[c4_i5], 7U);
  }

  _SFD_DATA_RANGE_CHECK(*c4_Fx, 8U);
  _SFD_DATA_RANGE_CHECK(*c4_Fy, 9U);
  _SFD_DATA_RANGE_CHECK(*c4_M, 10U);
  chartInstance->c4_sfEvent = CALL_EVENT;
  c4_chartstep_c4_BoatLibraryTest01(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_BoatLibraryTest01MachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c4_chartstep_c4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance)
{
  real_T c4_hoistedGlobal;
  real_T c4_b_hoistedGlobal;
  real_T c4_c_hoistedGlobal;
  real_T c4_d_hoistedGlobal;
  real_T c4_e_hoistedGlobal;
  real_T c4_f_hoistedGlobal;
  real_T c4_g_hoistedGlobal;
  int32_T c4_i6;
  real_T c4_u0[4];
  real_T c4_rx01;
  real_T c4_rx02;
  real_T c4_ry01;
  real_T c4_ry02;
  int32_T c4_i7;
  real_T c4_C0[7];
  int32_T c4_i8;
  real_T c4_inputBound[8];
  real_T c4_Fx;
  real_T c4_Fy;
  real_T c4_M;
  uint32_T c4_debug_family_var_map[27];
  real_T c4_rx1;
  real_T c4_rx2;
  real_T c4_ry1;
  real_T c4_ry2;
  real_T c4_C[7];
  real_T c4_Fxref;
  real_T c4_Fyref;
  real_T c4_Mref;
  real_T c4_gridNum;
  real_T c4_grid[20];
  real_T c4_u1[4];
  real_T c4_k;
  real_T c4_h[4];
  real_T c4_alpha;
  real_T c4_nargin = 10.0;
  real_T c4_nargout = 1.0;
  real_T c4_output[4];
  int32_T c4_i9;
  int32_T c4_i10;
  real_T c4_b_inputBound[8];
  uint32_T c4_b_debug_family_var_map[10];
  real_T c4_p;
  real_T c4_theta1[5];
  real_T c4_f1[5];
  real_T c4_theta2[5];
  real_T c4_f2[5];
  real_T c4_b_gridNum;
  real_T c4_b_nargin = 2.0;
  real_T c4_b_nargout = 1.0;
  real_T c4_dv1[5];
  int32_T c4_i11;
  int32_T c4_i12;
  int32_T c4_i13;
  int32_T c4_i14;
  int32_T c4_i15;
  int32_T c4_i16;
  int32_T c4_i17;
  int32_T c4_i18;
  int32_T c4_i19;
  real_T c4_b_grid[20];
  int32_T c4_i20;
  real_T c4_b_u0[4];
  int32_T c4_i21;
  real_T c4_b_C[7];
  real_T c4_dv2[4];
  int32_T c4_i22;
  int32_T c4_b_k;
  int32_T c4_i23;
  real_T c4_input[4];
  int32_T c4_i24;
  real_T c4_preinput[4];
  real_T c4_b_Fxref;
  real_T c4_b_Fyref;
  real_T c4_b_Mref;
  real_T c4_b_rx1;
  real_T c4_b_rx2;
  real_T c4_b_ry1;
  real_T c4_b_ry2;
  int32_T c4_i25;
  real_T c4_c_C[7];
  uint32_T c4_c_debug_family_var_map[16];
  real_T c4_hsd[4];
  real_T c4_hn[4];
  real_T c4_i;
  real_T c4_c_nargin = 10.0;
  real_T c4_c_nargout = 1.0;
  int32_T c4_i26;
  real_T c4_b_input[4];
  int32_T c4_i27;
  real_T c4_b_preinput[4];
  int32_T c4_i28;
  real_T c4_d_C[7];
  real_T c4_dv3[4];
  int32_T c4_i29;
  int32_T c4_i30;
  real_T c4_c_input[4];
  int32_T c4_i31;
  real_T c4_e_C[7];
  real_T c4_dv4[4];
  int32_T c4_i32;
  int32_T c4_i33;
  int32_T c4_b_i;
  real_T c4_x;
  real_T c4_b_x;
  int32_T c4_i34;
  real_T c4_d_input[4];
  int32_T c4_i35;
  real_T c4_c_preinput[4];
  int32_T c4_i36;
  real_T c4_b_h[4];
  real_T c4_c_Fxref;
  real_T c4_c_Fyref;
  real_T c4_c_Mref;
  real_T c4_c_rx1;
  real_T c4_c_rx2;
  real_T c4_c_ry1;
  real_T c4_c_ry2;
  int32_T c4_i37;
  real_T c4_f_C[7];
  uint32_T c4_d_debug_family_var_map[19];
  real_T c4_b_alpha;
  real_T c4_c_u0[4];
  real_T c4_b_u1[4];
  real_T c4_v0;
  real_T c4_v1;
  real_T c4_d_nargin = 11.0;
  real_T c4_d_nargout = 1.0;
  int32_T c4_i38;
  int32_T c4_i39;
  real_T c4_b[4];
  int32_T c4_i40;
  int32_T c4_i41;
  real_T c4_d_u0[4];
  int32_T c4_i42;
  real_T c4_d_preinput[4];
  int32_T c4_i43;
  real_T c4_g_C[7];
  int32_T c4_i44;
  real_T c4_c_u1[4];
  int32_T c4_i45;
  real_T c4_e_preinput[4];
  int32_T c4_i46;
  real_T c4_h_C[7];
  real_T c4_a;
  int32_T c4_i47;
  int32_T c4_i48;
  int32_T c4_i49;
  int32_T c4_i50;
  real_T c4_d_u1[4];
  int32_T c4_i51;
  real_T c4_f_preinput[4];
  int32_T c4_i52;
  real_T c4_i_C[7];
  real_T c4_b_a;
  int32_T c4_i53;
  int32_T c4_i54;
  int32_T c4_i55;
  int32_T c4_i56;
  real_T c4_e_u1[4];
  int32_T c4_i57;
  real_T c4_g_preinput[4];
  int32_T c4_i58;
  real_T c4_j_C[7];
  real_T c4_c_a;
  int32_T c4_i59;
  int32_T c4_i60;
  int32_T c4_i61;
  int32_T c4_i62;
  real_T c4_e_input[4];
  int32_T c4_i63;
  real_T c4_c_inputBound[8];
  uint32_T c4_e_debug_family_var_map[7];
  real_T c4_c_k[4];
  real_T c4_c_i;
  real_T c4_e_nargin = 2.0;
  real_T c4_e_nargout = 1.0;
  int32_T c4_i64;
  int32_T c4_d_i;
  real_T c4_f_input;
  int32_T c4_e_i;
  int32_T c4_i65;
  real_T c4_bound[2];
  uint32_T c4_f_debug_family_var_map[5];
  real_T c4_f_nargin = 2.0;
  real_T c4_f_nargout = 1.0;
  real_T c4_b_output;
  int32_T c4_i66;
  int32_T c4_i67;
  int32_T c4_i68;
  real_T *c4_b_rx01;
  real_T *c4_b_rx02;
  real_T *c4_b_ry01;
  real_T *c4_b_ry02;
  real_T *c4_b_Fx;
  real_T *c4_b_Fy;
  real_T *c4_b_M;
  real_T (*c4_c_output)[4];
  real_T (*c4_d_inputBound)[8];
  real_T (*c4_b_C0)[7];
  real_T (*c4_e_u0)[4];
  boolean_T guard1 = FALSE;
  int32_T exitg1;
  boolean_T exitg2;
  c4_b_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c4_b_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c4_b_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c4_d_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S, 6);
  c4_b_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
  c4_b_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c4_b_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c4_b_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c4_b_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c4_c_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c4_e_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 3U, chartInstance->c4_sfEvent);
  c4_hoistedGlobal = *c4_b_rx01;
  c4_b_hoistedGlobal = *c4_b_rx02;
  c4_c_hoistedGlobal = *c4_b_ry01;
  c4_d_hoistedGlobal = *c4_b_ry02;
  c4_e_hoistedGlobal = *c4_b_Fx;
  c4_f_hoistedGlobal = *c4_b_Fy;
  c4_g_hoistedGlobal = *c4_b_M;
  for (c4_i6 = 0; c4_i6 < 4; c4_i6++) {
    c4_u0[c4_i6] = (*c4_e_u0)[c4_i6];
  }

  c4_rx01 = c4_hoistedGlobal;
  c4_rx02 = c4_b_hoistedGlobal;
  c4_ry01 = c4_c_hoistedGlobal;
  c4_ry02 = c4_d_hoistedGlobal;
  for (c4_i7 = 0; c4_i7 < 7; c4_i7++) {
    c4_C0[c4_i7] = (*c4_b_C0)[c4_i7];
  }

  for (c4_i8 = 0; c4_i8 < 8; c4_i8++) {
    c4_inputBound[c4_i8] = (*c4_d_inputBound)[c4_i8];
  }

  c4_Fx = c4_e_hoistedGlobal;
  c4_Fy = c4_f_hoistedGlobal;
  c4_M = c4_g_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 27U, 27U, c4_debug_family_names,
    c4_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx1, 0U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx2, 1U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry1, 2U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry2, 3U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_C, 4U, c4_d_sf_marshallOut,
    c4_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fxref, 5U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fyref, 6U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Mref, 7U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_gridNum, 8U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_grid, 9U, c4_e_sf_marshallOut,
    c4_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_u1, 10U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_k, 11U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_h, 12U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_alpha, 13U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargin, 14U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargout, 15U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c4_u0, 16U, c4_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_rx01, 17U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_rx02, 18U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_ry01, 19U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_ry02, 20U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c4_C0, 21U, c4_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c4_inputBound, 22U, c4_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_Fx, 23U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_Fy, 24U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_M, 25U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_output, 26U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 7);
  c4_rx1 = c4_rx01;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 7);
  c4_rx2 = c4_rx02;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 8);
  c4_ry1 = c4_ry01;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 8);
  c4_ry2 = c4_ry02;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 10);
  for (c4_i9 = 0; c4_i9 < 7; c4_i9++) {
    c4_C[c4_i9] = c4_C0[c4_i9];
  }

  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 12);
  c4_Fxref = c4_Fx;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 12);
  c4_Fyref = c4_Fy;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 12);
  c4_Mref = c4_M;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 14);
  c4_gridNum = 5.0;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 16);
  for (c4_i10 = 0; c4_i10 < 8; c4_i10++) {
    c4_b_inputBound[c4_i10] = c4_inputBound[c4_i10];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 10U, 10U, c4_b_debug_family_names,
    c4_b_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_p, 0U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_theta1, 1U, c4_f_sf_marshallOut,
    c4_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_f1, 2U, c4_f_sf_marshallOut,
    c4_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_theta2, 3U, c4_f_sf_marshallOut,
    c4_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_f2, 4U, c4_f_sf_marshallOut,
    c4_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_b_gridNum, 5U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_nargin, 6U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_nargout, 7U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_b_inputBound, 8U, c4_c_sf_marshallOut,
    c4_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_grid, 9U, c4_e_sf_marshallOut,
    c4_c_sf_marshallIn);
  c4_b_gridNum = 5.0;
  CV_EML_FCN(0, 3);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 67);
  c4_p = 5.0;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 69);
  c4_linspace(chartInstance, c4_b_inputBound[0], c4_b_inputBound[4], c4_dv1);
  for (c4_i11 = 0; c4_i11 < 5; c4_i11++) {
    c4_theta1[c4_i11] = c4_dv1[c4_i11];
  }

  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 70);
  c4_linspace(chartInstance, c4_b_inputBound[1], c4_b_inputBound[5], c4_dv1);
  for (c4_i12 = 0; c4_i12 < 5; c4_i12++) {
    c4_f1[c4_i12] = c4_dv1[c4_i12];
  }

  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 71);
  c4_linspace(chartInstance, c4_b_inputBound[2], c4_b_inputBound[6], c4_dv1);
  for (c4_i13 = 0; c4_i13 < 5; c4_i13++) {
    c4_theta2[c4_i13] = c4_dv1[c4_i13];
  }

  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 72);
  c4_linspace(chartInstance, c4_b_inputBound[3], c4_b_inputBound[7], c4_dv1);
  for (c4_i14 = 0; c4_i14 < 5; c4_i14++) {
    c4_f2[c4_i14] = c4_dv1[c4_i14];
  }

  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 74);
  for (c4_i15 = 0; c4_i15 < 5; c4_i15++) {
    c4_grid[c4_i15] = c4_theta1[c4_i15];
  }

  for (c4_i16 = 0; c4_i16 < 5; c4_i16++) {
    c4_grid[c4_i16 + 5] = c4_f1[c4_i16];
  }

  for (c4_i17 = 0; c4_i17 < 5; c4_i17++) {
    c4_grid[c4_i17 + 10] = c4_theta2[c4_i17];
  }

  for (c4_i18 = 0; c4_i18 < 5; c4_i18++) {
    c4_grid[c4_i18 + 15] = c4_f2[c4_i18];
  }

  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -74);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 18);
  for (c4_i19 = 0; c4_i19 < 20; c4_i19++) {
    c4_b_grid[c4_i19] = c4_grid[c4_i19];
  }

  for (c4_i20 = 0; c4_i20 < 4; c4_i20++) {
    c4_b_u0[c4_i20] = c4_u0[c4_i20];
  }

  for (c4_i21 = 0; c4_i21 < 7; c4_i21++) {
    c4_b_C[c4_i21] = c4_C[c4_i21];
  }

  c4_getMinFromGrid(chartInstance, c4_b_grid, c4_b_u0, c4_Fxref, c4_Fyref,
                    c4_Mref, c4_rx1, c4_rx2, c4_ry1, c4_ry2, c4_b_C, c4_dv2);
  for (c4_i22 = 0; c4_i22 < 4; c4_i22++) {
    c4_u1[c4_i22] = c4_dv2[c4_i22];
  }

  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 20);
  c4_k = 1.0;
  c4_b_k = 0;
  while (c4_b_k < 10) {
    c4_k = 1.0 + (real_T)c4_b_k;
    CV_EML_FOR(0, 1, 0, 1);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 21);
    for (c4_i23 = 0; c4_i23 < 4; c4_i23++) {
      c4_input[c4_i23] = c4_u1[c4_i23];
    }

    for (c4_i24 = 0; c4_i24 < 4; c4_i24++) {
      c4_preinput[c4_i24] = c4_u0[c4_i24];
    }

    c4_b_Fxref = c4_Fxref;
    c4_b_Fyref = c4_Fyref;
    c4_b_Mref = c4_Mref;
    c4_b_rx1 = c4_rx1;
    c4_b_rx2 = c4_rx2;
    c4_b_ry1 = c4_ry1;
    c4_b_ry2 = c4_ry2;
    for (c4_i25 = 0; c4_i25 < 7; c4_i25++) {
      c4_c_C[c4_i25] = c4_C[c4_i25];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 16U, 16U, c4_h_debug_family_names,
      c4_c_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_hsd, 0U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_hn, 1U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_i, 2U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_nargin, 3U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_nargout, 4U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_input, 5U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_preinput, 6U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_Fxref, 7U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_Fyref, 8U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_Mref, 9U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_rx1, 10U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_rx2, 11U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_ry1, 12U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_ry2, 13U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_c_C, 14U, c4_d_sf_marshallOut,
      c4_d_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_h, 15U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    CV_EML_FCN(0, 7);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 156U);
    for (c4_i26 = 0; c4_i26 < 4; c4_i26++) {
      c4_b_input[c4_i26] = c4_input[c4_i26];
    }

    for (c4_i27 = 0; c4_i27 < 4; c4_i27++) {
      c4_b_preinput[c4_i27] = c4_preinput[c4_i27];
    }

    for (c4_i28 = 0; c4_i28 < 7; c4_i28++) {
      c4_d_C[c4_i28] = c4_c_C[c4_i28];
    }

    c4_SteepestDescent(chartInstance, c4_b_input, c4_b_preinput, c4_b_Fxref,
                       c4_b_Fyref, c4_b_Mref, c4_b_rx1, c4_b_rx2, c4_b_ry1,
                       c4_b_ry2, c4_d_C, c4_dv3);
    for (c4_i29 = 0; c4_i29 < 4; c4_i29++) {
      c4_hsd[c4_i29] = c4_dv3[c4_i29];
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 157U);
    for (c4_i30 = 0; c4_i30 < 4; c4_i30++) {
      c4_c_input[c4_i30] = c4_input[c4_i30];
    }

    for (c4_i31 = 0; c4_i31 < 7; c4_i31++) {
      c4_e_C[c4_i31] = c4_c_C[c4_i31];
    }

    c4_Newtons(chartInstance, c4_c_input, c4_b_Fxref, c4_b_Fyref, c4_b_Mref,
               c4_b_rx1, c4_b_rx2, c4_b_ry1, c4_b_ry2, c4_e_C, c4_dv4);
    for (c4_i32 = 0; c4_i32 < 4; c4_i32++) {
      c4_hn[c4_i32] = c4_dv4[c4_i32];
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 159U);
    for (c4_i33 = 0; c4_i33 < 4; c4_i33++) {
      c4_h[c4_i33] = 0.0;
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 161U);
    c4_i = 1.0;
    c4_b_i = 0;
    while (c4_b_i < 4) {
      c4_i = 1.0 + (real_T)c4_b_i;
      CV_EML_FOR(0, 1, 6, 1);
      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 163U);
      c4_x = c4_hn[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("hn", (int32_T)
        _SFD_INTEGER_CHECK("i", c4_i), 1, 4, 1, 0) - 1];
      c4_b_x = c4_x;
      c4_b_x = muDoubleScalarSign(c4_b_x);
      if (CV_EML_IF(0, 1, 5, c4_b_x > 0.0)) {
        _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 164U);
        c4_h[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("output", (int32_T)
          _SFD_INTEGER_CHECK("i", c4_i), 1, 4, 1, 0) - 1] = c4_hsd[(int32_T)
          (real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("hsd", (int32_T)_SFD_INTEGER_CHECK
          ("i", c4_i), 1, 4, 1, 0) - 1];
      } else {
        _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 166U);
        c4_h[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("output", (int32_T)
          _SFD_INTEGER_CHECK("i", c4_i), 1, 4, 1, 0) - 1] = c4_hsd[(int32_T)
          (real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("hsd", (int32_T)_SFD_INTEGER_CHECK
          ("i", c4_i), 1, 4, 1, 0) - 1];
      }

      c4_b_i++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 6, 0);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -166);
    _SFD_SYMBOL_SCOPE_POP();
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 22);
    for (c4_i34 = 0; c4_i34 < 4; c4_i34++) {
      c4_d_input[c4_i34] = c4_u1[c4_i34];
    }

    for (c4_i35 = 0; c4_i35 < 4; c4_i35++) {
      c4_c_preinput[c4_i35] = c4_u0[c4_i35];
    }

    for (c4_i36 = 0; c4_i36 < 4; c4_i36++) {
      c4_b_h[c4_i36] = c4_h[c4_i36];
    }

    c4_c_Fxref = c4_Fxref;
    c4_c_Fyref = c4_Fyref;
    c4_c_Mref = c4_Mref;
    c4_c_rx1 = c4_rx1;
    c4_c_rx2 = c4_rx2;
    c4_c_ry1 = c4_ry1;
    c4_c_ry2 = c4_ry2;
    for (c4_i37 = 0; c4_i37 < 7; c4_i37++) {
      c4_f_C[c4_i37] = c4_C[c4_i37];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 19U, 19U, c4_i_debug_family_names,
      c4_d_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML(&c4_b_alpha, 0U, c4_b_sf_marshallOut);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_c_u0, 1U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_b_u1, 2U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_v0, 3U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_v1, 4U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_d_nargin, 5U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_d_nargout, 6U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_d_input, 7U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_c_preinput, 8U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_b_h, 9U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_Fxref, 10U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_Fyref, 11U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_Mref, 12U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_rx1, 13U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_rx2, 14U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_ry1, 15U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_ry2, 16U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_f_C, 17U, c4_d_sf_marshallOut,
      c4_d_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_alpha, 18U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    CV_EML_FCN(0, 5);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 94);
    c4_b_alpha = 1.0;
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 96);
    for (c4_i38 = 0; c4_i38 < 4; c4_i38++) {
      c4_c_u0[c4_i38] = c4_d_input[c4_i38];
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 97);
    for (c4_i39 = 0; c4_i39 < 4; c4_i39++) {
      c4_b[c4_i39] = c4_b_h[c4_i39];
    }

    for (c4_i40 = 0; c4_i40 < 4; c4_i40++) {
      c4_b_u1[c4_i40] = c4_d_input[c4_i40] + c4_b[c4_i40];
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 99);
    for (c4_i41 = 0; c4_i41 < 4; c4_i41++) {
      c4_d_u0[c4_i41] = c4_c_u0[c4_i41];
    }

    for (c4_i42 = 0; c4_i42 < 4; c4_i42++) {
      c4_d_preinput[c4_i42] = c4_c_preinput[c4_i42];
    }

    for (c4_i43 = 0; c4_i43 < 7; c4_i43++) {
      c4_g_C[c4_i43] = c4_f_C[c4_i43];
    }

    c4_v0 = c4_b_costFunction(chartInstance, c4_d_u0, c4_d_preinput, c4_c_Fxref,
      c4_c_Fyref, c4_c_Mref, c4_c_rx1, c4_c_rx2, c4_c_ry1, c4_c_ry2, c4_g_C);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 100);
    for (c4_i44 = 0; c4_i44 < 4; c4_i44++) {
      c4_c_u1[c4_i44] = c4_b_u1[c4_i44];
    }

    for (c4_i45 = 0; c4_i45 < 4; c4_i45++) {
      c4_e_preinput[c4_i45] = c4_c_preinput[c4_i45];
    }

    for (c4_i46 = 0; c4_i46 < 7; c4_i46++) {
      c4_h_C[c4_i46] = c4_f_C[c4_i46];
    }

    c4_v1 = c4_b_costFunction(chartInstance, c4_c_u1, c4_e_preinput, c4_c_Fxref,
      c4_c_Fyref, c4_c_Mref, c4_c_rx1, c4_c_rx2, c4_c_ry1, c4_c_ry2, c4_h_C);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 102);
    exitg2 = FALSE;
    while ((exitg2 == FALSE) && CV_EML_WHILE(0, 1, 0, c4_v1 < c4_v0)) {
      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 103);
      c4_b_alpha++;
      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 104);
      c4_a = c4_b_alpha;
      for (c4_i47 = 0; c4_i47 < 4; c4_i47++) {
        c4_b[c4_i47] = c4_b_h[c4_i47];
      }

      for (c4_i48 = 0; c4_i48 < 4; c4_i48++) {
        c4_b[c4_i48] *= c4_a;
      }

      for (c4_i49 = 0; c4_i49 < 4; c4_i49++) {
        c4_b_u1[c4_i49] = c4_d_input[c4_i49] + c4_b[c4_i49];
      }

      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 105);
      c4_v0 = c4_v1;
      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 106);
      for (c4_i50 = 0; c4_i50 < 4; c4_i50++) {
        c4_d_u1[c4_i50] = c4_b_u1[c4_i50];
      }

      for (c4_i51 = 0; c4_i51 < 4; c4_i51++) {
        c4_f_preinput[c4_i51] = c4_c_preinput[c4_i51];
      }

      for (c4_i52 = 0; c4_i52 < 7; c4_i52++) {
        c4_i_C[c4_i52] = c4_f_C[c4_i52];
      }

      c4_v1 = c4_b_costFunction(chartInstance, c4_d_u1, c4_f_preinput,
        c4_c_Fxref, c4_c_Fyref, c4_c_Mref, c4_c_rx1, c4_c_rx2, c4_c_ry1,
        c4_c_ry2, c4_i_C);
      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 107);
      if (CV_EML_IF(0, 1, 3, c4_v1 != 0.0)) {
        _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 108);
        exitg2 = TRUE;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 102);
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      }
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 112);
    guard1 = FALSE;
    do {
      exitg1 = 0;
      if (c4_v1 > c4_v0) {
        if (c4_b_alpha > 0.0) {
          CV_EML_WHILE(0, 1, 1, TRUE);
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 113);
          c4_b_alpha -= 0.01;
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 114);
          c4_b_a = c4_b_alpha;
          for (c4_i53 = 0; c4_i53 < 4; c4_i53++) {
            c4_b[c4_i53] = c4_b_h[c4_i53];
          }

          for (c4_i54 = 0; c4_i54 < 4; c4_i54++) {
            c4_b[c4_i54] *= c4_b_a;
          }

          for (c4_i55 = 0; c4_i55 < 4; c4_i55++) {
            c4_b_u1[c4_i55] = c4_d_input[c4_i55] + c4_b[c4_i55];
          }

          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 115);
          c4_v0 = c4_v1;
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 116);
          for (c4_i56 = 0; c4_i56 < 4; c4_i56++) {
            c4_e_u1[c4_i56] = c4_b_u1[c4_i56];
          }

          for (c4_i57 = 0; c4_i57 < 4; c4_i57++) {
            c4_g_preinput[c4_i57] = c4_c_preinput[c4_i57];
          }

          for (c4_i58 = 0; c4_i58 < 7; c4_i58++) {
            c4_j_C[c4_i58] = c4_f_C[c4_i58];
          }

          c4_v1 = c4_b_costFunction(chartInstance, c4_e_u1, c4_g_preinput,
            c4_c_Fxref, c4_c_Fyref, c4_c_Mref, c4_c_rx1, c4_c_rx2, c4_c_ry1,
            c4_c_ry2, c4_j_C);
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 117);
          if (CV_EML_IF(0, 1, 4, c4_v1 != 0.0)) {
            _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 118);
            exitg1 = 1;
          } else {
            _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 112);
            _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
            guard1 = FALSE;
          }
        } else {
          guard1 = TRUE;
          exitg1 = 1;
        }
      } else {
        guard1 = TRUE;
        exitg1 = 1;
      }
    } while (exitg1 == 0);

    if (guard1 == TRUE) {
      CV_EML_WHILE(0, 1, 1, FALSE);
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 122);
    c4_alpha = c4_b_alpha;
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -122);
    _SFD_SYMBOL_SCOPE_POP();
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 23);
    c4_c_a = c4_alpha;
    for (c4_i59 = 0; c4_i59 < 4; c4_i59++) {
      c4_b[c4_i59] = c4_h[c4_i59];
    }

    for (c4_i60 = 0; c4_i60 < 4; c4_i60++) {
      c4_b[c4_i60] *= c4_c_a;
    }

    for (c4_i61 = 0; c4_i61 < 4; c4_i61++) {
      c4_u1[c4_i61] += c4_b[c4_i61];
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 24);
    for (c4_i62 = 0; c4_i62 < 4; c4_i62++) {
      c4_e_input[c4_i62] = c4_u1[c4_i62];
    }

    for (c4_i63 = 0; c4_i63 < 8; c4_i63++) {
      c4_c_inputBound[c4_i63] = c4_inputBound[c4_i63];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 7U, 7U, c4_k_debug_family_names,
      c4_e_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_c_k, 0U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_c_i, 1U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_e_nargin, 2U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_e_nargout, 3U, c4_b_sf_marshallOut,
      c4_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_e_input, 4U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_c_inputBound, 5U,
      c4_c_sf_marshallOut, c4_e_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_u1, 6U, c4_sf_marshallOut,
      c4_sf_marshallIn);
    CV_EML_FCN(0, 2);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 58);
    for (c4_i64 = 0; c4_i64 < 4; c4_i64++) {
      c4_c_k[c4_i64] = 0.0;
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 59);
    c4_c_i = 1.0;
    c4_d_i = 0;
    while (c4_d_i < 4) {
      c4_c_i = 1.0 + (real_T)c4_d_i;
      CV_EML_FOR(0, 1, 5, 1);
      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 60);
      c4_f_input = c4_e_input[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
        "input", (int32_T)_SFD_INTEGER_CHECK("i", c4_c_i), 1, 4, 1, 0) - 1];
      c4_e_i = (int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("inputBound",
        (int32_T)_SFD_INTEGER_CHECK("i", c4_c_i), 1, 4, 1, 0) - 1;
      for (c4_i65 = 0; c4_i65 < 2; c4_i65++) {
        c4_bound[c4_i65] = c4_c_inputBound[c4_e_i + (c4_i65 << 2)];
      }

      _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 5U, 5U, c4_j_debug_family_names,
        c4_f_debug_family_var_map);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f_nargin, 0U, c4_b_sf_marshallOut,
        c4_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f_nargout, 1U,
        c4_b_sf_marshallOut, c4_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f_input, 2U, c4_b_sf_marshallOut,
        c4_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_bound, 3U, c4_h_sf_marshallOut,
        c4_h_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_b_output, 4U, c4_b_sf_marshallOut,
        c4_b_sf_marshallIn);
      CV_EML_FCN(0, 4);
      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 80);
      if (CV_EML_IF(0, 1, 1, c4_f_input < c4_bound[0])) {
        _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 81);
        c4_b_output = c4_bound[0];
      } else {
        _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 82);
        if (CV_EML_IF(0, 1, 2, c4_f_input > c4_bound[1])) {
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 83);
          c4_b_output = c4_bound[1];
        } else {
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 85);
          c4_b_output = c4_f_input;
        }
      }

      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -85);
      _SFD_SYMBOL_SCOPE_POP();
      c4_c_k[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("k", (int32_T)
        _SFD_INTEGER_CHECK("i", c4_c_i), 1, 4, 1, 0) - 1] = c4_b_output;
      c4_d_i++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 5, 0);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 62);
    for (c4_i66 = 0; c4_i66 < 4; c4_i66++) {
      c4_u1[c4_i66] = c4_c_k[c4_i66];
    }

    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -62);
    _SFD_SYMBOL_SCOPE_POP();
    c4_b_k++;
    _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
  }

  CV_EML_FOR(0, 1, 0, 0);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 27);
  for (c4_i67 = 0; c4_i67 < 4; c4_i67++) {
    c4_output[c4_i67] = c4_u1[c4_i67];
  }

  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -27);
  _SFD_SYMBOL_SCOPE_POP();
  for (c4_i68 = 0; c4_i68 < 4; c4_i68++) {
    (*c4_c_output)[c4_i68] = c4_output[c4_i68];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 3U, chartInstance->c4_sfEvent);
}

static void initSimStructsc4_BoatLibraryTest01
  (SFc4_BoatLibraryTest01InstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c4_machineNumber, uint32_T
  c4_chartNumber)
{
}

static const mxArray *c4_sf_marshallOut(void *chartInstanceVoid, void *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  int32_T c4_i69;
  real_T c4_b_inData[4];
  int32_T c4_i70;
  real_T c4_u[4];
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  for (c4_i69 = 0; c4_i69 < 4; c4_i69++) {
    c4_b_inData[c4_i69] = (*(real_T (*)[4])c4_inData)[c4_i69];
  }

  for (c4_i70 = 0; c4_i70 < 4; c4_i70++) {
    c4_u[c4_i70] = c4_b_inData[c4_i70];
  }

  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", c4_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static void c4_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_output, const char_T *c4_identifier, real_T
  c4_y[4])
{
  emlrtMsgIdentifier c4_thisId;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_output), &c4_thisId, c4_y);
  sf_mex_destroy(&c4_output);
}

static void c4_b_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[4])
{
  real_T c4_dv5[4];
  int32_T c4_i71;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), c4_dv5, 1, 0, 0U, 1, 0U, 1, 4);
  for (c4_i71 = 0; c4_i71 < 4; c4_i71++) {
    c4_y[c4_i71] = c4_dv5[c4_i71];
  }

  sf_mex_destroy(&c4_u);
}

static void c4_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_output;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  real_T c4_y[4];
  int32_T c4_i72;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_output = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_output), &c4_thisId, c4_y);
  sf_mex_destroy(&c4_output);
  for (c4_i72 = 0; c4_i72 < 4; c4_i72++) {
    (*(real_T (*)[4])c4_outData)[c4_i72] = c4_y[c4_i72];
  }

  sf_mex_destroy(&c4_mxArrayInData);
}

static const mxArray *c4_b_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  real_T c4_u;
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  c4_u = *(real_T *)c4_inData;
  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", &c4_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static const mxArray *c4_c_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  int32_T c4_i73;
  int32_T c4_i74;
  int32_T c4_i75;
  real_T c4_b_inData[8];
  int32_T c4_i76;
  int32_T c4_i77;
  int32_T c4_i78;
  real_T c4_u[8];
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  c4_i73 = 0;
  for (c4_i74 = 0; c4_i74 < 2; c4_i74++) {
    for (c4_i75 = 0; c4_i75 < 4; c4_i75++) {
      c4_b_inData[c4_i75 + c4_i73] = (*(real_T (*)[8])c4_inData)[c4_i75 + c4_i73];
    }

    c4_i73 += 4;
  }

  c4_i76 = 0;
  for (c4_i77 = 0; c4_i77 < 2; c4_i77++) {
    for (c4_i78 = 0; c4_i78 < 4; c4_i78++) {
      c4_u[c4_i78 + c4_i76] = c4_b_inData[c4_i78 + c4_i76];
    }

    c4_i76 += 4;
  }

  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", c4_u, 0, 0U, 1U, 0U, 2, 4, 2), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static const mxArray *c4_d_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  int32_T c4_i79;
  real_T c4_b_inData[7];
  int32_T c4_i80;
  real_T c4_u[7];
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  for (c4_i79 = 0; c4_i79 < 7; c4_i79++) {
    c4_b_inData[c4_i79] = (*(real_T (*)[7])c4_inData)[c4_i79];
  }

  for (c4_i80 = 0; c4_i80 < 7; c4_i80++) {
    c4_u[c4_i80] = c4_b_inData[c4_i80];
  }

  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", c4_u, 0, 0U, 1U, 0U, 1, 7), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static real_T c4_c_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId)
{
  real_T c4_y;
  real_T c4_d0;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), &c4_d0, 1, 0, 0U, 0, 0U, 0);
  c4_y = c4_d0;
  sf_mex_destroy(&c4_u);
  return c4_y;
}

static void c4_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_nargout;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  real_T c4_y;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_nargout = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_y = c4_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_nargout), &c4_thisId);
  sf_mex_destroy(&c4_nargout);
  *(real_T *)c4_outData = c4_y;
  sf_mex_destroy(&c4_mxArrayInData);
}

static const mxArray *c4_e_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  int32_T c4_i81;
  int32_T c4_i82;
  int32_T c4_i83;
  real_T c4_b_inData[20];
  int32_T c4_i84;
  int32_T c4_i85;
  int32_T c4_i86;
  real_T c4_u[20];
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  c4_i81 = 0;
  for (c4_i82 = 0; c4_i82 < 4; c4_i82++) {
    for (c4_i83 = 0; c4_i83 < 5; c4_i83++) {
      c4_b_inData[c4_i83 + c4_i81] = (*(real_T (*)[20])c4_inData)[c4_i83 +
        c4_i81];
    }

    c4_i81 += 5;
  }

  c4_i84 = 0;
  for (c4_i85 = 0; c4_i85 < 4; c4_i85++) {
    for (c4_i86 = 0; c4_i86 < 5; c4_i86++) {
      c4_u[c4_i86 + c4_i84] = c4_b_inData[c4_i86 + c4_i84];
    }

    c4_i84 += 5;
  }

  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", c4_u, 0, 0U, 1U, 0U, 2, 5, 4), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static void c4_d_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[20])
{
  real_T c4_dv6[20];
  int32_T c4_i87;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), c4_dv6, 1, 0, 0U, 1, 0U, 2, 5, 4);
  for (c4_i87 = 0; c4_i87 < 20; c4_i87++) {
    c4_y[c4_i87] = c4_dv6[c4_i87];
  }

  sf_mex_destroy(&c4_u);
}

static void c4_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_grid;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  real_T c4_y[20];
  int32_T c4_i88;
  int32_T c4_i89;
  int32_T c4_i90;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_grid = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_grid), &c4_thisId, c4_y);
  sf_mex_destroy(&c4_grid);
  c4_i88 = 0;
  for (c4_i89 = 0; c4_i89 < 4; c4_i89++) {
    for (c4_i90 = 0; c4_i90 < 5; c4_i90++) {
      (*(real_T (*)[20])c4_outData)[c4_i90 + c4_i88] = c4_y[c4_i90 + c4_i88];
    }

    c4_i88 += 5;
  }

  sf_mex_destroy(&c4_mxArrayInData);
}

static void c4_e_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[7])
{
  real_T c4_dv7[7];
  int32_T c4_i91;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), c4_dv7, 1, 0, 0U, 1, 0U, 1, 7);
  for (c4_i91 = 0; c4_i91 < 7; c4_i91++) {
    c4_y[c4_i91] = c4_dv7[c4_i91];
  }

  sf_mex_destroy(&c4_u);
}

static void c4_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_C;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  real_T c4_y[7];
  int32_T c4_i92;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_C = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_C), &c4_thisId, c4_y);
  sf_mex_destroy(&c4_C);
  for (c4_i92 = 0; c4_i92 < 7; c4_i92++) {
    (*(real_T (*)[7])c4_outData)[c4_i92] = c4_y[c4_i92];
  }

  sf_mex_destroy(&c4_mxArrayInData);
}

static void c4_f_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[8])
{
  real_T c4_dv8[8];
  int32_T c4_i93;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), c4_dv8, 1, 0, 0U, 1, 0U, 2, 4, 2);
  for (c4_i93 = 0; c4_i93 < 8; c4_i93++) {
    c4_y[c4_i93] = c4_dv8[c4_i93];
  }

  sf_mex_destroy(&c4_u);
}

static void c4_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_inputBound;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  real_T c4_y[8];
  int32_T c4_i94;
  int32_T c4_i95;
  int32_T c4_i96;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_inputBound = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_inputBound), &c4_thisId,
                        c4_y);
  sf_mex_destroy(&c4_inputBound);
  c4_i94 = 0;
  for (c4_i95 = 0; c4_i95 < 2; c4_i95++) {
    for (c4_i96 = 0; c4_i96 < 4; c4_i96++) {
      (*(real_T (*)[8])c4_outData)[c4_i96 + c4_i94] = c4_y[c4_i96 + c4_i94];
    }

    c4_i94 += 4;
  }

  sf_mex_destroy(&c4_mxArrayInData);
}

static const mxArray *c4_f_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  int32_T c4_i97;
  real_T c4_b_inData[5];
  int32_T c4_i98;
  real_T c4_u[5];
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  for (c4_i97 = 0; c4_i97 < 5; c4_i97++) {
    c4_b_inData[c4_i97] = (*(real_T (*)[5])c4_inData)[c4_i97];
  }

  for (c4_i98 = 0; c4_i98 < 5; c4_i98++) {
    c4_u[c4_i98] = c4_b_inData[c4_i98];
  }

  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", c4_u, 0, 0U, 1U, 0U, 1, 5), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static void c4_g_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[5])
{
  real_T c4_dv9[5];
  int32_T c4_i99;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), c4_dv9, 1, 0, 0U, 1, 0U, 1, 5);
  for (c4_i99 = 0; c4_i99 < 5; c4_i99++) {
    c4_y[c4_i99] = c4_dv9[c4_i99];
  }

  sf_mex_destroy(&c4_u);
}

static void c4_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_f2;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  real_T c4_y[5];
  int32_T c4_i100;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_f2 = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_f2), &c4_thisId, c4_y);
  sf_mex_destroy(&c4_f2);
  for (c4_i100 = 0; c4_i100 < 5; c4_i100++) {
    (*(real_T (*)[5])c4_outData)[c4_i100] = c4_y[c4_i100];
  }

  sf_mex_destroy(&c4_mxArrayInData);
}

static const mxArray *c4_g_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  int32_T c4_i101;
  real_T c4_b_inData[4];
  int32_T c4_i102;
  real_T c4_u[4];
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  for (c4_i101 = 0; c4_i101 < 4; c4_i101++) {
    c4_b_inData[c4_i101] = (*(real_T (*)[4])c4_inData)[c4_i101];
  }

  for (c4_i102 = 0; c4_i102 < 4; c4_i102++) {
    c4_u[c4_i102] = c4_b_inData[c4_i102];
  }

  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", c4_u, 0, 0U, 1U, 0U, 2, 1, 4), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static void c4_h_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[4])
{
  real_T c4_dv10[4];
  int32_T c4_i103;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), c4_dv10, 1, 0, 0U, 1, 0U, 2, 1, 4);
  for (c4_i103 = 0; c4_i103 < 4; c4_i103++) {
    c4_y[c4_i103] = c4_dv10[c4_i103];
  }

  sf_mex_destroy(&c4_u);
}

static void c4_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_input;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  real_T c4_y[4];
  int32_T c4_i104;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_input = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_input), &c4_thisId, c4_y);
  sf_mex_destroy(&c4_input);
  for (c4_i104 = 0; c4_i104 < 4; c4_i104++) {
    (*(real_T (*)[4])c4_outData)[c4_i104] = c4_y[c4_i104];
  }

  sf_mex_destroy(&c4_mxArrayInData);
}

static const mxArray *c4_h_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  int32_T c4_i105;
  real_T c4_b_inData[2];
  int32_T c4_i106;
  real_T c4_u[2];
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  for (c4_i105 = 0; c4_i105 < 2; c4_i105++) {
    c4_b_inData[c4_i105] = (*(real_T (*)[2])c4_inData)[c4_i105];
  }

  for (c4_i106 = 0; c4_i106 < 2; c4_i106++) {
    c4_u[c4_i106] = c4_b_inData[c4_i106];
  }

  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", c4_u, 0, 0U, 1U, 0U, 2, 1, 2), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static void c4_i_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId,
  real_T c4_y[2])
{
  real_T c4_dv11[2];
  int32_T c4_i107;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), c4_dv11, 1, 0, 0U, 1, 0U, 2, 1, 2);
  for (c4_i107 = 0; c4_i107 < 2; c4_i107++) {
    c4_y[c4_i107] = c4_dv11[c4_i107];
  }

  sf_mex_destroy(&c4_u);
}

static void c4_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_bound;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  real_T c4_y[2];
  int32_T c4_i108;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_bound = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_bound), &c4_thisId, c4_y);
  sf_mex_destroy(&c4_bound);
  for (c4_i108 = 0; c4_i108 < 2; c4_i108++) {
    (*(real_T (*)[2])c4_outData)[c4_i108] = c4_y[c4_i108];
  }

  sf_mex_destroy(&c4_mxArrayInData);
}

const mxArray *sf_c4_BoatLibraryTest01_get_eml_resolved_functions_info(void)
{
  const mxArray *c4_nameCaptureInfo = NULL;
  c4_nameCaptureInfo = NULL;
  sf_mex_assign(&c4_nameCaptureInfo, sf_mex_createstruct("structure", 2, 57, 1),
                FALSE);
  c4_info_helper(&c4_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c4_nameCaptureInfo);
  return c4_nameCaptureInfo;
}

static void c4_info_helper(const mxArray **c4_info)
{
  const mxArray *c4_rhs0 = NULL;
  const mxArray *c4_lhs0 = NULL;
  const mxArray *c4_rhs1 = NULL;
  const mxArray *c4_lhs1 = NULL;
  const mxArray *c4_rhs2 = NULL;
  const mxArray *c4_lhs2 = NULL;
  const mxArray *c4_rhs3 = NULL;
  const mxArray *c4_lhs3 = NULL;
  const mxArray *c4_rhs4 = NULL;
  const mxArray *c4_lhs4 = NULL;
  const mxArray *c4_rhs5 = NULL;
  const mxArray *c4_lhs5 = NULL;
  const mxArray *c4_rhs6 = NULL;
  const mxArray *c4_lhs6 = NULL;
  const mxArray *c4_rhs7 = NULL;
  const mxArray *c4_lhs7 = NULL;
  const mxArray *c4_rhs8 = NULL;
  const mxArray *c4_lhs8 = NULL;
  const mxArray *c4_rhs9 = NULL;
  const mxArray *c4_lhs9 = NULL;
  const mxArray *c4_rhs10 = NULL;
  const mxArray *c4_lhs10 = NULL;
  const mxArray *c4_rhs11 = NULL;
  const mxArray *c4_lhs11 = NULL;
  const mxArray *c4_rhs12 = NULL;
  const mxArray *c4_lhs12 = NULL;
  const mxArray *c4_rhs13 = NULL;
  const mxArray *c4_lhs13 = NULL;
  const mxArray *c4_rhs14 = NULL;
  const mxArray *c4_lhs14 = NULL;
  const mxArray *c4_rhs15 = NULL;
  const mxArray *c4_lhs15 = NULL;
  const mxArray *c4_rhs16 = NULL;
  const mxArray *c4_lhs16 = NULL;
  const mxArray *c4_rhs17 = NULL;
  const mxArray *c4_lhs17 = NULL;
  const mxArray *c4_rhs18 = NULL;
  const mxArray *c4_lhs18 = NULL;
  const mxArray *c4_rhs19 = NULL;
  const mxArray *c4_lhs19 = NULL;
  const mxArray *c4_rhs20 = NULL;
  const mxArray *c4_lhs20 = NULL;
  const mxArray *c4_rhs21 = NULL;
  const mxArray *c4_lhs21 = NULL;
  const mxArray *c4_rhs22 = NULL;
  const mxArray *c4_lhs22 = NULL;
  const mxArray *c4_rhs23 = NULL;
  const mxArray *c4_lhs23 = NULL;
  const mxArray *c4_rhs24 = NULL;
  const mxArray *c4_lhs24 = NULL;
  const mxArray *c4_rhs25 = NULL;
  const mxArray *c4_lhs25 = NULL;
  const mxArray *c4_rhs26 = NULL;
  const mxArray *c4_lhs26 = NULL;
  const mxArray *c4_rhs27 = NULL;
  const mxArray *c4_lhs27 = NULL;
  const mxArray *c4_rhs28 = NULL;
  const mxArray *c4_lhs28 = NULL;
  const mxArray *c4_rhs29 = NULL;
  const mxArray *c4_lhs29 = NULL;
  const mxArray *c4_rhs30 = NULL;
  const mxArray *c4_lhs30 = NULL;
  const mxArray *c4_rhs31 = NULL;
  const mxArray *c4_lhs31 = NULL;
  const mxArray *c4_rhs32 = NULL;
  const mxArray *c4_lhs32 = NULL;
  const mxArray *c4_rhs33 = NULL;
  const mxArray *c4_lhs33 = NULL;
  const mxArray *c4_rhs34 = NULL;
  const mxArray *c4_lhs34 = NULL;
  const mxArray *c4_rhs35 = NULL;
  const mxArray *c4_lhs35 = NULL;
  const mxArray *c4_rhs36 = NULL;
  const mxArray *c4_lhs36 = NULL;
  const mxArray *c4_rhs37 = NULL;
  const mxArray *c4_lhs37 = NULL;
  const mxArray *c4_rhs38 = NULL;
  const mxArray *c4_lhs38 = NULL;
  const mxArray *c4_rhs39 = NULL;
  const mxArray *c4_lhs39 = NULL;
  const mxArray *c4_rhs40 = NULL;
  const mxArray *c4_lhs40 = NULL;
  const mxArray *c4_rhs41 = NULL;
  const mxArray *c4_lhs41 = NULL;
  const mxArray *c4_rhs42 = NULL;
  const mxArray *c4_lhs42 = NULL;
  const mxArray *c4_rhs43 = NULL;
  const mxArray *c4_lhs43 = NULL;
  const mxArray *c4_rhs44 = NULL;
  const mxArray *c4_lhs44 = NULL;
  const mxArray *c4_rhs45 = NULL;
  const mxArray *c4_lhs45 = NULL;
  const mxArray *c4_rhs46 = NULL;
  const mxArray *c4_lhs46 = NULL;
  const mxArray *c4_rhs47 = NULL;
  const mxArray *c4_lhs47 = NULL;
  const mxArray *c4_rhs48 = NULL;
  const mxArray *c4_lhs48 = NULL;
  const mxArray *c4_rhs49 = NULL;
  const mxArray *c4_lhs49 = NULL;
  const mxArray *c4_rhs50 = NULL;
  const mxArray *c4_lhs50 = NULL;
  const mxArray *c4_rhs51 = NULL;
  const mxArray *c4_lhs51 = NULL;
  const mxArray *c4_rhs52 = NULL;
  const mxArray *c4_lhs52 = NULL;
  const mxArray *c4_rhs53 = NULL;
  const mxArray *c4_lhs53 = NULL;
  const mxArray *c4_rhs54 = NULL;
  const mxArray *c4_lhs54 = NULL;
  const mxArray *c4_rhs55 = NULL;
  const mxArray *c4_lhs55 = NULL;
  const mxArray *c4_rhs56 = NULL;
  const mxArray *c4_lhs56 = NULL;
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("linspace"), "name", "name", 0);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "resolved",
                  "resolved", 0);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731858U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c4_rhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs0), "rhs", "rhs", 0);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs0), "lhs", "lhs", 0);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 1);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 1);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 1);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c4_rhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs1), "rhs", "rhs", 1);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs1), "lhs", "lhs", 1);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 2);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 2);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 2);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c4_rhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs2), "rhs", "rhs", 2);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs2), "lhs", "lhs", 2);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 3);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_floor"), "name",
                  "name", 3);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m"),
                  "resolved", "resolved", 3);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840326U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c4_rhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs3), "rhs", "rhs", 3);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs3), "lhs", "lhs", 3);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 4);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 4);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 4);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1323192178U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c4_rhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs4), "rhs", "rhs", 4);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs4), "lhs", "lhs", 4);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 5);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("realmax"), "name", "name", 5);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmax.m"), "resolved",
                  "resolved", 5);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1307672842U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c4_rhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs5), "rhs", "rhs", 5);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs5), "lhs", "lhs", 5);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmax.m"), "context",
                  "context", 6);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_realmax"), "name", "name",
                  6);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "resolved",
                  "resolved", 6);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1326749596U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c4_rhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs6), "rhs", "rhs", 6);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs6), "lhs", "lhs", 6);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "context",
                  "context", 7);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_float_model"), "name",
                  "name", 7);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_float_model.m"),
                  "resolved", "resolved", 7);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1326749596U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c4_rhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs7), "rhs", "rhs", 7);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs7), "lhs", "lhs", 7);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "context",
                  "context", 8);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("mtimes"), "name", "name", 8);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 8);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c4_rhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs8), "rhs", "rhs", 8);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs8), "lhs", "lhs", 8);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m!common_checks"),
                  "context", "context", 9);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 9);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 9);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c4_rhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs9), "rhs", "rhs", 9);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs9), "lhs", "lhs", 9);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 10);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("mrdivide"), "name", "name", 10);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 10);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c4_rhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "context",
                  "context", 11);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("rdivide"), "name", "name", 11);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "resolved",
                  "resolved", 11);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c4_rhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 12);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 12);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 12);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c4_rhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 13);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalexp_compatible"),
                  "name", "name", 13);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m"),
                  "resolved", "resolved", 13);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c4_rhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 14);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_div"), "name", "name", 14);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m"), "resolved",
                  "resolved", 14);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731866U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c4_rhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs14), "lhs", "lhs",
                  14);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 15);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("mtimes"), "name", "name", 15);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 15);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 15);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 15);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 15);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 15);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 15);
  sf_mex_assign(&c4_rhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs15), "rhs", "rhs",
                  15);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs15), "lhs", "lhs",
                  15);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "context", "context", 16);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("sind"), "name", "name", 16);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 16);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "resolved",
                  "resolved", 16);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1343851986U), "fileTimeLo",
                  "fileTimeLo", 16);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 16);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 16);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 16);
  sf_mex_assign(&c4_rhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs16), "rhs", "rhs",
                  16);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs16), "lhs", "lhs",
                  16);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "context",
                  "context", 17);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 17);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 17);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 17);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 17);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 17);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 17);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 17);
  sf_mex_assign(&c4_rhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs17), "rhs", "rhs",
                  17);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs17), "lhs", "lhs",
                  17);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 18);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("isfinite"), "name", "name", 18);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 18);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "resolved",
                  "resolved", 18);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 18);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 18);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 18);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 18);
  sf_mex_assign(&c4_rhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs18), "rhs", "rhs",
                  18);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs18), "lhs", "lhs",
                  18);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 19);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 19);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 19);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 19);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 19);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 19);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 19);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 19);
  sf_mex_assign(&c4_rhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs19), "rhs", "rhs",
                  19);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs19), "lhs", "lhs",
                  19);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 20);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("isinf"), "name", "name", 20);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 20);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "resolved",
                  "resolved", 20);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 20);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 20);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 20);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 20);
  sf_mex_assign(&c4_rhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs20), "rhs", "rhs",
                  20);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs20), "lhs", "lhs",
                  20);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "context",
                  "context", 21);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 21);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 21);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 21);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 21);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 21);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 21);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 21);
  sf_mex_assign(&c4_rhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs21), "rhs", "rhs",
                  21);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs21), "lhs", "lhs",
                  21);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 22);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("isnan"), "name", "name", 22);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 22);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "resolved",
                  "resolved", 22);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731858U), "fileTimeLo",
                  "fileTimeLo", 22);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 22);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 22);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 22);
  sf_mex_assign(&c4_rhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs22), "rhs", "rhs",
                  22);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs22), "lhs", "lhs",
                  22);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "context",
                  "context", 23);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 23);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 23);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 23);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 23);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 23);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 23);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 23);
  sf_mex_assign(&c4_rhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs23), "rhs", "rhs",
                  23);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs23), "lhs", "lhs",
                  23);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 24);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_guarded_nan"), "name",
                  "name", 24);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 24);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "resolved", "resolved", 24);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840376U), "fileTimeLo",
                  "fileTimeLo", 24);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 24);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 24);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 24);
  sf_mex_assign(&c4_rhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs24), "rhs", "rhs",
                  24);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs24), "lhs", "lhs",
                  24);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "context", "context", 25);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_is_float_class"), "name",
                  "name", 25);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 25);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m"),
                  "resolved", "resolved", 25);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840382U), "fileTimeLo",
                  "fileTimeLo", 25);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 25);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 25);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 25);
  sf_mex_assign(&c4_rhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs25), "rhs", "rhs",
                  25);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs25), "lhs", "lhs",
                  25);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 26);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_rem90"), "name",
                  "name", 26);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 26);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "resolved", "resolved", 26);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 26);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 26);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 26);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 26);
  sf_mex_assign(&c4_rhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs26), "rhs", "rhs",
                  26);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs26), "lhs", "lhs",
                  26);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 27);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("mrdivide"), "name", "name", 27);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 27);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 27);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 27);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 27);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 27);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 27);
  sf_mex_assign(&c4_rhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs27), "rhs", "rhs",
                  27);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs27), "lhs", "lhs",
                  27);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 28);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("rem"), "name", "name", 28);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 28);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "resolved",
                  "resolved", 28);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731854U), "fileTimeLo",
                  "fileTimeLo", 28);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 28);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 28);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 28);
  sf_mex_assign(&c4_rhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs28), "rhs", "rhs",
                  28);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs28), "lhs", "lhs",
                  28);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 29);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 29);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 29);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 29);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 29);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 29);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 29);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 29);
  sf_mex_assign(&c4_rhs29, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs29, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs29), "rhs", "rhs",
                  29);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs29), "lhs", "lhs",
                  29);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 30);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 30);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 30);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 30);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 30);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 30);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 30);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 30);
  sf_mex_assign(&c4_rhs30, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs30, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs30), "rhs", "rhs",
                  30);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs30), "lhs", "lhs",
                  30);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 31);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 31);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 31);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 31);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1358207740U), "fileTimeLo",
                  "fileTimeLo", 31);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 31);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 31);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 31);
  sf_mex_assign(&c4_rhs31, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs31, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs31), "rhs", "rhs",
                  31);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs31), "lhs", "lhs",
                  31);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 32);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("abs"), "name", "name", 32);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 32);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "resolved",
                  "resolved", 32);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731852U), "fileTimeLo",
                  "fileTimeLo", 32);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 32);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 32);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 32);
  sf_mex_assign(&c4_rhs32, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs32, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs32), "rhs", "rhs",
                  32);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs32), "lhs", "lhs",
                  32);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 33);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 33);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 33);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 33);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 33);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 33);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 33);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 33);
  sf_mex_assign(&c4_rhs33, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs33, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs33), "rhs", "rhs",
                  33);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs33), "lhs", "lhs",
                  33);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 34);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_abs"), "name",
                  "name", 34);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 34);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m"),
                  "resolved", "resolved", 34);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840312U), "fileTimeLo",
                  "fileTimeLo", 34);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 34);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 34);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 34);
  sf_mex_assign(&c4_rhs34, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs34, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs34), "rhs", "rhs",
                  34);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs34), "lhs", "lhs",
                  34);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 35);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("mtimes"), "name", "name", 35);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 35);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 35);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 35);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 35);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 35);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 35);
  sf_mex_assign(&c4_rhs35, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs35, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs35), "rhs", "rhs",
                  35);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs35), "lhs", "lhs",
                  35);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "context", "context", 36);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("mtimes"), "name", "name", 36);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 36);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 36);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 36);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 36);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 36);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 36);
  sf_mex_assign(&c4_rhs36, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs36, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs36), "rhs", "rhs",
                  36);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs36), "lhs", "lhs",
                  36);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "context", "context", 37);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("cosd"), "name", "name", 37);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 37);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "resolved",
                  "resolved", 37);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1343851972U), "fileTimeLo",
                  "fileTimeLo", 37);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 37);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 37);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 37);
  sf_mex_assign(&c4_rhs37, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs37, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs37), "rhs", "rhs",
                  37);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs37), "lhs", "lhs",
                  37);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "context",
                  "context", 38);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 38);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 38);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 38);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 38);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 38);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 38);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 38);
  sf_mex_assign(&c4_rhs38, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs38, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs38), "rhs", "rhs",
                  38);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs38), "lhs", "lhs",
                  38);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "context", "context", 39);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("mpower"), "name", "name", 39);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 39);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "resolved",
                  "resolved", 39);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 39);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 39);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 39);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 39);
  sf_mex_assign(&c4_rhs39, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs39, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs39), "rhs", "rhs",
                  39);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs39), "lhs", "lhs",
                  39);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 40);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 40);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 40);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 40);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 40);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 40);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 40);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 40);
  sf_mex_assign(&c4_rhs40, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs40, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs40), "rhs", "rhs",
                  40);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs40), "lhs", "lhs",
                  40);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 41);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("ismatrix"), "name", "name", 41);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 41);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/ismatrix.m"), "resolved",
                  "resolved", 41);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1331326458U), "fileTimeLo",
                  "fileTimeLo", 41);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 41);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 41);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 41);
  sf_mex_assign(&c4_rhs41, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs41, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs41), "rhs", "rhs",
                  41);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs41), "lhs", "lhs",
                  41);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 42);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("power"), "name", "name", 42);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 42);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m"), "resolved",
                  "resolved", 42);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 42);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 42);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 42);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 42);
  sf_mex_assign(&c4_rhs42, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs42, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs42), "rhs", "rhs",
                  42);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs42), "lhs", "lhs",
                  42);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m"), "context",
                  "context", 43);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 43);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 43);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 43);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 43);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 43);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 43);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 43);
  sf_mex_assign(&c4_rhs43, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs43, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs43), "rhs", "rhs",
                  43);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs43), "lhs", "lhs",
                  43);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 44);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 44);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 44);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 44);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 44);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 44);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 44);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 44);
  sf_mex_assign(&c4_rhs44, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs44, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs44), "rhs", "rhs",
                  44);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs44), "lhs", "lhs",
                  44);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 45);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 45);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 45);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 45);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1358207740U), "fileTimeLo",
                  "fileTimeLo", 45);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 45);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 45);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 45);
  sf_mex_assign(&c4_rhs45, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs45, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs45), "rhs", "rhs",
                  45);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs45), "lhs", "lhs",
                  45);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 46);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("floor"), "name", "name", 46);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 46);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "resolved",
                  "resolved", 46);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731854U), "fileTimeLo",
                  "fileTimeLo", 46);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 46);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 46);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 46);
  sf_mex_assign(&c4_rhs46, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs46, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs46), "rhs", "rhs",
                  46);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs46), "lhs", "lhs",
                  46);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "context",
                  "context", 47);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 47);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 47);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 47);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 47);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 47);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 47);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 47);
  sf_mex_assign(&c4_rhs47, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs47, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs47), "rhs", "rhs",
                  47);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs47), "lhs", "lhs",
                  47);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "context",
                  "context", 48);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_floor"), "name",
                  "name", 48);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 48);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m"),
                  "resolved", "resolved", 48);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840326U), "fileTimeLo",
                  "fileTimeLo", 48);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 48);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 48);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 48);
  sf_mex_assign(&c4_rhs48, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs48, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs48), "rhs", "rhs",
                  48);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs48), "lhs", "lhs",
                  48);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!scalar_float_power"),
                  "context", "context", 49);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 49);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 49);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 49);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 49);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 49);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 49);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 49);
  sf_mex_assign(&c4_rhs49, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs49, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs49), "rhs", "rhs",
                  49);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs49), "lhs", "lhs",
                  49);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!scalar_float_power"),
                  "context", "context", 50);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("mtimes"), "name", "name", 50);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 50);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 50);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 50);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 50);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 50);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 50);
  sf_mex_assign(&c4_rhs50, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs50, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs50), "rhs", "rhs",
                  50);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs50), "lhs", "lhs",
                  50);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "context", "context", 51);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("sin"), "name", "name", 51);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 51);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m"), "resolved",
                  "resolved", 51);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1343851986U), "fileTimeLo",
                  "fileTimeLo", 51);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 51);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 51);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 51);
  sf_mex_assign(&c4_rhs51, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs51, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs51), "rhs", "rhs",
                  51);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs51), "lhs", "lhs",
                  51);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m"), "context",
                  "context", 52);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_sin"), "name",
                  "name", 52);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 52);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sin.m"),
                  "resolved", "resolved", 52);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1286840336U), "fileTimeLo",
                  "fileTimeLo", 52);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 52);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 52);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 52);
  sf_mex_assign(&c4_rhs52, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs52, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs52), "rhs", "rhs",
                  52);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs52), "lhs", "lhs",
                  52);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "context", "context", 53);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("length"), "name", "name", 53);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 53);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/length.m"), "resolved",
                  "resolved", 53);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1303167806U), "fileTimeLo",
                  "fileTimeLo", 53);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 53);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 53);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 53);
  sf_mex_assign(&c4_rhs53, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs53, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs53), "rhs", "rhs",
                  53);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs53), "lhs", "lhs",
                  53);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(""), "context", "context", 54);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("sign"), "name", "name", 54);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 54);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sign.m"), "resolved",
                  "resolved", 54);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 54);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 54);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 54);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 54);
  sf_mex_assign(&c4_rhs54, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs54, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs54), "rhs", "rhs",
                  54);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs54), "lhs", "lhs",
                  54);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sign.m"), "context",
                  "context", 55);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 55);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 55);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 55);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 55);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 55);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 55);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 55);
  sf_mex_assign(&c4_rhs55, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs55, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs55), "rhs", "rhs",
                  55);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs55), "lhs", "lhs",
                  55);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sign.m"), "context",
                  "context", 56);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("eml_scalar_sign"), "name",
                  "name", 56);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 56);
  sf_mex_addfield(*c4_info, c4_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sign.m"),
                  "resolved", "resolved", 56);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(1356563094U), "fileTimeLo",
                  "fileTimeLo", 56);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 56);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 56);
  sf_mex_addfield(*c4_info, c4_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 56);
  sf_mex_assign(&c4_rhs56, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c4_lhs56, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_rhs56), "rhs", "rhs",
                  56);
  sf_mex_addfield(*c4_info, sf_mex_duplicatearraysafe(&c4_lhs56), "lhs", "lhs",
                  56);
  sf_mex_destroy(&c4_rhs0);
  sf_mex_destroy(&c4_lhs0);
  sf_mex_destroy(&c4_rhs1);
  sf_mex_destroy(&c4_lhs1);
  sf_mex_destroy(&c4_rhs2);
  sf_mex_destroy(&c4_lhs2);
  sf_mex_destroy(&c4_rhs3);
  sf_mex_destroy(&c4_lhs3);
  sf_mex_destroy(&c4_rhs4);
  sf_mex_destroy(&c4_lhs4);
  sf_mex_destroy(&c4_rhs5);
  sf_mex_destroy(&c4_lhs5);
  sf_mex_destroy(&c4_rhs6);
  sf_mex_destroy(&c4_lhs6);
  sf_mex_destroy(&c4_rhs7);
  sf_mex_destroy(&c4_lhs7);
  sf_mex_destroy(&c4_rhs8);
  sf_mex_destroy(&c4_lhs8);
  sf_mex_destroy(&c4_rhs9);
  sf_mex_destroy(&c4_lhs9);
  sf_mex_destroy(&c4_rhs10);
  sf_mex_destroy(&c4_lhs10);
  sf_mex_destroy(&c4_rhs11);
  sf_mex_destroy(&c4_lhs11);
  sf_mex_destroy(&c4_rhs12);
  sf_mex_destroy(&c4_lhs12);
  sf_mex_destroy(&c4_rhs13);
  sf_mex_destroy(&c4_lhs13);
  sf_mex_destroy(&c4_rhs14);
  sf_mex_destroy(&c4_lhs14);
  sf_mex_destroy(&c4_rhs15);
  sf_mex_destroy(&c4_lhs15);
  sf_mex_destroy(&c4_rhs16);
  sf_mex_destroy(&c4_lhs16);
  sf_mex_destroy(&c4_rhs17);
  sf_mex_destroy(&c4_lhs17);
  sf_mex_destroy(&c4_rhs18);
  sf_mex_destroy(&c4_lhs18);
  sf_mex_destroy(&c4_rhs19);
  sf_mex_destroy(&c4_lhs19);
  sf_mex_destroy(&c4_rhs20);
  sf_mex_destroy(&c4_lhs20);
  sf_mex_destroy(&c4_rhs21);
  sf_mex_destroy(&c4_lhs21);
  sf_mex_destroy(&c4_rhs22);
  sf_mex_destroy(&c4_lhs22);
  sf_mex_destroy(&c4_rhs23);
  sf_mex_destroy(&c4_lhs23);
  sf_mex_destroy(&c4_rhs24);
  sf_mex_destroy(&c4_lhs24);
  sf_mex_destroy(&c4_rhs25);
  sf_mex_destroy(&c4_lhs25);
  sf_mex_destroy(&c4_rhs26);
  sf_mex_destroy(&c4_lhs26);
  sf_mex_destroy(&c4_rhs27);
  sf_mex_destroy(&c4_lhs27);
  sf_mex_destroy(&c4_rhs28);
  sf_mex_destroy(&c4_lhs28);
  sf_mex_destroy(&c4_rhs29);
  sf_mex_destroy(&c4_lhs29);
  sf_mex_destroy(&c4_rhs30);
  sf_mex_destroy(&c4_lhs30);
  sf_mex_destroy(&c4_rhs31);
  sf_mex_destroy(&c4_lhs31);
  sf_mex_destroy(&c4_rhs32);
  sf_mex_destroy(&c4_lhs32);
  sf_mex_destroy(&c4_rhs33);
  sf_mex_destroy(&c4_lhs33);
  sf_mex_destroy(&c4_rhs34);
  sf_mex_destroy(&c4_lhs34);
  sf_mex_destroy(&c4_rhs35);
  sf_mex_destroy(&c4_lhs35);
  sf_mex_destroy(&c4_rhs36);
  sf_mex_destroy(&c4_lhs36);
  sf_mex_destroy(&c4_rhs37);
  sf_mex_destroy(&c4_lhs37);
  sf_mex_destroy(&c4_rhs38);
  sf_mex_destroy(&c4_lhs38);
  sf_mex_destroy(&c4_rhs39);
  sf_mex_destroy(&c4_lhs39);
  sf_mex_destroy(&c4_rhs40);
  sf_mex_destroy(&c4_lhs40);
  sf_mex_destroy(&c4_rhs41);
  sf_mex_destroy(&c4_lhs41);
  sf_mex_destroy(&c4_rhs42);
  sf_mex_destroy(&c4_lhs42);
  sf_mex_destroy(&c4_rhs43);
  sf_mex_destroy(&c4_lhs43);
  sf_mex_destroy(&c4_rhs44);
  sf_mex_destroy(&c4_lhs44);
  sf_mex_destroy(&c4_rhs45);
  sf_mex_destroy(&c4_lhs45);
  sf_mex_destroy(&c4_rhs46);
  sf_mex_destroy(&c4_lhs46);
  sf_mex_destroy(&c4_rhs47);
  sf_mex_destroy(&c4_lhs47);
  sf_mex_destroy(&c4_rhs48);
  sf_mex_destroy(&c4_lhs48);
  sf_mex_destroy(&c4_rhs49);
  sf_mex_destroy(&c4_lhs49);
  sf_mex_destroy(&c4_rhs50);
  sf_mex_destroy(&c4_lhs50);
  sf_mex_destroy(&c4_rhs51);
  sf_mex_destroy(&c4_lhs51);
  sf_mex_destroy(&c4_rhs52);
  sf_mex_destroy(&c4_lhs52);
  sf_mex_destroy(&c4_rhs53);
  sf_mex_destroy(&c4_lhs53);
  sf_mex_destroy(&c4_rhs54);
  sf_mex_destroy(&c4_lhs54);
  sf_mex_destroy(&c4_rhs55);
  sf_mex_destroy(&c4_lhs55);
  sf_mex_destroy(&c4_rhs56);
  sf_mex_destroy(&c4_lhs56);
}

static const mxArray *c4_emlrt_marshallOut(char * c4_u)
{
  const mxArray *c4_y = NULL;
  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", c4_u, 15, 0U, 0U, 0U, 2, 1, strlen
    (c4_u)), FALSE);
  return c4_y;
}

static const mxArray *c4_b_emlrt_marshallOut(uint32_T c4_u)
{
  const mxArray *c4_y = NULL;
  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", &c4_u, 7, 0U, 0U, 0U, 0), FALSE);
  return c4_y;
}

static void c4_eml_scalar_eg(SFc4_BoatLibraryTest01InstanceStruct *chartInstance)
{
}

static void c4_linspace(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
  real_T c4_d1, real_T c4_d2, real_T c4_y[5])
{
  real_T c4_A;
  real_T c4_x;
  real_T c4_b_x;
  real_T c4_delta1;
  real_T c4_b_A;
  real_T c4_c_x;
  real_T c4_d_x;
  real_T c4_delta2;
  int32_T c4_k;
  real_T c4_b_k;
  real_T c4_a;
  real_T c4_b;
  real_T c4_b_y;
  real_T c4_b_a;
  real_T c4_b_b;
  real_T c4_c_y;
  real_T c4_c_A;
  real_T c4_e_x;
  real_T c4_f_x;
  int32_T c4_c_k;
  real_T c4_c_a;
  real_T c4_c_b;
  real_T c4_d_y;
  boolean_T guard1 = FALSE;
  boolean_T guard2 = FALSE;
  boolean_T guard3 = FALSE;
  c4_eml_scalar_eg(chartInstance);
  c4_y[4] = c4_d2;
  c4_y[0] = c4_d1;
  guard1 = FALSE;
  guard2 = FALSE;
  guard3 = FALSE;
  if (c4_d1 < 0.0 != c4_d2 < 0.0) {
    if (muDoubleScalarAbs(c4_d1) > 8.9884656743115785E+307) {
      guard2 = TRUE;
    } else {
      guard3 = TRUE;
    }
  } else {
    guard3 = TRUE;
  }

  if (guard3 == TRUE) {
    if (muDoubleScalarAbs(c4_d2) > 8.9884656743115785E+307) {
      guard2 = TRUE;
    } else if (muDoubleScalarAbs(0.0) > 8.9884656743115785E+307) {
      guard1 = TRUE;
    } else {
      c4_c_A = c4_d2 - c4_d1;
      c4_e_x = c4_c_A;
      c4_f_x = c4_e_x;
      c4_delta1 = c4_f_x / 4.0;
      for (c4_c_k = 0; c4_c_k < 3; c4_c_k++) {
        c4_b_k = 1.0 + (real_T)c4_c_k;
        c4_c_a = c4_b_k;
        c4_c_b = c4_delta1;
        c4_d_y = c4_c_a * c4_c_b;
        c4_y[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
          _SFD_INTEGER_CHECK("", c4_b_k + 1.0), 1, 5, 1, 0) - 1] = c4_d1 +
          c4_d_y;
      }
    }
  }

  if (guard2 == TRUE) {
    guard1 = TRUE;
  }

  if (guard1 == TRUE) {
    c4_A = c4_d1;
    c4_x = c4_A;
    c4_b_x = c4_x;
    c4_delta1 = c4_b_x / 4.0;
    c4_b_A = c4_d2;
    c4_c_x = c4_b_A;
    c4_d_x = c4_c_x;
    c4_delta2 = c4_d_x / 4.0;
    for (c4_k = 0; c4_k < 3; c4_k++) {
      c4_b_k = 1.0 + (real_T)c4_k;
      c4_a = c4_delta2;
      c4_b = c4_b_k;
      c4_b_y = c4_a * c4_b;
      c4_b_a = c4_delta1;
      c4_b_b = c4_b_k;
      c4_c_y = c4_b_a * c4_b_b;
      c4_y[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
        _SFD_INTEGER_CHECK("", c4_b_k + 1.0), 1, 5, 1, 0) - 1] = (c4_d1 + c4_b_y)
        - c4_c_y;
    }
  }
}

static void c4_getMinFromGrid(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, real_T c4_grid[20], real_T c4_preinput[4], real_T c4_Fxref,
  real_T c4_Fyref, real_T c4_Mref, real_T c4_rx1, real_T c4_rx2, real_T c4_ry1,
  real_T c4_ry2, real_T c4_C[7], real_T c4_output[4])
{
  uint32_T c4_debug_family_var_map[22];
  real_T c4_ii;
  real_T c4_iii;
  real_T c4_v0;
  real_T c4_i;
  real_T c4_j;
  real_T c4_k;
  real_T c4_l;
  real_T c4_input[4];
  real_T c4_v1;
  real_T c4_nargin = 10.0;
  real_T c4_nargout = 1.0;
  int32_T c4_i109;
  int32_T c4_i110;
  real_T c4_b_grid[4];
  int32_T c4_i111;
  real_T c4_b_preinput[4];
  int32_T c4_i112;
  real_T c4_b_C[7];
  int32_T c4_b_i;
  int32_T c4_b_j;
  int32_T c4_b_k;
  int32_T c4_b_l;
  int32_T c4_i113;
  real_T c4_b_input[4];
  int32_T c4_i114;
  real_T c4_c_preinput[4];
  int32_T c4_i115;
  real_T c4_c_C[7];
  int32_T c4_i116;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 22U, 22U, c4_e_debug_family_names,
    c4_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c4_ii, 0U, c4_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_iii, 1U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_v0, 2U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_i, 3U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_j, 4U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_k, 5U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_l, 6U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_input, 7U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_v1, 8U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargin, 9U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargout, 10U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_grid, 11U, c4_e_sf_marshallOut,
    c4_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_preinput, 12U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fxref, 13U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fyref, 14U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Mref, 15U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx1, 16U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx2, 17U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry1, 18U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry2, 19U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_C, 20U, c4_d_sf_marshallOut,
    c4_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_output, 21U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  CV_EML_FCN(0, 1);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 33);
  c4_ii = 5.0;
  c4_iii = 4.0;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 34);
  c4_i109 = 0;
  for (c4_i110 = 0; c4_i110 < 4; c4_i110++) {
    c4_b_grid[c4_i110] = c4_grid[c4_i109];
    c4_i109 += 5;
  }

  for (c4_i111 = 0; c4_i111 < 4; c4_i111++) {
    c4_b_preinput[c4_i111] = c4_preinput[c4_i111];
  }

  for (c4_i112 = 0; c4_i112 < 7; c4_i112++) {
    c4_b_C[c4_i112] = c4_C[c4_i112];
  }

  c4_v0 = c4_costFunction(chartInstance, c4_b_grid, c4_b_preinput, c4_Fxref,
    c4_Fyref, c4_Mref, c4_rx1, c4_rx2, c4_ry1, c4_ry2, c4_b_C);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 36);
  c4_output[0] = c4_grid[0];
  c4_output[1] = c4_grid[5];
  c4_output[2] = c4_grid[10];
  c4_output[3] = c4_grid[15];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 38);
  c4_i = 1.0;
  c4_b_i = 0;
  while (c4_b_i < 5) {
    c4_i = 1.0 + (real_T)c4_b_i;
    CV_EML_FOR(0, 1, 1, 1);
    _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 39);
    c4_j = 1.0;
    c4_b_j = 0;
    while (c4_b_j < 5) {
      c4_j = 1.0 + (real_T)c4_b_j;
      CV_EML_FOR(0, 1, 2, 1);
      _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 40);
      c4_k = 1.0;
      c4_b_k = 0;
      while (c4_b_k < 5) {
        c4_k = 1.0 + (real_T)c4_b_k;
        CV_EML_FOR(0, 1, 3, 1);
        _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 41);
        c4_l = 1.0;
        c4_b_l = 0;
        while (c4_b_l < 5) {
          c4_l = 1.0 + (real_T)c4_b_l;
          CV_EML_FOR(0, 1, 4, 1);
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 42);
          c4_input[0] = c4_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
            "grid", (int32_T)_SFD_INTEGER_CHECK("i", c4_i), 1, 5, 1, 0) - 1];
          c4_input[1] = c4_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
            "grid", (int32_T)_SFD_INTEGER_CHECK("j", c4_j), 1, 5, 1, 0) + 4];
          c4_input[2] = c4_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
            "grid", (int32_T)_SFD_INTEGER_CHECK("k", c4_k), 1, 5, 1, 0) + 9];
          c4_input[3] = c4_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
            "grid", (int32_T)_SFD_INTEGER_CHECK("l", c4_l), 1, 5, 1, 0) + 14];
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 43);
          for (c4_i113 = 0; c4_i113 < 4; c4_i113++) {
            c4_b_input[c4_i113] = c4_input[c4_i113];
          }

          for (c4_i114 = 0; c4_i114 < 4; c4_i114++) {
            c4_c_preinput[c4_i114] = c4_preinput[c4_i114];
          }

          for (c4_i115 = 0; c4_i115 < 7; c4_i115++) {
            c4_c_C[c4_i115] = c4_C[c4_i115];
          }

          c4_v1 = c4_b_costFunction(chartInstance, c4_b_input, c4_c_preinput,
            c4_Fxref, c4_Fyref, c4_Mref, c4_rx1, c4_rx2, c4_ry1, c4_ry2, c4_c_C);
          _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 44);
          if (CV_EML_IF(0, 1, 0, c4_v0 > c4_v1)) {
            _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 45);
            c4_v0 = c4_v1;
            _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 46);
            for (c4_i116 = 0; c4_i116 < 4; c4_i116++) {
              c4_output[c4_i116] = c4_input[c4_i116];
            }
          } else {
            _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 48);
          }

          c4_b_l++;
          _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
        }

        CV_EML_FOR(0, 1, 4, 0);
        c4_b_k++;
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      }

      CV_EML_FOR(0, 1, 3, 0);
      c4_b_j++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 2, 0);
    c4_b_i++;
    _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
  }

  CV_EML_FOR(0, 1, 1, 0);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -48);
  _SFD_SYMBOL_SCOPE_POP();
}

static real_T c4_costFunction(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, real_T c4_input[4], real_T c4_preinput[4], real_T c4_Fxref,
  real_T c4_Fyref, real_T c4_Mref, real_T c4_rx1, real_T c4_rx2, real_T c4_ry1,
  real_T c4_ry2, real_T c4_C[7])
{
  real_T c4_output;
  uint32_T c4_debug_family_var_map[22];
  real_T c4_theta1;
  real_T c4_f1;
  real_T c4_theta2;
  real_T c4_f2;
  real_T c4_pretheta1;
  real_T c4_pretheta2;
  real_T c4_Fx;
  real_T c4_Fy;
  real_T c4_M;
  real_T c4_nargin = 10.0;
  real_T c4_nargout = 1.0;
  real_T c4_a;
  real_T c4_b;
  real_T c4_y;
  real_T c4_b_a;
  real_T c4_b_b;
  real_T c4_b_y;
  real_T c4_c_a;
  real_T c4_c_b;
  real_T c4_c_y;
  real_T c4_d_a;
  real_T c4_d_b;
  real_T c4_d_y;
  real_T c4_e_a;
  real_T c4_e_b;
  real_T c4_e_y;
  real_T c4_f_a;
  real_T c4_f_b;
  real_T c4_f_y;
  real_T c4_g_a;
  real_T c4_g_b;
  real_T c4_g_y;
  real_T c4_h_a;
  real_T c4_h_b;
  real_T c4_h_y;
  real_T c4_i_a;
  real_T c4_i_b;
  real_T c4_i_y;
  real_T c4_j_a;
  real_T c4_j_b;
  real_T c4_j_y;
  real_T c4_k_a;
  real_T c4_k_b;
  real_T c4_k_y;
  real_T c4_l_a;
  real_T c4_l_b;
  real_T c4_l_y;
  real_T c4_m_a;
  real_T c4_m_b;
  real_T c4_m_y;
  real_T c4_n_a;
  real_T c4_n_b;
  real_T c4_n_y;
  real_T c4_o_a;
  real_T c4_o_b;
  real_T c4_o_y;
  real_T c4_p_a;
  real_T c4_p_b;
  real_T c4_p_y;
  real_T c4_q_a;
  real_T c4_q_b;
  real_T c4_q_y;
  real_T c4_r_a;
  real_T c4_r_b;
  real_T c4_r_y;
  real_T c4_s_a;
  real_T c4_s_b;
  real_T c4_s_y;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 22U, 22U, c4_c_debug_family_names,
    c4_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_theta1, 0U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f1, 1U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_theta2, 2U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f2, 3U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_pretheta1, 4U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_pretheta2, 5U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fx, 6U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fy, 7U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_M, 8U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargin, 9U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargout, 10U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_input, 11U, c4_g_sf_marshallOut,
    c4_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_preinput, 12U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fxref, 13U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fyref, 14U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Mref, 15U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx1, 16U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx2, 17U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry1, 18U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry2, 19U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_C, 20U, c4_d_sf_marshallOut,
    c4_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_output, 21U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  CV_EML_FCN(0, 6);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 131U);
  c4_theta1 = c4_input[0];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 132U);
  c4_f1 = c4_input[1];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 133U);
  c4_theta2 = c4_input[2];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 134U);
  c4_f2 = c4_input[3];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 136U);
  c4_pretheta1 = c4_preinput[0];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 137U);
  c4_pretheta2 = c4_preinput[2];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 139U);
  c4_a = -c4_f1;
  c4_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_b);
  c4_y = c4_a * c4_b;
  c4_b_a = c4_f2;
  c4_b_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_b_b);
  c4_b_y = c4_b_a * c4_b_b;
  c4_Fx = c4_y - c4_b_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 140U);
  c4_c_a = c4_f1;
  c4_c_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_c_b);
  c4_c_y = c4_c_a * c4_c_b;
  c4_d_a = c4_f2;
  c4_d_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_d_b);
  c4_d_y = c4_d_a * c4_d_b;
  c4_Fy = c4_c_y + c4_d_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 141U);
  c4_e_a = c4_rx1;
  c4_e_b = c4_f1;
  c4_e_y = c4_e_a * c4_e_b;
  c4_f_a = c4_e_y;
  c4_f_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_f_b);
  c4_f_y = c4_f_a * c4_f_b;
  c4_g_a = c4_ry1;
  c4_g_b = c4_f1;
  c4_g_y = c4_g_a * c4_g_b;
  c4_h_a = c4_g_y;
  c4_h_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_h_b);
  c4_h_y = c4_h_a * c4_h_b;
  c4_i_a = c4_rx2;
  c4_i_b = c4_f2;
  c4_i_y = c4_i_a * c4_i_b;
  c4_j_a = c4_i_y;
  c4_j_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_j_b);
  c4_j_y = c4_j_a * c4_j_b;
  c4_k_a = c4_ry2;
  c4_k_b = c4_f2;
  c4_k_y = c4_k_a * c4_k_b;
  c4_l_a = c4_k_y;
  c4_l_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_l_b);
  c4_l_y = c4_l_a * c4_l_b;
  c4_M = ((c4_f_y + c4_h_y) + c4_j_y) + c4_l_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 144U);
  c4_m_a = c4_C[0];
  c4_m_b = c4_mpower(chartInstance, c4_Fx - c4_Fxref);
  c4_m_y = c4_m_a * c4_m_b;
  c4_n_a = c4_C[1];
  c4_n_b = c4_mpower(chartInstance, c4_Fy - c4_Fyref);
  c4_n_y = c4_n_a * c4_n_b;
  c4_o_a = c4_C[2];
  c4_o_b = c4_mpower(chartInstance, c4_M - c4_Mref);
  c4_o_y = c4_o_a * c4_o_b;
  c4_p_a = c4_C[3];
  c4_p_b = c4_mpower(chartInstance, c4_f1);
  c4_p_y = c4_p_a * c4_p_b;
  c4_q_a = c4_C[4];
  c4_q_b = c4_mpower(chartInstance, c4_f2);
  c4_q_y = c4_q_a * c4_q_b;
  c4_r_a = c4_C[5];
  c4_r_b = c4_mpower(chartInstance, c4_theta1 - c4_pretheta1);
  c4_r_y = c4_r_a * c4_r_b;
  c4_s_a = c4_C[6];
  c4_s_b = c4_mpower(chartInstance, c4_theta2 - c4_pretheta2);
  c4_s_y = c4_s_a * c4_s_b;
  c4_output = (((((c4_m_y + c4_n_y) + c4_o_y) + c4_p_y) + c4_q_y) + c4_r_y) +
    c4_s_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -144);
  _SFD_SYMBOL_SCOPE_POP();
  return c4_output;
}

static real_T c4_sind(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
                      real_T c4_x)
{
  real_T c4_b_x;
  c4_b_x = c4_x;
  c4_b_sind(chartInstance, &c4_b_x);
  return c4_b_x;
}

static boolean_T c4_isfinite(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
  real_T c4_x)
{
  real_T c4_b_x;
  boolean_T c4_b_b;
  boolean_T c4_b0;
  real_T c4_c_x;
  boolean_T c4_c_b;
  boolean_T c4_b1;
  c4_b_x = c4_x;
  c4_b_b = muDoubleScalarIsInf(c4_b_x);
  c4_b0 = !c4_b_b;
  c4_c_x = c4_x;
  c4_c_b = muDoubleScalarIsNaN(c4_c_x);
  c4_b1 = !c4_c_b;
  return c4_b0 && c4_b1;
}

static real_T c4_cosd(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
                      real_T c4_x)
{
  real_T c4_b_x;
  c4_b_x = c4_x;
  c4_b_cosd(chartInstance, &c4_b_x);
  return c4_b_x;
}

static real_T c4_mpower(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
  real_T c4_a)
{
  real_T c4_b_a;
  real_T c4_c_a;
  real_T c4_ak;
  real_T c4_d_a;
  real_T c4_e_a;
  real_T c4_b;
  c4_b_a = c4_a;
  c4_c_a = c4_b_a;
  c4_eml_scalar_eg(chartInstance);
  c4_ak = c4_c_a;
  c4_d_a = c4_ak;
  c4_eml_scalar_eg(chartInstance);
  c4_e_a = c4_d_a;
  c4_b = c4_d_a;
  return c4_e_a * c4_b;
}

static real_T c4_b_costFunction(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, real_T c4_input[4], real_T c4_preinput[4], real_T c4_Fxref,
  real_T c4_Fyref, real_T c4_Mref, real_T c4_rx1, real_T c4_rx2, real_T c4_ry1,
  real_T c4_ry2, real_T c4_C[7])
{
  real_T c4_output;
  uint32_T c4_debug_family_var_map[22];
  real_T c4_theta1;
  real_T c4_f1;
  real_T c4_theta2;
  real_T c4_f2;
  real_T c4_pretheta1;
  real_T c4_pretheta2;
  real_T c4_Fx;
  real_T c4_Fy;
  real_T c4_M;
  real_T c4_nargin = 10.0;
  real_T c4_nargout = 1.0;
  real_T c4_a;
  real_T c4_b;
  real_T c4_y;
  real_T c4_b_a;
  real_T c4_b_b;
  real_T c4_b_y;
  real_T c4_c_a;
  real_T c4_c_b;
  real_T c4_c_y;
  real_T c4_d_a;
  real_T c4_d_b;
  real_T c4_d_y;
  real_T c4_e_a;
  real_T c4_e_b;
  real_T c4_e_y;
  real_T c4_f_a;
  real_T c4_f_b;
  real_T c4_f_y;
  real_T c4_g_a;
  real_T c4_g_b;
  real_T c4_g_y;
  real_T c4_h_a;
  real_T c4_h_b;
  real_T c4_h_y;
  real_T c4_i_a;
  real_T c4_i_b;
  real_T c4_i_y;
  real_T c4_j_a;
  real_T c4_j_b;
  real_T c4_j_y;
  real_T c4_k_a;
  real_T c4_k_b;
  real_T c4_k_y;
  real_T c4_l_a;
  real_T c4_l_b;
  real_T c4_l_y;
  real_T c4_m_a;
  real_T c4_m_b;
  real_T c4_m_y;
  real_T c4_n_a;
  real_T c4_n_b;
  real_T c4_n_y;
  real_T c4_o_a;
  real_T c4_o_b;
  real_T c4_o_y;
  real_T c4_p_a;
  real_T c4_p_b;
  real_T c4_p_y;
  real_T c4_q_a;
  real_T c4_q_b;
  real_T c4_q_y;
  real_T c4_r_a;
  real_T c4_r_b;
  real_T c4_r_y;
  real_T c4_s_a;
  real_T c4_s_b;
  real_T c4_s_y;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 22U, 22U, c4_d_debug_family_names,
    c4_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_theta1, 0U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f1, 1U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_theta2, 2U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f2, 3U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_pretheta1, 4U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_pretheta2, 5U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fx, 6U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fy, 7U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_M, 8U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargin, 9U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargout, 10U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_input, 11U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_preinput, 12U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fxref, 13U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fyref, 14U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Mref, 15U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx1, 16U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx2, 17U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry1, 18U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry2, 19U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_C, 20U, c4_d_sf_marshallOut,
    c4_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_output, 21U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  CV_EML_FCN(0, 6);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 131U);
  c4_theta1 = c4_input[0];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 132U);
  c4_f1 = c4_input[1];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 133U);
  c4_theta2 = c4_input[2];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 134U);
  c4_f2 = c4_input[3];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 136U);
  c4_pretheta1 = c4_preinput[0];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 137U);
  c4_pretheta2 = c4_preinput[2];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 139U);
  c4_a = -c4_f1;
  c4_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_b);
  c4_y = c4_a * c4_b;
  c4_b_a = c4_f2;
  c4_b_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_b_b);
  c4_b_y = c4_b_a * c4_b_b;
  c4_Fx = c4_y - c4_b_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 140U);
  c4_c_a = c4_f1;
  c4_c_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_c_b);
  c4_c_y = c4_c_a * c4_c_b;
  c4_d_a = c4_f2;
  c4_d_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_d_b);
  c4_d_y = c4_d_a * c4_d_b;
  c4_Fy = c4_c_y + c4_d_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 141U);
  c4_e_a = c4_rx1;
  c4_e_b = c4_f1;
  c4_e_y = c4_e_a * c4_e_b;
  c4_f_a = c4_e_y;
  c4_f_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_f_b);
  c4_f_y = c4_f_a * c4_f_b;
  c4_g_a = c4_ry1;
  c4_g_b = c4_f1;
  c4_g_y = c4_g_a * c4_g_b;
  c4_h_a = c4_g_y;
  c4_h_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_h_b);
  c4_h_y = c4_h_a * c4_h_b;
  c4_i_a = c4_rx2;
  c4_i_b = c4_f2;
  c4_i_y = c4_i_a * c4_i_b;
  c4_j_a = c4_i_y;
  c4_j_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_j_b);
  c4_j_y = c4_j_a * c4_j_b;
  c4_k_a = c4_ry2;
  c4_k_b = c4_f2;
  c4_k_y = c4_k_a * c4_k_b;
  c4_l_a = c4_k_y;
  c4_l_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_l_b);
  c4_l_y = c4_l_a * c4_l_b;
  c4_M = ((c4_f_y + c4_h_y) + c4_j_y) + c4_l_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 144U);
  c4_m_a = c4_C[0];
  c4_m_b = c4_mpower(chartInstance, c4_Fx - c4_Fxref);
  c4_m_y = c4_m_a * c4_m_b;
  c4_n_a = c4_C[1];
  c4_n_b = c4_mpower(chartInstance, c4_Fy - c4_Fyref);
  c4_n_y = c4_n_a * c4_n_b;
  c4_o_a = c4_C[2];
  c4_o_b = c4_mpower(chartInstance, c4_M - c4_Mref);
  c4_o_y = c4_o_a * c4_o_b;
  c4_p_a = c4_C[3];
  c4_p_b = c4_mpower(chartInstance, c4_f1);
  c4_p_y = c4_p_a * c4_p_b;
  c4_q_a = c4_C[4];
  c4_q_b = c4_mpower(chartInstance, c4_f2);
  c4_q_y = c4_q_a * c4_q_b;
  c4_r_a = c4_C[5];
  c4_r_b = c4_mpower(chartInstance, c4_theta1 - c4_pretheta1);
  c4_r_y = c4_r_a * c4_r_b;
  c4_s_a = c4_C[6];
  c4_s_b = c4_mpower(chartInstance, c4_theta2 - c4_pretheta2);
  c4_s_y = c4_s_a * c4_s_b;
  c4_output = (((((c4_m_y + c4_n_y) + c4_o_y) + c4_p_y) + c4_q_y) + c4_r_y) +
    c4_s_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -144);
  _SFD_SYMBOL_SCOPE_POP();
  return c4_output;
}

static void c4_SteepestDescent(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, real_T c4_input[4], real_T c4_preinput[4], real_T c4_Fxref,
  real_T c4_Fyref, real_T c4_Mref, real_T c4_rx1, real_T c4_rx2, real_T c4_ry1,
  real_T c4_ry2, real_T c4_C[7], real_T c4_output[4])
{
  uint32_T c4_debug_family_var_map[23];
  real_T c4_theta1;
  real_T c4_f1;
  real_T c4_theta2;
  real_T c4_f2;
  real_T c4_pretheta1;
  real_T c4_pretheta2;
  real_T c4_dVdtheta1;
  real_T c4_dVdf1;
  real_T c4_dVdtheta2;
  real_T c4_dVdf2;
  real_T c4_nargin = 10.0;
  real_T c4_nargout = 1.0;
  real_T c4_a;
  real_T c4_b;
  real_T c4_y;
  real_T c4_b_a;
  real_T c4_b_b;
  real_T c4_b_y;
  real_T c4_c_a;
  real_T c4_c_b;
  real_T c4_c_y;
  real_T c4_d_a;
  real_T c4_d_b;
  real_T c4_d_y;
  real_T c4_e_a;
  real_T c4_e_b;
  real_T c4_e_y;
  real_T c4_f_a;
  real_T c4_f_b;
  real_T c4_f_y;
  real_T c4_g_a;
  real_T c4_g_b;
  real_T c4_g_y;
  real_T c4_h_a;
  real_T c4_h_b;
  real_T c4_h_y;
  real_T c4_i_a;
  real_T c4_i_b;
  real_T c4_i_y;
  real_T c4_j_a;
  real_T c4_j_b;
  real_T c4_j_y;
  real_T c4_x;
  real_T c4_b_x;
  real_T c4_k_a;
  real_T c4_k_b;
  real_T c4_k_y;
  real_T c4_l_a;
  real_T c4_l_b;
  real_T c4_l_y;
  real_T c4_m_a;
  real_T c4_m_b;
  real_T c4_m_y;
  real_T c4_n_a;
  real_T c4_n_b;
  real_T c4_n_y;
  real_T c4_o_a;
  real_T c4_o_b;
  real_T c4_o_y;
  real_T c4_p_a;
  real_T c4_p_b;
  real_T c4_p_y;
  real_T c4_q_a;
  real_T c4_q_b;
  real_T c4_q_y;
  real_T c4_r_a;
  real_T c4_r_b;
  real_T c4_r_y;
  real_T c4_s_a;
  real_T c4_s_b;
  real_T c4_s_y;
  real_T c4_t_a;
  real_T c4_t_b;
  real_T c4_t_y;
  real_T c4_u_a;
  real_T c4_u_b;
  real_T c4_u_y;
  real_T c4_v_a;
  real_T c4_v_b;
  real_T c4_v_y;
  real_T c4_w_a;
  real_T c4_w_b;
  real_T c4_w_y;
  real_T c4_x_a;
  real_T c4_x_b;
  real_T c4_x_y;
  real_T c4_y_a;
  real_T c4_y_b;
  real_T c4_y_y;
  real_T c4_ab_a;
  real_T c4_ab_b;
  real_T c4_ab_y;
  real_T c4_bb_a;
  real_T c4_bb_b;
  real_T c4_bb_y;
  real_T c4_c_x;
  real_T c4_d_x;
  real_T c4_cb_a;
  real_T c4_cb_b;
  real_T c4_cb_y;
  real_T c4_db_a;
  real_T c4_db_b;
  real_T c4_db_y;
  real_T c4_eb_a;
  real_T c4_eb_b;
  real_T c4_eb_y;
  real_T c4_fb_a;
  real_T c4_fb_b;
  real_T c4_fb_y;
  real_T c4_gb_a;
  real_T c4_gb_b;
  real_T c4_gb_y;
  real_T c4_hb_a;
  real_T c4_hb_b;
  real_T c4_hb_y;
  real_T c4_ib_a;
  real_T c4_ib_b;
  real_T c4_ib_y;
  real_T c4_jb_a;
  real_T c4_jb_b;
  real_T c4_jb_y;
  real_T c4_kb_a;
  real_T c4_kb_b;
  real_T c4_kb_y;
  real_T c4_lb_a;
  real_T c4_lb_b;
  real_T c4_lb_y;
  real_T c4_mb_a;
  real_T c4_mb_b;
  real_T c4_mb_y;
  real_T c4_nb_a;
  real_T c4_nb_b;
  real_T c4_nb_y;
  real_T c4_ob_a;
  real_T c4_ob_b;
  real_T c4_ob_y;
  real_T c4_pb_a;
  real_T c4_pb_b;
  real_T c4_pb_y;
  real_T c4_qb_a;
  real_T c4_qb_b;
  real_T c4_qb_y;
  real_T c4_rb_a;
  real_T c4_rb_b;
  real_T c4_rb_y;
  real_T c4_sb_a;
  real_T c4_sb_b;
  real_T c4_sb_y;
  real_T c4_tb_a;
  real_T c4_tb_b;
  real_T c4_tb_y;
  real_T c4_ub_a;
  real_T c4_ub_b;
  real_T c4_ub_y;
  real_T c4_vb_a;
  real_T c4_vb_b;
  real_T c4_vb_y;
  real_T c4_wb_a;
  real_T c4_wb_b;
  real_T c4_wb_y;
  real_T c4_xb_a;
  real_T c4_xb_b;
  real_T c4_xb_y;
  real_T c4_yb_a;
  real_T c4_yb_b;
  real_T c4_yb_y;
  real_T c4_ac_a;
  real_T c4_ac_b;
  real_T c4_ac_y;
  real_T c4_bc_a;
  real_T c4_bc_b;
  real_T c4_bc_y;
  real_T c4_cc_a;
  real_T c4_cc_b;
  real_T c4_cc_y;
  real_T c4_dc_a;
  real_T c4_dc_b;
  real_T c4_dc_y;
  real_T c4_ec_a;
  real_T c4_ec_b;
  real_T c4_ec_y;
  real_T c4_fc_a;
  real_T c4_fc_b;
  real_T c4_fc_y;
  real_T c4_gc_a;
  real_T c4_gc_b;
  real_T c4_gc_y;
  real_T c4_hc_a;
  real_T c4_hc_b;
  real_T c4_hc_y;
  real_T c4_ic_a;
  real_T c4_ic_b;
  real_T c4_ic_y;
  real_T c4_jc_a;
  real_T c4_jc_b;
  real_T c4_jc_y;
  real_T c4_kc_a;
  real_T c4_kc_b;
  real_T c4_kc_y;
  real_T c4_lc_a;
  real_T c4_lc_b;
  real_T c4_lc_y;
  real_T c4_mc_a;
  real_T c4_mc_b;
  real_T c4_mc_y;
  real_T c4_nc_a;
  real_T c4_nc_b;
  real_T c4_nc_y;
  real_T c4_oc_a;
  real_T c4_oc_b;
  real_T c4_oc_y;
  real_T c4_pc_a;
  real_T c4_pc_b;
  real_T c4_pc_y;
  real_T c4_qc_a;
  real_T c4_qc_b;
  real_T c4_qc_y;
  real_T c4_rc_a;
  real_T c4_rc_b;
  real_T c4_rc_y;
  real_T c4_sc_a;
  real_T c4_sc_b;
  real_T c4_sc_y;
  real_T c4_tc_a;
  real_T c4_tc_b;
  real_T c4_tc_y;
  real_T c4_uc_a;
  real_T c4_uc_b;
  real_T c4_uc_y;
  real_T c4_vc_a;
  real_T c4_vc_b;
  real_T c4_vc_y;
  real_T c4_wc_a;
  real_T c4_wc_b;
  real_T c4_wc_y;
  real_T c4_xc_a;
  real_T c4_xc_b;
  real_T c4_xc_y;
  real_T c4_yc_a;
  real_T c4_yc_b;
  real_T c4_yc_y;
  real_T c4_ad_a;
  real_T c4_ad_b;
  real_T c4_ad_y;
  real_T c4_bd_a;
  real_T c4_bd_b;
  real_T c4_bd_y;
  real_T c4_cd_a;
  real_T c4_cd_b;
  real_T c4_cd_y;
  real_T c4_dd_a;
  real_T c4_dd_b;
  real_T c4_dd_y;
  real_T c4_ed_a;
  real_T c4_ed_b;
  real_T c4_ed_y;
  real_T c4_fd_a;
  real_T c4_fd_b;
  real_T c4_fd_y;
  real_T c4_gd_a;
  real_T c4_gd_b;
  real_T c4_gd_y;
  real_T c4_hd_a;
  real_T c4_hd_b;
  real_T c4_hd_y;
  real_T c4_id_a;
  real_T c4_id_b;
  real_T c4_id_y;
  real_T c4_jd_a;
  real_T c4_jd_b;
  real_T c4_jd_y;
  real_T c4_kd_a;
  real_T c4_kd_b;
  real_T c4_kd_y;
  real_T c4_ld_a;
  real_T c4_ld_b;
  real_T c4_ld_y;
  real_T c4_md_a;
  real_T c4_md_b;
  real_T c4_md_y;
  real_T c4_nd_a;
  real_T c4_nd_b;
  real_T c4_nd_y;
  real_T c4_od_a;
  real_T c4_od_b;
  real_T c4_od_y;
  real_T c4_pd_a;
  real_T c4_pd_b;
  real_T c4_pd_y;
  real_T c4_qd_a;
  real_T c4_qd_b;
  real_T c4_qd_y;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 23U, 23U, c4_f_debug_family_names,
    c4_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_theta1, 0U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f1, 1U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_theta2, 2U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f2, 3U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_pretheta1, 4U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_pretheta2, 5U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_dVdtheta1, 6U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_dVdf1, 7U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_dVdtheta2, 8U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_dVdf2, 9U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargin, 10U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargout, 11U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_input, 12U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_preinput, 13U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fxref, 14U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fyref, 15U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Mref, 16U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx1, 17U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx2, 18U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry1, 19U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry2, 20U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_C, 21U, c4_d_sf_marshallOut,
    c4_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_output, 22U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  CV_EML_FCN(0, 8);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 176U);
  c4_theta1 = c4_input[0];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 177U);
  c4_f1 = c4_input[1];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 178U);
  c4_theta2 = c4_input[2];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 179U);
  c4_f2 = c4_input[3];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 181U);
  c4_pretheta1 = c4_preinput[0];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 182U);
  c4_pretheta2 = c4_preinput[2];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 184U);
  c4_a = c4_f1;
  c4_b = c4_ry1;
  c4_y = c4_a * c4_b;
  c4_b_a = c4_y;
  c4_b_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_b_b);
  c4_b_y = c4_b_a * c4_b_b;
  c4_c_a = c4_f1;
  c4_c_b = c4_rx1;
  c4_c_y = c4_c_a * c4_c_b;
  c4_d_a = c4_c_y;
  c4_d_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_d_b);
  c4_d_y = c4_d_a * c4_d_b;
  c4_e_a = c4_C[2];
  c4_e_b = c4_b_y - c4_d_y;
  c4_e_y = c4_e_a * c4_e_b;
  c4_f_a = c4_f1;
  c4_f_b = c4_rx1;
  c4_f_y = c4_f_a * c4_f_b;
  c4_g_a = c4_f_y;
  c4_g_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_g_b);
  c4_g_y = c4_g_a * c4_g_b;
  c4_h_a = c4_f2;
  c4_h_b = c4_rx2;
  c4_h_y = c4_h_a * c4_h_b;
  c4_i_a = c4_h_y;
  c4_i_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_i_b);
  c4_i_y = c4_i_a * c4_i_b;
  c4_j_a = c4_f1;
  c4_j_b = c4_ry1;
  c4_j_y = c4_j_a * c4_j_b;
  c4_x = c4_theta1;
  c4_b_x = c4_x;
  c4_b_x = muDoubleScalarSin(c4_b_x);
  c4_k_a = c4_j_y;
  c4_k_b = c4_b_x;
  c4_k_y = c4_k_a * c4_k_b;
  c4_l_a = c4_f2;
  c4_l_b = c4_ry2;
  c4_l_y = c4_l_a * c4_l_b;
  c4_m_a = c4_l_y;
  c4_m_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_m_b);
  c4_m_y = c4_m_a * c4_m_b;
  c4_n_a = c4_e_y;
  c4_n_b = (((c4_g_y - c4_Mref) + c4_i_y) + c4_k_y) + c4_m_y;
  c4_n_y = c4_n_a * c4_n_b;
  c4_o_a = c4_C[5];
  c4_o_b = c4_pretheta1 - c4_theta1;
  c4_o_y = c4_o_a * c4_o_b;
  c4_p_a = c4_C[1];
  c4_p_b = c4_f1;
  c4_p_y = c4_p_a * c4_p_b;
  c4_q_a = c4_p_y;
  c4_q_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_q_b);
  c4_q_y = c4_q_a * c4_q_b;
  c4_r_a = c4_f1;
  c4_r_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_r_b);
  c4_r_y = c4_r_a * c4_r_b;
  c4_s_a = c4_f2;
  c4_s_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_s_b);
  c4_s_y = c4_s_a * c4_s_b;
  c4_t_a = c4_q_y;
  c4_t_b = (c4_r_y - c4_Fyref) + c4_s_y;
  c4_t_y = c4_t_a * c4_t_b;
  c4_u_a = c4_C[0];
  c4_u_b = c4_f1;
  c4_u_y = c4_u_a * c4_u_b;
  c4_v_a = c4_u_y;
  c4_v_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_v_b);
  c4_v_y = c4_v_a * c4_v_b;
  c4_w_a = c4_f1;
  c4_w_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_w_b);
  c4_w_y = c4_w_a * c4_w_b;
  c4_x_a = c4_f2;
  c4_x_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_x_b);
  c4_x_y = c4_x_a * c4_x_b;
  c4_y_a = c4_v_y;
  c4_y_b = (c4_Fxref + c4_w_y) + c4_x_y;
  c4_y_y = c4_y_a * c4_y_b;
  c4_dVdtheta1 = ((c4_n_y - c4_o_y) - c4_t_y) + c4_y_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 191U);
  c4_ab_a = c4_C[3];
  c4_ab_b = c4_f1;
  c4_ab_y = c4_ab_a * c4_ab_b;
  c4_bb_a = c4_C[0];
  c4_bb_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_bb_b);
  c4_bb_y = c4_bb_a * c4_bb_b;
  c4_c_x = c4_theta1;
  c4_d_x = c4_c_x;
  c4_d_x = muDoubleScalarSin(c4_d_x);
  c4_cb_a = c4_f1;
  c4_cb_b = c4_d_x;
  c4_cb_y = c4_cb_a * c4_cb_b;
  c4_db_a = c4_f2;
  c4_db_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_db_b);
  c4_db_y = c4_db_a * c4_db_b;
  c4_eb_a = c4_bb_y;
  c4_eb_b = (c4_Fxref + c4_cb_y) + c4_db_y;
  c4_eb_y = c4_eb_a * c4_eb_b;
  c4_fb_a = c4_C[1];
  c4_fb_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_fb_b);
  c4_fb_y = c4_fb_a * c4_fb_b;
  c4_gb_a = c4_f1;
  c4_gb_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_gb_b);
  c4_gb_y = c4_gb_a * c4_gb_b;
  c4_hb_a = c4_f2;
  c4_hb_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_hb_b);
  c4_hb_y = c4_hb_a * c4_hb_b;
  c4_ib_a = c4_fb_y;
  c4_ib_b = (c4_gb_y - c4_Fyref) + c4_hb_y;
  c4_ib_y = c4_ib_a * c4_ib_b;
  c4_jb_a = c4_rx1;
  c4_jb_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_jb_b);
  c4_jb_y = c4_jb_a * c4_jb_b;
  c4_kb_a = c4_ry1;
  c4_kb_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_kb_b);
  c4_kb_y = c4_kb_a * c4_kb_b;
  c4_lb_a = c4_C[2];
  c4_lb_b = c4_jb_y + c4_kb_y;
  c4_lb_y = c4_lb_a * c4_lb_b;
  c4_mb_a = c4_f1;
  c4_mb_b = c4_rx1;
  c4_mb_y = c4_mb_a * c4_mb_b;
  c4_nb_a = c4_mb_y;
  c4_nb_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_nb_b);
  c4_nb_y = c4_nb_a * c4_nb_b;
  c4_ob_a = c4_f2;
  c4_ob_b = c4_rx2;
  c4_ob_y = c4_ob_a * c4_ob_b;
  c4_pb_a = c4_ob_y;
  c4_pb_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_pb_b);
  c4_pb_y = c4_pb_a * c4_pb_b;
  c4_qb_a = c4_f1;
  c4_qb_b = c4_ry1;
  c4_qb_y = c4_qb_a * c4_qb_b;
  c4_rb_a = c4_qb_y;
  c4_rb_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_rb_b);
  c4_rb_y = c4_rb_a * c4_rb_b;
  c4_sb_a = c4_f2;
  c4_sb_b = c4_ry2;
  c4_sb_y = c4_sb_a * c4_sb_b;
  c4_tb_a = c4_sb_y;
  c4_tb_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_tb_b);
  c4_tb_y = c4_tb_a * c4_tb_b;
  c4_ub_a = c4_lb_y;
  c4_ub_b = (((c4_nb_y - c4_Mref) + c4_pb_y) + c4_rb_y) + c4_tb_y;
  c4_ub_y = c4_ub_a * c4_ub_b;
  c4_dVdf1 = ((c4_ab_y + c4_eb_y) + c4_ib_y) + c4_ub_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 197U);
  c4_vb_a = c4_f2;
  c4_vb_b = c4_ry2;
  c4_vb_y = c4_vb_a * c4_vb_b;
  c4_wb_a = c4_vb_y;
  c4_wb_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_wb_b);
  c4_wb_y = c4_wb_a * c4_wb_b;
  c4_xb_a = c4_f2;
  c4_xb_b = c4_rx2;
  c4_xb_y = c4_xb_a * c4_xb_b;
  c4_yb_a = c4_xb_y;
  c4_yb_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_yb_b);
  c4_yb_y = c4_yb_a * c4_yb_b;
  c4_ac_a = c4_C[2];
  c4_ac_b = c4_wb_y - c4_yb_y;
  c4_ac_y = c4_ac_a * c4_ac_b;
  c4_bc_a = c4_f1;
  c4_bc_b = c4_rx1;
  c4_bc_y = c4_bc_a * c4_bc_b;
  c4_cc_a = c4_bc_y;
  c4_cc_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_cc_b);
  c4_cc_y = c4_cc_a * c4_cc_b;
  c4_dc_a = c4_f2;
  c4_dc_b = c4_rx2;
  c4_dc_y = c4_dc_a * c4_dc_b;
  c4_ec_a = c4_dc_y;
  c4_ec_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_ec_b);
  c4_ec_y = c4_ec_a * c4_ec_b;
  c4_fc_a = c4_f1;
  c4_fc_b = c4_ry1;
  c4_fc_y = c4_fc_a * c4_fc_b;
  c4_gc_a = c4_fc_y;
  c4_gc_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_gc_b);
  c4_gc_y = c4_gc_a * c4_gc_b;
  c4_hc_a = c4_f2;
  c4_hc_b = c4_ry2;
  c4_hc_y = c4_hc_a * c4_hc_b;
  c4_ic_a = c4_hc_y;
  c4_ic_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_ic_b);
  c4_ic_y = c4_ic_a * c4_ic_b;
  c4_jc_a = c4_ac_y;
  c4_jc_b = (((c4_cc_y - c4_Mref) + c4_ec_y) + c4_gc_y) + c4_ic_y;
  c4_jc_y = c4_jc_a * c4_jc_b;
  c4_kc_a = c4_C[6];
  c4_kc_b = c4_pretheta2 - c4_theta2;
  c4_kc_y = c4_kc_a * c4_kc_b;
  c4_lc_a = c4_C[1];
  c4_lc_b = c4_f2;
  c4_lc_y = c4_lc_a * c4_lc_b;
  c4_mc_a = c4_lc_y;
  c4_mc_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_mc_b);
  c4_mc_y = c4_mc_a * c4_mc_b;
  c4_nc_a = c4_f1;
  c4_nc_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_nc_b);
  c4_nc_y = c4_nc_a * c4_nc_b;
  c4_oc_a = c4_f2;
  c4_oc_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_oc_b);
  c4_oc_y = c4_oc_a * c4_oc_b;
  c4_pc_a = c4_mc_y;
  c4_pc_b = (c4_nc_y - c4_Fyref) + c4_oc_y;
  c4_pc_y = c4_pc_a * c4_pc_b;
  c4_qc_a = c4_C[0];
  c4_qc_b = c4_f2;
  c4_qc_y = c4_qc_a * c4_qc_b;
  c4_rc_a = c4_qc_y;
  c4_rc_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_rc_b);
  c4_rc_y = c4_rc_a * c4_rc_b;
  c4_sc_a = c4_f1;
  c4_sc_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_sc_b);
  c4_sc_y = c4_sc_a * c4_sc_b;
  c4_tc_a = c4_f2;
  c4_tc_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_tc_b);
  c4_tc_y = c4_tc_a * c4_tc_b;
  c4_uc_a = c4_rc_y;
  c4_uc_b = (c4_Fxref + c4_sc_y) + c4_tc_y;
  c4_uc_y = c4_uc_a * c4_uc_b;
  c4_dVdtheta2 = ((c4_jc_y - c4_kc_y) - c4_pc_y) + c4_uc_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 204U);
  c4_vc_a = c4_C[4];
  c4_vc_b = c4_f2;
  c4_vc_y = c4_vc_a * c4_vc_b;
  c4_wc_a = c4_C[0];
  c4_wc_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_wc_b);
  c4_wc_y = c4_wc_a * c4_wc_b;
  c4_xc_a = c4_f1;
  c4_xc_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_xc_b);
  c4_xc_y = c4_xc_a * c4_xc_b;
  c4_yc_a = c4_f2;
  c4_yc_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_yc_b);
  c4_yc_y = c4_yc_a * c4_yc_b;
  c4_ad_a = c4_wc_y;
  c4_ad_b = (c4_Fxref + c4_xc_y) + c4_yc_y;
  c4_ad_y = c4_ad_a * c4_ad_b;
  c4_bd_a = c4_C[1];
  c4_bd_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_bd_b);
  c4_bd_y = c4_bd_a * c4_bd_b;
  c4_cd_a = c4_f1;
  c4_cd_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_cd_b);
  c4_cd_y = c4_cd_a * c4_cd_b;
  c4_dd_a = c4_f2;
  c4_dd_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_dd_b);
  c4_dd_y = c4_dd_a * c4_dd_b;
  c4_ed_a = c4_bd_y;
  c4_ed_b = (c4_cd_y - c4_Fyref) + c4_dd_y;
  c4_ed_y = c4_ed_a * c4_ed_b;
  c4_fd_a = c4_rx2;
  c4_fd_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_fd_b);
  c4_fd_y = c4_fd_a * c4_fd_b;
  c4_gd_a = c4_ry2;
  c4_gd_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_gd_b);
  c4_gd_y = c4_gd_a * c4_gd_b;
  c4_hd_a = c4_C[2];
  c4_hd_b = c4_fd_y + c4_gd_y;
  c4_hd_y = c4_hd_a * c4_hd_b;
  c4_id_a = c4_f1;
  c4_id_b = c4_rx1;
  c4_id_y = c4_id_a * c4_id_b;
  c4_jd_a = c4_id_y;
  c4_jd_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_jd_b);
  c4_jd_y = c4_jd_a * c4_jd_b;
  c4_kd_a = c4_f2;
  c4_kd_b = c4_rx2;
  c4_kd_y = c4_kd_a * c4_kd_b;
  c4_ld_a = c4_kd_y;
  c4_ld_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_ld_b);
  c4_ld_y = c4_ld_a * c4_ld_b;
  c4_md_a = c4_f1;
  c4_md_b = c4_ry1;
  c4_md_y = c4_md_a * c4_md_b;
  c4_nd_a = c4_md_y;
  c4_nd_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_nd_b);
  c4_nd_y = c4_nd_a * c4_nd_b;
  c4_od_a = c4_f2;
  c4_od_b = c4_ry2;
  c4_od_y = c4_od_a * c4_od_b;
  c4_pd_a = c4_od_y;
  c4_pd_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_pd_b);
  c4_pd_y = c4_pd_a * c4_pd_b;
  c4_qd_a = c4_hd_y;
  c4_qd_b = (((c4_jd_y - c4_Mref) + c4_ld_y) + c4_nd_y) + c4_pd_y;
  c4_qd_y = c4_qd_a * c4_qd_b;
  c4_dVdf2 = ((c4_vc_y + c4_ad_y) + c4_ed_y) + c4_qd_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 210U);
  c4_output[0] = c4_dVdtheta1;
  c4_output[1] = c4_dVdf1;
  c4_output[2] = c4_dVdtheta2;
  c4_output[3] = c4_dVdf2;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -210);
  _SFD_SYMBOL_SCOPE_POP();
}

static void c4_Newtons(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
  real_T c4_input[4], real_T c4_Fxref, real_T c4_Fyref, real_T c4_Mref, real_T
  c4_rx1, real_T c4_rx2, real_T c4_ry1, real_T c4_ry2, real_T c4_C[7], real_T
  c4_output[4])
{
  uint32_T c4_debug_family_var_map[20];
  real_T c4_theta1;
  real_T c4_f1;
  real_T c4_theta2;
  real_T c4_f2;
  real_T c4_ddVddtheta1;
  real_T c4_ddVddf1;
  real_T c4_ddVddtheta2;
  real_T c4_ddVddf2;
  real_T c4_nargin = 9.0;
  real_T c4_nargout = 1.0;
  real_T c4_a;
  real_T c4_b;
  real_T c4_y;
  real_T c4_b_a;
  real_T c4_b_b;
  real_T c4_b_y;
  real_T c4_c_a;
  real_T c4_c_b;
  real_T c4_c_y;
  real_T c4_d_a;
  real_T c4_d_b;
  real_T c4_d_y;
  real_T c4_e_a;
  real_T c4_e_b;
  real_T c4_e_y;
  real_T c4_f_a;
  real_T c4_f_b;
  real_T c4_f_y;
  real_T c4_g_a;
  real_T c4_g_b;
  real_T c4_g_y;
  real_T c4_h_a;
  real_T c4_h_b;
  real_T c4_h_y;
  real_T c4_i_a;
  real_T c4_i_b;
  real_T c4_i_y;
  real_T c4_j_a;
  real_T c4_j_b;
  real_T c4_j_y;
  real_T c4_k_a;
  real_T c4_k_b;
  real_T c4_k_y;
  real_T c4_l_a;
  real_T c4_l_b;
  real_T c4_l_y;
  real_T c4_m_a;
  real_T c4_m_b;
  real_T c4_m_y;
  real_T c4_n_a;
  real_T c4_n_b;
  real_T c4_n_y;
  real_T c4_o_a;
  real_T c4_o_b;
  real_T c4_o_y;
  real_T c4_p_a;
  real_T c4_p_b;
  real_T c4_p_y;
  real_T c4_q_a;
  real_T c4_q_b;
  real_T c4_q_y;
  real_T c4_r_a;
  real_T c4_r_b;
  real_T c4_r_y;
  real_T c4_s_a;
  real_T c4_s_b;
  real_T c4_s_y;
  real_T c4_t_a;
  real_T c4_t_b;
  real_T c4_t_y;
  real_T c4_u_a;
  real_T c4_d1;
  real_T c4_u_b;
  real_T c4_u_y;
  real_T c4_v_a;
  real_T c4_v_b;
  real_T c4_v_y;
  real_T c4_w_a;
  real_T c4_d2;
  real_T c4_w_b;
  real_T c4_w_y;
  real_T c4_x_a;
  real_T c4_x_b;
  real_T c4_x_y;
  real_T c4_y_a;
  real_T c4_y_b;
  real_T c4_y_y;
  real_T c4_ab_a;
  real_T c4_ab_b;
  real_T c4_ab_y;
  real_T c4_bb_a;
  real_T c4_bb_b;
  real_T c4_bb_y;
  real_T c4_cb_a;
  real_T c4_cb_b;
  real_T c4_cb_y;
  real_T c4_db_a;
  real_T c4_db_b;
  real_T c4_db_y;
  real_T c4_eb_a;
  real_T c4_eb_b;
  real_T c4_eb_y;
  real_T c4_fb_a;
  real_T c4_fb_b;
  real_T c4_fb_y;
  real_T c4_gb_a;
  real_T c4_gb_b;
  real_T c4_gb_y;
  real_T c4_hb_a;
  real_T c4_hb_b;
  real_T c4_hb_y;
  real_T c4_ib_a;
  real_T c4_ib_b;
  real_T c4_ib_y;
  real_T c4_jb_a;
  real_T c4_jb_b;
  real_T c4_jb_y;
  real_T c4_kb_a;
  real_T c4_kb_b;
  real_T c4_kb_y;
  real_T c4_lb_a;
  real_T c4_d3;
  real_T c4_lb_b;
  real_T c4_lb_y;
  real_T c4_mb_a;
  real_T c4_d4;
  real_T c4_mb_b;
  real_T c4_mb_y;
  real_T c4_nb_a;
  real_T c4_nb_b;
  real_T c4_nb_y;
  real_T c4_ob_a;
  real_T c4_ob_b;
  real_T c4_ob_y;
  real_T c4_pb_a;
  real_T c4_pb_b;
  real_T c4_pb_y;
  real_T c4_qb_a;
  real_T c4_qb_b;
  real_T c4_qb_y;
  real_T c4_rb_a;
  real_T c4_rb_b;
  real_T c4_rb_y;
  real_T c4_sb_a;
  real_T c4_sb_b;
  real_T c4_sb_y;
  real_T c4_tb_a;
  real_T c4_tb_b;
  real_T c4_tb_y;
  real_T c4_ub_a;
  real_T c4_ub_b;
  real_T c4_ub_y;
  real_T c4_vb_a;
  real_T c4_vb_b;
  real_T c4_vb_y;
  real_T c4_wb_a;
  real_T c4_wb_b;
  real_T c4_wb_y;
  real_T c4_xb_a;
  real_T c4_xb_b;
  real_T c4_xb_y;
  real_T c4_yb_a;
  real_T c4_yb_b;
  real_T c4_yb_y;
  real_T c4_ac_a;
  real_T c4_ac_b;
  real_T c4_ac_y;
  real_T c4_bc_a;
  real_T c4_bc_b;
  real_T c4_bc_y;
  real_T c4_cc_a;
  real_T c4_cc_b;
  real_T c4_cc_y;
  real_T c4_dc_a;
  real_T c4_dc_b;
  real_T c4_dc_y;
  real_T c4_ec_a;
  real_T c4_ec_b;
  real_T c4_ec_y;
  real_T c4_fc_a;
  real_T c4_fc_b;
  real_T c4_fc_y;
  real_T c4_gc_a;
  real_T c4_gc_b;
  real_T c4_gc_y;
  real_T c4_hc_a;
  real_T c4_hc_b;
  real_T c4_hc_y;
  real_T c4_ic_a;
  real_T c4_d5;
  real_T c4_ic_b;
  real_T c4_ic_y;
  real_T c4_jc_a;
  real_T c4_jc_b;
  real_T c4_jc_y;
  real_T c4_kc_a;
  real_T c4_d6;
  real_T c4_kc_b;
  real_T c4_kc_y;
  real_T c4_lc_a;
  real_T c4_lc_b;
  real_T c4_lc_y;
  real_T c4_mc_a;
  real_T c4_mc_b;
  real_T c4_mc_y;
  real_T c4_nc_a;
  real_T c4_nc_b;
  real_T c4_nc_y;
  real_T c4_oc_a;
  real_T c4_oc_b;
  real_T c4_oc_y;
  real_T c4_pc_a;
  real_T c4_pc_b;
  real_T c4_pc_y;
  real_T c4_qc_a;
  real_T c4_qc_b;
  real_T c4_qc_y;
  real_T c4_rc_a;
  real_T c4_rc_b;
  real_T c4_rc_y;
  real_T c4_sc_a;
  real_T c4_sc_b;
  real_T c4_sc_y;
  real_T c4_tc_a;
  real_T c4_tc_b;
  real_T c4_tc_y;
  real_T c4_uc_a;
  real_T c4_uc_b;
  real_T c4_uc_y;
  real_T c4_vc_a;
  real_T c4_vc_b;
  real_T c4_vc_y;
  real_T c4_wc_a;
  real_T c4_wc_b;
  real_T c4_wc_y;
  real_T c4_xc_a;
  real_T c4_xc_b;
  real_T c4_xc_y;
  real_T c4_yc_a;
  real_T c4_d7;
  real_T c4_yc_b;
  real_T c4_yc_y;
  real_T c4_ad_a;
  real_T c4_d8;
  real_T c4_ad_b;
  real_T c4_ad_y;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 20U, 20U, c4_g_debug_family_names,
    c4_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_theta1, 0U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f1, 1U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_theta2, 2U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_f2, 3U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ddVddtheta1, 4U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ddVddf1, 5U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ddVddtheta2, 6U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ddVddf2, 7U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargin, 8U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_nargout, 9U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_input, 10U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fxref, 11U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Fyref, 12U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_Mref, 13U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx1, 14U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_rx2, 15U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry1, 16U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c4_ry2, 17U, c4_b_sf_marshallOut,
    c4_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_C, 18U, c4_d_sf_marshallOut,
    c4_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c4_output, 19U, c4_sf_marshallOut,
    c4_sf_marshallIn);
  CV_EML_FCN(0, 9);
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 218U);
  c4_theta1 = c4_input[0];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 219U);
  c4_f1 = c4_input[1];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 220U);
  c4_theta2 = c4_input[2];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 221U);
  c4_f2 = c4_input[3];
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 223U);
  c4_a = c4_f1;
  c4_b = c4_ry1;
  c4_y = c4_a * c4_b;
  c4_b_a = c4_y;
  c4_b_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_b_b);
  c4_b_y = c4_b_a * c4_b_b;
  c4_c_a = c4_f1;
  c4_c_b = c4_rx1;
  c4_c_y = c4_c_a * c4_c_b;
  c4_d_a = c4_c_y;
  c4_d_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_d_b);
  c4_d_y = c4_d_a * c4_d_b;
  c4_e_a = c4_C[2];
  c4_e_b = c4_mpower(chartInstance, c4_b_y - c4_d_y);
  c4_e_y = c4_e_a * c4_e_b;
  c4_f_a = c4_f1;
  c4_f_b = c4_rx1;
  c4_f_y = c4_f_a * c4_f_b;
  c4_g_a = c4_f_y;
  c4_g_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_g_b);
  c4_g_y = c4_g_a * c4_g_b;
  c4_h_a = c4_f1;
  c4_h_b = c4_ry1;
  c4_h_y = c4_h_a * c4_h_b;
  c4_i_a = c4_h_y;
  c4_i_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_i_b);
  c4_i_y = c4_i_a * c4_i_b;
  c4_j_a = c4_C[2];
  c4_j_b = c4_g_y + c4_i_y;
  c4_j_y = c4_j_a * c4_j_b;
  c4_k_a = c4_f1;
  c4_k_b = c4_rx1;
  c4_k_y = c4_k_a * c4_k_b;
  c4_l_a = c4_k_y;
  c4_l_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_l_b);
  c4_l_y = c4_l_a * c4_l_b;
  c4_m_a = c4_f2;
  c4_m_b = c4_rx2;
  c4_m_y = c4_m_a * c4_m_b;
  c4_n_a = c4_m_y;
  c4_n_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_n_b);
  c4_n_y = c4_n_a * c4_n_b;
  c4_o_a = c4_f1;
  c4_o_b = c4_ry1;
  c4_o_y = c4_o_a * c4_o_b;
  c4_p_a = c4_o_y;
  c4_p_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_p_b);
  c4_p_y = c4_p_a * c4_p_b;
  c4_q_a = c4_f2;
  c4_q_b = c4_ry2;
  c4_q_y = c4_q_a * c4_q_b;
  c4_r_a = c4_q_y;
  c4_r_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_r_b);
  c4_r_y = c4_r_a * c4_r_b;
  c4_s_a = c4_j_y;
  c4_s_b = (((c4_l_y - c4_Mref) + c4_n_y) + c4_p_y) + c4_r_y;
  c4_s_y = c4_s_a * c4_s_b;
  c4_t_a = c4_C[0];
  c4_t_b = c4_mpower(chartInstance, c4_f1);
  c4_t_y = c4_t_a * c4_t_b;
  c4_u_a = c4_t_y;
  c4_d1 = c4_theta1;
  c4_b_cosd(chartInstance, &c4_d1);
  c4_u_b = c4_mpower(chartInstance, c4_d1);
  c4_u_y = c4_u_a * c4_u_b;
  c4_v_a = c4_C[1];
  c4_v_b = c4_mpower(chartInstance, c4_f1);
  c4_v_y = c4_v_a * c4_v_b;
  c4_w_a = c4_v_y;
  c4_d2 = c4_theta1;
  c4_b_sind(chartInstance, &c4_d2);
  c4_w_b = c4_mpower(chartInstance, c4_d2);
  c4_w_y = c4_w_a * c4_w_b;
  c4_x_a = c4_C[1];
  c4_x_b = c4_f1;
  c4_x_y = c4_x_a * c4_x_b;
  c4_y_a = c4_x_y;
  c4_y_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_y_b);
  c4_y_y = c4_y_a * c4_y_b;
  c4_ab_a = c4_f1;
  c4_ab_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_ab_b);
  c4_ab_y = c4_ab_a * c4_ab_b;
  c4_bb_a = c4_f2;
  c4_bb_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_bb_b);
  c4_bb_y = c4_bb_a * c4_bb_b;
  c4_cb_a = c4_y_y;
  c4_cb_b = (c4_ab_y - c4_Fyref) + c4_bb_y;
  c4_cb_y = c4_cb_a * c4_cb_b;
  c4_db_a = c4_C[0];
  c4_db_b = c4_f1;
  c4_db_y = c4_db_a * c4_db_b;
  c4_eb_a = c4_db_y;
  c4_eb_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_eb_b);
  c4_eb_y = c4_eb_a * c4_eb_b;
  c4_fb_a = c4_f1;
  c4_fb_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_fb_b);
  c4_fb_y = c4_fb_a * c4_fb_b;
  c4_gb_a = c4_f2;
  c4_gb_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_gb_b);
  c4_gb_y = c4_gb_a * c4_gb_b;
  c4_hb_a = c4_eb_y;
  c4_hb_b = (c4_Fxref + c4_fb_y) + c4_gb_y;
  c4_hb_y = c4_hb_a * c4_hb_b;
  c4_ddVddtheta1 = (((((c4_C[5] + c4_e_y) - c4_s_y) + c4_u_y) + c4_w_y) -
                    c4_cb_y) - c4_hb_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 231U);
  c4_ib_a = c4_rx1;
  c4_ib_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_ib_b);
  c4_ib_y = c4_ib_a * c4_ib_b;
  c4_jb_a = c4_ry1;
  c4_jb_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_jb_b);
  c4_jb_y = c4_jb_a * c4_jb_b;
  c4_kb_a = c4_C[2];
  c4_kb_b = c4_mpower(chartInstance, c4_ib_y + c4_jb_y);
  c4_kb_y = c4_kb_a * c4_kb_b;
  c4_lb_a = c4_C[1];
  c4_d3 = c4_theta1;
  c4_b_cosd(chartInstance, &c4_d3);
  c4_lb_b = c4_mpower(chartInstance, c4_d3);
  c4_lb_y = c4_lb_a * c4_lb_b;
  c4_mb_a = c4_C[0];
  c4_d4 = c4_theta1;
  c4_b_sind(chartInstance, &c4_d4);
  c4_mb_b = c4_mpower(chartInstance, c4_d4);
  c4_mb_y = c4_mb_a * c4_mb_b;
  c4_ddVddf1 = ((c4_C[3] + c4_kb_y) + c4_lb_y) + c4_mb_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 235U);
  c4_nb_a = c4_f2;
  c4_nb_b = c4_ry2;
  c4_nb_y = c4_nb_a * c4_nb_b;
  c4_ob_a = c4_nb_y;
  c4_ob_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_ob_b);
  c4_ob_y = c4_ob_a * c4_ob_b;
  c4_pb_a = c4_f2;
  c4_pb_b = c4_rx2;
  c4_pb_y = c4_pb_a * c4_pb_b;
  c4_qb_a = c4_pb_y;
  c4_qb_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_qb_b);
  c4_qb_y = c4_qb_a * c4_qb_b;
  c4_rb_a = c4_C[2];
  c4_rb_b = c4_mpower(chartInstance, c4_ob_y - c4_qb_y);
  c4_rb_y = c4_rb_a * c4_rb_b;
  c4_sb_a = c4_f2;
  c4_sb_b = c4_rx2;
  c4_sb_y = c4_sb_a * c4_sb_b;
  c4_tb_a = c4_sb_y;
  c4_tb_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_tb_b);
  c4_tb_y = c4_tb_a * c4_tb_b;
  c4_ub_a = c4_f2;
  c4_ub_b = c4_ry2;
  c4_ub_y = c4_ub_a * c4_ub_b;
  c4_vb_a = c4_ub_y;
  c4_vb_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_vb_b);
  c4_vb_y = c4_vb_a * c4_vb_b;
  c4_wb_a = c4_C[2];
  c4_wb_b = c4_tb_y + c4_vb_y;
  c4_wb_y = c4_wb_a * c4_wb_b;
  c4_xb_a = c4_f1;
  c4_xb_b = c4_rx1;
  c4_xb_y = c4_xb_a * c4_xb_b;
  c4_yb_a = c4_xb_y;
  c4_yb_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_yb_b);
  c4_yb_y = c4_yb_a * c4_yb_b;
  c4_ac_a = c4_f2;
  c4_ac_b = c4_rx2;
  c4_ac_y = c4_ac_a * c4_ac_b;
  c4_bc_a = c4_ac_y;
  c4_bc_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_bc_b);
  c4_bc_y = c4_bc_a * c4_bc_b;
  c4_cc_a = c4_f1;
  c4_cc_b = c4_ry1;
  c4_cc_y = c4_cc_a * c4_cc_b;
  c4_dc_a = c4_cc_y;
  c4_dc_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_dc_b);
  c4_dc_y = c4_dc_a * c4_dc_b;
  c4_ec_a = c4_f2;
  c4_ec_b = c4_ry2;
  c4_ec_y = c4_ec_a * c4_ec_b;
  c4_fc_a = c4_ec_y;
  c4_fc_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_fc_b);
  c4_fc_y = c4_fc_a * c4_fc_b;
  c4_gc_a = c4_wb_y;
  c4_gc_b = (((c4_yb_y - c4_Mref) + c4_bc_y) + c4_dc_y) + c4_fc_y;
  c4_gc_y = c4_gc_a * c4_gc_b;
  c4_hc_a = c4_C[0];
  c4_hc_b = c4_mpower(chartInstance, c4_f2);
  c4_hc_y = c4_hc_a * c4_hc_b;
  c4_ic_a = c4_hc_y;
  c4_d5 = c4_theta2;
  c4_b_cosd(chartInstance, &c4_d5);
  c4_ic_b = c4_mpower(chartInstance, c4_d5);
  c4_ic_y = c4_ic_a * c4_ic_b;
  c4_jc_a = c4_C[1];
  c4_jc_b = c4_mpower(chartInstance, c4_f2);
  c4_jc_y = c4_jc_a * c4_jc_b;
  c4_kc_a = c4_jc_y;
  c4_d6 = c4_theta2;
  c4_b_sind(chartInstance, &c4_d6);
  c4_kc_b = c4_mpower(chartInstance, c4_d6);
  c4_kc_y = c4_kc_a * c4_kc_b;
  c4_lc_a = c4_C[1];
  c4_lc_b = c4_f2;
  c4_lc_y = c4_lc_a * c4_lc_b;
  c4_mc_a = c4_lc_y;
  c4_mc_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_mc_b);
  c4_mc_y = c4_mc_a * c4_mc_b;
  c4_nc_a = c4_f1;
  c4_nc_b = c4_theta1;
  c4_b_cosd(chartInstance, &c4_nc_b);
  c4_nc_y = c4_nc_a * c4_nc_b;
  c4_oc_a = c4_f2;
  c4_oc_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_oc_b);
  c4_oc_y = c4_oc_a * c4_oc_b;
  c4_pc_a = c4_mc_y;
  c4_pc_b = (c4_nc_y - c4_Fyref) + c4_oc_y;
  c4_pc_y = c4_pc_a * c4_pc_b;
  c4_qc_a = c4_C[0];
  c4_qc_b = c4_f2;
  c4_qc_y = c4_qc_a * c4_qc_b;
  c4_rc_a = c4_qc_y;
  c4_rc_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_rc_b);
  c4_rc_y = c4_rc_a * c4_rc_b;
  c4_sc_a = c4_f1;
  c4_sc_b = c4_theta1;
  c4_b_sind(chartInstance, &c4_sc_b);
  c4_sc_y = c4_sc_a * c4_sc_b;
  c4_tc_a = c4_f2;
  c4_tc_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_tc_b);
  c4_tc_y = c4_tc_a * c4_tc_b;
  c4_uc_a = c4_rc_y;
  c4_uc_b = (c4_Fxref + c4_sc_y) + c4_tc_y;
  c4_uc_y = c4_uc_a * c4_uc_b;
  c4_ddVddtheta2 = (((((c4_C[6] + c4_rb_y) - c4_gc_y) + c4_ic_y) + c4_kc_y) -
                    c4_pc_y) - c4_uc_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 243U);
  c4_vc_a = c4_rx2;
  c4_vc_b = c4_theta2;
  c4_b_cosd(chartInstance, &c4_vc_b);
  c4_vc_y = c4_vc_a * c4_vc_b;
  c4_wc_a = c4_ry2;
  c4_wc_b = c4_theta2;
  c4_b_sind(chartInstance, &c4_wc_b);
  c4_wc_y = c4_wc_a * c4_wc_b;
  c4_xc_a = c4_C[2];
  c4_xc_b = c4_mpower(chartInstance, c4_vc_y + c4_wc_y);
  c4_xc_y = c4_xc_a * c4_xc_b;
  c4_yc_a = c4_C[1];
  c4_d7 = c4_theta2;
  c4_b_cosd(chartInstance, &c4_d7);
  c4_yc_b = c4_mpower(chartInstance, c4_d7);
  c4_yc_y = c4_yc_a * c4_yc_b;
  c4_ad_a = c4_C[0];
  c4_d8 = c4_theta2;
  c4_b_sind(chartInstance, &c4_d8);
  c4_ad_b = c4_mpower(chartInstance, c4_d8);
  c4_ad_y = c4_ad_a * c4_ad_b;
  c4_ddVddf2 = ((c4_C[4] + c4_xc_y) + c4_yc_y) + c4_ad_y;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, 248U);
  c4_output[0] = c4_ddVddtheta1;
  c4_output[1] = c4_ddVddf1;
  c4_output[2] = c4_ddVddtheta2;
  c4_output[3] = c4_ddVddf2;
  _SFD_EML_CALL(0U, chartInstance->c4_sfEvent, -248);
  _SFD_SYMBOL_SCOPE_POP();
}

static const mxArray *c4_i_sf_marshallOut(void *chartInstanceVoid, void
  *c4_inData)
{
  const mxArray *c4_mxArrayOutData = NULL;
  int32_T c4_u;
  const mxArray *c4_y = NULL;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_mxArrayOutData = NULL;
  c4_u = *(int32_T *)c4_inData;
  c4_y = NULL;
  sf_mex_assign(&c4_y, sf_mex_create("y", &c4_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c4_mxArrayOutData, c4_y, FALSE);
  return c4_mxArrayOutData;
}

static int32_T c4_j_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId)
{
  int32_T c4_y;
  int32_T c4_i117;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), &c4_i117, 1, 6, 0U, 0, 0U, 0);
  c4_y = c4_i117;
  sf_mex_destroy(&c4_u);
  return c4_y;
}

static void c4_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c4_mxArrayInData, const char_T *c4_varName, void *c4_outData)
{
  const mxArray *c4_b_sfEvent;
  const char_T *c4_identifier;
  emlrtMsgIdentifier c4_thisId;
  int32_T c4_y;
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c4_b_sfEvent = sf_mex_dup(c4_mxArrayInData);
  c4_identifier = c4_varName;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_y = c4_j_emlrt_marshallIn(chartInstance, sf_mex_dup(c4_b_sfEvent),
    &c4_thisId);
  sf_mex_destroy(&c4_b_sfEvent);
  *(int32_T *)c4_outData = c4_y;
  sf_mex_destroy(&c4_mxArrayInData);
}

static uint8_T c4_k_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_b_is_active_c4_BoatLibraryTest01, const
  char_T *c4_identifier)
{
  uint8_T c4_y;
  emlrtMsgIdentifier c4_thisId;
  c4_thisId.fIdentifier = c4_identifier;
  c4_thisId.fParent = NULL;
  c4_y = c4_l_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c4_b_is_active_c4_BoatLibraryTest01), &c4_thisId);
  sf_mex_destroy(&c4_b_is_active_c4_BoatLibraryTest01);
  return c4_y;
}

static uint8_T c4_l_emlrt_marshallIn(SFc4_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c4_u, const emlrtMsgIdentifier *c4_parentId)
{
  uint8_T c4_y;
  uint8_T c4_u0;
  sf_mex_import(c4_parentId, sf_mex_dup(c4_u), &c4_u0, 1, 3, 0U, 0, 0U, 0);
  c4_y = c4_u0;
  sf_mex_destroy(&c4_u);
  return c4_y;
}

static void c4_b_sind(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
                      real_T *c4_x)
{
  real_T c4_b_x;
  real_T c4_d9;
  real_T c4_c_x;
  real_T c4_d_x;
  real_T c4_e_x;
  real_T c4_xk;
  real_T c4_f_x;
  real_T c4_g_x;
  real_T c4_absx;
  real_T c4_h_x;
  real_T c4_i_x;
  real_T c4_b;
  int8_T c4_n;
  real_T c4_b_b;
  real_T c4_c_b;
  real_T c4_d_b;
  real_T c4_e_b;
  int8_T c4_b_n;
  c4_b_x = *c4_x;
  if (!c4_isfinite(chartInstance, c4_b_x)) {
    c4_d9 = rtNaN;
  } else {
    c4_c_x = c4_b_x;
    c4_d_x = c4_c_x;
    c4_e_x = c4_d_x;
    c4_eml_scalar_eg(chartInstance);
    c4_xk = c4_e_x;
    c4_d_x = muDoubleScalarRem(c4_xk, 360.0);
    c4_f_x = c4_d_x;
    c4_g_x = c4_f_x;
    c4_absx = muDoubleScalarAbs(c4_g_x);
    if (c4_absx > 180.0) {
      if (c4_d_x > 0.0) {
        c4_d_x -= 360.0;
      } else {
        c4_d_x += 360.0;
      }

      c4_h_x = c4_d_x;
      c4_i_x = c4_h_x;
      c4_absx = muDoubleScalarAbs(c4_i_x);
    }

    if (c4_absx <= 45.0) {
      c4_b = c4_d_x;
      c4_d_x = 0.017453292519943295 * c4_b;
      c4_n = 0;
    } else if (c4_absx <= 135.0) {
      if (c4_d_x > 0.0) {
        c4_b_b = c4_d_x - 90.0;
        c4_d_x = 0.017453292519943295 * c4_b_b;
        c4_n = 1;
      } else {
        c4_c_b = c4_d_x + 90.0;
        c4_d_x = 0.017453292519943295 * c4_c_b;
        c4_n = -1;
      }
    } else if (c4_d_x > 0.0) {
      c4_d_b = c4_d_x - 180.0;
      c4_d_x = 0.017453292519943295 * c4_d_b;
      c4_n = 2;
    } else {
      c4_e_b = c4_d_x + 180.0;
      c4_d_x = 0.017453292519943295 * c4_e_b;
      c4_n = -2;
    }

    c4_b_n = c4_n;
    c4_b_x = c4_d_x;
    if ((real_T)c4_b_n == 0.0) {
      c4_d9 = muDoubleScalarSin(c4_b_x);
    } else if ((real_T)c4_b_n == 1.0) {
      c4_d9 = muDoubleScalarCos(c4_b_x);
    } else if ((real_T)c4_b_n == -1.0) {
      c4_d9 = -muDoubleScalarCos(c4_b_x);
    } else {
      c4_d9 = -muDoubleScalarSin(c4_b_x);
    }
  }

  *c4_x = c4_d9;
}

static void c4_b_cosd(SFc4_BoatLibraryTest01InstanceStruct *chartInstance,
                      real_T *c4_x)
{
  real_T c4_b_x;
  real_T c4_c_x;
  real_T c4_d_x;
  real_T c4_e_x;
  real_T c4_xk;
  real_T c4_f_x;
  real_T c4_g_x;
  real_T c4_absx;
  real_T c4_h_x;
  real_T c4_i_x;
  real_T c4_b;
  int8_T c4_n;
  real_T c4_b_b;
  real_T c4_c_b;
  real_T c4_d_b;
  real_T c4_e_b;
  int8_T c4_b_n;
  c4_b_x = *c4_x;
  if (!c4_isfinite(chartInstance, c4_b_x)) {
    *c4_x = rtNaN;
  } else {
    c4_c_x = c4_b_x;
    c4_d_x = c4_c_x;
    c4_e_x = c4_d_x;
    c4_eml_scalar_eg(chartInstance);
    c4_xk = c4_e_x;
    c4_d_x = muDoubleScalarRem(c4_xk, 360.0);
    c4_f_x = c4_d_x;
    c4_g_x = c4_f_x;
    c4_absx = muDoubleScalarAbs(c4_g_x);
    if (c4_absx > 180.0) {
      if (c4_d_x > 0.0) {
        c4_d_x -= 360.0;
      } else {
        c4_d_x += 360.0;
      }

      c4_h_x = c4_d_x;
      c4_i_x = c4_h_x;
      c4_absx = muDoubleScalarAbs(c4_i_x);
    }

    if (c4_absx <= 45.0) {
      c4_b = c4_d_x;
      c4_d_x = 0.017453292519943295 * c4_b;
      c4_n = 0;
    } else if (c4_absx <= 135.0) {
      if (c4_d_x > 0.0) {
        c4_b_b = c4_d_x - 90.0;
        c4_d_x = 0.017453292519943295 * c4_b_b;
        c4_n = 1;
      } else {
        c4_c_b = c4_d_x + 90.0;
        c4_d_x = 0.017453292519943295 * c4_c_b;
        c4_n = -1;
      }
    } else if (c4_d_x > 0.0) {
      c4_d_b = c4_d_x - 180.0;
      c4_d_x = 0.017453292519943295 * c4_d_b;
      c4_n = 2;
    } else {
      c4_e_b = c4_d_x + 180.0;
      c4_d_x = 0.017453292519943295 * c4_e_b;
      c4_n = -2;
    }

    c4_b_n = c4_n;
    c4_b_x = c4_d_x;
    if ((real_T)c4_b_n == 0.0) {
      *c4_x = muDoubleScalarCos(c4_b_x);
    } else if ((real_T)c4_b_n == 1.0) {
      *c4_x = -muDoubleScalarSin(c4_b_x);
    } else if ((real_T)c4_b_n == -1.0) {
      *c4_x = muDoubleScalarSin(c4_b_x);
    } else {
      *c4_x = -muDoubleScalarCos(c4_b_x);
    }
  }
}

static void init_dsm_address_info(SFc4_BoatLibraryTest01InstanceStruct
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

void sf_c4_BoatLibraryTest01_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3356827816U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1251006258U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3346537313U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(871453432U);
}

mxArray *sf_c4_BoatLibraryTest01_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("8ykukiY3HLPb4rwzq0kRGG");
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

mxArray *sf_c4_BoatLibraryTest01_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c4_BoatLibraryTest01_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c4_BoatLibraryTest01(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"output\",},{M[8],M[0],T\"is_active_c4_BoatLibraryTest01\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c4_BoatLibraryTest01_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
    chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _BoatLibraryTest01MachineNumber_,
           4,
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
          init_script_number_translation(_BoatLibraryTest01MachineNumber_,
            chartInstance->chartNumber);
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_BoatLibraryTest01MachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _BoatLibraryTest01MachineNumber_,
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
        _SFD_CV_INIT_EML(0,1,10,6,0,0,0,7,2,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,647);
        _SFD_CV_INIT_EML_FCN(0,1,"getMinFromGrid",649,-1,1365);
        _SFD_CV_INIT_EML_FCN(0,2,"checkInputBound",1367,-1,1535);
        _SFD_CV_INIT_EML_FCN(0,3,"boundGrid",1537,-1,1873);
        _SFD_CV_INIT_EML_FCN(0,4,"sat",1875,-1,2063);
        _SFD_CV_INIT_EML_FCN(0,5,"getStepSize",2065,-1,2937);
        _SFD_CV_INIT_EML_FCN(0,6,"costFunction",2939,-1,3672);
        _SFD_CV_INIT_EML_FCN(0,7,"getDirection",3674,-1,4089);
        _SFD_CV_INIT_EML_FCN(0,8,"SteepestDescent",4091,-1,5766);
        _SFD_CV_INIT_EML_FCN(0,9,"Newtons",5768,-1,7169);
        _SFD_CV_INIT_EML_IF(0,1,0,1156,1166,1248,1320);
        _SFD_CV_INIT_EML_IF(0,1,1,1916,1935,1967,2058);
        _SFD_CV_INIT_EML_IF(0,1,2,1967,1990,2022,2058);
        _SFD_CV_INIT_EML_IF(0,1,3,2609,2619,2427,2658);
        _SFD_CV_INIT_EML_IF(0,1,4,2862,2872,2664,2911);
        _SFD_CV_INIT_EML_IF(0,1,5,3970,3988,4029,4077);
        _SFD_CV_INIT_EML_FOR(0,1,0,368,381,620);
        _SFD_CV_INIT_EML_FOR(0,1,1,903,916,1360);
        _SFD_CV_INIT_EML_FOR(0,1,2,920,933,1356);
        _SFD_CV_INIT_EML_FOR(0,1,3,941,954,1348);
        _SFD_CV_INIT_EML_FOR(0,1,4,966,979,1336);
        _SFD_CV_INIT_EML_FOR(0,1,5,1438,1462,1515);
        _SFD_CV_INIT_EML_FOR(0,1,6,3940,3961,4085);
        _SFD_CV_INIT_EML_WHILE(0,1,0,2427,2441,2658);
        _SFD_CV_INIT_EML_WHILE(0,1,1,2664,2691,2911);

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c4_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c4_sf_marshallOut,(MexInFcnForType)
            c4_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c4_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c4_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c4_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c4_b_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 7;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c4_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 4;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c4_c_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c4_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c4_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c4_b_sf_marshallOut,(MexInFcnForType)NULL);

        {
          real_T *c4_rx01;
          real_T *c4_rx02;
          real_T *c4_ry01;
          real_T *c4_ry02;
          real_T *c4_Fx;
          real_T *c4_Fy;
          real_T *c4_M;
          real_T (*c4_u0)[4];
          real_T (*c4_output)[4];
          real_T (*c4_C0)[7];
          real_T (*c4_inputBound)[8];
          c4_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
          c4_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
          c4_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
          c4_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S,
            6);
          c4_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
          c4_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
          c4_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c4_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c4_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c4_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
          c4_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, *c4_u0);
          _SFD_SET_DATA_VALUE_PTR(1U, *c4_output);
          _SFD_SET_DATA_VALUE_PTR(2U, c4_rx01);
          _SFD_SET_DATA_VALUE_PTR(3U, c4_rx02);
          _SFD_SET_DATA_VALUE_PTR(4U, c4_ry01);
          _SFD_SET_DATA_VALUE_PTR(5U, c4_ry02);
          _SFD_SET_DATA_VALUE_PTR(6U, *c4_C0);
          _SFD_SET_DATA_VALUE_PTR(7U, *c4_inputBound);
          _SFD_SET_DATA_VALUE_PTR(8U, c4_Fx);
          _SFD_SET_DATA_VALUE_PTR(9U, c4_Fy);
          _SFD_SET_DATA_VALUE_PTR(10U, c4_M);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _BoatLibraryTest01MachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "nMy32IW8BmmBtGNgxM7DnE";
}

static void sf_opaque_initialize_c4_BoatLibraryTest01(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc4_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
  initialize_c4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c4_BoatLibraryTest01(void *chartInstanceVar)
{
  enable_c4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c4_BoatLibraryTest01(void *chartInstanceVar)
{
  disable_c4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c4_BoatLibraryTest01(void *chartInstanceVar)
{
  sf_c4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c4_BoatLibraryTest01(SimStruct*
  S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c4_BoatLibraryTest01
    ((SFc4_BoatLibraryTest01InstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c4_BoatLibraryTest01();/* state var info */
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

extern void sf_internal_set_sim_state_c4_BoatLibraryTest01(SimStruct* S, const
  mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c4_BoatLibraryTest01();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c4_BoatLibraryTest01(SimStruct* S)
{
  return sf_internal_get_sim_state_c4_BoatLibraryTest01(S);
}

static void sf_opaque_set_sim_state_c4_BoatLibraryTest01(SimStruct* S, const
  mxArray *st)
{
  sf_internal_set_sim_state_c4_BoatLibraryTest01(S, st);
}

static void sf_opaque_terminate_c4_BoatLibraryTest01(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc4_BoatLibraryTest01InstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_BoatLibraryTest01_optimization_info();
    }

    finalize_c4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
      chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c4_BoatLibraryTest01(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c4_BoatLibraryTest01((SFc4_BoatLibraryTest01InstanceStruct*)
      (((ChartInfoStruct *)ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c4_BoatLibraryTest01(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_BoatLibraryTest01_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      4);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,4,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,4,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,4);
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
        infoStruct,4,10);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,4,1);
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

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,4);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2172511737U));
  ssSetChecksum1(S,(2994793895U));
  ssSetChecksum2(S,(2833440349U));
  ssSetChecksum3(S,(332412096U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c4_BoatLibraryTest01(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c4_BoatLibraryTest01(SimStruct *S)
{
  SFc4_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc4_BoatLibraryTest01InstanceStruct *)utMalloc(sizeof
    (SFc4_BoatLibraryTest01InstanceStruct));
  memset(chartInstance, 0, sizeof(SFc4_BoatLibraryTest01InstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c4_BoatLibraryTest01;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c4_BoatLibraryTest01;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c4_BoatLibraryTest01;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c4_BoatLibraryTest01;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c4_BoatLibraryTest01;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c4_BoatLibraryTest01;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c4_BoatLibraryTest01;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c4_BoatLibraryTest01;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c4_BoatLibraryTest01;
  chartInstance->chartInfo.mdlStart = mdlStart_c4_BoatLibraryTest01;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c4_BoatLibraryTest01;
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

void c4_BoatLibraryTest01_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c4_BoatLibraryTest01(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c4_BoatLibraryTest01(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c4_BoatLibraryTest01(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c4_BoatLibraryTest01_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
