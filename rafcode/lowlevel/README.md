# RAFCODEPHI low-level island V1 — freestanding ELF/DEX header probe

**Source authority:** this repository owns only this scoped addition. OpenJDK upstream code and build files remain unchanged. **claim_allowed=false; release_allowed=false; merge_allowed=false** until gates and licensing are approved.

## Architecture / runtime boundaries

| Layer | May do | May NOT claim |
|---|---|---|
| `raf_ll_probe_v1.h` (new, authorial island) | Header *plausibility* for ELF32/ELF64 and legacy DEX 035..040; bounded byte reads; no include, libc, heap, syscall, JNI, Java, GC | Loading/verifying an ELF binary, parsing DEX methods, executing bytecode, ART/JVM replacement, signature integrity |
| Host tests | `stdio`, `string.h`, GCC/Clang and sanitizers, **host-only** | Freestanding runtime deployment, physical E2E |
| OpenJDK `src/`, `make/` | Original upstream JDK build and HotSpot | No dependencies or automatic Android ART/DEX compatibility |
| Android ART / loader / kernel | Platform-controlled execution of DEX, ELF and apps; documented external boundary | Autor-controlled bare metal from a normal Android app |

API: `raf_ll_probe_v1(const unsigned char *bytes, unsigned long available_bytes, struct raf_ll_result_v1 *out)`.
It uses static inline helpers: the source-to-source interface remains a function; compiler may inline but exact emitted instructions are compiler/target dependent. Bounds depend on caller passing the **real accessible byte length**. The function does not dereference beyond the declared bound.

The status `RAF_LL_HEADER_PLAUSIBLE` **does not prove validity or safety**. For DEX, the file-size/endian/map-offset/header-size are checked; Adler32, SHA-1, map tables and type references are NOT verified. DEX v041+ deliberately returns UNSUPPORTED because its container model differs, not because newer DEX is inherently invalid. ELF section/program tables, segments, relocation, symbol resolution, signatures and ABI compatibility are NOT validated. Never execute untrusted inputs based on this classifier.

## Reproduce scoped tests on an authorized hosted build machine

```sh
cd rafcode/lowlevel
gcc -std=c11 -O2 -Wall -Wextra -Werror -pedantic -ffreestanding \
  -fno-builtin -fno-stack-protector -nostdinc -I. \
  -c tests/probe_object_v1.c -o /tmp/raf_probe_v1.o
nm -u /tmp/raf_probe_v1.o                 # expected: zero undefined symbols
gcc -std=c11 -O1 -g -Wall -Wextra -Werror -pedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -I. \
  tests/test_raf_ll_probe_v1.c -o /tmp/raf_probe_test
/tmp/raf_probe_test
clang --target=armv7a-none-eabi -std=c11 -O2 -Wall -Wextra -Werror \
  -ffreestanding -fno-builtin -nostdinc -I. -c tests/probe_object_v1.c -o /tmp/raf_probe_armv7.o
clang --target=aarch64-none-elf -std=c11 -O2 -Wall -Wextra -Werror \
  -ffreestanding -fno-builtin -nostdinc -I. -c tests/probe_object_v1.c -o /tmp/raf_probe_arm64.o
```

**Observed in this session:** host GCC freestanding object PASS, `nm -u` empty, hosted ASan+UBSan test PASS, ARMv7 and AArch64 compile-to-object PASS. Header/test/object Git blobs matched exact tested bytes, see `tests/SCOPED_RECEIPT_V1.md`. No JDK build, Android APK, physical ARM execution or device test performed.

No assembly/hexadecimal hotpath was inserted without a comparative benchmark. `-ffreestanding` does not eliminate the required compiler, linker, loader, boot environment or OS; `-nostdinc` excludes standard include directories for this object but is not a license or complete binary independence guarantee. C comments are not machine instructions. Compiler/preprocessor/linker flags and source-token order can change generated code; formatting alone has no guaranteed efficiency effect.

References:
- Android DEX spec: https://source.android.com/docs/core/runtime/dex-format
- ART architecture: https://source.android.com/docs/core/runtime
- Original repository: `LICENSE`, `ADDITIONAL_LICENSE_INFO`, `ASSEMBLY_EXCEPTION`

**R3:** F_ok scoped C header and checks; F_gap full JVM/ART, ABI behavior, license approval, CI and hardware NOT_RUN; F_next instrument actual profiles and add fuzz corpora without changing OpenJDK.
