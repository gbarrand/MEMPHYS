/* 
 * G.Barrand : copied from the pain.
 *             remove R__EXTERN and #ifndef __CINT__
 */

#ifndef OUREX__GETLINE_H
#define OUREX__GETLINE_H

#ifdef __cplusplus
extern "C" {
#endif

/*typedef enum { kInit = -1, kLine1, kOneChar, kCleanUp } EGetLineMode;*/

char *Getline(const char *prompt);
char *Getlinem(int mode, const char *prompt);
void Gl_config(const char *which, int value);
void Gl_setwidth(int width);
void Gl_windowchanged();
void Gl_histinit(char *file);
void Gl_histadd(char *buf);
int  Gl_eof();

int Gl_is_valid(); /*G.Barrand*/
int Gl_is_there_stdin_input(int*); /*G.Barrand*/
void Gl_erase_line(); /*G.Barrand*/
int Gl_is_windows_console(); /*G.Barrand*/

extern int (*Gl_in_hook)(char *buf);
extern int (*Gl_out_hook)(char *buf);
extern int (*Gl_tab_hook)(char *buf, int prompt_width, int *cursor_loc,void* tag); /*G.Barrand : add void* tag.*/
extern int (*Gl_in_key)(int key);
extern void* Gl_tab_tag; /*G.Barrand.*/

#ifdef __cplusplus
}
#endif

#endif   /* OUREX_GETLINE_ */
