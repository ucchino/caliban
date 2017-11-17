/*PPU************************************************************************\
*
*    $Source: times.c
*  $Revision: 0.7.1
*      $Date: Thu Jul  8 19:31:07 1999 
*
\************************************************************************PPU*/

# include "libs.h"

/****************************************************************************/

# include <time.h>
# include <sys/types.h>

#ifndef BORLAND
# include <sys/times.h>
#endif

/****************************************************************************/

time_t  Program_Start_Date;

clock_t Simulation_Stop_Time;
clock_t Simulation_Start_Time;
clock_t Simulation_Elapsed_Time;

/****************************************************************************/
/*                                                                          */
/* Fx Name: Print_Time                                                      */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Print_Time(void)
{
 clock_t Current;
 CHAR    String[STRING_LEN];

 Bar(BAR_COLOR);
 Get_Date(String,Program_Start_Date);
 Print2(FBWhite,"Program Start :",String);
 
 Get_Clock(Current);

 SPrint_Time(String,Current);
 Print2(FBWhite,"Elapsed Time  :",String);
 Bar(BAR_COLOR);
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
