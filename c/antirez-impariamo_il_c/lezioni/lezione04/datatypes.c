#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    /* ================= FLOATING POINT ================= */

    printf("FLOATING POINT RANGES\n");
    printf("---------------------\n");

    /* float */
    printf("float:\n");
    printf("  min positive  = %e\n",  FLT_MIN);
    printf("  max positive  = %e\n",  FLT_MAX);
    printf("  min negative  = %e\n", -FLT_MAX);
    printf("  precision     = %d digits\n\n", FLT_DIG);

    /* double */
    printf("double:\n");
    printf("  min positive  = %e\n",  DBL_MIN);
    printf("  max positive  = %e\n",  DBL_MAX);
    printf("  min negative  = %e\n", -DBL_MAX);
    printf("  precision     = %d digits\n\n", DBL_DIG);

    /* long double */
    printf("long double:\n");
    printf("  min positive  = %Le\n",  LDBL_MIN);
    printf("  max positive  = %Le\n",  LDBL_MAX);
    printf("  min negative  = %Le\n", -LDBL_MAX);
    printf("  precision     = %d digits\n\n", LDBL_DIG);

    /* ================= INTEGER TYPES ================= */

    printf("INTEGER TYPE RANGES\n");
    printf("-------------------\n");

    printf("CHAR_BIT   = %d\n\n", CHAR_BIT);

    printf("signed char:\n");
    printf("  min = %d\n", SCHAR_MIN);
    printf("  max = %d\n\n", SCHAR_MAX);

    printf("unsigned char:\n");
    printf("  max = %u\n\n", UCHAR_MAX);

    printf("char:\n");
    printf("  min = %d\n", CHAR_MIN);
    printf("  max = %d\n\n", CHAR_MAX);

    printf("short:\n");
    printf("  min = %d\n", SHRT_MIN);
    printf("  max = %d\n\n", SHRT_MAX);

    printf("unsigned short:\n");
    printf("  max = %u\n\n", USHRT_MAX);

    printf("int:\n");
    printf("  min = %d\n", INT_MIN);
    printf("  max = %d\n\n", INT_MAX);

    printf("unsigned int:\n");
    printf("  max = %u\n\n", UINT_MAX);

    printf("long:\n");
    printf("  min = %ld\n", LONG_MIN);
    printf("  max = %ld\n\n", LONG_MAX);

    printf("unsigned long:\n");
    printf("  max = %lu\n\n", ULONG_MAX);

    printf("long long:\n");
    printf("  min = %lld\n", LLONG_MIN);
    printf("  max = %lld\n\n", LLONG_MAX);

    printf("unsigned long long:\n");
    printf("  max = %llu\n", ULLONG_MAX);

    return 0;
}
