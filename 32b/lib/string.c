/*
@Auteur : Alaa-eddine KADDOURI
@Projet : X Operating System

@Description :



*/

#include <string.h>

/**
 * strcmp - Compare two strings
 * @cs: One string
 * @ct: Another string
 */
int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}


/**
 * strchr - Find the first occurrence of a character in a string
 * @s: The string to be searched
 * @c: The character to search for
 */
char * strchr(const char * s, int c)
{
	for(; *s != (char) c; ++s)
		if (*s == '\0')
		return '\0';
	return (char *) s;
}

/**
 * strlen - Find the length of a string
 * @s: The string to be sized
 */
int strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}
/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 */
char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

/**
 * strncpy - Copy a length-limited, %NUL-terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 * @count: The maximum number of bytes to copy
 *
 * The result is not %NUL-terminated if the source exceeds
 * @count bytes.
 */
char * strncpy(char * dest,const char *src,unsigned int count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0) src++;
		tmp++;
		count--;
	}
	return dest;
}



/**
 * strcat - Append one %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 */
char * strcat(char * dest, const char * src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;

	return tmp;
}





/**
 * strsep - Split a string into tokens
 * @s: The string to be searched
 * @ct: The characters to search for
 *
 * strsep() updates @s to point after the token, ready for the next call.
 *
 * It returns empty tokens, too, behaving exactly like the libc function
 * of that name. In fact, it was stolen from glibc2 and de-fancy-fied.
 * Same semantics, slimmer shape. ;)
 */

char * strsep(char **s, const char *ct)
{
  char *sbegin = *s, *end;

  if (sbegin == NULL)
    return NULL;

  end = strpbrk(sbegin, ct);
  if (end)
    *end++ = '\0';
  *s = end;

  return sbegin;
}

/**
 * strpbrk - Find the first occurrence of a set of characters
 * @cs: The string to be searched
 * @ct: The characters to search for
 */
char * strpbrk(const char * cs,const char * ct)
{
  const char *sc1,*sc2;

  for( sc1 = cs; *sc1 != '\0'; ++sc1) {
    for( sc2 = ct; *sc2 != '\0'; ++sc2) {
      if (*sc1 == *sc2)
	return (char *) sc1;
    }
  }
  return NULL;
}

