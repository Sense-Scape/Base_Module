#ifndef BASE_MODULE_TEST
#define BASE_MODULE_TEST

#include "doctest.h"
#include "BaseModule.h"

TEST_CASE("BaseModule Test") 
{
    unsigned uBufferLength = 10;
    BaseModule baseModule(uBufferLength);

    CHECK(baseModule.GetModuleType() == ModuleType::ModuleBase);
}

#endif