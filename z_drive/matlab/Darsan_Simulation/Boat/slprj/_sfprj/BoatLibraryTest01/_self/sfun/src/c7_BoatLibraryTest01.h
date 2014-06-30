#ifndef __c7_BoatLibraryTest01_h__
#define __c7_BoatLibraryTest01_h__

/* Include files */
#include "sfc_sf.h"
#include "sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc7_BoatLibraryTest01InstanceStruct
#define typedef_SFc7_BoatLibraryTest01InstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c7_sfEvent;
  boolean_T c7_isStable;
  boolean_T c7_doneDoubleBufferReInit;
  uint8_T c7_is_active_c7_BoatLibraryTest01;
} SFc7_BoatLibraryTest01InstanceStruct;

#endif                                 /*typedef_SFc7_BoatLibraryTest01InstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c7_BoatLibraryTest01_get_eml_resolved_functions_info
  (void);

/* Function Definitions */
extern void sf_c7_BoatLibraryTest01_get_check_sum(mxArray *plhs[]);
extern void c7_BoatLibraryTest01_method_dispatcher(SimStruct *S, int_T method,
  void *data);

#endif
