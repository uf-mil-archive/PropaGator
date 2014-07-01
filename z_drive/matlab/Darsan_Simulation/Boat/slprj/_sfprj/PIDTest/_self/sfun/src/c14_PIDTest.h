#ifndef __c14_PIDTest_h__
#define __c14_PIDTest_h__

/* Include files */
#include "sfc_sf.h"
#include "sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc14_PIDTestInstanceStruct
#define typedef_SFc14_PIDTestInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c14_sfEvent;
  boolean_T c14_isStable;
  boolean_T c14_doneDoubleBufferReInit;
  uint8_T c14_is_active_c14_PIDTest;
} SFc14_PIDTestInstanceStruct;

#endif                                 /*typedef_SFc14_PIDTestInstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c14_PIDTest_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c14_PIDTest_get_check_sum(mxArray *plhs[]);
extern void c14_PIDTest_method_dispatcher(SimStruct *S, int_T method, void *data);

#endif
