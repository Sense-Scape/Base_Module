# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.20

# compile CXX with D:/SC_SDKs/esp32/Espressif/tools/xtensa-esp32-elf/esp-2021r2-patch2-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-g++.exe
CXX_DEFINES = 

CXX_INCLUDES = -IC:\Users\Grant\Documents\SoundScape\SC_Pipelines\PL_ESP32_Acoustic_Sensor\PL_ESP32_Acoustic_Sensor\build\config -IC:\Users\Grant\Documents\SoundScape\SC_Libs\Lib_Base_Module\BaseModule\SC_Chunk_Types -IC:\Users\Grant\Documents\SoundScape\SC_Libs\Lib_Base_Module\BaseModule\INTERFACE -IC:\Users\Grant\Documents\SoundScape\SC_Libs\Lib_Chunk_Types\Chunk_Types -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\newlib\platform_include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\freertos\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\freertos\include\esp_additions\freertos -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\freertos\port\xtensa\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\freertos\include\esp_additions -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_hw_support\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_hw_support\include\soc -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_hw_support\include\soc\esp32 -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_hw_support\port\esp32\. -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\heap\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\log\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\lwip\include\apps -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\lwip\include\apps\sntp -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\lwip\lwip\src\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\lwip\port\esp32\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\lwip\port\esp32\include\arch -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\soc\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\soc\esp32\. -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\soc\esp32\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\hal\esp32\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\hal\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\hal\platform_port\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_rom\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_rom\include\esp32 -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_rom\esp32 -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_common\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_system\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_system\port\soc -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_system\port\public_compat -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp32\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\xtensa\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\xtensa\esp32\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\driver\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\driver\esp32\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_pm\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_ringbuf\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\efuse\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\efuse\esp32\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\vfs\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_wifi\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_event\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_netif\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_eth\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\tcpip_adapter\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_phy\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_phy\esp32\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_ipc\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\app_trace\include -ID:\SC_SDKs\esp32\esp-idf-v4.4\esp-idf-v4.4\components\esp_timer\include

CXX_FLAGS = -mlongcalls -Wno-frame-address -g -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -ggdb -Og -fmacro-prefix-map=C:/Users/Grant/Documents/SoundScape/SC_Pipelines/PL_ESP32_Acoustic_Sensor/PL_ESP32_Acoustic_Sensor=. -fmacro-prefix-map=D:/SC_SDKs/esp32/esp-idf-v4.4/esp-idf-v4.4=IDF -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -fno-jump-tables -fno-tree-switch-conversion -std=gnu++11 -fno-exceptions -fno-rtti -D_GNU_SOURCE -DIDF_VER=\"v4.4-dirty\" -DESP_PLATFORM -D_POSIX_READER_WRITER_LOCKS

