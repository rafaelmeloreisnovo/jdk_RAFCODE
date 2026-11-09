/* Independent RAFCODE layout guard: no OpenJDK/ART source imported.
 * SPDX-License-Identifier: LicenseRef-RAFCODE-AUTHORIAL-PENDING
 * Header/table bounds only. NOT a loader, verifier, signature check or runtime.
 */
#ifndef RAF_LL_LAYOUT_V1_H
#define RAF_LL_LAYOUT_V1_H
#include "raf_ll_probe_v1.h"

static inline unsigned long long raf_layout_num(const unsigned char *p,
                                                  unsigned int count,
                                                  unsigned int be) {
    unsigned long long v = 0ull;
    unsigned int i;
    if (be) {
        for (i = 0u; i < count; ++i) v = (v << 8) | (unsigned long long)p[i];
    } else {
        for (i = count; i != 0u; --i) v = (v << 8) | (unsigned long long)p[i-1u];
    }
    return v;
}

static inline unsigned int raf_layout_span(unsigned long long off,
                                            unsigned long long size,
                                            unsigned long n) {
    return off <= (unsigned long long)n &&
           size <= (unsigned long long)n - off;
}

/* Independent, conservative second-stage checker. Returns existing RAF_LL statuses.
 * Header-only parsing cannot establish ELF/DEX validity. Legacy DEX v035-040 only.
 * Reject extended ELF numbering for this intentionally bounded scope.
 */
static inline unsigned int raf_ll_layout_v1(const unsigned char *p,
                                              unsigned long n) {
    struct raf_ll_result_v1 r;
    unsigned int st = raf_ll_probe_v1(p, n, &r);
    if (st != RAF_LL_HEADER_PLAUSIBLE) return st;
    if (r.kind == RAF_LL_ELF) {
        unsigned int cls = p[4], be = (unsigned int)(p[5] == 2u);
        unsigned int offsz = cls == 1u ? 4u : 8u;
        unsigned int phsize = cls == 1u ? 32u : 56u;
        unsigned int shsize = cls == 1u ? 40u : 64u;
        unsigned int hdr = cls == 1u ? 40u : 52u;
        unsigned long long phoff = raf_layout_num(p + (cls == 1u ? 28u : 32u),offsz,be);
        unsigned long long shoff = raf_layout_num(p + (cls == 1u ? 32u : 40u),offsz,be);
        unsigned int phent = (unsigned int)raf_layout_num(p + hdr + 2u,2u,be);
        unsigned int phnum = (unsigned int)raf_layout_num(p + hdr + 4u,2u,be);
        unsigned int shent = (unsigned int)raf_layout_num(p + hdr + 6u,2u,be);
        unsigned int shnum = (unsigned int)raf_layout_num(p + hdr + 8u,2u,be);
        unsigned int shstr = (unsigned int)raf_layout_num(p + hdr + 10u,2u,be);
        unsigned int i;
        if (phnum == 65535u || shstr == 65535u || (shoff != 0ull && shnum == 0u))
            return RAF_LL_UNSUPPORTED; /* Extended ELF numbering not implemented. */
        if ((phnum == 0u) != (phoff == 0ull) ||
            (shnum == 0u) != (shoff == 0ull) ||
            (phnum && phent != phsize) ||
            (shnum && (shent != shsize || (shstr && shstr >= shnum))))
            return RAF_LL_MALFORMED;
        if ((phnum && (!raf_layout_span(phoff,(unsigned long long)phnum*phsize,n) || phoff < r.header_bytes)) ||
            (shnum && (!raf_layout_span(shoff,(unsigned long long)shnum*shsize,n) || shoff < r.header_bytes)))
            return RAF_LL_MALFORMED;
        /* File-backed program segments must stay inside supplied byte span. */
        for (i = 0u; i < phnum; ++i) {
            const unsigned char *ent = p + (unsigned long)(phoff + (unsigned long long)i*phsize);
            unsigned int type = (unsigned int)raf_layout_num(ent,4u,be);
            unsigned long long poff = raf_layout_num(ent+(cls==1u?4u:8u),offsz,be);
            unsigned long long filesz = raf_layout_num(ent+(cls==1u?16u:32u),offsz,be);
            unsigned long long memsz = raf_layout_num(ent+(cls==1u?20u:40u),offsz,be);
            if (!raf_layout_span(poff,filesz,n) || (type == 1u && memsz < filesz))
                return RAF_LL_MALFORMED;
        }
        return RAF_LL_HEADER_PLAUSIBLE;
    }
    if (r.kind == RAF_LL_DEX) {
        unsigned long long map = (unsigned long long)raf_ll_u32le(p+52u);
        unsigned long long data_size = (unsigned long long)raf_ll_u32le(p+104u);
        unsigned long long data_off = (unsigned long long)raf_ll_u32le(p+108u);
        unsigned long long count, j;
        unsigned long long prev = 0ull;
        unsigned int seen_header = 0u, seen_map = 0u;
        if ((map & 3ull) || (data_size & 3ull) || (data_off & 3ull) ||
            !raf_layout_span(data_off,data_size,n) ||
            map < data_off || map >= data_off + data_size ||
            !raf_layout_span(map,4ull,n)) return RAF_LL_MALFORMED;
        count = raf_ll_u32le(p+(unsigned long)map);
        if (count < 2ull || !raf_layout_span(map+4ull,count*12ull,n) ||
            map + 4ull + count*12ull > data_off + data_size)
            return RAF_LL_MALFORMED;
        for (j = 0ull; j < count; ++j) {
            const unsigned char *ent = p+(unsigned long)(map+4ull+j*12ull);
            unsigned int type = raf_ll_u16(ent,0u);
            unsigned int reserved = raf_ll_u16(ent+2u,0u);
            unsigned long long qty = raf_ll_u32le(ent+4u);
            unsigned long long off = raf_ll_u32le(ent+8u);
            if (reserved || qty == 0ull || off >= (unsigned long long)n ||
                (j != 0ull && off <= prev)) return RAF_LL_MALFORMED;
            prev = off;
            if (type == 0u) {
                if (seen_header || off != 0ull || qty != 1ull) return RAF_LL_MALFORMED;
                seen_header = 1u;
            }
            if (type == 0x1000u) {
                if (seen_map || off != map || qty != 1ull) return RAF_LL_MALFORMED;
                seen_map = 1u;
            }
        }
        return (seen_header && seen_map) ? RAF_LL_HEADER_PLAUSIBLE : RAF_LL_MALFORMED;
    }
    return RAF_LL_UNSUPPORTED;
}
#endif
