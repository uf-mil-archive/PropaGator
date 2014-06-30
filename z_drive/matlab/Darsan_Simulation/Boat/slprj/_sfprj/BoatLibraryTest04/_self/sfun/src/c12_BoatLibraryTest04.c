/* Include files */

#include <stddef.h>
#include "blas.h"
#include "BoatLibraryTest04_sfun.h"
#include "c12_BoatLibraryTest04.h"
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
static const char * c12_debug_family_names[11] = { "R", "FxyM", "nargin",
  "nargout", "Fx", "Fy", "M", "phi", "fx", "fy", "m" };

/* Function Declarations */
static void initialize_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance);
static void initialize_params_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance);
static void enable_c12_BoatLibraryTest04(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance);
static void disable_c12_BoatLibraryTest04(SFc12_BoatLibraryTest04InstanceStruct *
  chartInstance);
static void c12_update_debugger_state_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance);
static const mxArray *get_sim_state_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance);
static void set_sim_state_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance, const mxArray *c12_st);
static void finalize_c12_BoatLibraryTest04(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance);
static void sf_c12_BoatLibraryTest04(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance);
static void c12_chartstep_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance);
static void initSimStructsc12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c12_machineNumber, uint32_T
  c12_chartNumber);
static const mxArray *c12_sf_marshallOut(void *chartInstanceVoid, void
  *c12_inData);
static real_T c12_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_m, const char_T *c12_identifier);
static real_T c12_b_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId);
static void c12_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c12_mxArrayInData, const char_T *c12_varName, void *c12_outData);
static const mxArray *c12_b_sf_marshallOut(void *chartInstanceVoid, void
  *c12_inData);
static void c12_c_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId,
  real_T c12_y[3]);
static void c12_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c12_mxArrayInData, const char_T *c12_varName, void *c12_outData);
static const mxArray *c12_c_sf_marshallOut(void *chartInstanceVoid, void
  *c12_inData);
static void c12_d_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId,
  real_T c12_y[9]);
static void c12_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c12_mxArrayInData, const char_T *c12_varName, void *c12_outData);
static void c12_info_helper(const mxArray **c12_info);
static const mxArray *c12_emlrt_marshallOut(char * c12_u);
static const mxArray *c12_b_emlrt_marshallOut(uint32_T c12_u);
static real_T c12_cosd(SFc12_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c12_x);
static boolean_T c12_isfinite(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, real_T c12_x);
static real_T c12_sind(SFc12_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c12_x);
static void c12_eml_scalar_eg(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance);
static const mxArray *c12_d_sf_marshallOut(void *chartInstanceVoid, void
  *c12_inData);
static int32_T c12_e_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId);
static void c12_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c12_mxArrayInData, const char_T *c12_varName, void *c12_outData);
static uint8_T c12_f_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_b_is_active_c12_BoatLibraryTest04, const
  char_T *c12_identifier);
static uint8_T c12_g_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId);
static void c12_b_cosd(SFc12_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T *c12_x);
static void c12_b_sind(SFc12_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T *c12_x);
static void init_dsm_address_info(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance)
{
  chartInstance->c12_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c12_is_active_c12_BoatLibraryTest04 = 0U;
}

static void initialize_params_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance)
{
}

static void enable_c12_BoatLibraryTest04(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c12_BoatLibraryTest04(SFc12_BoatLibraryTest04InstanceStruct *
  chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c12_update_debugger_state_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance)
{
}

static const mxArray *get_sim_state_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance)
{
  const mxArray *c12_st;
  const mxArray *c12_y = NULL;
  real_T c12_hoistedGlobal;
  real_T c12_u;
  const mxArray *c12_b_y = NULL;
  real_T c12_b_hoistedGlobal;
  real_T c12_b_u;
  const mxArray *c12_c_y = NULL;
  real_T c12_c_hoistedGlobal;
  real_T c12_c_u;
  const mxArray *c12_d_y = NULL;
  uint8_T c12_d_hoistedGlobal;
  uint8_T c12_d_u;
  const mxArray *c12_e_y = NULL;
  real_T *c12_fx;
  real_T *c12_fy;
  real_T *c12_m;
  c12_m = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c12_fy = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c12_fx = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c12_st = NULL;
  c12_st = NULL;
  c12_y = NULL;
  sf_mex_assign(&c12_y, sf_mex_createcellarray(4), FALSE);
  c12_hoistedGlobal = *c12_fx;
  c12_u = c12_hoistedGlobal;
  c12_b_y = NULL;
  sf_mex_assign(&c12_b_y, sf_mex_create("y", &c12_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c12_y, 0, c12_b_y);
  c12_b_hoistedGlobal = *c12_fy;
  c12_b_u = c12_b_hoistedGlobal;
  c12_c_y = NULL;
  sf_mex_assign(&c12_c_y, sf_mex_create("y", &c12_b_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c12_y, 1, c12_c_y);
  c12_c_hoistedGlobal = *c12_m;
  c12_c_u = c12_c_hoistedGlobal;
  c12_d_y = NULL;
  sf_mex_assign(&c12_d_y, sf_mex_create("y", &c12_c_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c12_y, 2, c12_d_y);
  c12_d_hoistedGlobal = chartInstance->c12_is_active_c12_BoatLibraryTest04;
  c12_d_u = c12_d_hoistedGlobal;
  c12_e_y = NULL;
  sf_mex_assign(&c12_e_y, sf_mex_create("y", &c12_d_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c12_y, 3, c12_e_y);
  sf_mex_assign(&c12_st, c12_y, FALSE);
  return c12_st;
}

static void set_sim_state_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance, const mxArray *c12_st)
{
  const mxArray *c12_u;
  real_T *c12_fx;
  real_T *c12_fy;
  real_T *c12_m;
  c12_m = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c12_fy = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c12_fx = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c12_doneDoubleBufferReInit = TRUE;
  c12_u = sf_mex_dup(c12_st);
  *c12_fx = c12_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c12_u,
    0)), "fx");
  *c12_fy = c12_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c12_u,
    1)), "fy");
  *c12_m = c12_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c12_u,
    2)), "m");
  chartInstance->c12_is_active_c12_BoatLibraryTest04 = c12_f_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c12_u, 3)),
     "is_active_c12_BoatLibraryTest04");
  sf_mex_destroy(&c12_u);
  c12_update_debugger_state_c12_BoatLibraryTest04(chartInstance);
  sf_mex_destroy(&c12_st);
}

static void finalize_c12_BoatLibraryTest04(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance)
{
}

static void sf_c12_BoatLibraryTest04(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance)
{
  real_T *c12_Fx;
  real_T *c12_fx;
  real_T *c12_Fy;
  real_T *c12_M;
  real_T *c12_fy;
  real_T *c12_m;
  real_T *c12_phi;
  c12_phi = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c12_m = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c12_fy = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c12_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c12_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c12_fx = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c12_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 3U, chartInstance->c12_sfEvent);
  _SFD_DATA_RANGE_CHECK(*c12_Fx, 0U);
  _SFD_DATA_RANGE_CHECK(*c12_fx, 1U);
  _SFD_DATA_RANGE_CHECK(*c12_Fy, 2U);
  _SFD_DATA_RANGE_CHECK(*c12_M, 3U);
  _SFD_DATA_RANGE_CHECK(*c12_fy, 4U);
  _SFD_DATA_RANGE_CHECK(*c12_m, 5U);
  _SFD_DATA_RANGE_CHECK(*c12_phi, 6U);
  chartInstance->c12_sfEvent = CALL_EVENT;
  c12_chartstep_c12_BoatLibraryTest04(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_BoatLibraryTest04MachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c12_chartstep_c12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance)
{
  real_T c12_hoistedGlobal;
  real_T c12_b_hoistedGlobal;
  real_T c12_c_hoistedGlobal;
  real_T c12_d_hoistedGlobal;
  real_T c12_Fx;
  real_T c12_Fy;
  real_T c12_M;
  real_T c12_phi;
  uint32_T c12_debug_family_var_map[11];
  real_T c12_R[9];
  real_T c12_FxyM[3];
  real_T c12_nargin = 4.0;
  real_T c12_nargout = 3.0;
  real_T c12_fx;
  real_T c12_fy;
  real_T c12_m;
  real_T c12_d0;
  real_T c12_d1;
  real_T c12_d2;
  real_T c12_d3;
  int32_T c12_i0;
  int32_T c12_i1;
  static real_T c12_dv0[3] = { 0.0, 0.0, 1.0 };

  int32_T c12_i2;
  int32_T c12_i3;
  int32_T c12_i4;
  int32_T c12_i5;
  real_T c12_a[9];
  real_T c12_b[3];
  int32_T c12_i6;
  int32_T c12_i7;
  int32_T c12_i8;
  real_T c12_C[3];
  int32_T c12_i9;
  int32_T c12_i10;
  int32_T c12_i11;
  int32_T c12_i12;
  int32_T c12_i13;
  int32_T c12_i14;
  real_T *c12_b_Fx;
  real_T *c12_b_Fy;
  real_T *c12_b_M;
  real_T *c12_b_phi;
  real_T *c12_b_fx;
  real_T *c12_b_fy;
  real_T *c12_b_m;
  c12_b_phi = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c12_b_m = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c12_b_fy = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c12_b_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c12_b_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c12_b_fx = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c12_b_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 3U, chartInstance->c12_sfEvent);
  c12_hoistedGlobal = *c12_b_Fx;
  c12_b_hoistedGlobal = *c12_b_Fy;
  c12_c_hoistedGlobal = *c12_b_M;
  c12_d_hoistedGlobal = *c12_b_phi;
  c12_Fx = c12_hoistedGlobal;
  c12_Fy = c12_b_hoistedGlobal;
  c12_M = c12_c_hoistedGlobal;
  c12_phi = c12_d_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 11U, 11U, c12_debug_family_names,
    c12_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c12_R, 0U, c12_c_sf_marshallOut,
    c12_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c12_FxyM, 1U, c12_b_sf_marshallOut,
    c12_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c12_nargin, 2U, c12_sf_marshallOut,
    c12_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c12_nargout, 3U, c12_sf_marshallOut,
    c12_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c12_Fx, 4U, c12_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c12_Fy, 5U, c12_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c12_M, 6U, c12_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c12_phi, 7U, c12_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c12_fx, 8U, c12_sf_marshallOut,
    c12_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c12_fy, 9U, c12_sf_marshallOut,
    c12_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c12_m, 10U, c12_sf_marshallOut,
    c12_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c12_sfEvent, 4);
  c12_d0 = c12_phi;
  c12_b_cosd(chartInstance, &c12_d0);
  c12_d1 = c12_phi;
  c12_b_sind(chartInstance, &c12_d1);
  c12_d2 = c12_phi;
  c12_b_sind(chartInstance, &c12_d2);
  c12_d3 = c12_phi;
  c12_b_cosd(chartInstance, &c12_d3);
  c12_R[0] = c12_d0;
  c12_R[3] = -c12_d1;
  c12_R[6] = 0.0;
  c12_R[1] = c12_d2;
  c12_R[4] = c12_d3;
  c12_R[7] = 0.0;
  c12_i0 = 0;
  for (c12_i1 = 0; c12_i1 < 3; c12_i1++) {
    c12_R[c12_i0 + 2] = c12_dv0[c12_i1];
    c12_i0 += 3;
  }

  _SFD_EML_CALL(0U, chartInstance->c12_sfEvent, 8);
  c12_i2 = 0;
  for (c12_i3 = 0; c12_i3 < 3; c12_i3++) {
    c12_i4 = 0;
    for (c12_i5 = 0; c12_i5 < 3; c12_i5++) {
      c12_a[c12_i5 + c12_i2] = c12_R[c12_i4 + c12_i3];
      c12_i4 += 3;
    }

    c12_i2 += 3;
  }

  c12_b[0] = c12_Fx;
  c12_b[1] = c12_Fy;
  c12_b[2] = c12_M;
  c12_eml_scalar_eg(chartInstance);
  c12_eml_scalar_eg(chartInstance);
  for (c12_i6 = 0; c12_i6 < 3; c12_i6++) {
    c12_FxyM[c12_i6] = 0.0;
  }

  for (c12_i7 = 0; c12_i7 < 3; c12_i7++) {
    c12_FxyM[c12_i7] = 0.0;
  }

  for (c12_i8 = 0; c12_i8 < 3; c12_i8++) {
    c12_C[c12_i8] = c12_FxyM[c12_i8];
  }

  for (c12_i9 = 0; c12_i9 < 3; c12_i9++) {
    c12_FxyM[c12_i9] = c12_C[c12_i9];
  }

  for (c12_i10 = 0; c12_i10 < 3; c12_i10++) {
    c12_C[c12_i10] = c12_FxyM[c12_i10];
  }

  for (c12_i11 = 0; c12_i11 < 3; c12_i11++) {
    c12_FxyM[c12_i11] = c12_C[c12_i11];
  }

  for (c12_i12 = 0; c12_i12 < 3; c12_i12++) {
    c12_FxyM[c12_i12] = 0.0;
    c12_i13 = 0;
    for (c12_i14 = 0; c12_i14 < 3; c12_i14++) {
      c12_FxyM[c12_i12] += c12_a[c12_i13 + c12_i12] * c12_b[c12_i14];
      c12_i13 += 3;
    }
  }

  _SFD_EML_CALL(0U, chartInstance->c12_sfEvent, 10);
  c12_fx = c12_FxyM[0];
  _SFD_EML_CALL(0U, chartInstance->c12_sfEvent, 11);
  c12_fy = c12_FxyM[1];
  _SFD_EML_CALL(0U, chartInstance->c12_sfEvent, 12);
  c12_m = c12_FxyM[2];
  _SFD_EML_CALL(0U, chartInstance->c12_sfEvent, -12);
  _SFD_SYMBOL_SCOPE_POP();
  *c12_b_fx = c12_fx;
  *c12_b_fy = c12_fy;
  *c12_b_m = c12_m;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 3U, chartInstance->c12_sfEvent);
}

static void initSimStructsc12_BoatLibraryTest04
  (SFc12_BoatLibraryTest04InstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c12_machineNumber, uint32_T
  c12_chartNumber)
{
}

static const mxArray *c12_sf_marshallOut(void *chartInstanceVoid, void
  *c12_inData)
{
  const mxArray *c12_mxArrayOutData = NULL;
  real_T c12_u;
  const mxArray *c12_y = NULL;
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c12_mxArrayOutData = NULL;
  c12_u = *(real_T *)c12_inData;
  c12_y = NULL;
  sf_mex_assign(&c12_y, sf_mex_create("y", &c12_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c12_mxArrayOutData, c12_y, FALSE);
  return c12_mxArrayOutData;
}

static real_T c12_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_m, const char_T *c12_identifier)
{
  real_T c12_y;
  emlrtMsgIdentifier c12_thisId;
  c12_thisId.fIdentifier = c12_identifier;
  c12_thisId.fParent = NULL;
  c12_y = c12_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c12_m), &c12_thisId);
  sf_mex_destroy(&c12_m);
  return c12_y;
}

static real_T c12_b_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId)
{
  real_T c12_y;
  real_T c12_d4;
  sf_mex_import(c12_parentId, sf_mex_dup(c12_u), &c12_d4, 1, 0, 0U, 0, 0U, 0);
  c12_y = c12_d4;
  sf_mex_destroy(&c12_u);
  return c12_y;
}

static void c12_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c12_mxArrayInData, const char_T *c12_varName, void *c12_outData)
{
  const mxArray *c12_m;
  const char_T *c12_identifier;
  emlrtMsgIdentifier c12_thisId;
  real_T c12_y;
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c12_m = sf_mex_dup(c12_mxArrayInData);
  c12_identifier = c12_varName;
  c12_thisId.fIdentifier = c12_identifier;
  c12_thisId.fParent = NULL;
  c12_y = c12_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c12_m), &c12_thisId);
  sf_mex_destroy(&c12_m);
  *(real_T *)c12_outData = c12_y;
  sf_mex_destroy(&c12_mxArrayInData);
}

static const mxArray *c12_b_sf_marshallOut(void *chartInstanceVoid, void
  *c12_inData)
{
  const mxArray *c12_mxArrayOutData = NULL;
  int32_T c12_i15;
  real_T c12_b_inData[3];
  int32_T c12_i16;
  real_T c12_u[3];
  const mxArray *c12_y = NULL;
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c12_mxArrayOutData = NULL;
  for (c12_i15 = 0; c12_i15 < 3; c12_i15++) {
    c12_b_inData[c12_i15] = (*(real_T (*)[3])c12_inData)[c12_i15];
  }

  for (c12_i16 = 0; c12_i16 < 3; c12_i16++) {
    c12_u[c12_i16] = c12_b_inData[c12_i16];
  }

  c12_y = NULL;
  sf_mex_assign(&c12_y, sf_mex_create("y", c12_u, 0, 0U, 1U, 0U, 1, 3), FALSE);
  sf_mex_assign(&c12_mxArrayOutData, c12_y, FALSE);
  return c12_mxArrayOutData;
}

static void c12_c_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId,
  real_T c12_y[3])
{
  real_T c12_dv1[3];
  int32_T c12_i17;
  sf_mex_import(c12_parentId, sf_mex_dup(c12_u), c12_dv1, 1, 0, 0U, 1, 0U, 1, 3);
  for (c12_i17 = 0; c12_i17 < 3; c12_i17++) {
    c12_y[c12_i17] = c12_dv1[c12_i17];
  }

  sf_mex_destroy(&c12_u);
}

static void c12_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c12_mxArrayInData, const char_T *c12_varName, void *c12_outData)
{
  const mxArray *c12_FxyM;
  const char_T *c12_identifier;
  emlrtMsgIdentifier c12_thisId;
  real_T c12_y[3];
  int32_T c12_i18;
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c12_FxyM = sf_mex_dup(c12_mxArrayInData);
  c12_identifier = c12_varName;
  c12_thisId.fIdentifier = c12_identifier;
  c12_thisId.fParent = NULL;
  c12_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c12_FxyM), &c12_thisId, c12_y);
  sf_mex_destroy(&c12_FxyM);
  for (c12_i18 = 0; c12_i18 < 3; c12_i18++) {
    (*(real_T (*)[3])c12_outData)[c12_i18] = c12_y[c12_i18];
  }

  sf_mex_destroy(&c12_mxArrayInData);
}

static const mxArray *c12_c_sf_marshallOut(void *chartInstanceVoid, void
  *c12_inData)
{
  const mxArray *c12_mxArrayOutData = NULL;
  int32_T c12_i19;
  int32_T c12_i20;
  int32_T c12_i21;
  real_T c12_b_inData[9];
  int32_T c12_i22;
  int32_T c12_i23;
  int32_T c12_i24;
  real_T c12_u[9];
  const mxArray *c12_y = NULL;
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c12_mxArrayOutData = NULL;
  c12_i19 = 0;
  for (c12_i20 = 0; c12_i20 < 3; c12_i20++) {
    for (c12_i21 = 0; c12_i21 < 3; c12_i21++) {
      c12_b_inData[c12_i21 + c12_i19] = (*(real_T (*)[9])c12_inData)[c12_i21 +
        c12_i19];
    }

    c12_i19 += 3;
  }

  c12_i22 = 0;
  for (c12_i23 = 0; c12_i23 < 3; c12_i23++) {
    for (c12_i24 = 0; c12_i24 < 3; c12_i24++) {
      c12_u[c12_i24 + c12_i22] = c12_b_inData[c12_i24 + c12_i22];
    }

    c12_i22 += 3;
  }

  c12_y = NULL;
  sf_mex_assign(&c12_y, sf_mex_create("y", c12_u, 0, 0U, 1U, 0U, 2, 3, 3), FALSE);
  sf_mex_assign(&c12_mxArrayOutData, c12_y, FALSE);
  return c12_mxArrayOutData;
}

static void c12_d_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId,
  real_T c12_y[9])
{
  real_T c12_dv2[9];
  int32_T c12_i25;
  sf_mex_import(c12_parentId, sf_mex_dup(c12_u), c12_dv2, 1, 0, 0U, 1, 0U, 2, 3,
                3);
  for (c12_i25 = 0; c12_i25 < 9; c12_i25++) {
    c12_y[c12_i25] = c12_dv2[c12_i25];
  }

  sf_mex_destroy(&c12_u);
}

static void c12_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c12_mxArrayInData, const char_T *c12_varName, void *c12_outData)
{
  const mxArray *c12_R;
  const char_T *c12_identifier;
  emlrtMsgIdentifier c12_thisId;
  real_T c12_y[9];
  int32_T c12_i26;
  int32_T c12_i27;
  int32_T c12_i28;
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c12_R = sf_mex_dup(c12_mxArrayInData);
  c12_identifier = c12_varName;
  c12_thisId.fIdentifier = c12_identifier;
  c12_thisId.fParent = NULL;
  c12_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c12_R), &c12_thisId, c12_y);
  sf_mex_destroy(&c12_R);
  c12_i26 = 0;
  for (c12_i27 = 0; c12_i27 < 3; c12_i27++) {
    for (c12_i28 = 0; c12_i28 < 3; c12_i28++) {
      (*(real_T (*)[9])c12_outData)[c12_i28 + c12_i26] = c12_y[c12_i28 + c12_i26];
    }

    c12_i26 += 3;
  }

  sf_mex_destroy(&c12_mxArrayInData);
}

const mxArray *sf_c12_BoatLibraryTest04_get_eml_resolved_functions_info(void)
{
  const mxArray *c12_nameCaptureInfo = NULL;
  c12_nameCaptureInfo = NULL;
  sf_mex_assign(&c12_nameCaptureInfo, sf_mex_createstruct("structure", 2, 36, 1),
                FALSE);
  c12_info_helper(&c12_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c12_nameCaptureInfo);
  return c12_nameCaptureInfo;
}

static void c12_info_helper(const mxArray **c12_info)
{
  const mxArray *c12_rhs0 = NULL;
  const mxArray *c12_lhs0 = NULL;
  const mxArray *c12_rhs1 = NULL;
  const mxArray *c12_lhs1 = NULL;
  const mxArray *c12_rhs2 = NULL;
  const mxArray *c12_lhs2 = NULL;
  const mxArray *c12_rhs3 = NULL;
  const mxArray *c12_lhs3 = NULL;
  const mxArray *c12_rhs4 = NULL;
  const mxArray *c12_lhs4 = NULL;
  const mxArray *c12_rhs5 = NULL;
  const mxArray *c12_lhs5 = NULL;
  const mxArray *c12_rhs6 = NULL;
  const mxArray *c12_lhs6 = NULL;
  const mxArray *c12_rhs7 = NULL;
  const mxArray *c12_lhs7 = NULL;
  const mxArray *c12_rhs8 = NULL;
  const mxArray *c12_lhs8 = NULL;
  const mxArray *c12_rhs9 = NULL;
  const mxArray *c12_lhs9 = NULL;
  const mxArray *c12_rhs10 = NULL;
  const mxArray *c12_lhs10 = NULL;
  const mxArray *c12_rhs11 = NULL;
  const mxArray *c12_lhs11 = NULL;
  const mxArray *c12_rhs12 = NULL;
  const mxArray *c12_lhs12 = NULL;
  const mxArray *c12_rhs13 = NULL;
  const mxArray *c12_lhs13 = NULL;
  const mxArray *c12_rhs14 = NULL;
  const mxArray *c12_lhs14 = NULL;
  const mxArray *c12_rhs15 = NULL;
  const mxArray *c12_lhs15 = NULL;
  const mxArray *c12_rhs16 = NULL;
  const mxArray *c12_lhs16 = NULL;
  const mxArray *c12_rhs17 = NULL;
  const mxArray *c12_lhs17 = NULL;
  const mxArray *c12_rhs18 = NULL;
  const mxArray *c12_lhs18 = NULL;
  const mxArray *c12_rhs19 = NULL;
  const mxArray *c12_lhs19 = NULL;
  const mxArray *c12_rhs20 = NULL;
  const mxArray *c12_lhs20 = NULL;
  const mxArray *c12_rhs21 = NULL;
  const mxArray *c12_lhs21 = NULL;
  const mxArray *c12_rhs22 = NULL;
  const mxArray *c12_lhs22 = NULL;
  const mxArray *c12_rhs23 = NULL;
  const mxArray *c12_lhs23 = NULL;
  const mxArray *c12_rhs24 = NULL;
  const mxArray *c12_lhs24 = NULL;
  const mxArray *c12_rhs25 = NULL;
  const mxArray *c12_lhs25 = NULL;
  const mxArray *c12_rhs26 = NULL;
  const mxArray *c12_lhs26 = NULL;
  const mxArray *c12_rhs27 = NULL;
  const mxArray *c12_lhs27 = NULL;
  const mxArray *c12_rhs28 = NULL;
  const mxArray *c12_lhs28 = NULL;
  const mxArray *c12_rhs29 = NULL;
  const mxArray *c12_lhs29 = NULL;
  const mxArray *c12_rhs30 = NULL;
  const mxArray *c12_lhs30 = NULL;
  const mxArray *c12_rhs31 = NULL;
  const mxArray *c12_lhs31 = NULL;
  const mxArray *c12_rhs32 = NULL;
  const mxArray *c12_lhs32 = NULL;
  const mxArray *c12_rhs33 = NULL;
  const mxArray *c12_lhs33 = NULL;
  const mxArray *c12_rhs34 = NULL;
  const mxArray *c12_lhs34 = NULL;
  const mxArray *c12_rhs35 = NULL;
  const mxArray *c12_lhs35 = NULL;
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("cosd"), "name", "name", 0);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "resolved",
                  "resolved", 0);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1343851972U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c12_rhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs0), "rhs", "rhs",
                  0);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs0), "lhs", "lhs",
                  0);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "context",
                  "context", 1);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 1);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 1);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c12_rhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs1), "rhs", "rhs",
                  1);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs1), "lhs", "lhs",
                  1);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 2);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("isfinite"), "name", "name",
                  2);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "resolved",
                  "resolved", 2);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c12_rhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs2), "rhs", "rhs",
                  2);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs2), "lhs", "lhs",
                  2);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 3);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 3);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 3);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c12_rhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs3), "rhs", "rhs",
                  3);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs3), "lhs", "lhs",
                  3);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 4);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("isinf"), "name", "name", 4);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "resolved",
                  "resolved", 4);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c12_rhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs4), "rhs", "rhs",
                  4);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs4), "lhs", "lhs",
                  4);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "context",
                  "context", 5);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 5);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 5);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c12_rhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs5), "rhs", "rhs",
                  5);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs5), "lhs", "lhs",
                  5);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 6);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("isnan"), "name", "name", 6);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "resolved",
                  "resolved", 6);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731858U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c12_rhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs6), "rhs", "rhs",
                  6);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs6), "lhs", "lhs",
                  6);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "context",
                  "context", 7);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 7);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 7);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c12_rhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs7), "rhs", "rhs",
                  7);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs7), "lhs", "lhs",
                  7);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 8);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_guarded_nan"), "name",
                  "name", 8);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "resolved", "resolved", 8);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1286840376U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c12_rhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs8), "rhs", "rhs",
                  8);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs8), "lhs", "lhs",
                  8);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "context", "context", 9);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_is_float_class"), "name",
                  "name", 9);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m"),
                  "resolved", "resolved", 9);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1286840382U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c12_rhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs9), "rhs", "rhs",
                  9);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs9), "lhs", "lhs",
                  9);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 10);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalar_rem90"), "name",
                  "name", 10);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "resolved", "resolved", 10);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c12_rhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 11);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("mrdivide"), "name", "name",
                  11);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 11);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c12_rhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "context",
                  "context", 12);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("rdivide"), "name", "name",
                  12);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "resolved",
                  "resolved", 12);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c12_rhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 13);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 13);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 13);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c12_rhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 14);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalexp_compatible"),
                  "name", "name", 14);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m"),
                  "resolved", "resolved", 14);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c12_rhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs14), "lhs", "lhs",
                  14);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 15);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_div"), "name", "name",
                  15);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 15);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m"), "resolved",
                  "resolved", 15);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731866U), "fileTimeLo",
                  "fileTimeLo", 15);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 15);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 15);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 15);
  sf_mex_assign(&c12_rhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs15), "rhs", "rhs",
                  15);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs15), "lhs", "lhs",
                  15);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 16);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("rem"), "name", "name", 16);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 16);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "resolved",
                  "resolved", 16);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731854U), "fileTimeLo",
                  "fileTimeLo", 16);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 16);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 16);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 16);
  sf_mex_assign(&c12_rhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs16), "rhs", "rhs",
                  16);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs16), "lhs", "lhs",
                  16);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 17);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 17);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 17);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 17);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 17);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 17);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 17);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 17);
  sf_mex_assign(&c12_rhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs17), "rhs", "rhs",
                  17);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs17), "lhs", "lhs",
                  17);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 18);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 18);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 18);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 18);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 18);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 18);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 18);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 18);
  sf_mex_assign(&c12_rhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs18), "rhs", "rhs",
                  18);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs18), "lhs", "lhs",
                  18);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 19);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 19);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 19);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 19);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1358207740U), "fileTimeLo",
                  "fileTimeLo", 19);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 19);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 19);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 19);
  sf_mex_assign(&c12_rhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs19), "rhs", "rhs",
                  19);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs19), "lhs", "lhs",
                  19);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 20);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("abs"), "name", "name", 20);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 20);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "resolved",
                  "resolved", 20);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731852U), "fileTimeLo",
                  "fileTimeLo", 20);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 20);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 20);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 20);
  sf_mex_assign(&c12_rhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs20), "rhs", "rhs",
                  20);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs20), "lhs", "lhs",
                  20);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 21);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 21);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 21);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 21);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 21);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 21);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 21);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 21);
  sf_mex_assign(&c12_rhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs21), "rhs", "rhs",
                  21);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs21), "lhs", "lhs",
                  21);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 22);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalar_abs"), "name",
                  "name", 22);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 22);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m"),
                  "resolved", "resolved", 22);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1286840312U), "fileTimeLo",
                  "fileTimeLo", 22);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 22);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 22);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 22);
  sf_mex_assign(&c12_rhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs22), "rhs", "rhs",
                  22);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs22), "lhs", "lhs",
                  22);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 23);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("mtimes"), "name", "name", 23);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 23);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 23);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 23);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 23);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 23);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 23);
  sf_mex_assign(&c12_rhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs23), "rhs", "rhs",
                  23);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs23), "lhs", "lhs",
                  23);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m!common_checks"),
                  "context", "context", 24);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 24);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 24);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 24);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 24);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 24);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 24);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 24);
  sf_mex_assign(&c12_rhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs24), "rhs", "rhs",
                  24);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs24), "lhs", "lhs",
                  24);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(""), "context", "context", 25);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("sind"), "name", "name", 25);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 25);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "resolved",
                  "resolved", 25);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1343851986U), "fileTimeLo",
                  "fileTimeLo", 25);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 25);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 25);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 25);
  sf_mex_assign(&c12_rhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs25), "rhs", "rhs",
                  25);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs25), "lhs", "lhs",
                  25);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "context",
                  "context", 26);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 26);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 26);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 26);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 26);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 26);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 26);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 26);
  sf_mex_assign(&c12_rhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs26), "rhs", "rhs",
                  26);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs26), "lhs", "lhs",
                  26);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(""), "context", "context", 27);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("mtimes"), "name", "name", 27);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 27);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 27);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 27);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 27);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 27);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 27);
  sf_mex_assign(&c12_rhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs27), "rhs", "rhs",
                  27);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs27), "lhs", "lhs",
                  27);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "context",
                  "context", 28);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 28);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 28);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 28);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1323192178U), "fileTimeLo",
                  "fileTimeLo", 28);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 28);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 28);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 28);
  sf_mex_assign(&c12_rhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs28), "rhs", "rhs",
                  28);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs28), "lhs", "lhs",
                  28);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "context",
                  "context", 29);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 29);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 29);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 29);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 29);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 29);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 29);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 29);
  sf_mex_assign(&c12_rhs29, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs29, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs29), "rhs", "rhs",
                  29);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs29), "lhs", "lhs",
                  29);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "context",
                  "context", 30);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_xgemm"), "name", "name",
                  30);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 30);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m"),
                  "resolved", "resolved", 30);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731870U), "fileTimeLo",
                  "fileTimeLo", 30);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 30);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 30);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 30);
  sf_mex_assign(&c12_rhs30, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs30, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs30), "rhs", "rhs",
                  30);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs30), "lhs", "lhs",
                  30);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m"), "context",
                  "context", 31);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_blas_inline"), "name",
                  "name", 31);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 31);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m"),
                  "resolved", "resolved", 31);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1299098368U), "fileTimeLo",
                  "fileTimeLo", 31);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 31);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 31);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 31);
  sf_mex_assign(&c12_rhs31, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs31, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs31), "rhs", "rhs",
                  31);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs31), "lhs", "lhs",
                  31);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m!below_threshold"),
                  "context", "context", 32);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("mtimes"), "name", "name", 32);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 32);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 32);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 32);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 32);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 32);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 32);
  sf_mex_assign(&c12_rhs32, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs32, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs32), "rhs", "rhs",
                  32);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs32), "lhs", "lhs",
                  32);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m"),
                  "context", "context", 33);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 33);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 33);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 33);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1323192178U), "fileTimeLo",
                  "fileTimeLo", 33);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 33);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 33);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 33);
  sf_mex_assign(&c12_rhs33, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs33, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs33), "rhs", "rhs",
                  33);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs33), "lhs", "lhs",
                  33);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m"),
                  "context", "context", 34);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 34);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 34);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 34);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 34);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 34);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 34);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 34);
  sf_mex_assign(&c12_rhs34, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs34, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs34), "rhs", "rhs",
                  34);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs34), "lhs", "lhs",
                  34);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xgemm.m"),
                  "context", "context", 35);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("eml_refblas_xgemm"), "name",
                  "name", 35);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 35);
  sf_mex_addfield(*c12_info, c12_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgemm.m"),
                  "resolved", "resolved", 35);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(1360303950U), "fileTimeLo",
                  "fileTimeLo", 35);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 35);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 35);
  sf_mex_addfield(*c12_info, c12_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 35);
  sf_mex_assign(&c12_rhs35, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c12_lhs35, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_rhs35), "rhs", "rhs",
                  35);
  sf_mex_addfield(*c12_info, sf_mex_duplicatearraysafe(&c12_lhs35), "lhs", "lhs",
                  35);
  sf_mex_destroy(&c12_rhs0);
  sf_mex_destroy(&c12_lhs0);
  sf_mex_destroy(&c12_rhs1);
  sf_mex_destroy(&c12_lhs1);
  sf_mex_destroy(&c12_rhs2);
  sf_mex_destroy(&c12_lhs2);
  sf_mex_destroy(&c12_rhs3);
  sf_mex_destroy(&c12_lhs3);
  sf_mex_destroy(&c12_rhs4);
  sf_mex_destroy(&c12_lhs4);
  sf_mex_destroy(&c12_rhs5);
  sf_mex_destroy(&c12_lhs5);
  sf_mex_destroy(&c12_rhs6);
  sf_mex_destroy(&c12_lhs6);
  sf_mex_destroy(&c12_rhs7);
  sf_mex_destroy(&c12_lhs7);
  sf_mex_destroy(&c12_rhs8);
  sf_mex_destroy(&c12_lhs8);
  sf_mex_destroy(&c12_rhs9);
  sf_mex_destroy(&c12_lhs9);
  sf_mex_destroy(&c12_rhs10);
  sf_mex_destroy(&c12_lhs10);
  sf_mex_destroy(&c12_rhs11);
  sf_mex_destroy(&c12_lhs11);
  sf_mex_destroy(&c12_rhs12);
  sf_mex_destroy(&c12_lhs12);
  sf_mex_destroy(&c12_rhs13);
  sf_mex_destroy(&c12_lhs13);
  sf_mex_destroy(&c12_rhs14);
  sf_mex_destroy(&c12_lhs14);
  sf_mex_destroy(&c12_rhs15);
  sf_mex_destroy(&c12_lhs15);
  sf_mex_destroy(&c12_rhs16);
  sf_mex_destroy(&c12_lhs16);
  sf_mex_destroy(&c12_rhs17);
  sf_mex_destroy(&c12_lhs17);
  sf_mex_destroy(&c12_rhs18);
  sf_mex_destroy(&c12_lhs18);
  sf_mex_destroy(&c12_rhs19);
  sf_mex_destroy(&c12_lhs19);
  sf_mex_destroy(&c12_rhs20);
  sf_mex_destroy(&c12_lhs20);
  sf_mex_destroy(&c12_rhs21);
  sf_mex_destroy(&c12_lhs21);
  sf_mex_destroy(&c12_rhs22);
  sf_mex_destroy(&c12_lhs22);
  sf_mex_destroy(&c12_rhs23);
  sf_mex_destroy(&c12_lhs23);
  sf_mex_destroy(&c12_rhs24);
  sf_mex_destroy(&c12_lhs24);
  sf_mex_destroy(&c12_rhs25);
  sf_mex_destroy(&c12_lhs25);
  sf_mex_destroy(&c12_rhs26);
  sf_mex_destroy(&c12_lhs26);
  sf_mex_destroy(&c12_rhs27);
  sf_mex_destroy(&c12_lhs27);
  sf_mex_destroy(&c12_rhs28);
  sf_mex_destroy(&c12_lhs28);
  sf_mex_destroy(&c12_rhs29);
  sf_mex_destroy(&c12_lhs29);
  sf_mex_destroy(&c12_rhs30);
  sf_mex_destroy(&c12_lhs30);
  sf_mex_destroy(&c12_rhs31);
  sf_mex_destroy(&c12_lhs31);
  sf_mex_destroy(&c12_rhs32);
  sf_mex_destroy(&c12_lhs32);
  sf_mex_destroy(&c12_rhs33);
  sf_mex_destroy(&c12_lhs33);
  sf_mex_destroy(&c12_rhs34);
  sf_mex_destroy(&c12_lhs34);
  sf_mex_destroy(&c12_rhs35);
  sf_mex_destroy(&c12_lhs35);
}

static const mxArray *c12_emlrt_marshallOut(char * c12_u)
{
  const mxArray *c12_y = NULL;
  c12_y = NULL;
  sf_mex_assign(&c12_y, sf_mex_create("y", c12_u, 15, 0U, 0U, 0U, 2, 1, strlen
    (c12_u)), FALSE);
  return c12_y;
}

static const mxArray *c12_b_emlrt_marshallOut(uint32_T c12_u)
{
  const mxArray *c12_y = NULL;
  c12_y = NULL;
  sf_mex_assign(&c12_y, sf_mex_create("y", &c12_u, 7, 0U, 0U, 0U, 0), FALSE);
  return c12_y;
}

static real_T c12_cosd(SFc12_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c12_x)
{
  real_T c12_b_x;
  c12_b_x = c12_x;
  c12_b_cosd(chartInstance, &c12_b_x);
  return c12_b_x;
}

static boolean_T c12_isfinite(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, real_T c12_x)
{
  real_T c12_b_x;
  boolean_T c12_b_b;
  boolean_T c12_b0;
  real_T c12_c_x;
  boolean_T c12_c_b;
  boolean_T c12_b1;
  c12_b_x = c12_x;
  c12_b_b = muDoubleScalarIsInf(c12_b_x);
  c12_b0 = !c12_b_b;
  c12_c_x = c12_x;
  c12_c_b = muDoubleScalarIsNaN(c12_c_x);
  c12_b1 = !c12_c_b;
  return c12_b0 && c12_b1;
}

static real_T c12_sind(SFc12_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T c12_x)
{
  real_T c12_b_x;
  c12_b_x = c12_x;
  c12_b_sind(chartInstance, &c12_b_x);
  return c12_b_x;
}

static void c12_eml_scalar_eg(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance)
{
}

static const mxArray *c12_d_sf_marshallOut(void *chartInstanceVoid, void
  *c12_inData)
{
  const mxArray *c12_mxArrayOutData = NULL;
  int32_T c12_u;
  const mxArray *c12_y = NULL;
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c12_mxArrayOutData = NULL;
  c12_u = *(int32_T *)c12_inData;
  c12_y = NULL;
  sf_mex_assign(&c12_y, sf_mex_create("y", &c12_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c12_mxArrayOutData, c12_y, FALSE);
  return c12_mxArrayOutData;
}

static int32_T c12_e_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId)
{
  int32_T c12_y;
  int32_T c12_i29;
  sf_mex_import(c12_parentId, sf_mex_dup(c12_u), &c12_i29, 1, 6, 0U, 0, 0U, 0);
  c12_y = c12_i29;
  sf_mex_destroy(&c12_u);
  return c12_y;
}

static void c12_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c12_mxArrayInData, const char_T *c12_varName, void *c12_outData)
{
  const mxArray *c12_b_sfEvent;
  const char_T *c12_identifier;
  emlrtMsgIdentifier c12_thisId;
  int32_T c12_y;
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)chartInstanceVoid;
  c12_b_sfEvent = sf_mex_dup(c12_mxArrayInData);
  c12_identifier = c12_varName;
  c12_thisId.fIdentifier = c12_identifier;
  c12_thisId.fParent = NULL;
  c12_y = c12_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c12_b_sfEvent),
    &c12_thisId);
  sf_mex_destroy(&c12_b_sfEvent);
  *(int32_T *)c12_outData = c12_y;
  sf_mex_destroy(&c12_mxArrayInData);
}

static uint8_T c12_f_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_b_is_active_c12_BoatLibraryTest04, const
  char_T *c12_identifier)
{
  uint8_T c12_y;
  emlrtMsgIdentifier c12_thisId;
  c12_thisId.fIdentifier = c12_identifier;
  c12_thisId.fParent = NULL;
  c12_y = c12_g_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c12_b_is_active_c12_BoatLibraryTest04), &c12_thisId);
  sf_mex_destroy(&c12_b_is_active_c12_BoatLibraryTest04);
  return c12_y;
}

static uint8_T c12_g_emlrt_marshallIn(SFc12_BoatLibraryTest04InstanceStruct
  *chartInstance, const mxArray *c12_u, const emlrtMsgIdentifier *c12_parentId)
{
  uint8_T c12_y;
  uint8_T c12_u0;
  sf_mex_import(c12_parentId, sf_mex_dup(c12_u), &c12_u0, 1, 3, 0U, 0, 0U, 0);
  c12_y = c12_u0;
  sf_mex_destroy(&c12_u);
  return c12_y;
}

static void c12_b_cosd(SFc12_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T *c12_x)
{
  real_T c12_b_x;
  real_T c12_c_x;
  real_T c12_d_x;
  real_T c12_e_x;
  real_T c12_xk;
  real_T c12_f_x;
  real_T c12_g_x;
  real_T c12_absx;
  real_T c12_h_x;
  real_T c12_i_x;
  real_T c12_b;
  int8_T c12_n;
  real_T c12_b_b;
  real_T c12_c_b;
  real_T c12_d_b;
  real_T c12_e_b;
  int8_T c12_b_n;
  c12_b_x = *c12_x;
  if (!c12_isfinite(chartInstance, c12_b_x)) {
    *c12_x = rtNaN;
  } else {
    c12_c_x = c12_b_x;
    c12_d_x = c12_c_x;
    c12_e_x = c12_d_x;
    c12_xk = c12_e_x;
    c12_d_x = muDoubleScalarRem(c12_xk, 360.0);
    c12_f_x = c12_d_x;
    c12_g_x = c12_f_x;
    c12_absx = muDoubleScalarAbs(c12_g_x);
    if (c12_absx > 180.0) {
      if (c12_d_x > 0.0) {
        c12_d_x -= 360.0;
      } else {
        c12_d_x += 360.0;
      }

      c12_h_x = c12_d_x;
      c12_i_x = c12_h_x;
      c12_absx = muDoubleScalarAbs(c12_i_x);
    }

    if (c12_absx <= 45.0) {
      c12_b = c12_d_x;
      c12_d_x = 0.017453292519943295 * c12_b;
      c12_n = 0;
    } else if (c12_absx <= 135.0) {
      if (c12_d_x > 0.0) {
        c12_b_b = c12_d_x - 90.0;
        c12_d_x = 0.017453292519943295 * c12_b_b;
        c12_n = 1;
      } else {
        c12_c_b = c12_d_x + 90.0;
        c12_d_x = 0.017453292519943295 * c12_c_b;
        c12_n = -1;
      }
    } else if (c12_d_x > 0.0) {
      c12_d_b = c12_d_x - 180.0;
      c12_d_x = 0.017453292519943295 * c12_d_b;
      c12_n = 2;
    } else {
      c12_e_b = c12_d_x + 180.0;
      c12_d_x = 0.017453292519943295 * c12_e_b;
      c12_n = -2;
    }

    c12_b_n = c12_n;
    c12_b_x = c12_d_x;
    if ((real_T)c12_b_n == 0.0) {
      *c12_x = muDoubleScalarCos(c12_b_x);
    } else if ((real_T)c12_b_n == 1.0) {
      *c12_x = -muDoubleScalarSin(c12_b_x);
    } else if ((real_T)c12_b_n == -1.0) {
      *c12_x = muDoubleScalarSin(c12_b_x);
    } else {
      *c12_x = -muDoubleScalarCos(c12_b_x);
    }
  }
}

static void c12_b_sind(SFc12_BoatLibraryTest04InstanceStruct *chartInstance,
  real_T *c12_x)
{
  real_T c12_b_x;
  real_T c12_d5;
  real_T c12_c_x;
  real_T c12_d_x;
  real_T c12_e_x;
  real_T c12_xk;
  real_T c12_f_x;
  real_T c12_g_x;
  real_T c12_absx;
  real_T c12_h_x;
  real_T c12_i_x;
  real_T c12_b;
  int8_T c12_n;
  real_T c12_b_b;
  real_T c12_c_b;
  real_T c12_d_b;
  real_T c12_e_b;
  int8_T c12_b_n;
  c12_b_x = *c12_x;
  if (!c12_isfinite(chartInstance, c12_b_x)) {
    c12_d5 = rtNaN;
  } else {
    c12_c_x = c12_b_x;
    c12_d_x = c12_c_x;
    c12_e_x = c12_d_x;
    c12_xk = c12_e_x;
    c12_d_x = muDoubleScalarRem(c12_xk, 360.0);
    c12_f_x = c12_d_x;
    c12_g_x = c12_f_x;
    c12_absx = muDoubleScalarAbs(c12_g_x);
    if (c12_absx > 180.0) {
      if (c12_d_x > 0.0) {
        c12_d_x -= 360.0;
      } else {
        c12_d_x += 360.0;
      }

      c12_h_x = c12_d_x;
      c12_i_x = c12_h_x;
      c12_absx = muDoubleScalarAbs(c12_i_x);
    }

    if (c12_absx <= 45.0) {
      c12_b = c12_d_x;
      c12_d_x = 0.017453292519943295 * c12_b;
      c12_n = 0;
    } else if (c12_absx <= 135.0) {
      if (c12_d_x > 0.0) {
        c12_b_b = c12_d_x - 90.0;
        c12_d_x = 0.017453292519943295 * c12_b_b;
        c12_n = 1;
      } else {
        c12_c_b = c12_d_x + 90.0;
        c12_d_x = 0.017453292519943295 * c12_c_b;
        c12_n = -1;
      }
    } else if (c12_d_x > 0.0) {
      c12_d_b = c12_d_x - 180.0;
      c12_d_x = 0.017453292519943295 * c12_d_b;
      c12_n = 2;
    } else {
      c12_e_b = c12_d_x + 180.0;
      c12_d_x = 0.017453292519943295 * c12_e_b;
      c12_n = -2;
    }

    c12_b_n = c12_n;
    c12_b_x = c12_d_x;
    if ((real_T)c12_b_n == 0.0) {
      c12_d5 = muDoubleScalarSin(c12_b_x);
    } else if ((real_T)c12_b_n == 1.0) {
      c12_d5 = muDoubleScalarCos(c12_b_x);
    } else if ((real_T)c12_b_n == -1.0) {
      c12_d5 = -muDoubleScalarCos(c12_b_x);
    } else {
      c12_d5 = -muDoubleScalarSin(c12_b_x);
    }
  }

  *c12_x = c12_d5;
}

static void init_dsm_address_info(SFc12_BoatLibraryTest04InstanceStruct
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

void sf_c12_BoatLibraryTest04_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(629892132U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(582960420U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1493189627U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2224329104U);
}

mxArray *sf_c12_BoatLibraryTest04_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("FM7LWvF0B869LcIphGqgaD");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,4,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
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
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,3,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
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
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c12_BoatLibraryTest04_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c12_BoatLibraryTest04_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c12_BoatLibraryTest04(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x4'type','srcId','name','auxInfo'{{M[1],M[5],T\"fx\",},{M[1],M[8],T\"fy\",},{M[1],M[9],T\"m\",},{M[8],M[0],T\"is_active_c12_BoatLibraryTest04\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 4, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c12_BoatLibraryTest04_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
    chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _BoatLibraryTest04MachineNumber_,
           12,
           1,
           1,
           7,
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
          _SFD_SET_DATA_PROPS(0,1,1,0,"Fx");
          _SFD_SET_DATA_PROPS(1,2,0,1,"fx");
          _SFD_SET_DATA_PROPS(2,1,1,0,"Fy");
          _SFD_SET_DATA_PROPS(3,1,1,0,"M");
          _SFD_SET_DATA_PROPS(4,2,0,1,"fy");
          _SFD_SET_DATA_PROPS(5,2,0,1,"m");
          _SFD_SET_DATA_PROPS(6,1,1,0,"phi");
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
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,245);
        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c12_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c12_sf_marshallOut,(MexInFcnForType)c12_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c12_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c12_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c12_sf_marshallOut,(MexInFcnForType)c12_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c12_sf_marshallOut,(MexInFcnForType)c12_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c12_sf_marshallOut,(MexInFcnForType)NULL);

        {
          real_T *c12_Fx;
          real_T *c12_fx;
          real_T *c12_Fy;
          real_T *c12_M;
          real_T *c12_fy;
          real_T *c12_m;
          real_T *c12_phi;
          c12_phi = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c12_m = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
          c12_fy = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
          c12_M = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c12_Fy = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c12_fx = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
          c12_Fx = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, c12_Fx);
          _SFD_SET_DATA_VALUE_PTR(1U, c12_fx);
          _SFD_SET_DATA_VALUE_PTR(2U, c12_Fy);
          _SFD_SET_DATA_VALUE_PTR(3U, c12_M);
          _SFD_SET_DATA_VALUE_PTR(4U, c12_fy);
          _SFD_SET_DATA_VALUE_PTR(5U, c12_m);
          _SFD_SET_DATA_VALUE_PTR(6U, c12_phi);
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
  return "m8h0Ic7g5d9DDUhONhCT7B";
}

static void sf_opaque_initialize_c12_BoatLibraryTest04(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc12_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c12_BoatLibraryTest04((SFc12_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
  initialize_c12_BoatLibraryTest04((SFc12_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c12_BoatLibraryTest04(void *chartInstanceVar)
{
  enable_c12_BoatLibraryTest04((SFc12_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c12_BoatLibraryTest04(void *chartInstanceVar)
{
  disable_c12_BoatLibraryTest04((SFc12_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c12_BoatLibraryTest04(void *chartInstanceVar)
{
  sf_c12_BoatLibraryTest04((SFc12_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c12_BoatLibraryTest04(SimStruct*
  S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c12_BoatLibraryTest04
    ((SFc12_BoatLibraryTest04InstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c12_BoatLibraryTest04();/* state var info */
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

extern void sf_internal_set_sim_state_c12_BoatLibraryTest04(SimStruct* S, const
  mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c12_BoatLibraryTest04();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c12_BoatLibraryTest04((SFc12_BoatLibraryTest04InstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c12_BoatLibraryTest04(SimStruct* S)
{
  return sf_internal_get_sim_state_c12_BoatLibraryTest04(S);
}

static void sf_opaque_set_sim_state_c12_BoatLibraryTest04(SimStruct* S, const
  mxArray *st)
{
  sf_internal_set_sim_state_c12_BoatLibraryTest04(S, st);
}

static void sf_opaque_terminate_c12_BoatLibraryTest04(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc12_BoatLibraryTest04InstanceStruct*) chartInstanceVar)
      ->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_BoatLibraryTest04_optimization_info();
    }

    finalize_c12_BoatLibraryTest04((SFc12_BoatLibraryTest04InstanceStruct*)
      chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc12_BoatLibraryTest04((SFc12_BoatLibraryTest04InstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c12_BoatLibraryTest04(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c12_BoatLibraryTest04
      ((SFc12_BoatLibraryTest04InstanceStruct*)(((ChartInfoStruct *)
         ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c12_BoatLibraryTest04(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_BoatLibraryTest04_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      12);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,12,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,12,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,12);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,12,4);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,12,3);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=3; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 4; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,12);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2921666594U));
  ssSetChecksum1(S,(1769433281U));
  ssSetChecksum2(S,(3362496522U));
  ssSetChecksum3(S,(1399810497U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c12_BoatLibraryTest04(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c12_BoatLibraryTest04(SimStruct *S)
{
  SFc12_BoatLibraryTest04InstanceStruct *chartInstance;
  chartInstance = (SFc12_BoatLibraryTest04InstanceStruct *)utMalloc(sizeof
    (SFc12_BoatLibraryTest04InstanceStruct));
  memset(chartInstance, 0, sizeof(SFc12_BoatLibraryTest04InstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c12_BoatLibraryTest04;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c12_BoatLibraryTest04;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c12_BoatLibraryTest04;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c12_BoatLibraryTest04;
  chartInstance->chartInfo.disableChart =
    sf_opaque_disable_c12_BoatLibraryTest04;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c12_BoatLibraryTest04;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c12_BoatLibraryTest04;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c12_BoatLibraryTest04;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c12_BoatLibraryTest04;
  chartInstance->chartInfo.mdlStart = mdlStart_c12_BoatLibraryTest04;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c12_BoatLibraryTest04;
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

void c12_BoatLibraryTest04_method_dispatcher(SimStruct *S, int_T method, void
  *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c12_BoatLibraryTest04(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c12_BoatLibraryTest04(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c12_BoatLibraryTest04(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c12_BoatLibraryTest04_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
