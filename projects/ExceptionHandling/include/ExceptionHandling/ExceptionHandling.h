#ifndef __EXCEPTIONHANDLING_H__
#define __EXCEPTIONHANDLING_H__

#include "ExceptionHandling/EXCEPTIONHANDLING_Export.h"
#include "ExceptionHandling/version.h"
#include "ace/Singleton.h"

namespace ExceptionHandling_ns
{
    extern "C" EXCEPTIONHANDLING_Export void ExceptionHandling_get_version(int & major, int & minor, int & patch);

#ifdef ACE_WIN32
    // TO_DO (doesn't work in multithread)
    //class EXCEPTIONHANDLING_Export DumpFile
    //{
    //public:
    //    DumpFile();
    //    void set(const std::string & file);
    //    std::string get();

    //protected:
    //    std::string file_;
    //};



    typedef LONG (WINAPI *pRedirectedSetUnhandledExceptionFilter)(EXCEPTION_POINTERS *);

    class EXCEPTIONHANDLING_Export ExceptionHandling
    {
        friend ACE_Singleton<ExceptionHandling_ns::ExceptionHandling, ACE_Recursive_Thread_Mutex>;

    public:
        enum {SIZE = 6};

        ExceptionHandling();
        ~ExceptionHandling();

        void set_specific_handler(void (*seh)(int));

    protected:
        pRedirectedSetUnhandledExceptionFilter pOrigAddress;
        DWORD oldProtect;
        DWORD myProtect;
        BYTE JMP[SIZE];
        BYTE oldBytes[SIZE];
#else
    class EXCEPTIONHANDLING_Export ExceptionHandling
    {
        //friend ACE_Singleton<ExceptionHandling_ns::ExceptionHandling, ACE_Recursive_Thread_Mutex>;

    public:
        enum {SIZE = 6};

        ExceptionHandling();
        ~ExceptionHandling();

        void set_specific_handler(void (*seh)(int));

    protected:
#ifdef ACE_WIN32
        pRedirectedSetUnhandledExceptionFilter pOrigAddress;
        DWORD oldProtect;
        DWORD myProtect;
        BYTE JMP[SIZE];
        BYTE oldBytes[SIZE];
#endif
#endif
    };

}

typedef ACE_Singleton<ExceptionHandling_ns::ExceptionHandling, ACE_Recursive_Thread_Mutex> ExceptionHandlingSingleton;

#ifdef ACE_WIN32
    #pragma warning(push)
    #pragma warning(disable: 4231) // will disapear when VC is conform to C++11
#endif
    EXCEPTIONHANDLING_SINGLETON_DECLARE(ACE_Singleton, ExceptionHandling_ns::ExceptionHandling, ACE_Recursive_Thread_Mutex);
#ifdef ACE_WIN32
    #pragma warning(push)
#endif


#define EXCEPTIONHANDLING ExceptionHandlingSingleton::instance()

#endif // __EXCEPTIONHANDLING_H__
