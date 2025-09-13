/*
@Auteur : Alaa-eddine KADDOURI
@Projet : X Operating System

@Description : convertion d'une chaine en un entier
si la chaine commance par 0x : la convertion est faite en base 16
si la chaine commance par 0  : la convertion est faite en base 8
sinon la convertion est faite en base 10



@Fonctions externes :
    * <string.h> : strchr()


@NOTE : pour les chaines en hexadecimal seul les caractères en MAJUSCULES sont accepte
*/

#include <string.h>
int toint(const char digit, int base)
{
  char uhexdigit[] ="0123456789ABCDEF";


  switch(base){
  case 8:
    {
      if((digit-'0') <= 7)
	return digit-'0';
    }break;
  case 10:
    {
      if((digit-'0') <= 9)
	return digit-'0';
    }break;
  case 16:
    {
      if((strchr(uhexdigit, (int)digit) - uhexdigit) <= 15)	
	return strchr(uhexdigit, (int)digit) - uhexdigit;	
	  
    }break;
  }
  return -1;
}

int atoi (const char *nptr)
{
  char * ptr = (char *)nptr;
  int nbr=0;
  char digit;
  int base=10;
  
  if (*ptr == '0')
    {
      ptr++;      
      if((*ptr == 'x') || (*ptr == 'X'))
	{
	  ptr++;
	  base = 16;
	}
      else 
	{
	  base = 8;
	  ptr--;
	}
    }
    
  while((digit=*ptr++))
    if (toint(digit, base) >= 0)
      nbr = nbr*base + toint(digit, base);
    else
      return 0;

  return nbr;
}

