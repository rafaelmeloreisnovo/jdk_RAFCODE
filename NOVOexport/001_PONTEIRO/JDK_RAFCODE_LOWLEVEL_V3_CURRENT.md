# CURRENT — JDK_RAFCODE L0 ELF/DEX layout guard V3 (draft candidate)

DATE=2026-11-09
PRODUCER=rafaelmeloreisnovo/jdk_RAFCODE
BASE=master@f61e53b9ce5640864fad05895d22138cbd6f45d4
PARENT=NOVOexport/001_PONTEIRO/JDK_RAFCODE_LOWLEVEL_V2_CURRENT.md
SOURCE=rafcode/lowlevel/raf_ll_probe_v1.h (unchanged, prior merged #1)
IMPLEMENTATION=rafcode/lowlevel/raf_ll_layout_v1.h
TEST=rafcode/lowlevel/tests/test_raf_ll_layout_v1.c
CI=.github/workflows/raf-lowlevel-scoped.yml (extended with layout test and ARM targets)
AUTHORITY=upstream OpenJDK LICENSE + ADDITIONAL_LICENSE_INFO + ASSEMBLY_EXCEPTION remain untouched; new authorial island license decision TOKEN_VAZIO, see rafcode/lowlevel/LICENSE_BOUNDARY_V1.md
BOUNDARY=ELF32/ELF64 table bounds and program-segment extents; DEX v035/037-040 map_list bounds, alignment and presence of header/map items. Extended ELF numbering, DEX 036/041+, all bytecode references, signatures, checksum, ART/JNI/JVM runtime unsupported/not verified.
EVIDENCE_LOCAL=host GCC C11 UBSan test PASS; clang armv7a-none-eabi and aarch64-none-elf objects PASS; nm -u empty. Only local synthetic fixtures; no full JDK build or physical hardware.
GIT_BLOB_HEADER=81becfa65cc000a5aee368eed5407f1ef39da81b
GIT_BLOB_TEST=7642f123cdc23320f9be50164e3f71f49aa5bee6
CI_EXACT_HEAD=TOKEN_VAZIO until terminal job readback
STATE=DRAFT/HOLD; release_allowed=false; claim_allowed=false; no OpenJDK/ART changes; no merge; no upstream license replacement.
ROLLBACK=close draft PR, preserve prior master+receipts; supersede this marker rather than delete.
R3=<F_ok: isolated bounded layout guard and local tests; F_gap: deep structure, real ABI/runtime, license approval, CI; F_next: exact-head CI and independent adversarial corpus>
