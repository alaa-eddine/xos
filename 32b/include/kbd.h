#include <types.h>
#ifndef _KBD_H_
#define _KBD_H_
typedef enum e_keymaps {km_fr, km_en} keymaps;


void kbd_int(void);
char getSC(void);
char * kb_drvid(void);
void switch_num_lock();
void switch_caps_lock();
void switch_scroll_lock();
short isnum();
short iscaps();
void kbd_led(char status);
int set_keymap(keymaps km);
char key(unsigned int val);

#endif

