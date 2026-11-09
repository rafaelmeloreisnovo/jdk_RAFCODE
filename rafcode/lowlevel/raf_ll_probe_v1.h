/* RAFCODEPHI independent freestanding header-only probe V1.
 * Authorship boundary: new code only; license decision pending.
 * No OpenJDK code copied. Header plausibility, NOT file verification.
 * No headers, libc, heap, system calls, ART, JVM, JNI or Java runtime.
 */
#ifndef RAF_LL_PROBE_V1_H
#define RAF_LL_PROBE_V1_H

enum {
    RAF_LL_UNKNOWN = 0u,
    RAF_LL_ELF = 1u,
    RAF_LL_DEX = 2u
};
enum {
    RAF_LL_HEADER_PLAUSIBLE = 0u,
    RAF_LL_UNKNOWN_FORMAT = 1u,
    RAF_LL_TRUNCATED = 2u,
    RAF_LL_MALFORMED = 3u,
    RAF_LL_UNSUPPORTED = 4u,
    RAF_LL_INVALID_INPUT = 5u
};

struct raf_ll_result_v1 {
    unsigned int kind;
    unsigned int status;
    unsigned int machine;
    unsigned int format_version;
    unsigned int declared_bytes;
    unsigned int header_bytes;
};

static inline unsigned int raf_ll_u16(const unsigned char *p, unsigned int be) {
    if (be) return ((unsigned int)p[0] << 8) | (unsigned int)p[1];
    return (unsigned int)p[0] | ((unsigned int)p[1] << 8);
}
static inline unsigned int raf_ll_u32le(const unsigned char *p) {
    return (unsigned int)p[0]
         | ((unsigned int)p[1] << 8)
         | ((unsigned int)p[2] << 16)
         | ((unsigned int)p[3] << 24);
}

/* Caller supplies complete immutable bytes and result buffer.
 * Reads are bounded before indexing; never casts unaligned pointers.
 * Return status == RAF_LL_HEADER_PLAUSIBLE only for header plausibility.
 */
static inline unsigned int raf_ll_probe_v1(
    const unsigned char *p, unsigned long n, struct raf_ll_result_v1 *r
) {
    unsigned int be, klass, size, version, header, map;
    if (!r) return RAF_LL_INVALID_INPUT;
    r->kind = RAF_LL_UNKNOWN;
    r->status = RAF_LL_UNKNOWN_FORMAT;
    r->machine = 0u;
    r->format_version = 0u;
    r->declared_bytes = 0u;
    r->header_bytes = 0u;
    if (!p) {
        r->status = RAF_LL_INVALID_INPUT;
        return r->status;
    }
    if (n < 4ul) return r->status;

    if (p[0] == 0x7fu && p[1] == 0x45u &&
        p[2] == 0x4cu && p[3] == 0x46u) {
        r->kind = RAF_LL_ELF;
        if (n < 16ul) { r->status = RAF_LL_TRUNCATED; return r->status; }
        klass = (unsigned int)p[4];
        be = (unsigned int)p[5];
        if ((klass != 1u && klass != 2u) ||
            (be != 1u && be != 2u) || p[6] != 1u) {
            r->status = RAF_LL_UNSUPPORTED;
            return r->status;
        }
        header = klass == 1u ? 52u : 64u;
        if (n < (unsigned long)header) {
            r->status = RAF_LL_TRUNCATED;
            return r->status;
        }
        r->machine = raf_ll_u16(p + 18, be == 2u);
        r->format_version = raf_ll_u32le(p + 20);
        if (be == 2u) {
            r->format_version = ((unsigned int)p[20] << 24) |
                                ((unsigned int)p[21] << 16) |
                                ((unsigned int)p[22] << 8) |
                                (unsigned int)p[23];
        }
        r->header_bytes = raf_ll_u16(p + (klass == 1u ? 40u : 52u), be == 2u);
        if (r->format_version != 1u || r->header_bytes != header) {
            r->status = RAF_LL_MALFORMED;
            return r->status;
        }
        r->status = RAF_LL_HEADER_PLAUSIBLE;
        return r->status;
    }

    if (p[0] == (unsigned char)'d' && p[1] == (unsigned char)'e' &&
        p[2] == (unsigned char)'x' && p[3] == 0x0au) {
        r->kind = RAF_LL_DEX;
        if (n < 8ul) { r->status = RAF_LL_TRUNCATED; return r->status; }
        if (p[4] < (unsigned char)'0' || p[4] > (unsigned char)'9' ||
            p[5] < (unsigned char)'0' || p[5] > (unsigned char)'9' ||
            p[6] < (unsigned char)'0' || p[6] > (unsigned char)'9' ||
            p[7] != 0u) {
            r->status = RAF_LL_MALFORMED;
            return r->status;
        }
        version = 100u * ((unsigned int)p[4] - (unsigned int)'0') +
                   10u * ((unsigned int)p[5] - (unsigned int)'0') +
                   ((unsigned int)p[6] - (unsigned int)'0');
        r->format_version = version;
        /* DEX 036 is not documented for this legacy model; 041+ has container semantics: fail closed. */
        if (version < 35u || version == 36u || version > 40u) {
            r->status = RAF_LL_UNSUPPORTED;
            return r->status;
        }
        if (n < 112ul) { r->status = RAF_LL_TRUNCATED; return r->status; }
        size = raf_ll_u32le(p + 32);
        header = raf_ll_u32le(p + 36);
        map = raf_ll_u32le(p + 52);
        r->declared_bytes = size;
        r->header_bytes = header;
        if (header != 112u || n != (unsigned long)size ||
            raf_ll_u32le(p + 40) != 0x12345678u ||
            map < header || (unsigned long)map > n - 4ul) {
            r->status = RAF_LL_MALFORMED;
            return r->status;
        }
        r->status = RAF_LL_HEADER_PLAUSIBLE;
        return r->status;
    }

    return r->status;
}
#endif
