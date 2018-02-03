#ifndef _CONST
#define _CONST
#define MAX_TB 6144//最大容许传输的码块大小
//#define NaN NULL//填充比特
#define NaN 0
#define d_NaN (std::numeric_limits<double>::quiet_NaN())
#define INF (std::numeric_limits<double>::infinity())
#define RUN 1//开始计时
#define  END 2//结束计时，并打印时间
#define _LTEA_NO_MULTI_THREAD
#define SingleAnt 1
#define spatialMultiplexing 3
#define tranmitDiversity 2
#define largeDelayCDD 13
#define FFT_SIZE 2048
#define normal 1
#define extend 0
#define Freq 1
#define Time_Select 2
#define complex_i std::complex<double>(0,1)
#define PedA 100
#define PedB 101
#define VehA 102
#define  VehB 103
#define  Ideal 104
#define MMSE 105
#define ZF 106
#define LTE_MAP 107
#define  LTE_LOGMAP 108
#define LTE_LOGMAX 109
#define LTE_TABLE 110
#endif

