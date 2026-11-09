# CURRENT — RAFCODE JDK ELF/DEX freestanding V2 (candidate)

- INTENT: low-level autoral without regression, OpenJDK/ART remains original upstream.
- SOURCE: rafaelmeloreisnovo/jdk_RAFCODE master@b97deb0f22edb0bc4a823d9f4766d374106acf0c; PR #1 DRAFT branch rafaelia/lowlevel-elf-dex-boundary-v1-20261109.
- AUTHORITY: OpenJDK LICENSE, ADDITIONAL_LICENSE_INFO, ASSEMBLY_EXCEPTION, upstream src/ and make/ unchanged. Own rafcode/lowlevel candidate new code. Own redistribution license TOKEN_VAZIO until author decides + compatibility review; separation of directories is not legal dual licensing.
- PREDECESSOR: NOVOexport/001_PONTEIRO/JDK_RAFCODE_LOWLEVEL_V1_CURRENT.md and rafcode/lowlevel/tests/SCOPED_RECEIPT_V1.md remain unchanged.
- V2 HOTFIX: rafcode/lowlevel/raf_ll_probe_v1.h rejects undocumented DEX036 as UNSUPPORTED. Previously numeric interval 035..040 gave false positive. No ELF path or public signature change.
- FALSIFIER: rafcode/lowlevel/tests/test_raf_ll_regression_v2.c tests 036 reject, 035/037/038/039/040 synthetic legacy header plausibility, 041 reject, bad map/length and status reset. No complete DEX verification.
- CI: .github/workflows/raf-lowlevel-scoped.yml; path filtered and pinned checkout; GCC ASan+UBSan tests; GCC -ffreestanding -fno-builtin -nostdinc object nm -u empty; Clang ARMv7 and AArch64 object compilation. No artifact upload; host runner/toolchain is an external build dependency, but the produced standalone object has no linked libc.
- GATES: full OpenJDK/HotSpot/classfile verification, ART/DEX method loading, JNI, Android APK, ARM hardware execution, stack budget, asm benchmark are NOT_RUN. No claim of zero GC for Java or no native dependency of ART; C void is not absence of functions.
- NO ASM: no measured hotpath/ABI/clobber/stack comparison, so no inline assembly inserted.
- ACCEPTANCE: only scoped tests on exact commit and readback; claim_allowed=false; merge_allowed=false; release_allowed=false until license and producer gates.
- FURTHER: source exact blob SHA + run/job id + receipt; root and existing JDK build unmodified. ROLLBACK close DRAFT PR; supersession append-only without deletion.
- R3=<F_ok: DEX036 negative hotfix and gated test; F_gap: real device/JDK and license; F_next: independent fixtures and ABI/stack tests>.
