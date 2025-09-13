#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#define RAMSCREEN 0xB8000	/* debut de la memoire video */
#define SIZESCREEN 0x1F40 	/* 8000, nombres d'octets d'une page texte 80x50 */
#define SCREENLIM 0xB9F40
#define FALSE 0
#define TRUE (!FALSE)


void updtAttr(void);
void updateCursor(void);
int getX(void);
int getY(void);
void scrollup (unsigned int nline);
void print (char* string);
void printd (unsigned int x);
void putcar (char car);
void temoin(void);
void printsc(unsigned int sc);
void printNb(unsigned long nb);
void printNbHex(unsigned long int nb);
void printOctetBin(unsigned int nb);
void gotoxy (int gX, int gY);

void attrb (int attribut);
int getattrb(void);

void clrscr(void);
void printHeure(unsigned int param);

void XOSmsg(const char *msg);
void XOSmsgln(const char *msg);
void ok(void);
void star(void);
#endif


