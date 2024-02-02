/* Exploratory program to reveal size of types on the system */
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  /* Declare types */

  // Data types
  int a;
  unsigned int b;
  short c;
  unsigned short d;
  long e;
  unsigned long f;
  char g;
  unsigned char h;
  signed char i;
  float j;
  double k;
  long double l;

  /* Pointer types */
  int *ip;
  float *fp;
  char *cp;
  void *vp;

  /* Print it all */

  // Size of primitive data types
  printf("Size of primitive data types in bytes:\n");
  printf(" int size\t:%lu\n", sizeof(a));
  printf(" unsigned int\t:%lu\n", sizeof(b));
  printf(" short size\t:%lu\n", sizeof(c));
  printf(" unsigned short\t:%lu\n", sizeof(d));
  printf(" long	\t:%lu\n", sizeof(e));
  printf(" unsigned long\t:%lu\n", sizeof(f));
  printf(" char	\t:%lu\n", sizeof(g));
  printf(" unsigned char\t:%lu\n", sizeof(h));
  printf(" signed char\t:%lu\n", sizeof(i));
  printf(" float size\t:%lu\n", sizeof(j));
  printf(" double	\t:%lu\n", sizeof(k));
  printf(" long double\t:%lu\n", sizeof(l));

  // Size of pointers

  printf("\nSize of pointers in bytes:\n");

  printf(" int pointer\t:%lu\n", sizeof(ip));
  printf(" float pointer\t:%lu\n", sizeof(fp));
  printf(" char pointer\t:%lu\n", sizeof(cp));
  printf(" void pointer\t:%lu\n", sizeof(vp));

  // Limits of Types - Minimum and maximum values of types
  printf("\nLimits of types - minimum and maximum values:\n");
  printf(" CHAR_BIT\t:%d\n", CHAR_BIT);
  printf(" MB_LEN_MAX\t:%d\n\n", MB_LEN_MAX);

  printf(" CHAR_MIN\t:%+d\n", CHAR_MIN);
  printf(" CHAR_MAX\t:%+d\n", CHAR_MAX);
  printf(" SCHAR_MIN\t:%+d\n", SCHAR_MIN);
  printf(" SCHAR_MAX\t:%+d\n", SCHAR_MAX);
  printf(" UCHAR_MAX\t:%u\n\n", UCHAR_MAX);

  printf(" SHRT_MIN\t:%+d\n", SHRT_MIN);
  printf(" SHRT_MAX\t:%+d\n", SHRT_MAX);
  printf(" USHRT_MAX\t:%u\n\n", USHRT_MAX);

  printf(" INT_MIN\t:%+d\n", INT_MIN);
  printf(" INT_MAX\t:%+d\n", INT_MAX);
  printf(" UINT_MAX\t:%u\n\n", UINT_MAX);

  printf(" LONG_MIN\t:%+ld\n", LONG_MIN);
  printf(" LONG_MAX\t:%+ld\n", LONG_MAX);
  printf(" ULONG_MAX\t:%lu\n\n", ULONG_MAX);

  printf(" LLONG_MIN\t:%+lld\n", LLONG_MIN);
  printf(" LLONG_MAX\t:%+lld\n", LLONG_MAX);
  printf(" ULLONG_MAX\t:%llu\n\n", ULLONG_MAX);

  printf(" PTRDIFF_MIN\t:%td\n", PTRDIFF_MIN);
  printf(" PTRDIFF_MAX\t:%+td\n", PTRDIFF_MAX);
  printf(" SIZE_MAX\t:%zu\n", SIZE_MAX);
  printf(" DBL_MIN\t:%lf\n", DBL_MIN);
  printf(" DBL_MAX\t:%lf\n", DBL_MAX);
  printf(" SIG_ATOMIC_MIN\t:%+jd\n", (intmax_t)SIG_ATOMIC_MIN);
  printf(" SIG_ATOMIC_MAX\t:%+jd\n", (intmax_t)SIG_ATOMIC_MAX);
  printf(" WCHAR_MIN\t:%+jd\n", (intmax_t)WCHAR_MIN);
  printf(" WCHAR_MAX\t:%+jd\n", (intmax_t)WCHAR_MAX);
  printf(" WINT_MIN\t:%jd\n", (intmax_t)WINT_MIN);
  printf(" WINT_MAX\t:%jd\n", (intmax_t)WINT_MAX);

  return 0;
}
