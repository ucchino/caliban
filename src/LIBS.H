/*PPU************************************************************************\
*
*    $Source: libs.h
*  $Revision: 0.7.0
*      $Date: Thu Jul  8 19:30:54 1999 
*
\************************************************************************PPU*/

#ifdef __MSDOS__
# include <dos.h>
# include <conio.h>
#endif

/****************************************************************************/

#if __TURBOC__ || __BORLANDC__ || __BCPLUSPLUS__ || __TCPLUSPLUS__
# define BORLAND       1
# define ANSI          1
# define STILL_TESTING 1
# define FS32BIT       1
# define _Compiler     "BORLAND"
# include <process.h>
#endif

/****************************************************************************/

#ifdef __GNUC__
# define  GNU 1
# define _Compiler "GNUC"
#ifdef __MSDOS__
# include <std.h>
# include <osfcn.h>
# include <unistd.h>
# include <stdarg.h>
# include <sys/times.h>
#else
# include <unistd.h>
# include <termcap.h>
# include <sys/times.h>
# include <sys/types.h>
#endif
#endif

/****************************************************************************/

# define Directory()                  system(Dir)

# define Unix_Dir                     "ls *.*"

/****************************************************************************/

#ifdef  System
#undef  System
#endif

/****************************************************************************/

#ifdef   __sun
#ifdef   Solaris
# define System "Solaris"
# define Dir    
# define NO_STDIO_PROTOS
#endif
#ifdef __sparc__
# define System "SunSparc"
# define Dir    Unix_Dir
# define NO_STDIO_PROTOS
#else
# define System "SunOS"
# define Dir    Unix_Dir
# define NO_STDIO_PROTOS
#endif
#endif

/****************************************************************************/

#ifdef __mips__
# define System "Mips"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   __arm__
# define System "Arm-OS (Mad-Max)"
# define Dir    "exe *"
#endif

/****************************************************************************/
        
#ifdef   AIX
# define System "AIX 2.2.1"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   _AIX
# define System "AIX 3.x"
# define Dir    ""
#endif  

/****************************************************************************/

#ifdef pyr
# define System "OSx"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   SCOXNX
# define System "SCO Xenix"
# define Dir    ""
#endif 

/****************************************************************************/

#ifdef   __sgi
# define System "IRIX"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   SCO322
# define System "SCO Unix 3.2v2"
# define Dir    Unix_Dir
#endif 

/****************************************************************************/

#ifdef   SCO324
# define System "SCO Unix 3.2v4"
# define Dir    Unix_Dir
#endif

/****************************************************************************/

#ifdef   linux
# define System "Linux"
# define Dir    Unix_Dir
#endif

/****************************************************************************/

#ifdef   aux
# define System "A/UX"
# define Dir    Unix_Dir
#endif

/****************************************************************************/

#ifdef   ISC
# define System "Interactive Unix"
# define Dir    Unix_Dir
#endif

/****************************************************************************/

#ifdef   __NeXT__
# define System "NeXTStep"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   SVR4
# define System "System V.4"
# define Dir    ""
#endif 

/****************************************************************************/

#ifdef   ultrix
# define System "Ultrix"
# define Dir    Unix_Dir
#endif  

/****************************************************************************/

#ifdef   SINIX
# define System "SINIX"
# define Dir    Unix_Dir
#endif

/****************************************************************************/

#ifdef   _SEQUENT_
# define DYNIXPTX 1
# define System "Dynix/PTX"
# define Dir    ""
#endif   

/****************************************************************************/

#ifdef   DYNIX
# define System "Dynix"
# define Dir    ""
#endif   

/****************************************************************************/

#ifdef   BULL
# define System "Bull DPX/2 BOS"
# define Dir    "" 
#endif     

/****************************************************************************/

#ifdef   __dgux
# define System "DG/UX"
# define Dir    ""
#endif   

/****************************************************************************/

#ifdef   __bsdi__
# define System "BSDi"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   __FreeBSD__
# define System "FreeBSD"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   __hpux
#ifndef  HPUX
# define HPUX TRUE
# define System "HP-UX"
# define Dir    ""
#endif
#endif

/****************************************************************************/

#ifdef   __QNX__
# define QNX
# define System "QNX 4.21 (POSIX)"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   apollo
# define System "Apollo"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef   __osf__
#ifdef   __alpha__
# define System "DEC OSF/1 Alpha"
# define Dir    ""
#endif
#endif

/****************************************************************************/

#ifdef   __MSDOS__
# define System "MsDos Or 4Dos"
# define Dir    "dir *.* /w/p"
#endif

/****************************************************************************/

#ifndef  System
# define System "Unknown"
# define Dir    ""
#endif

/****************************************************************************/

#ifdef NO_STD_PROTOS
extern int    tolower(int);
extern int    toupper(int);
extern int    fgetc(FILE *);
extern int    fflush(FILE *);
extern int    fclose(FILE *);
extern int    rewind(FILE *);
extern int    fputc(int, FILE *);
extern int    printf(char *,...);
extern int    fputs(char *, FILE *);
extern int    rename(char *, char *);
extern int    sscanf(char *, char *,...);
extern int    fprintf(FILE *, char *,...);
extern int    gettimeofday(struct timeval *, struct timezone *);
extern time_t mktime(struct tm *);
extern time_t time(time_t *);
#endif

/****************************************************************************/

# include <math.h>
# include <time.h>
# include <stdio.h>
# include <ctype.h>
# include <limits.h>
# include <signal.h>
# include <string.h>
# include <stdlib.h>
# include <values.h>
# include <stdarg.h>
# include <stddef.h>

/****************************************************************************/

# include "types.h"    /* Don't Move From This Position */
# include "string.h"   /* Don't Move From This Position */

/****************************************************************************/

# include "iofx.h"
# include "iosys.h"
# include "debug.h"
# include "modes.h"
# include "times.h"
# include "ctrl-c.h"
# include "macros.h"
# include "memory.h"
# include "random.h"
# include "strings.h"
# include "ansicode.h"
# include "gatetest.h"

/****************************************************************************/

# include "files.h"
# include "gates.h"
# include "struct.h"

# include "3values.h"

/*PPU************************************************************************\
*  ________________________________________________________________________  * 
* /                                                                        \ * 
* \    "Quiero Hacer Con Tigo Lo Que La Primavera Hace Con Los Cerezos"    / * 
*  \                                                                      /  * 
*   \                                                                    /   * 
*    \                      ==== PABLO NERUDA ====                      /    * 
*     \________________________________________________________________/     * 
*                                                                            * 
*    ____________________________________________________________________    * 
*   /                                                                    \   * 
*   \              .oO=> THIS IS THE END OF THE FILE! <=Oo.              /   * 
*    \__________________________________________________________________/    * 
*                                                                            *
\************************************************************************PPU*/
