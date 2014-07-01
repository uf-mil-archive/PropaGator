/* Include files */

#include <stddef.h>
#include "blas.h"
#include "systemTest_sfun.h"
#include "c1_systemTest.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "systemTest_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c1_debug_family_names[23] = { "nargin", "nargout",
  "x_current", "x_desired", "x_velocity_current", "x_velocity_desired",
  "p_gain_x", "d_gain_x", "y_current", "y_desired", "y_velocity_current",
  "y_velocity_desired", "p_gain_y", "d_gain_y", "yaw_current", "yaw_desired",
  "yaw_velocity_current", "yaw_velocity_desired", "p_gain_yaw", "d_gain_yaw",
  "requiredForceX", "requiredForceY", "requiredMomentZ" };

/* Function Declarations */
static void initialize_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance);
static void initialize_params_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance);
static void enable_c1_systemTest(SFc1_systemTestInstanceStruct *chartInstance);
static void disable_c1_systemTest(SFc1_systemTestInstanceStruct *chartInstance);
static void c1_update_debugger_state_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c1_systemTest(SFc1_systemTestInstanceStruct *
  chartInstance);
static void set_sim_state_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance, const mxArray *c1_st);
static void finalize_c1_systemTest(SFc1_systemTestInstanceStruct *chartInstance);
static void sf_gateway_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance);
static void initSimStructsc1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance);
static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber, uint32_T c1_instanceNumber);
static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData);
static real_T c1_emlrt_marshallIn(SFc1_systemTestInstanceStruct *chartInstance,
  const mxArray *c1_requiredMomentZ, const char_T *c1_identifier);
static real_T c1_b_emlrt_marshallIn(SFc1_systemTestInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static int32_T c1_c_emlrt_marshallIn(SFc1_systemTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static uint8_T c1_d_emlrt_marshallIn(SFc1_systemTestInstanceStruct
  *chartInstance, const mxArray *c1_b_is_active_c1_systemTest, const char_T
  *c1_identifier);
static uint8_T c1_e_emlrt_marshallIn(SFc1_systemTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void init_dsm_address_info(SFc1_systemTestInstanceStruct *chartInstance);

/* Function Definitions */
static void initialize_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance)
{
  chartInstance->c1_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c1_is_active_c1_systemTest = 0U;
}

static void initialize_params_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void enable_c1_systemTest(SFc1_systemTestInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c1_systemTest(SFc1_systemTestInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c1_update_debugger_state_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c1_systemTest(SFc1_systemTestInstanceStruct *
  chartInstance)
{
  const mxArray *c1_st;
  const mxArray *c1_y = NULL;
  real_T c1_hoistedGlobal;
  real_T c1_u;
  const mxArray *c1_b_y = NULL;
  real_T c1_b_hoistedGlobal;
  real_T c1_b_u;
  const mxArray *c1_c_y = NULL;
  real_T c1_c_hoistedGlobal;
  real_T c1_c_u;
  const mxArray *c1_d_y = NULL;
  uint8_T c1_d_hoistedGlobal;
  uint8_T c1_d_u;
  const mxArray *c1_e_y = NULL;
  real_T *c1_requiredForceX;
  real_T *c1_requiredForceY;
  real_T *c1_requiredMomentZ;
  c1_requiredMomentZ = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c1_requiredForceY = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c1_requiredForceX = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c1_st = NULL;
  c1_st = NULL;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_createcellmatrix(4, 1), false);
  c1_hoistedGlobal = *c1_requiredForceX;
  c1_u = c1_hoistedGlobal;
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", &c1_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c1_y, 0, c1_b_y);
  c1_b_hoistedGlobal = *c1_requiredForceY;
  c1_b_u = c1_b_hoistedGlobal;
  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", &c1_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c1_y, 1, c1_c_y);
  c1_c_hoistedGlobal = *c1_requiredMomentZ;
  c1_c_u = c1_c_hoistedGlobal;
  c1_d_y = NULL;
  sf_mex_assign(&c1_d_y, sf_mex_create("y", &c1_c_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c1_y, 2, c1_d_y);
  c1_d_hoistedGlobal = chartInstance->c1_is_active_c1_systemTest;
  c1_d_u = c1_d_hoistedGlobal;
  c1_e_y = NULL;
  sf_mex_assign(&c1_e_y, sf_mex_create("y", &c1_d_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c1_y, 3, c1_e_y);
  sf_mex_assign(&c1_st, c1_y, false);
  return c1_st;
}

static void set_sim_state_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance, const mxArray *c1_st)
{
  const mxArray *c1_u;
  real_T *c1_requiredForceX;
  real_T *c1_requiredForceY;
  real_T *c1_requiredMomentZ;
  c1_requiredMomentZ = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c1_requiredForceY = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c1_requiredForceX = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c1_doneDoubleBufferReInit = true;
  c1_u = sf_mex_dup(c1_st);
  *c1_requiredForceX = c1_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c1_u, 0)), "requiredForceX");
  *c1_requiredForceY = c1_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c1_u, 1)), "requiredForceY");
  *c1_requiredMomentZ = c1_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c1_u, 2)), "requiredMomentZ");
  chartInstance->c1_is_active_c1_systemTest = c1_d_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 3)),
     "is_active_c1_systemTest");
  sf_mex_destroy(&c1_u);
  c1_update_debugger_state_c1_systemTest(chartInstance);
  sf_mex_destroy(&c1_st);
}

static void finalize_c1_systemTest(SFc1_systemTestInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance)
{
  real_T c1_hoistedGlobal;
  real_T c1_b_hoistedGlobal;
  real_T c1_c_hoistedGlobal;
  real_T c1_d_hoistedGlobal;
  real_T c1_e_hoistedGlobal;
  real_T c1_f_hoistedGlobal;
  real_T c1_g_hoistedGlobal;
  real_T c1_h_hoistedGlobal;
  real_T c1_i_hoistedGlobal;
  real_T c1_j_hoistedGlobal;
  real_T c1_k_hoistedGlobal;
  real_T c1_l_hoistedGlobal;
  real_T c1_m_hoistedGlobal;
  real_T c1_n_hoistedGlobal;
  real_T c1_o_hoistedGlobal;
  real_T c1_p_hoistedGlobal;
  real_T c1_q_hoistedGlobal;
  real_T c1_r_hoistedGlobal;
  real_T c1_x_current;
  real_T c1_x_desired;
  real_T c1_x_velocity_current;
  real_T c1_x_velocity_desired;
  real_T c1_p_gain_x;
  real_T c1_d_gain_x;
  real_T c1_y_current;
  real_T c1_y_desired;
  real_T c1_y_velocity_current;
  real_T c1_y_velocity_desired;
  real_T c1_p_gain_y;
  real_T c1_d_gain_y;
  real_T c1_yaw_current;
  real_T c1_yaw_desired;
  real_T c1_yaw_velocity_current;
  real_T c1_yaw_velocity_desired;
  real_T c1_p_gain_yaw;
  real_T c1_d_gain_yaw;
  uint32_T c1_debug_family_var_map[23];
  real_T c1_nargin = 18.0;
  real_T c1_nargout = 3.0;
  real_T c1_requiredForceX;
  real_T c1_requiredForceY;
  real_T c1_requiredMomentZ;
  real_T *c1_b_x_current;
  real_T *c1_b_requiredForceX;
  real_T *c1_b_x_desired;
  real_T *c1_b_x_velocity_current;
  real_T *c1_b_x_velocity_desired;
  real_T *c1_b_p_gain_x;
  real_T *c1_b_d_gain_x;
  real_T *c1_b_y_current;
  real_T *c1_b_y_desired;
  real_T *c1_b_y_velocity_current;
  real_T *c1_b_y_velocity_desired;
  real_T *c1_b_p_gain_y;
  real_T *c1_b_d_gain_y;
  real_T *c1_b_yaw_current;
  real_T *c1_b_yaw_desired;
  real_T *c1_b_yaw_velocity_current;
  real_T *c1_b_yaw_velocity_desired;
  real_T *c1_b_p_gain_yaw;
  real_T *c1_b_d_gain_yaw;
  real_T *c1_b_requiredForceY;
  real_T *c1_b_requiredMomentZ;
  c1_b_requiredMomentZ = (real_T *)ssGetOutputPortSignal(chartInstance->S, 3);
  c1_b_requiredForceY = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c1_b_d_gain_yaw = (real_T *)ssGetInputPortSignal(chartInstance->S, 17);
  c1_b_p_gain_yaw = (real_T *)ssGetInputPortSignal(chartInstance->S, 16);
  c1_b_yaw_velocity_desired = (real_T *)ssGetInputPortSignal(chartInstance->S,
    15);
  c1_b_yaw_velocity_current = (real_T *)ssGetInputPortSignal(chartInstance->S,
    14);
  c1_b_yaw_desired = (real_T *)ssGetInputPortSignal(chartInstance->S, 13);
  c1_b_yaw_current = (real_T *)ssGetInputPortSignal(chartInstance->S, 12);
  c1_b_d_gain_y = (real_T *)ssGetInputPortSignal(chartInstance->S, 11);
  c1_b_p_gain_y = (real_T *)ssGetInputPortSignal(chartInstance->S, 10);
  c1_b_y_velocity_desired = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c1_b_y_velocity_current = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c1_b_y_desired = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c1_b_y_current = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
  c1_b_d_gain_x = (real_T *)ssGetInputPortSignal(chartInstance->S, 5);
  c1_b_p_gain_x = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c1_b_x_velocity_desired = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c1_b_x_velocity_current = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c1_b_x_desired = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c1_b_requiredForceX = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c1_b_x_current = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  _SFD_DATA_RANGE_CHECK(*c1_b_x_current, 0U);
  chartInstance->c1_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  c1_hoistedGlobal = *c1_b_x_current;
  c1_b_hoistedGlobal = *c1_b_x_desired;
  c1_c_hoistedGlobal = *c1_b_x_velocity_current;
  c1_d_hoistedGlobal = *c1_b_x_velocity_desired;
  c1_e_hoistedGlobal = *c1_b_p_gain_x;
  c1_f_hoistedGlobal = *c1_b_d_gain_x;
  c1_g_hoistedGlobal = *c1_b_y_current;
  c1_h_hoistedGlobal = *c1_b_y_desired;
  c1_i_hoistedGlobal = *c1_b_y_velocity_current;
  c1_j_hoistedGlobal = *c1_b_y_velocity_desired;
  c1_k_hoistedGlobal = *c1_b_p_gain_y;
  c1_l_hoistedGlobal = *c1_b_d_gain_y;
  c1_m_hoistedGlobal = *c1_b_yaw_current;
  c1_n_hoistedGlobal = *c1_b_yaw_desired;
  c1_o_hoistedGlobal = *c1_b_yaw_velocity_current;
  c1_p_hoistedGlobal = *c1_b_yaw_velocity_desired;
  c1_q_hoistedGlobal = *c1_b_p_gain_yaw;
  c1_r_hoistedGlobal = *c1_b_d_gain_yaw;
  c1_x_current = c1_hoistedGlobal;
  c1_x_desired = c1_b_hoistedGlobal;
  c1_x_velocity_current = c1_c_hoistedGlobal;
  c1_x_velocity_desired = c1_d_hoistedGlobal;
  c1_p_gain_x = c1_e_hoistedGlobal;
  c1_d_gain_x = c1_f_hoistedGlobal;
  c1_y_current = c1_g_hoistedGlobal;
  c1_y_desired = c1_h_hoistedGlobal;
  c1_y_velocity_current = c1_i_hoistedGlobal;
  c1_y_velocity_desired = c1_j_hoistedGlobal;
  c1_p_gain_y = c1_k_hoistedGlobal;
  c1_d_gain_y = c1_l_hoistedGlobal;
  c1_yaw_current = c1_m_hoistedGlobal;
  c1_yaw_desired = c1_n_hoistedGlobal;
  c1_yaw_velocity_current = c1_o_hoistedGlobal;
  c1_yaw_velocity_desired = c1_p_hoistedGlobal;
  c1_p_gain_yaw = c1_q_hoistedGlobal;
  c1_d_gain_yaw = c1_r_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 23U, 23U, c1_debug_family_names,
    c1_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargin, 0U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargout, 1U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_x_current, 2U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_x_desired, 3U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_x_velocity_current, 4U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_x_velocity_desired, 5U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_p_gain_x, 6U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_d_gain_x, 7U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_y_current, 8U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_y_desired, 9U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_y_velocity_current, 10U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_y_velocity_desired, 11U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_p_gain_y, 12U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_d_gain_y, 13U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_yaw_current, 14U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_yaw_desired, 15U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_yaw_velocity_current, 16U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_yaw_velocity_desired, 17U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_p_gain_yaw, 18U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_d_gain_yaw, 19U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_requiredForceX, 20U,
    c1_sf_marshallOut, c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_requiredForceY, 21U,
    c1_sf_marshallOut, c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_requiredMomentZ, 22U,
    c1_sf_marshallOut, c1_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 5);
  c1_requiredForceX = c1_p_gain_x * (c1_x_desired - c1_x_current) + c1_d_gain_x *
    (c1_x_velocity_desired - c1_x_velocity_current);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 6);
  c1_requiredForceY = c1_p_gain_y * (c1_y_desired - c1_y_current) + c1_d_gain_y *
    (c1_y_velocity_desired - c1_y_velocity_current);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 7);
  c1_requiredMomentZ = c1_p_gain_yaw * (c1_yaw_desired - c1_yaw_current) +
    c1_d_gain_yaw * (c1_yaw_velocity_desired - c1_yaw_velocity_current);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, -7);
  _SFD_SYMBOL_SCOPE_POP();
  *c1_b_requiredForceX = c1_requiredForceX;
  *c1_b_requiredForceY = c1_requiredForceY;
  *c1_b_requiredMomentZ = c1_requiredMomentZ;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_systemTestMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  _SFD_DATA_RANGE_CHECK(*c1_b_requiredForceX, 1U);
  _SFD_DATA_RANGE_CHECK(*c1_b_x_desired, 2U);
  _SFD_DATA_RANGE_CHECK(*c1_b_x_velocity_current, 3U);
  _SFD_DATA_RANGE_CHECK(*c1_b_x_velocity_desired, 4U);
  _SFD_DATA_RANGE_CHECK(*c1_b_p_gain_x, 5U);
  _SFD_DATA_RANGE_CHECK(*c1_b_d_gain_x, 6U);
  _SFD_DATA_RANGE_CHECK(*c1_b_y_current, 7U);
  _SFD_DATA_RANGE_CHECK(*c1_b_y_desired, 8U);
  _SFD_DATA_RANGE_CHECK(*c1_b_y_velocity_current, 9U);
  _SFD_DATA_RANGE_CHECK(*c1_b_y_velocity_desired, 10U);
  _SFD_DATA_RANGE_CHECK(*c1_b_p_gain_y, 11U);
  _SFD_DATA_RANGE_CHECK(*c1_b_d_gain_y, 12U);
  _SFD_DATA_RANGE_CHECK(*c1_b_yaw_current, 13U);
  _SFD_DATA_RANGE_CHECK(*c1_b_yaw_desired, 14U);
  _SFD_DATA_RANGE_CHECK(*c1_b_yaw_velocity_current, 15U);
  _SFD_DATA_RANGE_CHECK(*c1_b_yaw_velocity_desired, 16U);
  _SFD_DATA_RANGE_CHECK(*c1_b_p_gain_yaw, 17U);
  _SFD_DATA_RANGE_CHECK(*c1_b_d_gain_yaw, 18U);
  _SFD_DATA_RANGE_CHECK(*c1_b_requiredForceY, 19U);
  _SFD_DATA_RANGE_CHECK(*c1_b_requiredMomentZ, 20U);
}

static void initSimStructsc1_systemTest(SFc1_systemTestInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber, uint32_T c1_instanceNumber)
{
  (void)c1_machineNumber;
  (void)c1_chartNumber;
  (void)c1_instanceNumber;
}

static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  real_T c1_u;
  const mxArray *c1_y = NULL;
  SFc1_systemTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_systemTestInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_u = *(real_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static real_T c1_emlrt_marshallIn(SFc1_systemTestInstanceStruct *chartInstance,
  const mxArray *c1_requiredMomentZ, const char_T *c1_identifier)
{
  real_T c1_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_requiredMomentZ),
    &c1_thisId);
  sf_mex_destroy(&c1_requiredMomentZ);
  return c1_y;
}

static real_T c1_b_emlrt_marshallIn(SFc1_systemTestInstanceStruct *chartInstance,
  const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  real_T c1_y;
  real_T c1_d0;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_d0, 1, 0, 0U, 0, 0U, 0);
  c1_y = c1_d0;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_requiredMomentZ;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y;
  SFc1_systemTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_systemTestInstanceStruct *)chartInstanceVoid;
  c1_requiredMomentZ = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_requiredMomentZ),
    &c1_thisId);
  sf_mex_destroy(&c1_requiredMomentZ);
  *(real_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

const mxArray *sf_c1_systemTest_get_eml_resolved_functions_info(void)
{
  const mxArray *c1_nameCaptureInfo = NULL;
  c1_nameCaptureInfo = NULL;
  sf_mex_assign(&c1_nameCaptureInfo, sf_mex_create("nameCaptureInfo", NULL, 0,
    0U, 1U, 0U, 2, 0, 1), false);
  return c1_nameCaptureInfo;
}

static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_u;
  const mxArray *c1_y = NULL;
  SFc1_systemTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_systemTestInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_u = *(int32_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static int32_T c1_c_emlrt_marshallIn(SFc1_systemTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  int32_T c1_y;
  int32_T c1_i0;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_i0, 1, 6, 0U, 0, 0U, 0);
  c1_y = c1_i0;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_sfEvent;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  int32_T c1_y;
  SFc1_systemTestInstanceStruct *chartInstance;
  chartInstance = (SFc1_systemTestInstanceStruct *)chartInstanceVoid;
  c1_b_sfEvent = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_sfEvent),
    &c1_thisId);
  sf_mex_destroy(&c1_b_sfEvent);
  *(int32_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static uint8_T c1_d_emlrt_marshallIn(SFc1_systemTestInstanceStruct
  *chartInstance, const mxArray *c1_b_is_active_c1_systemTest, const char_T
  *c1_identifier)
{
  uint8_T c1_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_e_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c1_b_is_active_c1_systemTest), &c1_thisId);
  sf_mex_destroy(&c1_b_is_active_c1_systemTest);
  return c1_y;
}

static uint8_T c1_e_emlrt_marshallIn(SFc1_systemTestInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  uint8_T c1_y;
  uint8_T c1_u0;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_u0, 1, 3, 0U, 0, 0U, 0);
  c1_y = c1_u0;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void init_dsm_address_info(SFc1_systemTestInstanceStruct *chartInstance)
{
  (void)chartInstance;
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

void sf_c1_systemTest_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(589319363U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(4236359390U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(12029767U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2467814283U);
}

mxArray *sf_c1_systemTest_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("G1RzymHFVhpc6Q5Jo4Y2ZF");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,18,3,dataFields);

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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,10,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,10,"type",mxType);
    }

    mxSetField(mxData,10,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,11,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,11,"type",mxType);
    }

    mxSetField(mxData,11,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,12,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,12,"type",mxType);
    }

    mxSetField(mxData,12,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,13,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,13,"type",mxType);
    }

    mxSetField(mxData,13,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,14,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,14,"type",mxType);
    }

    mxSetField(mxData,14,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,15,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,15,"type",mxType);
    }

    mxSetField(mxData,15,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,16,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,16,"type",mxType);
    }

    mxSetField(mxData,16,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,17,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,17,"type",mxType);
    }

    mxSetField(mxData,17,"complexity",mxCreateDoubleScalar(0));
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

mxArray *sf_c1_systemTest_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c1_systemTest_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c1_systemTest(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x4'type','srcId','name','auxInfo'{{M[1],M[5],T\"requiredForceX\",},{M[1],M[23],T\"requiredForceY\",},{M[1],M[24],T\"requiredMomentZ\",},{M[8],M[0],T\"is_active_c1_systemTest\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 4, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c1_systemTest_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc1_systemTestInstanceStruct *chartInstance;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    chartInstance = (SFc1_systemTestInstanceStruct *) chartInfo->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _systemTestMachineNumber_,
           1,
           1,
           1,
           0,
           21,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           (void *)S);

        /* Each instance must initialize ist own list of scripts */
        init_script_number_translation(_systemTestMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_systemTestMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _systemTestMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"x_current");
          _SFD_SET_DATA_PROPS(1,2,0,1,"requiredForceX");
          _SFD_SET_DATA_PROPS(2,1,1,0,"x_desired");
          _SFD_SET_DATA_PROPS(3,1,1,0,"x_velocity_current");
          _SFD_SET_DATA_PROPS(4,1,1,0,"x_velocity_desired");
          _SFD_SET_DATA_PROPS(5,1,1,0,"p_gain_x");
          _SFD_SET_DATA_PROPS(6,1,1,0,"d_gain_x");
          _SFD_SET_DATA_PROPS(7,1,1,0,"y_current");
          _SFD_SET_DATA_PROPS(8,1,1,0,"y_desired");
          _SFD_SET_DATA_PROPS(9,1,1,0,"y_velocity_current");
          _SFD_SET_DATA_PROPS(10,1,1,0,"y_velocity_desired");
          _SFD_SET_DATA_PROPS(11,1,1,0,"p_gain_y");
          _SFD_SET_DATA_PROPS(12,1,1,0,"d_gain_y");
          _SFD_SET_DATA_PROPS(13,1,1,0,"yaw_current");
          _SFD_SET_DATA_PROPS(14,1,1,0,"yaw_desired");
          _SFD_SET_DATA_PROPS(15,1,1,0,"yaw_velocity_current");
          _SFD_SET_DATA_PROPS(16,1,1,0,"yaw_velocity_desired");
          _SFD_SET_DATA_PROPS(17,1,1,0,"p_gain_yaw");
          _SFD_SET_DATA_PROPS(18,1,1,0,"d_gain_yaw");
          _SFD_SET_DATA_PROPS(19,2,0,1,"requiredForceY");
          _SFD_SET_DATA_PROPS(20,2,0,1,"requiredMomentZ");
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
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,714);
        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)c1_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(11,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(12,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(13,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(14,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(15,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(16,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(17,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(18,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(19,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)c1_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(20,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)c1_sf_marshallIn);

        {
          real_T *c1_x_current;
          real_T *c1_requiredForceX;
          real_T *c1_x_desired;
          real_T *c1_x_velocity_current;
          real_T *c1_x_velocity_desired;
          real_T *c1_p_gain_x;
          real_T *c1_d_gain_x;
          real_T *c1_y_current;
          real_T *c1_y_desired;
          real_T *c1_y_velocity_current;
          real_T *c1_y_velocity_desired;
          real_T *c1_p_gain_y;
          real_T *c1_d_gain_y;
          real_T *c1_yaw_current;
          real_T *c1_yaw_desired;
          real_T *c1_yaw_velocity_current;
          real_T *c1_yaw_velocity_desired;
          real_T *c1_p_gain_yaw;
          real_T *c1_d_gain_yaw;
          real_T *c1_requiredForceY;
          real_T *c1_requiredMomentZ;
          c1_requiredMomentZ = (real_T *)ssGetOutputPortSignal(chartInstance->S,
            3);
          c1_requiredForceY = (real_T *)ssGetOutputPortSignal(chartInstance->S,
            2);
          c1_d_gain_yaw = (real_T *)ssGetInputPortSignal(chartInstance->S, 17);
          c1_p_gain_yaw = (real_T *)ssGetInputPortSignal(chartInstance->S, 16);
          c1_yaw_velocity_desired = (real_T *)ssGetInputPortSignal
            (chartInstance->S, 15);
          c1_yaw_velocity_current = (real_T *)ssGetInputPortSignal
            (chartInstance->S, 14);
          c1_yaw_desired = (real_T *)ssGetInputPortSignal(chartInstance->S, 13);
          c1_yaw_current = (real_T *)ssGetInputPortSignal(chartInstance->S, 12);
          c1_d_gain_y = (real_T *)ssGetInputPortSignal(chartInstance->S, 11);
          c1_p_gain_y = (real_T *)ssGetInputPortSignal(chartInstance->S, 10);
          c1_y_velocity_desired = (real_T *)ssGetInputPortSignal
            (chartInstance->S, 9);
          c1_y_velocity_current = (real_T *)ssGetInputPortSignal
            (chartInstance->S, 8);
          c1_y_desired = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
          c1_y_current = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
          c1_d_gain_x = (real_T *)ssGetInputPortSignal(chartInstance->S, 5);
          c1_p_gain_x = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
          c1_x_velocity_desired = (real_T *)ssGetInputPortSignal
            (chartInstance->S, 3);
          c1_x_velocity_current = (real_T *)ssGetInputPortSignal
            (chartInstance->S, 2);
          c1_x_desired = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c1_requiredForceX = (real_T *)ssGetOutputPortSignal(chartInstance->S,
            1);
          c1_x_current = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, c1_x_current);
          _SFD_SET_DATA_VALUE_PTR(1U, c1_requiredForceX);
          _SFD_SET_DATA_VALUE_PTR(2U, c1_x_desired);
          _SFD_SET_DATA_VALUE_PTR(3U, c1_x_velocity_current);
          _SFD_SET_DATA_VALUE_PTR(4U, c1_x_velocity_desired);
          _SFD_SET_DATA_VALUE_PTR(5U, c1_p_gain_x);
          _SFD_SET_DATA_VALUE_PTR(6U, c1_d_gain_x);
          _SFD_SET_DATA_VALUE_PTR(7U, c1_y_current);
          _SFD_SET_DATA_VALUE_PTR(8U, c1_y_desired);
          _SFD_SET_DATA_VALUE_PTR(9U, c1_y_velocity_current);
          _SFD_SET_DATA_VALUE_PTR(10U, c1_y_velocity_desired);
          _SFD_SET_DATA_VALUE_PTR(11U, c1_p_gain_y);
          _SFD_SET_DATA_VALUE_PTR(12U, c1_d_gain_y);
          _SFD_SET_DATA_VALUE_PTR(13U, c1_yaw_current);
          _SFD_SET_DATA_VALUE_PTR(14U, c1_yaw_desired);
          _SFD_SET_DATA_VALUE_PTR(15U, c1_yaw_velocity_current);
          _SFD_SET_DATA_VALUE_PTR(16U, c1_yaw_velocity_desired);
          _SFD_SET_DATA_VALUE_PTR(17U, c1_p_gain_yaw);
          _SFD_SET_DATA_VALUE_PTR(18U, c1_d_gain_yaw);
          _SFD_SET_DATA_VALUE_PTR(19U, c1_requiredForceY);
          _SFD_SET_DATA_VALUE_PTR(20U, c1_requiredMomentZ);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _systemTestMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "vJD8Js0mSb9TNqxEBds9F";
}

static void sf_opaque_initialize_c1_systemTest(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc1_systemTestInstanceStruct*) chartInstanceVar
    )->S,0);
  initialize_params_c1_systemTest((SFc1_systemTestInstanceStruct*)
    chartInstanceVar);
  initialize_c1_systemTest((SFc1_systemTestInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c1_systemTest(void *chartInstanceVar)
{
  enable_c1_systemTest((SFc1_systemTestInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c1_systemTest(void *chartInstanceVar)
{
  disable_c1_systemTest((SFc1_systemTestInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c1_systemTest(void *chartInstanceVar)
{
  sf_gateway_c1_systemTest((SFc1_systemTestInstanceStruct*) chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c1_systemTest(SimStruct* S)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c1_systemTest
    ((SFc1_systemTestInstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c1_systemTest();/* state var info */
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

extern void sf_internal_set_sim_state_c1_systemTest(SimStruct* S, const mxArray *
  st)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[3];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxDuplicateArray(st);      /* high level simctx */
  prhs[2] = (mxArray*) sf_get_sim_state_info_c1_systemTest();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 3, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c1_systemTest((SFc1_systemTestInstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c1_systemTest(SimStruct* S)
{
  return sf_internal_get_sim_state_c1_systemTest(S);
}

static void sf_opaque_set_sim_state_c1_systemTest(SimStruct* S, const mxArray
  *st)
{
  sf_internal_set_sim_state_c1_systemTest(S, st);
}

static void sf_opaque_terminate_c1_systemTest(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc1_systemTestInstanceStruct*) chartInstanceVar)->S;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_systemTest_optimization_info();
    }

    finalize_c1_systemTest((SFc1_systemTestInstanceStruct*) chartInstanceVar);
    utFree((void *)chartInstanceVar);
    if (crtInfo != NULL) {
      utFree((void *)crtInfo);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc1_systemTest((SFc1_systemTestInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c1_systemTest(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    initialize_params_c1_systemTest((SFc1_systemTestInstanceStruct*)
      (chartInfo->chartInstance));
  }
}

static void mdlSetWorkWidths_c1_systemTest(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_systemTest_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,1);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(sf_get_instance_specialization(),
                infoStruct,1,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,1,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(sf_get_instance_specialization(),infoStruct,1);
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
      ssSetInputPortOptimOpts(S, 10, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 11, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 12, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 13, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 14, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 15, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 16, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 17, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,1,18);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,1,3);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=3; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 18; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,1);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(1276549045U));
  ssSetChecksum1(S,(465397098U));
  ssSetChecksum2(S,(968945393U));
  ssSetChecksum3(S,(892289618U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c1_systemTest(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c1_systemTest(SimStruct *S)
{
  SFc1_systemTestInstanceStruct *chartInstance;
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)utMalloc(sizeof
    (ChartRunTimeInfo));
  chartInstance = (SFc1_systemTestInstanceStruct *)utMalloc(sizeof
    (SFc1_systemTestInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc1_systemTestInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c1_systemTest;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c1_systemTest;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c1_systemTest;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c1_systemTest;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c1_systemTest;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c1_systemTest;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c1_systemTest;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c1_systemTest;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c1_systemTest;
  chartInstance->chartInfo.mdlStart = mdlStart_c1_systemTest;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c1_systemTest;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  crtInfo->instanceInfo = (&(chartInstance->chartInfo));
  crtInfo->isJITEnabled = false;
  ssSetUserData(S,(void *)(crtInfo));  /* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  sf_opaque_init_subchart_simstructs(chartInstance->chartInfo.chartInstance);
  chart_debug_initialization(S,1);
}

void c1_systemTest_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c1_systemTest(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c1_systemTest(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c1_systemTest(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c1_systemTest_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
