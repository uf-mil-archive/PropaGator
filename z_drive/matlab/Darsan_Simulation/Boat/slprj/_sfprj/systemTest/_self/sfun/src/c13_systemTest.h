#ifndef __c13_systemTest_h__
#define __c13_systemTest_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc13_systemTestInstanceStruct
#define typedef_SFc13_systemTestInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c13_sfEvent;
  boolean_T c13_isStable;
  boolean_T c13_doneDoubleBufferReInit;
  uint8_T c13_is_active_c13_systemTest;
} SFc13_systemTestInstanceStruct;

#endif                                 /*typedef_SFc13_systemTestInstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c13_systemTest_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c13_systemTest_get_check_sum(mxArray *plhs[]);
extern void c13_systemTest_method_dispatcher(SimStruct *S, int_T method, void
  *data);

#endif
