/*PPU************************************************************************\
*
*    $Source: files.c
*  $Revision: 0.7.6
*      $Date: Thu Jul  8 19:30:48 1999 
*
\************************************************************************PPU*/

# include "libs.h"

/****************************************************************************/

FILE_BLOCK File[]={{NULL,".clf","--UNKNOWN--"},
                   {NULL,".slf","--UNKNOWN--"},
                   {NULL,".dlf","--UNKNOWN--"},
                   {NULL,".flf","--UNKNOWN--"},
                   {NULL,".cfg","--UNKNOWN--"},
                   {NULL,".xxx","--UNKNOWN--"},
                   {NULL,".inp","--UNKNOWN--"},
                   {NULL,".cal","--UNKNOWN--"},
                   {NULL,".edf","--UNKNOWN--"},
                   {NULL,".dee","--UNKNOWN--"},
                   {NULL,".fau","--UNKNOWN--"},
                   {NULL,".gen","--UNKNOWN--"},
                   {NULL,".ffi","--UNKNOWN--"},
                   {NULL,".cbf","--UNKNOWN--"},
                   {NULL,".man","--UNKNOWN--"},
                   {NULL,".alg","--UNKNOWN--"}};

/****************************************************************************/

CHAR* Open_Mode[]={"r","w","a"};

/****************************************************************************/
/*                                                                          */
/* Fx Name: Fopen                                                           */
/* Param. : char*,int,int,int,int                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Fopen(CHAR* Name,INT Id,INT Ext,INT Mode,INT Error)
{
 strcpy(File[Id].Name,Name);
 strcat(File[Id].Name,File[Ext].Ext);
 
 if((File[Id].Name==NULL) || (strlen(File[Id].Name) > FNLEN) || (strlen(File[Id].Name) == NONE))
 {
  Sys_Error(File[Id].Name,E_FILENAMELEN);
  return(ERROR);
 }
 
 if(File[Id].Ptr==NULL)
 {
  if((File[Id].Ptr=fopen(File[Id].Name,Open_Mode[Mode]))==NULL)
  {
   Sys_Error(File[Id].Name,Error);
   Strset(File[Id].Name);
   return(ERROR);
  }
  else
  {
   return(OK);
  }
 }
 else 
 {  
  Sys_Error(File[Id].Name,E_ALREADYOPEN);
  return(ERROR);
 }
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
