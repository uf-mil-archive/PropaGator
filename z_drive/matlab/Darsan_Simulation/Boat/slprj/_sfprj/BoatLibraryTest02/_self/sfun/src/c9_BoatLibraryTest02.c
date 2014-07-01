/* Include files */

#include <stddef.h>
#include "blas.h"
#include "BoatLibraryTest02_sfun.h"
#include "c9_BoatLibraryTest02.h"
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
static const char * c9_debug_family_names[27] = { "rx1", "rx2", "ry1", "ry2",
  "C", "Fxref", "Fyref", "Mref", "gridNum", "grid", "u1", "k", "h", "alpha",
  "nargin", "nargout", "u0", "rx01", "rx02", "ry01", "ry02", "C0", "inputBound",
  "Fx", "Fy", "M", "output" };

static const char * c9_b_debug_family_names[10] = { "p", "theta1", "f1",
  "theta2", "f2", "gridNum", "nargin", "nargout", "inputBound", "output" };

static const char * c9_c_debug_family_names[23] = { "theta1", "f1", "theta2",
  "f2", "pretheta1", "pretheta2", "dVdtheta1", "dVdf1", "dVdtheta2", "dVdf2",
  "nargin", "nargout", "input", "preinput", "Fxref", "Fyref", "Mref", "rx1",
  "rx2", "ry1", "ry2", "C", "output" };

static const char * c9_d_debug_family_names[15] = { "hsd", "i", "nargin",
  "nargout", "input", "preinput", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1",
  "ry2", "C", "output" };

static const char * c9_e_debug_family_names[22] = { "theta1", "f1", "theta2",
  "f2", "pretheta1", "pretheta2", "Fx", "Fy", "M", "nargin", "nargout", "input",
  "preinput", "Fxref", "Fyref", "Mref", "rx1", "rx2", "ry1", "ry2", "C",
  "output" };

static const char * c9_f_debug_family_names[19] = { "alpha", "u0", "u1", "v0",
  "v1", "nargin", "nargout", "input", "preinput", "h", "Fxref", "Fyref", "Mref",
  "rx1", "rx2", "ry1", "ry2", "C", "output" };

static const char * c9_g_debug_family_names[5] = { "nargin", "nargout", "input",
  "bound", "output" };

static const char * c9_h_debug_family_names[7] = { "k", "i", "nargin", "nargout",
  "input", "inputBound", "output" };

/* Function Declarations */
static void initialize_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void initialize_params_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance);
static void enable_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void disable_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void c9_update_debugger_state_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance);
static const mxArray *get_sim_state_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance);
static void set_sim_state_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance, const mxArray *c9_st);
static void finalize_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void sf_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance);
static void c9_chartstep_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance);
static void initSimStructsc9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c9_machineNumber, uint32_T
  c9_chartNumber);
static const mxArray *c9_sf_marshallOut(void *chartInstanceVoid, void *c9_inData);
static void c9_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_output, const char_T *c9_identifier, real_T
  c9_y[4]);
static void c9_b_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[4]);
static void c9_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData);
static const mxArray *c9_b_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData);
static const mxArray *c9_c_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData);
static const mxArray *c9_d_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData);
static real_T c9_c_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId);
static void c9_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData);
static const mxArray *c9_e_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData);
static void c9_d_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[20]);
static void c9_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData);
static void c9_e_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[7]);
static void c9_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData);
static void c9_f_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[8]);
static void c9_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData);
static const mxArray *c9_f_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData);
static void c9_g_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[5]);
static void c9_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData);
static const mxArray *c9_g_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData);
static void c9_h_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[2]);
static void c9_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData);
static void c9_info_helper(const mxArray **c9_info);
static const mxArray *c9_emlrt_marshallOut(char * c9_u);
static const mxArray *c9_b_emlrt_marshallOut(uint32_T c9_u);
static void c9_eml_scalar_eg(SFc9_BoatLibraryTest02InstanceStruct *chartInstance);
static void c9_linspace(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c9_d1, real_T c9_d2, real_T c9_y[5]);
static void c9_SteepestDescent(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c9_input[4], real_T c9_preinput[4], real_T c9_Fxref,
  real_T c9_Fyref, real_T c9_Mref, real_T c9_rx1, real_T c9_rx2, real_T c9_ry1,
  real_T c9_ry2, real_T c9_C[7], real_T c9_output[4]);
static real_T c9_cosd(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
                      real_T c9_x);
static boolean_T c9_isfinite(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c9_x);
static real_T c9_sind(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
                      real_T c9_x);
static real_T c9_costFunction(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c9_input[4], real_T c9_preinput[4], real_T c9_Fxref,
  real_T c9_Fyref, real_T c9_Mref, real_T c9_rx1, real_T c9_rx2, real_T c9_ry1,
  real_T c9_ry2, real_T c9_C[7]);
static real_T c9_mpower(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c9_a);
static const mxArray *c9_h_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData);
static int32_T c9_i_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId);
static void c9_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData);
static uint8_T c9_j_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_b_is_active_c9_BoatLibraryTest02, const
  char_T *c9_identifier);
static uint8_T c9_k_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId);
static void c9_b_cosd(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
                      real_T *c9_x);
static void c9_b_sind(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
                      real_T *c9_x);
static void init_dsm_address_info(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
  chartInstance->c9_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c9_is_active_c9_BoatLibraryTest02 = 0U;
}

static void initialize_params_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance)
{
}

static void enable_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c9_update_debugger_state_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance)
{
}

static const mxArray *get_sim_state_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance)
{
  const mxArray *c9_st;
  const mxArray *c9_y = NULL;
  int32_T c9_i0;
  real_T c9_u[4];
  const mxArray *c9_b_y = NULL;
  uint8_T c9_hoistedGlobal;
  uint8_T c9_b_u;
  const mxArray *c9_c_y = NULL;
  real_T (*c9_output)[4];
  c9_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c9_st = NULL;
  c9_st = NULL;
  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_createcellarray(2), FALSE);
  for (c9_i0 = 0; c9_i0 < 4; c9_i0++) {
    c9_u[c9_i0] = (*c9_output)[c9_i0];
  }

  c9_b_y = NULL;
  sf_mex_assign(&c9_b_y, sf_mex_create("y", c9_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_setcell(c9_y, 0, c9_b_y);
  c9_hoistedGlobal = chartInstance->c9_is_active_c9_BoatLibraryTest02;
  c9_b_u = c9_hoistedGlobal;
  c9_c_y = NULL;
  sf_mex_assign(&c9_c_y, sf_mex_create("y", &c9_b_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c9_y, 1, c9_c_y);
  sf_mex_assign(&c9_st, c9_y, FALSE);
  return c9_st;
}

static void set_sim_state_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance, const mxArray *c9_st)
{
  const mxArray *c9_u;
  real_T c9_dv0[4];
  int32_T c9_i1;
  real_T (*c9_output)[4];
  c9_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c9_doneDoubleBufferReInit = TRUE;
  c9_u = sf_mex_dup(c9_st);
  c9_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c9_u, 0)),
                      "output", c9_dv0);
  for (c9_i1 = 0; c9_i1 < 4; c9_i1++) {
    (*c9_output)[c9_i1] = c9_dv0[c9_i1];
  }

  chartInstance->c9_is_active_c9_BoatLibraryTest02 = c9_j_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c9_u, 1)),
     "is_active_c9_BoatLibraryTest02");
  sf_mex_destroy(&c9_u);
  c9_update_debugger_state_c9_BoatLibraryTest02(chartInstance);
  sf_mex_destroy(&c9_st);
}

static void finalize_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
}

static void sf_c9_BoatLibraryTest02(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance)
{
  int32_T c9_i2;
  int32_T c9_i3;
  int32_T c9_i4;
  int32_T c9_i5;
  real_T *c9_rx01;
  real_T *c9_rx02;
  real_T *c9_ry01;
  real_T *c9_ry02;
  real_T *c9_Fx;
  real_T *c9_Fy;
  real_T *c9_M;
  real_T (*c9_inputBound)[8];
  real_T (*c9_C0)[7];
  real_T (*c9_output)[4];
  real_T (*c9_u0)[4];
  c9_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c9_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c9_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c9_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S, 6);
  c9_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
  c9_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c9_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c9_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c9_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c9_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c9_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 8U, chartInstance->c9_sfEvent);
  for (c9_i2 = 0; c9_i2 < 4; c9_i2++) {
    _SFD_DATA_RANGE_CHECK((*c9_u0)[c9_i2], 0U);
  }

  for (c9_i3 = 0; c9_i3 < 4; c9_i3++) {
    _SFD_DATA_RANGE_CHECK((*c9_output)[c9_i3], 1U);
  }

  _SFD_DATA_RANGE_CHECK(*c9_rx01, 2U);
  _SFD_DATA_RANGE_CHECK(*c9_rx02, 3U);
  _SFD_DATA_RANGE_CHECK(*c9_ry01, 4U);
  _SFD_DATA_RANGE_CHECK(*c9_ry02, 5U);
  for (c9_i4 = 0; c9_i4 < 7; c9_i4++) {
    _SFD_DATA_RANGE_CHECK((*c9_C0)[c9_i4], 6U);
  }

  for (c9_i5 = 0; c9_i5 < 8; c9_i5++) {
    _SFD_DATA_RANGE_CHECK((*c9_inputBound)[c9_i5], 7U);
  }

  _SFD_DATA_RANGE_CHECK(*c9_Fx, 8U);
  _SFD_DATA_RANGE_CHECK(*c9_Fy, 9U);
  _SFD_DATA_RANGE_CHECK(*c9_M, 10U);
  chartInstance->c9_sfEvent = CALL_EVENT;
  c9_chartstep_c9_BoatLibraryTest02(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_BoatLibraryTest02MachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c9_chartstep_c9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance)
{
  real_T c9_hoistedGlobal;
  real_T c9_b_hoistedGlobal;
  real_T c9_c_hoistedGlobal;
  real_T c9_d_hoistedGlobal;
  real_T c9_e_hoistedGlobal;
  real_T c9_f_hoistedGlobal;
  real_T c9_g_hoistedGlobal;
  int32_T c9_i6;
  real_T c9_u0[4];
  real_T c9_rx01;
  real_T c9_rx02;
  real_T c9_ry01;
  real_T c9_ry02;
  int32_T c9_i7;
  real_T c9_C0[7];
  int32_T c9_i8;
  real_T c9_inputBound[8];
  real_T c9_Fx;
  real_T c9_Fy;
  real_T c9_M;
  uint32_T c9_debug_family_var_map[27];
  real_T c9_rx1;
  real_T c9_rx2;
  real_T c9_ry1;
  real_T c9_ry2;
  real_T c9_C[7];
  real_T c9_Fxref;
  real_T c9_Fyref;
  real_T c9_Mref;
  real_T c9_gridNum;
  real_T c9_grid[20];
  real_T c9_u1[4];
  real_T c9_k;
  real_T c9_h[4];
  real_T c9_alpha;
  real_T c9_nargin = 10.0;
  real_T c9_nargout = 1.0;
  real_T c9_output[4];
  int32_T c9_i9;
  int32_T c9_i10;
  real_T c9_b_inputBound[8];
  uint32_T c9_b_debug_family_var_map[10];
  real_T c9_p;
  real_T c9_theta1[5];
  real_T c9_f1[5];
  real_T c9_theta2[5];
  real_T c9_f2[5];
  real_T c9_b_gridNum;
  real_T c9_b_nargin = 2.0;
  real_T c9_b_nargout = 1.0;
  real_T c9_dv1[5];
  int32_T c9_i11;
  int32_T c9_i12;
  int32_T c9_i13;
  int32_T c9_i14;
  int32_T c9_i15;
  int32_T c9_i16;
  int32_T c9_i17;
  int32_T c9_i18;
  int32_T c9_i19;
  int32_T c9_b_k;
  int32_T c9_i20;
  real_T c9_input[4];
  int32_T c9_i21;
  real_T c9_preinput[4];
  real_T c9_b_Fxref;
  real_T c9_b_Fyref;
  real_T c9_b_Mref;
  real_T c9_b_rx1;
  real_T c9_b_rx2;
  real_T c9_b_ry1;
  real_T c9_b_ry2;
  int32_T c9_i22;
  real_T c9_b_C[7];
  uint32_T c9_c_debug_family_var_map[15];
  real_T c9_hsd[4];
  real_T c9_i;
  real_T c9_c_nargin = 10.0;
  real_T c9_c_nargout = 1.0;
  int32_T c9_i23;
  real_T c9_b_input[4];
  int32_T c9_i24;
  real_T c9_b_preinput[4];
  int32_T c9_i25;
  real_T c9_c_C[7];
  real_T c9_dv2[4];
  int32_T c9_i26;
  int32_T c9_i27;
  int32_T c9_b_i;
  int32_T c9_i28;
  real_T c9_c_input[4];
  int32_T c9_i29;
  real_T c9_c_preinput[4];
  int32_T c9_i30;
  real_T c9_b_h[4];
  real_T c9_c_Fxref;
  real_T c9_c_Fyref;
  real_T c9_c_Mref;
  real_T c9_c_rx1;
  real_T c9_c_rx2;
  real_T c9_c_ry1;
  real_T c9_c_ry2;
  int32_T c9_i31;
  real_T c9_d_C[7];
  uint32_T c9_d_debug_family_var_map[19];
  real_T c9_b_alpha;
  real_T c9_b_u0[4];
  real_T c9_b_u1[4];
  real_T c9_v0;
  real_T c9_v1;
  real_T c9_d_nargin = 11.0;
  real_T c9_d_nargout = 1.0;
  int32_T c9_i32;
  int32_T c9_i33;
  real_T c9_b[4];
  int32_T c9_i34;
  int32_T c9_i35;
  real_T c9_c_u0[4];
  int32_T c9_i36;
  real_T c9_d_preinput[4];
  int32_T c9_i37;
  real_T c9_e_C[7];
  int32_T c9_i38;
  real_T c9_c_u1[4];
  int32_T c9_i39;
  real_T c9_e_preinput[4];
  int32_T c9_i40;
  real_T c9_f_C[7];
  real_T c9_a;
  int32_T c9_i41;
  int32_T c9_i42;
  int32_T c9_i43;
  int32_T c9_i44;
  real_T c9_d_u1[4];
  int32_T c9_i45;
  real_T c9_f_preinput[4];
  int32_T c9_i46;
  real_T c9_g_C[7];
  real_T c9_b_a;
  int32_T c9_i47;
  int32_T c9_i48;
  int32_T c9_i49;
  int32_T c9_i50;
  real_T c9_e_u1[4];
  int32_T c9_i51;
  real_T c9_g_preinput[4];
  int32_T c9_i52;
  real_T c9_h_C[7];
  real_T c9_c_a;
  int32_T c9_i53;
  int32_T c9_i54;
  int32_T c9_i55;
  int32_T c9_i56;
  real_T c9_d_input[4];
  int32_T c9_i57;
  real_T c9_c_inputBound[8];
  uint32_T c9_e_debug_family_var_map[7];
  real_T c9_c_k[4];
  real_T c9_c_i;
  real_T c9_e_nargin = 2.0;
  real_T c9_e_nargout = 1.0;
  int32_T c9_i58;
  int32_T c9_d_i;
  real_T c9_e_input;
  int32_T c9_e_i;
  int32_T c9_i59;
  real_T c9_bound[2];
  uint32_T c9_f_debug_family_var_map[5];
  real_T c9_f_nargin = 2.0;
  real_T c9_f_nargout = 1.0;
  real_T c9_b_output;
  int32_T c9_i60;
  int32_T c9_i61;
  int32_T c9_i62;
  real_T *c9_b_rx01;
  real_T *c9_b_rx02;
  real_T *c9_b_ry01;
  real_T *c9_b_ry02;
  real_T *c9_b_Fx;
  real_T *c9_b_Fy;
  real_T *c9_b_M;
  real_T (*c9_c_output)[4];
  real_T (*c9_d_inputBound)[8];
  real_T (*c9_b_C0)[7];
  real_T (*c9_d_u0)[4];
  boolean_T exitg1;
  boolean_T exitg2;
  c9_b_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c9_b_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c9_b_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c9_d_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S, 6);
  c9_b_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
  c9_b_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c9_b_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c9_b_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c9_b_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c9_c_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c9_d_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 8U, chartInstance->c9_sfEvent);
  c9_hoistedGlobal = *c9_b_rx01;
  c9_b_hoistedGlobal = *c9_b_rx02;
  c9_c_hoistedGlobal = *c9_b_ry01;
  c9_d_hoistedGlobal = *c9_b_ry02;
  c9_e_hoistedGlobal = *c9_b_Fx;
  c9_f_hoistedGlobal = *c9_b_Fy;
  c9_g_hoistedGlobal = *c9_b_M;
  for (c9_i6 = 0; c9_i6 < 4; c9_i6++) {
    c9_u0[c9_i6] = (*c9_d_u0)[c9_i6];
  }

  c9_rx01 = c9_hoistedGlobal;
  c9_rx02 = c9_b_hoistedGlobal;
  c9_ry01 = c9_c_hoistedGlobal;
  c9_ry02 = c9_d_hoistedGlobal;
  for (c9_i7 = 0; c9_i7 < 7; c9_i7++) {
    c9_C0[c9_i7] = (*c9_b_C0)[c9_i7];
  }

  for (c9_i8 = 0; c9_i8 < 8; c9_i8++) {
    c9_inputBound[c9_i8] = (*c9_d_inputBound)[c9_i8];
  }

  c9_Fx = c9_e_hoistedGlobal;
  c9_Fy = c9_f_hoistedGlobal;
  c9_M = c9_g_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 27U, 27U, c9_debug_family_names,
    c9_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_rx1, 0U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_rx2, 1U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_ry1, 2U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_ry2, 3U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_C, 4U, c9_d_sf_marshallOut,
    c9_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Fxref, 5U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Fyref, 6U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Mref, 7U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_gridNum, 8U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_grid, 9U, c9_e_sf_marshallOut,
    c9_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_u1, 10U, c9_sf_marshallOut,
    c9_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_k, 11U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_h, 12U, c9_sf_marshallOut,
    c9_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_alpha, 13U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_nargin, 14U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_nargout, 15U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c9_u0, 16U, c9_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_rx01, 17U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_rx02, 18U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_ry01, 19U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_ry02, 20U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c9_C0, 21U, c9_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c9_inputBound, 22U, c9_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_Fx, 23U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_Fy, 24U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_M, 25U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_output, 26U, c9_sf_marshallOut,
    c9_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 3);
  c9_rx1 = c9_rx01;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 3);
  c9_rx2 = c9_rx02;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 4);
  c9_ry1 = c9_ry01;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 4);
  c9_ry2 = c9_ry02;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 6);
  for (c9_i9 = 0; c9_i9 < 7; c9_i9++) {
    c9_C[c9_i9] = c9_C0[c9_i9];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 8);
  c9_Fxref = c9_Fx;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 8);
  c9_Fyref = c9_Fy;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 8);
  c9_Mref = c9_M;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 10);
  c9_gridNum = 5.0;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 12);
  for (c9_i10 = 0; c9_i10 < 8; c9_i10++) {
    c9_b_inputBound[c9_i10] = c9_inputBound[c9_i10];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 10U, 10U, c9_b_debug_family_names,
    c9_b_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_p, 0U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_theta1, 1U, c9_f_sf_marshallOut,
    c9_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_f1, 2U, c9_f_sf_marshallOut,
    c9_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_theta2, 3U, c9_f_sf_marshallOut,
    c9_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_f2, 4U, c9_f_sf_marshallOut,
    c9_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c9_b_gridNum, 5U, c9_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_nargin, 6U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_nargout, 7U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_b_inputBound, 8U, c9_c_sf_marshallOut,
    c9_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_grid, 9U, c9_e_sf_marshallOut,
    c9_c_sf_marshallIn);
  c9_b_gridNum = 5.0;
  CV_EML_FCN(0, 3);
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 64);
  c9_p = 5.0;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 66);
  c9_linspace(chartInstance, c9_b_inputBound[0], c9_b_inputBound[4], c9_dv1);
  for (c9_i11 = 0; c9_i11 < 5; c9_i11++) {
    c9_theta1[c9_i11] = c9_dv1[c9_i11];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 67);
  c9_linspace(chartInstance, c9_b_inputBound[1], c9_b_inputBound[5], c9_dv1);
  for (c9_i12 = 0; c9_i12 < 5; c9_i12++) {
    c9_f1[c9_i12] = c9_dv1[c9_i12];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 68);
  c9_linspace(chartInstance, c9_b_inputBound[2], c9_b_inputBound[6], c9_dv1);
  for (c9_i13 = 0; c9_i13 < 5; c9_i13++) {
    c9_theta2[c9_i13] = c9_dv1[c9_i13];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 69);
  c9_linspace(chartInstance, c9_b_inputBound[3], c9_b_inputBound[7], c9_dv1);
  for (c9_i14 = 0; c9_i14 < 5; c9_i14++) {
    c9_f2[c9_i14] = c9_dv1[c9_i14];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 71);
  for (c9_i15 = 0; c9_i15 < 5; c9_i15++) {
    c9_grid[c9_i15] = c9_theta1[c9_i15];
  }

  for (c9_i16 = 0; c9_i16 < 5; c9_i16++) {
    c9_grid[c9_i16 + 5] = c9_f1[c9_i16];
  }

  for (c9_i17 = 0; c9_i17 < 5; c9_i17++) {
    c9_grid[c9_i17 + 10] = c9_theta2[c9_i17];
  }

  for (c9_i18 = 0; c9_i18 < 5; c9_i18++) {
    c9_grid[c9_i18 + 15] = c9_f2[c9_i18];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, -71);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 16);
  for (c9_i19 = 0; c9_i19 < 4; c9_i19++) {
    c9_u1[c9_i19] = c9_u0[c9_i19];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 18);
  c9_k = 1.0;
  c9_b_k = 0;
  while (c9_b_k < 100) {
    c9_k = 1.0 + (real_T)c9_b_k;
    CV_EML_FOR(0, 1, 0, 1);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 19);
    for (c9_i20 = 0; c9_i20 < 4; c9_i20++) {
      c9_input[c9_i20] = c9_u1[c9_i20];
    }

    for (c9_i21 = 0; c9_i21 < 4; c9_i21++) {
      c9_preinput[c9_i21] = c9_u0[c9_i21];
    }

    c9_b_Fxref = c9_Fxref;
    c9_b_Fyref = c9_Fyref;
    c9_b_Mref = c9_Mref;
    c9_b_rx1 = c9_rx1;
    c9_b_rx2 = c9_rx2;
    c9_b_ry1 = c9_ry1;
    c9_b_ry2 = c9_ry2;
    for (c9_i22 = 0; c9_i22 < 7; c9_i22++) {
      c9_b_C[c9_i22] = c9_C[c9_i22];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 15U, 15U, c9_d_debug_family_names,
      c9_c_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_hsd, 0U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_i, 1U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_nargin, 2U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_nargout, 3U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_input, 4U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_preinput, 5U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_Fxref, 6U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_Fyref, 7U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_Mref, 8U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_rx1, 9U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_rx2, 10U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_ry1, 11U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_ry2, 12U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_b_C, 13U, c9_d_sf_marshallOut,
      c9_d_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_h, 14U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    CV_EML_FCN(0, 7);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 146U);
    for (c9_i23 = 0; c9_i23 < 4; c9_i23++) {
      c9_b_input[c9_i23] = c9_input[c9_i23];
    }

    for (c9_i24 = 0; c9_i24 < 4; c9_i24++) {
      c9_b_preinput[c9_i24] = c9_preinput[c9_i24];
    }

    for (c9_i25 = 0; c9_i25 < 7; c9_i25++) {
      c9_c_C[c9_i25] = c9_b_C[c9_i25];
    }

    c9_SteepestDescent(chartInstance, c9_b_input, c9_b_preinput, c9_b_Fxref,
                       c9_b_Fyref, c9_b_Mref, c9_b_rx1, c9_b_rx2, c9_b_ry1,
                       c9_b_ry2, c9_c_C, c9_dv2);
    for (c9_i26 = 0; c9_i26 < 4; c9_i26++) {
      c9_hsd[c9_i26] = c9_dv2[c9_i26];
    }

    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 148U);
    for (c9_i27 = 0; c9_i27 < 4; c9_i27++) {
      c9_h[c9_i27] = 0.0;
    }

    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 150U);
    c9_i = 1.0;
    c9_b_i = 0;
    while (c9_b_i < 4) {
      c9_i = 1.0 + (real_T)c9_b_i;
      CV_EML_FOR(0, 1, 6, 1);
      _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 151U);
      c9_h[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("output", (int32_T)
        _SFD_INTEGER_CHECK("i", c9_i), 1, 4, 1, 0) - 1] = c9_hsd[(int32_T)
        (real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("hsd", (int32_T)_SFD_INTEGER_CHECK(
        "i", c9_i), 1, 4, 1, 0) - 1];
      c9_b_i++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 6, 0);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, -151);
    _SFD_SYMBOL_SCOPE_POP();
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 20);
    for (c9_i28 = 0; c9_i28 < 4; c9_i28++) {
      c9_c_input[c9_i28] = c9_u1[c9_i28];
    }

    for (c9_i29 = 0; c9_i29 < 4; c9_i29++) {
      c9_c_preinput[c9_i29] = c9_u0[c9_i29];
    }

    for (c9_i30 = 0; c9_i30 < 4; c9_i30++) {
      c9_b_h[c9_i30] = c9_h[c9_i30];
    }

    c9_c_Fxref = c9_Fxref;
    c9_c_Fyref = c9_Fyref;
    c9_c_Mref = c9_Mref;
    c9_c_rx1 = c9_rx1;
    c9_c_rx2 = c9_rx2;
    c9_c_ry1 = c9_ry1;
    c9_c_ry2 = c9_ry2;
    for (c9_i31 = 0; c9_i31 < 7; c9_i31++) {
      c9_d_C[c9_i31] = c9_C[c9_i31];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 19U, 19U, c9_f_debug_family_names,
      c9_d_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML(&c9_b_alpha, 0U, c9_b_sf_marshallOut);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_b_u0, 1U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_b_u1, 2U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_v0, 3U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_v1, 4U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_d_nargin, 5U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_d_nargout, 6U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_c_input, 7U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_c_preinput, 8U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_b_h, 9U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_Fxref, 10U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_Fyref, 11U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_Mref, 12U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_rx1, 13U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_rx2, 14U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_ry1, 15U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_ry2, 16U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_d_C, 17U, c9_d_sf_marshallOut,
      c9_d_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_alpha, 18U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    CV_EML_FCN(0, 5);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 89);
    c9_b_alpha = 1.0;
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 91);
    for (c9_i32 = 0; c9_i32 < 4; c9_i32++) {
      c9_b_u0[c9_i32] = c9_c_input[c9_i32];
    }

    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 92);
    for (c9_i33 = 0; c9_i33 < 4; c9_i33++) {
      c9_b[c9_i33] = c9_b_h[c9_i33];
    }

    for (c9_i34 = 0; c9_i34 < 4; c9_i34++) {
      c9_b_u1[c9_i34] = c9_c_input[c9_i34] + c9_b[c9_i34];
    }

    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 94);
    for (c9_i35 = 0; c9_i35 < 4; c9_i35++) {
      c9_c_u0[c9_i35] = c9_b_u0[c9_i35];
    }

    for (c9_i36 = 0; c9_i36 < 4; c9_i36++) {
      c9_d_preinput[c9_i36] = c9_c_preinput[c9_i36];
    }

    for (c9_i37 = 0; c9_i37 < 7; c9_i37++) {
      c9_e_C[c9_i37] = c9_d_C[c9_i37];
    }

    c9_v0 = c9_costFunction(chartInstance, c9_c_u0, c9_d_preinput, c9_c_Fxref,
      c9_c_Fyref, c9_c_Mref, c9_c_rx1, c9_c_rx2, c9_c_ry1, c9_c_ry2, c9_e_C);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 95);
    for (c9_i38 = 0; c9_i38 < 4; c9_i38++) {
      c9_c_u1[c9_i38] = c9_b_u1[c9_i38];
    }

    for (c9_i39 = 0; c9_i39 < 4; c9_i39++) {
      c9_e_preinput[c9_i39] = c9_c_preinput[c9_i39];
    }

    for (c9_i40 = 0; c9_i40 < 7; c9_i40++) {
      c9_f_C[c9_i40] = c9_d_C[c9_i40];
    }

    c9_v1 = c9_costFunction(chartInstance, c9_c_u1, c9_e_preinput, c9_c_Fxref,
      c9_c_Fyref, c9_c_Mref, c9_c_rx1, c9_c_rx2, c9_c_ry1, c9_c_ry2, c9_f_C);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 97);
    exitg2 = FALSE;
    while ((exitg2 == FALSE) && (c9_v1 <= c9_v0)) {
      if (c9_v1 != 0.0) {
        CV_EML_WHILE(0, 1, 0, TRUE);
        _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 98);
        c9_b_alpha++;
        _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 99);
        c9_a = c9_b_alpha;
        for (c9_i41 = 0; c9_i41 < 4; c9_i41++) {
          c9_b[c9_i41] = c9_b_h[c9_i41];
        }

        for (c9_i42 = 0; c9_i42 < 4; c9_i42++) {
          c9_b[c9_i42] *= c9_a;
        }

        for (c9_i43 = 0; c9_i43 < 4; c9_i43++) {
          c9_b_u1[c9_i43] = c9_c_input[c9_i43] + c9_b[c9_i43];
        }

        _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 100);
        c9_v0 = c9_v1;
        _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 101);
        for (c9_i44 = 0; c9_i44 < 4; c9_i44++) {
          c9_d_u1[c9_i44] = c9_b_u1[c9_i44];
        }

        for (c9_i45 = 0; c9_i45 < 4; c9_i45++) {
          c9_f_preinput[c9_i45] = c9_c_preinput[c9_i45];
        }

        for (c9_i46 = 0; c9_i46 < 7; c9_i46++) {
          c9_g_C[c9_i46] = c9_d_C[c9_i46];
        }

        c9_v1 = c9_costFunction(chartInstance, c9_d_u1, c9_f_preinput,
          c9_c_Fxref, c9_c_Fyref, c9_c_Mref, c9_c_rx1, c9_c_rx2, c9_c_ry1,
          c9_c_ry2, c9_g_C);
        _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 97);
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      } else {
        exitg2 = TRUE;
      }
    }

    CV_EML_WHILE(0, 1, 0, FALSE);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 104);
    c9_v0 = c9_v1;
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 106);
    exitg1 = FALSE;
    while ((exitg1 == FALSE) && (c9_v1 <= c9_v0)) {
      if (c9_b_alpha > 0.0) {
        if (c9_v1 != 0.0) {
          CV_EML_WHILE(0, 1, 1, TRUE);
          _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 107);
          c9_b_alpha -= 0.01;
          _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 108);
          c9_b_a = c9_b_alpha;
          for (c9_i47 = 0; c9_i47 < 4; c9_i47++) {
            c9_b[c9_i47] = c9_b_h[c9_i47];
          }

          for (c9_i48 = 0; c9_i48 < 4; c9_i48++) {
            c9_b[c9_i48] *= c9_b_a;
          }

          for (c9_i49 = 0; c9_i49 < 4; c9_i49++) {
            c9_b_u1[c9_i49] = c9_c_input[c9_i49] + c9_b[c9_i49];
          }

          _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 109);
          c9_v0 = c9_v1;
          _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 110);
          for (c9_i50 = 0; c9_i50 < 4; c9_i50++) {
            c9_e_u1[c9_i50] = c9_b_u1[c9_i50];
          }

          for (c9_i51 = 0; c9_i51 < 4; c9_i51++) {
            c9_g_preinput[c9_i51] = c9_c_preinput[c9_i51];
          }

          for (c9_i52 = 0; c9_i52 < 7; c9_i52++) {
            c9_h_C[c9_i52] = c9_d_C[c9_i52];
          }

          c9_v1 = c9_costFunction(chartInstance, c9_e_u1, c9_g_preinput,
            c9_c_Fxref, c9_c_Fyref, c9_c_Mref, c9_c_rx1, c9_c_rx2, c9_c_ry1,
            c9_c_ry2, c9_h_C);
          _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 106);
          _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
        } else {
          exitg1 = TRUE;
        }
      } else {
        exitg1 = TRUE;
      }
    }

    CV_EML_WHILE(0, 1, 1, FALSE);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 113);
    c9_b_alpha += 0.01;
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 115);
    c9_alpha = c9_b_alpha;
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, -115);
    _SFD_SYMBOL_SCOPE_POP();
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 21);
    c9_c_a = c9_alpha;
    for (c9_i53 = 0; c9_i53 < 4; c9_i53++) {
      c9_b[c9_i53] = c9_h[c9_i53];
    }

    for (c9_i54 = 0; c9_i54 < 4; c9_i54++) {
      c9_b[c9_i54] *= c9_c_a;
    }

    for (c9_i55 = 0; c9_i55 < 4; c9_i55++) {
      c9_u1[c9_i55] += c9_b[c9_i55];
    }

    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 22);
    for (c9_i56 = 0; c9_i56 < 4; c9_i56++) {
      c9_d_input[c9_i56] = c9_u1[c9_i56];
    }

    for (c9_i57 = 0; c9_i57 < 8; c9_i57++) {
      c9_c_inputBound[c9_i57] = c9_inputBound[c9_i57];
    }

    _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 7U, 7U, c9_h_debug_family_names,
      c9_e_debug_family_var_map);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_c_k, 0U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_c_i, 1U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_e_nargin, 2U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_e_nargout, 3U, c9_b_sf_marshallOut,
      c9_b_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_d_input, 4U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_c_inputBound, 5U,
      c9_c_sf_marshallOut, c9_e_sf_marshallIn);
    _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_u1, 6U, c9_sf_marshallOut,
      c9_sf_marshallIn);
    CV_EML_FCN(0, 2);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 55);
    for (c9_i58 = 0; c9_i58 < 4; c9_i58++) {
      c9_c_k[c9_i58] = 0.0;
    }

    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 56);
    c9_c_i = 1.0;
    c9_d_i = 0;
    while (c9_d_i < 4) {
      c9_c_i = 1.0 + (real_T)c9_d_i;
      CV_EML_FOR(0, 1, 5, 1);
      _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 57);
      c9_e_input = c9_d_input[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK(
        "input", (int32_T)_SFD_INTEGER_CHECK("i", c9_c_i), 1, 4, 1, 0) - 1];
      c9_e_i = (int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("inputBound",
        (int32_T)_SFD_INTEGER_CHECK("i", c9_c_i), 1, 4, 1, 0) - 1;
      for (c9_i59 = 0; c9_i59 < 2; c9_i59++) {
        c9_bound[c9_i59] = c9_c_inputBound[c9_e_i + (c9_i59 << 2)];
      }

      _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 5U, 5U, c9_g_debug_family_names,
        c9_f_debug_family_var_map);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_f_nargin, 0U, c9_b_sf_marshallOut,
        c9_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_f_nargout, 1U,
        c9_b_sf_marshallOut, c9_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_e_input, 2U, c9_b_sf_marshallOut,
        c9_b_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_bound, 3U, c9_g_sf_marshallOut,
        c9_g_sf_marshallIn);
      _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_b_output, 4U, c9_b_sf_marshallOut,
        c9_b_sf_marshallIn);
      CV_EML_FCN(0, 4);
      _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 77);
      if (CV_EML_IF(0, 1, 1, c9_e_input < c9_bound[0])) {
        _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 78);
        c9_b_output = c9_bound[0];
      } else {
        _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 79);
        if (CV_EML_IF(0, 1, 2, c9_e_input > c9_bound[1])) {
          _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 80);
          c9_b_output = c9_bound[1];
        } else {
          _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 82);
          c9_b_output = c9_e_input;
        }
      }

      _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, -82);
      _SFD_SYMBOL_SCOPE_POP();
      c9_c_k[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("k", (int32_T)
        _SFD_INTEGER_CHECK("i", c9_c_i), 1, 4, 1, 0) - 1] = c9_b_output;
      c9_d_i++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 5, 0);
    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 59);
    for (c9_i60 = 0; c9_i60 < 4; c9_i60++) {
      c9_u1[c9_i60] = c9_c_k[c9_i60];
    }

    _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, -59);
    _SFD_SYMBOL_SCOPE_POP();
    c9_b_k++;
    _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
  }

  CV_EML_FOR(0, 1, 0, 0);
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 25);
  for (c9_i61 = 0; c9_i61 < 4; c9_i61++) {
    c9_output[c9_i61] = c9_u1[c9_i61];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, -25);
  _SFD_SYMBOL_SCOPE_POP();
  for (c9_i62 = 0; c9_i62 < 4; c9_i62++) {
    (*c9_c_output)[c9_i62] = c9_output[c9_i62];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 8U, chartInstance->c9_sfEvent);
}

static void initSimStructsc9_BoatLibraryTest02
  (SFc9_BoatLibraryTest02InstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c9_machineNumber, uint32_T
  c9_chartNumber)
{
}

static const mxArray *c9_sf_marshallOut(void *chartInstanceVoid, void *c9_inData)
{
  const mxArray *c9_mxArrayOutData = NULL;
  int32_T c9_i63;
  real_T c9_b_inData[4];
  int32_T c9_i64;
  real_T c9_u[4];
  const mxArray *c9_y = NULL;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_mxArrayOutData = NULL;
  for (c9_i63 = 0; c9_i63 < 4; c9_i63++) {
    c9_b_inData[c9_i63] = (*(real_T (*)[4])c9_inData)[c9_i63];
  }

  for (c9_i64 = 0; c9_i64 < 4; c9_i64++) {
    c9_u[c9_i64] = c9_b_inData[c9_i64];
  }

  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", c9_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_assign(&c9_mxArrayOutData, c9_y, FALSE);
  return c9_mxArrayOutData;
}

static void c9_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_output, const char_T *c9_identifier, real_T
  c9_y[4])
{
  emlrtMsgIdentifier c9_thisId;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_output), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_output);
}

static void c9_b_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[4])
{
  real_T c9_dv3[4];
  int32_T c9_i65;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), c9_dv3, 1, 0, 0U, 1, 0U, 1, 4);
  for (c9_i65 = 0; c9_i65 < 4; c9_i65++) {
    c9_y[c9_i65] = c9_dv3[c9_i65];
  }

  sf_mex_destroy(&c9_u);
}

static void c9_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData)
{
  const mxArray *c9_output;
  const char_T *c9_identifier;
  emlrtMsgIdentifier c9_thisId;
  real_T c9_y[4];
  int32_T c9_i66;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_output = sf_mex_dup(c9_mxArrayInData);
  c9_identifier = c9_varName;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_output), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_output);
  for (c9_i66 = 0; c9_i66 < 4; c9_i66++) {
    (*(real_T (*)[4])c9_outData)[c9_i66] = c9_y[c9_i66];
  }

  sf_mex_destroy(&c9_mxArrayInData);
}

static const mxArray *c9_b_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData)
{
  const mxArray *c9_mxArrayOutData = NULL;
  real_T c9_u;
  const mxArray *c9_y = NULL;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_mxArrayOutData = NULL;
  c9_u = *(real_T *)c9_inData;
  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", &c9_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c9_mxArrayOutData, c9_y, FALSE);
  return c9_mxArrayOutData;
}

static const mxArray *c9_c_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData)
{
  const mxArray *c9_mxArrayOutData = NULL;
  int32_T c9_i67;
  int32_T c9_i68;
  int32_T c9_i69;
  real_T c9_b_inData[8];
  int32_T c9_i70;
  int32_T c9_i71;
  int32_T c9_i72;
  real_T c9_u[8];
  const mxArray *c9_y = NULL;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_mxArrayOutData = NULL;
  c9_i67 = 0;
  for (c9_i68 = 0; c9_i68 < 2; c9_i68++) {
    for (c9_i69 = 0; c9_i69 < 4; c9_i69++) {
      c9_b_inData[c9_i69 + c9_i67] = (*(real_T (*)[8])c9_inData)[c9_i69 + c9_i67];
    }

    c9_i67 += 4;
  }

  c9_i70 = 0;
  for (c9_i71 = 0; c9_i71 < 2; c9_i71++) {
    for (c9_i72 = 0; c9_i72 < 4; c9_i72++) {
      c9_u[c9_i72 + c9_i70] = c9_b_inData[c9_i72 + c9_i70];
    }

    c9_i70 += 4;
  }

  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", c9_u, 0, 0U, 1U, 0U, 2, 4, 2), FALSE);
  sf_mex_assign(&c9_mxArrayOutData, c9_y, FALSE);
  return c9_mxArrayOutData;
}

static const mxArray *c9_d_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData)
{
  const mxArray *c9_mxArrayOutData = NULL;
  int32_T c9_i73;
  real_T c9_b_inData[7];
  int32_T c9_i74;
  real_T c9_u[7];
  const mxArray *c9_y = NULL;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_mxArrayOutData = NULL;
  for (c9_i73 = 0; c9_i73 < 7; c9_i73++) {
    c9_b_inData[c9_i73] = (*(real_T (*)[7])c9_inData)[c9_i73];
  }

  for (c9_i74 = 0; c9_i74 < 7; c9_i74++) {
    c9_u[c9_i74] = c9_b_inData[c9_i74];
  }

  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", c9_u, 0, 0U, 1U, 0U, 1, 7), FALSE);
  sf_mex_assign(&c9_mxArrayOutData, c9_y, FALSE);
  return c9_mxArrayOutData;
}

static real_T c9_c_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId)
{
  real_T c9_y;
  real_T c9_d0;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), &c9_d0, 1, 0, 0U, 0, 0U, 0);
  c9_y = c9_d0;
  sf_mex_destroy(&c9_u);
  return c9_y;
}

static void c9_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData)
{
  const mxArray *c9_nargout;
  const char_T *c9_identifier;
  emlrtMsgIdentifier c9_thisId;
  real_T c9_y;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_nargout = sf_mex_dup(c9_mxArrayInData);
  c9_identifier = c9_varName;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_y = c9_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_nargout), &c9_thisId);
  sf_mex_destroy(&c9_nargout);
  *(real_T *)c9_outData = c9_y;
  sf_mex_destroy(&c9_mxArrayInData);
}

static const mxArray *c9_e_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData)
{
  const mxArray *c9_mxArrayOutData = NULL;
  int32_T c9_i75;
  int32_T c9_i76;
  int32_T c9_i77;
  real_T c9_b_inData[20];
  int32_T c9_i78;
  int32_T c9_i79;
  int32_T c9_i80;
  real_T c9_u[20];
  const mxArray *c9_y = NULL;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_mxArrayOutData = NULL;
  c9_i75 = 0;
  for (c9_i76 = 0; c9_i76 < 4; c9_i76++) {
    for (c9_i77 = 0; c9_i77 < 5; c9_i77++) {
      c9_b_inData[c9_i77 + c9_i75] = (*(real_T (*)[20])c9_inData)[c9_i77 +
        c9_i75];
    }

    c9_i75 += 5;
  }

  c9_i78 = 0;
  for (c9_i79 = 0; c9_i79 < 4; c9_i79++) {
    for (c9_i80 = 0; c9_i80 < 5; c9_i80++) {
      c9_u[c9_i80 + c9_i78] = c9_b_inData[c9_i80 + c9_i78];
    }

    c9_i78 += 5;
  }

  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", c9_u, 0, 0U, 1U, 0U, 2, 5, 4), FALSE);
  sf_mex_assign(&c9_mxArrayOutData, c9_y, FALSE);
  return c9_mxArrayOutData;
}

static void c9_d_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[20])
{
  real_T c9_dv4[20];
  int32_T c9_i81;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), c9_dv4, 1, 0, 0U, 1, 0U, 2, 5, 4);
  for (c9_i81 = 0; c9_i81 < 20; c9_i81++) {
    c9_y[c9_i81] = c9_dv4[c9_i81];
  }

  sf_mex_destroy(&c9_u);
}

static void c9_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData)
{
  const mxArray *c9_grid;
  const char_T *c9_identifier;
  emlrtMsgIdentifier c9_thisId;
  real_T c9_y[20];
  int32_T c9_i82;
  int32_T c9_i83;
  int32_T c9_i84;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_grid = sf_mex_dup(c9_mxArrayInData);
  c9_identifier = c9_varName;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_grid), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_grid);
  c9_i82 = 0;
  for (c9_i83 = 0; c9_i83 < 4; c9_i83++) {
    for (c9_i84 = 0; c9_i84 < 5; c9_i84++) {
      (*(real_T (*)[20])c9_outData)[c9_i84 + c9_i82] = c9_y[c9_i84 + c9_i82];
    }

    c9_i82 += 5;
  }

  sf_mex_destroy(&c9_mxArrayInData);
}

static void c9_e_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[7])
{
  real_T c9_dv5[7];
  int32_T c9_i85;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), c9_dv5, 1, 0, 0U, 1, 0U, 1, 7);
  for (c9_i85 = 0; c9_i85 < 7; c9_i85++) {
    c9_y[c9_i85] = c9_dv5[c9_i85];
  }

  sf_mex_destroy(&c9_u);
}

static void c9_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData)
{
  const mxArray *c9_C;
  const char_T *c9_identifier;
  emlrtMsgIdentifier c9_thisId;
  real_T c9_y[7];
  int32_T c9_i86;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_C = sf_mex_dup(c9_mxArrayInData);
  c9_identifier = c9_varName;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_C), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_C);
  for (c9_i86 = 0; c9_i86 < 7; c9_i86++) {
    (*(real_T (*)[7])c9_outData)[c9_i86] = c9_y[c9_i86];
  }

  sf_mex_destroy(&c9_mxArrayInData);
}

static void c9_f_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[8])
{
  real_T c9_dv6[8];
  int32_T c9_i87;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), c9_dv6, 1, 0, 0U, 1, 0U, 2, 4, 2);
  for (c9_i87 = 0; c9_i87 < 8; c9_i87++) {
    c9_y[c9_i87] = c9_dv6[c9_i87];
  }

  sf_mex_destroy(&c9_u);
}

static void c9_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData)
{
  const mxArray *c9_inputBound;
  const char_T *c9_identifier;
  emlrtMsgIdentifier c9_thisId;
  real_T c9_y[8];
  int32_T c9_i88;
  int32_T c9_i89;
  int32_T c9_i90;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_inputBound = sf_mex_dup(c9_mxArrayInData);
  c9_identifier = c9_varName;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_inputBound), &c9_thisId,
                        c9_y);
  sf_mex_destroy(&c9_inputBound);
  c9_i88 = 0;
  for (c9_i89 = 0; c9_i89 < 2; c9_i89++) {
    for (c9_i90 = 0; c9_i90 < 4; c9_i90++) {
      (*(real_T (*)[8])c9_outData)[c9_i90 + c9_i88] = c9_y[c9_i90 + c9_i88];
    }

    c9_i88 += 4;
  }

  sf_mex_destroy(&c9_mxArrayInData);
}

static const mxArray *c9_f_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData)
{
  const mxArray *c9_mxArrayOutData = NULL;
  int32_T c9_i91;
  real_T c9_b_inData[5];
  int32_T c9_i92;
  real_T c9_u[5];
  const mxArray *c9_y = NULL;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_mxArrayOutData = NULL;
  for (c9_i91 = 0; c9_i91 < 5; c9_i91++) {
    c9_b_inData[c9_i91] = (*(real_T (*)[5])c9_inData)[c9_i91];
  }

  for (c9_i92 = 0; c9_i92 < 5; c9_i92++) {
    c9_u[c9_i92] = c9_b_inData[c9_i92];
  }

  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", c9_u, 0, 0U, 1U, 0U, 1, 5), FALSE);
  sf_mex_assign(&c9_mxArrayOutData, c9_y, FALSE);
  return c9_mxArrayOutData;
}

static void c9_g_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[5])
{
  real_T c9_dv7[5];
  int32_T c9_i93;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), c9_dv7, 1, 0, 0U, 1, 0U, 1, 5);
  for (c9_i93 = 0; c9_i93 < 5; c9_i93++) {
    c9_y[c9_i93] = c9_dv7[c9_i93];
  }

  sf_mex_destroy(&c9_u);
}

static void c9_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData)
{
  const mxArray *c9_f2;
  const char_T *c9_identifier;
  emlrtMsgIdentifier c9_thisId;
  real_T c9_y[5];
  int32_T c9_i94;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_f2 = sf_mex_dup(c9_mxArrayInData);
  c9_identifier = c9_varName;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_f2), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_f2);
  for (c9_i94 = 0; c9_i94 < 5; c9_i94++) {
    (*(real_T (*)[5])c9_outData)[c9_i94] = c9_y[c9_i94];
  }

  sf_mex_destroy(&c9_mxArrayInData);
}

static const mxArray *c9_g_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData)
{
  const mxArray *c9_mxArrayOutData = NULL;
  int32_T c9_i95;
  real_T c9_b_inData[2];
  int32_T c9_i96;
  real_T c9_u[2];
  const mxArray *c9_y = NULL;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_mxArrayOutData = NULL;
  for (c9_i95 = 0; c9_i95 < 2; c9_i95++) {
    c9_b_inData[c9_i95] = (*(real_T (*)[2])c9_inData)[c9_i95];
  }

  for (c9_i96 = 0; c9_i96 < 2; c9_i96++) {
    c9_u[c9_i96] = c9_b_inData[c9_i96];
  }

  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", c9_u, 0, 0U, 1U, 0U, 2, 1, 2), FALSE);
  sf_mex_assign(&c9_mxArrayOutData, c9_y, FALSE);
  return c9_mxArrayOutData;
}

static void c9_h_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId,
  real_T c9_y[2])
{
  real_T c9_dv8[2];
  int32_T c9_i97;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), c9_dv8, 1, 0, 0U, 1, 0U, 2, 1, 2);
  for (c9_i97 = 0; c9_i97 < 2; c9_i97++) {
    c9_y[c9_i97] = c9_dv8[c9_i97];
  }

  sf_mex_destroy(&c9_u);
}

static void c9_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData)
{
  const mxArray *c9_bound;
  const char_T *c9_identifier;
  emlrtMsgIdentifier c9_thisId;
  real_T c9_y[2];
  int32_T c9_i98;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_bound = sf_mex_dup(c9_mxArrayInData);
  c9_identifier = c9_varName;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_bound), &c9_thisId, c9_y);
  sf_mex_destroy(&c9_bound);
  for (c9_i98 = 0; c9_i98 < 2; c9_i98++) {
    (*(real_T (*)[2])c9_outData)[c9_i98] = c9_y[c9_i98];
  }

  sf_mex_destroy(&c9_mxArrayInData);
}

const mxArray *sf_c9_BoatLibraryTest02_get_eml_resolved_functions_info(void)
{
  const mxArray *c9_nameCaptureInfo = NULL;
  c9_nameCaptureInfo = NULL;
  sf_mex_assign(&c9_nameCaptureInfo, sf_mex_createstruct("structure", 2, 54, 1),
                FALSE);
  c9_info_helper(&c9_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c9_nameCaptureInfo);
  return c9_nameCaptureInfo;
}

static void c9_info_helper(const mxArray **c9_info)
{
  const mxArray *c9_rhs0 = NULL;
  const mxArray *c9_lhs0 = NULL;
  const mxArray *c9_rhs1 = NULL;
  const mxArray *c9_lhs1 = NULL;
  const mxArray *c9_rhs2 = NULL;
  const mxArray *c9_lhs2 = NULL;
  const mxArray *c9_rhs3 = NULL;
  const mxArray *c9_lhs3 = NULL;
  const mxArray *c9_rhs4 = NULL;
  const mxArray *c9_lhs4 = NULL;
  const mxArray *c9_rhs5 = NULL;
  const mxArray *c9_lhs5 = NULL;
  const mxArray *c9_rhs6 = NULL;
  const mxArray *c9_lhs6 = NULL;
  const mxArray *c9_rhs7 = NULL;
  const mxArray *c9_lhs7 = NULL;
  const mxArray *c9_rhs8 = NULL;
  const mxArray *c9_lhs8 = NULL;
  const mxArray *c9_rhs9 = NULL;
  const mxArray *c9_lhs9 = NULL;
  const mxArray *c9_rhs10 = NULL;
  const mxArray *c9_lhs10 = NULL;
  const mxArray *c9_rhs11 = NULL;
  const mxArray *c9_lhs11 = NULL;
  const mxArray *c9_rhs12 = NULL;
  const mxArray *c9_lhs12 = NULL;
  const mxArray *c9_rhs13 = NULL;
  const mxArray *c9_lhs13 = NULL;
  const mxArray *c9_rhs14 = NULL;
  const mxArray *c9_lhs14 = NULL;
  const mxArray *c9_rhs15 = NULL;
  const mxArray *c9_lhs15 = NULL;
  const mxArray *c9_rhs16 = NULL;
  const mxArray *c9_lhs16 = NULL;
  const mxArray *c9_rhs17 = NULL;
  const mxArray *c9_lhs17 = NULL;
  const mxArray *c9_rhs18 = NULL;
  const mxArray *c9_lhs18 = NULL;
  const mxArray *c9_rhs19 = NULL;
  const mxArray *c9_lhs19 = NULL;
  const mxArray *c9_rhs20 = NULL;
  const mxArray *c9_lhs20 = NULL;
  const mxArray *c9_rhs21 = NULL;
  const mxArray *c9_lhs21 = NULL;
  const mxArray *c9_rhs22 = NULL;
  const mxArray *c9_lhs22 = NULL;
  const mxArray *c9_rhs23 = NULL;
  const mxArray *c9_lhs23 = NULL;
  const mxArray *c9_rhs24 = NULL;
  const mxArray *c9_lhs24 = NULL;
  const mxArray *c9_rhs25 = NULL;
  const mxArray *c9_lhs25 = NULL;
  const mxArray *c9_rhs26 = NULL;
  const mxArray *c9_lhs26 = NULL;
  const mxArray *c9_rhs27 = NULL;
  const mxArray *c9_lhs27 = NULL;
  const mxArray *c9_rhs28 = NULL;
  const mxArray *c9_lhs28 = NULL;
  const mxArray *c9_rhs29 = NULL;
  const mxArray *c9_lhs29 = NULL;
  const mxArray *c9_rhs30 = NULL;
  const mxArray *c9_lhs30 = NULL;
  const mxArray *c9_rhs31 = NULL;
  const mxArray *c9_lhs31 = NULL;
  const mxArray *c9_rhs32 = NULL;
  const mxArray *c9_lhs32 = NULL;
  const mxArray *c9_rhs33 = NULL;
  const mxArray *c9_lhs33 = NULL;
  const mxArray *c9_rhs34 = NULL;
  const mxArray *c9_lhs34 = NULL;
  const mxArray *c9_rhs35 = NULL;
  const mxArray *c9_lhs35 = NULL;
  const mxArray *c9_rhs36 = NULL;
  const mxArray *c9_lhs36 = NULL;
  const mxArray *c9_rhs37 = NULL;
  const mxArray *c9_lhs37 = NULL;
  const mxArray *c9_rhs38 = NULL;
  const mxArray *c9_lhs38 = NULL;
  const mxArray *c9_rhs39 = NULL;
  const mxArray *c9_lhs39 = NULL;
  const mxArray *c9_rhs40 = NULL;
  const mxArray *c9_lhs40 = NULL;
  const mxArray *c9_rhs41 = NULL;
  const mxArray *c9_lhs41 = NULL;
  const mxArray *c9_rhs42 = NULL;
  const mxArray *c9_lhs42 = NULL;
  const mxArray *c9_rhs43 = NULL;
  const mxArray *c9_lhs43 = NULL;
  const mxArray *c9_rhs44 = NULL;
  const mxArray *c9_lhs44 = NULL;
  const mxArray *c9_rhs45 = NULL;
  const mxArray *c9_lhs45 = NULL;
  const mxArray *c9_rhs46 = NULL;
  const mxArray *c9_lhs46 = NULL;
  const mxArray *c9_rhs47 = NULL;
  const mxArray *c9_lhs47 = NULL;
  const mxArray *c9_rhs48 = NULL;
  const mxArray *c9_lhs48 = NULL;
  const mxArray *c9_rhs49 = NULL;
  const mxArray *c9_lhs49 = NULL;
  const mxArray *c9_rhs50 = NULL;
  const mxArray *c9_lhs50 = NULL;
  const mxArray *c9_rhs51 = NULL;
  const mxArray *c9_lhs51 = NULL;
  const mxArray *c9_rhs52 = NULL;
  const mxArray *c9_lhs52 = NULL;
  const mxArray *c9_rhs53 = NULL;
  const mxArray *c9_lhs53 = NULL;
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("linspace"), "name", "name", 0);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "resolved",
                  "resolved", 0);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731858U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c9_rhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs0), "rhs", "rhs", 0);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs0), "lhs", "lhs", 0);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 1);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 1);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 1);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c9_rhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs1), "rhs", "rhs", 1);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs1), "lhs", "lhs", 1);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 2);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 2);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 2);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c9_rhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs2), "rhs", "rhs", 2);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs2), "lhs", "lhs", 2);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 3);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_floor"), "name",
                  "name", 3);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m"),
                  "resolved", "resolved", 3);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840326U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c9_rhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs3), "rhs", "rhs", 3);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs3), "lhs", "lhs", 3);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 4);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 4);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 4);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1323192178U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c9_rhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs4), "rhs", "rhs", 4);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs4), "lhs", "lhs", 4);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 5);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("realmax"), "name", "name", 5);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmax.m"), "resolved",
                  "resolved", 5);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1307672842U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c9_rhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs5), "rhs", "rhs", 5);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs5), "lhs", "lhs", 5);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmax.m"), "context",
                  "context", 6);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_realmax"), "name", "name",
                  6);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "resolved",
                  "resolved", 6);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1326749596U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c9_rhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs6), "rhs", "rhs", 6);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs6), "lhs", "lhs", 6);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "context",
                  "context", 7);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_float_model"), "name",
                  "name", 7);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_float_model.m"),
                  "resolved", "resolved", 7);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1326749596U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c9_rhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs7), "rhs", "rhs", 7);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs7), "lhs", "lhs", 7);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmax.m"), "context",
                  "context", 8);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("mtimes"), "name", "name", 8);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 8);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c9_rhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs8), "rhs", "rhs", 8);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs8), "lhs", "lhs", 8);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m!common_checks"),
                  "context", "context", 9);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 9);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 9);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c9_rhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs9), "rhs", "rhs", 9);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs9), "lhs", "lhs", 9);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 10);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("mrdivide"), "name", "name", 10);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 10);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c9_rhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "context",
                  "context", 11);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("rdivide"), "name", "name", 11);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "resolved",
                  "resolved", 11);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c9_rhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 12);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 12);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 12);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c9_rhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 13);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalexp_compatible"),
                  "name", "name", 13);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m"),
                  "resolved", "resolved", 13);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c9_rhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 14);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_div"), "name", "name", 14);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m"), "resolved",
                  "resolved", 14);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731866U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c9_rhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs14), "lhs", "lhs",
                  14);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/linspace.m"), "context",
                  "context", 15);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("mtimes"), "name", "name", 15);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 15);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 15);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 15);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 15);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 15);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 15);
  sf_mex_assign(&c9_rhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs15), "rhs", "rhs",
                  15);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs15), "lhs", "lhs",
                  15);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(""), "context", "context", 16);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("mtimes"), "name", "name", 16);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 16);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 16);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 16);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 16);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 16);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 16);
  sf_mex_assign(&c9_rhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs16), "rhs", "rhs",
                  16);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs16), "lhs", "lhs",
                  16);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(""), "context", "context", 17);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("cosd"), "name", "name", 17);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 17);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "resolved",
                  "resolved", 17);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1343851972U), "fileTimeLo",
                  "fileTimeLo", 17);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 17);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 17);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 17);
  sf_mex_assign(&c9_rhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs17), "rhs", "rhs",
                  17);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs17), "lhs", "lhs",
                  17);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "context",
                  "context", 18);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 18);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 18);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 18);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 18);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 18);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 18);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 18);
  sf_mex_assign(&c9_rhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs18), "rhs", "rhs",
                  18);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs18), "lhs", "lhs",
                  18);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 19);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("isfinite"), "name", "name", 19);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 19);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "resolved",
                  "resolved", 19);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 19);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 19);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 19);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 19);
  sf_mex_assign(&c9_rhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs19), "rhs", "rhs",
                  19);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs19), "lhs", "lhs",
                  19);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 20);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 20);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 20);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 20);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 20);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 20);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 20);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 20);
  sf_mex_assign(&c9_rhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs20), "rhs", "rhs",
                  20);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs20), "lhs", "lhs",
                  20);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 21);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("isinf"), "name", "name", 21);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 21);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "resolved",
                  "resolved", 21);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 21);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 21);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 21);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 21);
  sf_mex_assign(&c9_rhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs21), "rhs", "rhs",
                  21);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs21), "lhs", "lhs",
                  21);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "context",
                  "context", 22);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 22);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 22);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 22);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 22);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 22);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 22);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 22);
  sf_mex_assign(&c9_rhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs22), "rhs", "rhs",
                  22);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs22), "lhs", "lhs",
                  22);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 23);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("isnan"), "name", "name", 23);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 23);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "resolved",
                  "resolved", 23);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731858U), "fileTimeLo",
                  "fileTimeLo", 23);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 23);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 23);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 23);
  sf_mex_assign(&c9_rhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs23), "rhs", "rhs",
                  23);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs23), "lhs", "lhs",
                  23);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "context",
                  "context", 24);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 24);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 24);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 24);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 24);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 24);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 24);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 24);
  sf_mex_assign(&c9_rhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs24), "rhs", "rhs",
                  24);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs24), "lhs", "lhs",
                  24);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 25);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_guarded_nan"), "name",
                  "name", 25);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 25);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "resolved", "resolved", 25);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840376U), "fileTimeLo",
                  "fileTimeLo", 25);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 25);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 25);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 25);
  sf_mex_assign(&c9_rhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs25), "rhs", "rhs",
                  25);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs25), "lhs", "lhs",
                  25);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "context", "context", 26);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_is_float_class"), "name",
                  "name", 26);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 26);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m"),
                  "resolved", "resolved", 26);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840382U), "fileTimeLo",
                  "fileTimeLo", 26);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 26);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 26);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 26);
  sf_mex_assign(&c9_rhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs26), "rhs", "rhs",
                  26);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs26), "lhs", "lhs",
                  26);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 27);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_rem90"), "name",
                  "name", 27);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 27);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "resolved", "resolved", 27);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 27);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 27);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 27);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 27);
  sf_mex_assign(&c9_rhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs27), "rhs", "rhs",
                  27);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs27), "lhs", "lhs",
                  27);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 28);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("mrdivide"), "name", "name", 28);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 28);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 28);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 28);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 28);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 28);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 28);
  sf_mex_assign(&c9_rhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs28), "rhs", "rhs",
                  28);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs28), "lhs", "lhs",
                  28);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 29);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("rem"), "name", "name", 29);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 29);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "resolved",
                  "resolved", 29);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731854U), "fileTimeLo",
                  "fileTimeLo", 29);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 29);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 29);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 29);
  sf_mex_assign(&c9_rhs29, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs29, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs29), "rhs", "rhs",
                  29);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs29), "lhs", "lhs",
                  29);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 30);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 30);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 30);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 30);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 30);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 30);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 30);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 30);
  sf_mex_assign(&c9_rhs30, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs30, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs30), "rhs", "rhs",
                  30);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs30), "lhs", "lhs",
                  30);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 31);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 31);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 31);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 31);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 31);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 31);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 31);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 31);
  sf_mex_assign(&c9_rhs31, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs31, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs31), "rhs", "rhs",
                  31);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs31), "lhs", "lhs",
                  31);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 32);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 32);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 32);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 32);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1358207740U), "fileTimeLo",
                  "fileTimeLo", 32);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 32);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 32);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 32);
  sf_mex_assign(&c9_rhs32, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs32, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs32), "rhs", "rhs",
                  32);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs32), "lhs", "lhs",
                  32);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 33);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("abs"), "name", "name", 33);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 33);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "resolved",
                  "resolved", 33);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731852U), "fileTimeLo",
                  "fileTimeLo", 33);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 33);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 33);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 33);
  sf_mex_assign(&c9_rhs33, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs33, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs33), "rhs", "rhs",
                  33);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs33), "lhs", "lhs",
                  33);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 34);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 34);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 34);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 34);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 34);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 34);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 34);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 34);
  sf_mex_assign(&c9_rhs34, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs34, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs34), "rhs", "rhs",
                  34);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs34), "lhs", "lhs",
                  34);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 35);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_abs"), "name",
                  "name", 35);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 35);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m"),
                  "resolved", "resolved", 35);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840312U), "fileTimeLo",
                  "fileTimeLo", 35);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 35);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 35);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 35);
  sf_mex_assign(&c9_rhs35, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs35, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs35), "rhs", "rhs",
                  35);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs35), "lhs", "lhs",
                  35);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 36);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("mtimes"), "name", "name", 36);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 36);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 36);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 36);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 36);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 36);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 36);
  sf_mex_assign(&c9_rhs36, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs36, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs36), "rhs", "rhs",
                  36);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs36), "lhs", "lhs",
                  36);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(""), "context", "context", 37);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("sind"), "name", "name", 37);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 37);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "resolved",
                  "resolved", 37);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1343851986U), "fileTimeLo",
                  "fileTimeLo", 37);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 37);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 37);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 37);
  sf_mex_assign(&c9_rhs37, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs37, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs37), "rhs", "rhs",
                  37);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs37), "lhs", "lhs",
                  37);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "context",
                  "context", 38);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 38);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 38);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 38);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 38);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 38);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 38);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 38);
  sf_mex_assign(&c9_rhs38, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs38, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs38), "rhs", "rhs",
                  38);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs38), "lhs", "lhs",
                  38);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(""), "context", "context", 39);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("sin"), "name", "name", 39);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 39);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m"), "resolved",
                  "resolved", 39);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1343851986U), "fileTimeLo",
                  "fileTimeLo", 39);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 39);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 39);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 39);
  sf_mex_assign(&c9_rhs39, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs39, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs39), "rhs", "rhs",
                  39);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs39), "lhs", "lhs",
                  39);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sin.m"), "context",
                  "context", 40);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_sin"), "name",
                  "name", 40);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 40);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_sin.m"),
                  "resolved", "resolved", 40);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840336U), "fileTimeLo",
                  "fileTimeLo", 40);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 40);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 40);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 40);
  sf_mex_assign(&c9_rhs40, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs40, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs40), "rhs", "rhs",
                  40);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs40), "lhs", "lhs",
                  40);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(""), "context", "context", 41);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("length"), "name", "name", 41);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 41);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/length.m"), "resolved",
                  "resolved", 41);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1303167806U), "fileTimeLo",
                  "fileTimeLo", 41);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 41);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 41);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 41);
  sf_mex_assign(&c9_rhs41, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs41, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs41), "rhs", "rhs",
                  41);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs41), "lhs", "lhs",
                  41);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(""), "context", "context", 42);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("mpower"), "name", "name", 42);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 42);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "resolved",
                  "resolved", 42);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 42);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 42);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 42);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 42);
  sf_mex_assign(&c9_rhs42, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs42, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs42), "rhs", "rhs",
                  42);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs42), "lhs", "lhs",
                  42);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 43);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 43);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 43);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 43);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 43);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 43);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 43);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 43);
  sf_mex_assign(&c9_rhs43, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs43, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs43), "rhs", "rhs",
                  43);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs43), "lhs", "lhs",
                  43);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 44);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("ismatrix"), "name", "name", 44);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 44);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/ismatrix.m"), "resolved",
                  "resolved", 44);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1331326458U), "fileTimeLo",
                  "fileTimeLo", 44);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 44);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 44);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 44);
  sf_mex_assign(&c9_rhs44, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs44, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs44), "rhs", "rhs",
                  44);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs44), "lhs", "lhs",
                  44);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m"), "context",
                  "context", 45);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("power"), "name", "name", 45);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 45);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m"), "resolved",
                  "resolved", 45);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 45);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 45);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 45);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 45);
  sf_mex_assign(&c9_rhs45, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs45, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs45), "rhs", "rhs",
                  45);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs45), "lhs", "lhs",
                  45);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m"), "context",
                  "context", 46);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 46);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 46);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 46);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 46);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 46);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 46);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 46);
  sf_mex_assign(&c9_rhs46, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs46, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs46), "rhs", "rhs",
                  46);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs46), "lhs", "lhs",
                  46);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 47);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 47);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 47);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 47);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 47);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 47);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 47);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 47);
  sf_mex_assign(&c9_rhs47, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs47, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs47), "rhs", "rhs",
                  47);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs47), "lhs", "lhs",
                  47);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 48);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 48);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 48);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 48);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1358207740U), "fileTimeLo",
                  "fileTimeLo", 48);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 48);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 48);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 48);
  sf_mex_assign(&c9_rhs48, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs48, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs48), "rhs", "rhs",
                  48);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs48), "lhs", "lhs",
                  48);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!fltpower"), "context",
                  "context", 49);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("floor"), "name", "name", 49);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 49);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "resolved",
                  "resolved", 49);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731854U), "fileTimeLo",
                  "fileTimeLo", 49);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 49);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 49);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 49);
  sf_mex_assign(&c9_rhs49, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs49, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs49), "rhs", "rhs",
                  49);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs49), "lhs", "lhs",
                  49);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "context",
                  "context", 50);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 50);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 50);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 50);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 50);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 50);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 50);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 50);
  sf_mex_assign(&c9_rhs50, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs50, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs50), "rhs", "rhs",
                  50);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs50), "lhs", "lhs",
                  50);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m"), "context",
                  "context", 51);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_floor"), "name",
                  "name", 51);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 51);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m"),
                  "resolved", "resolved", 51);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840326U), "fileTimeLo",
                  "fileTimeLo", 51);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 51);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 51);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 51);
  sf_mex_assign(&c9_rhs51, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs51, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs51), "rhs", "rhs",
                  51);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs51), "lhs", "lhs",
                  51);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!scalar_float_power"),
                  "context", "context", 52);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 52);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 52);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 52);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 52);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 52);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 52);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 52);
  sf_mex_assign(&c9_rhs52, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs52, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs52), "rhs", "rhs",
                  52);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs52), "lhs", "lhs",
                  52);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m!scalar_float_power"),
                  "context", "context", 53);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("mtimes"), "name", "name", 53);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 53);
  sf_mex_addfield(*c9_info, c9_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 53);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 53);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 53);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 53);
  sf_mex_addfield(*c9_info, c9_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 53);
  sf_mex_assign(&c9_rhs53, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c9_lhs53, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_rhs53), "rhs", "rhs",
                  53);
  sf_mex_addfield(*c9_info, sf_mex_duplicatearraysafe(&c9_lhs53), "lhs", "lhs",
                  53);
  sf_mex_destroy(&c9_rhs0);
  sf_mex_destroy(&c9_lhs0);
  sf_mex_destroy(&c9_rhs1);
  sf_mex_destroy(&c9_lhs1);
  sf_mex_destroy(&c9_rhs2);
  sf_mex_destroy(&c9_lhs2);
  sf_mex_destroy(&c9_rhs3);
  sf_mex_destroy(&c9_lhs3);
  sf_mex_destroy(&c9_rhs4);
  sf_mex_destroy(&c9_lhs4);
  sf_mex_destroy(&c9_rhs5);
  sf_mex_destroy(&c9_lhs5);
  sf_mex_destroy(&c9_rhs6);
  sf_mex_destroy(&c9_lhs6);
  sf_mex_destroy(&c9_rhs7);
  sf_mex_destroy(&c9_lhs7);
  sf_mex_destroy(&c9_rhs8);
  sf_mex_destroy(&c9_lhs8);
  sf_mex_destroy(&c9_rhs9);
  sf_mex_destroy(&c9_lhs9);
  sf_mex_destroy(&c9_rhs10);
  sf_mex_destroy(&c9_lhs10);
  sf_mex_destroy(&c9_rhs11);
  sf_mex_destroy(&c9_lhs11);
  sf_mex_destroy(&c9_rhs12);
  sf_mex_destroy(&c9_lhs12);
  sf_mex_destroy(&c9_rhs13);
  sf_mex_destroy(&c9_lhs13);
  sf_mex_destroy(&c9_rhs14);
  sf_mex_destroy(&c9_lhs14);
  sf_mex_destroy(&c9_rhs15);
  sf_mex_destroy(&c9_lhs15);
  sf_mex_destroy(&c9_rhs16);
  sf_mex_destroy(&c9_lhs16);
  sf_mex_destroy(&c9_rhs17);
  sf_mex_destroy(&c9_lhs17);
  sf_mex_destroy(&c9_rhs18);
  sf_mex_destroy(&c9_lhs18);
  sf_mex_destroy(&c9_rhs19);
  sf_mex_destroy(&c9_lhs19);
  sf_mex_destroy(&c9_rhs20);
  sf_mex_destroy(&c9_lhs20);
  sf_mex_destroy(&c9_rhs21);
  sf_mex_destroy(&c9_lhs21);
  sf_mex_destroy(&c9_rhs22);
  sf_mex_destroy(&c9_lhs22);
  sf_mex_destroy(&c9_rhs23);
  sf_mex_destroy(&c9_lhs23);
  sf_mex_destroy(&c9_rhs24);
  sf_mex_destroy(&c9_lhs24);
  sf_mex_destroy(&c9_rhs25);
  sf_mex_destroy(&c9_lhs25);
  sf_mex_destroy(&c9_rhs26);
  sf_mex_destroy(&c9_lhs26);
  sf_mex_destroy(&c9_rhs27);
  sf_mex_destroy(&c9_lhs27);
  sf_mex_destroy(&c9_rhs28);
  sf_mex_destroy(&c9_lhs28);
  sf_mex_destroy(&c9_rhs29);
  sf_mex_destroy(&c9_lhs29);
  sf_mex_destroy(&c9_rhs30);
  sf_mex_destroy(&c9_lhs30);
  sf_mex_destroy(&c9_rhs31);
  sf_mex_destroy(&c9_lhs31);
  sf_mex_destroy(&c9_rhs32);
  sf_mex_destroy(&c9_lhs32);
  sf_mex_destroy(&c9_rhs33);
  sf_mex_destroy(&c9_lhs33);
  sf_mex_destroy(&c9_rhs34);
  sf_mex_destroy(&c9_lhs34);
  sf_mex_destroy(&c9_rhs35);
  sf_mex_destroy(&c9_lhs35);
  sf_mex_destroy(&c9_rhs36);
  sf_mex_destroy(&c9_lhs36);
  sf_mex_destroy(&c9_rhs37);
  sf_mex_destroy(&c9_lhs37);
  sf_mex_destroy(&c9_rhs38);
  sf_mex_destroy(&c9_lhs38);
  sf_mex_destroy(&c9_rhs39);
  sf_mex_destroy(&c9_lhs39);
  sf_mex_destroy(&c9_rhs40);
  sf_mex_destroy(&c9_lhs40);
  sf_mex_destroy(&c9_rhs41);
  sf_mex_destroy(&c9_lhs41);
  sf_mex_destroy(&c9_rhs42);
  sf_mex_destroy(&c9_lhs42);
  sf_mex_destroy(&c9_rhs43);
  sf_mex_destroy(&c9_lhs43);
  sf_mex_destroy(&c9_rhs44);
  sf_mex_destroy(&c9_lhs44);
  sf_mex_destroy(&c9_rhs45);
  sf_mex_destroy(&c9_lhs45);
  sf_mex_destroy(&c9_rhs46);
  sf_mex_destroy(&c9_lhs46);
  sf_mex_destroy(&c9_rhs47);
  sf_mex_destroy(&c9_lhs47);
  sf_mex_destroy(&c9_rhs48);
  sf_mex_destroy(&c9_lhs48);
  sf_mex_destroy(&c9_rhs49);
  sf_mex_destroy(&c9_lhs49);
  sf_mex_destroy(&c9_rhs50);
  sf_mex_destroy(&c9_lhs50);
  sf_mex_destroy(&c9_rhs51);
  sf_mex_destroy(&c9_lhs51);
  sf_mex_destroy(&c9_rhs52);
  sf_mex_destroy(&c9_lhs52);
  sf_mex_destroy(&c9_rhs53);
  sf_mex_destroy(&c9_lhs53);
}

static const mxArray *c9_emlrt_marshallOut(char * c9_u)
{
  const mxArray *c9_y = NULL;
  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", c9_u, 15, 0U, 0U, 0U, 2, 1, strlen
    (c9_u)), FALSE);
  return c9_y;
}

static const mxArray *c9_b_emlrt_marshallOut(uint32_T c9_u)
{
  const mxArray *c9_y = NULL;
  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", &c9_u, 7, 0U, 0U, 0U, 0), FALSE);
  return c9_y;
}

static void c9_eml_scalar_eg(SFc9_BoatLibraryTest02InstanceStruct *chartInstance)
{
}

static void c9_linspace(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c9_d1, real_T c9_d2, real_T c9_y[5])
{
  real_T c9_A;
  real_T c9_x;
  real_T c9_b_x;
  real_T c9_delta1;
  real_T c9_b_A;
  real_T c9_c_x;
  real_T c9_d_x;
  real_T c9_delta2;
  int32_T c9_k;
  real_T c9_b_k;
  real_T c9_a;
  real_T c9_b;
  real_T c9_b_y;
  real_T c9_b_a;
  real_T c9_b_b;
  real_T c9_c_y;
  real_T c9_c_A;
  real_T c9_e_x;
  real_T c9_f_x;
  int32_T c9_c_k;
  real_T c9_c_a;
  real_T c9_c_b;
  real_T c9_d_y;
  boolean_T guard1 = FALSE;
  boolean_T guard2 = FALSE;
  boolean_T guard3 = FALSE;
  c9_eml_scalar_eg(chartInstance);
  c9_y[4] = c9_d2;
  c9_y[0] = c9_d1;
  guard1 = FALSE;
  guard2 = FALSE;
  guard3 = FALSE;
  if (c9_d1 < 0.0 != c9_d2 < 0.0) {
    if (muDoubleScalarAbs(c9_d1) > 8.9884656743115785E+307) {
      guard2 = TRUE;
    } else {
      guard3 = TRUE;
    }
  } else {
    guard3 = TRUE;
  }

  if (guard3 == TRUE) {
    if (muDoubleScalarAbs(c9_d2) > 8.9884656743115785E+307) {
      guard2 = TRUE;
    } else if (muDoubleScalarAbs(0.0) > 8.9884656743115785E+307) {
      guard1 = TRUE;
    } else {
      c9_c_A = c9_d2 - c9_d1;
      c9_e_x = c9_c_A;
      c9_f_x = c9_e_x;
      c9_delta1 = c9_f_x / 4.0;
      for (c9_c_k = 0; c9_c_k < 3; c9_c_k++) {
        c9_b_k = 1.0 + (real_T)c9_c_k;
        c9_c_a = c9_b_k;
        c9_c_b = c9_delta1;
        c9_d_y = c9_c_a * c9_c_b;
        c9_y[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
          _SFD_INTEGER_CHECK("", c9_b_k + 1.0), 1, 5, 1, 0) - 1] = c9_d1 +
          c9_d_y;
      }
    }
  }

  if (guard2 == TRUE) {
    guard1 = TRUE;
  }

  if (guard1 == TRUE) {
    c9_A = c9_d1;
    c9_x = c9_A;
    c9_b_x = c9_x;
    c9_delta1 = c9_b_x / 4.0;
    c9_b_A = c9_d2;
    c9_c_x = c9_b_A;
    c9_d_x = c9_c_x;
    c9_delta2 = c9_d_x / 4.0;
    for (c9_k = 0; c9_k < 3; c9_k++) {
      c9_b_k = 1.0 + (real_T)c9_k;
      c9_a = c9_delta2;
      c9_b = c9_b_k;
      c9_b_y = c9_a * c9_b;
      c9_b_a = c9_delta1;
      c9_b_b = c9_b_k;
      c9_c_y = c9_b_a * c9_b_b;
      c9_y[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
        _SFD_INTEGER_CHECK("", c9_b_k + 1.0), 1, 5, 1, 0) - 1] = (c9_d1 + c9_b_y)
        - c9_c_y;
    }
  }
}

static void c9_SteepestDescent(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c9_input[4], real_T c9_preinput[4], real_T c9_Fxref,
  real_T c9_Fyref, real_T c9_Mref, real_T c9_rx1, real_T c9_rx2, real_T c9_ry1,
  real_T c9_ry2, real_T c9_C[7], real_T c9_output[4])
{
  uint32_T c9_debug_family_var_map[23];
  real_T c9_theta1;
  real_T c9_f1;
  real_T c9_theta2;
  real_T c9_f2;
  real_T c9_pretheta1;
  real_T c9_pretheta2;
  real_T c9_dVdtheta1;
  real_T c9_dVdf1;
  real_T c9_dVdtheta2;
  real_T c9_dVdf2;
  real_T c9_nargin = 10.0;
  real_T c9_nargout = 1.0;
  real_T c9_a;
  real_T c9_b;
  real_T c9_y;
  real_T c9_b_a;
  real_T c9_b_b;
  real_T c9_b_y;
  real_T c9_c_a;
  real_T c9_c_b;
  real_T c9_c_y;
  real_T c9_d_a;
  real_T c9_d_b;
  real_T c9_d_y;
  real_T c9_e_a;
  real_T c9_e_b;
  real_T c9_e_y;
  real_T c9_f_a;
  real_T c9_f_b;
  real_T c9_f_y;
  real_T c9_g_a;
  real_T c9_g_b;
  real_T c9_g_y;
  real_T c9_h_a;
  real_T c9_h_b;
  real_T c9_h_y;
  real_T c9_i_a;
  real_T c9_i_b;
  real_T c9_i_y;
  real_T c9_j_a;
  real_T c9_j_b;
  real_T c9_j_y;
  real_T c9_x;
  real_T c9_b_x;
  real_T c9_k_a;
  real_T c9_k_b;
  real_T c9_k_y;
  real_T c9_l_a;
  real_T c9_l_b;
  real_T c9_l_y;
  real_T c9_m_a;
  real_T c9_m_b;
  real_T c9_m_y;
  real_T c9_n_a;
  real_T c9_n_b;
  real_T c9_n_y;
  real_T c9_o_a;
  real_T c9_o_b;
  real_T c9_o_y;
  real_T c9_p_a;
  real_T c9_p_b;
  real_T c9_p_y;
  real_T c9_q_a;
  real_T c9_q_b;
  real_T c9_q_y;
  real_T c9_r_a;
  real_T c9_r_b;
  real_T c9_r_y;
  real_T c9_s_a;
  real_T c9_s_b;
  real_T c9_s_y;
  real_T c9_t_a;
  real_T c9_t_b;
  real_T c9_t_y;
  real_T c9_u_a;
  real_T c9_u_b;
  real_T c9_u_y;
  real_T c9_v_a;
  real_T c9_v_b;
  real_T c9_v_y;
  real_T c9_w_a;
  real_T c9_w_b;
  real_T c9_w_y;
  real_T c9_x_a;
  real_T c9_x_b;
  real_T c9_x_y;
  real_T c9_y_a;
  real_T c9_y_b;
  real_T c9_y_y;
  real_T c9_ab_a;
  real_T c9_ab_b;
  real_T c9_ab_y;
  real_T c9_bb_a;
  real_T c9_bb_b;
  real_T c9_bb_y;
  real_T c9_c_x;
  real_T c9_d_x;
  real_T c9_cb_a;
  real_T c9_cb_b;
  real_T c9_cb_y;
  real_T c9_db_a;
  real_T c9_db_b;
  real_T c9_db_y;
  real_T c9_eb_a;
  real_T c9_eb_b;
  real_T c9_eb_y;
  real_T c9_fb_a;
  real_T c9_fb_b;
  real_T c9_fb_y;
  real_T c9_gb_a;
  real_T c9_gb_b;
  real_T c9_gb_y;
  real_T c9_hb_a;
  real_T c9_hb_b;
  real_T c9_hb_y;
  real_T c9_ib_a;
  real_T c9_ib_b;
  real_T c9_ib_y;
  real_T c9_jb_a;
  real_T c9_jb_b;
  real_T c9_jb_y;
  real_T c9_kb_a;
  real_T c9_kb_b;
  real_T c9_kb_y;
  real_T c9_lb_a;
  real_T c9_lb_b;
  real_T c9_lb_y;
  real_T c9_mb_a;
  real_T c9_mb_b;
  real_T c9_mb_y;
  real_T c9_nb_a;
  real_T c9_nb_b;
  real_T c9_nb_y;
  real_T c9_ob_a;
  real_T c9_ob_b;
  real_T c9_ob_y;
  real_T c9_pb_a;
  real_T c9_pb_b;
  real_T c9_pb_y;
  real_T c9_qb_a;
  real_T c9_qb_b;
  real_T c9_qb_y;
  real_T c9_rb_a;
  real_T c9_rb_b;
  real_T c9_rb_y;
  real_T c9_sb_a;
  real_T c9_sb_b;
  real_T c9_sb_y;
  real_T c9_tb_a;
  real_T c9_tb_b;
  real_T c9_tb_y;
  real_T c9_ub_a;
  real_T c9_ub_b;
  real_T c9_ub_y;
  real_T c9_vb_a;
  real_T c9_vb_b;
  real_T c9_vb_y;
  real_T c9_wb_a;
  real_T c9_wb_b;
  real_T c9_wb_y;
  real_T c9_xb_a;
  real_T c9_xb_b;
  real_T c9_xb_y;
  real_T c9_yb_a;
  real_T c9_yb_b;
  real_T c9_yb_y;
  real_T c9_ac_a;
  real_T c9_ac_b;
  real_T c9_ac_y;
  real_T c9_bc_a;
  real_T c9_bc_b;
  real_T c9_bc_y;
  real_T c9_cc_a;
  real_T c9_cc_b;
  real_T c9_cc_y;
  real_T c9_dc_a;
  real_T c9_dc_b;
  real_T c9_dc_y;
  real_T c9_ec_a;
  real_T c9_ec_b;
  real_T c9_ec_y;
  real_T c9_fc_a;
  real_T c9_fc_b;
  real_T c9_fc_y;
  real_T c9_gc_a;
  real_T c9_gc_b;
  real_T c9_gc_y;
  real_T c9_hc_a;
  real_T c9_hc_b;
  real_T c9_hc_y;
  real_T c9_ic_a;
  real_T c9_ic_b;
  real_T c9_ic_y;
  real_T c9_jc_a;
  real_T c9_jc_b;
  real_T c9_jc_y;
  real_T c9_kc_a;
  real_T c9_kc_b;
  real_T c9_kc_y;
  real_T c9_lc_a;
  real_T c9_lc_b;
  real_T c9_lc_y;
  real_T c9_mc_a;
  real_T c9_mc_b;
  real_T c9_mc_y;
  real_T c9_nc_a;
  real_T c9_nc_b;
  real_T c9_nc_y;
  real_T c9_oc_a;
  real_T c9_oc_b;
  real_T c9_oc_y;
  real_T c9_pc_a;
  real_T c9_pc_b;
  real_T c9_pc_y;
  real_T c9_qc_a;
  real_T c9_qc_b;
  real_T c9_qc_y;
  real_T c9_rc_a;
  real_T c9_rc_b;
  real_T c9_rc_y;
  real_T c9_sc_a;
  real_T c9_sc_b;
  real_T c9_sc_y;
  real_T c9_tc_a;
  real_T c9_tc_b;
  real_T c9_tc_y;
  real_T c9_uc_a;
  real_T c9_uc_b;
  real_T c9_uc_y;
  real_T c9_vc_a;
  real_T c9_vc_b;
  real_T c9_vc_y;
  real_T c9_wc_a;
  real_T c9_wc_b;
  real_T c9_wc_y;
  real_T c9_xc_a;
  real_T c9_xc_b;
  real_T c9_xc_y;
  real_T c9_yc_a;
  real_T c9_yc_b;
  real_T c9_yc_y;
  real_T c9_ad_a;
  real_T c9_ad_b;
  real_T c9_ad_y;
  real_T c9_bd_a;
  real_T c9_bd_b;
  real_T c9_bd_y;
  real_T c9_cd_a;
  real_T c9_cd_b;
  real_T c9_cd_y;
  real_T c9_dd_a;
  real_T c9_dd_b;
  real_T c9_dd_y;
  real_T c9_ed_a;
  real_T c9_ed_b;
  real_T c9_ed_y;
  real_T c9_fd_a;
  real_T c9_fd_b;
  real_T c9_fd_y;
  real_T c9_gd_a;
  real_T c9_gd_b;
  real_T c9_gd_y;
  real_T c9_hd_a;
  real_T c9_hd_b;
  real_T c9_hd_y;
  real_T c9_id_a;
  real_T c9_id_b;
  real_T c9_id_y;
  real_T c9_jd_a;
  real_T c9_jd_b;
  real_T c9_jd_y;
  real_T c9_kd_a;
  real_T c9_kd_b;
  real_T c9_kd_y;
  real_T c9_ld_a;
  real_T c9_ld_b;
  real_T c9_ld_y;
  real_T c9_md_a;
  real_T c9_md_b;
  real_T c9_md_y;
  real_T c9_nd_a;
  real_T c9_nd_b;
  real_T c9_nd_y;
  real_T c9_od_a;
  real_T c9_od_b;
  real_T c9_od_y;
  real_T c9_pd_a;
  real_T c9_pd_b;
  real_T c9_pd_y;
  real_T c9_qd_a;
  real_T c9_qd_b;
  real_T c9_qd_y;
  real_T c9_b_dVdtheta1[4];
  int32_T c9_i99;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 23U, 23U, c9_c_debug_family_names,
    c9_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_theta1, 0U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_f1, 1U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_theta2, 2U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_f2, 3U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_pretheta1, 4U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_pretheta2, 5U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_dVdtheta1, 6U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_dVdf1, 7U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_dVdtheta2, 8U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_dVdf2, 9U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_nargin, 10U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_nargout, 11U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_input, 12U, c9_sf_marshallOut,
    c9_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_preinput, 13U, c9_sf_marshallOut,
    c9_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Fxref, 14U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Fyref, 15U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Mref, 16U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_rx1, 17U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_rx2, 18U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_ry1, 19U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_ry2, 20U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_C, 21U, c9_d_sf_marshallOut,
    c9_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_output, 22U, c9_sf_marshallOut,
    c9_sf_marshallIn);
  CV_EML_FCN(0, 8);
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 157U);
  c9_theta1 = c9_input[0];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 158U);
  c9_f1 = c9_input[1];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 159U);
  c9_theta2 = c9_input[2];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 160U);
  c9_f2 = c9_input[3];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 162U);
  c9_pretheta1 = c9_preinput[0];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 163U);
  c9_pretheta2 = c9_preinput[2];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 165U);
  c9_a = c9_f1;
  c9_b = c9_ry1;
  c9_y = c9_a * c9_b;
  c9_b_a = c9_y;
  c9_b_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_b_b);
  c9_b_y = c9_b_a * c9_b_b;
  c9_c_a = c9_f1;
  c9_c_b = c9_rx1;
  c9_c_y = c9_c_a * c9_c_b;
  c9_d_a = c9_c_y;
  c9_d_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_d_b);
  c9_d_y = c9_d_a * c9_d_b;
  c9_e_a = c9_C[2];
  c9_e_b = c9_b_y - c9_d_y;
  c9_e_y = c9_e_a * c9_e_b;
  c9_f_a = c9_f1;
  c9_f_b = c9_rx1;
  c9_f_y = c9_f_a * c9_f_b;
  c9_g_a = c9_f_y;
  c9_g_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_g_b);
  c9_g_y = c9_g_a * c9_g_b;
  c9_h_a = c9_f2;
  c9_h_b = c9_rx2;
  c9_h_y = c9_h_a * c9_h_b;
  c9_i_a = c9_h_y;
  c9_i_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_i_b);
  c9_i_y = c9_i_a * c9_i_b;
  c9_j_a = c9_f1;
  c9_j_b = c9_ry1;
  c9_j_y = c9_j_a * c9_j_b;
  c9_x = c9_theta1;
  c9_b_x = c9_x;
  c9_b_x = muDoubleScalarSin(c9_b_x);
  c9_k_a = c9_j_y;
  c9_k_b = c9_b_x;
  c9_k_y = c9_k_a * c9_k_b;
  c9_l_a = c9_f2;
  c9_l_b = c9_ry2;
  c9_l_y = c9_l_a * c9_l_b;
  c9_m_a = c9_l_y;
  c9_m_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_m_b);
  c9_m_y = c9_m_a * c9_m_b;
  c9_n_a = c9_e_y;
  c9_n_b = (((c9_g_y - c9_Mref) + c9_i_y) + c9_k_y) + c9_m_y;
  c9_n_y = c9_n_a * c9_n_b;
  c9_o_a = c9_C[5];
  c9_o_b = c9_pretheta1 - c9_theta1;
  c9_o_y = c9_o_a * c9_o_b;
  c9_p_a = c9_C[1];
  c9_p_b = c9_f1;
  c9_p_y = c9_p_a * c9_p_b;
  c9_q_a = c9_p_y;
  c9_q_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_q_b);
  c9_q_y = c9_q_a * c9_q_b;
  c9_r_a = c9_f1;
  c9_r_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_r_b);
  c9_r_y = c9_r_a * c9_r_b;
  c9_s_a = c9_f2;
  c9_s_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_s_b);
  c9_s_y = c9_s_a * c9_s_b;
  c9_t_a = c9_q_y;
  c9_t_b = (c9_r_y - c9_Fyref) + c9_s_y;
  c9_t_y = c9_t_a * c9_t_b;
  c9_u_a = c9_C[0];
  c9_u_b = c9_f1;
  c9_u_y = c9_u_a * c9_u_b;
  c9_v_a = c9_u_y;
  c9_v_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_v_b);
  c9_v_y = c9_v_a * c9_v_b;
  c9_w_a = c9_f1;
  c9_w_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_w_b);
  c9_w_y = c9_w_a * c9_w_b;
  c9_x_a = c9_f2;
  c9_x_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_x_b);
  c9_x_y = c9_x_a * c9_x_b;
  c9_y_a = c9_v_y;
  c9_y_b = (c9_Fxref + c9_w_y) + c9_x_y;
  c9_y_y = c9_y_a * c9_y_b;
  c9_dVdtheta1 = ((c9_n_y - c9_o_y) - c9_t_y) + c9_y_y;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 172U);
  c9_ab_a = c9_C[3];
  c9_ab_b = c9_f1;
  c9_ab_y = c9_ab_a * c9_ab_b;
  c9_bb_a = c9_C[0];
  c9_bb_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_bb_b);
  c9_bb_y = c9_bb_a * c9_bb_b;
  c9_c_x = c9_theta1;
  c9_d_x = c9_c_x;
  c9_d_x = muDoubleScalarSin(c9_d_x);
  c9_cb_a = c9_f1;
  c9_cb_b = c9_d_x;
  c9_cb_y = c9_cb_a * c9_cb_b;
  c9_db_a = c9_f2;
  c9_db_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_db_b);
  c9_db_y = c9_db_a * c9_db_b;
  c9_eb_a = c9_bb_y;
  c9_eb_b = (c9_Fxref + c9_cb_y) + c9_db_y;
  c9_eb_y = c9_eb_a * c9_eb_b;
  c9_fb_a = c9_C[1];
  c9_fb_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_fb_b);
  c9_fb_y = c9_fb_a * c9_fb_b;
  c9_gb_a = c9_f1;
  c9_gb_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_gb_b);
  c9_gb_y = c9_gb_a * c9_gb_b;
  c9_hb_a = c9_f2;
  c9_hb_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_hb_b);
  c9_hb_y = c9_hb_a * c9_hb_b;
  c9_ib_a = c9_fb_y;
  c9_ib_b = (c9_gb_y - c9_Fyref) + c9_hb_y;
  c9_ib_y = c9_ib_a * c9_ib_b;
  c9_jb_a = c9_rx1;
  c9_jb_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_jb_b);
  c9_jb_y = c9_jb_a * c9_jb_b;
  c9_kb_a = c9_ry1;
  c9_kb_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_kb_b);
  c9_kb_y = c9_kb_a * c9_kb_b;
  c9_lb_a = c9_C[2];
  c9_lb_b = c9_jb_y + c9_kb_y;
  c9_lb_y = c9_lb_a * c9_lb_b;
  c9_mb_a = c9_f1;
  c9_mb_b = c9_rx1;
  c9_mb_y = c9_mb_a * c9_mb_b;
  c9_nb_a = c9_mb_y;
  c9_nb_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_nb_b);
  c9_nb_y = c9_nb_a * c9_nb_b;
  c9_ob_a = c9_f2;
  c9_ob_b = c9_rx2;
  c9_ob_y = c9_ob_a * c9_ob_b;
  c9_pb_a = c9_ob_y;
  c9_pb_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_pb_b);
  c9_pb_y = c9_pb_a * c9_pb_b;
  c9_qb_a = c9_f1;
  c9_qb_b = c9_ry1;
  c9_qb_y = c9_qb_a * c9_qb_b;
  c9_rb_a = c9_qb_y;
  c9_rb_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_rb_b);
  c9_rb_y = c9_rb_a * c9_rb_b;
  c9_sb_a = c9_f2;
  c9_sb_b = c9_ry2;
  c9_sb_y = c9_sb_a * c9_sb_b;
  c9_tb_a = c9_sb_y;
  c9_tb_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_tb_b);
  c9_tb_y = c9_tb_a * c9_tb_b;
  c9_ub_a = c9_lb_y;
  c9_ub_b = (((c9_nb_y - c9_Mref) + c9_pb_y) + c9_rb_y) + c9_tb_y;
  c9_ub_y = c9_ub_a * c9_ub_b;
  c9_dVdf1 = ((c9_ab_y + c9_eb_y) + c9_ib_y) + c9_ub_y;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 178U);
  c9_vb_a = c9_f2;
  c9_vb_b = c9_ry2;
  c9_vb_y = c9_vb_a * c9_vb_b;
  c9_wb_a = c9_vb_y;
  c9_wb_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_wb_b);
  c9_wb_y = c9_wb_a * c9_wb_b;
  c9_xb_a = c9_f2;
  c9_xb_b = c9_rx2;
  c9_xb_y = c9_xb_a * c9_xb_b;
  c9_yb_a = c9_xb_y;
  c9_yb_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_yb_b);
  c9_yb_y = c9_yb_a * c9_yb_b;
  c9_ac_a = c9_C[2];
  c9_ac_b = c9_wb_y - c9_yb_y;
  c9_ac_y = c9_ac_a * c9_ac_b;
  c9_bc_a = c9_f1;
  c9_bc_b = c9_rx1;
  c9_bc_y = c9_bc_a * c9_bc_b;
  c9_cc_a = c9_bc_y;
  c9_cc_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_cc_b);
  c9_cc_y = c9_cc_a * c9_cc_b;
  c9_dc_a = c9_f2;
  c9_dc_b = c9_rx2;
  c9_dc_y = c9_dc_a * c9_dc_b;
  c9_ec_a = c9_dc_y;
  c9_ec_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_ec_b);
  c9_ec_y = c9_ec_a * c9_ec_b;
  c9_fc_a = c9_f1;
  c9_fc_b = c9_ry1;
  c9_fc_y = c9_fc_a * c9_fc_b;
  c9_gc_a = c9_fc_y;
  c9_gc_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_gc_b);
  c9_gc_y = c9_gc_a * c9_gc_b;
  c9_hc_a = c9_f2;
  c9_hc_b = c9_ry2;
  c9_hc_y = c9_hc_a * c9_hc_b;
  c9_ic_a = c9_hc_y;
  c9_ic_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_ic_b);
  c9_ic_y = c9_ic_a * c9_ic_b;
  c9_jc_a = c9_ac_y;
  c9_jc_b = (((c9_cc_y - c9_Mref) + c9_ec_y) + c9_gc_y) + c9_ic_y;
  c9_jc_y = c9_jc_a * c9_jc_b;
  c9_kc_a = c9_C[6];
  c9_kc_b = c9_pretheta2 - c9_theta2;
  c9_kc_y = c9_kc_a * c9_kc_b;
  c9_lc_a = c9_C[1];
  c9_lc_b = c9_f2;
  c9_lc_y = c9_lc_a * c9_lc_b;
  c9_mc_a = c9_lc_y;
  c9_mc_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_mc_b);
  c9_mc_y = c9_mc_a * c9_mc_b;
  c9_nc_a = c9_f1;
  c9_nc_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_nc_b);
  c9_nc_y = c9_nc_a * c9_nc_b;
  c9_oc_a = c9_f2;
  c9_oc_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_oc_b);
  c9_oc_y = c9_oc_a * c9_oc_b;
  c9_pc_a = c9_mc_y;
  c9_pc_b = (c9_nc_y - c9_Fyref) + c9_oc_y;
  c9_pc_y = c9_pc_a * c9_pc_b;
  c9_qc_a = c9_C[0];
  c9_qc_b = c9_f2;
  c9_qc_y = c9_qc_a * c9_qc_b;
  c9_rc_a = c9_qc_y;
  c9_rc_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_rc_b);
  c9_rc_y = c9_rc_a * c9_rc_b;
  c9_sc_a = c9_f1;
  c9_sc_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_sc_b);
  c9_sc_y = c9_sc_a * c9_sc_b;
  c9_tc_a = c9_f2;
  c9_tc_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_tc_b);
  c9_tc_y = c9_tc_a * c9_tc_b;
  c9_uc_a = c9_rc_y;
  c9_uc_b = (c9_Fxref + c9_sc_y) + c9_tc_y;
  c9_uc_y = c9_uc_a * c9_uc_b;
  c9_dVdtheta2 = ((c9_jc_y - c9_kc_y) - c9_pc_y) + c9_uc_y;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 185U);
  c9_vc_a = c9_C[4];
  c9_vc_b = c9_f2;
  c9_vc_y = c9_vc_a * c9_vc_b;
  c9_wc_a = c9_C[0];
  c9_wc_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_wc_b);
  c9_wc_y = c9_wc_a * c9_wc_b;
  c9_xc_a = c9_f1;
  c9_xc_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_xc_b);
  c9_xc_y = c9_xc_a * c9_xc_b;
  c9_yc_a = c9_f2;
  c9_yc_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_yc_b);
  c9_yc_y = c9_yc_a * c9_yc_b;
  c9_ad_a = c9_wc_y;
  c9_ad_b = (c9_Fxref + c9_xc_y) + c9_yc_y;
  c9_ad_y = c9_ad_a * c9_ad_b;
  c9_bd_a = c9_C[1];
  c9_bd_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_bd_b);
  c9_bd_y = c9_bd_a * c9_bd_b;
  c9_cd_a = c9_f1;
  c9_cd_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_cd_b);
  c9_cd_y = c9_cd_a * c9_cd_b;
  c9_dd_a = c9_f2;
  c9_dd_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_dd_b);
  c9_dd_y = c9_dd_a * c9_dd_b;
  c9_ed_a = c9_bd_y;
  c9_ed_b = (c9_cd_y - c9_Fyref) + c9_dd_y;
  c9_ed_y = c9_ed_a * c9_ed_b;
  c9_fd_a = c9_rx2;
  c9_fd_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_fd_b);
  c9_fd_y = c9_fd_a * c9_fd_b;
  c9_gd_a = c9_ry2;
  c9_gd_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_gd_b);
  c9_gd_y = c9_gd_a * c9_gd_b;
  c9_hd_a = c9_C[2];
  c9_hd_b = c9_fd_y + c9_gd_y;
  c9_hd_y = c9_hd_a * c9_hd_b;
  c9_id_a = c9_f1;
  c9_id_b = c9_rx1;
  c9_id_y = c9_id_a * c9_id_b;
  c9_jd_a = c9_id_y;
  c9_jd_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_jd_b);
  c9_jd_y = c9_jd_a * c9_jd_b;
  c9_kd_a = c9_f2;
  c9_kd_b = c9_rx2;
  c9_kd_y = c9_kd_a * c9_kd_b;
  c9_ld_a = c9_kd_y;
  c9_ld_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_ld_b);
  c9_ld_y = c9_ld_a * c9_ld_b;
  c9_md_a = c9_f1;
  c9_md_b = c9_ry1;
  c9_md_y = c9_md_a * c9_md_b;
  c9_nd_a = c9_md_y;
  c9_nd_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_nd_b);
  c9_nd_y = c9_nd_a * c9_nd_b;
  c9_od_a = c9_f2;
  c9_od_b = c9_ry2;
  c9_od_y = c9_od_a * c9_od_b;
  c9_pd_a = c9_od_y;
  c9_pd_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_pd_b);
  c9_pd_y = c9_pd_a * c9_pd_b;
  c9_qd_a = c9_hd_y;
  c9_qd_b = (((c9_jd_y - c9_Mref) + c9_ld_y) + c9_nd_y) + c9_pd_y;
  c9_qd_y = c9_qd_a * c9_qd_b;
  c9_dVdf2 = ((c9_vc_y + c9_ad_y) + c9_ed_y) + c9_qd_y;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 191U);
  c9_b_dVdtheta1[0] = c9_dVdtheta1;
  c9_b_dVdtheta1[1] = c9_dVdf1;
  c9_b_dVdtheta1[2] = c9_dVdtheta2;
  c9_b_dVdtheta1[3] = c9_dVdf2;
  for (c9_i99 = 0; c9_i99 < 4; c9_i99++) {
    c9_output[c9_i99] = -c9_b_dVdtheta1[c9_i99];
  }

  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, -191);
  _SFD_SYMBOL_SCOPE_POP();
}

static real_T c9_cosd(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
                      real_T c9_x)
{
  real_T c9_b_x;
  c9_b_x = c9_x;
  c9_b_cosd(chartInstance, &c9_b_x);
  return c9_b_x;
}

static boolean_T c9_isfinite(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c9_x)
{
  real_T c9_b_x;
  boolean_T c9_b_b;
  boolean_T c9_b0;
  real_T c9_c_x;
  boolean_T c9_c_b;
  boolean_T c9_b1;
  c9_b_x = c9_x;
  c9_b_b = muDoubleScalarIsInf(c9_b_x);
  c9_b0 = !c9_b_b;
  c9_c_x = c9_x;
  c9_c_b = muDoubleScalarIsNaN(c9_c_x);
  c9_b1 = !c9_c_b;
  return c9_b0 && c9_b1;
}

static real_T c9_sind(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
                      real_T c9_x)
{
  real_T c9_b_x;
  c9_b_x = c9_x;
  c9_b_sind(chartInstance, &c9_b_x);
  return c9_b_x;
}

static real_T c9_costFunction(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, real_T c9_input[4], real_T c9_preinput[4], real_T c9_Fxref,
  real_T c9_Fyref, real_T c9_Mref, real_T c9_rx1, real_T c9_rx2, real_T c9_ry1,
  real_T c9_ry2, real_T c9_C[7])
{
  real_T c9_output;
  uint32_T c9_debug_family_var_map[22];
  real_T c9_theta1;
  real_T c9_f1;
  real_T c9_theta2;
  real_T c9_f2;
  real_T c9_pretheta1;
  real_T c9_pretheta2;
  real_T c9_Fx;
  real_T c9_Fy;
  real_T c9_M;
  real_T c9_nargin = 10.0;
  real_T c9_nargout = 1.0;
  real_T c9_a;
  real_T c9_b;
  real_T c9_y;
  real_T c9_b_a;
  real_T c9_b_b;
  real_T c9_b_y;
  real_T c9_c_a;
  real_T c9_c_b;
  real_T c9_c_y;
  real_T c9_d_a;
  real_T c9_d_b;
  real_T c9_d_y;
  real_T c9_e_a;
  real_T c9_e_b;
  real_T c9_e_y;
  real_T c9_f_a;
  real_T c9_f_b;
  real_T c9_f_y;
  real_T c9_g_a;
  real_T c9_g_b;
  real_T c9_g_y;
  real_T c9_h_a;
  real_T c9_h_b;
  real_T c9_h_y;
  real_T c9_i_a;
  real_T c9_i_b;
  real_T c9_i_y;
  real_T c9_j_a;
  real_T c9_j_b;
  real_T c9_j_y;
  real_T c9_k_a;
  real_T c9_k_b;
  real_T c9_k_y;
  real_T c9_l_a;
  real_T c9_l_b;
  real_T c9_l_y;
  real_T c9_m_a;
  real_T c9_m_b;
  real_T c9_m_y;
  real_T c9_n_a;
  real_T c9_n_b;
  real_T c9_n_y;
  real_T c9_o_a;
  real_T c9_o_b;
  real_T c9_o_y;
  real_T c9_p_a;
  real_T c9_p_b;
  real_T c9_p_y;
  real_T c9_q_a;
  real_T c9_q_b;
  real_T c9_q_y;
  real_T c9_r_a;
  real_T c9_r_b;
  real_T c9_r_y;
  real_T c9_s_a;
  real_T c9_s_b;
  real_T c9_s_y;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 22U, 22U, c9_e_debug_family_names,
    c9_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_theta1, 0U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_f1, 1U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_theta2, 2U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_f2, 3U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_pretheta1, 4U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_pretheta2, 5U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Fx, 6U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Fy, 7U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_M, 8U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_nargin, 9U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_nargout, 10U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_input, 11U, c9_sf_marshallOut,
    c9_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_preinput, 12U, c9_sf_marshallOut,
    c9_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Fxref, 13U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Fyref, 14U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_Mref, 15U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_rx1, 16U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_rx2, 17U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_ry1, 18U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_ry2, 19U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c9_C, 20U, c9_d_sf_marshallOut,
    c9_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c9_output, 21U, c9_b_sf_marshallOut,
    c9_b_sf_marshallIn);
  CV_EML_FCN(0, 6);
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 121);
  c9_theta1 = c9_input[0];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 122);
  c9_f1 = c9_input[1];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 123);
  c9_theta2 = c9_input[2];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 124);
  c9_f2 = c9_input[3];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 126);
  c9_pretheta1 = c9_preinput[0];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, MAX_int8_T);
  c9_pretheta2 = c9_preinput[2];
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 129U);
  c9_a = -c9_f1;
  c9_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_b);
  c9_y = c9_a * c9_b;
  c9_b_a = c9_f2;
  c9_b_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_b_b);
  c9_b_y = c9_b_a * c9_b_b;
  c9_Fx = c9_y - c9_b_y;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 130U);
  c9_c_a = c9_f1;
  c9_c_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_c_b);
  c9_c_y = c9_c_a * c9_c_b;
  c9_d_a = c9_f2;
  c9_d_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_d_b);
  c9_d_y = c9_d_a * c9_d_b;
  c9_Fy = c9_c_y + c9_d_y;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 131U);
  c9_e_a = c9_rx1;
  c9_e_b = c9_f1;
  c9_e_y = c9_e_a * c9_e_b;
  c9_f_a = c9_e_y;
  c9_f_b = c9_theta1;
  c9_b_cosd(chartInstance, &c9_f_b);
  c9_f_y = c9_f_a * c9_f_b;
  c9_g_a = c9_ry1;
  c9_g_b = c9_f1;
  c9_g_y = c9_g_a * c9_g_b;
  c9_h_a = c9_g_y;
  c9_h_b = c9_theta1;
  c9_b_sind(chartInstance, &c9_h_b);
  c9_h_y = c9_h_a * c9_h_b;
  c9_i_a = c9_rx2;
  c9_i_b = c9_f2;
  c9_i_y = c9_i_a * c9_i_b;
  c9_j_a = c9_i_y;
  c9_j_b = c9_theta2;
  c9_b_cosd(chartInstance, &c9_j_b);
  c9_j_y = c9_j_a * c9_j_b;
  c9_k_a = c9_ry2;
  c9_k_b = c9_f2;
  c9_k_y = c9_k_a * c9_k_b;
  c9_l_a = c9_k_y;
  c9_l_b = c9_theta2;
  c9_b_sind(chartInstance, &c9_l_b);
  c9_l_y = c9_l_a * c9_l_b;
  c9_M = ((c9_f_y + c9_h_y) + c9_j_y) + c9_l_y;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, 134U);
  c9_m_a = c9_C[0];
  c9_m_b = c9_mpower(chartInstance, c9_Fx - c9_Fxref);
  c9_m_y = c9_m_a * c9_m_b;
  c9_n_a = c9_C[1];
  c9_n_b = c9_mpower(chartInstance, c9_Fy - c9_Fyref);
  c9_n_y = c9_n_a * c9_n_b;
  c9_o_a = c9_C[2];
  c9_o_b = c9_mpower(chartInstance, c9_M - c9_Mref);
  c9_o_y = c9_o_a * c9_o_b;
  c9_p_a = c9_C[3];
  c9_p_b = c9_mpower(chartInstance, c9_f1);
  c9_p_y = c9_p_a * c9_p_b;
  c9_q_a = c9_C[4];
  c9_q_b = c9_mpower(chartInstance, c9_f2);
  c9_q_y = c9_q_a * c9_q_b;
  c9_r_a = c9_C[5];
  c9_r_b = c9_mpower(chartInstance, c9_theta1 - c9_pretheta1);
  c9_r_y = c9_r_a * c9_r_b;
  c9_s_a = c9_C[6];
  c9_s_b = c9_mpower(chartInstance, c9_theta2 - c9_pretheta2);
  c9_s_y = c9_s_a * c9_s_b;
  c9_output = (((((c9_m_y + c9_n_y) + c9_o_y) + c9_p_y) + c9_q_y) + c9_r_y) +
    c9_s_y;
  _SFD_EML_CALL(0U, chartInstance->c9_sfEvent, -134);
  _SFD_SYMBOL_SCOPE_POP();
  return c9_output;
}

static real_T c9_mpower(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
  real_T c9_a)
{
  real_T c9_b_a;
  real_T c9_c_a;
  real_T c9_ak;
  real_T c9_d_a;
  real_T c9_e_a;
  real_T c9_b;
  c9_b_a = c9_a;
  c9_c_a = c9_b_a;
  c9_eml_scalar_eg(chartInstance);
  c9_ak = c9_c_a;
  c9_d_a = c9_ak;
  c9_eml_scalar_eg(chartInstance);
  c9_e_a = c9_d_a;
  c9_b = c9_d_a;
  return c9_e_a * c9_b;
}

static const mxArray *c9_h_sf_marshallOut(void *chartInstanceVoid, void
  *c9_inData)
{
  const mxArray *c9_mxArrayOutData = NULL;
  int32_T c9_u;
  const mxArray *c9_y = NULL;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_mxArrayOutData = NULL;
  c9_u = *(int32_T *)c9_inData;
  c9_y = NULL;
  sf_mex_assign(&c9_y, sf_mex_create("y", &c9_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c9_mxArrayOutData, c9_y, FALSE);
  return c9_mxArrayOutData;
}

static int32_T c9_i_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId)
{
  int32_T c9_y;
  int32_T c9_i100;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), &c9_i100, 1, 6, 0U, 0, 0U, 0);
  c9_y = c9_i100;
  sf_mex_destroy(&c9_u);
  return c9_y;
}

static void c9_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c9_mxArrayInData, const char_T *c9_varName, void *c9_outData)
{
  const mxArray *c9_b_sfEvent;
  const char_T *c9_identifier;
  emlrtMsgIdentifier c9_thisId;
  int32_T c9_y;
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)chartInstanceVoid;
  c9_b_sfEvent = sf_mex_dup(c9_mxArrayInData);
  c9_identifier = c9_varName;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_y = c9_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c9_b_sfEvent),
    &c9_thisId);
  sf_mex_destroy(&c9_b_sfEvent);
  *(int32_T *)c9_outData = c9_y;
  sf_mex_destroy(&c9_mxArrayInData);
}

static uint8_T c9_j_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_b_is_active_c9_BoatLibraryTest02, const
  char_T *c9_identifier)
{
  uint8_T c9_y;
  emlrtMsgIdentifier c9_thisId;
  c9_thisId.fIdentifier = c9_identifier;
  c9_thisId.fParent = NULL;
  c9_y = c9_k_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c9_b_is_active_c9_BoatLibraryTest02), &c9_thisId);
  sf_mex_destroy(&c9_b_is_active_c9_BoatLibraryTest02);
  return c9_y;
}

static uint8_T c9_k_emlrt_marshallIn(SFc9_BoatLibraryTest02InstanceStruct
  *chartInstance, const mxArray *c9_u, const emlrtMsgIdentifier *c9_parentId)
{
  uint8_T c9_y;
  uint8_T c9_u0;
  sf_mex_import(c9_parentId, sf_mex_dup(c9_u), &c9_u0, 1, 3, 0U, 0, 0U, 0);
  c9_y = c9_u0;
  sf_mex_destroy(&c9_u);
  return c9_y;
}

static void c9_b_cosd(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
                      real_T *c9_x)
{
  real_T c9_b_x;
  real_T c9_c_x;
  real_T c9_d_x;
  real_T c9_e_x;
  real_T c9_xk;
  real_T c9_f_x;
  real_T c9_g_x;
  real_T c9_absx;
  real_T c9_h_x;
  real_T c9_i_x;
  real_T c9_b;
  int8_T c9_n;
  real_T c9_b_b;
  real_T c9_c_b;
  real_T c9_d_b;
  real_T c9_e_b;
  int8_T c9_b_n;
  c9_b_x = *c9_x;
  if (!c9_isfinite(chartInstance, c9_b_x)) {
    *c9_x = rtNaN;
  } else {
    c9_c_x = c9_b_x;
    c9_d_x = c9_c_x;
    c9_e_x = c9_d_x;
    c9_eml_scalar_eg(chartInstance);
    c9_xk = c9_e_x;
    c9_d_x = muDoubleScalarRem(c9_xk, 360.0);
    c9_f_x = c9_d_x;
    c9_g_x = c9_f_x;
    c9_absx = muDoubleScalarAbs(c9_g_x);
    if (c9_absx > 180.0) {
      if (c9_d_x > 0.0) {
        c9_d_x -= 360.0;
      } else {
        c9_d_x += 360.0;
      }

      c9_h_x = c9_d_x;
      c9_i_x = c9_h_x;
      c9_absx = muDoubleScalarAbs(c9_i_x);
    }

    if (c9_absx <= 45.0) {
      c9_b = c9_d_x;
      c9_d_x = 0.017453292519943295 * c9_b;
      c9_n = 0;
    } else if (c9_absx <= 135.0) {
      if (c9_d_x > 0.0) {
        c9_b_b = c9_d_x - 90.0;
        c9_d_x = 0.017453292519943295 * c9_b_b;
        c9_n = 1;
      } else {
        c9_c_b = c9_d_x + 90.0;
        c9_d_x = 0.017453292519943295 * c9_c_b;
        c9_n = -1;
      }
    } else if (c9_d_x > 0.0) {
      c9_d_b = c9_d_x - 180.0;
      c9_d_x = 0.017453292519943295 * c9_d_b;
      c9_n = 2;
    } else {
      c9_e_b = c9_d_x + 180.0;
      c9_d_x = 0.017453292519943295 * c9_e_b;
      c9_n = -2;
    }

    c9_b_n = c9_n;
    c9_b_x = c9_d_x;
    if ((real_T)c9_b_n == 0.0) {
      *c9_x = muDoubleScalarCos(c9_b_x);
    } else if ((real_T)c9_b_n == 1.0) {
      *c9_x = -muDoubleScalarSin(c9_b_x);
    } else if ((real_T)c9_b_n == -1.0) {
      *c9_x = muDoubleScalarSin(c9_b_x);
    } else {
      *c9_x = -muDoubleScalarCos(c9_b_x);
    }
  }
}

static void c9_b_sind(SFc9_BoatLibraryTest02InstanceStruct *chartInstance,
                      real_T *c9_x)
{
  real_T c9_b_x;
  real_T c9_d1;
  real_T c9_c_x;
  real_T c9_d_x;
  real_T c9_e_x;
  real_T c9_xk;
  real_T c9_f_x;
  real_T c9_g_x;
  real_T c9_absx;
  real_T c9_h_x;
  real_T c9_i_x;
  real_T c9_b;
  int8_T c9_n;
  real_T c9_b_b;
  real_T c9_c_b;
  real_T c9_d_b;
  real_T c9_e_b;
  int8_T c9_b_n;
  c9_b_x = *c9_x;
  if (!c9_isfinite(chartInstance, c9_b_x)) {
    c9_d1 = rtNaN;
  } else {
    c9_c_x = c9_b_x;
    c9_d_x = c9_c_x;
    c9_e_x = c9_d_x;
    c9_eml_scalar_eg(chartInstance);
    c9_xk = c9_e_x;
    c9_d_x = muDoubleScalarRem(c9_xk, 360.0);
    c9_f_x = c9_d_x;
    c9_g_x = c9_f_x;
    c9_absx = muDoubleScalarAbs(c9_g_x);
    if (c9_absx > 180.0) {
      if (c9_d_x > 0.0) {
        c9_d_x -= 360.0;
      } else {
        c9_d_x += 360.0;
      }

      c9_h_x = c9_d_x;
      c9_i_x = c9_h_x;
      c9_absx = muDoubleScalarAbs(c9_i_x);
    }

    if (c9_absx <= 45.0) {
      c9_b = c9_d_x;
      c9_d_x = 0.017453292519943295 * c9_b;
      c9_n = 0;
    } else if (c9_absx <= 135.0) {
      if (c9_d_x > 0.0) {
        c9_b_b = c9_d_x - 90.0;
        c9_d_x = 0.017453292519943295 * c9_b_b;
        c9_n = 1;
      } else {
        c9_c_b = c9_d_x + 90.0;
        c9_d_x = 0.017453292519943295 * c9_c_b;
        c9_n = -1;
      }
    } else if (c9_d_x > 0.0) {
      c9_d_b = c9_d_x - 180.0;
      c9_d_x = 0.017453292519943295 * c9_d_b;
      c9_n = 2;
    } else {
      c9_e_b = c9_d_x + 180.0;
      c9_d_x = 0.017453292519943295 * c9_e_b;
      c9_n = -2;
    }

    c9_b_n = c9_n;
    c9_b_x = c9_d_x;
    if ((real_T)c9_b_n == 0.0) {
      c9_d1 = muDoubleScalarSin(c9_b_x);
    } else if ((real_T)c9_b_n == 1.0) {
      c9_d1 = muDoubleScalarCos(c9_b_x);
    } else if ((real_T)c9_b_n == -1.0) {
      c9_d1 = -muDoubleScalarCos(c9_b_x);
    } else {
      c9_d1 = -muDoubleScalarSin(c9_b_x);
    }
  }

  *c9_x = c9_d1;
}

static void init_dsm_address_info(SFc9_BoatLibraryTest02InstanceStruct
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

void sf_c9_BoatLibraryTest02_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3972372168U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2662454249U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(786910651U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1662844860U);
}

mxArray *sf_c9_BoatLibraryTest02_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("phh1a14zSwn6zo1dkRay9");
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

mxArray *sf_c9_BoatLibraryTest02_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c9_BoatLibraryTest02_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c9_BoatLibraryTest02(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"output\",},{M[8],M[0],T\"is_active_c9_BoatLibraryTest02\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c9_BoatLibraryTest02_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
    chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _BoatLibraryTest02MachineNumber_,
           9,
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
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,603);
        _SFD_CV_INIT_EML_FCN(0,1,"getMinFromGrid",605,-1,1290);
        _SFD_CV_INIT_EML_FCN(0,2,"checkInputBound",1292,-1,1460);
        _SFD_CV_INIT_EML_FCN(0,3,"boundGrid",1462,-1,1798);
        _SFD_CV_INIT_EML_FCN(0,4,"sat",1800,-1,1988);
        _SFD_CV_INIT_EML_FCN(0,5,"getStepSize",1990,-1,2800);
        _SFD_CV_INIT_EML_FCN(0,6,"costFunction",2802,-1,3479);
        _SFD_CV_INIT_EML_FCN(0,7,"getDirection",3481,-1,3744);
        _SFD_CV_INIT_EML_FCN(0,8,"SteepestDescent",3746,-1,5366);
        _SFD_CV_INIT_EML_IF(0,1,0,1112,1122,1204,1245);
        _SFD_CV_INIT_EML_IF(0,1,1,1841,1860,1892,1983);
        _SFD_CV_INIT_EML_IF(0,1,2,1892,1915,1947,1983);
        _SFD_CV_INIT_EML_FOR(0,1,0,323,337,576);
        _SFD_CV_INIT_EML_FOR(0,1,1,859,872,1285);
        _SFD_CV_INIT_EML_FOR(0,1,2,876,889,1281);
        _SFD_CV_INIT_EML_FOR(0,1,3,897,910,1273);
        _SFD_CV_INIT_EML_FOR(0,1,4,922,935,1261);
        _SFD_CV_INIT_EML_FOR(0,1,5,1363,1387,1440);
        _SFD_CV_INIT_EML_FOR(0,1,6,3683,3705,3740);
        _SFD_CV_INIT_EML_WHILE(0,1,0,2317,2343,2510);
        _SFD_CV_INIT_EML_WHILE(0,1,1,2534,2573,2743);

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c9_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c9_sf_marshallOut,(MexInFcnForType)
            c9_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c9_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c9_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c9_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c9_b_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 7;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c9_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 4;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c9_c_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c9_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c9_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c9_b_sf_marshallOut,(MexInFcnForType)NULL);

        {
          real_T *c9_rx01;
          real_T *c9_rx02;
          real_T *c9_ry01;
          real_T *c9_ry02;
          real_T *c9_Fx;
          real_T *c9_Fy;
          real_T *c9_M;
          real_T (*c9_u0)[4];
          real_T (*c9_output)[4];
          real_T (*c9_C0)[7];
          real_T (*c9_inputBound)[8];
          c9_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
          c9_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
          c9_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
          c9_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S,
            6);
          c9_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
          c9_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
          c9_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c9_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c9_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c9_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
          c9_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, *c9_u0);
          _SFD_SET_DATA_VALUE_PTR(1U, *c9_output);
          _SFD_SET_DATA_VALUE_PTR(2U, c9_rx01);
          _SFD_SET_DATA_VALUE_PTR(3U, c9_rx02);
          _SFD_SET_DATA_VALUE_PTR(4U, c9_ry01);
          _SFD_SET_DATA_VALUE_PTR(5U, c9_ry02);
          _SFD_SET_DATA_VALUE_PTR(6U, *c9_C0);
          _SFD_SET_DATA_VALUE_PTR(7U, *c9_inputBound);
          _SFD_SET_DATA_VALUE_PTR(8U, c9_Fx);
          _SFD_SET_DATA_VALUE_PTR(9U, c9_Fy);
          _SFD_SET_DATA_VALUE_PTR(10U, c9_M);
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
  return "WY65oJijP2n0bKtiUYBQn";
}

static void sf_opaque_initialize_c9_BoatLibraryTest02(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc9_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
  initialize_c9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c9_BoatLibraryTest02(void *chartInstanceVar)
{
  enable_c9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c9_BoatLibraryTest02(void *chartInstanceVar)
{
  disable_c9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c9_BoatLibraryTest02(void *chartInstanceVar)
{
  sf_c9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c9_BoatLibraryTest02(SimStruct*
  S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c9_BoatLibraryTest02
    ((SFc9_BoatLibraryTest02InstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c9_BoatLibraryTest02();/* state var info */
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

extern void sf_internal_set_sim_state_c9_BoatLibraryTest02(SimStruct* S, const
  mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c9_BoatLibraryTest02();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c9_BoatLibraryTest02(SimStruct* S)
{
  return sf_internal_get_sim_state_c9_BoatLibraryTest02(S);
}

static void sf_opaque_set_sim_state_c9_BoatLibraryTest02(SimStruct* S, const
  mxArray *st)
{
  sf_internal_set_sim_state_c9_BoatLibraryTest02(S, st);
}

static void sf_opaque_terminate_c9_BoatLibraryTest02(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc9_BoatLibraryTest02InstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_BoatLibraryTest02_optimization_info();
    }

    finalize_c9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
      chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c9_BoatLibraryTest02(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c9_BoatLibraryTest02((SFc9_BoatLibraryTest02InstanceStruct*)
      (((ChartInfoStruct *)ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c9_BoatLibraryTest02(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_BoatLibraryTest02_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      9);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,9,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,9,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,9);
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
        infoStruct,9,10);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,9,1);
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

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,9);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(4109308754U));
  ssSetChecksum1(S,(1315261542U));
  ssSetChecksum2(S,(328635509U));
  ssSetChecksum3(S,(3522237483U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c9_BoatLibraryTest02(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c9_BoatLibraryTest02(SimStruct *S)
{
  SFc9_BoatLibraryTest02InstanceStruct *chartInstance;
  chartInstance = (SFc9_BoatLibraryTest02InstanceStruct *)utMalloc(sizeof
    (SFc9_BoatLibraryTest02InstanceStruct));
  memset(chartInstance, 0, sizeof(SFc9_BoatLibraryTest02InstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c9_BoatLibraryTest02;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c9_BoatLibraryTest02;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c9_BoatLibraryTest02;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c9_BoatLibraryTest02;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c9_BoatLibraryTest02;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c9_BoatLibraryTest02;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c9_BoatLibraryTest02;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c9_BoatLibraryTest02;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c9_BoatLibraryTest02;
  chartInstance->chartInfo.mdlStart = mdlStart_c9_BoatLibraryTest02;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c9_BoatLibraryTest02;
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

void c9_BoatLibraryTest02_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c9_BoatLibraryTest02(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c9_BoatLibraryTest02(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c9_BoatLibraryTest02(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c9_BoatLibraryTest02_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
