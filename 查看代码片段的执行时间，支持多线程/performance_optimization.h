/*******************************************************************
*******************************************************************/

#ifndef __PERFORMANCE_OPTIMIZATION_H__
#define __PERFORMANCE_OPTIMIZATION_H__

#include <string>
#include <vector>
#include <stack>
#if !defined(WIN64) && !defined(WIN32)
#include <pthread.h>
#endif

using namespace std;
typedef struct tagPerfLog 
{
	vector<string> log;
	stack<int> callTime;
}PerfLog;

class CPerformanceOptimization 
{
public:
	static int init();
	static int uninit();
	static int start(const char* fun, int line);
	static int end(int interval, const char* fun, int line);
	static int in(const char *fun, int line);
	static int out(const char *fun, int line);
	static int stack();
};

#if !defined(DISABLE_BASIC_PERFORMANCE_OPTIMIZATION) && !defined(WIN64) && !defined(WIN32)
#define PerfInit() CPerformanceOptimization::init()
#define PerfUninit() CPerformanceOptimization::uninit()
#define PerfStart() int __PerfFlags__ = CPerformanceOptimization::start(__FUNCTION__, __LINE__)
#define PerfEnd(a) if (__PerfFlags__ == HOMED_OK) CPerformanceOptimization::end(a, __FUNCTION__, __LINE__)
#define PerfInSelf() CPerformanceOptimization::in(__FUNCTION__, __LINE__)
#define PerfOutSelf() CPerformanceOptimization::out(__FUNCTION__, __LINE__)
#define PerfInOther(a) CPerformanceOptimization::in(#a, __LINE__)
#define PerfOutOther(a) CPerformanceOptimization::out(#a, __LINE__)
#define PerfStack() CPerformanceOptimization::stack()
#define PerfInOtherStr(a) CPerformanceOptimization::in(a, __LINE__)
#define PerfOutOtherStr(a) CPerformanceOptimization::out(a, __LINE__)
#else
#define PerfInit()
#define PerfUninit()
#define PerfStart()
#define PerfEnd(a)
#define PerfInSelf()
#define PerfOutSelf()
#define PerfInOther(a)
#define PerfOutOther(a)
#define PerfStack()
#define PerfInOtherStr(a)
#define PerfOutOtherStr(a)
#endif

class CSelfServicePerf
{
public:
	CSelfServicePerf(string s);
	~CSelfServicePerf();
private:
	string m_string;
	bool m_open;
};

#endif /* __PERFORMANCE_OPTIMIZATION_H__ */
