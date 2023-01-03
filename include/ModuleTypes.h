#ifndef MODULE_TYPES
#define MODULE_TYPES

/*Standard Includes*/
#include <string>

/*Custom Includes*/

enum class ModuleType
{
    ModuleBase,
    SessionProcModule,
    RouterModule,
    WinUDPRxModule,
    WinUDPTxModule,
    WAVWriterModule,
    WAVAccumulatorModule,
    TimeToWavModule,
    SimulatorModule
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
        case ModuleType::SimulatorModule: return "SimulatorModule";

        // Network Modules
        case ModuleType::WinUDPRxModule: return "WinUDPRxModule";
        case ModuleType::WinUDPTxModule: return "WinUDPTxModule";
        
        // WAV Modules
        case ModuleType::WAVWriterModule: return "WAVWriterModule";
        case ModuleType::WAVAccumulatorModule: return "WAVAccumulatorModule";
        case ModuleType::TimeToWavModule: return "TimeToWavModule";

        default: return "Unknown Module";
        }
    }
};

#endif