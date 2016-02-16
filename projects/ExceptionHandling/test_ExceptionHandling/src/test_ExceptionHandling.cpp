
#include "test_ExceptionHandlingAppLayer.h"
#include "ace/Init_ACE.h"

int main(int, char *[])
{
    ACE::init();

    Test_ExceptionHandlingAppLayer app;

    int rc = app.run();

    ACE::fini();

    return rc;
}
