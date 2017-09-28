#include <dtvs/comm_def.h>
#include <dtvs/comm_head.h>
#include "mod_ext/iTimer.h"
#include "main.h"
#include "dtvs/recommend/RecommendSystemConfig.h"


#include "performance_optimization.h"
#if !defined(WIN64) && !defined(WIN32)
#include <execinfo.h>
static pthread_key_t thread_log_key;

int CPerformanceOptimization::init()
{
	pthread_key_create(&thread_log_key, NULL);
	return HOMED_OK;
}

int CPerformanceOptimization::uninit()
{
	pthread_key_delete(thread_log_key);
	return HOMED_OK;
}

int CPerformanceOptimization::start(const char *fun, int line)
{
	PerfLog* pl = (PerfLog*)pthread_getspecific(thread_log_key);
	if (pl == NULL)
	{
		pl = new PerfLog;
		if (pl == NULL)
		{
			LOG4CPLUS_WARN(g_oServerLogger, "in performance, can't alloc the PerfLog");
			return HOMED_ERR;
		}
		pthread_setspecific(thread_log_key, (void*)pl);
	}
	in(fun, line);
	return HOMED_OK;
}

int CPerformanceOptimization::end(int interval, const char *fun, int line)
{
	PerfLog* pl = (PerfLog*)pthread_getspecific(thread_log_key);
	if (pl == NULL || pl->callTime.empty())
	{
		//LOG4CPLUS_WARN(g_oServerLogger, "in performance, the thread local variable is null, can't free anything");
		return HOMED_ERR;
	}

	if (pl->callTime.size() > 1)
		return out(fun, line);

	if (iTimer::m_tNowMS >= interval*1000 + pl->callTime.top())
	{
		vector<string>::iterator it;
		for(it = pl->log.begin(); it != pl->log.end(); it++)
			LOG4CPLUS_INFO(g_oServerLogger, "[Perf]" << *it);

		LOG4CPLUS_INFO(g_oServerLogger, "[Perf]" << pl->callTime.size() << " " << fun << ":" << line << " begin:" << pl->callTime.top() << "ms spend:" << (iTimer::m_tNowMS - pl->callTime.top()) << "ms");
		pl->callTime.pop();
	}
	pthread_setspecific(thread_log_key, NULL);
	pl->log.clear();
	delete pl;
	return HOMED_OK;
}

int CPerformanceOptimization::in(const char *fun, int line)
{
	PerfLog* pl = (PerfLog*)pthread_getspecific(thread_log_key);
	if (pl == NULL)
	{
		//LOG4CPLUS_WARN(g_oServerLogger, "in performance, the thread local variable is null, can't inset anything");
		return HOMED_ERR;
	}
	pl->callTime.push(iTimer::m_tNowMS);
	return HOMED_OK;
}

int CPerformanceOptimization::out(const char *fun, int line)
{
	PerfLog* pl = (PerfLog*)pthread_getspecific(thread_log_key);
	if (pl == NULL)
	{
		//LOG4CPLUS_WARN(g_oServerLogger, "in performance, the thread local variable is null, can't out anything");
		return HOMED_ERR;
	}
	if (iTimer::m_tNowMS > 1 + pl->callTime.top())
	{
		ostringstream ts;
		ts << pl->callTime.size() << " " << fun << ":" << line << " begin:" << pl->callTime.top() << "ms spend:" << (iTimer::m_tNowMS - pl->callTime.top())<< "ms";
		pl->log.push_back(ts.str()); 
	}
	pl->callTime.pop();
	return HOMED_OK;
}

int CPerformanceOptimization::stack()
{
	const int size = 100;
	void *buffer[size];
	char** string;
	int nptrs;
	nptrs = backtrace(buffer, size);
	string = backtrace_symbols(buffer, nptrs);
	for (int i = 0; i < nptrs; i++)
		LOG4CPLUS_INFO(g_oServerLogger, "[Perf][stack]" << string[i]);
	free(string);
}
#endif

CSelfServicePerf::CSelfServicePerf(string s) 
	:m_string(s), m_open(false)
{
   	if (g_recommend_system_config->openPerf()) 
	{
		m_open = true;
		PerfInOtherStr(m_string.c_str());
	}
}

CSelfServicePerf::~CSelfServicePerf()
{
   	if (m_open)
	{
		PerfOutOtherStr(m_string.c_str());
	}
}
