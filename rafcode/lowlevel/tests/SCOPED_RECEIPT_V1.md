# SCOPED RECEIPT V1 — JDK low-level ELF/DEX boundary

- Date reference: 2026-11-09. Producer: `rafaelmeloreisnovo/jdk_RAFCODE` upstream `master` at `b97deb0f22edb0bc4a823d9f4766d374106acf0c`; master Git tree search returned 56,745 objects (truncated), **NOT** a complete corpus inventory.
- New isolated branch: `rafaelia/lowlevel-elf-dex-boundary-v1-20261109`; no existing upstream files edited.
- Git blob SHA **readback matched exact local bytes**:
  - `raf_ll_probe_v1.h` `cf3e1728603cba7e524e6d327534ee1acf8858f3`
  - `tests/test_raf_ll_probe_v1.c` `75f683e35d65fcc74a33c4753e926982af67f03c`
  - `tests/probe_object_v1.c` `33a9947f04e213fb77c54181f16ee4cbc4b50185`
- Local GCC: `-std=c11 -O2 -Wall -Wextra -Werror -pedantic -ffreestanding -fno-builtin -fno-stack-protector -nostdinc` object PASS, `nm -u` empty: zero undefined symbols (for object only).
- Host tests: GCC `-fsanitize=address,undefined` compiled/runs PASS; **17 deterministic KAT checks** (null, unknown, ELF32 ARM, ELF64 big-endian AArch64, malformed/truncated, DEX 039 and DEX 041 reject, size/endian/map), plus 4,096 pseudo-random 128-byte buffers each at all 129 bounded lengths = **528,384 probe calls**; checks that statuses are in range and match output. Synthetic generated inputs, not full format certification.
- Clang compile-only freestanding target ARMv7A EABI5 ELF32 relocatable PASS and AArch64 ELF64 relocatable PASS. No linker executable or runtime on either target.
- No JNI, libc, malloc, syscall, external symbol or platform native API in core.
- Tests hosted on Linux use GCC/Clang and C standard library; scoped intentionally separate from core.
- **Missing:** full ELF/DEX validation, fuzz coverage proof, security certification, JDK build, runtime ART/JVM integration, APK, ARM hardware E2E, CI exact HEAD, license compatibility. `claim_allowed=false`; `IMPLEMENTED_TESTED_SCOPED != PASS_INTEGRATION`.
- Falsifier: validate real ELF/DEX samples; compare reported header class with a well-established independent verifier; any overread, misclassification or unsupported v041 accepted as valid fails gate; cross-check layout offsets against current AOSP DEX format.
