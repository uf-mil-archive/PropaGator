/* Include files */

#include "blascompat32.h"
#include "boat_sfun.h"
#include "c5_boat.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "boat_sfun_debug_macros.h"

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c5_debug_family_names[20] = { "v11", "v12", "v13", "v14",
  "v15", "v16", "nargin", "nargout", "v0", "u0", "rx1", "rx2", "ry1", "ry2", "m",
  "I", "friX", "friY", "friZ", "v1" };

/* Function Declarations */
static void initialize_c5_boat(SFc5_boatInstanceStruct *chartInstance);
static void initialize_params_c5_boat(SFc5_boatInstanceStruct *chartInstance);
static void enable_c5_boat(SFc5_boatInstanceStruct *chartInstance);
static void disable_c5_boat(SFc5_boatInstanceStruct *chartInstance);
static void c5_update_debugger_state_c5_boat(SFc5_boatInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c5_boat(SFc5_boatInstanceStruct
  *chartInstance);
static void set_sim_state_c5_boat(SFc5_boatInstanceStruct *chartInstance, const
  mxArray *c5_st);
static void finalize_c5_boat(SFc5_boatInstanceStruct *chartInstance);
static void sf_c5_boat(SFc5_boatInstanceStruct *chartInstance);
static void initSimStructsc5_boat(SFc5_boatInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c5_machineNumber, uint32_T
  c5_chartNumber);
static const mxArray *c5_sf_marshallOut(void *chartInstanceVoid, void *c5_inData);
static void c5_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance, const
  mxArray *c5_v1, const char_T *c5_identifier, real_T c5_y[6]);
static void c5_b_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance, const
  mxArray *c5_u, const emlrtMsgIdentifier *c5_parentId, real_T c5_y[6]);
static void c5_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c5_mxArrayInData, const char_T *c5_varName, void *c5_outData);
static const mxArray *c5_b_sf_marshallOut(void *chartInstanceVoid, void
  *c5_inData);
static const mxArray *c5_c_sf_marshallOut(void *chartInstanceVoid, void
  *c5_inData);
static real_T c5_c_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance,
  const mxArray *c5_u, const emlrtMsgIdentifier *c5_parentId);
static void c5_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c5_mxArrayInData, const char_T *c5_varName, void *c5_outData);
static void c5_info_helper(c5_ResolvedFunctionInfo c5_info[21]);
static real_T c5_cosd(SFc5_boatInstanceStruct *chartInstance, real_T c5_x);
static void c5_eml_scalar_rem90(SFc5_boatInstanceStruct *chartInstance, real_T
  c5_x, real_T *c5_n, real_T *c5_y);
static real_T c5_sind(SFc5_boatInstanceStruct *chartInstance, real_T c5_x);
static const mxArray *c5_d_sf_marshallOut(void *chartInstanceVoid, void
  *c5_inData);
static int32_T c5_d_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance,
  const mxArray *c5_u, const emlrtMsgIdentifier *c5_parentId);
static void c5_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c5_mxArrayInData, const char_T *c5_varName, void *c5_outData);
static uint8_T c5_e_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance,
  const mxArray *c5_b_is_active_c5_boat, const char_T *c5_identifier);
static uint8_T c5_f_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance,
  const mxArray *c5_u, const emlrtMsgIdentifier *c5_parentId);
static void c5_b_cosd(SFc5_boatInstanceStruct *chartInstance, real_T *c5_x);
static void c5_b_sind(SFc5_boatInstanceStruct *chartInstance, real_T *c5_x);
static void init_dsm_address_info(SFc5_boatInstanceStruct *chartInstance);

/* Function Definitions */
static void initialize_c5_boat(SFc5_boatInstanceStruct *chartInstance)
{
  chartInstance->c5_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c5_is_active_c5_boat = 0U;
}

static void initialize_params_c5_boat(SFc5_boatInstanceStruct *chartInstance)
{
}

static void enable_c5_boat(SFc5_boatInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c5_boat(SFc5_boatInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c5_update_debugger_state_c5_boat(SFc5_boatInstanceStruct
  *chartInstance)
{
}

static const mxArray *get_sim_state_c5_boat(SFc5_boatInstanceStruct
  *chartInstance)
{
  const mxArray *c5_st;
  const mxArray *c5_y = NULL;
  int32_T c5_i0;
  real_T c5_u[6];
  const mxArray *c5_b_y = NULL;
  uint8_T c5_hoistedGlobal;
  uint8_T c5_b_u;
  const mxArray *c5_c_y = NULL;
  real_T (*c5_v1)[6];
  c5_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  c5_st = NULL;
  c5_st = NULL;
  c5_y = NULL;
  sf_mex_assign(&c5_y, sf_mex_createcellarray(2), FALSE);
  for (c5_i0 = 0; c5_i0 < 6; c5_i0++) {
    c5_u[c5_i0] = (*c5_v1)[c5_i0];
  }

  c5_b_y = NULL;
  sf_mex_assign(&c5_b_y, sf_mex_create("y", c5_u, 0, 0U, 1U, 0U, 1, 6), FALSE);
  sf_mex_setcell(c5_y, 0, c5_b_y);
  c5_hoistedGlobal = chartInstance->c5_is_active_c5_boat;
  c5_b_u = c5_hoistedGlobal;
  c5_c_y = NULL;
  sf_mex_assign(&c5_c_y, sf_mex_create("y", &c5_b_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c5_y, 1, c5_c_y);
  sf_mex_assign(&c5_st, c5_y, FALSE);
  return c5_st;
}

static void set_sim_state_c5_boat(SFc5_boatInstanceStruct *chartInstance, const
  mxArray *c5_st)
{
  const mxArray *c5_u;
  real_T c5_dv0[6];
  int32_T c5_i1;
  real_T (*c5_v1)[6];
  c5_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c5_doneDoubleBufferReInit = TRUE;
  c5_u = sf_mex_dup(c5_st);
  c5_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c5_u, 0)), "v1",
                      c5_dv0);
  for (c5_i1 = 0; c5_i1 < 6; c5_i1++) {
    (*c5_v1)[c5_i1] = c5_dv0[c5_i1];
  }

  chartInstance->c5_is_active_c5_boat = c5_e_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c5_u, 1)), "is_active_c5_boat");
  sf_mex_destroy(&c5_u);
  c5_update_debugger_state_c5_boat(chartInstance);
  sf_mex_destroy(&c5_st);
}

static void finalize_c5_boat(SFc5_boatInstanceStruct *chartInstance)
{
}

static void sf_c5_boat(SFc5_boatInstanceStruct *chartInstance)
{
  int32_T c5_i2;
  int32_T c5_i3;
  int32_T c5_i4;
  real_T c5_hoistedGlobal;
  real_T c5_b_hoistedGlobal;
  real_T c5_c_hoistedGlobal;
  real_T c5_d_hoistedGlobal;
  real_T c5_e_hoistedGlobal;
  real_T c5_f_hoistedGlobal;
  real_T c5_g_hoistedGlobal;
  real_T c5_h_hoistedGlobal;
  real_T c5_i_hoistedGlobal;
  int32_T c5_i5;
  real_T c5_v0[6];
  int32_T c5_i6;
  real_T c5_u0[4];
  real_T c5_rx1;
  real_T c5_rx2;
  real_T c5_ry1;
  real_T c5_ry2;
  real_T c5_m;
  real_T c5_I;
  real_T c5_friX;
  real_T c5_friY;
  real_T c5_friZ;
  uint32_T c5_debug_family_var_map[20];
  real_T c5_v11;
  real_T c5_v12;
  real_T c5_v13;
  real_T c5_v14;
  real_T c5_v15;
  real_T c5_v16;
  real_T c5_nargin = 11.0;
  real_T c5_nargout = 1.0;
  real_T c5_v1[6];
  real_T c5_A;
  real_T c5_B;
  real_T c5_x;
  real_T c5_y;
  real_T c5_b_x;
  real_T c5_b_y;
  real_T c5_c_y;
  real_T c5_a;
  real_T c5_b;
  real_T c5_d_y;
  real_T c5_b_A;
  real_T c5_b_B;
  real_T c5_c_x;
  real_T c5_e_y;
  real_T c5_d_x;
  real_T c5_f_y;
  real_T c5_g_y;
  real_T c5_b_a;
  real_T c5_b_b;
  real_T c5_h_y;
  real_T c5_c_a;
  real_T c5_c_b;
  real_T c5_i_y;
  real_T c5_d_a;
  real_T c5_d0;
  real_T c5_d1;
  real_T c5_d_b;
  real_T c5_j_y;
  real_T c5_c_A;
  real_T c5_c_B;
  real_T c5_e_x;
  real_T c5_k_y;
  real_T c5_f_x;
  real_T c5_l_y;
  real_T c5_m_y;
  real_T c5_e_a;
  real_T c5_e_b;
  real_T c5_n_y;
  real_T c5_d_A;
  real_T c5_d_B;
  real_T c5_g_x;
  real_T c5_o_y;
  real_T c5_h_x;
  real_T c5_p_y;
  real_T c5_q_y;
  real_T c5_f_a;
  real_T c5_f_b;
  real_T c5_r_y;
  real_T c5_g_a;
  real_T c5_g_b;
  real_T c5_s_y;
  real_T c5_h_a;
  real_T c5_d2;
  real_T c5_d3;
  real_T c5_h_b;
  real_T c5_t_y;
  real_T c5_e_A;
  real_T c5_e_B;
  real_T c5_i_x;
  real_T c5_u_y;
  real_T c5_j_x;
  real_T c5_v_y;
  real_T c5_w_y;
  real_T c5_i_a;
  real_T c5_i_b;
  real_T c5_x_y;
  real_T c5_j_a;
  real_T c5_j_b;
  real_T c5_y_y;
  real_T c5_f_A;
  real_T c5_f_B;
  real_T c5_k_x;
  real_T c5_ab_y;
  real_T c5_l_x;
  real_T c5_bb_y;
  real_T c5_cb_y;
  real_T c5_k_a;
  real_T c5_k_b;
  real_T c5_db_y;
  real_T c5_l_a;
  real_T c5_l_b;
  real_T c5_eb_y;
  real_T c5_g_A;
  real_T c5_g_B;
  real_T c5_m_x;
  real_T c5_fb_y;
  real_T c5_n_x;
  real_T c5_gb_y;
  real_T c5_hb_y;
  real_T c5_m_a;
  real_T c5_m_b;
  real_T c5_ib_y;
  real_T c5_n_a;
  real_T c5_n_b;
  real_T c5_jb_y;
  real_T c5_h_A;
  real_T c5_h_B;
  real_T c5_o_x;
  real_T c5_kb_y;
  real_T c5_p_x;
  real_T c5_lb_y;
  real_T c5_mb_y;
  real_T c5_o_a;
  real_T c5_o_b;
  real_T c5_nb_y;
  real_T c5_p_a;
  real_T c5_p_b;
  real_T c5_ob_y;
  real_T c5_q_a;
  real_T c5_q_b;
  real_T c5_pb_y;
  int32_T c5_i7;
  real_T *c5_b_friZ;
  real_T *c5_b_friY;
  real_T *c5_b_friX;
  real_T *c5_b_I;
  real_T *c5_b_m;
  real_T *c5_b_ry2;
  real_T *c5_b_ry1;
  real_T *c5_b_rx2;
  real_T *c5_b_rx1;
  real_T (*c5_b_v1)[6];
  real_T (*c5_b_u0)[4];
  real_T (*c5_b_v0)[6];
  c5_b_friZ = (real_T *)ssGetInputPortSignal(chartInstance->S, 10);
  c5_b_friY = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
  c5_b_friX = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
  c5_b_I = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
  c5_b_m = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
  c5_b_ry2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 5);
  c5_b_ry1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c5_b_rx2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c5_b_rx1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c5_b_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 1);
  c5_b_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
  c5_b_v0 = (real_T (*)[6])ssGetInputPortSignal(chartInstance->S, 0);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c5_sfEvent);
  for (c5_i2 = 0; c5_i2 < 6; c5_i2++) {
    _SFD_DATA_RANGE_CHECK((*c5_b_v0)[c5_i2], 0U);
  }

  for (c5_i3 = 0; c5_i3 < 6; c5_i3++) {
    _SFD_DATA_RANGE_CHECK((*c5_b_v1)[c5_i3], 1U);
  }

  for (c5_i4 = 0; c5_i4 < 4; c5_i4++) {
    _SFD_DATA_RANGE_CHECK((*c5_b_u0)[c5_i4], 2U);
  }

  _SFD_DATA_RANGE_CHECK(*c5_b_rx1, 3U);
  _SFD_DATA_RANGE_CHECK(*c5_b_rx2, 4U);
  _SFD_DATA_RANGE_CHECK(*c5_b_ry1, 5U);
  _SFD_DATA_RANGE_CHECK(*c5_b_ry2, 6U);
  _SFD_DATA_RANGE_CHECK(*c5_b_m, 7U);
  _SFD_DATA_RANGE_CHECK(*c5_b_I, 8U);
  _SFD_DATA_RANGE_CHECK(*c5_b_friX, 9U);
  _SFD_DATA_RANGE_CHECK(*c5_b_friY, 10U);
  _SFD_DATA_RANGE_CHECK(*c5_b_friZ, 11U);
  chartInstance->c5_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c5_sfEvent);
  c5_hoistedGlobal = *c5_b_rx1;
  c5_b_hoistedGlobal = *c5_b_rx2;
  c5_c_hoistedGlobal = *c5_b_ry1;
  c5_d_hoistedGlobal = *c5_b_ry2;
  c5_e_hoistedGlobal = *c5_b_m;
  c5_f_hoistedGlobal = *c5_b_I;
  c5_g_hoistedGlobal = *c5_b_friX;
  c5_h_hoistedGlobal = *c5_b_friY;
  c5_i_hoistedGlobal = *c5_b_friZ;
  for (c5_i5 = 0; c5_i5 < 6; c5_i5++) {
    c5_v0[c5_i5] = (*c5_b_v0)[c5_i5];
  }

  for (c5_i6 = 0; c5_i6 < 4; c5_i6++) {
    c5_u0[c5_i6] = (*c5_b_u0)[c5_i6];
  }

  c5_rx1 = c5_hoistedGlobal;
  c5_rx2 = c5_b_hoistedGlobal;
  c5_ry1 = c5_c_hoistedGlobal;
  c5_ry2 = c5_d_hoistedGlobal;
  c5_m = c5_e_hoistedGlobal;
  c5_I = c5_f_hoistedGlobal;
  c5_friX = c5_g_hoistedGlobal;
  c5_friY = c5_h_hoistedGlobal;
  c5_friZ = c5_i_hoistedGlobal;
  sf_debug_symbol_scope_push_eml(0U, 20U, 20U, c5_debug_family_names,
    c5_debug_family_var_map);
  sf_debug_symbol_scope_add_eml_importable(&c5_v11, 0U, c5_b_sf_marshallOut,
    c5_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c5_v12, 1U, c5_b_sf_marshallOut,
    c5_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c5_v13, 2U, c5_b_sf_marshallOut,
    c5_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c5_v14, 3U, c5_b_sf_marshallOut,
    c5_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c5_v15, 4U, c5_b_sf_marshallOut,
    c5_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c5_v16, 5U, c5_b_sf_marshallOut,
    c5_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c5_nargin, 6U, c5_b_sf_marshallOut,
    c5_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c5_nargout, 7U, c5_b_sf_marshallOut,
    c5_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml(c5_v0, 8U, c5_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(c5_u0, 9U, c5_c_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_rx1, 10U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_rx2, 11U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_ry1, 12U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_ry2, 13U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_m, 14U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_I, 15U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_friX, 16U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_friY, 17U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(&c5_friZ, 18U, c5_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml_importable(c5_v1, 19U, c5_sf_marshallOut,
    c5_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c5_sfEvent, 12);
  c5_v11 = c5_v0[1];
  _SFD_EML_CALL(0U, chartInstance->c5_sfEvent, 13);
  c5_A = c5_u0[1];
  c5_B = c5_m;
  c5_x = c5_A;
  c5_y = c5_B;
  c5_b_x = c5_x;
  c5_b_y = c5_y;
  c5_c_y = c5_b_x / c5_b_y;
  c5_a = c5_c_y;
  c5_b = c5_u0[0] + c5_v0[4];
  c5_b_cosd(chartInstance, &c5_b);
  c5_d_y = c5_a * c5_b;
  c5_b_A = c5_u0[3];
  c5_b_B = c5_m;
  c5_c_x = c5_b_A;
  c5_e_y = c5_b_B;
  c5_d_x = c5_c_x;
  c5_f_y = c5_e_y;
  c5_g_y = c5_d_x / c5_f_y;
  c5_b_a = c5_g_y;
  c5_b_b = c5_u0[2] + c5_v0[4];
  c5_b_cosd(chartInstance, &c5_b_b);
  c5_h_y = c5_b_a * c5_b_b;
  c5_c_a = c5_friX;
  c5_c_b = c5_v0[1];
  c5_i_y = c5_c_a * c5_c_b;
  c5_d_a = c5_i_y;
  c5_d0 = c5_v0[4];
  c5_b_sind(chartInstance, &c5_d0);
  c5_d1 = c5_v0[4];
  c5_b_cosd(chartInstance, &c5_d1);
  c5_d_b = c5_d1 + c5_d0;
  c5_j_y = c5_d_a * c5_d_b;
  c5_v12 = (c5_d_y + c5_h_y) - c5_j_y;
  _SFD_EML_CALL(0U, chartInstance->c5_sfEvent, 14);
  c5_v13 = c5_v0[3];
  _SFD_EML_CALL(0U, chartInstance->c5_sfEvent, 15);
  c5_c_A = c5_u0[1];
  c5_c_B = c5_m;
  c5_e_x = c5_c_A;
  c5_k_y = c5_c_B;
  c5_f_x = c5_e_x;
  c5_l_y = c5_k_y;
  c5_m_y = c5_f_x / c5_l_y;
  c5_e_a = c5_m_y;
  c5_e_b = c5_u0[0] + c5_v0[4];
  c5_b_sind(chartInstance, &c5_e_b);
  c5_n_y = c5_e_a * c5_e_b;
  c5_d_A = c5_u0[3];
  c5_d_B = c5_m;
  c5_g_x = c5_d_A;
  c5_o_y = c5_d_B;
  c5_h_x = c5_g_x;
  c5_p_y = c5_o_y;
  c5_q_y = c5_h_x / c5_p_y;
  c5_f_a = c5_q_y;
  c5_f_b = c5_u0[2] + c5_v0[4];
  c5_b_sind(chartInstance, &c5_f_b);
  c5_r_y = c5_f_a * c5_f_b;
  c5_g_a = c5_friY;
  c5_g_b = c5_v0[3];
  c5_s_y = c5_g_a * c5_g_b;
  c5_h_a = c5_s_y;
  c5_d2 = c5_v0[4];
  c5_b_cosd(chartInstance, &c5_d2);
  c5_d3 = c5_v0[4];
  c5_b_sind(chartInstance, &c5_d3);
  c5_h_b = -c5_d3 + c5_d2;
  c5_t_y = c5_h_a * c5_h_b;
  c5_v14 = (c5_n_y + c5_r_y) - c5_t_y;
  _SFD_EML_CALL(0U, chartInstance->c5_sfEvent, 16);
  c5_v15 = c5_v0[5];
  _SFD_EML_CALL(0U, chartInstance->c5_sfEvent, 17);
  c5_e_A = c5_rx1;
  c5_e_B = c5_I;
  c5_i_x = c5_e_A;
  c5_u_y = c5_e_B;
  c5_j_x = c5_i_x;
  c5_v_y = c5_u_y;
  c5_w_y = c5_j_x / c5_v_y;
  c5_i_a = c5_w_y;
  c5_i_b = c5_u0[1];
  c5_x_y = c5_i_a * c5_i_b;
  c5_j_a = c5_x_y;
  c5_j_b = c5_u0[0];
  c5_b_sind(chartInstance, &c5_j_b);
  c5_y_y = c5_j_a * c5_j_b;
  c5_f_A = c5_ry1;
  c5_f_B = c5_I;
  c5_k_x = c5_f_A;
  c5_ab_y = c5_f_B;
  c5_l_x = c5_k_x;
  c5_bb_y = c5_ab_y;
  c5_cb_y = c5_l_x / c5_bb_y;
  c5_k_a = c5_cb_y;
  c5_k_b = c5_u0[1];
  c5_db_y = c5_k_a * c5_k_b;
  c5_l_a = c5_db_y;
  c5_l_b = c5_u0[0];
  c5_b_cosd(chartInstance, &c5_l_b);
  c5_eb_y = c5_l_a * c5_l_b;
  c5_g_A = c5_rx2;
  c5_g_B = c5_I;
  c5_m_x = c5_g_A;
  c5_fb_y = c5_g_B;
  c5_n_x = c5_m_x;
  c5_gb_y = c5_fb_y;
  c5_hb_y = c5_n_x / c5_gb_y;
  c5_m_a = c5_hb_y;
  c5_m_b = c5_u0[3];
  c5_ib_y = c5_m_a * c5_m_b;
  c5_n_a = c5_ib_y;
  c5_n_b = c5_u0[2];
  c5_b_sind(chartInstance, &c5_n_b);
  c5_jb_y = c5_n_a * c5_n_b;
  c5_h_A = c5_ry2;
  c5_h_B = c5_I;
  c5_o_x = c5_h_A;
  c5_kb_y = c5_h_B;
  c5_p_x = c5_o_x;
  c5_lb_y = c5_kb_y;
  c5_mb_y = c5_p_x / c5_lb_y;
  c5_o_a = c5_mb_y;
  c5_o_b = c5_u0[3];
  c5_nb_y = c5_o_a * c5_o_b;
  c5_p_a = c5_nb_y;
  c5_p_b = c5_u0[2];
  c5_b_cosd(chartInstance, &c5_p_b);
  c5_ob_y = c5_p_a * c5_p_b;
  c5_q_a = c5_friZ;
  c5_q_b = c5_v0[5];
  c5_pb_y = c5_q_a * c5_q_b;
  c5_v16 = (((c5_y_y - c5_eb_y) + c5_jb_y) - c5_ob_y) - c5_pb_y;
  _SFD_EML_CALL(0U, chartInstance->c5_sfEvent, 19);
  c5_v1[0] = c5_v11;
  c5_v1[1] = c5_v12;
  c5_v1[2] = c5_v13;
  c5_v1[3] = c5_v14;
  c5_v1[4] = c5_v15;
  c5_v1[5] = c5_v16;
  _SFD_EML_CALL(0U, chartInstance->c5_sfEvent, -19);
  sf_debug_symbol_scope_pop();
  for (c5_i7 = 0; c5_i7 < 6; c5_i7++) {
    (*c5_b_v1)[c5_i7] = c5_v1[c5_i7];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c5_sfEvent);
  sf_debug_check_for_state_inconsistency(_boatMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void initSimStructsc5_boat(SFc5_boatInstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c5_machineNumber, uint32_T
  c5_chartNumber)
{
}

static const mxArray *c5_sf_marshallOut(void *chartInstanceVoid, void *c5_inData)
{
  const mxArray *c5_mxArrayOutData = NULL;
  int32_T c5_i8;
  real_T c5_b_inData[6];
  int32_T c5_i9;
  real_T c5_u[6];
  const mxArray *c5_y = NULL;
  SFc5_boatInstanceStruct *chartInstance;
  chartInstance = (SFc5_boatInstanceStruct *)chartInstanceVoid;
  c5_mxArrayOutData = NULL;
  for (c5_i8 = 0; c5_i8 < 6; c5_i8++) {
    c5_b_inData[c5_i8] = (*(real_T (*)[6])c5_inData)[c5_i8];
  }

  for (c5_i9 = 0; c5_i9 < 6; c5_i9++) {
    c5_u[c5_i9] = c5_b_inData[c5_i9];
  }

  c5_y = NULL;
  sf_mex_assign(&c5_y, sf_mex_create("y", c5_u, 0, 0U, 1U, 0U, 1, 6), FALSE);
  sf_mex_assign(&c5_mxArrayOutData, c5_y, FALSE);
  return c5_mxArrayOutData;
}

static void c5_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance, const
  mxArray *c5_v1, const char_T *c5_identifier, real_T c5_y[6])
{
  emlrtMsgIdentifier c5_thisId;
  c5_thisId.fIdentifier = c5_identifier;
  c5_thisId.fParent = NULL;
  c5_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c5_v1), &c5_thisId, c5_y);
  sf_mex_destroy(&c5_v1);
}

static void c5_b_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance, const
  mxArray *c5_u, const emlrtMsgIdentifier *c5_parentId, real_T c5_y[6])
{
  real_T c5_dv1[6];
  int32_T c5_i10;
  sf_mex_import(c5_parentId, sf_mex_dup(c5_u), c5_dv1, 1, 0, 0U, 1, 0U, 1, 6);
  for (c5_i10 = 0; c5_i10 < 6; c5_i10++) {
    c5_y[c5_i10] = c5_dv1[c5_i10];
  }

  sf_mex_destroy(&c5_u);
}

static void c5_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c5_mxArrayInData, const char_T *c5_varName, void *c5_outData)
{
  const mxArray *c5_v1;
  const char_T *c5_identifier;
  emlrtMsgIdentifier c5_thisId;
  real_T c5_y[6];
  int32_T c5_i11;
  SFc5_boatInstanceStruct *chartInstance;
  chartInstance = (SFc5_boatInstanceStruct *)chartInstanceVoid;
  c5_v1 = sf_mex_dup(c5_mxArrayInData);
  c5_identifier = c5_varName;
  c5_thisId.fIdentifier = c5_identifier;
  c5_thisId.fParent = NULL;
  c5_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c5_v1), &c5_thisId, c5_y);
  sf_mex_destroy(&c5_v1);
  for (c5_i11 = 0; c5_i11 < 6; c5_i11++) {
    (*(real_T (*)[6])c5_outData)[c5_i11] = c5_y[c5_i11];
  }

  sf_mex_destroy(&c5_mxArrayInData);
}

static const mxArray *c5_b_sf_marshallOut(void *chartInstanceVoid, void
  *c5_inData)
{
  const mxArray *c5_mxArrayOutData = NULL;
  real_T c5_u;
  const mxArray *c5_y = NULL;
  SFc5_boatInstanceStruct *chartInstance;
  chartInstance = (SFc5_boatInstanceStruct *)chartInstanceVoid;
  c5_mxArrayOutData = NULL;
  c5_u = *(real_T *)c5_inData;
  c5_y = NULL;
  sf_mex_assign(&c5_y, sf_mex_create("y", &c5_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c5_mxArrayOutData, c5_y, FALSE);
  return c5_mxArrayOutData;
}

static const mxArray *c5_c_sf_marshallOut(void *chartInstanceVoid, void
  *c5_inData)
{
  const mxArray *c5_mxArrayOutData = NULL;
  int32_T c5_i12;
  real_T c5_b_inData[4];
  int32_T c5_i13;
  real_T c5_u[4];
  const mxArray *c5_y = NULL;
  SFc5_boatInstanceStruct *chartInstance;
  chartInstance = (SFc5_boatInstanceStruct *)chartInstanceVoid;
  c5_mxArrayOutData = NULL;
  for (c5_i12 = 0; c5_i12 < 4; c5_i12++) {
    c5_b_inData[c5_i12] = (*(real_T (*)[4])c5_inData)[c5_i12];
  }

  for (c5_i13 = 0; c5_i13 < 4; c5_i13++) {
    c5_u[c5_i13] = c5_b_inData[c5_i13];
  }

  c5_y = NULL;
  sf_mex_assign(&c5_y, sf_mex_create("y", c5_u, 0, 0U, 1U, 0U, 1, 4), FALSE);
  sf_mex_assign(&c5_mxArrayOutData, c5_y, FALSE);
  return c5_mxArrayOutData;
}

static real_T c5_c_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance,
  const mxArray *c5_u, const emlrtMsgIdentifier *c5_parentId)
{
  real_T c5_y;
  real_T c5_d4;
  sf_mex_import(c5_parentId, sf_mex_dup(c5_u), &c5_d4, 1, 0, 0U, 0, 0U, 0);
  c5_y = c5_d4;
  sf_mex_destroy(&c5_u);
  return c5_y;
}

static void c5_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c5_mxArrayInData, const char_T *c5_varName, void *c5_outData)
{
  const mxArray *c5_nargout;
  const char_T *c5_identifier;
  emlrtMsgIdentifier c5_thisId;
  real_T c5_y;
  SFc5_boatInstanceStruct *chartInstance;
  chartInstance = (SFc5_boatInstanceStruct *)chartInstanceVoid;
  c5_nargout = sf_mex_dup(c5_mxArrayInData);
  c5_identifier = c5_varName;
  c5_thisId.fIdentifier = c5_identifier;
  c5_thisId.fParent = NULL;
  c5_y = c5_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c5_nargout), &c5_thisId);
  sf_mex_destroy(&c5_nargout);
  *(real_T *)c5_outData = c5_y;
  sf_mex_destroy(&c5_mxArrayInData);
}

const mxArray *sf_c5_boat_get_eml_resolved_functions_info(void)
{
  const mxArray *c5_nameCaptureInfo;
  c5_ResolvedFunctionInfo c5_info[21];
  const mxArray *c5_m0 = NULL;
  int32_T c5_i14;
  c5_ResolvedFunctionInfo *c5_r0;
  c5_nameCaptureInfo = NULL;
  c5_nameCaptureInfo = NULL;
  c5_info_helper(c5_info);
  sf_mex_assign(&c5_m0, sf_mex_createstruct("nameCaptureInfo", 1, 21), FALSE);
  for (c5_i14 = 0; c5_i14 < 21; c5_i14++) {
    c5_r0 = &c5_info[c5_i14];
    sf_mex_addfield(c5_m0, sf_mex_create("nameCaptureInfo", c5_r0->context, 15,
      0U, 0U, 0U, 2, 1, strlen(c5_r0->context)), "context", "nameCaptureInfo",
                    c5_i14);
    sf_mex_addfield(c5_m0, sf_mex_create("nameCaptureInfo", c5_r0->name, 15, 0U,
      0U, 0U, 2, 1, strlen(c5_r0->name)), "name", "nameCaptureInfo", c5_i14);
    sf_mex_addfield(c5_m0, sf_mex_create("nameCaptureInfo", c5_r0->dominantType,
      15, 0U, 0U, 0U, 2, 1, strlen(c5_r0->dominantType)), "dominantType",
                    "nameCaptureInfo", c5_i14);
    sf_mex_addfield(c5_m0, sf_mex_create("nameCaptureInfo", c5_r0->resolved, 15,
      0U, 0U, 0U, 2, 1, strlen(c5_r0->resolved)), "resolved", "nameCaptureInfo",
                    c5_i14);
    sf_mex_addfield(c5_m0, sf_mex_create("nameCaptureInfo", &c5_r0->fileTimeLo,
      7, 0U, 0U, 0U, 0), "fileTimeLo", "nameCaptureInfo", c5_i14);
    sf_mex_addfield(c5_m0, sf_mex_create("nameCaptureInfo", &c5_r0->fileTimeHi,
      7, 0U, 0U, 0U, 0), "fileTimeHi", "nameCaptureInfo", c5_i14);
    sf_mex_addfield(c5_m0, sf_mex_create("nameCaptureInfo", &c5_r0->mFileTimeLo,
      7, 0U, 0U, 0U, 0), "mFileTimeLo", "nameCaptureInfo", c5_i14);
    sf_mex_addfield(c5_m0, sf_mex_create("nameCaptureInfo", &c5_r0->mFileTimeHi,
      7, 0U, 0U, 0U, 0), "mFileTimeHi", "nameCaptureInfo", c5_i14);
  }

  sf_mex_assign(&c5_nameCaptureInfo, c5_m0, FALSE);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c5_nameCaptureInfo);
  return c5_nameCaptureInfo;
}

static void c5_info_helper(c5_ResolvedFunctionInfo c5_info[21])
{
  c5_info[0].context = "";
  c5_info[0].name = "mrdivide";
  c5_info[0].dominantType = "double";
  c5_info[0].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/ops/mrdivide.p";
  c5_info[0].fileTimeLo = 1325145738U;
  c5_info[0].fileTimeHi = 0U;
  c5_info[0].mFileTimeLo = 1319751566U;
  c5_info[0].mFileTimeHi = 0U;
  c5_info[1].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/ops/mrdivide.p";
  c5_info[1].name = "rdivide";
  c5_info[1].dominantType = "double";
  c5_info[1].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/ops/rdivide.m";
  c5_info[1].fileTimeLo = 1286840444U;
  c5_info[1].fileTimeHi = 0U;
  c5_info[1].mFileTimeLo = 0U;
  c5_info[1].mFileTimeHi = 0U;
  c5_info[2].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/ops/rdivide.m";
  c5_info[2].name = "eml_div";
  c5_info[2].dominantType = "double";
  c5_info[2].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/eml/eml_div.m";
  c5_info[2].fileTimeLo = 1313369410U;
  c5_info[2].fileTimeHi = 0U;
  c5_info[2].mFileTimeLo = 0U;
  c5_info[2].mFileTimeHi = 0U;
  c5_info[3].context = "";
  c5_info[3].name = "cosd";
  c5_info[3].dominantType = "double";
  c5_info[3].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/cosd.m";
  c5_info[3].fileTimeLo = 1286840306U;
  c5_info[3].fileTimeHi = 0U;
  c5_info[3].mFileTimeLo = 0U;
  c5_info[3].mFileTimeHi = 0U;
  c5_info[4].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/cosd.m";
  c5_info[4].name = "eml_scalar_cosd";
  c5_info[4].dominantType = "double";
  c5_info[4].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd.m";
  c5_info[4].fileTimeLo = 1320911036U;
  c5_info[4].fileTimeHi = 0U;
  c5_info[4].mFileTimeLo = 0U;
  c5_info[4].mFileTimeHi = 0U;
  c5_info[5].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd.m";
  c5_info[5].name = "isfinite";
  c5_info[5].dominantType = "double";
  c5_info[5].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elmat/isfinite.m";
  c5_info[5].fileTimeLo = 1286840358U;
  c5_info[5].fileTimeHi = 0U;
  c5_info[5].mFileTimeLo = 0U;
  c5_info[5].mFileTimeHi = 0U;
  c5_info[6].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elmat/isfinite.m";
  c5_info[6].name = "isinf";
  c5_info[6].dominantType = "double";
  c5_info[6].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elmat/isinf.m";
  c5_info[6].fileTimeLo = 1286840360U;
  c5_info[6].fileTimeHi = 0U;
  c5_info[6].mFileTimeLo = 0U;
  c5_info[6].mFileTimeHi = 0U;
  c5_info[7].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elmat/isfinite.m";
  c5_info[7].name = "isnan";
  c5_info[7].dominantType = "double";
  c5_info[7].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elmat/isnan.m";
  c5_info[7].fileTimeLo = 1286840360U;
  c5_info[7].fileTimeHi = 0U;
  c5_info[7].mFileTimeLo = 0U;
  c5_info[7].mFileTimeHi = 0U;
  c5_info[8].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_cosd.m";
  c5_info[8].name = "eml_scalar_rem90";
  c5_info[8].dominantType = "double";
  c5_info[8].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m";
  c5_info[8].fileTimeLo = 1320911036U;
  c5_info[8].fileTimeHi = 0U;
  c5_info[8].mFileTimeLo = 0U;
  c5_info[8].mFileTimeHi = 0U;
  c5_info[9].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m";
  c5_info[9].name = "rem";
  c5_info[9].dominantType = "double";
  c5_info[9].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/rem.m";
  c5_info[9].fileTimeLo = 1286840348U;
  c5_info[9].fileTimeHi = 0U;
  c5_info[9].mFileTimeLo = 0U;
  c5_info[9].mFileTimeHi = 0U;
  c5_info[10].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/rem.m";
  c5_info[10].name = "eml_scalar_eg";
  c5_info[10].dominantType = "double";
  c5_info[10].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c5_info[10].fileTimeLo = 1286840396U;
  c5_info[10].fileTimeHi = 0U;
  c5_info[10].mFileTimeLo = 0U;
  c5_info[10].mFileTimeHi = 0U;
  c5_info[11].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/rem.m";
  c5_info[11].name = "eml_scalexp_alloc";
  c5_info[11].dominantType = "double";
  c5_info[11].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m";
  c5_info[11].fileTimeLo = 1286840396U;
  c5_info[11].fileTimeHi = 0U;
  c5_info[11].mFileTimeLo = 0U;
  c5_info[11].mFileTimeHi = 0U;
  c5_info[12].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/rem.m";
  c5_info[12].name = "eml_scalar_rem";
  c5_info[12].dominantType = "double";
  c5_info[12].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_rem.m";
  c5_info[12].fileTimeLo = 1286840332U;
  c5_info[12].fileTimeHi = 0U;
  c5_info[12].mFileTimeLo = 0U;
  c5_info[12].mFileTimeHi = 0U;
  c5_info[13].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m";
  c5_info[13].name = "abs";
  c5_info[13].dominantType = "double";
  c5_info[13].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/abs.m";
  c5_info[13].fileTimeLo = 1286840294U;
  c5_info[13].fileTimeHi = 0U;
  c5_info[13].mFileTimeLo = 0U;
  c5_info[13].mFileTimeHi = 0U;
  c5_info[14].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/abs.m";
  c5_info[14].name = "eml_scalar_abs";
  c5_info[14].dominantType = "double";
  c5_info[14].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m";
  c5_info[14].fileTimeLo = 1286840312U;
  c5_info[14].fileTimeHi = 0U;
  c5_info[14].mFileTimeLo = 0U;
  c5_info[14].mFileTimeHi = 0U;
  c5_info[15].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m";
  c5_info[15].name = "mtimes";
  c5_info[15].dominantType = "double";
  c5_info[15].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/ops/mtimes.m";
  c5_info[15].fileTimeLo = 1289541292U;
  c5_info[15].fileTimeHi = 0U;
  c5_info[15].mFileTimeLo = 0U;
  c5_info[15].mFileTimeHi = 0U;
  c5_info[16].context = "";
  c5_info[16].name = "mtimes";
  c5_info[16].dominantType = "double";
  c5_info[16].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/ops/mtimes.m";
  c5_info[16].fileTimeLo = 1289541292U;
  c5_info[16].fileTimeHi = 0U;
  c5_info[16].mFileTimeLo = 0U;
  c5_info[16].mFileTimeHi = 0U;
  c5_info[17].context = "";
  c5_info[17].name = "sind";
  c5_info[17].dominantType = "double";
  c5_info[17].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/sind.m";
  c5_info[17].fileTimeLo = 1286840350U;
  c5_info[17].fileTimeHi = 0U;
  c5_info[17].mFileTimeLo = 0U;
  c5_info[17].mFileTimeHi = 0U;
  c5_info[18].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/sind.m";
  c5_info[18].name = "eml_scalar_sind";
  c5_info[18].dominantType = "double";
  c5_info[18].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_sind.m";
  c5_info[18].fileTimeLo = 1320911036U;
  c5_info[18].fileTimeHi = 0U;
  c5_info[18].mFileTimeLo = 0U;
  c5_info[18].mFileTimeHi = 0U;
  c5_info[19].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_sind.m";
  c5_info[19].name = "isfinite";
  c5_info[19].dominantType = "double";
  c5_info[19].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elmat/isfinite.m";
  c5_info[19].fileTimeLo = 1286840358U;
  c5_info[19].fileTimeHi = 0U;
  c5_info[19].mFileTimeLo = 0U;
  c5_info[19].mFileTimeHi = 0U;
  c5_info[20].context =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_sind.m";
  c5_info[20].name = "eml_scalar_rem90";
  c5_info[20].dominantType = "double";
  c5_info[20].resolved =
    "[ILXE]C:/Program Files/MATLAB/R2012a/toolbox/eml/lib/matlab/elfun/eml_scalar_rem90.m";
  c5_info[20].fileTimeLo = 1320911036U;
  c5_info[20].fileTimeHi = 0U;
  c5_info[20].mFileTimeLo = 0U;
  c5_info[20].mFileTimeHi = 0U;
}

static real_T c5_cosd(SFc5_boatInstanceStruct *chartInstance, real_T c5_x)
{
  real_T c5_b_x;
  c5_b_x = c5_x;
  c5_b_cosd(chartInstance, &c5_b_x);
  return c5_b_x;
}

static void c5_eml_scalar_rem90(SFc5_boatInstanceStruct *chartInstance, real_T
  c5_x, real_T *c5_n, real_T *c5_y)
{
  real_T c5_b_x;
  real_T c5_xk;
  real_T c5_c_x;
  real_T c5_d_x;
  real_T c5_e_x;
  real_T c5_ix;
  real_T c5_f_x;
  real_T c5_g_x;
  real_T c5_b;
  real_T c5_b_b;
  real_T c5_c_b;
  real_T c5_d_b;
  real_T c5_e_b;
  c5_b_x = c5_x;
  c5_xk = c5_b_x;
  c5_c_x = c5_xk;
  c5_x = muDoubleScalarRem(c5_c_x, 360.0);
  c5_d_x = c5_x;
  c5_e_x = c5_d_x;
  c5_ix = muDoubleScalarAbs(c5_e_x);
  if (c5_ix > 180.0) {
    if (c5_x > 0.0) {
      c5_x -= 360.0;
    } else {
      c5_x += 360.0;
    }

    c5_f_x = c5_x;
    c5_g_x = c5_f_x;
    c5_ix = muDoubleScalarAbs(c5_g_x);
  }

  if (c5_ix <= 45.0) {
    c5_b = c5_x;
    *c5_y = 0.017453292519943295 * c5_b;
    *c5_n = 0.0;
  } else if (c5_ix <= 135.0) {
    if (c5_x > 0.0) {
      c5_b_b = c5_x - 90.0;
      *c5_y = 0.017453292519943295 * c5_b_b;
      *c5_n = 1.0;
    } else {
      c5_c_b = c5_x + 90.0;
      *c5_y = 0.017453292519943295 * c5_c_b;
      *c5_n = -1.0;
    }
  } else if (c5_x > 0.0) {
    c5_d_b = c5_x - 180.0;
    *c5_y = 0.017453292519943295 * c5_d_b;
    *c5_n = 2.0;
  } else {
    c5_e_b = c5_x + 180.0;
    *c5_y = 0.017453292519943295 * c5_e_b;
    *c5_n = -2.0;
  }
}

static real_T c5_sind(SFc5_boatInstanceStruct *chartInstance, real_T c5_x)
{
  real_T c5_b_x;
  c5_b_x = c5_x;
  c5_b_sind(chartInstance, &c5_b_x);
  return c5_b_x;
}

static const mxArray *c5_d_sf_marshallOut(void *chartInstanceVoid, void
  *c5_inData)
{
  const mxArray *c5_mxArrayOutData = NULL;
  int32_T c5_u;
  const mxArray *c5_y = NULL;
  SFc5_boatInstanceStruct *chartInstance;
  chartInstance = (SFc5_boatInstanceStruct *)chartInstanceVoid;
  c5_mxArrayOutData = NULL;
  c5_u = *(int32_T *)c5_inData;
  c5_y = NULL;
  sf_mex_assign(&c5_y, sf_mex_create("y", &c5_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c5_mxArrayOutData, c5_y, FALSE);
  return c5_mxArrayOutData;
}

static int32_T c5_d_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance,
  const mxArray *c5_u, const emlrtMsgIdentifier *c5_parentId)
{
  int32_T c5_y;
  int32_T c5_i15;
  sf_mex_import(c5_parentId, sf_mex_dup(c5_u), &c5_i15, 1, 6, 0U, 0, 0U, 0);
  c5_y = c5_i15;
  sf_mex_destroy(&c5_u);
  return c5_y;
}

static void c5_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c5_mxArrayInData, const char_T *c5_varName, void *c5_outData)
{
  const mxArray *c5_b_sfEvent;
  const char_T *c5_identifier;
  emlrtMsgIdentifier c5_thisId;
  int32_T c5_y;
  SFc5_boatInstanceStruct *chartInstance;
  chartInstance = (SFc5_boatInstanceStruct *)chartInstanceVoid;
  c5_b_sfEvent = sf_mex_dup(c5_mxArrayInData);
  c5_identifier = c5_varName;
  c5_thisId.fIdentifier = c5_identifier;
  c5_thisId.fParent = NULL;
  c5_y = c5_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c5_b_sfEvent),
    &c5_thisId);
  sf_mex_destroy(&c5_b_sfEvent);
  *(int32_T *)c5_outData = c5_y;
  sf_mex_destroy(&c5_mxArrayInData);
}

static uint8_T c5_e_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance,
  const mxArray *c5_b_is_active_c5_boat, const char_T *c5_identifier)
{
  uint8_T c5_y;
  emlrtMsgIdentifier c5_thisId;
  c5_thisId.fIdentifier = c5_identifier;
  c5_thisId.fParent = NULL;
  c5_y = c5_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c5_b_is_active_c5_boat),
    &c5_thisId);
  sf_mex_destroy(&c5_b_is_active_c5_boat);
  return c5_y;
}

static uint8_T c5_f_emlrt_marshallIn(SFc5_boatInstanceStruct *chartInstance,
  const mxArray *c5_u, const emlrtMsgIdentifier *c5_parentId)
{
  uint8_T c5_y;
  uint8_T c5_u0;
  sf_mex_import(c5_parentId, sf_mex_dup(c5_u), &c5_u0, 1, 3, 0U, 0, 0U, 0);
  c5_y = c5_u0;
  sf_mex_destroy(&c5_u);
  return c5_y;
}

static void c5_b_cosd(SFc5_boatInstanceStruct *chartInstance, real_T *c5_x)
{
  real_T c5_b_x;
  real_T c5_c_x;
  boolean_T c5_b;
  boolean_T c5_b0;
  real_T c5_d_x;
  boolean_T c5_b_b;
  boolean_T c5_b1;
  boolean_T c5_c_b;
  real_T c5_e_x;
  real_T c5_n;
  real_T c5_b_n;
  c5_b_x = *c5_x;
  c5_c_x = c5_b_x;
  c5_b = muDoubleScalarIsInf(c5_c_x);
  c5_b0 = !c5_b;
  c5_d_x = c5_b_x;
  c5_b_b = muDoubleScalarIsNaN(c5_d_x);
  c5_b1 = !c5_b_b;
  c5_c_b = (c5_b0 && c5_b1);
  if (!c5_c_b) {
    *c5_x = rtNaN;
  } else {
    c5_eml_scalar_rem90(chartInstance, *c5_x, &c5_n, &c5_e_x);
    c5_b_n = c5_n;
    *c5_x = c5_e_x;
    if (c5_b_n == 0.0) {
      *c5_x = muDoubleScalarCos(*c5_x);
    } else if (c5_b_n == 1.0) {
      *c5_x = -muDoubleScalarSin(*c5_x);
    } else if (c5_b_n == -1.0) {
      *c5_x = muDoubleScalarSin(*c5_x);
    } else {
      *c5_x = -muDoubleScalarCos(*c5_x);
    }
  }
}

static void c5_b_sind(SFc5_boatInstanceStruct *chartInstance, real_T *c5_x)
{
  real_T c5_b_x;
  real_T c5_c_x;
  boolean_T c5_b;
  boolean_T c5_b2;
  real_T c5_d_x;
  boolean_T c5_b_b;
  boolean_T c5_b3;
  boolean_T c5_c_b;
  real_T c5_e_x;
  real_T c5_n;
  real_T c5_b_n;
  c5_b_x = *c5_x;
  c5_c_x = c5_b_x;
  c5_b = muDoubleScalarIsInf(c5_c_x);
  c5_b2 = !c5_b;
  c5_d_x = c5_b_x;
  c5_b_b = muDoubleScalarIsNaN(c5_d_x);
  c5_b3 = !c5_b_b;
  c5_c_b = (c5_b2 && c5_b3);
  if (!c5_c_b) {
    *c5_x = rtNaN;
  } else {
    c5_eml_scalar_rem90(chartInstance, *c5_x, &c5_n, &c5_e_x);
    c5_b_n = c5_n;
    *c5_x = c5_e_x;
    if (c5_b_n == 0.0) {
      *c5_x = muDoubleScalarSin(*c5_x);
    } else if (c5_b_n == 1.0) {
      *c5_x = muDoubleScalarCos(*c5_x);
    } else if (c5_b_n == -1.0) {
      *c5_x = -muDoubleScalarCos(*c5_x);
    } else {
      *c5_x = -muDoubleScalarSin(*c5_x);
    }
  }
}

static void init_dsm_address_info(SFc5_boatInstanceStruct *chartInstance)
{
}

/* SFunction Glue Code */
void sf_c5_boat_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(958595255U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1924419845U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3873057514U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2274308560U);
}

mxArray *sf_c5_boat_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("fUz7kIw3xGy04zvXmiyf9E");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,11,3,dataFields);

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

static const mxArray *sf_get_sim_state_info_c5_boat(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"v1\",},{M[8],M[0],T\"is_active_c5_boat\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c5_boat_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc5_boatInstanceStruct *chartInstance;
    chartInstance = (SFc5_boatInstanceStruct *) ((ChartInfoStruct *)
      (ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart(_boatMachineNumber_,
          5,
          1,
          1,
          12,
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
          init_script_number_translation(_boatMachineNumber_,
            chartInstance->chartNumber);
          sf_debug_set_chart_disable_implicit_casting(_boatMachineNumber_,
            chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(_boatMachineNumber_,
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
          _SFD_SET_DATA_PROPS(9,1,1,0,"friX");
          _SFD_SET_DATA_PROPS(10,1,1,0,"friY");
          _SFD_SET_DATA_PROPS(11,1,1,0,"friZ");
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
        _SFD_CV_INIT_EML(0,1,1,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,950);
        _SFD_TRANS_COV_WTS(0,0,0,1,0);
        if (chartAlreadyPresent==0) {
          _SFD_TRANS_COV_MAPS(0,
                              0,NULL,NULL,
                              0,NULL,NULL,
                              1,NULL,NULL,
                              0,NULL,NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 6;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c5_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 6;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c5_sf_marshallOut,(MexInFcnForType)
            c5_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c5_c_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(11,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c5_b_sf_marshallOut,(MexInFcnForType)NULL);

        {
          real_T *c5_rx1;
          real_T *c5_rx2;
          real_T *c5_ry1;
          real_T *c5_ry2;
          real_T *c5_m;
          real_T *c5_I;
          real_T *c5_friX;
          real_T *c5_friY;
          real_T *c5_friZ;
          real_T (*c5_v0)[6];
          real_T (*c5_v1)[6];
          real_T (*c5_u0)[4];
          c5_friZ = (real_T *)ssGetInputPortSignal(chartInstance->S, 10);
          c5_friY = (real_T *)ssGetInputPortSignal(chartInstance->S, 9);
          c5_friX = (real_T *)ssGetInputPortSignal(chartInstance->S, 8);
          c5_I = (real_T *)ssGetInputPortSignal(chartInstance->S, 7);
          c5_m = (real_T *)ssGetInputPortSignal(chartInstance->S, 6);
          c5_ry2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 5);
          c5_ry1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
          c5_rx2 = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c5_rx1 = (real_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c5_u0 = (real_T (*)[4])ssGetInputPortSignal(chartInstance->S, 1);
          c5_v1 = (real_T (*)[6])ssGetOutputPortSignal(chartInstance->S, 1);
          c5_v0 = (real_T (*)[6])ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, *c5_v0);
          _SFD_SET_DATA_VALUE_PTR(1U, *c5_v1);
          _SFD_SET_DATA_VALUE_PTR(2U, *c5_u0);
          _SFD_SET_DATA_VALUE_PTR(3U, c5_rx1);
          _SFD_SET_DATA_VALUE_PTR(4U, c5_rx2);
          _SFD_SET_DATA_VALUE_PTR(5U, c5_ry1);
          _SFD_SET_DATA_VALUE_PTR(6U, c5_ry2);
          _SFD_SET_DATA_VALUE_PTR(7U, c5_m);
          _SFD_SET_DATA_VALUE_PTR(8U, c5_I);
          _SFD_SET_DATA_VALUE_PTR(9U, c5_friX);
          _SFD_SET_DATA_VALUE_PTR(10U, c5_friY);
          _SFD_SET_DATA_VALUE_PTR(11U, c5_friZ);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(_boatMachineNumber_,
        chartInstance->chartNumber,chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization()
{
  return "udV9ZJWqetDp2TpJbYmoZD";
}

static void sf_opaque_initialize_c5_boat(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc5_boatInstanceStruct*) chartInstanceVar)->S,0);
  initialize_params_c5_boat((SFc5_boatInstanceStruct*) chartInstanceVar);
  initialize_c5_boat((SFc5_boatInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c5_boat(void *chartInstanceVar)
{
  enable_c5_boat((SFc5_boatInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c5_boat(void *chartInstanceVar)
{
  disable_c5_boat((SFc5_boatInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c5_boat(void *chartInstanceVar)
{
  sf_c5_boat((SFc5_boatInstanceStruct*) chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c5_boat(SimStruct* S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c5_boat((SFc5_boatInstanceStruct*)
    chartInfo->chartInstance);         /* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c5_boat();/* state var info */
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

extern void sf_internal_set_sim_state_c5_boat(SimStruct* S, const mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c5_boat();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c5_boat((SFc5_boatInstanceStruct*)chartInfo->chartInstance,
                        mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c5_boat(SimStruct* S)
{
  return sf_internal_get_sim_state_c5_boat(S);
}

static void sf_opaque_set_sim_state_c5_boat(SimStruct* S, const mxArray *st)
{
  sf_internal_set_sim_state_c5_boat(S, st);
}

static void sf_opaque_terminate_c5_boat(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc5_boatInstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
    }

    finalize_c5_boat((SFc5_boatInstanceStruct*) chartInstanceVar);
    free((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }

  unload_boat_optimization_info();
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc5_boat((SFc5_boatInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c5_boat(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c5_boat((SFc5_boatInstanceStruct*)(((ChartInfoStruct *)
      ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c5_boat(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_boat_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      5);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,5,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,5,
      "gatewayCannotBeInlinedMultipleTimes"));
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
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,5,11);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,5,1);
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,5);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(1251512525U));
  ssSetChecksum1(S,(2445195301U));
  ssSetChecksum2(S,(3461680930U));
  ssSetChecksum3(S,(1338676370U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
}

static void mdlRTW_c5_boat(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c5_boat(SimStruct *S)
{
  SFc5_boatInstanceStruct *chartInstance;
  chartInstance = (SFc5_boatInstanceStruct *)malloc(sizeof
    (SFc5_boatInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc5_boatInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c5_boat;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c5_boat;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c5_boat;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c5_boat;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c5_boat;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c5_boat;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c5_boat;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c5_boat;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c5_boat;
  chartInstance->chartInfo.mdlStart = mdlStart_c5_boat;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c5_boat;
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

void c5_boat_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c5_boat(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c5_boat(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c5_boat(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c5_boat_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
