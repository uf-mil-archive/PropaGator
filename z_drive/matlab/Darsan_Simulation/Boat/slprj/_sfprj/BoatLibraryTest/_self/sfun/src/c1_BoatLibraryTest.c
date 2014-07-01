/* Include files */

#include <stddef.h>
#include "blas.h"
#include "BoatLibraryTest_sfun.h"
#include "c1_BoatLibraryTest.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "BoatLibraryTest_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c1_debug_family_names[17] = { "v11", "v12", "v13", "v14",
  "v15", "v16", "nargin", "nargout", "v0", "u0", "rx1", "rx2", "ry1", "ry2", "m",
  "I", "v1" };

/* Function Declarations */
static void initialize_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance);
static void initialize_params_c1_BoatLibraryTest
  (SFc1_BoatLibraryTestInstanceStruct *chartInstance);
static void enable_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance);
static void disable_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance);
static void c1_update_debugger_state_c1_BoatLibraryTest
  (SFc1_BoatLibraryTestInstanceStruct *chartInstance);
static const mxArray *get_sim_state_c1_BoatLibraryTest
  (SFc1_BoatLibraryTestInstanceStruct *chartInstance);
static void set_sim_state_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct *
  chartInstance, const mxArray *c1_st);
static void finalize_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance);
static void sf_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance);
static void c1_chartstep_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance);
static void initSimStructsc1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct *
  chartInstance);
static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber);
static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData);
static void c1_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_v1, const char_T *c1_identifier, real_T
  c1_y[6]);
static void c1_b_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[6]);
static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static real_T c1_c_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static void c1_info_helper(const mxArray **c1_info);
static const mxArray *c1_emlrt_marshallOut(char * c1_u);
static const mxArray *c1_b_emlrt_marshallOut(uint32_T c1_u);
static real_T c1_sind(SFc1_BoatLibraryTestInstanceStruct *chartInstance, real_T
                      c1_x);
static boolean_T c1_isfinite(SFc1_BoatLibraryTestInstanceStruct *chartInstance,
  real_T c1_x);
static real_T c1_cosd(SFc1_BoatLibraryTestInstanceStruct *chartInstance, real_T
                      c1_x);
static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static int32_T c1_d_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static uint8_T c1_e_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_b_is_active_c1_BoatLibraryTest, const char_T
  *c1_identifier);
static uint8_T c1_f_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_b_sind(SFc1_BoatLibraryTestInstanceStruct *chartInstance, real_T *
                      c1_x);
static void c1_b_cosd(SFc1_BoatLibraryTestInstanceStruct *chartInstance, real_T *
                      c1_x);
static void init_dsm_address_info(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance)
{
  chartInstance->c1_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c1_is_active_c1_BoatLibraryTest = 0U;
}

static void initialize_params_c1_BoatLibraryTest
  (SFc1_BoatLibraryTestInstanceStruct *chartInstance)
{
}

static void enable_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c1_update_debugger_state_c1_BoatLibraryTest
  (SFc1_BoatLibraryTestInstanceStruct *chartInstance)
{
}

static const mxArray *get_sim_state_c1_BoatLibraryTest
  (SFc1_BoatLibraryTestInstanceStruct *chartInstance)
{
  const mxArray *c1_st;
  const mxArray *c1_y = NULL;
  int32_T c1_i0;
  real_T c1_u[6];
  const mxArray *c1_b_y = NULL;
  uint8_T c1_hoistedGlobal;
  uint8_T c1_b_u;
  const mxArray *c1_c_y = NULL;
  real_T (*c1_v1)[6];
  c1_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  c1_st = NULL;
  c1_st = NULL;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_createcellarray(2), FALSE);
  for (c1_i0 = 0; c1_i0 < 6; c1_i0++) {
    c1_u[c1_i0] = (*c1_v1)[c1_i0];
  }

  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 1, 6), FALSE);
  sf_mex_setcell(c1_y, 0, c1_b_y);
  c1_hoistedGlobal = chartInstance->c1_is_active_c1_BoatLibraryTest;
  c1_b_u = c1_hoistedGlobal;
  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", &c1_b_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c1_y, 1, c1_c_y);
  sf_mex_assign(&c1_st, c1_y, FALSE);
  return c1_st;
}

static void set_sim_state_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct *
  chartInstance, const mxArray *c1_st)
{
  const mxArray *c1_u;
  real_T c1_dv0[6];
  int32_T c1_i1;
  real_T (*c1_v1)[6];
  c1_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c1_doneDoubleBufferReInit = TRUE;
  c1_u = sf_mex_dup(c1_st);
  c1_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 0)), "v1",
                      c1_dv0);
  for (c1_i1 = 0; c1_i1 < 6; c1_i1++) {
    (*c1_v1)[c1_i1] = c1_dv0[c1_i1];
  }

  chartInstance->c1_is_active_c1_BoatLibraryTest = c1_e_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 1)),
     "is_active_c1_BoatLibraryTest");
  sf_mex_destroy(&c1_u);
  c1_update_debugger_state_c1_BoatLibraryTest(chartInstance);
  sf_mex_destroy(&c1_st);
}

static void finalize_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance)
{
}

static void sf_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance)
{
  int32_T c1_i2;
  int32_T c1_i3;
  int32_T c1_i4;
  real_T *c1_rx1;
  real_T *c1_rx2;
  real_T *c1_ry1;
  real_T *c1_ry2;
  real_T *c1_m;
  real_T *c1_I;
  real_T (*c1_u0)[4];
  real_T (*c1_v1)[6];
  real_T (*c1_v0)[6];
  c1_I = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c1_m = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
  c1_ry2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 5);
  c1_ry1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c1_rx2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c1_rx1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c1_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 1);
  c1_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  c1_v0 = (real_T (*)[6])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  for (c1_i2 = 0; c1_i2 < 6; c1_i2++) {
    _SFD_DATA_RANGE_CHECK((*c1_v0)[c1_i2], 0U);
  }

  for (c1_i3 = 0; c1_i3 < 6; c1_i3++) {
    _SFD_DATA_RANGE_CHECK((*c1_v1)[c1_i3], 1U);
  }

  for (c1_i4 = 0; c1_i4 < 4; c1_i4++) {
    _SFD_DATA_RANGE_CHECK((*c1_u0)[c1_i4], 2U);
  }

  _SFD_DATA_RANGE_CHECK(*c1_rx1, 3U);
  _SFD_DATA_RANGE_CHECK(*c1_rx2, 4U);
  _SFD_DATA_RANGE_CHECK(*c1_ry1, 5U);
  _SFD_DATA_RANGE_CHECK(*c1_ry2, 6U);
  _SFD_DATA_RANGE_CHECK(*c1_m, 7U);
  _SFD_DATA_RANGE_CHECK(*c1_I, 8U);
  chartInstance->c1_sfEvent = CALL_EVENT;
  c1_chartstep_c1_BoatLibraryTest(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_BoatLibraryTestMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void c1_chartstep_c1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance)
{
  real_T c1_hoistedGlobal;
  real_T c1_b_hoistedGlobal;
  real_T c1_c_hoistedGlobal;
  real_T c1_d_hoistedGlobal;
  real_T c1_e_hoistedGlobal;
  real_T c1_f_hoistedGlobal;
  int32_T c1_i5;
  real_T c1_v0[6];
  int32_T c1_i6;
  real_T c1_u0[4];
  real_T c1_rx1;
  real_T c1_rx2;
  real_T c1_ry1;
  real_T c1_ry2;
  real_T c1_m;
  real_T c1_I;
  uint32_T c1_debug_family_var_map[17];
  real_T c1_v11;
  real_T c1_v12;
  real_T c1_v13;
  real_T c1_v14;
  real_T c1_v15;
  real_T c1_v16;
  real_T c1_nargin = 8.0;
  real_T c1_nargout = 1.0;
  real_T c1_v1[6];
  real_T c1_A;
  real_T c1_B;
  real_T c1_x;
  real_T c1_y;
  real_T c1_b_x;
  real_T c1_b_y;
  real_T c1_c_y;
  real_T c1_a;
  real_T c1_b;
  real_T c1_d_y;
  real_T c1_b_A;
  real_T c1_b_B;
  real_T c1_c_x;
  real_T c1_e_y;
  real_T c1_d_x;
  real_T c1_f_y;
  real_T c1_g_y;
  real_T c1_b_a;
  real_T c1_b_b;
  real_T c1_h_y;
  real_T c1_c_b;
  real_T c1_i_y;
  real_T c1_c_A;
  real_T c1_c_B;
  real_T c1_e_x;
  real_T c1_j_y;
  real_T c1_f_x;
  real_T c1_k_y;
  real_T c1_l_y;
  real_T c1_c_a;
  real_T c1_d_b;
  real_T c1_m_y;
  real_T c1_d_A;
  real_T c1_d_B;
  real_T c1_g_x;
  real_T c1_n_y;
  real_T c1_h_x;
  real_T c1_o_y;
  real_T c1_p_y;
  real_T c1_d_a;
  real_T c1_e_b;
  real_T c1_q_y;
  real_T c1_f_b;
  real_T c1_r_y;
  real_T c1_e_A;
  real_T c1_e_B;
  real_T c1_i_x;
  real_T c1_s_y;
  real_T c1_j_x;
  real_T c1_t_y;
  real_T c1_u_y;
  real_T c1_e_a;
  real_T c1_g_b;
  real_T c1_v_y;
  real_T c1_f_a;
  real_T c1_h_b;
  real_T c1_w_y;
  real_T c1_f_A;
  real_T c1_f_B;
  real_T c1_k_x;
  real_T c1_x_y;
  real_T c1_l_x;
  real_T c1_y_y;
  real_T c1_ab_y;
  real_T c1_g_a;
  real_T c1_i_b;
  real_T c1_bb_y;
  real_T c1_h_a;
  real_T c1_j_b;
  real_T c1_cb_y;
  real_T c1_g_A;
  real_T c1_g_B;
  real_T c1_m_x;
  real_T c1_db_y;
  real_T c1_n_x;
  real_T c1_eb_y;
  real_T c1_fb_y;
  real_T c1_i_a;
  real_T c1_k_b;
  real_T c1_gb_y;
  real_T c1_j_a;
  real_T c1_l_b;
  real_T c1_hb_y;
  real_T c1_h_A;
  real_T c1_h_B;
  real_T c1_o_x;
  real_T c1_ib_y;
  real_T c1_p_x;
  real_T c1_jb_y;
  real_T c1_kb_y;
  real_T c1_k_a;
  real_T c1_m_b;
  real_T c1_lb_y;
  real_T c1_l_a;
  real_T c1_n_b;
  real_T c1_mb_y;
  real_T c1_o_b;
  real_T c1_nb_y;
  int32_T c1_i7;
  real_T *c1_b_I;
  real_T *c1_b_m;
  real_T *c1_b_ry2;
  real_T *c1_b_ry1;
  real_T *c1_b_rx2;
  real_T *c1_b_rx1;
  real_T (*c1_b_v1)[6];
  real_T (*c1_b_u0)[4];
  real_T (*c1_b_v0)[6];
  c1_b_I = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c1_b_m = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
  c1_b_ry2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 5);
  c1_b_ry1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c1_b_rx2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c1_b_rx1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c1_b_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 1);
  c1_b_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  c1_b_v0 = (real_T (*)[6])ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  c1_hoistedGlobal = *c1_b_rx1;
  c1_b_hoistedGlobal = *c1_b_rx2;
  c1_c_hoistedGlobal = *c1_b_ry1;
  c1_d_hoistedGlobal = *c1_b_ry2;
  c1_e_hoistedGlobal = *c1_b_m;
  c1_f_hoistedGlobal = *c1_b_I;
  for (c1_i5 = 0; c1_i5 < 6; c1_i5++) {
    c1_v0[c1_i5] = (*c1_b_v0)[c1_i5];
  }

  for (c1_i6 = 0; c1_i6 < 4; c1_i6++) {
    c1_u0[c1_i6] = (*c1_b_u0)[c1_i6];
  }

  c1_rx1 = c1_hoistedGlobal;
  c1_rx2 = c1_b_hoistedGlobal;
  c1_ry1 = c1_c_hoistedGlobal;
  c1_ry2 = c1_d_hoistedGlobal;
  c1_m = c1_e_hoistedGlobal;
  c1_I = c1_f_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 17U, 17U, c1_debug_family_names,
    c1_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_v11, 0U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_v12, 1U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_v13, 2U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_v14, 3U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_v15, 4U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_v16, 5U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargin, 6U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargout, 7U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c1_v0, 8U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c1_u0, 9U, c1_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_rx1, 10U, c1_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_rx2, 11U, c1_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_ry1, 12U, c1_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_ry2, 13U, c1_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_m, 14U, c1_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_I, 15U, c1_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_v1, 16U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 12);
  c1_v11 = c1_v0[1];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 13);
  c1_A = -c1_u0[1];
  c1_B = c1_m;
  c1_x = c1_A;
  c1_y = c1_B;
  c1_b_x = c1_x;
  c1_b_y = c1_y;
  c1_c_y = c1_b_x / c1_b_y;
  c1_a = c1_c_y;
  c1_b = c1_u0[0] + c1_v0[4];
  c1_b_sind(chartInstance, &c1_b);
  c1_d_y = c1_a * c1_b;
  c1_b_A = c1_u0[3];
  c1_b_B = c1_m;
  c1_c_x = c1_b_A;
  c1_e_y = c1_b_B;
  c1_d_x = c1_c_x;
  c1_f_y = c1_e_y;
  c1_g_y = c1_d_x / c1_f_y;
  c1_b_a = c1_g_y;
  c1_b_b = c1_u0[2] + c1_v0[4];
  c1_b_sind(chartInstance, &c1_b_b);
  c1_h_y = c1_b_a * c1_b_b;
  c1_c_b = c1_v0[0];
  c1_i_y = 0.0 * c1_c_b;
  c1_v12 = (c1_d_y - c1_h_y) - c1_i_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 14);
  c1_v13 = c1_v0[3];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 15);
  c1_c_A = c1_u0[1];
  c1_c_B = c1_m;
  c1_e_x = c1_c_A;
  c1_j_y = c1_c_B;
  c1_f_x = c1_e_x;
  c1_k_y = c1_j_y;
  c1_l_y = c1_f_x / c1_k_y;
  c1_c_a = c1_l_y;
  c1_d_b = c1_u0[0] + c1_v0[4];
  c1_b_cosd(chartInstance, &c1_d_b);
  c1_m_y = c1_c_a * c1_d_b;
  c1_d_A = c1_u0[3];
  c1_d_B = c1_m;
  c1_g_x = c1_d_A;
  c1_n_y = c1_d_B;
  c1_h_x = c1_g_x;
  c1_o_y = c1_n_y;
  c1_p_y = c1_h_x / c1_o_y;
  c1_d_a = c1_p_y;
  c1_e_b = c1_u0[2] + c1_v0[4];
  c1_b_cosd(chartInstance, &c1_e_b);
  c1_q_y = c1_d_a * c1_e_b;
  c1_f_b = c1_v0[2];
  c1_r_y = 0.0 * c1_f_b;
  c1_v14 = (c1_m_y + c1_q_y) - c1_r_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 16);
  c1_v15 = c1_v0[5];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 17);
  c1_e_A = c1_rx1;
  c1_e_B = c1_I;
  c1_i_x = c1_e_A;
  c1_s_y = c1_e_B;
  c1_j_x = c1_i_x;
  c1_t_y = c1_s_y;
  c1_u_y = c1_j_x / c1_t_y;
  c1_e_a = c1_u_y;
  c1_g_b = c1_u0[1];
  c1_v_y = c1_e_a * c1_g_b;
  c1_f_a = c1_v_y;
  c1_h_b = c1_u0[0];
  c1_b_cosd(chartInstance, &c1_h_b);
  c1_w_y = c1_f_a * c1_h_b;
  c1_f_A = c1_ry1;
  c1_f_B = c1_I;
  c1_k_x = c1_f_A;
  c1_x_y = c1_f_B;
  c1_l_x = c1_k_x;
  c1_y_y = c1_x_y;
  c1_ab_y = c1_l_x / c1_y_y;
  c1_g_a = c1_ab_y;
  c1_i_b = c1_u0[1];
  c1_bb_y = c1_g_a * c1_i_b;
  c1_h_a = c1_bb_y;
  c1_j_b = c1_u0[0];
  c1_b_sind(chartInstance, &c1_j_b);
  c1_cb_y = c1_h_a * c1_j_b;
  c1_g_A = c1_rx2;
  c1_g_B = c1_I;
  c1_m_x = c1_g_A;
  c1_db_y = c1_g_B;
  c1_n_x = c1_m_x;
  c1_eb_y = c1_db_y;
  c1_fb_y = c1_n_x / c1_eb_y;
  c1_i_a = c1_fb_y;
  c1_k_b = c1_u0[3];
  c1_gb_y = c1_i_a * c1_k_b;
  c1_j_a = c1_gb_y;
  c1_l_b = c1_u0[2];
  c1_b_cosd(chartInstance, &c1_l_b);
  c1_hb_y = c1_j_a * c1_l_b;
  c1_h_A = c1_ry2;
  c1_h_B = c1_I;
  c1_o_x = c1_h_A;
  c1_ib_y = c1_h_B;
  c1_p_x = c1_o_x;
  c1_jb_y = c1_ib_y;
  c1_kb_y = c1_p_x / c1_jb_y;
  c1_k_a = c1_kb_y;
  c1_m_b = c1_u0[3];
  c1_lb_y = c1_k_a * c1_m_b;
  c1_l_a = c1_lb_y;
  c1_n_b = c1_u0[2];
  c1_b_sind(chartInstance, &c1_n_b);
  c1_mb_y = c1_l_a * c1_n_b;
  c1_o_b = c1_v0[4];
  c1_nb_y = 0.0 * c1_o_b;
  c1_v16 = (((c1_w_y + c1_cb_y) + c1_hb_y) + c1_mb_y) - c1_nb_y;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 19);
  c1_v1[0] = c1_v11;
  c1_v1[1] = c1_v12;
  c1_v1[2] = c1_v13;
  c1_v1[3] = c1_v14;
  c1_v1[4] = c1_v15;
  c1_v1[5] = c1_v16;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, -19);
  _SFD_SYMBOL_SCOPE_POP();
  for (c1_i7 = 0; c1_i7 < 6; c1_i7++) {
    (*c1_b_v1)[c1_i7] = c1_v1[c1_i7];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
}

static void initSimStructsc1_BoatLibraryTest(SFc1_BoatLibraryTestInstanceStruct *
  chartInstance)
{
}

static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber)
{
}

static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i8;
  real_T c1_b_inData[6];
  int32_T c1_i9;
  real_T c1_u[6];
  const mxArray *c1_y = NULL;
  SFc1_BoatLibraryTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_BoatLibraryTestInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  for (c1_i8 = 0; c1_i8 < 6; c1_i8++) {
    c1_b_inData[c1_i8] = (*(real_T (*)[6])c1_inData)[c1_i8];
  }

  for (c1_i9 = 0; c1_i9 < 6; c1_i9++) {
    c1_u[c1_i9] = c1_b_inData[c1_i9];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 1, 6), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static void c1_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_v1, const char_T *c1_identifier, real_T
  c1_y[6])
{
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_v1), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_v1);
}

static void c1_b_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[6])
{
  real_T c1_dv1[6];
  int32_T c1_i10;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), c1_dv1, 1, 0, 0U, 1, 0U, 1, 6);
  for (c1_i10 = 0; c1_i10 < 6; c1_i10++) {
    c1_y[c1_i10] = c1_dv1[c1_i10];
  }

  sf_mex_destroy(&c1_u);
}

static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_v1;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[6];
  int32_T c1_i11;
  SFc1_BoatLibraryTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_BoatLibraryTestInstanceStruct *)chartInstanceVoid;
  c1_v1 = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_v1), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_v1);
  for (c1_i11 = 0; c1_i11 < 6; c1_i11++) {
    (*(real_T (*)[6])c1_outData)[c1_i11] = c1_y[c1_i11];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  real_T c1_u;
  const mxArray *c1_y = NULL;
  SFc1_BoatLibraryTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_BoatLibraryTestInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_u = *(real_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i12;
  real_T c1_b_inData[4];
  int32_T c1_i13;
  real_T c1_u[4];
  const mxArray *c1_y = NULL;
  SFc1_BoatLibraryTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_BoatLibraryTestInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  for (c1_i12 = 0; c1_i12 < 4; c1_i12++) {
    c1_b_inData[c1_i12] = (*(real_T (*)[4])c1_inData)[c1_i12];
  }

  for (c1_i13 = 0; c1_i13 < 4; c1_i13++) {
    c1_u[c1_i13] = c1_b_inData[c1_i13];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static real_T c1_c_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  real_T c1_y;
  real_T c1_d0;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_d0, 1, 0, 0U, 0, 0U, 0);
  c1_y = c1_d0;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_nargout;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y;
  SFc1_BoatLibraryTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_BoatLibraryTestInstanceStruct *)chartInstanceVoid;
  c1_nargout = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_nargout), &c1_thisId);
  sf_mex_destroy(&c1_nargout);
  *(real_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

const mxArray *sf_c1_BoatLibraryTest_get_eml_resolved_functions_info(void)
{
  const mxArray *c1_nameCaptureInfo = NULL;
  c1_nameCaptureInfo = NULL;
  sf_mex_assign(&c1_nameCaptureInfo, sf_mex_createstruct("structure", 2, 29, 1),
                FALSE);
  c1_info_helper(&c1_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c1_nameCaptureInfo);
  return c1_nameCaptureInfo;
}

static void c1_info_helper(const mxArray **c1_info)
{
  const mxArray *c1_rhs0 = NULL;
  const mxArray *c1_lhs0 = NULL;
  const mxArray *c1_rhs1 = NULL;
  const mxArray *c1_lhs1 = NULL;
  const mxArray *c1_rhs2 = NULL;
  const mxArray *c1_lhs2 = NULL;
  const mxArray *c1_rhs3 = NULL;
  const mxArray *c1_lhs3 = NULL;
  const mxArray *c1_rhs4 = NULL;
  const mxArray *c1_lhs4 = NULL;
  const mxArray *c1_rhs5 = NULL;
  const mxArray *c1_lhs5 = NULL;
  const mxArray *c1_rhs6 = NULL;
  const mxArray *c1_lhs6 = NULL;
  const mxArray *c1_rhs7 = NULL;
  const mxArray *c1_lhs7 = NULL;
  const mxArray *c1_rhs8 = NULL;
  const mxArray *c1_lhs8 = NULL;
  const mxArray *c1_rhs9 = NULL;
  const mxArray *c1_lhs9 = NULL;
  const mxArray *c1_rhs10 = NULL;
  const mxArray *c1_lhs10 = NULL;
  const mxArray *c1_rhs11 = NULL;
  const mxArray *c1_lhs11 = NULL;
  const mxArray *c1_rhs12 = NULL;
  const mxArray *c1_lhs12 = NULL;
  const mxArray *c1_rhs13 = NULL;
  const mxArray *c1_lhs13 = NULL;
  const mxArray *c1_rhs14 = NULL;
  const mxArray *c1_lhs14 = NULL;
  const mxArray *c1_rhs15 = NULL;
  const mxArray *c1_lhs15 = NULL;
  const mxArray *c1_rhs16 = NULL;
  const mxArray *c1_lhs16 = NULL;
  const mxArray *c1_rhs17 = NULL;
  const mxArray *c1_lhs17 = NULL;
  const mxArray *c1_rhs18 = NULL;
  const mxArray *c1_lhs18 = NULL;
  const mxArray *c1_rhs19 = NULL;
  const mxArray *c1_lhs19 = NULL;
  const mxArray *c1_rhs20 = NULL;
  const mxArray *c1_lhs20 = NULL;
  const mxArray *c1_rhs21 = NULL;
  const mxArray *c1_lhs21 = NULL;
  const mxArray *c1_rhs22 = NULL;
  const mxArray *c1_lhs22 = NULL;
  const mxArray *c1_rhs23 = NULL;
  const mxArray *c1_lhs23 = NULL;
  const mxArray *c1_rhs24 = NULL;
  const mxArray *c1_lhs24 = NULL;
  const mxArray *c1_rhs25 = NULL;
  const mxArray *c1_lhs25 = NULL;
  const mxArray *c1_rhs26 = NULL;
  const mxArray *c1_lhs26 = NULL;
  const mxArray *c1_rhs27 = NULL;
  const mxArray *c1_lhs27 = NULL;
  const mxArray *c1_rhs28 = NULL;
  const mxArray *c1_lhs28 = NULL;
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("mrdivide"), "name", "name", 0);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 0);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c1_rhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs0, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs0), "rhs", "rhs", 0);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs0), "lhs", "lhs", 0);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "context",
                  "context", 1);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("rdivide"), "name", "name", 1);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "resolved",
                  "resolved", 1);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731880U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c1_rhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs1, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs1), "rhs", "rhs", 1);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs1), "lhs", "lhs", 1);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 2);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 2);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 2);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c1_rhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs2, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs2), "rhs", "rhs", 2);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs2), "lhs", "lhs", 2);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 3);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_scalexp_compatible"),
                  "name", "name", 3);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_compatible.m"),
                  "resolved", "resolved", 3);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c1_rhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs3, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs3), "rhs", "rhs", 3);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs3), "lhs", "lhs", 3);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m"), "context",
                  "context", 4);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_div"), "name", "name", 4);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m"), "resolved",
                  "resolved", 4);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731866U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c1_rhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs4, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs4), "rhs", "rhs", 4);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs4), "lhs", "lhs", 4);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(""), "context", "context", 5);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("sind"), "name", "name", 5);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "resolved",
                  "resolved", 5);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1343851986U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c1_rhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs5, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs5), "rhs", "rhs", 5);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs5), "lhs", "lhs", 5);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/sind.m"), "context",
                  "context", 6);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 6);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 6);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c1_rhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs6, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs6), "rhs", "rhs", 6);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs6), "lhs", "lhs", 6);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 7);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("isfinite"), "name", "name", 7);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "resolved",
                  "resolved", 7);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c1_rhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs7, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs7), "rhs", "rhs", 7);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs7), "lhs", "lhs", 7);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 8);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 8);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 8);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c1_rhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs8, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs8), "rhs", "rhs", 8);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs8), "lhs", "lhs", 8);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 9);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("isinf"), "name", "name", 9);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "resolved",
                  "resolved", 9);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731856U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c1_rhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs9, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs9), "rhs", "rhs", 9);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs9), "lhs", "lhs", 9);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isinf.m"), "context",
                  "context", 10);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 10);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 10);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c1_rhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs10, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isfinite.m"), "context",
                  "context", 11);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("isnan"), "name", "name", 11);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "resolved",
                  "resolved", 11);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731858U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c1_rhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs11, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m"), "context",
                  "context", 12);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 12);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 12);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c1_rhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs12, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 13);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_guarded_nan"), "name",
                  "name", 13);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "resolved", "resolved", 13);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1286840376U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c1_rhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs13, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m"),
                  "context", "context", 14);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_is_float_class"), "name",
                  "name", 14);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m"),
                  "resolved", "resolved", 14);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1286840382U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c1_rhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs14, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs14), "lhs", "lhs",
                  14);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "context", "context", 15);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_scalar_rem90"), "name",
                  "name", 15);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 15);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "resolved", "resolved", 15);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 15);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 15);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 15);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 15);
  sf_mex_assign(&c1_rhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs15, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs15), "rhs", "rhs",
                  15);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs15), "lhs", "lhs",
                  15);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 16);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("mrdivide"), "name", "name", 16);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 16);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p"), "resolved",
                  "resolved", 16);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1373328108U), "fileTimeLo",
                  "fileTimeLo", 16);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 16);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1319751566U), "mFileTimeLo",
                  "mFileTimeLo", 16);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 16);
  sf_mex_assign(&c1_rhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs16, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs16), "rhs", "rhs",
                  16);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs16), "lhs", "lhs",
                  16);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 17);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("rem"), "name", "name", 17);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 17);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "resolved",
                  "resolved", 17);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731854U), "fileTimeLo",
                  "fileTimeLo", 17);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 17);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 17);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 17);
  sf_mex_assign(&c1_rhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs17, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs17), "rhs", "rhs",
                  17);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs17), "lhs", "lhs",
                  17);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 18);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 18);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 18);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 18);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 18);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 18);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 18);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 18);
  sf_mex_assign(&c1_rhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs18, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs18), "rhs", "rhs",
                  18);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs18), "lhs", "lhs",
                  18);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 19);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 19);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 19);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 19);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1286840396U), "fileTimeLo",
                  "fileTimeLo", 19);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 19);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 19);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 19);
  sf_mex_assign(&c1_rhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs19, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs19), "rhs", "rhs",
                  19);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs19), "lhs", "lhs",
                  19);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/rem.m"), "context",
                  "context", 20);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_scalexp_alloc"), "name",
                  "name", 20);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 20);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m"),
                  "resolved", "resolved", 20);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1358207740U), "fileTimeLo",
                  "fileTimeLo", 20);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 20);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 20);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 20);
  sf_mex_assign(&c1_rhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs20, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs20), "rhs", "rhs",
                  20);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs20), "lhs", "lhs",
                  20);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 21);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("abs"), "name", "name", 21);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 21);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "resolved",
                  "resolved", 21);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731852U), "fileTimeLo",
                  "fileTimeLo", 21);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 21);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 21);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 21);
  sf_mex_assign(&c1_rhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs21, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs21), "rhs", "rhs",
                  21);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs21), "lhs", "lhs",
                  21);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 22);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 22);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 22);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 22);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 22);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 22);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 22);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 22);
  sf_mex_assign(&c1_rhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs22, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs22), "rhs", "rhs",
                  22);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs22), "lhs", "lhs",
                  22);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m"), "context",
                  "context", 23);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_scalar_abs"), "name",
                  "name", 23);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 23);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m"),
                  "resolved", "resolved", 23);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1286840312U), "fileTimeLo",
                  "fileTimeLo", 23);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 23);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 23);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 23);
  sf_mex_assign(&c1_rhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs23, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs23), "rhs", "rhs",
                  23);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs23), "lhs", "lhs",
                  23);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m"),
                  "context", "context", 24);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("mtimes"), "name", "name", 24);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 24);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 24);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 24);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 24);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 24);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 24);
  sf_mex_assign(&c1_rhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs24, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs24), "rhs", "rhs",
                  24);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs24), "lhs", "lhs",
                  24);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m!common_checks"),
                  "context", "context", 25);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 25);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 25);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 25);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363732556U), "fileTimeLo",
                  "fileTimeLo", 25);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 25);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 25);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 25);
  sf_mex_assign(&c1_rhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs25, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs25), "rhs", "rhs",
                  25);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs25), "lhs", "lhs",
                  25);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(""), "context", "context", 26);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("mtimes"), "name", "name", 26);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 26);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m"), "resolved",
                  "resolved", 26);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1363731878U), "fileTimeLo",
                  "fileTimeLo", 26);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 26);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 26);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 26);
  sf_mex_assign(&c1_rhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs26, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs26), "rhs", "rhs",
                  26);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs26), "lhs", "lhs",
                  26);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(""), "context", "context", 27);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("cosd"), "name", "name", 27);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 27);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "resolved",
                  "resolved", 27);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1343851972U), "fileTimeLo",
                  "fileTimeLo", 27);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 27);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 27);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 27);
  sf_mex_assign(&c1_rhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs27, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs27), "rhs", "rhs",
                  27);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs27), "lhs", "lhs",
                  27);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/cosd.m"), "context",
                  "context", 28);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("eml_scalar_cosd_and_sind"),
                  "name", "name", 28);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 28);
  sf_mex_addfield(*c1_info, c1_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd_and_sind.m"),
                  "resolved", "resolved", 28);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(1343851978U), "fileTimeLo",
                  "fileTimeLo", 28);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 28);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 28);
  sf_mex_addfield(*c1_info, c1_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 28);
  sf_mex_assign(&c1_rhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_assign(&c1_lhs28, sf_mex_createcellarray(0), FALSE);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_rhs28), "rhs", "rhs",
                  28);
  sf_mex_addfield(*c1_info, sf_mex_duplicatearraysafe(&c1_lhs28), "lhs", "lhs",
                  28);
  sf_mex_destroy(&c1_rhs0);
  sf_mex_destroy(&c1_lhs0);
  sf_mex_destroy(&c1_rhs1);
  sf_mex_destroy(&c1_lhs1);
  sf_mex_destroy(&c1_rhs2);
  sf_mex_destroy(&c1_lhs2);
  sf_mex_destroy(&c1_rhs3);
  sf_mex_destroy(&c1_lhs3);
  sf_mex_destroy(&c1_rhs4);
  sf_mex_destroy(&c1_lhs4);
  sf_mex_destroy(&c1_rhs5);
  sf_mex_destroy(&c1_lhs5);
  sf_mex_destroy(&c1_rhs6);
  sf_mex_destroy(&c1_lhs6);
  sf_mex_destroy(&c1_rhs7);
  sf_mex_destroy(&c1_lhs7);
  sf_mex_destroy(&c1_rhs8);
  sf_mex_destroy(&c1_lhs8);
  sf_mex_destroy(&c1_rhs9);
  sf_mex_destroy(&c1_lhs9);
  sf_mex_destroy(&c1_rhs10);
  sf_mex_destroy(&c1_lhs10);
  sf_mex_destroy(&c1_rhs11);
  sf_mex_destroy(&c1_lhs11);
  sf_mex_destroy(&c1_rhs12);
  sf_mex_destroy(&c1_lhs12);
  sf_mex_destroy(&c1_rhs13);
  sf_mex_destroy(&c1_lhs13);
  sf_mex_destroy(&c1_rhs14);
  sf_mex_destroy(&c1_lhs14);
  sf_mex_destroy(&c1_rhs15);
  sf_mex_destroy(&c1_lhs15);
  sf_mex_destroy(&c1_rhs16);
  sf_mex_destroy(&c1_lhs16);
  sf_mex_destroy(&c1_rhs17);
  sf_mex_destroy(&c1_lhs17);
  sf_mex_destroy(&c1_rhs18);
  sf_mex_destroy(&c1_lhs18);
  sf_mex_destroy(&c1_rhs19);
  sf_mex_destroy(&c1_lhs19);
  sf_mex_destroy(&c1_rhs20);
  sf_mex_destroy(&c1_lhs20);
  sf_mex_destroy(&c1_rhs21);
  sf_mex_destroy(&c1_lhs21);
  sf_mex_destroy(&c1_rhs22);
  sf_mex_destroy(&c1_lhs22);
  sf_mex_destroy(&c1_rhs23);
  sf_mex_destroy(&c1_lhs23);
  sf_mex_destroy(&c1_rhs24);
  sf_mex_destroy(&c1_lhs24);
  sf_mex_destroy(&c1_rhs25);
  sf_mex_destroy(&c1_lhs25);
  sf_mex_destroy(&c1_rhs26);
  sf_mex_destroy(&c1_lhs26);
  sf_mex_destroy(&c1_rhs27);
  sf_mex_destroy(&c1_lhs27);
  sf_mex_destroy(&c1_rhs28);
  sf_mex_destroy(&c1_lhs28);
}

static const mxArray *c1_emlrt_marshallOut(char * c1_u)
{
  const mxArray *c1_y = NULL;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 15, 0U, 0U, 0U, 2, 1, strlen
    (c1_u)), FALSE);
  return c1_y;
}

static const mxArray *c1_b_emlrt_marshallOut(uint32_T c1_u)
{
  const mxArray *c1_y = NULL;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 7, 0U, 0U, 0U, 0), FALSE);
  return c1_y;
}

static real_T c1_sind(SFc1_BoatLibraryTestInstanceStruct *chartInstance, real_T
                      c1_x)
{
  real_T c1_b_x;
  c1_b_x = c1_x;
  c1_b_sind(chartInstance, &c1_b_x);
  return c1_b_x;
}

static boolean_T c1_isfinite(SFc1_BoatLibraryTestInstanceStruct *chartInstance,
  real_T c1_x)
{
  real_T c1_b_x;
  boolean_T c1_b_b;
  boolean_T c1_b0;
  real_T c1_c_x;
  boolean_T c1_c_b;
  boolean_T c1_b1;
  c1_b_x = c1_x;
  c1_b_b = muDoubleScalarIsInf(c1_b_x);
  c1_b0 = !c1_b_b;
  c1_c_x = c1_x;
  c1_c_b = muDoubleScalarIsNaN(c1_c_x);
  c1_b1 = !c1_c_b;
  return c1_b0 && c1_b1;
}

static real_T c1_cosd(SFc1_BoatLibraryTestInstanceStruct *chartInstance, real_T
                      c1_x)
{
  real_T c1_b_x;
  c1_b_x = c1_x;
  c1_b_cosd(chartInstance, &c1_b_x);
  return c1_b_x;
}

static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_u;
  const mxArray *c1_y = NULL;
  SFc1_BoatLibraryTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_BoatLibraryTestInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_u = *(int32_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static int32_T c1_d_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  int32_T c1_y;
  int32_T c1_i14;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_i14, 1, 6, 0U, 0, 0U, 0);
  c1_y = c1_i14;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_sfEvent;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  int32_T c1_y;
  SFc1_BoatLibraryTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_BoatLibraryTestInstanceStruct *)chartInstanceVoid;
  c1_b_sfEvent = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_sfEvent),
    &c1_thisId);
  sf_mex_destroy(&c1_b_sfEvent);
  *(int32_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static uint8_T c1_e_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_b_is_active_c1_BoatLibraryTest, const char_T
  *c1_identifier)
{
  uint8_T c1_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_f_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c1_b_is_active_c1_BoatLibraryTest), &c1_thisId);
  sf_mex_destroy(&c1_b_is_active_c1_BoatLibraryTest);
  return c1_y;
}

static uint8_T c1_f_emlrt_marshallIn(SFc1_BoatLibraryTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  uint8_T c1_y;
  uint8_T c1_u0;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_u0, 1, 3, 0U, 0, 0U, 0);
  c1_y = c1_u0;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_b_sind(SFc1_BoatLibraryTestInstanceStruct *chartInstance, real_T *
                      c1_x)
{
  real_T c1_b_x;
  real_T c1_d1;
  real_T c1_c_x;
  real_T c1_d_x;
  real_T c1_e_x;
  real_T c1_xk;
  real_T c1_f_x;
  real_T c1_g_x;
  real_T c1_absx;
  real_T c1_h_x;
  real_T c1_i_x;
  real_T c1_b;
  int8_T c1_n;
  real_T c1_b_b;
  real_T c1_c_b;
  real_T c1_d_b;
  real_T c1_e_b;
  int8_T c1_b_n;
  c1_b_x = *c1_x;
  if (!c1_isfinite(chartInstance, c1_b_x)) {
    c1_d1 = rtNaN;
  } else {
    c1_c_x = c1_b_x;
    c1_d_x = c1_c_x;
    c1_e_x = c1_d_x;
    c1_xk = c1_e_x;
    c1_d_x = muDoubleScalarRem(c1_xk, 360.0);
    c1_f_x = c1_d_x;
    c1_g_x = c1_f_x;
    c1_absx = muDoubleScalarAbs(c1_g_x);
    if (c1_absx > 180.0) {
      if (c1_d_x > 0.0) {
        c1_d_x -= 360.0;
      } else {
        c1_d_x += 360.0;
      }

      c1_h_x = c1_d_x;
      c1_i_x = c1_h_x;
      c1_absx = muDoubleScalarAbs(c1_i_x);
    }

    if (c1_absx <= 45.0) {
      c1_b = c1_d_x;
      c1_d_x = 0.017453292519943295 * c1_b;
      c1_n = 0;
    } else if (c1_absx <= 135.0) {
      if (c1_d_x > 0.0) {
        c1_b_b = c1_d_x - 90.0;
        c1_d_x = 0.017453292519943295 * c1_b_b;
        c1_n = 1;
      } else {
        c1_c_b = c1_d_x + 90.0;
        c1_d_x = 0.017453292519943295 * c1_c_b;
        c1_n = -1;
      }
    } else if (c1_d_x > 0.0) {
      c1_d_b = c1_d_x - 180.0;
      c1_d_x = 0.017453292519943295 * c1_d_b;
      c1_n = 2;
    } else {
      c1_e_b = c1_d_x + 180.0;
      c1_d_x = 0.017453292519943295 * c1_e_b;
      c1_n = -2;
    }

    c1_b_n = c1_n;
    c1_b_x = c1_d_x;
    if ((real_T)c1_b_n == 0.0) {
      c1_d1 = muDoubleScalarSin(c1_b_x);
    } else if ((real_T)c1_b_n == 1.0) {
      c1_d1 = muDoubleScalarCos(c1_b_x);
    } else if ((real_T)c1_b_n == -1.0) {
      c1_d1 = -muDoubleScalarCos(c1_b_x);
    } else {
      c1_d1 = -muDoubleScalarSin(c1_b_x);
    }
  }

  *c1_x = c1_d1;
}

static void c1_b_cosd(SFc1_BoatLibraryTestInstanceStruct *chartInstance, real_T *
                      c1_x)
{
  real_T c1_b_x;
  real_T c1_c_x;
  real_T c1_d_x;
  real_T c1_e_x;
  real_T c1_xk;
  real_T c1_f_x;
  real_T c1_g_x;
  real_T c1_absx;
  real_T c1_h_x;
  real_T c1_i_x;
  real_T c1_b;
  int8_T c1_n;
  real_T c1_b_b;
  real_T c1_c_b;
  real_T c1_d_b;
  real_T c1_e_b;
  int8_T c1_b_n;
  c1_b_x = *c1_x;
  if (!c1_isfinite(chartInstance, c1_b_x)) {
    *c1_x = rtNaN;
  } else {
    c1_c_x = c1_b_x;
    c1_d_x = c1_c_x;
    c1_e_x = c1_d_x;
    c1_xk = c1_e_x;
    c1_d_x = muDoubleScalarRem(c1_xk, 360.0);
    c1_f_x = c1_d_x;
    c1_g_x = c1_f_x;
    c1_absx = muDoubleScalarAbs(c1_g_x);
    if (c1_absx > 180.0) {
      if (c1_d_x > 0.0) {
        c1_d_x -= 360.0;
      } else {
        c1_d_x += 360.0;
      }

      c1_h_x = c1_d_x;
      c1_i_x = c1_h_x;
      c1_absx = muDoubleScalarAbs(c1_i_x);
    }

    if (c1_absx <= 45.0) {
      c1_b = c1_d_x;
      c1_d_x = 0.017453292519943295 * c1_b;
      c1_n = 0;
    } else if (c1_absx <= 135.0) {
      if (c1_d_x > 0.0) {
        c1_b_b = c1_d_x - 90.0;
        c1_d_x = 0.017453292519943295 * c1_b_b;
        c1_n = 1;
      } else {
        c1_c_b = c1_d_x + 90.0;
        c1_d_x = 0.017453292519943295 * c1_c_b;
        c1_n = -1;
      }
    } else if (c1_d_x > 0.0) {
      c1_d_b = c1_d_x - 180.0;
      c1_d_x = 0.017453292519943295 * c1_d_b;
      c1_n = 2;
    } else {
      c1_e_b = c1_d_x + 180.0;
      c1_d_x = 0.017453292519943295 * c1_e_b;
      c1_n = -2;
    }

    c1_b_n = c1_n;
    c1_b_x = c1_d_x;
    if ((real_T)c1_b_n == 0.0) {
      *c1_x = muDoubleScalarCos(c1_b_x);
    } else if ((real_T)c1_b_n == 1.0) {
      *c1_x = -muDoubleScalarSin(c1_b_x);
    } else if ((real_T)c1_b_n == -1.0) {
      *c1_x = muDoubleScalarSin(c1_b_x);
    } else {
      *c1_x = -muDoubleScalarCos(c1_b_x);
    }
  }
}

static void init_dsm_address_info(SFc1_BoatLibraryTestInstanceStruct
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

void sf_c1_BoatLibraryTest_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2889719341U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(3117435410U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1402298953U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(3312534131U);
}

mxArray *sf_c1_BoatLibraryTest_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("LwBSOFHVUw2CZNJad6rVgB");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,8,3,dataFields);

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
      pr[0] = (double)(4);
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
      pr[0] = (double)(1);
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
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c1_BoatLibraryTest_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c1_BoatLibraryTest_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c1_BoatLibraryTest(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"v1\",},{M[8],M[0],T\"is_active_c1_BoatLibraryTest\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c1_BoatLibraryTest_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc1_BoatLibraryTestInstanceStruct *chartInstance;
    chartInstance = (SFc1_BoatLibraryTestInstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _BoatLibraryTestMachineNumber_,
           1,
           1,
           1,
           9,
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
          init_script_number_translation(_BoatLibraryTestMachineNumber_,
            chartInstance->chartNumber);
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_BoatLibraryTestMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _BoatLibraryTestMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"v0");
          _SFD_SET_DATA_PROPS(1,2,0,1,"v1");
          _SFD_SET_DATA_PROPS(2,1,1,0,"u0");
          _SFD_SET_DATA_PROPS(3,1,1,0,"rx1");
          _SFD_SET_DATA_PROPS(4,1,1,0,"rx2");
          _SFD_SET_DATA_PROPS(5,1,1,0,"ry1");
          _SFD_SET_DATA_PROPS(6,1,1,0,"ry2");
          _SFD_SET_DATA_PROPS(7,1,1,0,"m");
          _SFD_SET_DATA_PROPS(8,1,1,0,"I");
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
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,877);

        {
          unsigned int dimVector[1];
          dimVector[0]= 6;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 6;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)
            c1_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_c_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)NULL);

        {
          real_T *c1_rx1;
          real_T *c1_rx2;
          real_T *c1_ry1;
          real_T *c1_ry2;
          real_T *c1_m;
          real_T *c1_I;
          real_T (*c1_v0)[6];
          real_T (*c1_v1)[6];
          real_T (*c1_u0)[4];
          c1_I = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
          c1_m = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
          c1_ry2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 5);
          c1_ry1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
          c1_rx2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c1_rx1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c1_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 1);
          c1_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
          c1_v0 = (real_T (*)[6])ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, *c1_v0);
          _SFD_SET_DATA_VALUE_PTR(1U, *c1_v1);
          _SFD_SET_DATA_VALUE_PTR(2U, *c1_u0);
          _SFD_SET_DATA_VALUE_PTR(3U, c1_rx1);
          _SFD_SET_DATA_VALUE_PTR(4U, c1_rx2);
          _SFD_SET_DATA_VALUE_PTR(5U, c1_ry1);
          _SFD_SET_DATA_VALUE_PTR(6U, c1_ry2);
          _SFD_SET_DATA_VALUE_PTR(7U, c1_m);
          _SFD_SET_DATA_VALUE_PTR(8U, c1_I);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _BoatLibraryTestMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "dIl1gPLmURJUV5mMbWa3oE";
}

static void sf_opaque_initialize_c1_BoatLibraryTest(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc1_BoatLibraryTestInstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*)
    chartInstanceVar);
  initialize_c1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c1_BoatLibraryTest(void *chartInstanceVar)
{
  enable_c1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c1_BoatLibraryTest(void *chartInstanceVar)
{
  disable_c1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c1_BoatLibraryTest(void *chartInstanceVar)
{
  sf_c1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*) chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c1_BoatLibraryTest(SimStruct* S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c1_BoatLibraryTest
    ((SFc1_BoatLibraryTestInstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c1_BoatLibraryTest();/* state var info */
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

extern void sf_internal_set_sim_state_c1_BoatLibraryTest(SimStruct* S, const
  mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c1_BoatLibraryTest();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c1_BoatLibraryTest(SimStruct* S)
{
  return sf_internal_get_sim_state_c1_BoatLibraryTest(S);
}

static void sf_opaque_set_sim_state_c1_BoatLibraryTest(SimStruct* S, const
  mxArray *st)
{
  sf_internal_set_sim_state_c1_BoatLibraryTest(S, st);
}

static void sf_opaque_terminate_c1_BoatLibraryTest(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc1_BoatLibraryTestInstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_BoatLibraryTest_optimization_info();
    }

    finalize_c1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*)
      chartInstanceVar);
    utFree((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c1_BoatLibraryTest(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c1_BoatLibraryTest((SFc1_BoatLibraryTestInstanceStruct*)
      (((ChartInfoStruct *)ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c1_BoatLibraryTest(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_BoatLibraryTest_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      1);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,1,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,1,
      "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(S,sf_get_instance_specialization(),infoStruct,1);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 5, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 6, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 7, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,1,8);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,1,1);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=1; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 8; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,1);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(342122646U));
  ssSetChecksum1(S,(2394978379U));
  ssSetChecksum2(S,(347728521U));
  ssSetChecksum3(S,(475936731U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c1_BoatLibraryTest(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c1_BoatLibraryTest(SimStruct *S)
{
  SFc1_BoatLibraryTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_BoatLibraryTestInstanceStruct *)utMalloc(sizeof
    (SFc1_BoatLibraryTestInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc1_BoatLibraryTestInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c1_BoatLibraryTest;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c1_BoatLibraryTest;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c1_BoatLibraryTest;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c1_BoatLibraryTest;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c1_BoatLibraryTest;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c1_BoatLibraryTest;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c1_BoatLibraryTest;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c1_BoatLibraryTest;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c1_BoatLibraryTest;
  chartInstance->chartInfo.mdlStart = mdlStart_c1_BoatLibraryTest;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c1_BoatLibraryTest;
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

void c1_BoatLibraryTest_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c1_BoatLibraryTest(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c1_BoatLibraryTest(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c1_BoatLibraryTest(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c1_BoatLibraryTest_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
