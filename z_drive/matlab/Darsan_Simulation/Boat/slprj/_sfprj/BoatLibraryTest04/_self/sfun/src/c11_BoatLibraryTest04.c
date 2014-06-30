/* Include files */

#include <stddef.h>
#include "blas.h"
#include "BoatLibraryTest04_sfun.h"
#include "c11_BoatLibraryTest04.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "BoatLibraryTest04_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c11_debug_family_names[24] = { "rx1", "rx2", "ry1", "ry2",
  "C", "Fxref", "Fyref", "Mref", "gridNum", "grid", "u1", "nargin", "nargout",
  "u0", "rx01", "rx02", "ry01", "ry02", "C0", "inputBound", "Fx", "Fy", "M",
  "output" };

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

/* Function Declarations */
static void initialize_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance);
static void initialize_params_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance);
static void enable_c11_BoatLibraryTest04(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance);
static void disable_c11_BoatLibraryTest04(SFc11_BoatLibraryTest04InstanceStruct *
  chartInstance);
static void c11_update_debugger_state_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance);
static const mxArray *get_sim_state_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance);
static void set_sim_state_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance, const mxArray *c11_st);
static void finalize_c11_BoatLibraryTest04(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance);
static void sf_c11_BoatLibraryTest04(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance);
static void c11_chartstep_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance);
static void initSimStructsc11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c11_machineNumber, uint32_T
  c11_chartNumber);
static const mxArray *c11_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_output, const char_T *c11_identifier,
  real_T c11_y[4]);
static void c11_b_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
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
static real_T c11_c_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId);
static void c11_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static const mxArray *c11_e_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_d_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[92]);
static void c11_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static void c11_e_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[7]);
static void c11_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static void c11_f_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[8]);
static void c11_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static const mxArray *c11_f_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_g_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[20]);
static void c11_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static const mxArray *c11_g_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static void c11_h_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[4]);
static void c11_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static void c11_info_helper(const mxArray **c11_info);
static const mxArray *c11_emlrt_marshallOut(char * c11_u);
static const mxArray *c11_b_emlrt_marshallOut(uint32_T c11_u);
static void c11_eml_scalar_eg(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance);
static void c11_linspace(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c11_d1, real_T c11_d2, real_T c11_y[20]);
static real_T c11_costFunction(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, real_T c11_input[4], real_T c11_preinput[4], real_T c11_Fxref,
  real_T c11_Fyref, real_T c11_Mref, real_T c11_rx1, real_T c11_rx2, real_T
  c11_ry1, real_T c11_ry2, real_T c11_C[7]);
static real_T c11_sind(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c11_x);
static boolean_T c11_isfinite(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, real_T c11_x);
static real_T c11_cosd(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c11_x);
static real_T c11_mpower(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c11_a);
static real_T c11_b_costFunction(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, real_T c11_input[4], real_T c11_preinput[4], real_T c11_Fxref,
  real_T c11_Fyref, real_T c11_Mref, real_T c11_rx1, real_T c11_rx2, real_T
  c11_ry1, real_T c11_ry2, real_T c11_C[7]);
static const mxArray *c11_h_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData);
static int32_T c11_i_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId);
static void c11_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData);
static uint8_T c11_j_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_b_is_active_c11_BoatLibraryTest04, const
  char_T *c11_identifier);
static uint8_T c11_k_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId);
static void c11_b_sind(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T *c11_x);
static void c11_b_cosd(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T *c11_x);
static void init_dsm_address_info(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance)
{
  chartInstance->c11_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c11_is_active_c11_BoatLibraryTest04 = 0U;
}

static void initialize_params_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance)
{
}

static void enable_c11_BoatLibraryTest04(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c11_BoatLibraryTest04(SFc11_BoatLibraryTest04InstanceStruct *
  chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c11_update_debugger_state_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance)
{
}

static const mxArray *get_sim_state_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance)
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
  c11_hoistedGlobal = chartInstance->c11_is_active_c11_BoatLibraryTest04;
  c11_b_u = c11_hoistedGlobal;
  c11_c_y = NULL;
  sf_mex_assign(&c11_c_y, sf_mex_create("y", &c11_b_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c11_y, 1, c11_c_y);
  sf_mex_assign(&c11_st, c11_y, FALSE);
  return c11_st;
}

static void set_sim_state_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance, const mxArray *c11_st)
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

  chartInstance->c11_is_active_c11_BoatLibraryTest04 = c11_j_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c11_u, 1)),
     "is_active_c11_BoatLibraryTest04");
  sf_mex_destroy(&c11_u);
  c11_update_debugger_state_c11_BoatLibraryTest04(chartInstance);
  sf_mex_destroy(&c11_st);
}

static void finalize_c11_BoatLibraryTest04(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance)
{
}

static void sf_c11_BoatLibraryTest04(SFc11_BoatLibraryTest04InstanceStruct
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
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 2U, chartInstance->c11_sfEvent);
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
  c11_chartstep_c11_BoatLibraryTest04(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_BoatLibraryTest04MachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c11_chartstep_c11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance)
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
  uint32_T c11_debug_family_var_map[24];
  real_T c11_rx1;
  real_T c11_rx2;
  real_T c11_ry1;
  real_T c11_ry2;
  real_T c11_C[7];
  real_T c11_Fxref;
  real_T c11_Fyref;
  real_T c11_Mref;
  real_T c11_gridNum;
  real_T c11_grid[92];
  real_T c11_u1[4];
  real_T c11_nargin = 10.0;
  real_T c11_nargout = 1.0;
  real_T c11_output[4];
  int32_T c11_i9;
  int32_T c11_i10;
  real_T c11_b_inputBound[8];
  real_T c11_b_rx1;
  real_T c11_b_ry1;
  uint32_T c11_b_debug_family_var_map[13];
  real_T c11_p;
  real_T c11_theta1[20];
  real_T c11_f1[20];
  real_T c11_theta2[20];
  real_T c11_f2[20];
  real_T c11_i;
  real_T c11_b_gridNum;
  real_T c11_b_nargin = 4.0;
  real_T c11_b_nargout = 1.0;
  real_T c11_dv1[20];
  int32_T c11_i11;
  int32_T c11_i12;
  int32_T c11_i13;
  int32_T c11_i14;
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
  int32_T c11_i15;
  int32_T c11_i16;
  int32_T c11_i17;
  int32_T c11_i18;
  int32_T c11_i19;
  int32_T c11_i20;
  static real_T c11_dv2[4] = { 0.0, 1.0, 0.0, 1.0 };

  int32_T c11_i21;
  real_T c11_b_grid[92];
  int32_T c11_i22;
  real_T c11_preinput[4];
  real_T c11_b_Fxref;
  real_T c11_b_Fyref;
  real_T c11_b_Mref;
  real_T c11_c_rx1;
  real_T c11_b_rx2;
  real_T c11_c_ry1;
  real_T c11_b_ry2;
  int32_T c11_i23;
  real_T c11_b_C[7];
  uint32_T c11_c_debug_family_var_map[22];
  real_T c11_ii;
  real_T c11_iii;
  real_T c11_v0;
  real_T c11_b_i;
  real_T c11_j;
  real_T c11_k;
  real_T c11_l;
  real_T c11_input[4];
  real_T c11_v1;
  real_T c11_c_nargin = 10.0;
  real_T c11_c_nargout = 1.0;
  int32_T c11_i24;
  int32_T c11_i25;
  real_T c11_c_grid[4];
  int32_T c11_i26;
  real_T c11_b_preinput[4];
  int32_T c11_i27;
  real_T c11_c_C[7];
  int32_T c11_c_i;
  int32_T c11_b_j;
  int32_T c11_b_k;
  int32_T c11_b_l;
  int32_T c11_i28;
  real_T c11_b_input[4];
  int32_T c11_i29;
  real_T c11_c_preinput[4];
  int32_T c11_i30;
  real_T c11_d_C[7];
  int32_T c11_i31;
  int32_T c11_i32;
  int32_T c11_i33;
  real_T *c11_b_M;
  real_T *c11_b_Fy;
  real_T *c11_b_Fx;
  real_T *c11_b_ry02;
  real_T *c11_b_ry01;
  real_T *c11_b_rx02;
  real_T *c11_b_rx01;
  real_T (*c11_b_output)[4];
  real_T (*c11_c_inputBound)[8];
  real_T (*c11_b_C0)[7];
  real_T (*c11_b_u0)[4];
  c11_b_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c11_b_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c11_b_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c11_c_inputBound = (real_T (*)[8])ssGetInputPortSignal(chartInstance->S, 6);
  c11_b_C0 = (real_T (*)[7])ssGetInputPortSignal(chartInstance->S, 5);
  c11_b_ry02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c11_b_ry01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c11_b_rx02 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c11_b_rx01 = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c11_b_output = (real_T (*)[4])ssGetOutputPortSignal(chartInstance->S, 1);
  c11_b_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 2U, chartInstance->c11_sfEvent);
  c11_hoistedGlobal = *c11_b_rx01;
  c11_b_hoistedGlobal = *c11_b_rx02;
  c11_c_hoistedGlobal = *c11_b_ry01;
  c11_d_hoistedGlobal = *c11_b_ry02;
  c11_e_hoistedGlobal = *c11_b_Fx;
  c11_f_hoistedGlobal = *c11_b_Fy;
  c11_g_hoistedGlobal = *c11_b_M;
  for (c11_i6 = 0; c11_i6 < 4; c11_i6++) {
    c11_u0[c11_i6] = (*c11_b_u0)[c11_i6];
  }

  c11_rx01 = c11_hoistedGlobal;
  c11_rx02 = c11_b_hoistedGlobal;
  c11_ry01 = c11_c_hoistedGlobal;
  c11_ry02 = c11_d_hoistedGlobal;
  for (c11_i7 = 0; c11_i7 < 7; c11_i7++) {
    c11_C0[c11_i7] = (*c11_b_C0)[c11_i7];
  }

  for (c11_i8 = 0; c11_i8 < 8; c11_i8++) {
    c11_inputBound[c11_i8] = (*c11_c_inputBound)[c11_i8];
  }

  c11_Fx = c11_e_hoistedGlobal;
  c11_Fy = c11_f_hoistedGlobal;
  c11_M = c11_g_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 24U, 24U, c11_debug_family_names,
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
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargin, 11U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_nargout, 12U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c11_u0, 13U, c11_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_rx01, 14U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_rx02, 15U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_ry01, 16U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_ry02, 17U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c11_C0, 18U, c11_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c11_inputBound, 19U, c11_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_Fx, 20U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_Fy, 21U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_M, 22U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_output, 23U, c11_sf_marshallOut,
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
  c11_gridNum = 20.0;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 12);
  for (c11_i10 = 0; c11_i10 < 8; c11_i10++) {
    c11_b_inputBound[c11_i10] = c11_inputBound[c11_i10];
  }

  c11_b_rx1 = c11_rx1;
  c11_b_ry1 = c11_ry1;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 13U, 13U, c11_b_debug_family_names,
    c11_b_debug_family_var_map);
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
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_b_gridNum, 6U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_nargin, 7U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_nargout, 8U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_b_inputBound, 9U,
    c11_c_sf_marshallOut, c11_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_rx1, 10U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_ry1, 11U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_grid, 12U, c11_e_sf_marshallOut,
    c11_c_sf_marshallIn);
  c11_b_gridNum = 20.0;
  CV_EML_FCN(0, 3);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 64);
  c11_p = 20.0;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 66);
  c11_linspace(chartInstance, c11_b_inputBound[0], c11_b_inputBound[4], c11_dv1);
  for (c11_i11 = 0; c11_i11 < 20; c11_i11++) {
    c11_theta1[c11_i11] = c11_dv1[c11_i11];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 67);
  c11_linspace(chartInstance, c11_b_inputBound[1], c11_b_inputBound[5], c11_dv1);
  for (c11_i12 = 0; c11_i12 < 20; c11_i12++) {
    c11_f1[c11_i12] = c11_dv1[c11_i12];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 68);
  c11_linspace(chartInstance, c11_b_inputBound[2], c11_b_inputBound[6], c11_dv1);
  for (c11_i13 = 0; c11_i13 < 20; c11_i13++) {
    c11_theta2[c11_i13] = c11_dv1[c11_i13];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 69);
  c11_linspace(chartInstance, c11_b_inputBound[3], c11_b_inputBound[7], c11_dv1);
  for (c11_i14 = 0; c11_i14 < 20; c11_i14++) {
    c11_f2[c11_i14] = c11_dv1[c11_i14];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 71);
  c11_A = c11_b_rx1;
  c11_B = c11_b_ry1;
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
  for (c11_i15 = 0; c11_i15 < 20; c11_i15++) {
    c11_grid[c11_i15] = c11_theta1[c11_i15];
  }

  for (c11_i16 = 0; c11_i16 < 20; c11_i16++) {
    c11_grid[c11_i16 + 23] = c11_f1[c11_i16];
  }

  for (c11_i17 = 0; c11_i17 < 20; c11_i17++) {
    c11_grid[c11_i17 + 46] = c11_theta2[c11_i17];
  }

  for (c11_i18 = 0; c11_i18 < 20; c11_i18++) {
    c11_grid[c11_i18 + 69] = c11_f2[c11_i18];
  }

  c11_i19 = 0;
  for (c11_i20 = 0; c11_i20 < 4; c11_i20++) {
    c11_grid[c11_i19 + 20] = c11_dv2[c11_i20];
    c11_i19 += 23;
  }

  c11_grid[21] = -c11_i;
  c11_grid[44] = -1.0;
  c11_grid[67] = -c11_i;
  c11_grid[90] = -1.0;
  c11_grid[22] = c11_i;
  c11_grid[45] = 0.0;
  c11_grid[68] = c11_i;
  c11_grid[91] = 0.0;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -73);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 14);
  for (c11_i21 = 0; c11_i21 < 92; c11_i21++) {
    c11_b_grid[c11_i21] = c11_grid[c11_i21];
  }

  for (c11_i22 = 0; c11_i22 < 4; c11_i22++) {
    c11_preinput[c11_i22] = c11_u0[c11_i22];
  }

  c11_b_Fxref = c11_Fxref;
  c11_b_Fyref = c11_Fyref;
  c11_b_Mref = c11_Mref;
  c11_c_rx1 = c11_rx1;
  c11_b_rx2 = c11_rx2;
  c11_c_ry1 = c11_ry1;
  c11_b_ry2 = c11_ry2;
  for (c11_i23 = 0; c11_i23 < 7; c11_i23++) {
    c11_b_C[c11_i23] = c11_C[c11_i23];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 22U, 22U, c11_e_debug_family_names,
    c11_c_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c11_ii, 0U, c11_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_iii, 1U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_v0, 2U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_i, 3U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_j, 4U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_k, 5U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_l, 6U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_input, 7U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_v1, 8U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_nargin, 9U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_nargout, 10U, c11_b_sf_marshallOut,
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
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_rx1, 16U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_rx2, 17U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_c_ry1, 18U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c11_b_ry2, 19U, c11_b_sf_marshallOut,
    c11_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_b_C, 20U, c11_d_sf_marshallOut,
    c11_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c11_u1, 21U, c11_sf_marshallOut,
    c11_sf_marshallIn);
  CV_EML_FCN(0, 1);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 31);
  c11_ii = 23.0;
  c11_iii = 4.0;
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 32);
  c11_i24 = 0;
  for (c11_i25 = 0; c11_i25 < 4; c11_i25++) {
    c11_c_grid[c11_i25] = c11_b_grid[c11_i24];
    c11_i24 += 23;
  }

  for (c11_i26 = 0; c11_i26 < 4; c11_i26++) {
    c11_b_preinput[c11_i26] = c11_preinput[c11_i26];
  }

  for (c11_i27 = 0; c11_i27 < 7; c11_i27++) {
    c11_c_C[c11_i27] = c11_b_C[c11_i27];
  }

  c11_v0 = c11_costFunction(chartInstance, c11_c_grid, c11_b_preinput,
    c11_b_Fxref, c11_b_Fyref, c11_b_Mref, c11_c_rx1, c11_b_rx2, c11_c_ry1,
    c11_b_ry2, c11_c_C);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 34);
  c11_u1[0] = c11_b_grid[0];
  c11_u1[1] = c11_b_grid[23];
  c11_u1[2] = c11_b_grid[46];
  c11_u1[3] = c11_b_grid[69];
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 36);
  c11_b_i = 1.0;
  c11_c_i = 0;
  while (c11_c_i < 23) {
    c11_b_i = 1.0 + (real_T)c11_c_i;
    CV_EML_FOR(0, 1, 0, 1);
    _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 37);
    c11_j = 1.0;
    c11_b_j = 0;
    while (c11_b_j < 23) {
      c11_j = 1.0 + (real_T)c11_b_j;
      CV_EML_FOR(0, 1, 1, 1);
      _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 38);
      c11_k = 1.0;
      c11_b_k = 0;
      while (c11_b_k < 23) {
        c11_k = 1.0 + (real_T)c11_b_k;
        CV_EML_FOR(0, 1, 2, 1);
        _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 39);
        c11_l = 1.0;
        c11_b_l = 0;
        while (c11_b_l < 23) {
          c11_l = 1.0 + (real_T)c11_b_l;
          CV_EML_FOR(0, 1, 3, 1);
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 40);
          c11_input[0] = c11_b_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK
            ("grid", (int32_T)_SFD_INTEGER_CHECK("i", c11_b_i), 1, 23, 1, 0) - 1];
          c11_input[1] = c11_b_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK
            ("grid", (int32_T)_SFD_INTEGER_CHECK("j", c11_j), 1, 23, 1, 0) + 22];
          c11_input[2] = c11_b_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK
            ("grid", (int32_T)_SFD_INTEGER_CHECK("k", c11_k), 1, 23, 1, 0) + 45];
          c11_input[3] = c11_b_grid[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK
            ("grid", (int32_T)_SFD_INTEGER_CHECK("l", c11_l), 1, 23, 1, 0) + 68];
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 41);
          for (c11_i28 = 0; c11_i28 < 4; c11_i28++) {
            c11_b_input[c11_i28] = c11_input[c11_i28];
          }

          for (c11_i29 = 0; c11_i29 < 4; c11_i29++) {
            c11_c_preinput[c11_i29] = c11_preinput[c11_i29];
          }

          for (c11_i30 = 0; c11_i30 < 7; c11_i30++) {
            c11_d_C[c11_i30] = c11_b_C[c11_i30];
          }

          c11_v1 = c11_b_costFunction(chartInstance, c11_b_input, c11_c_preinput,
            c11_b_Fxref, c11_b_Fyref, c11_b_Mref, c11_c_rx1, c11_b_rx2,
            c11_c_ry1, c11_b_ry2, c11_d_C);
          _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 42);
          if (CV_EML_IF(0, 1, 0, c11_v0 > c11_v1)) {
            _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 43);
            c11_v0 = c11_v1;
            _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 44);
            for (c11_i31 = 0; c11_i31 < 4; c11_i31++) {
              c11_u1[c11_i31] = c11_input[c11_i31];
            }
          }

          c11_b_l++;
          _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
        }

        CV_EML_FOR(0, 1, 3, 0);
        c11_b_k++;
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      }

      CV_EML_FOR(0, 1, 2, 0);
      c11_b_j++;
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }

    CV_EML_FOR(0, 1, 1, 0);
    c11_c_i++;
    _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
  }

  CV_EML_FOR(0, 1, 0, 0);
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -44);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, 25);
  for (c11_i32 = 0; c11_i32 < 4; c11_i32++) {
    c11_output[c11_i32] = c11_u1[c11_i32];
  }

  _SFD_EML_CALL(0U, chartInstance->c11_sfEvent, -25);
  _SFD_SYMBOL_SCOPE_POP();
  for (c11_i33 = 0; c11_i33 < 4; c11_i33++) {
    (*c11_b_output)[c11_i33] = c11_output[c11_i33];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 2U, chartInstance->c11_sfEvent);
}

static void initSimStructsc11_BoatLibraryTest04
  (SFc11_BoatLibraryTest04InstanceStruct *chartInstance)
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
  int32_T c11_i34;
  real_T c11_b_inData[4];
  int32_T c11_i35;
  real_T c11_u[4];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i34 = 0; c11_i34 < 4; c11_i34++) {
    c11_b_inData[c11_i34] = (*(real_T (*)[4])c11_inData)[c11_i34];
  }

  for (c11_i35 = 0; c11_i35 < 4; c11_i35++) {
    c11_u[c11_i35] = c11_b_inData[c11_i35];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
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

static void c11_b_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[4])
{
  real_T c11_dv3[4];
  int32_T c11_i36;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv3, 1, 0, 0U, 1, 0U, 1, 4);
  for (c11_i36 = 0; c11_i36 < 4; c11_i36++) {
    c11_y[c11_i36] = c11_dv3[c11_i36];
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
  int32_T c11_i37;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_output = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_output), &c11_thisId,
    c11_y);
  sf_mex_destroy(&c11_output);
  for (c11_i37 = 0; c11_i37 < 4; c11_i37++) {
    (*(real_T (*)[4])c11_outData)[c11_i37] = c11_y[c11_i37];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static const mxArray *c11_b_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  real_T c11_u;
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
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
  int32_T c11_i38;
  int32_T c11_i39;
  int32_T c11_i40;
  real_T c11_b_inData[8];
  int32_T c11_i41;
  int32_T c11_i42;
  int32_T c11_i43;
  real_T c11_u[8];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  c11_i38 = 0;
  for (c11_i39 = 0; c11_i39 < 2; c11_i39++) {
    for (c11_i40 = 0; c11_i40 < 4; c11_i40++) {
      c11_b_inData[c11_i40 + c11_i38] = (*(real_T (*)[8])c11_inData)[c11_i40 +
        c11_i38];
    }

    c11_i38 += 4;
  }

  c11_i41 = 0;
  for (c11_i42 = 0; c11_i42 < 2; c11_i42++) {
    for (c11_i43 = 0; c11_i43 < 4; c11_i43++) {
      c11_u[c11_i43 + c11_i41] = c11_b_inData[c11_i43 + c11_i41];
    }

    c11_i41 += 4;
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
  int32_T c11_i44;
  real_T c11_b_inData[7];
  int32_T c11_i45;
  real_T c11_u[7];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i44 = 0; c11_i44 < 7; c11_i44++) {
    c11_b_inData[c11_i44] = (*(real_T (*)[7])c11_inData)[c11_i44];
  }

  for (c11_i45 = 0; c11_i45 < 7; c11_i45++) {
    c11_u[c11_i45] = c11_b_inData[c11_i45];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 1, 7), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static real_T c11_c_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
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
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
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
  int32_T c11_i46;
  int32_T c11_i47;
  int32_T c11_i48;
  real_T c11_b_inData[92];
  int32_T c11_i49;
  int32_T c11_i50;
  int32_T c11_i51;
  real_T c11_u[92];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  c11_i46 = 0;
  for (c11_i47 = 0; c11_i47 < 4; c11_i47++) {
    for (c11_i48 = 0; c11_i48 < 23; c11_i48++) {
      c11_b_inData[c11_i48 + c11_i46] = (*(real_T (*)[92])c11_inData)[c11_i48 +
        c11_i46];
    }

    c11_i46 += 23;
  }

  c11_i49 = 0;
  for (c11_i50 = 0; c11_i50 < 4; c11_i50++) {
    for (c11_i51 = 0; c11_i51 < 23; c11_i51++) {
      c11_u[c11_i51 + c11_i49] = c11_b_inData[c11_i51 + c11_i49];
    }

    c11_i49 += 23;
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 2, 23, 4),
                FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_d_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[92])
{
  real_T c11_dv4[92];
  int32_T c11_i52;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv4, 1, 0, 0U, 1, 0U, 2, 23,
                4);
  for (c11_i52 = 0; c11_i52 < 92; c11_i52++) {
    c11_y[c11_i52] = c11_dv4[c11_i52];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_grid;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[92];
  int32_T c11_i53;
  int32_T c11_i54;
  int32_T c11_i55;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_grid = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_grid), &c11_thisId, c11_y);
  sf_mex_destroy(&c11_grid);
  c11_i53 = 0;
  for (c11_i54 = 0; c11_i54 < 4; c11_i54++) {
    for (c11_i55 = 0; c11_i55 < 23; c11_i55++) {
      (*(real_T (*)[92])c11_outData)[c11_i55 + c11_i53] = c11_y[c11_i55 +
        c11_i53];
    }

    c11_i53 += 23;
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static void c11_e_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[7])
{
  real_T c11_dv5[7];
  int32_T c11_i56;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv5, 1, 0, 0U, 1, 0U, 1, 7);
  for (c11_i56 = 0; c11_i56 < 7; c11_i56++) {
    c11_y[c11_i56] = c11_dv5[c11_i56];
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
  int32_T c11_i57;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_C = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_C), &c11_thisId, c11_y);
  sf_mex_destroy(&c11_C);
  for (c11_i57 = 0; c11_i57 < 7; c11_i57++) {
    (*(real_T (*)[7])c11_outData)[c11_i57] = c11_y[c11_i57];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static void c11_f_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[8])
{
  real_T c11_dv6[8];
  int32_T c11_i58;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv6, 1, 0, 0U, 1, 0U, 2, 4,
                2);
  for (c11_i58 = 0; c11_i58 < 8; c11_i58++) {
    c11_y[c11_i58] = c11_dv6[c11_i58];
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
  int32_T c11_i59;
  int32_T c11_i60;
  int32_T c11_i61;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_inputBound = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_inputBound), &c11_thisId,
    c11_y);
  sf_mex_destroy(&c11_inputBound);
  c11_i59 = 0;
  for (c11_i60 = 0; c11_i60 < 2; c11_i60++) {
    for (c11_i61 = 0; c11_i61 < 4; c11_i61++) {
      (*(real_T (*)[8])c11_outData)[c11_i61 + c11_i59] = c11_y[c11_i61 + c11_i59];
    }

    c11_i59 += 4;
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static const mxArray *c11_f_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i62;
  real_T c11_b_inData[20];
  int32_T c11_i63;
  real_T c11_u[20];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i62 = 0; c11_i62 < 20; c11_i62++) {
    c11_b_inData[c11_i62] = (*(real_T (*)[20])c11_inData)[c11_i62];
  }

  for (c11_i63 = 0; c11_i63 < 20; c11_i63++) {
    c11_u[c11_i63] = c11_b_inData[c11_i63];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 1, 20), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_g_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[20])
{
  real_T c11_dv7[20];
  int32_T c11_i64;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv7, 1, 0, 0U, 1, 0U, 1, 20);
  for (c11_i64 = 0; c11_i64 < 20; c11_i64++) {
    c11_y[c11_i64] = c11_dv7[c11_i64];
  }

  sf_mex_destroy(&c11_u);
}

static void c11_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_f2;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  real_T c11_y[20];
  int32_T c11_i65;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_f2 = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_f2), &c11_thisId, c11_y);
  sf_mex_destroy(&c11_f2);
  for (c11_i65 = 0; c11_i65 < 20; c11_i65++) {
    (*(real_T (*)[20])c11_outData)[c11_i65] = c11_y[c11_i65];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

static const mxArray *c11_g_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_i66;
  real_T c11_b_inData[4];
  int32_T c11_i67;
  real_T c11_u[4];
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  for (c11_i66 = 0; c11_i66 < 4; c11_i66++) {
    c11_b_inData[c11_i66] = (*(real_T (*)[4])c11_inData)[c11_i66];
  }

  for (c11_i67 = 0; c11_i67 < 4; c11_i67++) {
    c11_u[c11_i67] = c11_b_inData[c11_i67];
  }

  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", c11_u, 0, 0U, 1U, 0U, 2, 1, 4), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static void c11_h_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId,
  real_T c11_y[4])
{
  real_T c11_dv8[4];
  int32_T c11_i68;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), c11_dv8, 1, 0, 0U, 1, 0U, 2, 1,
                4);
  for (c11_i68 = 0; c11_i68 < 4; c11_i68++) {
    c11_y[c11_i68] = c11_dv8[c11_i68];
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
  int32_T c11_i69;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_input = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_input), &c11_thisId,
    c11_y);
  sf_mex_destroy(&c11_input);
  for (c11_i69 = 0; c11_i69 < 4; c11_i69++) {
    (*(real_T (*)[4])c11_outData)[c11_i69] = c11_y[c11_i69];
  }

  sf_mex_destroy(&c11_mxArrayInData);
}

const mxArray *sf_c11_BoatLibraryTest04_get_eml_resolved_functions_info(void)
{
  const mxArray *c11_nameCaptureInfo = NULL;
  c11_nameCaptureInfo = NULL;
  sf_mex_assign(&c11_nameCaptureInfo, sf_mex_createstruct("structure", 2, 54, 1),
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

static void c11_eml_scalar_eg(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance)
{
}

static void c11_linspace(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c11_d1, real_T c11_d2, real_T c11_y[20])
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
  c11_y[19] = c11_d2;
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
      c11_delta1 = c11_f_x / 19.0;
      for (c11_c_k = 0; c11_c_k < 18; c11_c_k++) {
        c11_b_k = 1.0 + (real_T)c11_c_k;
        c11_c_a = c11_b_k;
        c11_c_b = c11_delta1;
        c11_d_y = c11_c_a * c11_c_b;
        c11_y[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
          _SFD_INTEGER_CHECK("", c11_b_k + 1.0), 1, 20, 1, 0) - 1] = c11_d1 +
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
    c11_delta1 = c11_b_x / 19.0;
    c11_b_A = c11_d2;
    c11_c_x = c11_b_A;
    c11_d_x = c11_c_x;
    c11_delta2 = c11_d_x / 19.0;
    for (c11_k = 0; c11_k < 18; c11_k++) {
      c11_b_k = 1.0 + (real_T)c11_k;
      c11_a = c11_delta2;
      c11_b = c11_b_k;
      c11_b_y = c11_a * c11_b;
      c11_b_a = c11_delta1;
      c11_b_b = c11_b_k;
      c11_c_y = c11_b_a * c11_b_b;
      c11_y[(int32_T)(real_T)_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
        _SFD_INTEGER_CHECK("", c11_b_k + 1.0), 1, 20, 1, 0) - 1] = (c11_d1 +
        c11_b_y) - c11_c_y;
    }
  }
}

static real_T c11_costFunction(SFc11_BoatLibraryTest04InstanceStruct
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

static real_T c11_sind(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c11_x)
{
  real_T c11_b_x;
  c11_b_x = c11_x;
  c11_b_sind(chartInstance, &c11_b_x);
  return c11_b_x;
}

static boolean_T c11_isfinite(SFc11_BoatLibraryTest04InstanceStruct
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

static real_T c11_cosd(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c11_x)
{
  real_T c11_b_x;
  c11_b_x = c11_x;
  c11_b_cosd(chartInstance, &c11_b_x);
  return c11_b_x;
}

static real_T c11_mpower(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
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

static real_T c11_b_costFunction(SFc11_BoatLibraryTest04InstanceStruct
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

static const mxArray *c11_h_sf_marshallOut(void *chartInstanceVoid, void
  *c11_inData)
{
  const mxArray *c11_mxArrayOutData = NULL;
  int32_T c11_u;
  const mxArray *c11_y = NULL;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_mxArrayOutData = NULL;
  c11_u = *(int32_T *)c11_inData;
  c11_y = NULL;
  sf_mex_assign(&c11_y, sf_mex_create("y", &c11_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c11_mxArrayOutData, c11_y, FALSE);
  return c11_mxArrayOutData;
}

static int32_T c11_i_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId)
{
  int32_T c11_y;
  int32_T c11_i70;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), &c11_i70, 1, 6, 0U, 0, 0U, 0);
  c11_y = c11_i70;
  sf_mex_destroy(&c11_u);
  return c11_y;
}

static void c11_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c11_mxArrayInData, const char_T *c11_varName, void *c11_outData)
{
  const mxArray *c11_b_sfEvent;
  const char_T *c11_identifier;
  emlrtMsgIdentifier c11_thisId;
  int32_T c11_y;
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c11_b_sfEvent = sf_mex_dup(c11_mxArrayInData);
  c11_identifier = c11_varName;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_y = c11_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c11_b_sfEvent),
    &c11_thisId);
  sf_mex_destroy(&c11_b_sfEvent);
  *(int32_T *)c11_outData = c11_y;
  sf_mex_destroy(&c11_mxArrayInData);
}

static uint8_T c11_j_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_b_is_active_c11_BoatLibraryTest04, const
  char_T *c11_identifier)
{
  uint8_T c11_y;
  emlrtMsgIdentifier c11_thisId;
  c11_thisId.fIdentifier = c11_identifier;
  c11_thisId.fParent = NULL;
  c11_y = c11_k_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c11_b_is_active_c11_BoatLibraryTest04), &c11_thisId);
  sf_mex_destroy(&c11_b_is_active_c11_BoatLibraryTest04);
  return c11_y;
}

static uint8_T c11_k_emlrt_marshallIn(SFc11_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c11_u, const emlrtMsgIdentifier *c11_parentId)
{
  uint8_T c11_y;
  uint8_T c11_u0;
  sf_mex_import(c11_parentId, sf_mex_dup(c11_u), &c11_u0, 1, 3, 0U, 0, 0U, 0);
  c11_y = c11_u0;
  sf_mex_destroy(&c11_u);
  return c11_y;
}

static void c11_b_sind(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
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

static void c11_b_cosd(SFc11_BoatLibraryTest04InstanceStruct *chartInstance,
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

static void init_dsm_address_info(SFc11_BoatLibraryTest04InstanceStruct
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

void sf_c11_BoatLibraryTest04_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2841926842U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2885759174U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(187546689U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(4071360159U);
}

mxArray *sf_c11_BoatLibraryTest04_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("VltSs9SWyaAs2NH2EbKZ7E");
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

mxArray *sf_c11_BoatLibraryTest04_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c11_BoatLibraryTest04_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c11_BoatLibraryTest04(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"output\",},{M[8],M[0],T\"is_active_c11_BoatLibraryTest04\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c11_BoatLibraryTest04_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
    chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _BoatLibraryTest04MachineNumber_,
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
          init_script_number_translation(_BoatLibraryTest04MachineNumber_,
            chartInstance->chartNumber);
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_BoatLibraryTest04MachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _BoatLibraryTest04MachineNumber_,
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
        _SFD_CV_INIT_EML(0,1,9,3,0,0,0,6,2,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,636);
        _SFD_CV_INIT_EML_FCN(0,1,"getMinFromGrid",638,-1,1306);
        _SFD_CV_INIT_EML_FCN(0,2,"checkInputBound",1308,-1,1476);
        _SFD_CV_INIT_EML_FCN(0,3,"boundGrid",1478,-1,1958);
        _SFD_CV_INIT_EML_FCN(0,4,"sat",1960,-1,2148);
        _SFD_CV_INIT_EML_FCN(0,5,"getStepSize",2150,-1,2948);
        _SFD_CV_INIT_EML_FCN(0,6,"costFunction",2950,-1,3687);
        _SFD_CV_INIT_EML_FCN(0,7,"getDirection",3689,-1,3957);
        _SFD_CV_INIT_EML_FCN(0,8,"SteepestDescent",3959,-1,5579);
        _SFD_CV_INIT_EML_IF(0,1,0,1145,1155,1237,1261);
        _SFD_CV_INIT_EML_IF(0,1,1,2001,2020,2052,2143);
        _SFD_CV_INIT_EML_IF(0,1,2,2052,2075,2107,2143);
        _SFD_CV_INIT_EML_FOR(0,1,0,892,905,1301);
        _SFD_CV_INIT_EML_FOR(0,1,1,909,922,1297);
        _SFD_CV_INIT_EML_FOR(0,1,2,930,943,1289);
        _SFD_CV_INIT_EML_FOR(0,1,3,955,968,1277);
        _SFD_CV_INIT_EML_FOR(0,1,4,1379,1403,1456);
        _SFD_CV_INIT_EML_FOR(0,1,5,3891,3913,3948);
        _SFD_CV_INIT_EML_WHILE(0,1,0,2483,2509,2679);
        _SFD_CV_INIT_EML_WHILE(0,1,1,2722,2749,2921);

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
        _BoatLibraryTest04MachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "iGSdlSQay9NUAjqz2dp4qH";
}

static void sf_opaque_initialize_c11_BoatLibraryTest04(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc11_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c11_BoatLibraryTest04((SFc11_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
  initialize_c11_BoatLibraryTest04((SFc11_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c11_BoatLibraryTest04(void *chartInstanceVar)
{
  enable_c11_BoatLibraryTest04((SFc11_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c11_BoatLibraryTest04(void *chartInstanceVar)
{
  disable_c11_BoatLibraryTest04((SFc11_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c11_BoatLibraryTest04(void *chartInstanceVar)
{
  sf_c11_BoatLibraryTest04((SFc11_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c11_BoatLibraryTest04(SimStruct*
  S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c11_BoatLibraryTest04
    ((SFc11_BoatLibraryTest04InstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c11_BoatLibraryTest04();/* state var info */
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

extern void sf_internal_set_sim_state_c11_BoatLibraryTest04(SimStruct* S, const
  mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c11_BoatLibraryTest04();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c11_BoatLibraryTest04((SFc11_BoatLibraryTest04InstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c11_BoatLibraryTest04(SimStruct* S)
{
  return sf_internal_get_sim_state_c11_BoatLibraryTest04(S);
}

static void sf_opaque_set_sim_state_c11_BoatLibraryTest04(SimStruct* S, const
  mxArray *st)
{
  sf_internal_set_sim_state_c11_BoatLibraryTest04(S, st);
}

static void sf_opaque_terminate_c11_BoatLibraryTest04(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc11_BoatLibraryTest04InstanceStruct*) chartInstanceVar)
      ->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_BoatLibraryTest04_optimization_info();
    }

    finalize_c11_BoatLibraryTest04((SFc11_BoatLibraryTest04InstanceStruct*)
      chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc11_BoatLibraryTest04((SFc11_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c11_BoatLibraryTest04(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c11_BoatLibraryTest04
      ((SFc11_BoatLibraryTest04InstanceStruct*)(((ChartInfoStruct *)
         ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c11_BoatLibraryTest04(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_BoatLibraryTest04_optimization_info();
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
  ssSetChecksum0(S,(3671493274U));
  ssSetChecksum1(S,(2661008587U));
  ssSetChecksum2(S,(1195076801U));
  ssSetChecksum3(S,(3758367808U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c11_BoatLibraryTest04(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c11_BoatLibraryTest04(SimStruct *S)
{
  SFc11_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc11_BoatLibraryTest04InstanceStruct *)utMalloc(sizeof
    (SFc11_BoatLibraryTest04InstanceStruct));
  memset(chartInstance, 0, sizeof(SFc11_BoatLibraryTest04InstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c11_BoatLibraryTest04;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c11_BoatLibraryTest04;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c11_BoatLibraryTest04;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c11_BoatLibraryTest04;
  chartInstance->chartInfo.disableChart =
    sf_opaque_disable_c11_BoatLibraryTest04;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c11_BoatLibraryTest04;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c11_BoatLibraryTest04;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c11_BoatLibraryTest04;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c11_BoatLibraryTest04;
  chartInstance->chartInfo.mdlStart = mdlStart_c11_BoatLibraryTest04;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c11_BoatLibraryTest04;
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

void c11_BoatLibraryTest04_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c11_BoatLibraryTest04(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c11_BoatLibraryTest04(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c11_BoatLibraryTest04(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c11_BoatLibraryTest04_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
