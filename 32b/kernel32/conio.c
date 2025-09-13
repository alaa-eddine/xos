/*
@Auteur : Alaa-eddine KADDOURI
@Projet : X Operating System

@Description :



*/


#include <sys.h>
#include <conio.h>
#include <lib.h>
#include <mem.h>

#define MIN_X 0
#define MIN_Y 0

#define MAX_X 79
#define MAX_Y 49


unsigned short int curseur=0;
unsigned int posX=0;

//char Xmin = 11;
char kX = MIN_X;	/* position en X du curseur */
char kY = MIN_Y;	/* position en Y du curseur */
char kattr = 0x1F;


void updtAttr(void)
{
	unsigned char* video;
	video = (unsigned char*) (RAMSCREEN + 2 * kX + kY * 160);
	*(video+1) = kattr;
}


void updateCursor(void)
{
	unsigned int adresse=kX + kY * 80;

	attrb(kattr);
	updtAttr();
	outbp(0x3D4, 0x0F);
	outbp(0x3D5, adresse&0xFF);
	outbp(0x3D4, 0x0E);
	outbp(0x3D5, (adresse>>8)&0xFF);
}

int getX(void)
{
	return kX;
}

int getY(void)
{
	return kY;
}

/* nline = nombre de lignes a scroller */
void scrollup (unsigned int nline)
{
  unsigned char* video, *tmp;

  for(video=(unsigned char*)RAMSCREEN ; video<(unsigned char*)RAMSCREEN+160 ;video++)
      {
      *video++;
      *video=0x0F;
      }
  for(video=(unsigned char*)RAMSCREEN+160 ; video<(unsigned char*)RAMSCREEN+320 ;video++)
        {
	      *video++='\xc4';
	      *video=0x04;
	}
			  
for(video=(unsigned char*)RAMSCREEN+320 ; video<(unsigned char*)SCREENLIM ;video++)
    {
      tmp = (unsigned char*) (video+nline*160);


      if(tmp<(unsigned char*)SCREENLIM)
	{
	  *video++ = *tmp++;
	  *video=*tmp;

	}
      else
	{
	  *video++ = 0;
	  *video=kattr;
	}

    }

  kY-=nline;
  if(kY<MIN_Y) kY=MIN_Y;

}




int pos(void)
{
  return (kX+80*kY)*2;
}

void putcar(char c)
{
  if (c == '\n')
    {
      kX=0;
      kY++;
    }
  else
    {
      if (pos() < SIZESCREEN)
	{
	  memset((void*)(RAMSCREEN+pos()),c,1);
	  memset((void*)(RAMSCREEN+pos()+1),kattr,1);
	}
      kX++;
      if (kX > MAX_X)
	{
	  kX=0;
	  kY++;
	}
    }

  if (kY > MAX_Y - MIN_Y)
      scrollup(1);

  outbp(0x3D4, 0x0F);
  outbp(0x3D5, (pos()/2)&0xFF);
  outbp(0x3D4, 0x0E);
  outbp(0x3D5, ((pos()/2)>>8)&0xFF);
}

/*** On utilise printk désormé
void print (char* s)
{
  int i=0;
  while(s[i]!='\0')
   putcar(s[i++]);
}
*/

void temoin(void)
{
	int tempX=kX;
	int tempY=kY;
	int tempattr=kattr;
	gotoxy(79,0);
	attrb(0x4E);
	if (curseur>3) curseur=0;
	switch (curseur)
	{
		case 0 : putcar('-'); break;
		case 1 : putcar('\\'); break;
		case 2 : putcar('|'); break;
		case 3 : putcar('/'); break;
	}
	curseur++;
	attrb(tempattr);
	gotoxy(tempX,tempY);
}

void printsc(unsigned int sc)
{
	int tempX=kX;
	int tempY=kY;
	int tempattr=kattr;
	gotoxy(70, 0);
	attrb(0x0F);
	printk("Key=0x%x ",sc);
	attrb(tempattr);
	gotoxy(tempX,tempY);	
}

void printNb(unsigned long nb)
{
	int i;
	char string[32];
	short int ptr=0; // pointeur sur string
	char car;

	do
	{
		car=nb % 10 + '0';
		nb/=10;
		for (i=30;i>=0;i--)
			string[i+1]=string[i];
		string[0]=car;
		ptr++;
		//print(".");
	} while (nb!=0);
	string[ptr]='\0';
	printk(string);
}

void printNbHex(unsigned long int nb)
{
	char map[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	
	int  temp;
	char string[32];
	short int ptr=0; //pointeur sur string
	char car;
	int i;

	do
	{
		temp=nb % 16;

		car=map[temp]; 		
		for (i=30;i>=0;i--)
		{
			string[i+1]=string[i];
		}
		string[0]=car;
		ptr++;
		nb=nb / 16;
	} while (nb!=0);

	string[ptr]='\0';
	printk(string);
}

void printOctetBin(unsigned int nb)
{
	//unsigned int nb=param;
	unsigned short int temp=0;
	int i,j;
	unsigned char string[32];
	unsigned char car;

	for (j=0;j<8;j++)
	{
		temp=nb % 2;
		car=temp+48;
		for (i=31;i>0;i--)
		{
			string[i]=string[i-1];
		}
		string[0]=car;
		nb=nb / 2;
	}
	string[8]=0;
	printk(string);
}


void gotoxy (int gX, int gY)
{
	kX=gX;
	kY=gY;
	updateCursor();
}



void attrb (int attribut)
{
	kattr=attribut;
}

int getattrb(void)
{
   return kattr;
}

void clrscr(void)
{
	scrollup(MAX_Y - MIN_Y);
}

void printHeure(unsigned int param)
{
	unsigned int nb=param;
	unsigned short int temp=0;
	int i,j;
	unsigned char string[32];
	unsigned char car;

	for (j=0;j<2;j++)
	{
		temp=nb % 16;
		car=temp+48;
		for (i=31;i>0;i--)
		{
			string[i]=string[i-1];
		}
		string[0]=car;
		nb=nb / 16;
	}
	string[2]=0;
	printk(string);
}


void XOSmsg(const char *msg)
{
  char temp=kattr;
  attrb(0x19);
  printk("[");
  attrb(0x1A);
  printk(msg);
  attrb(0x19);
  printk("]");
  attrb(temp);
}
void XOSmsgln(const char *msg)
{
  XOSmsg(msg);
  printk("\n");
}

void ok(void){
  gotoxy(73, getY());
  XOSmsgln(" ok ");
}

void star(void)
{
  char temp=kattr;
  attrb(0x1E);
  printk("*");
  attrb(temp);
}
