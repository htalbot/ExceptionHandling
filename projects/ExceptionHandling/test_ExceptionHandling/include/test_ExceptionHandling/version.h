
#ifndef __TEST_EXCEPTIONHANDLING_VERSION_H__
#define __TEST_EXCEPTIONHANDLING_VERSION_H__

#define TEST_EXCEPTIONHANDLING_MAJOR 1
#define TEST_EXCEPTIONHANDLING_MINOR 0
#define TEST_EXCEPTIONHANDLING_PATCH 3


#if defined _MSC_VER
#   ifdef _DEBUG
        const char TEST_EXCEPTIONHANDLING_DebugFlag[] = "DEBUG";
#   else
        const char TEST_EXCEPTIONHANDLING_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char TEST_EXCEPTIONHANDLING_DebugFlag[] = "DEBUG";
#   else
        const char TEST_EXCEPTIONHANDLING_DebugFlag[] = "RELEASE";
#   endif
#endif


#endif // __TEST_EXCEPTIONHANDLING_VERSION_H__
