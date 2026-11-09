# RAFCODEPHI licensing and provenance boundary V1 (HOLD)

**No two-license assertion is being made merely by keeping files in separate directories.** The actual legal conditions depend on copyright, derivation, linking, distributions and each file's applicable notices. This is a technical separation proposal, not legal advice and not a new license grant.

## Partition

| Partition | Recorded source | Current treatment |
|---|---|---|
| `LICENSE`, `ADDITIONAL_LICENSE_INFO`, `ASSEMBLY_EXCEPTION`; original `src/`, `make/`, `test/`, `doc/` | OpenJDK code in this fork, with GPLv2 and applicable Classpath/assembly exceptions plus other source-specific licenses | Preserve unchanged and retain attribution, original notice and license terms. This V1 does not replace or relicense it. |
| `rafcode/lowlevel/**` | New isolated independent addition in this PR; written without copying upstream OpenJDK source | **AUTHORED_CANDIDATE / LICENSE_DECISION_PENDING**. No fabricated MIT/GPL/ClassPath grant or copyright transfer. Need author's explicit license text and compatibility review before distribution/combination. |
| ART, Android loaders, kernel, boot JDK, GCC/Clang | External platform/build dependencies | Not owned, not contained in this module. Each governed by its upstream license and platform APIs. |

**Requirements before claiming dual licensing:** verify who owns each new file; choose complete authorial license text; inspect any preexisting source/test similarities and third-party contributions; evaluate linking and distribution with OpenJDK and exceptions with a qualified legal review. A physical separation is evidence of architecture, **not proof of legal isolation**.

**Authorial claim:** RAFCODEPHI is the user's project identifier; objective authorship and legal rights require the actual provenance chain. `TOKEN_VAZIO` until rights/license verified.

**Release gate:** `claim_allowed=false`, `license_compatibility=TOKEN_VAZIO`, `release_allowed=false`, `merge_allowed=false`. Leave upstream licenses intact. Rollback by closing independent draft branch, no delete in upstream.
