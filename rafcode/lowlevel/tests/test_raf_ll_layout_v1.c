/* Hosted adversarial table/layout fixtures; tests are NOT JDK/ART execution. */
#include "raf_ll_layout_v1.h"
#define CHECK(x) do { if (!(x)) return __LINE__; } while (0)
static void put16(unsigned char *p, unsigned int v) {
    p[0]=(unsigned char)v; p[1]=(unsigned char)(v>>8);
}
static void put32(unsigned char *p, unsigned int v) {
    p[0]=(unsigned char)v; p[1]=(unsigned char)(v>>8);
    p[2]=(unsigned char)(v>>16); p[3]=(unsigned char)(v>>24);
}
static void put64(unsigned char *p, unsigned long long v) {
    unsigned int i; for (i=0u;i<8u;++i) p[i]=(unsigned char)(v>>(8u*i));
}
static void fill(unsigned char *p, unsigned int n) {
    unsigned int i; for (i=0u;i<n;++i)p[i]=0u;
}
int main(void) {
    unsigned char p[256];
    unsigned long i;
    fill(p,256u);
    p[0]=0x7fu; p[1]='E'; p[2]='L'; p[3]='F';
    p[4]=1u;p[5]=1u;p[6]=1u;
    put16(p+18,40u);put32(p+20,1u);put16(p+40,52u);
    put32(p+28,52u);put16(p+42,32u);put16(p+44,1u);
    put32(p+52,1u);put32(p+56,80u);put32(p+68,8u);put32(p+72,8u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_HEADER_PLAUSIBLE);
    CHECK(raf_ll_layout_v1(p,83ul)==RAF_LL_MALFORMED);
    put32(p+68,0xffffffffu);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put32(p+68,8u);put32(p+72,7u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put32(p+72,8u);put16(p+44,65535u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_UNSUPPORTED);
    put16(p+44,1u);put32(p+28,0xfffffff0u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    fill(p,256u);
    p[0]='d';p[1]='e';p[2]='x';p[3]=10u;
    p[4]='0';p[5]='3';p[6]='9';
    put32(p+32,256u);put32(p+36,112u);
    put32(p+40,0x12345678u);put32(p+52,128u);
    put32(p+104,128u);put32(p+108,128u);
    put32(p+128,2u);
    put16(p+132,0u);put32(p+136,1u);put32(p+140,0u);
    put16(p+144,0x1000u);put32(p+148,1u);put32(p+152,128u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_HEADER_PLAUSIBLE);
    CHECK(raf_ll_layout_v1(p,255ul)==RAF_LL_MALFORMED);
    put32(p+128,0xffffffffu);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put32(p+128,2u);put32(p+52,129u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put32(p+52,128u);put32(p+152,0u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put32(p+152,128u);put16(p+144,0u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put16(p+144,0x1000u);put16(p+146,1u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put16(p+146,0u);put32(p+104,0u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    for(i=0ul;i<112ul;++i) CHECK(raf_ll_layout_v1(p,i)!=RAF_LL_HEADER_PLAUSIBLE);
    CHECK(raf_ll_layout_v1((const unsigned char*)0,0ul)==RAF_LL_INVALID_INPUT);
    /* ELF64 AArch64: keep a valid PT_LOAD, then falsify each span invariant. */
    fill(p,256u);
    p[0]=0x7fu;p[1]='E';p[2]='L';p[3]='F';p[4]=2u;p[5]=1u;p[6]=1u;
    put16(p+18,183u);put32(p+20,1u);put16(p+52,64u);
    put64(p+32,64ull);put16(p+54,56u);put16(p+56,1u);
    put32(p+64,1u);put64(p+72,128ull);
    put64(p+96,16ull);put64(p+104,32ull);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_HEADER_PLAUSIBLE);
    put64(p+96,256ull);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put64(p+96,16ull);put64(p+104,15ull);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put64(p+104,32ull);put16(p+56,0u);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_MALFORMED);
    put64(p+32,0ull);
    CHECK(raf_ll_layout_v1(p,256ul)==RAF_LL_HEADER_PLAUSIBLE);
    return 0;
}
