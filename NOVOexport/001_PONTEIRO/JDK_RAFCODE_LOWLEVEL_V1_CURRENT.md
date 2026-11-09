# START HERE — JDK_RAFCODE_LOWLEVEL_V1

ROUTE=READ CURRENT_STATE → PRODUCER `rafaelmeloreisnovo/jdk_RAFCODE` → SOURCE `master@b97deb0f22edb0bc4a823d9f4766d374106acf0c` → INDEPENDENT_ISLAND `rafcode/lowlevel/` → TEST `tests/SCOPED_RECEIPT_V1.md` → LICENSING `LICENSE_BOUNDARY_V1.md` → PR HOLD.

- Core: `rafcode/lowlevel/raf_ll_probe_v1.h` freestanding read-only ELF/DEX header probe.
- Tests: `tests/test_raf_ll_probe_v1.c`; freestanding object sentinel `tests/probe_object_v1.c`.
- Boundaries: no copied upstream code, no automatic license grant, upstream GPLv2/applicable exceptions preserved, no ART replacement.
- Host GCC+ASan+UBSan and Clang armv7/aarch64 object PASS_SCOPED; full JDK/Android/hardware NOT_RUN.
- Source tree recursive fetch was truncated; no complete source census claimed.
- Branch independent, draft-only; no default-branch edits, no automerge.
- Rollback close draft; corrections as successor, keep prior evidence.
