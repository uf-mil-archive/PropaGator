#ifndef __c11_BoatLibraryTest04_h__
#define __c11_BoatLibraryTest04_h__

/* Include files */
#include "sfc_sf.h"
#include "sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc11_BoatLibraryTest04InstanceStruct
#define typedef_SFc11_BoatLibraryTest04InstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c11_sfEvent;
  boolean_T c11_isStable;
  boolean_T c11_doneDoubleBufferReInit;
  uint8_T c11_is_active_c11_BoatLibraryTest04;
} SFc11_BoatLibraryTest04InstanceStruct;

#endif                                 /*typedef_SFc11_BoatLibraryTest04InstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c11_BoatLibraryTest04_get_eml_resolved_functions_info
  (void);

/* Function Definitions */
extern void sf_c11_BoatLibraryTest04_get_check_sum(mxArray *plhs[]);
extern void c11_BoatLibraryTest04_method_dispatcher(SimStruct *S, int_T method,
  void *data);

#endif
