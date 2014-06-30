#ifndef __c6_systemTest_h__
#define __c6_systemTest_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc6_systemTestInstanceStruct
#define typedef_SFc6_systemTestInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c6_sfEvent;
  boolean_T c6_isStable;
  boolean_T c6_doneDoubleBufferReInit;
  uint8_T c6_is_active_c6_systemTest;
} SFc6_systemTestInstanceStruct;

#endif                                 /*typedef_SFc6_systemTestInstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c6_systemTest_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c6_systemTest_get_check_sum(mxArray *plhs[]);
extern void c6_systemTest_method_dispatcher(SimStruct *S, int_T method, void
  *data);

#endif
