#include "HookAPI.h"

#include "MCPatches.h"
#include "Options.h"

#undef FindSignature
#define FindSignature(signature)                                               \
  ((uint8_t *)FindSig("Minecraft.Windows.exe", signature))

void initMCPatches() {
  if (Options::vanilla2DeferredEnabled &&
      Options::disableRendererContextD3D12RTX) {
    // Deferred rendering no longer requires RendererContextD3D12RTX
    // since 1.19.80, so it can be disabled for better performance
    // bgfx::d3d12rtx::RendererContextD3D12RTX::init
    if (auto ptr = FindSignature("83 BF DC 02 00 00 65 ? ? ? ? ? ? ? ? ? ? ? ? "
                                 "? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? "
                                 "? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 02 00 00 65");
        ptr) {
      // 1.20.30.21 preview
      ScopedVP(ptr, 59, PAGE_READWRITE);
      ptr[6] = 0x7F;
      ptr[58] = 0x7F;
    } else {
      printf("Failed to patch bgfx::d3d12rtx::RendererContextD3D12RTX::init\n");
    }
  }

  // Bypass VendorID check to support some Intel GPUs
  // bgfx::d3d12::RendererContextD3D12::init
  if (auto ptr = FindSignature("81 BF ?? ?? 00 00 86 80 00 00"); ptr) {
    // 1.19.40
    ScopedVP(ptr, 10, PAGE_READWRITE);
    ptr[6] = 0;
    ptr[7] = 0;
  } else if (ptr = FindSignature("81 BE ?? ?? 00 00 86 80 00 00"); ptr) {
    // 1.20.0.23 preview
    ScopedVP(ptr, 10, PAGE_READWRITE);
    ptr[6] = 0;
    ptr[7] = 0;
  } else {
    printf("Failed to patch bgfx::d3d12::RendererContextD3D12::init\n");
  }

  // Fix rendering issues on some NVIDIA GPUs
  // dragon::bgfximpl::toSamplerFlags
  if (auto ptr = FindSignature("FF E1 80 7B ? ? B8 00 00 07 10"); ptr) {
    // 1.21.50
    ScopedVP(ptr, 10, PAGE_READWRITE);
    ptr[9] = 0;
  } else {
    printf("Failed to patch dragon::bgfximpl::toSamplerFlags\n");
  }

  // MinecraftGame::_updateLightingModel
  if (auto ptr = FindSignature(
          "41 83 FE 01 75 ? 48 8B 01 48 8B 40 ? FF 15 ? ? ? ? 32 C0 EB");
      ptr) {
    // 1.21.50
    ScopedVP(ptr, 22, PAGE_READWRITE);
    ptr[19] = 0xB0;
    ptr[20] = 0x01;
  } else {
    printf("Failed to patch MinecraftGame::_updateLightingModel\n");
  }

  // MinecraftGame::startFrame
  if (auto ptr = FindSignature("83 FF 01 75 ? 48 8B 42 ? 48 8D 4A 08 48 8B 40 "
                               "40 FF 15 C0 68 FA 05 32 C0");
      ptr) {
    // 1.21.50
    ScopedVP(ptr, 25, PAGE_READWRITE);
    ptr[23] = 0xB0;
    ptr[24] = 0x01;
  } else {
    printf("Failed to patch MinecraftGame::startFrame\n");
  }
}
