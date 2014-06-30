#ifndef __c5_test_h__
#define __c5_test_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc5_testInstanceStruct
#define typedef_SFc5_testInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c5_sfEvent;
  boolean_T c5_isStable;
  boolean_T c5_doneDoubleBufferReInit;
  uint8_T c5_is_active_c5_test;
} SFc5_testInstanceStruct;

#endif                                 /*typedef_SFc5_testInstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c5_test_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c5_test_get_check_sum(mxArray *plhs[]);
extern void c5_test_method_dispatcher(SimStruct *S, int_T method, void *data);

#endif
