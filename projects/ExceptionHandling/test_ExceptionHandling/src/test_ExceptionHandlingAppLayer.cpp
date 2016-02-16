
#include "test_ExceptionHandlingAppLayer.h"
#include "ExceptionHandling/ExceptionHandling.h"
#include "ace/Task.h"
#include <vector>
#include <stdio.h>

using namespace ExceptionHandling_ns;


void checkDivZero(void)
{
    int x=100, y=73;
    y -= y;
    x = x / y;
}

void OutOfBoundsVectorCrash()
{
    std::vector<int> v;
    v[0] = 5;
}

void VirtualFunctionCallCrash()
{
    struct B
    {
        B()
        {
            //std::cout << "Pure Virtual Function Call crash!" << std::endl;
            Bar();
        }

        virtual void Foo() = 0;

        void Bar()
        {
            Foo();
        }
    };

    struct D: public B
    {
        void Foo()
        {
        }
    };

    B* b = new D;
    // Just to silence the warning C4101:
    //    'VirtualFunctionCallCrash::B::Foo' : unreferenced local variable
    b->Foo();
}

void wildGooseChase(int i)
{
    for (;i < i*50; i += (i&0xff)^41) {
        wildGooseChase(i);
    }
}


class Thread: public ACE_Task_Base
{
public:
    Thread(): ok_(false) {}
    ~Thread() {printf("~Thread()\n");}
    bool ok() { return ok_; }
protected:
    int svc()
    {
        printf(0); // signal(SIGSEGV)
        ok_ = true;
        return 0;
    }

    bool ok_;
};



class Content
{
public:
    ~Content()
    {
        //checkDivZero(); // unhandled exception, not reported in release mode
        //printf(NULL);
        OutOfBoundsVectorCrash();
        //VirtualFunctionCallCrash(); // '_set_purecall_handler()'
    }
};

class Container
{
public:
    ~Container()
    {
        ExceptionHandling * ex = EXCEPTIONHANDLING;
    }

protected:
    Content content_;
};


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

Test_ExceptionHandlingAppLayer::Test_ExceptionHandlingAppLayer()
{
}


bool Test_ExceptionHandlingAppLayer::run()
{
    printf("***** Test_ExceptionHandlingAppLayer (%s) *****\n\n", TEST_EXCEPTIONHANDLING_DebugFlag);

    int major, minor, patch;
    ExceptionHandling_get_version(major, minor, patch);
    printf("ExceptionHandling version: %d.%d.%d\n\n", major, minor, patch);

#define TEST_CONTAINER
#ifdef TEST_CONTAINER
    {
        Container container;
    }
#else
    ExceptionHandling * ex = EXCEPTIONHANDLING;
    ExceptionHandling * ex2 = EXCEPTIONHANDLING; // to test singleton: ok
#endif


    // ***** UnhandledExceptions *****

    //checkDivZero(); // unhandled exception, not reported in release mode



    // ***** 'signal' handlers *****

    //printf(NULL); // signal(SIGSEGV)

    //OutOfBoundsVectorCrash(); // signal(SIGSEGV)

    //abort(); // signal(SIGABRT)

    //int * p;
    //printf("%d\n", *p); // signal(SIGSEGV)

    //while(true); // signal(SIGINT) when CTRL+C



    // ***** 'set' handlers *****

    //unexpected(); // 'set_terminate()' (not unhandled exception!)

    //terminate(); // 'set_terminate()'

    //VirtualFunctionCallCrash(); // '_set_purecall_handler()'



    // ***** nothing reported *****

    //wildGooseChase(0xdeadbeef); // nothing reported, anytime



    // ***** threads... *****

    //Thread t;
    //t.activate();
    //t.wait();
    //if (t.ok())
    //{
    //    printf("No exception in Thread.\n");
    //}
    //else
    //{
    //    printf("Exception in Thread.\n");
    //}

    //Thread * t = new Thread;
    //t->activate();
    //t->wait();
    //if (t->ok())
    //{
    //    printf("No exception in Thread.\n");
    //    delete t;
    //}
    //else
    //{
    //    printf("Exception in Thread.\n");
    //    //delete t; // In some cases (OpenDDSLayer) it's preferable to not delete...
    //}


    printf("**********>>> END.\n");

    return true;
}

