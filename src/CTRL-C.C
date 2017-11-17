/*PPU************************************************************************\
*
*    $Source: ctrl-c.c
*  $Revision: 0.7.3
*      $Date: Thu Jul  8 19:30:43 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "caliban.h"

/****************************************************************************/

# define MAX_IRQ 2

/****************************************************************************/

CHAR *Irq_Status[]={"-=- First  Interrupt-Request Signaled ! -=-",
                    "-=- Next   Request Will Be The Last   ! -=-",
                    "-=- Last   Interrupt-Request Signaled ! -=-"};

/****************************************************************************/
/*                                                                          */
/* Fx Name: Start_Irq                                                       */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Start_Irq(void)
{
 signal(SIGINT,Irq);

#ifndef BORLAND
 signal(SIGKILL,Irq);
 signal(SIGTERM,Irq);
 signal(SIGQUIT,Irq);
 signal(SIGTRAP,Irq);
 signal(SIGSTOP,Irq);
#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: --------                                                        */
/* Param. : --------                                                        */
/* Return : --------                                                        */
/* Written: By PPU !                                                        */
/* LastMod: --/--/--                                                        */
/*                                                                          */
/****************************************************************************/

void Irq(void)
{
 static INT Status_Level=NONE;

 Print(MESSAGE_COLOR,Irq_Status[Status_Level]);NL;

 Status_Level++;

 User_Quit_Program_CLog();
 User_Quit_Program_FLog();
 User_Quit_Program_DLog();
 User_Quit_Program_SLog();
 Release_All_Memory();FCloseall();
 Prompt();

 Clear_Screen();
 exit(OK);
}

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
