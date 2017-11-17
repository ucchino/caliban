/*PPU************************************************************************\
*
*    $Source: memory.c
*  $Revision: 0.6.6
*      $Date: Thu Jul  8 19:30:59 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "sim.h"
# include "gengis.h"
# include "load_cir.h"
# include "load_fau.h"
# include "load_pat.h"

/****************************************************************************/

void Comma(FLOAT Value,CHAR* String);

/****************************************************************************/
/*                                                                          */
/* Fx Name: Comma                                                           */
/* Param. : float,char*                                                     */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Comma(FLOAT Value,CHAR* String)
{
 ULONG index1=NONE; 

 Strset(String);

 sprintf(String,"%.3f",Value);
 for(index1=NONE;(index1 < strlen(String));index1++) 
 {
  if(String[index1]=='.') String[index1]=',';
 } 
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: SPrint_Mem_Used                                                 */
/* Param. : char*,unsigned long                                             */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void SPrint_Mem_Used(CHAR* String,ULONG mem)
{
 CHAR Src[STRING_LEN];

 Strset(Src);
 Strset(String);

 if(mem <= NONE) 
 {
  sprintf(String,"None");
 }
 else if(mem < 1024) 
 {
  sprintf(String,"%lu Byte%s",mem,S(mem));
 }
 else if(mem < (1024.0*1024.0)) 
 {
  Comma((mem/1024.0),Src);
  sprintf(String,"%s Kbyte%s",Src,S(mem/1024));
 }
 else if(mem < (1024.0*1024.0*1024.0))
 {
  Comma((mem/1024.0/1024.0),Src);
  sprintf(String,"%s Mbyte%s",Src,S(mem/1024/1024));
 }
 else
 {
  Comma((mem/1024.0/1024.0/1024.0),Src);
  sprintf(String,"%s Gbyte%s",Src,S(mem/1024/1024/1024));
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Allocate_Memory                                                 */
/* Param. : void*,unsigned long,unsigned long,char*,char*                   */
/* Return : unsigned long                                                   */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

ULONG Allocate_Memory(void* Pointer,ULONG NumBytes,ULONG NumBlocks,CHAR* Pointer_Name,CHAR* Pointer_Type)
{
 ULONG Mem_Used=NONE;
 
 CHAR  Str[STRING_LEN];
 CHAR  STR[STRING_LEN];

 Strset(Str);
 Strset(STR);
 
 Mem_Used=NONE;

 if(NumBlocks > 0)
 {
  Mem_Used=(NumBlocks*NumBytes);
  if(Pointer==NULL)
  {
   SPrint_Mem_Used(Str,Mem_Used);
   Print2(ERROR_COLOR,"Exeeding Requested Memory:",Str);
   Sys_Error(Pointer_Name,E_ALLOC);Break(ERROR);
  }
  else SPrint_Mem_Used(Str,Mem_Used);
  
  BarV(BAR_COLOR);
  sprintf(STR,"Allocated [%s] For: [%s]",Str,Pointer_Name);
  PrintV(MESSAGE_COLOR,STR);
  sprintf(STR,"Alias %lu [%s] Type Block%s",NumBlocks,Pointer_Type,S(NumBlocks));
  PrintV(Normal,STR);
  sprintf(STR,"Corrispondent To [%lu] Bytes For A Block",NumBytes);
  PrintV(Normal,STR);
  BarV(BAR_COLOR);
  memset(Pointer,0x00,Mem_Used);
 }
 else 
 {
  Sys_Error(Pointer_Name,E_ALLOC_ZERO);Break(ERROR);
 }
 return(Mem_Used);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_All_Memory                                              */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_All_Memory(void)
{
 Release_Circuit();
 Release_Pattern();
 Release_Fault_List();
 
 Release_FSim_Mem();
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
