
//#include "ace/OS.h"
#include "ace/Log_Msg.h"
//#include "ace/Guard_T.h"
#include "ExceptionHandling/ExceptionHandling.h"
#include "ExceptionHandling.evc.h"
#include "Killing/Killing.h"
//#include <dbghelp.h>
//#include <new.h>
//#include <sstream>

#ifdef ACE_WIN32
//#pragma comment ( lib, "dbghelp.lib" )
#endif


//ACE_Recursive_Thread_Mutex g_dump_file_mutex_;

void (*specific_exception_handler)(int) = 0;

namespace ExceptionHandling_ns
{
    void ExceptionHandling_get_version(int & major, int & minor, int & patch)
    {
        major = EXCEPTIONHANDLING_MAJOR;
        minor = EXCEPTIONHANDLING_MINOR;
        patch = EXCEPTIONHANDLING_PATCH;
    }

#ifdef ACE_WIN32
    //// ****************************
    //// ***** Static dump file *****
    //// ****************************
    //DumpFile::DumpFile()
    //{
    //    //ACE_Object_Manager::at_exit(this);
    //}

    //void DumpFile::set(const std::string & file)
    //{
    //    file_ = file;
    //}

    //std::string DumpFile::get()
    //{
    //    return file_;
    //}

    //DumpFile g_dump_file;


    // ****************************
    // ***** static functions *****
    // ****************************
    //void CreateMiniDump(EXCEPTION_POINTERS* pep)
    //{
        //ACE_Guard<ACE_Recursive_Thread_Mutex> guard(g_dump_file_mutex_);

        //ACE_DEBUG((LM_DEBUG, "000000000000000000000\n"));

        //HANDLE hFile = CreateFile(g_dump_file.get().c_str(), GENERIC_READ | GENERIC_WRITE,
        //    0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

        //ACE_DEBUG((LM_DEBUG, "aaaaaaaaaaaaaaaaaaaaa\n"));

        //if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) )
        //{
        //ACE_DEBUG((LM_DEBUG, "bbbbbbbbbbbbbbbbbbb\n"));
        //    // Create the minidump
        //    MINIDUMP_EXCEPTION_INFORMATION mdei;

        //    mdei.ThreadId           = GetCurrentThreadId();
        //    mdei.ExceptionPointers  = pep;
        //    mdei.ClientPointers     = FALSE;

        //    MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(MiniDumpWithFullMemory |
        //        MiniDumpWithFullMemoryInfo |
        //        MiniDumpWithHandleData |
        //        MiniDumpWithThreadInfo |
        //        MiniDumpWithUnloadedModules );

        //ACE_DEBUG((LM_DEBUG, "ccccccccccccccccccc\n"));

        //    BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
        //        hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 );

        //ACE_DEBUG((LM_DEBUG, "dddddddddddddddddddddddddddd\n"));

        //    if( !rv )
        //        ACE_DEBUG((LM_ERROR, "MiniDumpWriteDump failed. Error: %u \n", GetLastError()));
        //    else
        //        ACE_DEBUG((LM_ERROR, "Minidump created.\n"));

        //    CloseHandle( hFile );

        //ACE_DEBUG((LM_DEBUG, "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n"));

        //}
        //else
        //{
        //    ACE_DEBUG((LM_ERROR, "CreateFile failed. Error: %u (hFile = %d)\n", GetLastError(), hFile));
        //}

        //ACE_DEBUG((LM_DEBUG, "11111111111111111111111111\n"));
    //}

//    void my_terminate_handler()
//    {
//        ACE_DEBUG((LM_CRITICAL, "Caught terminate\n"));
//
//        EVENT_HANDLER_SYSTEMLOG->handle_event(_SRC,
//                    ExceptionHandlingEvents::ev_caught_terminate,
//                    LM_ERROR,
//                    "'TERMINATE'");
//
//        //CreateMiniDump(0);
//
////        exit(ExceptionHandlingEvents::ev_caught_terminate.id());
//    }

    void signal_handler(int signal)
    {
        std::string signal_text;

        switch(signal)
        {
        case SIGABRT:
            signal_text = "SIGABRT";
            break;

        case SIGFPE:
            signal_text = "SIGFPE";
            break;

        case SIGILL:
            signal_text = "SIGILL";
            break;

        case SIGINT:
            signal_text = "SIGINT";
            break;

        case SIGSEGV:
            signal_text = "SIGSEGV";
            break;

        case SIGTERM:
            signal_text = "SIGTERM";
            break;
        default:
            signal_text = "unknown signal";
            break;
        };

        ACE_DEBUG((LM_CRITICAL, "signal caught %s.\n", signal_text.c_str()));

        EVENT_HANDLER_SYSTEMLOG->handle_event(_SRC,
                    ExceptionHandlingEvents::ev_signal_caught,
                    LM_ERROR,
                    signal_text.c_str());

        if (specific_exception_handler)
        {
            specific_exception_handler(0);
        }

        ACE_DEBUG((LM_CRITICAL, "SUICIDE...\n"));
        KILLING->suicide();

        //CreateMiniDump(0);

        //exit(ExceptionHandlingEvents::ev_signal_caught.id());
    }

    //void my_pure_call_handler(void)
    //{
    //    ACE_DEBUG((LM_CRITICAL, "Caught C++ pure call exception.\n"));

    //    EVENT_HANDLER_SYSTEMLOG->handle_event(_SRC,
    //                ExceptionHandlingEvents::ev_pure_call_exception,
    //                LM_ERROR);

    //    //CreateMiniDump(0);

        //if (specific_exception_handler)
        //{
        //    specific_exception_handler(0);
        //}

  //      ACE_DEBUG((LM_CRITICAL, "SUICIDE...\n"));
        //KILLING->suicide();

    //    //exit(ExceptionHandlingEvents::ev_pure_call_exception.id());
    //}

    //int handle_program_memory_depletion( size_t )
    //{
    //    ACE_DEBUG((LM_CRITICAL, "Caught memory depletion.\n"));

    //    EVENT_HANDLER_SYSTEMLOG->handle_event(_SRC,
    //                ExceptionHandlingEvents::ev_caught_memory_depletion,
    //                LM_ERROR);

        //if (specific_exception_handler)
        //{
        //    specific_exception_handler(0);
        //}

  //      ACE_DEBUG((LM_CRITICAL, "SUICIDE...\n"));
        //KILLING->suicide();

    //    //exit(ExceptionHandlingEvents::ev_caught_memory_depletion.id());

    //    return 0; // TO_DO: return the right value
    //}

#endif


    // ********************************
    // ***** CPMExceptionHandling *****
    // ********************************

    static BOOL PreventSetUnhandledExceptionFilter()
    {
        HMODULE hKernel32 = LoadLibrary("kernel32.dll");
        if (hKernel32 == NULL) return FALSE;
        void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
        if (pOrgEntry == NULL) return FALSE;

#ifdef _M_IX86
        // Code for x86:
        // 33 C0                xor         eax,eax 
        // C2 04 00             ret         4
        unsigned char szExecute[] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };
#elif _M_X64
        // 33 C0                xor         eax,eax
        // C3                   ret 
        unsigned char szExecute[] = { 0x33, 0xC0, 0xC3 };
#else
#error "The following code only works for x86 and x64!"
#endif

        SIZE_T bytesWritten = 0;
        BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
            pOrgEntry, szExecute, sizeof(szExecute), &bytesWritten);
        return bRet;
    }

    LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *lpTopLevelExceptionFilter)
    {
        //// TODO: MiniDumpWriteDump
        //FatalAppExit(0, _T("Unhandled Exception occured!"));
        //return EXCEPTION_CONTINUE_SEARCH;

        if (specific_exception_handler)
        {
            specific_exception_handler(0);
        }

        ACE_DEBUG((LM_CRITICAL, "SUICIDE...\n"));
        KILLING->suicide();

        return /*EXCEPTION_EXECUTE_HANDLER*/EXCEPTION_CONTINUE_SEARCH;
    }

    ExceptionHandling::ExceptionHandling()
    {
#ifdef ACE_WIN32
        //ACE_DEBUG((LM_DEBUG, "%s\n", _SRC));

//        char szModuleName[2048];
//
//#       ifdef ACE_WIN32
//            GetModuleFileNameA(NULL, szModuleName, 2048);
//#       else
//            // TO_DO
//#       endif

        //std::stringstream ss;
        //ss << szModuleName;
        //ss << ".dmp";
        //g_dump_file.set(ss.str());

        _CrtSetReportMode(_CRT_ASSERT, 0);
        _CrtSetReportMode(_CRT_ERROR, 0);
        _CrtSetReportMode(_CRT_WARN, 0);

        DWORD dwMode = SetErrorMode(SEM_NOGPFAULTERRORBOX);
        SetErrorMode(dwMode | SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);

        //set_terminate(my_terminate_handler);
        //_set_purecall_handler(my_pure_call_handler);
        //_set_new_handler(handle_program_memory_depletion);
        signal(SIGABRT, signal_handler);
        signal(SIGFPE, signal_handler);
        signal(SIGILL, signal_handler);
        signal(SIGINT, signal_handler);
        signal(SIGSEGV, signal_handler);
        signal(SIGTERM, signal_handler);

        SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

        // http://blog.kalmbach-software.de/2008/04/02/unhandled-exceptions-in-vc8-and-above-for-x86-and-x64
        // http://blog.kalmbach-software.de/2013/05/23/improvedpreventsetunhandledexceptionfilter/
        BOOL bRet = PreventSetUnhandledExceptionFilter();
        if (!bRet)
        {
            EVENT_HANDLER_SYSTEMLOG->handle_event(_SRC,
                ExceptionHandlingEvents::ev_preventing_SetUnhandledExceptionFilter_failure,
                LM_ERROR);
        }

#endif
    }

    ExceptionHandling::~ExceptionHandling()
    {
    }

    void ExceptionHandling::set_specific_handler(void (*seh)(int))
    {
        specific_exception_handler = seh;
    }
}
