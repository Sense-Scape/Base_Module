#ifndef BASE_MODULE_TEST
#define BASE_MODULE_TEST

#include "doctest.h"
#include "BaseModule.h"

TEST_CASE("BaseModule Test") 
{
    unsigned uBufferLength = 10;
    BaseModule baseModule(uBufferLength);

    SUBCASE("Basic Type Check")
    {
        CHECK(baseModule.GetModuleType() == ModuleType::ModuleBase);
    }
    
    SUBCASE("Checking if I can try repeatedly start module and stop it")
    {
        // If this fails I will never get to the check
        baseModule.StartProcessing();
        baseModule.StartProcessing();
        baseModule.StopProcessing();
        CHECK(1 == 1);
    }
    
}

#endif