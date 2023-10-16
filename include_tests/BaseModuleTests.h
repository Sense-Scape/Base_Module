#ifndef BASE_MODULE_TEST
#define BASE_MODULE_TEST

#include "doctest.h"
#include "BaseModule.h"

TEST_CASE("BaseModule Test") 
{
    // First we set up logging so we can see what is going on
    plog::init(plog::info, "BaseModuleTestLogs.txt", 10000);

    unsigned uBufferLength = 10;
    BaseModule baseModule(uBufferLength);

    // This should avoid fail fast mode
    baseModule.SetTestMode(true);


    PLOG_INFO << __FUNCTION__ << "Starting Basic Type Check";
    SUBCASE("Basic Type Check")
    {
        CHECK(baseModule.GetModuleType() == ModuleType::ModuleBase);
    }
    

    PLOG_INFO << __FUNCTION__ << "Stating to check if one can try repeatedly start module and stop it";
    SUBCASE("Checking if one can try repeatedly start module and stop it")
    {
        // If this fails I will never get to the check
        baseModule.StartProcessing();
        baseModule.StartProcessing();
        baseModule.StopProcessing();
        CHECK(1 == 1);
    }
    
}

#endif