/*PPU************************************************************************\
*
*    $Source: caliban.h
*  $Revision: 0.7.4
*      $Date: Thu Jul  8 19:30:42 1999 
*
\************************************************************************PPU*/

void Rotate(void);
void Circular(void);

/****************************************************************************/

INT  Is_Ansi(void);
void Init_Video(void);

/****************************************************************************/

void Ck_Settings(void);
void Program_Limits(void);

/****************************************************************************/

void Mark(void);

/****************************************************************************/

void Man(void);
void Algo(void);
void Info(void);
void Title(void);
void CallMe(void);
void Author_Datas(void);

/****************************************************************************/

void Copyright_Long_Notice(void);
void Copyright_Short_Notice(void);

/****************************************************************************/

void Init_System(void);
void Init_Info_Structures(void);

/****************************************************************************/

# define _Program_Name "caliban"
# define _VersionP     "Ver. 9.9(BA.S.CO.) 3Val-Logic"
# define _Version      _VersionP

/****************************************************************************/

# define _Description  "A Parallel 3Values-Logic Event-Driven Fault Simulator "

/****************************************************************************/

#ifdef DEBUG
# define Prompt() Print(FBCyan,"\n[DEBUG-Mode:~]:>");
#else
# define Prompt() Print(FBCyan,"\n[3Val-Logic]["_Program_Name":~]:>");
#endif

/****************************************************************************/

# define _Year        " [1997/98]"

# define _Phone       "+39114334266"

/****************************************************************************/

# define Machine      "cclinf.polito.it"
# define Matricola    "s73259"

/****************************************************************************/

# define _PPU         "Ucchino G.P.P.(c)"_Year
# define _PPU2        "Ucchino G.P.P.(c) Known as PPU(c)"_Year

/****************************************************************************/

# define _Why         "Speed in Fault-Simulation (FK memory gain UAARGHH)    "
# define _Who         _Program_Name" Code Is Written By " _PPU
# define _Where       "Code Ideated, Written, and Debugged in a BathRoom     "

/****************************************************************************/

# define _PPUMail     "<ppu@"Machine">"
# define _PPUMail2    "<"Matricola"@"Machine">"
# define _PPUWww      "<http://"Machine"/~"Matricola">                     "

/****************************************************************************/

extern UINT Cols;

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
