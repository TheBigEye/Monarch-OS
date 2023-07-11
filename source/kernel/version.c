#include "version.h"

/* Kernel name. If you change this, you're not
 * my friend any more. */
char * __kernel_name = "butterfly";
char * __kernel_version = "1.2.5";

/* The release codename.
 *
 * History:
 *  * 0.0.X+ are part of the "whitewings" family
 */
char * __kernel_version_codename = "whitewings";

/* Build architecture (should probably not be
 * here as a string, but rather some sort of
 * preprocessor macro, or pulled from a script) */
char * __kernel_arch = "i686";

/* Rebuild from clean to reset these. */
char * __kernel_build_date = __DATE__;
char * __kernel_build_time = __TIME__;
