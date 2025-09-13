#ifndef NULL
#define NULL 0
#endif

#define RAND_MAX  2147483647

#define	abs(X)		((X)< 0  ? (-(X)) : (X))
/* these two aren't ANSI (but they should be!) */
#define	max(X,Y)	((X)>(Y) ? (X) : (Y))
#define	min(X,Y)	((X)>(Y) ? (Y) : (X))


/*
#ifndef UNSIGNED
# define UNSIGNED 0
# define INT long int
#else
# define INT unsigned long int
#endif



#if _LIBC - 0 == 0
# undef PARAMS
# if defined (__STDC__) && __STDC__
#  define PARAMS(Args) Args
# else
#  define PARAMS(Args) ()
# endif
#endif


INT strtol PARAMS ((const char *nptr, char **endptr, int base));
*/
