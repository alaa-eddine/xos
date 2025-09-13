#ifndef _STRING_H_
#define _STRING_H_

#define NULL 0 
int strcmp(const char * cs,const char * ct);
char * strsep(char **s, const char *ct);
int strlen(const char * s);
char * strchr(const char * s, int c);
char * strpbrk(const char * cs,const char * ct);
char * strsep(char **s, const char *ct);
char * strcat(char * dest, const char * src);
char * strncpy(char * dest,const char *src,unsigned int count);
char * strcpy(char * dest,const char *src);
int strlen(const char * s);

#endif
