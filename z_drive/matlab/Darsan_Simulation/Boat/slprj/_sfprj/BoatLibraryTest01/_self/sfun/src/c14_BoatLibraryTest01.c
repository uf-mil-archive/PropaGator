/* Include files */

#include <stddef.h>
#include "blas.h"
#include "BoatLibraryTest01_sfun.h"
#include "c14_BoatLibraryTest01.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "BoatLibraryTest01_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c14_debug_family_names[19] = { "x1", "x2", "x3", "x4", "x5",
  "x6", "nargin", "nargout", "C", "M", "I", "fw", "kr", "x0", "fx", "fy", "m",
  "x", "y" };

/* Function Declarations */
static void initialize_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance);
static void initialize_params_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance);
static void enable_c14_BoatLibraryTest01(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance);
static void disable_c14_BoatLibraryTest01(SFc14_BoatLibraryTest01InstanceStruct *
  chartInstance);
static void c14_update_debugger_state_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance);
static const mxArray *get_sim_state_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance);
static void set_sim_state_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance, const mxArray *c14_st);
static void finalize_c14_BoatLibraryTest01(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance);
static void sf_c14_BoatLibraryTest01(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance);
static void c14_chartstep_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance);
static void initSimStructsc14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c14_machineNumber, uint32_T
  c14_chartNumber);
static const mxArray *c14_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData);
static void c14_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_y, const char_T *c14_identifier, real_T
  c14_b_y[3]);
static void c14_b_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId,
  real_T c14_y[3]);
static void c14_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c14_mxArrayInData, const char_T *c14_varName, void *c14_outData);
static const mxArray *c14_b_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData);
static void c14_c_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_x, const char_T *c14_identifier, real_T
  c14_y[6]);
static void c14_d_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId,
  real_T c14_y[6]);
static void c14_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c14_mxArrayInData, const char_T *c14_varName, void *c14_outData);
static const mxArray *c14_c_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData);
static const mxArray *c14_d_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData);
static real_T c14_e_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId);
static void c14_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c14_mxArrayInData, const char_T *c14_varName, void *c14_outData);
static void c14_info_helper(const mxArray **c14_info);
static const mxArray *c14_emlrt_marshallOut(char * c14_u);
static const mxArray *c14_b_emlrt_marshallOut(uint32_T c14_u);
static void c14_eml_scalar_eg(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance);
static const mxArray *c14_e_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData);
static int32_T c14_f_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId);
static void c14_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c14_mxArrayInData, const char_T *c14_varName, void *c14_outData);
static uint8_T c14_g_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_b_is_active_c14_BoatLibraryTest01, const
  char_T *c14_identifier);
static uint8_T c14_h_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId);
static void init_dsm_address_info(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance)
{
  chartInstance->c14_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c14_is_active_c14_BoatLibraryTest01 = 0U;
}

static void initialize_params_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance)
{
}

static void enable_c14_BoatLibraryTest01(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c14_BoatLibraryTest01(SFc14_BoatLibraryTest01InstanceStruct *
  chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c14_update_debugger_state_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance)
{
}

static const mxArray *get_sim_state_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance)
{
  const mxArray *c14_st;
  const mxArray *c14_y = NULL;
  int32_T c14_i0;
  real_T c14_u[6];
  const mxArray *c14_b_y = NULL;
  int32_T c14_i1;
  real_T c14_b_u[3];
  const mxArray *c14_c_y = NULL;
  uint8_T c14_hoistedGlobal;
  uint8_T c14_c_u;
  const mxArray *c14_d_y = NULL;
  real_T (*c14_e_y)[3];
  real_T (*c14_x)[6];
  c14_e_y = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c14_x = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  c14_st = NULL;
  c14_st = NULL;
  c14_y = NULL;
  sf_mex_assign(&c14_y, sf_mex_createcellarray(3), FALSE);
  for (c14_i0 = 0; c14_i0 < 6; c14_i0++) {
    c14_u[c14_i0] = (*c14_x)[c14_i0];
  }

  c14_b_y = NULL;
  sf_mex_assign(&c14_b_y, sf_mex_create("y", c14_u, 0, 0U, 1U, 0U, 1, 6), FALSE);
  sf_mex_setcell(c14_y, 0, c14_b_y);
  for (c14_i1 = 0; c14_i1 < 3; c14_i1++) {
    c14_b_u[c14_i1] = (*c14_e_y)[c14_i1];
  }

  c14_c_y = NULL;
  sf_mex_assign(&c14_c_y, sf_mex_create("y", c14_b_u, 0, 0U, 1U, 0U, 1, 3),
                FALSE);
  sf_mex_setcell(c14_y, 1, c14_c_y);
  c14_hoistedGlobal = chartInstance->c14_is_active_c14_BoatLibraryTest01;
  c14_c_u = c14_hoistedGlobal;
  c14_d_y = NULL;
  sf_mex_assign(&c14_d_y, sf_mex_create("y", &c14_c_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c14_y, 2, c14_d_y);
  sf_mex_assign(&c14_st, c14_y, FALSE);
  return c14_st;
}

static void set_sim_state_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance, const mxArray *c14_st)
{
  const mxArray *c14_u;
  real_T c14_dv0[6];
  int32_T c14_i2;
  real_T c14_dv1[3];
  int32_T c14_i3;
  real_T (*c14_x)[6];
  real_T (*c14_y)[3];
  c14_y = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c14_x = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c14_doneDoubleBufferReInit = TRUE;
  c14_u = sf_mex_dup(c14_st);
  c14_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c14_u, 0)),
    "x", c14_dv0);
  for (c14_i2 = 0; c14_i2 < 6; c14_i2++) {
    (*c14_x)[c14_i2] = c14_dv0[c14_i2];
  }

  c14_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c14_u, 1)), "y",
                       c14_dv1);
  for (c14_i3 = 0; c14_i3 < 3; c14_i3++) {
    (*c14_y)[c14_i3] = c14_dv1[c14_i3];
  }

  chartInstance->c14_is_active_c14_BoatLibraryTest01 = c14_g_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c14_u, 2)),
     "is_active_c14_BoatLibraryTest01");
  sf_mex_destroy(&c14_u);
  c14_update_debugger_state_c14_BoatLibraryTest01(chartInstance);
  sf_mex_destroy(&c14_st);
}

static void finalize_c14_BoatLibraryTest01(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
}

static void sf_c14_BoatLibraryTest01(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
  int32_T c14_i4;
  int32_T c14_i5;
  int32_T c14_i6;
  int32_T c14_i7;
  real_T *c14_M;
  real_T *c14_I;
  real_T *c14_fw;
  real_T *c14_kr;
  real_T *c14_fx;
  real_T *c14_fy;
  real_T *c14_m;
  real_T (*c14_y)[3];
  real_T (*c14_x0)[6];
  real_T (*c14_x)[6];
  real_T (*c14_C)[18];
  c14_m = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c14_fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c14_y = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c14_fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
  c14_x0 = (real_T (*)[6])ssGetInputPortSignal(chartInstance->S, 5);
  c14_kr = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c14_fw = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c14_I = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c14_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c14_x = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  c14_C = (real_T (*)[18])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 13U, chartInstance->c14_sfEvent);
  for (c14_i4 = 0; c14_i4 < 18; c14_i4++) {
    _SFD_DATA_RANGE_CHECK((*c14_C)[c14_i4], 0U);
  }

  for (c14_i5 = 0; c14_i5 < 6; c14_i5++) {
    _SFD_DATA_RANGE_CHECK((*c14_x)[c14_i5], 1U);
  }

  _SFD_DATA_RANGE_CHECK(*c14_M, 2U);
  _SFD_DATA_RANGE_CHECK(*c14_I, 3U);
  _SFD_DATA_RANGE_CHECK(*c14_fw, 4U);
  _SFD_DATA_RANGE_CHECK(*c14_kr, 5U);
  for (c14_i6 = 0; c14_i6 < 6; c14_i6++) {
    _SFD_DATA_RANGE_CHECK((*c14_x0)[c14_i6], 6U);
  }

  _SFD_DATA_RANGE_CHECK(*c14_fx, 7U);
  for (c14_i7 = 0; c14_i7 < 3; c14_i7++) {
    _SFD_DATA_RANGE_CHECK((*c14_y)[c14_i7], 8U);
  }

  _SFD_DATA_RANGE_CHECK(*c14_fy, 9U);
  _SFD_DATA_RANGE_CHECK(*c14_m, 10U);
  chartInstance->c14_sfEvent = CALL_EVENT;
  c14_chartstep_c14_BoatLibraryTest01(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_BoatLibraryTest01MachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c14_chartstep_c14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance)
{
  real_T c14_hoistedGlobal;
  real_T c14_b_hoistedGlobal;
  real_T c14_c_hoistedGlobal;
  real_T c14_d_hoistedGlobal;
  real_T c14_e_hoistedGlobal;
  real_T c14_f_hoistedGlobal;
  real_T c14_g_hoistedGlobal;
  int32_T c14_i8;
  real_T c14_C[18];
  real_T c14_M;
  real_T c14_I;
  real_T c14_fw;
  real_T c14_kr;
  int32_T c14_i9;
  real_T c14_x0[6];
  real_T c14_fx;
  real_T c14_fy;
  real_T c14_m;
  uint32_T c14_debug_family_var_map[19];
  real_T c14_x1;
  real_T c14_x2;
  real_T c14_x3;
  real_T c14_x4;
  real_T c14_x5;
  real_T c14_x6;
  real_T c14_nargin = 9.0;
  real_T c14_nargout = 2.0;
  real_T c14_x[6];
  real_T c14_y[3];
  real_T c14_A;
  real_T c14_B;
  real_T c14_b_x;
  real_T c14_b_y;
  real_T c14_c_x;
  real_T c14_c_y;
  real_T c14_d_y;
  real_T c14_b;
  real_T c14_e_y;
  real_T c14_a;
  real_T c14_b_b;
  real_T c14_f_y;
  real_T c14_b_A;
  real_T c14_b_B;
  real_T c14_d_x;
  real_T c14_g_y;
  real_T c14_e_x;
  real_T c14_h_y;
  real_T c14_i_y;
  real_T c14_c_b;
  real_T c14_j_y;
  real_T c14_b_a;
  real_T c14_d_b;
  real_T c14_k_y;
  real_T c14_c_A;
  real_T c14_c_B;
  real_T c14_f_x;
  real_T c14_l_y;
  real_T c14_g_x;
  real_T c14_m_y;
  real_T c14_n_y;
  real_T c14_e_b;
  real_T c14_o_y;
  real_T c14_c_a;
  real_T c14_f_b;
  real_T c14_p_y;
  int32_T c14_i10;
  real_T c14_d_a[18];
  int32_T c14_i11;
  real_T c14_g_b[6];
  int32_T c14_i12;
  int32_T c14_i13;
  int32_T c14_i14;
  real_T c14_b_C[3];
  int32_T c14_i15;
  int32_T c14_i16;
  int32_T c14_i17;
  int32_T c14_i18;
  int32_T c14_i19;
  int32_T c14_i20;
  int32_T c14_i21;
  int32_T c14_i22;
  real_T *c14_b_M;
  real_T *c14_b_I;
  real_T *c14_b_fw;
  real_T *c14_b_kr;
  real_T *c14_b_fx;
  real_T *c14_b_fy;
  real_T *c14_b_m;
  real_T (*c14_h_x)[6];
  real_T (*c14_q_y)[3];
  real_T (*c14_b_x0)[6];
  real_T (*c14_c_C)[18];
  c14_b_m = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c14_b_fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c14_q_y = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
  c14_b_fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
  c14_b_x0 = (real_T (*)[6])ssGetInputPortSignal(chartInstance->S, 5);
  c14_b_kr = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c14_b_fw = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c14_b_I = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c14_b_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c14_h_x = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  c14_c_C = (real_T (*)[18])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 13U, chartInstance->c14_sfEvent);
  c14_hoistedGlobal = *c14_b_M;
  c14_b_hoistedGlobal = *c14_b_I;
  c14_c_hoistedGlobal = *c14_b_fw;
  c14_d_hoistedGlobal = *c14_b_kr;
  c14_e_hoistedGlobal = *c14_b_fx;
  c14_f_hoistedGlobal = *c14_b_fy;
  c14_g_hoistedGlobal = *c14_b_m;
  for (c14_i8 = 0; c14_i8 < 18; c14_i8++) {
    c14_C[c14_i8] = (*c14_c_C)[c14_i8];
  }

  c14_M = c14_hoistedGlobal;
  c14_I = c14_b_hoistedGlobal;
  c14_fw = c14_c_hoistedGlobal;
  c14_kr = c14_d_hoistedGlobal;
  for (c14_i9 = 0; c14_i9 < 6; c14_i9++) {
    c14_x0[c14_i9] = (*c14_b_x0)[c14_i9];
  }

  c14_fx = c14_e_hoistedGlobal;
  c14_fy = c14_f_hoistedGlobal;
  c14_m = c14_g_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 19U, 19U, c14_debug_family_names,
    c14_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c14_x1, 0U, c14_c_sf_marshallOut,
    c14_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c14_x2, 1U, c14_c_sf_marshallOut,
    c14_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c14_x3, 2U, c14_c_sf_marshallOut,
    c14_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c14_x4, 3U, c14_c_sf_marshallOut,
    c14_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c14_x5, 4U, c14_c_sf_marshallOut,
    c14_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c14_x6, 5U, c14_c_sf_marshallOut,
    c14_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c14_nargin, 6U, c14_c_sf_marshallOut,
    c14_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c14_nargout, 7U, c14_c_sf_marshallOut,
    c14_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c14_C, 8U, c14_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c14_M, 9U, c14_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c14_I, 10U, c14_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c14_fw, 11U, c14_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c14_kr, 12U, c14_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c14_x0, 13U, c14_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c14_fx, 14U, c14_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c14_fy, 15U, c14_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c14_m, 16U, c14_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c14_x, 17U, c14_b_sf_marshallOut,
    c14_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c14_y, 18U, c14_sf_marshallOut,
    c14_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, 7);
  c14_x1 = c14_x0[1];
  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, 8);
  c14_A = c14_fx;
  c14_B = c14_M;
  c14_b_x = c14_A;
  c14_b_y = c14_B;
  c14_c_x = c14_b_x;
  c14_c_y = c14_b_y;
  c14_d_y = c14_c_x / c14_c_y;
  c14_b = c14_x0[1];
  c14_e_y = 0.0 * c14_b;
  c14_a = c14_e_y;
  c14_b_b = c14_fw;
  c14_f_y = c14_a * c14_b_b;
  c14_x2 = c14_d_y - c14_f_y;
  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, 9);
  c14_x3 = c14_x0[3];
  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, 10);
  c14_b_A = c14_fy;
  c14_b_B = c14_M;
  c14_d_x = c14_b_A;
  c14_g_y = c14_b_B;
  c14_e_x = c14_d_x;
  c14_h_y = c14_g_y;
  c14_i_y = c14_e_x / c14_h_y;
  c14_c_b = c14_x0[3];
  c14_j_y = 0.0 * c14_c_b;
  c14_b_a = c14_j_y;
  c14_d_b = c14_fw;
  c14_k_y = c14_b_a * c14_d_b;
  c14_x4 = c14_i_y - c14_k_y;
  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, 11);
  c14_x5 = c14_x0[5];
  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, 12);
  c14_c_A = c14_m;
  c14_c_B = c14_I;
  c14_f_x = c14_c_A;
  c14_l_y = c14_c_B;
  c14_g_x = c14_f_x;
  c14_m_y = c14_l_y;
  c14_n_y = c14_g_x / c14_m_y;
  c14_e_b = c14_x0[5];
  c14_o_y = 0.0 * c14_e_b;
  c14_c_a = c14_o_y;
  c14_f_b = c14_kr;
  c14_p_y = c14_c_a * c14_f_b;
  c14_x6 = c14_n_y - c14_p_y;
  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, 14);
  c14_x[0] = c14_x1;
  c14_x[1] = c14_x2;
  c14_x[2] = c14_x3;
  c14_x[3] = c14_x4;
  c14_x[4] = c14_x5;
  c14_x[5] = c14_x6;
  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, 16);
  for (c14_i10 = 0; c14_i10 < 18; c14_i10++) {
    c14_d_a[c14_i10] = c14_C[c14_i10];
  }

  for (c14_i11 = 0; c14_i11 < 6; c14_i11++) {
    c14_g_b[c14_i11] = c14_x0[c14_i11];
  }

  c14_eml_scalar_eg(chartInstance);
  c14_eml_scalar_eg(chartInstance);
  for (c14_i12 = 0; c14_i12 < 3; c14_i12++) {
    c14_y[c14_i12] = 0.0;
  }

  for (c14_i13 = 0; c14_i13 < 3; c14_i13++) {
    c14_y[c14_i13] = 0.0;
  }

  for (c14_i14 = 0; c14_i14 < 3; c14_i14++) {
    c14_b_C[c14_i14] = c14_y[c14_i14];
  }

  for (c14_i15 = 0; c14_i15 < 3; c14_i15++) {
    c14_y[c14_i15] = c14_b_C[c14_i15];
  }

  for (c14_i16 = 0; c14_i16 < 3; c14_i16++) {
    c14_b_C[c14_i16] = c14_y[c14_i16];
  }

  for (c14_i17 = 0; c14_i17 < 3; c14_i17++) {
    c14_y[c14_i17] = c14_b_C[c14_i17];
  }

  for (c14_i18 = 0; c14_i18 < 3; c14_i18++) {
    c14_y[c14_i18] = 0.0;
    c14_i19 = 0;
    for (c14_i20 = 0; c14_i20 < 6; c14_i20++) {
      c14_y[c14_i18] += c14_d_a[c14_i19 + c14_i18] * c14_g_b[c14_i20];
      c14_i19 += 3;
    }
  }

  _SFD_EML_CALL(0U, chartInstance->c14_sfEvent, -16);
  _SFD_SYMBOL_SCOPE_POP();
  for (c14_i21 = 0; c14_i21 < 6; c14_i21++) {
    (*c14_h_x)[c14_i21] = c14_x[c14_i21];
  }

  for (c14_i22 = 0; c14_i22 < 3; c14_i22++) {
    (*c14_q_y)[c14_i22] = c14_y[c14_i22];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 13U, chartInstance->c14_sfEvent);
}

static void initSimStructsc14_BoatLibraryTest01
  (SFc14_BoatLibraryTest01InstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c14_machineNumber, uint32_T
  c14_chartNumber)
{
}

static const mxArray *c14_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData)
{
  const mxArray *c14_mxArrayOutData = NULL;
  int32_T c14_i23;
  real_T c14_b_inData[3];
  int32_T c14_i24;
  real_T c14_u[3];
  const mxArray *c14_y = NULL;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_mxArrayOutData = NULL;
  for (c14_i23 = 0; c14_i23 < 3; c14_i23++) {
    c14_b_inData[c14_i23] = (*(real_T (*)[3])c14_inData)[c14_i23];
  }

  for (c14_i24 = 0; c14_i24 < 3; c14_i24++) {
    c14_u[c14_i24] = c14_b_inData[c14_i24];
  }

  c14_y = NULL;
  sf_mex_assign(&c14_y, sf_mex_create("y", c14_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_assign(&c14_mxArrayOutData, c14_y, FALSE);
  return c14_mxArrayOutData;
}

static void c14_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_y, const char_T *c14_identifier, real_T
  c14_b_y[3])
{
  emlrtMsgIdentifier c14_thisId;
  c14_thisId.fIdentifier = c14_identifier;
  c14_thisId.fParent = NULL;
  c14_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c14_y), &c14_thisId, c14_b_y);
  sf_mex_destroy(&c14_y);
}

static void c14_b_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId,
  real_T c14_y[3])
{
  real_T c14_dv2[3];
  int32_T c14_i25;
  sf_mex_import(c14_parentId, sf_mex_dup(c14_u), c14_dv2, 1, 0, 0U, 1, 0U, 1, 3);
  for (c14_i25 = 0; c14_i25 < 3; c14_i25++) {
    c14_y[c14_i25] = c14_dv2[c14_i25];
  }

  sf_mex_destroy(&c14_u);
}

static void c14_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c14_mxArrayInData, const char_T *c14_varName, void *c14_outData)
{
  const mxArray *c14_y;
  const char_T *c14_identifier;
  emlrtMsgIdentifier c14_thisId;
  real_T c14_b_y[3];
  int32_T c14_i26;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_y = sf_mex_dup(c14_mxArrayInData);
  c14_identifier = c14_varName;
  c14_thisId.fIdentifier = c14_identifier;
  c14_thisId.fParent = NULL;
  c14_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c14_y), &c14_thisId, c14_b_y);
  sf_mex_destroy(&c14_y);
  for (c14_i26 = 0; c14_i26 < 3; c14_i26++) {
    (*(real_T (*)[3])c14_outData)[c14_i26] = c14_b_y[c14_i26];
  }

  sf_mex_destroy(&c14_mxArrayInData);
}

static const mxArray *c14_b_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData)
{
  const mxArray *c14_mxArrayOutData = NULL;
  int32_T c14_i27;
  real_T c14_b_inData[6];
  int32_T c14_i28;
  real_T c14_u[6];
  const mxArray *c14_y = NULL;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_mxArrayOutData = NULL;
  for (c14_i27 = 0; c14_i27 < 6; c14_i27++) {
    c14_b_inData[c14_i27] = (*(real_T (*)[6])c14_inData)[c14_i27];
  }

  for (c14_i28 = 0; c14_i28 < 6; c14_i28++) {
    c14_u[c14_i28] = c14_b_inData[c14_i28];
  }

  c14_y = NULL;
  sf_mex_assign(&c14_y, sf_mex_create("y", c14_u, 0, 0U, 1U, 0U, 1, 6), FALSE);
  sf_mex_assign(&c14_mxArrayOutData, c14_y, FALSE);
  return c14_mxArrayOutData;
}

static void c14_c_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_x, const char_T *c14_identifier, real_T
  c14_y[6])
{
  emlrtMsgIdentifier c14_thisId;
  c14_thisId.fIdentifier = c14_identifier;
  c14_thisId.fParent = NULL;
  c14_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c14_x), &c14_thisId, c14_y);
  sf_mex_destroy(&c14_x);
}

static void c14_d_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId,
  real_T c14_y[6])
{
  real_T c14_dv3[6];
  int32_T c14_i29;
  sf_mex_import(c14_parentId, sf_mex_dup(c14_u), c14_dv3, 1, 0, 0U, 1, 0U, 1, 6);
  for (c14_i29 = 0; c14_i29 < 6; c14_i29++) {
    c14_y[c14_i29] = c14_dv3[c14_i29];
  }

  sf_mex_destroy(&c14_u);
}

static void c14_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c14_mxArrayInData, const char_T *c14_varName, void *c14_outData)
{
  const mxArray *c14_x;
  const char_T *c14_identifier;
  emlrtMsgIdentifier c14_thisId;
  real_T c14_y[6];
  int32_T c14_i30;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_x = sf_mex_dup(c14_mxArrayInData);
  c14_identifier = c14_varName;
  c14_thisId.fIdentifier = c14_identifier;
  c14_thisId.fParent = NULL;
  c14_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c14_x), &c14_thisId, c14_y);
  sf_mex_destroy(&c14_x);
  for (c14_i30 = 0; c14_i30 < 6; c14_i30++) {
    (*(real_T (*)[6])c14_outData)[c14_i30] = c14_y[c14_i30];
  }

  sf_mex_destroy(&c14_mxArrayInData);
}

static const mxArray *c14_c_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData)
{
  const mxArray *c14_mxArrayOutData = NULL;
  real_T c14_u;
  const mxArray *c14_y = NULL;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_mxArrayOutData = NULL;
  c14_u = *(real_T *)c14_inData;
  c14_y = NULL;
  sf_mex_assign(&c14_y, sf_mex_create("y", &c14_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c14_mxArrayOutData, c14_y, FALSE);
  return c14_mxArrayOutData;
}

static const mxArray *c14_d_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData)
{
  const mxArray *c14_mxArrayOutData = NULL;
  int32_T c14_i31;
  int32_T c14_i32;
  int32_T c14_i33;
  real_T c14_b_inData[18];
  int32_T c14_i34;
  int32_T c14_i35;
  int32_T c14_i36;
  real_T c14_u[18];
  const mxArray *c14_y = NULL;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_mxArrayOutData = NULL;
  c14_i31 = 0;
  for (c14_i32 = 0; c14_i32 < 6; c14_i32++) {
    for (c14_i33 = 0; c14_i33 < 3; c14_i33++) {
      c14_b_inData[c14_i33 + c14_i31] = (*(real_T (*)[18])c14_inData)[c14_i33 +
        c14_i31];
    }

    c14_i31 += 3;
  }

  c14_i34 = 0;
  for (c14_i35 = 0; c14_i35 < 6; c14_i35++) {
    for (c14_i36 = 0; c14_i36 < 3; c14_i36++) {
      c14_u[c14_i36 + c14_i34] = c14_b_inData[c14_i36 + c14_i34];
    }

    c14_i34 += 3;
  }

  c14_y = NULL;
  sf_mex_assign(&c14_y, sf_mex_create("y", c14_u, 0, 0U, 1U, 0U, 2, 3, 6), FALSE);
  sf_mex_assign(&c14_mxArrayOutData, c14_y, FALSE);
  return c14_mxArrayOutData;
}

static real_T c14_e_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId)
{
  real_T c14_y;
  real_T c14_d0;
  sf_mex_import(c14_parentId, sf_mex_dup(c14_u), &c14_d0, 1, 0, 0U, 0, 0U, 0);
  c14_y = c14_d0;
  sf_mex_destroy(&c14_u);
  return c14_y;
}

static void c14_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c14_mxArrayInData, const char_T *c14_varName, void *c14_outData)
{
  const mxArray *c14_nargout;
  const char_T *c14_identifier;
  emlrtMsgIdentifier c14_thisId;
  real_T c14_y;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_nargout = sf_mex_dup(c14_mxArrayInData);
  c14_identifier = c14_varName;
  c14_thisId.fIdentifier = c14_identifier;
  c14_thisId.fParent = NULL;
  c14_y = c14_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c14_nargout),
    &c14_thisId);
  sf_mex_destroy(&c14_nargout);
  *(real_T *)c14_outData = c14_y;
  sf_mex_destroy(&c14_mxArrayInData);
}

const mxArray *sf_c14_BoatLibraryTest01_get_eml_resolved_functions_info(void)
{
  const mxArray *c14_nameCaptureInfo = NULL;
  c14_nameCaptureInfo = NULL;
  sf_mex_assign(&c14_nameCaptureInfo, sf_mex_createstruct("structure", 2, 15, 1),
                FALSE);
  c14_info_helper(&c14_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c14_nameCaptureInfo);
  return c14_nameCaptureInfo;
}

static void c14_info_helper(const mxArray **c14_info)
{
  const mxArray *c14_rhs0 = NULL;
  const mxArray *c14_lhs0 = NULL;
  const mxArray *c14_rhs1 = NULL;
  const mxArray *c14_lhs1 = NULL;
  const mxArray *c14_rhs2 = NULL;
  const mxArray *c14_lhs2 = NULL;
  const mxArray *c14_rhs3 = NULL;
  const mxArray *c14_lhs3 = NULL;
  const mxArray *c14_rhs4 = NULL;
  const mxArray *c14_lhs4 = NULL;
  const mxArray *c14_rhs5 = NULL;
  const mxArray *c14_lhs5 = NULL;
  const mxArray *c14_rhs6 = NULL;
  const mxArray *c14_lhs6 = NULL;
  const mxArray *c14_rhs7 = NULL;
  const mxArray *c14_lhs7 = NULL;
  const mxArray *c14_rhs8 = NULL;
  const mxArray *c14_lhs8 = NULL;
  const mxArray *c14_rhs9 = NULL;
  const mxArray *c14_lhs9 = NULL;
  const mxArray *c14_rhs10 = NULL;
  const mxArray *c14_lhs10 = NULL;
  const mxArray *c14_rhs11 = NULL;
  const mxArray *c14_lhs11 = NULL;
  const mxArray *c14_rhs12 = NULL;
  const mxArray *c14_lhs12 = NULL;
  const mxArray *c14_rhs13 = NULL;
  const mxArray *c14_lhs13 = NULL;
  const mxArray *c14_rhs14 = NULL;
  const mxArray *c14_lhs14 = NULL;
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("mrdivide"), "name", "name",
                  0);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 0);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c14_rhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs0), "rhs", "rhs",
                  0);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs0), "lhs", "lhs",
                  0);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "context",
                  "context", 1);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("rdivide"), "name", "name", 1);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "resolved",
                  "resolved", 1);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c14_rhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs1), "rhs", "rhs",
                  1);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs1), "lhs", "lhs",
                  1);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 2);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 2);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 2);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c14_rhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs2), "rhs", "rhs",
                  2);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs2), "lhs", "lhs",
                  2);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 3);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_scalexp_compatible"),
                  "name", "name", 3);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m"),
                  "resolved", "resolved", 3);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c14_rhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs3), "rhs", "rhs",
                  3);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs3), "lhs", "lhs",
                  3);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 4);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_div"), "name", "name", 4);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m"), "resolved",
                  "resolved", 4);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1363731866U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c14_rhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs4), "rhs", "rhs",
                  4);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs4), "lhs", "lhs",
                  4);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(""), "context", "context", 5);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("mtimes"), "name", "name", 5);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 5);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c14_rhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs5), "rhs", "rhs",
                  5);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs5), "lhs", "lhs",
                  5);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m!common_checks"),
                  "context", "context", 6);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 6);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 6);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c14_rhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs6), "rhs", "rhs",
                  6);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs6), "lhs", "lhs",
                  6);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "context",
                  "context", 7);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 7);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 7);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1323192178U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c14_rhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs7), "rhs", "rhs",
                  7);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs7), "lhs", "lhs",
                  7);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "context",
                  "context", 8);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 8);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 8);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c14_rhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs8), "rhs", "rhs",
                  8);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs8), "lhs", "lhs",
                  8);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "context",
                  "context", 9);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_xgemm"), "name", "name",
                  9);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m"),
                  "resolved", "resolved", 9);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1363731870U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c14_rhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs9), "rhs", "rhs",
                  9);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs9), "lhs", "lhs",
                  9);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m"), "context",
                  "context", 10);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_blas_inline"), "name",
                  "name", 10);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m"),
                  "resolved", "resolved", 10);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1299098368U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c14_rhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m!below_threshold"),
                  "context", "context", 11);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("mtimes"), "name", "name", 11);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 11);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c14_rhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m"),
                  "context", "context", 12);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 12);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 12);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1323192178U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c14_rhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m"),
                  "context", "context", 13);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 13);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 13);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c14_rhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m"),
                  "context", "context", 14);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("eml_refblas_xgemm"), "name",
                  "name", 14);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c14_info, c14_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgemm.m"),
                  "resolved", "resolved", 14);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(1360303950U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c14_info, c14_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c14_rhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c14_lhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c14_info, sf_mex_duplicatearraysafe(&c14_lhs14), "lhs", "lhs",
                  14);
  sf_mex_destroy(&c14_rhs0);
  sf_mex_destroy(&c14_lhs0);
  sf_mex_destroy(&c14_rhs1);
  sf_mex_destroy(&c14_lhs1);
  sf_mex_destroy(&c14_rhs2);
  sf_mex_destroy(&c14_lhs2);
  sf_mex_destroy(&c14_rhs3);
  sf_mex_destroy(&c14_lhs3);
  sf_mex_destroy(&c14_rhs4);
  sf_mex_destroy(&c14_lhs4);
  sf_mex_destroy(&c14_rhs5);
  sf_mex_destroy(&c14_lhs5);
  sf_mex_destroy(&c14_rhs6);
  sf_mex_destroy(&c14_lhs6);
  sf_mex_destroy(&c14_rhs7);
  sf_mex_destroy(&c14_lhs7);
  sf_mex_destroy(&c14_rhs8);
  sf_mex_destroy(&c14_lhs8);
  sf_mex_destroy(&c14_rhs9);
  sf_mex_destroy(&c14_lhs9);
  sf_mex_destroy(&c14_rhs10);
  sf_mex_destroy(&c14_lhs10);
  sf_mex_destroy(&c14_rhs11);
  sf_mex_destroy(&c14_lhs11);
  sf_mex_destroy(&c14_rhs12);
  sf_mex_destroy(&c14_lhs12);
  sf_mex_destroy(&c14_rhs13);
  sf_mex_destroy(&c14_lhs13);
  sf_mex_destroy(&c14_rhs14);
  sf_mex_destroy(&c14_lhs14);
}

static const mxArray *c14_emlrt_marshallOut(char * c14_u)
{
  const mxArray *c14_y = NULL;
  c14_y = NULL;
  sf_mex_assign(&c14_y, sf_mex_create("y", c14_u, 15, 0U, 0U, 0U, 2, 1, strlen
    (c14_u)), FALSE);
  return c14_y;
}

static const mxArray *c14_b_emlrt_marshallOut(uint32_T c14_u)
{
  const mxArray *c14_y = NULL;
  c14_y = NULL;
  sf_mex_assign(&c14_y, sf_mex_create("y", &c14_u, 7, 0U, 0U, 0U, 0), FALSE);
  return c14_y;
}

static void c14_eml_scalar_eg(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance)
{
}

static const mxArray *c14_e_sf_marshallOut(void *chartInstanceVoid, void
  *c14_inData)
{
  const mxArray *c14_mxArrayOutData = NULL;
  int32_T c14_u;
  const mxArray *c14_y = NULL;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_mxArrayOutData = NULL;
  c14_u = *(int32_T *)c14_inData;
  c14_y = NULL;
  sf_mex_assign(&c14_y, sf_mex_create("y", &c14_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c14_mxArrayOutData, c14_y, FALSE);
  return c14_mxArrayOutData;
}

static int32_T c14_f_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId)
{
  int32_T c14_y;
  int32_T c14_i37;
  sf_mex_import(c14_parentId, sf_mex_dup(c14_u), &c14_i37, 1, 6, 0U, 0, 0U, 0);
  c14_y = c14_i37;
  sf_mex_destroy(&c14_u);
  return c14_y;
}

static void c14_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c14_mxArrayInData, const char_T *c14_varName, void *c14_outData)
{
  const mxArray *c14_b_sfEvent;
  const char_T *c14_identifier;
  emlrtMsgIdentifier c14_thisId;
  int32_T c14_y;
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)chartInstanceVoid;
  c14_b_sfEvent = sf_mex_dup(c14_mxArrayInData);
  c14_identifier = c14_varName;
  c14_thisId.fIdentifier = c14_identifier;
  c14_thisId.fParent = NULL;
  c14_y = c14_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c14_b_sfEvent),
    &c14_thisId);
  sf_mex_destroy(&c14_b_sfEvent);
  *(int32_T *)c14_outData = c14_y;
  sf_mex_destroy(&c14_mxArrayInData);
}

static uint8_T c14_g_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_b_is_active_c14_BoatLibraryTest01, const
  char_T *c14_identifier)
{
  uint8_T c14_y;
  emlrtMsgIdentifier c14_thisId;
  c14_thisId.fIdentifier = c14_identifier;
  c14_thisId.fParent = NULL;
  c14_y = c14_h_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c14_b_is_active_c14_BoatLibraryTest01), &c14_thisId);
  sf_mex_destroy(&c14_b_is_active_c14_BoatLibraryTest01);
  return c14_y;
}

static uint8_T c14_h_emlrt_marshallIn(SFc14_BoatLibraryTest01InstanceStruct
  *chartInstance, const mxArray *c14_u, const emlrtMsgIdentifier *c14_parentId)
{
  uint8_T c14_y;
  uint8_T c14_u0;
  sf_mex_import(c14_parentId, sf_mex_dup(c14_u), &c14_u0, 1, 3, 0U, 0, 0U, 0);
  c14_y = c14_u0;
  sf_mex_destroy(&c14_u);
  return c14_y;
}

static void init_dsm_address_info(SFc14_BoatLibraryTest01InstanceStruct
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

void sf_c14_BoatLibraryTest01_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2197998461U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1056374901U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(399292863U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2320211648U);
}

mxArray *sf_c14_BoatLibraryTest01_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("zSSYjOUfiu58QeUOxgkVr");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,9,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      pr[1] = (double)(6);
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
      pr[0] = (double)(6);
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
      pr[0] = (double)(1);
      pr[1] = (double)(1);
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
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,2,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(6);
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
      pr[0] = (double)(3);
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
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c14_BoatLibraryTest01_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c14_BoatLibraryTest01_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c14_BoatLibraryTest01(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x3'type','srcId','name','auxInfo'{{M[1],M[11],T\"x\",},{M[1],M[5],T\"y\",},{M[8],M[0],T\"is_active_c14_BoatLibraryTest01\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 3, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c14_BoatLibraryTest01_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
    chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _BoatLibraryTest01MachineNumber_,
           14,
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
          _SFD_SET_DATA_PROPS(0,1,1,0,"C");
          _SFD_SET_DATA_PROPS(1,2,0,1,"x");
          _SFD_SET_DATA_PROPS(2,1,1,0,"M");
          _SFD_SET_DATA_PROPS(3,1,1,0,"I");
          _SFD_SET_DATA_PROPS(4,1,1,0,"fw");
          _SFD_SET_DATA_PROPS(5,1,1,0,"kr");
          _SFD_SET_DATA_PROPS(6,1,1,0,"x0");
          _SFD_SET_DATA_PROPS(7,1,1,0,"fx");
          _SFD_SET_DATA_PROPS(8,2,0,1,"y");
          _SFD_SET_DATA_PROPS(9,1,1,0,"fy");
          _SFD_SET_DATA_PROPS(10,1,1,0,"m");
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
        _SFD_CV_INIT_EML(0,1,1,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,380);

        {
          unsigned int dimVector[2];
          dimVector[0]= 3;
          dimVector[1]= 6;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c14_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 6;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c14_b_sf_marshallOut,(MexInFcnForType)
            c14_b_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c14_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c14_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c14_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c14_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 6;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c14_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c14_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 3;
          _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c14_sf_marshallOut,(MexInFcnForType)
            c14_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c14_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c14_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          real_T *c14_M;
          real_T *c14_I;
          real_T *c14_fw;
          real_T *c14_kr;
          real_T *c14_fx;
          real_T *c14_fy;
          real_T *c14_m;
          real_T (*c14_C)[18];
          real_T (*c14_x)[6];
          real_T (*c14_x0)[6];
          real_T (*c14_y)[3];
          c14_m = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
          c14_fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
          c14_y = (real_T (*)[3])ssGetOutputPortSignal(chartInstance->S, 2);
          c14_fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
          c14_x0 = (real_T (*)[6])ssGetInputPortSignal(chartInstance->S, 5);
          c14_kr = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
          c14_fw = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c14_I = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c14_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c14_x = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
          c14_C = (real_T (*)[18])ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, *c14_C);
          _SFD_SET_DATA_VALUE_PTR(1U, *c14_x);
          _SFD_SET_DATA_VALUE_PTR(2U, c14_M);
          _SFD_SET_DATA_VALUE_PTR(3U, c14_I);
          _SFD_SET_DATA_VALUE_PTR(4U, c14_fw);
          _SFD_SET_DATA_VALUE_PTR(5U, c14_kr);
          _SFD_SET_DATA_VALUE_PTR(6U, *c14_x0);
          _SFD_SET_DATA_VALUE_PTR(7U, c14_fx);
          _SFD_SET_DATA_VALUE_PTR(8U, *c14_y);
          _SFD_SET_DATA_VALUE_PTR(9U, c14_fy);
          _SFD_SET_DATA_VALUE_PTR(10U, c14_m);
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
  return "55C1VMlnBmJLveWWYKMav";
}

static void sf_opaque_initialize_c14_BoatLibraryTest01(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc14_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c14_BoatLibraryTest01((SFc14_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
  initialize_c14_BoatLibraryTest01((SFc14_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c14_BoatLibraryTest01(void *chartInstanceVar)
{
  enable_c14_BoatLibraryTest01((SFc14_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c14_BoatLibraryTest01(void *chartInstanceVar)
{
  disable_c14_BoatLibraryTest01((SFc14_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c14_BoatLibraryTest01(void *chartInstanceVar)
{
  sf_c14_BoatLibraryTest01((SFc14_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c14_BoatLibraryTest01(SimStruct*
  S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c14_BoatLibraryTest01
    ((SFc14_BoatLibraryTest01InstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c14_BoatLibraryTest01();/* state var info */
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

extern void sf_internal_set_sim_state_c14_BoatLibraryTest01(SimStruct* S, const
  mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c14_BoatLibraryTest01();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c14_BoatLibraryTest01((SFc14_BoatLibraryTest01InstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c14_BoatLibraryTest01(SimStruct* S)
{
  return sf_internal_get_sim_state_c14_BoatLibraryTest01(S);
}

static void sf_opaque_set_sim_state_c14_BoatLibraryTest01(SimStruct* S, const
  mxArray *st)
{
  sf_internal_set_sim_state_c14_BoatLibraryTest01(S, st);
}

static void sf_opaque_terminate_c14_BoatLibraryTest01(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc14_BoatLibraryTest01InstanceStruct*) chartInstanceVar)
      ->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_BoatLibraryTest01_optimization_info();
    }

    finalize_c14_BoatLibraryTest01((SFc14_BoatLibraryTest01InstanceStruct*)
      chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc14_BoatLibraryTest01((SFc14_BoatLibraryTest01InstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c14_BoatLibraryTest01(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c14_BoatLibraryTest01
      ((SFc14_BoatLibraryTest01InstanceStruct*)(((ChartInfoStruct *)
         ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c14_BoatLibraryTest01(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_BoatLibraryTest01_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      14);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,14,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,14,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,14);
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
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,14,9);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,14,2);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=2; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 9; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,14);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2486783690U));
  ssSetChecksum1(S,(31396304U));
  ssSetChecksum2(S,(1681962898U));
  ssSetChecksum3(S,(3968379963U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c14_BoatLibraryTest01(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c14_BoatLibraryTest01(SimStruct *S)
{
  SFc14_BoatLibraryTest01InstanceStruct *chartInstance;
  chartInstance = (SFc14_BoatLibraryTest01InstanceStruct *)utMalloc(sizeof
    (SFc14_BoatLibraryTest01InstanceStruct));
  memset(chartInstance, 0, sizeof(SFc14_BoatLibraryTest01InstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c14_BoatLibraryTest01;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c14_BoatLibraryTest01;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c14_BoatLibraryTest01;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c14_BoatLibraryTest01;
  chartInstance->chartInfo.disableChart =
    sf_opaque_disable_c14_BoatLibraryTest01;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c14_BoatLibraryTest01;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c14_BoatLibraryTest01;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c14_BoatLibraryTest01;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c14_BoatLibraryTest01;
  chartInstance->chartInfo.mdlStart = mdlStart_c14_BoatLibraryTest01;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c14_BoatLibraryTest01;
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

void c14_BoatLibraryTest01_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c14_BoatLibraryTest01(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c14_BoatLibraryTest01(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c14_BoatLibraryTest01(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c14_BoatLibraryTest01_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
