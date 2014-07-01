/* Include files */

#include "BoatLibraryTest03_sfun.h"
#include "BoatLibraryTest03_sfun_debug_macros.h"
#include "c2_BoatLibraryTest03.h"
#include "c5_BoatLibraryTest03.h"
#include "c6_BoatLibraryTest03.h"
#include "c7_BoatLibraryTest03.h"
#include "c11_BoatLibraryTest03.h"
#include "c12_BoatLibraryTest03.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */
uint32_T _BoatLibraryTest03MachineNumber_;
real_T _sfTime_;

/* Function Declarations */

/* Function Definitions */
void BoatLibraryTest03_initializer(void)
{
}

void BoatLibraryTest03_terminator(void)
{
}

/* SFunction Glue Code */
unsigned int sf_BoatLibraryTest03_method_dispatcher(SimStruct *simstructPtr,
  unsigned int chartFileNumber, const char* specsCksum, int_T method, void *data)
{
  if (chartFileNumber==2) {
    c2_BoatLibraryTest03_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==5) {
    c5_BoatLibraryTest03_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==6) {
    c6_BoatLibraryTest03_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==7) {
    c7_BoatLibraryTest03_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==11) {
    c11_BoatLibraryTest03_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==12) {
    c12_BoatLibraryTest03_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  return 0;
}

unsigned int sf_BoatLibraryTest03_process_check_sum_call( int nlhs, mxArray *
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
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(83139890U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2212867952U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1590383797U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(4094843440U);
    } else if (!strcmp(commandName,"exportedFcn")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0U);
    } else if (!strcmp(commandName,"makefile")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1065035556U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2758729292U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2948443672U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2813696923U);
    } else if (nrhs==3 && !strcmp(commandName,"chart")) {
      unsigned int chartFileNumber;
      chartFileNumber = (unsigned int)mxGetScalar(prhs[2]);
      switch (chartFileNumber) {
       case 2:
        {
          extern void sf_c2_BoatLibraryTest03_get_check_sum(mxArray *plhs[]);
          sf_c2_BoatLibraryTest03_get_check_sum(plhs);
          break;
        }

       case 5:
        {
          extern void sf_c5_BoatLibraryTest03_get_check_sum(mxArray *plhs[]);
          sf_c5_BoatLibraryTest03_get_check_sum(plhs);
          break;
        }

       case 6:
        {
          extern void sf_c6_BoatLibraryTest03_get_check_sum(mxArray *plhs[]);
          sf_c6_BoatLibraryTest03_get_check_sum(plhs);
          break;
        }

       case 7:
        {
          extern void sf_c7_BoatLibraryTest03_get_check_sum(mxArray *plhs[]);
          sf_c7_BoatLibraryTest03_get_check_sum(plhs);
          break;
        }

       case 11:
        {
          extern void sf_c11_BoatLibraryTest03_get_check_sum(mxArray *plhs[]);
          sf_c11_BoatLibraryTest03_get_check_sum(plhs);
          break;
        }

       case 12:
        {
          extern void sf_c12_BoatLibraryTest03_get_check_sum(mxArray *plhs[]);
          sf_c12_BoatLibraryTest03_get_check_sum(plhs);
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
    ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3432015806U);
    ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1487627703U);
    ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(382262306U);
    ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1419206755U);
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_BoatLibraryTest03_autoinheritance_info( int nlhs, mxArray *
  plhs[], int nrhs, const mxArray * prhs[] )
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
     case 2:
      {
        if (strcmp(aiChksum, "0LeYC0X72T9fIz2xt4ikMH") == 0) {
          extern mxArray *sf_c2_BoatLibraryTest03_get_autoinheritance_info(void);
          plhs[0] = sf_c2_BoatLibraryTest03_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 5:
      {
        if (strcmp(aiChksum, "LwBSOFHVUw2CZNJad6rVgB") == 0) {
          extern mxArray *sf_c5_BoatLibraryTest03_get_autoinheritance_info(void);
          plhs[0] = sf_c5_BoatLibraryTest03_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 6:
      {
        if (strcmp(aiChksum, "yt5GYGCQClI2Rxe18L1xxD") == 0) {
          extern mxArray *sf_c6_BoatLibraryTest03_get_autoinheritance_info(void);
          plhs[0] = sf_c6_BoatLibraryTest03_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 7:
      {
        if (strcmp(aiChksum, "2KTvX6M063zBeTPsReAdrH") == 0) {
          extern mxArray *sf_c7_BoatLibraryTest03_get_autoinheritance_info(void);
          plhs[0] = sf_c7_BoatLibraryTest03_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 11:
      {
        if (strcmp(aiChksum, "VltSs9SWyaAs2NH2EbKZ7E") == 0) {
          extern mxArray *sf_c11_BoatLibraryTest03_get_autoinheritance_info(void);
          plhs[0] = sf_c11_BoatLibraryTest03_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 12:
      {
        if (strcmp(aiChksum, "FM7LWvF0B869LcIphGqgaD") == 0) {
          extern mxArray *sf_c12_BoatLibraryTest03_get_autoinheritance_info(void);
          plhs[0] = sf_c12_BoatLibraryTest03_get_autoinheritance_info();
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

unsigned int sf_BoatLibraryTest03_get_eml_resolved_functions_info( int nlhs,
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
     case 2:
      {
        extern const mxArray
          *sf_c2_BoatLibraryTest03_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c2_BoatLibraryTest03_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 5:
      {
        extern const mxArray
          *sf_c5_BoatLibraryTest03_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c5_BoatLibraryTest03_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 6:
      {
        extern const mxArray
          *sf_c6_BoatLibraryTest03_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c6_BoatLibraryTest03_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 7:
      {
        extern const mxArray
          *sf_c7_BoatLibraryTest03_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c7_BoatLibraryTest03_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 11:
      {
        extern const mxArray
          *sf_c11_BoatLibraryTest03_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c11_BoatLibraryTest03_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 12:
      {
        extern const mxArray
          *sf_c12_BoatLibraryTest03_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c12_BoatLibraryTest03_get_eml_resolved_functions_info();
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

unsigned int sf_BoatLibraryTest03_third_party_uses_info( int nlhs, mxArray *
  plhs[], int nrhs, const mxArray * prhs[] )
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
     case 2:
      {
        if (strcmp(tpChksum, "0mJVB7xWVJDFfL53Pc5S1D") == 0) {
          extern mxArray *sf_c2_BoatLibraryTest03_third_party_uses_info(void);
          plhs[0] = sf_c2_BoatLibraryTest03_third_party_uses_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "dIl1gPLmURJUV5mMbWa3oE") == 0) {
          extern mxArray *sf_c5_BoatLibraryTest03_third_party_uses_info(void);
          plhs[0] = sf_c5_BoatLibraryTest03_third_party_uses_info();
          break;
        }
      }

     case 6:
      {
        if (strcmp(tpChksum, "twHOX9Ca5h6lSYMZ9DWSQC") == 0) {
          extern mxArray *sf_c6_BoatLibraryTest03_third_party_uses_info(void);
          plhs[0] = sf_c6_BoatLibraryTest03_third_party_uses_info();
          break;
        }
      }

     case 7:
      {
        if (strcmp(tpChksum, "kq1Mfuz1B0dh1e53Jcyh9E") == 0) {
          extern mxArray *sf_c7_BoatLibraryTest03_third_party_uses_info(void);
          plhs[0] = sf_c7_BoatLibraryTest03_third_party_uses_info();
          break;
        }
      }

     case 11:
      {
        if (strcmp(tpChksum, "iGSdlSQay9NUAjqz2dp4qH") == 0) {
          extern mxArray *sf_c11_BoatLibraryTest03_third_party_uses_info(void);
          plhs[0] = sf_c11_BoatLibraryTest03_third_party_uses_info();
          break;
        }
      }

     case 12:
      {
        if (strcmp(tpChksum, "m8h0Ic7g5d9DDUhONhCT7B") == 0) {
          extern mxArray *sf_c12_BoatLibraryTest03_third_party_uses_info(void);
          plhs[0] = sf_c12_BoatLibraryTest03_third_party_uses_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

unsigned int sf_BoatLibraryTest03_updateBuildInfo_args_info( int nlhs, mxArray *
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
     case 2:
      {
        if (strcmp(tpChksum, "0mJVB7xWVJDFfL53Pc5S1D") == 0) {
          extern mxArray *sf_c2_BoatLibraryTest03_updateBuildInfo_args_info(void);
          plhs[0] = sf_c2_BoatLibraryTest03_updateBuildInfo_args_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "dIl1gPLmURJUV5mMbWa3oE") == 0) {
          extern mxArray *sf_c5_BoatLibraryTest03_updateBuildInfo_args_info(void);
          plhs[0] = sf_c5_BoatLibraryTest03_updateBuildInfo_args_info();
          break;
        }
      }

     case 6:
      {
        if (strcmp(tpChksum, "twHOX9Ca5h6lSYMZ9DWSQC") == 0) {
          extern mxArray *sf_c6_BoatLibraryTest03_updateBuildInfo_args_info(void);
          plhs[0] = sf_c6_BoatLibraryTest03_updateBuildInfo_args_info();
          break;
        }
      }

     case 7:
      {
        if (strcmp(tpChksum, "kq1Mfuz1B0dh1e53Jcyh9E") == 0) {
          extern mxArray *sf_c7_BoatLibraryTest03_updateBuildInfo_args_info(void);
          plhs[0] = sf_c7_BoatLibraryTest03_updateBuildInfo_args_info();
          break;
        }
      }

     case 11:
      {
        if (strcmp(tpChksum, "iGSdlSQay9NUAjqz2dp4qH") == 0) {
          extern mxArray *sf_c11_BoatLibraryTest03_updateBuildInfo_args_info
            (void);
          plhs[0] = sf_c11_BoatLibraryTest03_updateBuildInfo_args_info();
          break;
        }
      }

     case 12:
      {
        if (strcmp(tpChksum, "m8h0Ic7g5d9DDUhONhCT7B") == 0) {
          extern mxArray *sf_c12_BoatLibraryTest03_updateBuildInfo_args_info
            (void);
          plhs[0] = sf_c12_BoatLibraryTest03_updateBuildInfo_args_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

void BoatLibraryTest03_debug_initialize(struct SfDebugInstanceStruct*
  debugInstance)
{
  _BoatLibraryTest03MachineNumber_ = sf_debug_initialize_machine(debugInstance,
    "BoatLibraryTest03","sfun",0,6,0,0,0);
  sf_debug_set_machine_event_thresholds(debugInstance,
    _BoatLibraryTest03MachineNumber_,0,0);
  sf_debug_set_machine_data_thresholds(debugInstance,
    _BoatLibraryTest03MachineNumber_,0);
}

void BoatLibraryTest03_register_exported_symbols(SimStruct* S)
{
}

static mxArray* sRtwOptimizationInfoStruct= NULL;
mxArray* load_BoatLibraryTest03_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct==NULL) {
    sRtwOptimizationInfoStruct = sf_load_rtw_optimization_info(
      "BoatLibraryTest03", "BoatLibraryTest03");
    mexMakeArrayPersistent(sRtwOptimizationInfoStruct);
  }

  return(sRtwOptimizationInfoStruct);
}

void unload_BoatLibraryTest03_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct!=NULL) {
    mxDestroyArray(sRtwOptimizationInfoStruct);
    sRtwOptimizationInfoStruct = NULL;
  }
}
