
#pragma once

#define EXCEPTIONHANDLING_MAJOR 1
#define EXCEPTIONHANDLING_MINOR 0
#define EXCEPTIONHANDLING_PATCH 3


#if defined _MSC_VER
#   ifdef _DEBUG
        const char EXCEPTIONHANDLING_DebugFlag[] = "DEBUG";
#   else
        const char EXCEPTIONHANDLING_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char EXCEPTIONHANDLING_DebugFlag[] = "DEBUG";
#   else
        const char EXCEPTIONHANDLING_DebugFlag[] = "RELEASE";
#   endif
#endif


