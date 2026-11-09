/* Hosted test ONLY. The embedded probe header has zero library imports. */
#include <stdio.h>
#include <string.h>
#include "raf_ll_probe_v1.h"

static unsigned int failures;
static void ck(const char *label, unsigned int cond) {
    if (!cond) { (void)fprintf(stderr, "FAIL:%s\n", label); failures++; }
}
static void put16(unsigned char *p, unsigned int v, unsigned int be) {
    p[0] = (unsigned char)(be ? (v >> 8) : v);
    p[1] = (unsigned char)(be ? v : (v >> 8));
}
static void put32(unsigned char *p, unsigned int v) {
    p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8);
    p[2] = (unsigned char)(v >> 16); p[3] = (unsigned char)(v >> 24);
}
int main(void) {
    unsigned char p[128];
    struct raf_ll_result_v1 r;
    unsigned long i, j;
    unsigned int seed = 0x52eaf1u, status;
    (void)memset(p, 0, sizeof(p));
    ck("null output", raf_ll_probe_v1(p, sizeof(p), 0) == RAF_LL_INVALID_INPUT);
    ck("null input", raf_ll_probe_v1(0, 0ul, &r) == RAF_LL_INVALID_INPUT);
    ck("unknown", raf_ll_probe_v1(p, 0ul, &r) == RAF_LL_UNKNOWN_FORMAT);

    p[0] = 0x7fu; p[1] = 'E'; p[2] = 'L'; p[3] = 'F';
    ck("elf truncated", raf_ll_probe_v1(p, 4ul, &r) == RAF_LL_TRUNCATED);
    p[4] = 1u; p[5] = 1u; p[6] = 1u;
    put16(p + 18, 40u, 0u); put32(p + 20, 1u); put16(p + 40, 52u, 0u);
    ck("elf32 arm", raf_ll_probe_v1(p, 52ul, &r) == RAF_LL_HEADER_PLAUSIBLE &&
                     r.kind == RAF_LL_ELF && r.machine == 40u && r.header_bytes == 52u);
    ck("elf32 truncated", raf_ll_probe_v1(p, 51ul, &r) == RAF_LL_TRUNCATED);
    p[40] = 0;
    ck("elf32 bad hdr", raf_ll_probe_v1(p, 52ul, &r) == RAF_LL_MALFORMED);
    p[40] = 52u; p[4] = 0u;
    ck("elf bad class", raf_ll_probe_v1(p, 52ul, &r) == RAF_LL_UNSUPPORTED);

    (void)memset(p, 0, sizeof(p));
    p[0] = 0x7fu; p[1] = 'E'; p[2] = 'L'; p[3] = 'F';
    p[4] = 2u; p[5] = 2u; p[6] = 1u;
    put16(p + 18, 183u, 1u); p[23] = 1u; put16(p + 52, 64u, 1u);
    ck("elf64 big", raf_ll_probe_v1(p, 64ul, &r) == RAF_LL_HEADER_PLAUSIBLE &&
                      r.machine == 183u && r.format_version == 1u);
    ck("elf64 truncated", raf_ll_probe_v1(p, 63ul, &r) == RAF_LL_TRUNCATED);

    (void)memset(p, 0, sizeof(p));
    p[0] = 'd'; p[1] = 'e'; p[2] = 'x'; p[3] = '\n';
    p[4] = '0'; p[5] = '3'; p[6] = '9'; p[7] = 0u;
    put32(p + 32, 128u); put32(p + 36, 112u);
    put32(p + 40, 0x12345678u); put32(p + 52, 112u);
    ck("dex header", raf_ll_probe_v1(p, sizeof(p), &r) == RAF_LL_HEADER_PLAUSIBLE &&
                      r.kind == RAF_LL_DEX && r.format_version == 39u);
    ck("dex truncated", raf_ll_probe_v1(p, 111ul, &r) == RAF_LL_TRUNCATED);
    put32(p + 32, 127u);
    ck("dex size mismatch", raf_ll_probe_v1(p, sizeof(p), &r) == RAF_LL_MALFORMED);
    put32(p + 32, 128u); put32(p + 40, 0u);
    ck("dex bad endianness", raf_ll_probe_v1(p, sizeof(p), &r) == RAF_LL_MALFORMED);
    put32(p + 40, 0x12345678u); p[5] = '4'; p[6] = '1';
    ck("dex v041 fail closed", raf_ll_probe_v1(p, sizeof(p), &r) == RAF_LL_UNSUPPORTED);
    p[5] = '3'; p[6] = '9'; p[7] = 1u;
    ck("dex malformed magic", raf_ll_probe_v1(p, sizeof(p), &r) == RAF_LL_MALFORMED);
    p[7] = 0u; put32(p + 52, 127u);
    ck("dex map out of bounds", raf_ll_probe_v1(p, sizeof(p), &r) == RAF_LL_MALFORMED);

    /* Deterministic adversarial mutations across every bounded prefix. */
    for (i = 0ul; i < 4096ul; ++i) {
        for (j = 0ul; j < sizeof(p); ++j) {
            seed = 1664525u * seed + 1013904223u;
            p[j] = (unsigned char)(seed >> 24);
        }
        for (j = 0ul; j <= sizeof(p); ++j) {
            status = raf_ll_probe_v1(p, j, &r);
            ck("fuzz status in range", status <= RAF_LL_INVALID_INPUT);
            ck("fuzz result consistent", status == r.status);
        }
    }
    if (failures) return 1;
    (void)puts("RAF_LL_PROBE_V1 PASS_SCOPED host KAT and bounded mutations");
    return 0;
}
