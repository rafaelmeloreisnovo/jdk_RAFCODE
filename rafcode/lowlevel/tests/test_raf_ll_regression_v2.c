/* Independent negative/edge fixtures; hosted executable, core remains freestanding.
 * Public-format constants only; no OpenJDK or ART source copied.
 * SPDX-License-Identifier: NOASSERTION (authorial license decision pending).
 */
#include "raf_ll_probe_v1.h"

static void put32le(unsigned char *p, unsigned int v) {
    p[0] = (unsigned char)v;
    p[1] = (unsigned char)(v >> 8);
    p[2] = (unsigned char)(v >> 16);
    p[3] = (unsigned char)(v >> 24);
}
#define CHECK(x) do { if (!(x)) return __LINE__; } while (0)

int main(void) {
    unsigned char dex[128] = {0};
    struct raf_ll_result_v1 result;
    unsigned int i;
    static const unsigned char supported[] = {'5','7','8','9','0'};
    dex[0]='d'; dex[1]='e'; dex[2]='x'; dex[3]=10u;
    dex[4]='0'; dex[5]='3'; dex[6]='6'; dex[7]=0u;
    put32le(dex+32,128u); put32le(dex+36,112u);
    put32le(dex+40,0x12345678u); put32le(dex+52,112u);

    /* Previously incorrectly accepted as plausible by interval [035,040]. */
    CHECK(raf_ll_probe_v1(dex,128ul,&result)==RAF_LL_UNSUPPORTED);
    CHECK(result.kind==RAF_LL_DEX && result.format_version==36u);
    CHECK(result.header_bytes==0u && result.declared_bytes==0u);

    for (i=0u;i<sizeof(supported);++i) {
        dex[5]=(supported[i]=='0'?'4':'3');
        dex[6]=supported[i];
        CHECK(raf_ll_probe_v1(dex,128ul,&result)==RAF_LL_HEADER_PLAUSIBLE);
        CHECK(result.format_version==(supported[i]=='0'?40u:
              30u+(unsigned int)(supported[i]-'0')));
        CHECK(result.declared_bytes==128u && result.header_bytes==112u);
        CHECK(raf_ll_probe_v1(dex,111ul,&result)==RAF_LL_TRUNCATED);
    }
    dex[5]='4'; dex[6]='1';
    CHECK(raf_ll_probe_v1(dex,128ul,&result)==RAF_LL_UNSUPPORTED);
    CHECK(result.format_version==41u);
    dex[5]='3'; dex[6]='9';
    put32le(dex+52,125u);
    CHECK(raf_ll_probe_v1(dex,128ul,&result)==RAF_LL_MALFORMED);
    put32le(dex+52,112u);
    CHECK(raf_ll_probe_v1(dex,127ul,&result)==RAF_LL_MALFORMED);

    /* Unknown input reinitializes output; invalid pointer never dereferenced. */
    dex[0]=0u;
    CHECK(raf_ll_probe_v1(dex,128ul,&result)==RAF_LL_UNKNOWN_FORMAT);
    CHECK(result.kind==RAF_LL_UNKNOWN && result.declared_bytes==0u);
    CHECK(raf_ll_probe_v1(0,128ul,&result)==RAF_LL_INVALID_INPUT);
    CHECK(raf_ll_probe_v1(dex,128ul,0)==RAF_LL_INVALID_INPUT);
    return 0;
}
