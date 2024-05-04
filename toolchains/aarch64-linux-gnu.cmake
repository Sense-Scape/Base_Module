
message("Using the aarch64-linux-gnu compiler")
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Path to Raspberry Pi sysroot directory
set(CMAKE_SYSROOT /usr/include)

# Path to cross-compiler binaries
set(tools /usr/bin/)
set(TOOLCHAIN_PREFIX aarch64-linux-gnu)
set(CMAKE_C_COMPILER ${tools}/${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${tools}/${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-as)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}-ld)
# Additional flags for cross-compilation if needed
# set(CMAKE_C_FLAGS "-march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4")
# set(CMAKE_CXX_FLAGS "-march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4")
