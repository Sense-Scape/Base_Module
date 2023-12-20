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
    SimulatorModule,
    HPFModule,
    WatchdogModule,
    WinTCPRxModule,
    WinTCPTxModule,
    ChunkToBytesModule,
    ToJSONModule,
    FFTModule,
    WinMultiClientTCPRxModule,
    WinMultiClientTCPTxModule,
    LinuxMultiClientTCPTxModule
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
        // Generic Modules
        case ModuleType::ModuleBase: return "ModuleBase";
        case ModuleType::SessionProcModule: return "SessionProcModule";
        case ModuleType::RouterModule: return "RouterModule";
        case ModuleType::SimulatorModule: return "SimulatorModule";
        case ModuleType::ChunkToBytesModule: return "ChunkToBytesModule";

        // Network Modules
        case ModuleType::WinUDPRxModule: return "WinUDPRxModule";
        case ModuleType::WinUDPTxModule: return "WinUDPTxModule";
        
        // WAV Modules
        case ModuleType::WAVWriterModule: return "WAVWriterModule";
        case ModuleType::WAVAccumulatorModule: return "WAVAccumulatorModule";
        case ModuleType::TimeToWavModule: return "TimeToWavModule";

        // Signal Processing Modulesa
        case ModuleType::HPFModule: return "HighPassFilterModule";

        // Status Reporting Modules
        case ModuleType::WatchdogModule: return "WatchdogModule";

        case ModuleType::LinuxMultiClientTCPTxModule: return "LinuxMultiClientTCPTxModule";

        default: return "Unknown Module";
        }
    }
};

#endif