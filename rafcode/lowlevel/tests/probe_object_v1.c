/* Freestanding compile sentinel: no standard headers, no unresolved libc symbols. */
#include "raf_ll_probe_v1.h"
unsigned int raf_probe_object_entry(const unsigned char *p, unsigned long n,
                                    struct raf_ll_result_v1 *r) {
    return raf_ll_probe_v1(p, n, r);
}
