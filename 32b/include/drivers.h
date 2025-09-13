/*
X-OS

@Auteur : Alaa-eddine KADDOURI

@Description: structure des driver sous XOS.

@Note : cette structure est temporaire, elle doit etre ameliorée par la suite on fonction des besions
*/

typedef struct _xos_driver_struct
{
  char * driverid;
  char * author;
  int (*init)(void);
} xosdriver;
