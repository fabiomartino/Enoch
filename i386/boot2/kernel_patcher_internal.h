/*
 * Copyright (c) 2009-2012 Evan Lojewski. All rights reserved.
 * Copyright (c) 2013-2015 xZenue LLC. All rights reserved.
 *
 *
 * This work is licensed under the
 *  Creative Commons Attribution-NonCommercial 3.0 Unported License.
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by-nc/3.0/.
 */

#ifndef __BOOT2_KERNEL_PATCHER_H
#define __BOOT2_KERNEL_PATCHER_H

#include <libkern/OSTypes.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/reloc.h>

#include "modules.h" 
#include "cpu.h"
#include "binaryPatcher.h"

#define CPU_MODEL_ANY       0x00
#define CPU_MODEL_UNKNOWN   0x01

#define KERNEL_ANY  0x00
#define KERNEL_64   0x01
#define KERNEL_32   0x02
#define KERNEL_ERR  0xFF

#define CPUIDFAMILY_DEFAULT 6


// kernel patches
void patch_kernel_internal(void *kernelData, u_int32_t uncompressed_size);

// Power management patch for kernel 13.0
static UInt8 KernelPatchPmSrc[] = {
    0x55, 0x48, 0x89, 0xe5, 0x41, 0x89, 0xd0, 0x85,
    0xf6, 0x74, 0x6c, 0x48, 0x83, 0xc7, 0x28, 0x90,
    0x8b, 0x05, 0x5e, 0x30, 0x5e, 0x00, 0x85, 0x47,
    0xdc, 0x74, 0x54, 0x8b, 0x4f, 0xd8, 0x45, 0x85,
    0xc0, 0x74, 0x08, 0x44, 0x39, 0xc1, 0x44, 0x89,
    0xc1, 0x75, 0x44, 0x0f, 0x32, 0x89, 0xc0, 0x48,
    0xc1, 0xe2, 0x20, 0x48, 0x09, 0xc2, 0x48, 0x89,
    0x57, 0xf8, 0x48, 0x8b, 0x47, 0xe8, 0x48, 0x85,
    0xc0, 0x74, 0x06, 0x48, 0xf7, 0xd0, 0x48, 0x21,
    0xc2, 0x48, 0x0b, 0x57, 0xf0, 0x49, 0x89, 0xd1,
    0x49, 0xc1, 0xe9, 0x20, 0x89, 0xd0, 0x8b, 0x4f,
    0xd8, 0x4c, 0x89, 0xca, 0x0f, 0x30, 0x8b, 0x4f,
    0xd8, 0x0f, 0x32, 0x89, 0xc0, 0x48, 0xc1, 0xe2,
    0x20, 0x48, 0x09, 0xc2, 0x48, 0x89, 0x17, 0x48,
    0x83, 0xc7, 0x30, 0xff, 0xce, 0x75, 0x99, 0x5d,
    0xc3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};
static UInt8 KernelPatchPmRepl[] = {
    0x55, 0x48, 0x89, 0xe5, 0x41, 0x89, 0xd0, 0x85,
    0xf6, 0x74, 0x73, 0x48, 0x83, 0xc7, 0x28, 0x90,
    0x8b, 0x05, 0x5e, 0x30, 0x5e, 0x00, 0x85, 0x47,
    0xdc, 0x74, 0x5b, 0x8b, 0x4f, 0xd8, 0x45, 0x85,
    0xc0, 0x74, 0x08, 0x44, 0x39, 0xc1, 0x44, 0x89,
    0xc1, 0x75, 0x4b, 0x0f, 0x32, 0x89, 0xc0, 0x48,
    0xc1, 0xe2, 0x20, 0x48, 0x09, 0xc2, 0x48, 0x89,
    0x57, 0xf8, 0x48, 0x8b, 0x47, 0xe8, 0x48, 0x85,
    0xc0, 0x74, 0x06, 0x48, 0xf7, 0xd0, 0x48, 0x21,
    0xc2, 0x48, 0x0b, 0x57, 0xf0, 0x49, 0x89, 0xd1,
    0x49, 0xc1, 0xe9, 0x20, 0x89, 0xd0, 0x8b, 0x4f,
    0xd8, 0x4c, 0x89, 0xca, 0x66, 0x81, 0xf9, 0xe2,
    0x00, 0x74, 0x02, 0x0f, 0x30, 0x8b, 0x4f, 0xd8,
    0x0f, 0x32, 0x89, 0xc0, 0x48, 0xc1, 0xe2, 0x20,
    0x48, 0x09, 0xc2, 0x48, 0x89, 0x17, 0x48, 0x83,
    0xc7, 0x30, 0xff, 0xce, 0x75, 0x92, 0x5d, 0xc3
};
// Power management patch for kernel 12.5
static UInt8 KernelPatchPmSrc2[] = {
    0x55, 0x48, 0x89, 0xe5, 0x41, 0x89, 0xd0, 0x85,
    0xf6, 0x74, 0x69, 0x48, 0x83, 0xc7, 0x28, 0x90,
    0x8b, 0x05, 0xfe, 0xce, 0x5f, 0x00, 0x85, 0x47,
    0xdc, 0x74, 0x51, 0x8b, 0x4f, 0xd8, 0x45, 0x85,
    0xc0, 0x74, 0x05, 0x44, 0x39, 0xc1, 0x75, 0x44,
    0x0f, 0x32, 0x89, 0xc0, 0x48, 0xc1, 0xe2, 0x20,
    0x48, 0x09, 0xc2, 0x48, 0x89, 0x57, 0xf8, 0x48,
    0x8b, 0x47, 0xe8, 0x48, 0x85, 0xc0, 0x74, 0x06,
    0x48, 0xf7, 0xd0, 0x48, 0x21, 0xc2, 0x48, 0x0b,
    0x57, 0xf0, 0x49, 0x89, 0xd1, 0x49, 0xc1, 0xe9,
    0x20, 0x89, 0xd0, 0x8b, 0x4f, 0xd8, 0x4c, 0x89,
    0xca, 0x0f, 0x30, 0x8b, 0x4f, 0xd8, 0x0f, 0x32,
    0x89, 0xc0, 0x48, 0xc1, 0xe2, 0x20, 0x48, 0x09,
    0xc2, 0x48, 0x89, 0x17, 0x48, 0x83, 0xc7, 0x30,
    0xff, 0xce, 0x75, 0x9c, 0x5d, 0xc3, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
};

static UInt8 KernelPatchPmRepl2[] = {
    0x55, 0x48, 0x89, 0xe5, 0x41, 0x89, 0xd0, 0x85,
    0xf6, 0x74, 0x70, 0x48, 0x83, 0xc7, 0x28, 0x90,
    0x8b, 0x05, 0xfe, 0xce, 0x5f, 0x00, 0x85, 0x47,
    0xdc, 0x74, 0x58, 0x8b, 0x4f, 0xd8, 0x45, 0x85,
    0xc0, 0x74, 0x05, 0x44, 0x39, 0xc1, 0x75, 0x4b,
    0x0f, 0x32, 0x89, 0xc0, 0x48, 0xc1, 0xe2, 0x20,
    0x48, 0x09, 0xc2, 0x48, 0x89, 0x57, 0xf8, 0x48,
    0x8b, 0x47, 0xe8, 0x48, 0x85, 0xc0, 0x74, 0x06,
    0x48, 0xf7, 0xd0, 0x48, 0x21, 0xc2, 0x48, 0x0b,
    0x57, 0xf0, 0x49, 0x89, 0xd1, 0x49, 0xc1, 0xe9,
    0x20, 0x89, 0xd0, 0x8b, 0x4f, 0xd8, 0x4c, 0x89,
    0xca, 0x66, 0x81, 0xf9, 0xe2, 0x00, 0x74, 0x02,
    0x0f, 0x30, 0x8b, 0x4f, 0xd8, 0x0f, 0x32, 0x89,
    0xc0, 0x48, 0xc1, 0xe2, 0x20, 0x48, 0x09, 0xc2,
    0x48, 0x89, 0x17, 0x48, 0x83, 0xc7, 0x30, 0xff,
    0xce, 0x75, 0x95, 0x5d, 0xc3, 0x90, 0x90, 0x90
};

#define KERNEL_PATCH_SIGNATURE     0x85d08941e5894855ULL

void patch_kernel_32(void *kernelData, u_int32_t uncompressed_size);
void patch_kernel_64(void *kernelData, u_int32_t uncompressed_size);

void patch_BooterExtensions_32(void *kernelData); 
void patch_BooterExtensions_64(void *kernelData);

bool patch_pm_init(void *kernelData);
bool patch_lapic_version_init_32(void *kernelData);
bool patch_lapic_version_init_64(void *kernelData);
bool patch_lapic_init_32(void *kernelData);
bool patch_lapic_init_64(void *kernelData);
bool patch_haswell_E_init(void *kernelData);
bool patch_haswell_ULT_init(void *kernelData);

void patch_SSE3_6(void *kernelData);
void patch_SSE3_5(void *kernelData);
void patch_SSE3_7(void *kernelData);

bool patch_string_XNU_init(void *kernelData);

// kexts pathes
unsigned int AppleRTC_Patch(void *data, UInt32 DriverSize, UInt32 StartLocation);
unsigned int AsusAICPUPMPatch(void *data, UInt32 DriverSize, UInt32 StartLocation);
unsigned int patch_NVDAStartupWeb(void *kernelData, UInt32 DriverSize, UInt32 StartLocation);
unsigned int trimEnablerSata(void *data, UInt32 DriverSize, UInt32 StartLocation);
unsigned int patch_AppleAHCIPort_OrangeFix(void *data, UInt32 DriverSize, UInt32 StartLocation);

void patch_prelinked_kexts(void *kernelData,
                           u_int32_t uncompressed_size,
                           unsigned prelinkTextVmaddr,
                           unsigned prelinkTextFileOff);

void patchBooterDefinedKext(const char *kext, void *driverAddr, UInt32 DriverSize, UInt32 StartLocation);

// os version
bool checkFullOSVer(const char *version);

#endif /* !__BOOT2_KERNEL_PATCHER_H */
