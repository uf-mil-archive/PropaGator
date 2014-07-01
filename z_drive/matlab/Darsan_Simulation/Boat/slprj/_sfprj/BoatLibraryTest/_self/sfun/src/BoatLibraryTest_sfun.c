/* Include files */

#include "BoatLibraryTest_sfun.h"
#include "BoatLibraryTest_sfun_debug_macros.h"
#include "c1_BoatLibraryTest.h"
#include "c2_BoatLibraryTest.h"
#include "c3_BoatLibraryTest.h"
#include "c4_BoatLibraryTest.h"
#include "c5_BoatLibraryTest.h"
#include "c6_BoatLibraryTest.h"
#include "c7_BoatLibraryTest.h"
#include "c8_BoatLibraryTest.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */
uint32_T _BoatLibraryTestMachineNumber_;
real_T _sfTime_;

/* Function Declarations */

/* Function Definitions */
void BoatLibraryTest_initializer(void)
{
}

void BoatLibraryTest_terminator(void)
{
}

/* SFunction Glue Code */
unsigned int sf_BoatLibraryTest_method_dispatcher(SimStruct *simstructPtr,
  unsigned int chartFileNumber, const char* specsCksum, int_T method, void *data)
{
  if (chartFileNumber==1) {
    c1_BoatLibraryTest_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==2) {
    c2_BoatLibraryTest_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==3) {
    c3_BoatLibraryTest_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==4) {
    c4_BoatLibraryTest_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==5) {
    c5_BoatLibraryTest_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==6) {
    c6_BoatLibraryTest_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==7) {
    c7_BoatLibraryTest_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==8) {
    c8_BoatLibraryTest_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  return 0;
}

unsigned int sf_BoatLibraryTest_process_check_sum_call( int nlhs, mxArray *
  plhs[], int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[20];
  if (nrhs<1 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the checksum */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"sf_get_check_sum"))
    return 0;
  plhs[0] = mxCreateDoubleMatrix( 1,4,mxREAL);
  if (nrhs>1 && mxIsChar(prhs[1])) {
    mxGetString(prhs[1], commandName,sizeof(commandName)/sizeof(char));
    commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
    if (!strcmp(commandName,"machine")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(506444599U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(691533560U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2401039551U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2933411977U);
    } else if (!strcmp(commandName,"exportedFcn")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0U);
    } else if (!strcmp(commandName,"makefile")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(562580907U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(802465415U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3930324344U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(4033948795U);
    } else if (nrhs==3 && !strcmp(commandName,"chart")) {
      unsigned int chartFileNumber;
      chartFileNumber = (unsigned int)mxGetScalar(prhs[2]);
      switch (chartFileNumber) {
       case 1:
        {
          extern void sf_c1_BoatLibraryTest_get_check_sum(mxArray *plhs[]);
          sf_c1_BoatLibraryTest_get_check_sum(plhs);
          break;
        }

       case 2:
        {
          extern void sf_c2_BoatLibraryTest_get_check_sum(mxArray *plhs[]);
          sf_c2_BoatLibraryTest_get_check_sum(plhs);
          break;
        }

       case 3:
        {
          extern void sf_c3_BoatLibraryTest_get_check_sum(mxArray *plhs[]);
          sf_c3_BoatLibraryTest_get_check_sum(plhs);
          break;
        }

       case 4:
        {
          extern void sf_c4_BoatLibraryTest_get_check_sum(mxArray *plhs[]);
          sf_c4_BoatLibraryTest_get_check_sum(plhs);
          break;
        }

       case 5:
        {
          extern void sf_c5_BoatLibraryTest_get_check_sum(mxArray *plhs[]);
          sf_c5_BoatLibraryTest_get_check_sum(plhs);
          break;
        }

       case 6:
        {
          extern void sf_c6_BoatLibraryTest_get_check_sum(mxArray *plhs[]);
          sf_c6_BoatLibraryTest_get_check_sum(plhs);
          break;
        }

       case 7:
        {
          extern void sf_c7_BoatLibraryTest_get_check_sum(mxArray *plhs[]);
          sf_c7_BoatLibraryTest_get_check_sum(plhs);
          break;
        }

       case 8:
        {
          extern void sf_c8_BoatLibraryTest_get_check_sum(mxArray *plhs[]);
          sf_c8_BoatLibraryTest_get_check_sum(plhs);
          break;
        }

       default:
        ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0.0);
      }
    } else if (!strcmp(commandName,"target")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3031367619U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(4001028638U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3978939492U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(838979348U);
    } else {
      return 0;
    }
  } else {
    ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2042111477U);
    ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(556461664U);
    ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(4094747158U);
    ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2246586334U);
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_BoatLibraryTest_autoinheritance_info( int nlhs, mxArray * plhs[],
  int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[32];
  char aiChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the autoinheritance_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_autoinheritance_info"))
    return 0;
  mxGetString(prhs[2], aiChksum,sizeof(aiChksum)/sizeof(char));
  aiChksum[(sizeof(aiChksum)/sizeof(char)-1)] = '\0';

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(aiChksum, "LwBSOFHVUw2CZNJad6rVgB") == 0) {
          extern mxArray *sf_c1_BoatLibraryTest_get_autoinheritance_info(void);
          plhs[0] = sf_c1_BoatLibraryTest_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 2:
      {
        if (strcmp(aiChksum, "r261P7oQ7yYACgo90ZoikD") == 0) {
          extern mxArray *sf_c2_BoatLibraryTest_get_autoinheritance_info(void);
          plhs[0] = sf_c2_BoatLibraryTest_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 3:
      {
        if (strcmp(aiChksum, "yt5GYGCQClI2Rxe18L1xxD") == 0) {
          extern mxArray *sf_c3_BoatLibraryTest_get_autoinheritance_info(void);
          plhs[0] = sf_c3_BoatLibraryTest_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 4:
      {
        if (strcmp(aiChksum, "JqvDPqs3k0HfLb6PY7k3KH") == 0) {
          extern mxArray *sf_c4_BoatLibraryTest_get_autoinheritance_info(void);
          plhs[0] = sf_c4_BoatLibraryTest_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 5:
      {
        if (strcmp(aiChksum, "LwBSOFHVUw2CZNJad6rVgB") == 0) {
          extern mxArray *sf_c5_BoatLibraryTest_get_autoinheritance_info(void);
          plhs[0] = sf_c5_BoatLibraryTest_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 6:
      {
        if (strcmp(aiChksum, "yt5GYGCQClI2Rxe18L1xxD") == 0) {
          extern mxArray *sf_c6_BoatLibraryTest_get_autoinheritance_info(void);
          plhs[0] = sf_c6_BoatLibraryTest_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 7:
      {
        if (strcmp(aiChksum, "2KTvX6M063zBeTPsReAdrH") == 0) {
          extern mxArray *sf_c7_BoatLibraryTest_get_autoinheritance_info(void);
          plhs[0] = sf_c7_BoatLibraryTest_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 8:
      {
        if (strcmp(aiChksum, "grq1SQLwJfFNzlDzvtBHoH") == 0) {
          extern mxArray *sf_c8_BoatLibraryTest_get_autoinheritance_info(void);
          plhs[0] = sf_c8_BoatLibraryTest_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_BoatLibraryTest_get_eml_resolved_functions_info( int nlhs,
  mxArray * plhs[], int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[64];
  if (nrhs<2 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the get_eml_resolved_functions_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_eml_resolved_functions_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        extern const mxArray
          *sf_c1_BoatLibraryTest_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c1_BoatLibraryTest_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 2:
      {
        extern const mxArray
          *sf_c2_BoatLibraryTest_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c2_BoatLibraryTest_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 3:
      {
        extern const mxArray
          *sf_c3_BoatLibraryTest_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c3_BoatLibraryTest_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 4:
      {
        extern const mxArray
          *sf_c4_BoatLibraryTest_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c4_BoatLibraryTest_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 5:
      {
        extern const mxArray
          *sf_c5_BoatLibraryTest_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c5_BoatLibraryTest_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 6:
      {
        extern const mxArray
          *sf_c6_BoatLibraryTest_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c6_BoatLibraryTest_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 7:
      {
        extern const mxArray
          *sf_c7_BoatLibraryTest_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c7_BoatLibraryTest_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 8:
      {
        extern const mxArray
          *sf_c8_BoatLibraryTest_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c8_BoatLibraryTest_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_BoatLibraryTest_third_party_uses_info( int nlhs, mxArray * plhs[],
  int nrhs, const mxArray * prhs[] )
{
  char commandName[64];
  char tpChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the third_party_uses_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  mxGetString(prhs[2], tpChksum,sizeof(tpChksum)/sizeof(char));
  tpChksum[(sizeof(tpChksum)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_third_party_uses_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(tpChksum, "dIl1gPLmURJUV5mMbWa3oE") == 0) {
          extern mxArray *sf_c1_BoatLibraryTest_third_party_uses_info(void);
          plhs[0] = sf_c1_BoatLibraryTest_third_party_uses_info();
          break;
        }
      }

     case 2:
      {
        if (strcmp(tpChksum, "EJ6zHb8Iag2nlxlw6fFhvH") == 0) {
          extern mxArray *sf_c2_BoatLibraryTest_third_party_uses_info(void);
          plhs[0] = sf_c2_BoatLibraryTest_third_party_uses_info();
          break;
        }
      }

     case 3:
      {
        if (strcmp(tpChksum, "twHOX9Ca5h6lSYMZ9DWSQC") == 0) {
          extern mxArray *sf_c3_BoatLibraryTest_third_party_uses_info(void);
          plhs[0] = sf_c3_BoatLibraryTest_third_party_uses_info();
          break;
        }
      }

     case 4:
      {
        if (strcmp(tpChksum, "UHokd3DikTW0BTsytXacFB") == 0) {
          extern mxArray *sf_c4_BoatLibraryTest_third_party_uses_info(void);
          plhs[0] = sf_c4_BoatLibraryTest_third_party_uses_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "dIl1gPLmURJUV5mMbWa3oE") == 0) {
          extern mxArray *sf_c5_BoatLibraryTest_third_party_uses_info(void);
          plhs[0] = sf_c5_BoatLibraryTest_third_party_uses_info();
          break;
        }
      }

     case 6:
      {
        if (strcmp(tpChksum, "twHOX9Ca5h6lSYMZ9DWSQC") == 0) {
          extern mxArray *sf_c6_BoatLibraryTest_third_party_uses_info(void);
          plhs[0] = sf_c6_BoatLibraryTest_third_party_uses_info();
          break;
        }
      }

     case 7:
      {
        if (strcmp(tpChksum, "kq1Mfuz1B0dh1e53Jcyh9E") == 0) {
          extern mxArray *sf_c7_BoatLibraryTest_third_party_uses_info(void);
          plhs[0] = sf_c7_BoatLibraryTest_third_party_uses_info();
          break;
        }
      }

     case 8:
      {
        if (strcmp(tpChksum, "XRFbqpw99xbtBQsPsPoV9C") == 0) {
          extern mxArray *sf_c8_BoatLibraryTest_third_party_uses_info(void);
          plhs[0] = sf_c8_BoatLibraryTest_third_party_uses_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

unsigned int sf_BoatLibraryTest_updateBuildInfo_args_info( int nlhs, mxArray *
  plhs[], int nrhs, const mxArray * prhs[] )
{
  char commandName[64];
  char tpChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the updateBuildInfo_args_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  mxGetString(prhs[2], tpChksum,sizeof(tpChksum)/sizeof(char));
  tpChksum[(sizeof(tpChksum)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_updateBuildInfo_args_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(tpChksum, "dIl1gPLmURJUV5mMbWa3oE") == 0) {
          extern mxArray *sf_c1_BoatLibraryTest_updateBuildInfo_args_info(void);
          plhs[0] = sf_c1_BoatLibraryTest_updateBuildInfo_args_info();
          break;
        }
      }

     case 2:
      {
        if (strcmp(tpChksum, "EJ6zHb8Iag2nlxlw6fFhvH") == 0) {
          extern mxArray *sf_c2_BoatLibraryTest_updateBuildInfo_args_info(void);
          plhs[0] = sf_c2_BoatLibraryTest_updateBuildInfo_args_info();
          break;
        }
      }

     case 3:
      {
        if (strcmp(tpChksum, "twHOX9Ca5h6lSYMZ9DWSQC") == 0) {
          extern mxArray *sf_c3_BoatLibraryTest_updateBuildInfo_args_info(void);
          plhs[0] = sf_c3_BoatLibraryTest_updateBuildInfo_args_info();
          break;
        }
      }

     case 4:
      {
        if (strcmp(tpChksum, "UHokd3DikTW0BTsytXacFB") == 0) {
          extern mxArray *sf_c4_BoatLibraryTest_updateBuildInfo_args_info(void);
          plhs[0] = sf_c4_BoatLibraryTest_updateBuildInfo_args_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "dIl1gPLmURJUV5mMbWa3oE") == 0) {
          extern mxArray *sf_c5_BoatLibraryTest_updateBuildInfo_args_info(void);
          plhs[0] = sf_c5_BoatLibraryTest_updateBuildInfo_args_info();
          break;
        }
      }

     case 6:
      {
        if (strcmp(tpChksum, "twHOX9Ca5h6lSYMZ9DWSQC") == 0) {
          extern mxArray *sf_c6_BoatLibraryTest_updateBuildInfo_args_info(void);
          plhs[0] = sf_c6_BoatLibraryTest_updateBuildInfo_args_info();
          break;
        }
      }

     case 7:
      {
        if (strcmp(tpChksum, "kq1Mfuz1B0dh1e53Jcyh9E") == 0) {
          extern mxArray *sf_c7_BoatLibraryTest_updateBuildInfo_args_info(void);
          plhs[0] = sf_c7_BoatLibraryTest_updateBuildInfo_args_info();
          break;
        }
      }

     case 8:
      {
        if (strcmp(tpChksum, "XRFbqpw99xbtBQsPsPoV9C") == 0) {
          extern mxArray *sf_c8_BoatLibraryTest_updateBuildInfo_args_info(void);
          plhs[0] = sf_c8_BoatLibraryTest_updateBuildInfo_args_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

void BoatLibraryTest_debug_initialize(struct SfDebugInstanceStruct*
  debugInstance)
{
  _BoatLibraryTestMachineNumber_ = sf_debug_initialize_machine(debugInstance,
    "BoatLibraryTest","sfun",0,8,0,0,0);
  sf_debug_set_machine_event_thresholds(debugInstance,
    _BoatLibraryTestMachineNumber_,0,0);
  sf_debug_set_machine_data_thresholds(debugInstance,
    _BoatLibraryTestMachineNumber_,0);
}

void BoatLibraryTest_register_exported_symbols(SimStruct* S)
{
}

static mxArray* sRtwOptimizationInfoStruct= NULL;
mxArray* load_BoatLibraryTest_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct==NULL) {
    sRtwOptimizationInfoStruct = sf_load_rtw_optimization_info("BoatLibraryTest",
      "BoatLibraryTest");
    mexMakeArrayPersistent(sRtwOptimizationInfoStruct);
  }

  return(sRtwOptimizationInfoStruct);
}

void unload_BoatLibraryTest_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct!=NULL) {
    mxDestroyArray(sRtwOptimizationInfoStruct);
    sRtwOptimizationInfoStruct = NULL;
  }
}
