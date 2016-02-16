#ifndef __EXCEPTIONHANDLING_EVC_H__
#define __EXCEPTIONHANDLING_EVC_H__

#include "BasicEventHandling/BasicEventHandling.h"

BEGIN_EVENT_DECL_NAMESPACE(ExceptionHandlingEvents)
	STATIC_EVENT_DECL(ev_unhandled_exception)
	STATIC_EVENT_DECL(ev_caught_terminate)
	STATIC_EVENT_DECL(ev_signal_caught)
	STATIC_EVENT_DECL(ev_pure_call_exception)
	STATIC_EVENT_DECL(ev_caught_memory_depletion)
	STATIC_EVENT_DECL(ev_preventing_SetUnhandledExceptionFilter_failure)
END_EVENT_NAMESPACE()


#endif // __EXCEPTIONHANDLING_EVC_H__
