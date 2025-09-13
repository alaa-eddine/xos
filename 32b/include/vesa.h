#include <types.h>
#ifndef _VESA_H_
#define _VESA_H_

#define VESABASE 0x8F000

#pragma pack(push, 2)
typedef struct _vbe_info
{
 char sig[4];           /*(ret) signature ("VESA")*/
		        /*(call) VESA 2.0 request signature ("VBE2"), required to receive*/
		        /*  version 2.0 info*/
 u16 ver;   	        /*VESA version number (one-digit minor version -- 0102h = v1.2)*/
 u16 ofs_name;      	/*pointer to OEM name (ofs)*/
 u16 seg_name;      	/*pointer to OEM name (seg)*/
		        /*"761295520" for ATI*/
 u32 cflag; 	        /*capabilities flags (see #00078)*/
 u32 modes;        	/*pointer to list of supported VESA and OEM video modes*/
	           	/*(list of words terminated with FFFFh)*/
 u16 vram;         	/*total amount of video memory in 64K blocks*/


 u16 softver;      	/*OEM software version (BCD, high byte = major, low byte = minor)*/
 u32 vname;        	/*pointer to vendor name*/
 u32 pname;        	/*pointer to product name*/
 u32 prev;         	/*pointer to product revision string*/
 u16 afver;        	/*(if capabilities bit 3 set) VBE/AF version (BCD)*/
	           	/*0100h for v1.0P*/
 u32 amodes;       	/*(if capabilities bit 3 set) pointer to list of supported*/
		        /*accelerated video modes (list of words terminated with FFFFh)*/
 char res0[216];      	/*reserved for VBE implementation*/
 char res1[256];       	/*OEM scratchpad (for OEM strings, etc.)*/
} vbe_info __attribute__ ((packed));


int isvesa(vbe_info * vbe);
char * get_sig(vbe_info * vbe);
char * get_oemname(vbe_info * vbe);
u16 * get_vram(vbe_info * vbe);
void print_vinfo(vbe_info * vbe);
#endif



