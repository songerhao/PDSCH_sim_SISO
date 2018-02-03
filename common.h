#ifndef _LIB
#define _LIB
#pragma once
#ifdef _DEBUG
#pragma comment(lib,"itpp_debug.lib")
#else
#pragma comment(lib,"itpp.lib")
#endif
#pragma comment(lib,"blas_win32.lib")
#pragma comment(lib,"lapack_win32.lib")
#pragma comment(lib,"libfftw3-3.lib")
//#include "error_log.h"
#include <assert.h>
//#include "run_time.h"
#include <itpp/base/array.h>
#include <itpp/itcomm.h>
//#define _LTEA_DEBUG
#define _LTEA_NO_MULTI_THREAD
#define _CODE_BASED
using namespace itpp;
#endif
