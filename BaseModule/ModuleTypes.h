#ifndef MODULETYPES
#define MODULETYPES

/*Standard Includes*/
#include <string>

/*Custom Includes*/


enum class ModuleType
{
    ModuleBase,
    SessionProcModule,
    RouterModule,
    WinUDPRxModule,
    WAVWriterModule,
    WAVAccumulatorModule
};

class ModuleTypeStrings
{
public:

    ModuleTypeStrings() {};
    ~ModuleTypeStrings() {};

    static std::string toString(ModuleType eModuleType)
    {
        switch (eModuleType)
        {
        // Core Modules
        case ModuleType::ModuleBase: return "ModuleBase";
        case ModuleType::SessionProcModule: return "SessionProcModule";
        case ModuleType::RouterModule: return "RouterModule";

        // Network Modules
        case ModuleType::WinUDPRxModule: return "WinUDPRxModule";
        
        // WAV Modules
        case ModuleType::WAVWriterModule: return "WAVWriterModule";
        case ModuleType::WAVAccumulatorModule: return "WAVAccumulatorModule";

        default: return "Unknown Module";
        }
    }
};

#endif