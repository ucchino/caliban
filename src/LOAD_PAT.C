/*PPU************************************************************************\
*
*    $Source: load_pat.c
*  $Revision: 0.7.2
*      $Date: Thu Jul  8 19:30:58 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "load_pat.h"

/****************************************************************************/

_Pattern_Block* Pattern=NULL;

/****************************************************************************/

# define OVER 44

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Pattern                                                  */
/* Param. : char*                                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_Pattern(CHAR* File_Name)
{
 ULONG      Last=NONE;
 ULONG      Read=NONE;
 CHAR*      Pattern_Read=NULL;
 PATTERN_ID Pattern_Index1=NONE;
 PATTERN_ID Pattern_Index2=NONE;
 
 if(NetList==NULL){Print(ERROR_COLOR,"Before Load A TEST PATTERN You MUST Load A Circuit !");return(ERROR);}

 if(Fopen(File_Name,INP,INP,OPEN_R,E_OPEN)==ERROR) return(ERROR);

 Release_Pattern();
 Phase("Loading Test Pattern");

 Alloc(Pattern_Read,(Circuit.Num_PI+OVER),CHAR,Read);
 
 for(Circuit.Num_Patterns=NONE;fgets(Pattern_Read,(Circuit.Num_PI+OVER),File[INP].Ptr);)
 {
  Read=strlen(Pattern_Read)-1;

  if(Read <= NONE)
  {
   FClose(GEN);
   PrintSL(ERROR_COLOR,"Invalid Line Number:",Circuit.Num_Patterns);
   Release_Pattern();
   Sys_Error("",E_PATTERN_EMPTY);
   continue;
  }

  if((Read!=Circuit.Num_PI) && (Pattern_Read[0]!=RESET_CHAR))
  {
   PrintSL(ERROR_COLOR,"Circuit PI          :",Circuit.Num_PI);
   Print2(ERROR_COLOR ,"Pattern String As   :",Pattern_Read);
   PrintSL(ERROR_COLOR,"Found Pattern Lenght:",Read);
   Release_Pattern();
   FClose(INP);
   Sys_Error("",E_PATTERN_LENGHT);
   return(ERROR);
  }

  for(Pattern_Index1=NONE;Pattern_Index1 < Circuit.Num_PI;Pattern_Index1++)
  {
   if((Pattern_Read[Pattern_Index1]!=ZERO_CHAR) && 
      (Pattern_Read[Pattern_Index1]!=ONE_CHAR)  && 
      (Pattern_Read[0]             !=RESET_CHAR)&& 
      (Pattern_Read[Pattern_Index1]!=ICS0_CHAR) && 
      (Pattern_Read[Pattern_Index1]!=ICS1_CHAR) && 
      (Pattern_Read[Pattern_Index1]!=ICS2_CHAR) && 
      (Pattern_Read[Pattern_Index1]!=ICS3_CHAR))
   {
    PrintSC(ERROR_COLOR,"Found Pattern CELL:",Pattern_Read[Pattern_Index1]);
    PrintSL(ERROR_COLOR,"Position          :",Pattern_Index1);
    Print2(ERROR_COLOR, "Invalid Pattern   :",Pattern_Read);
    Release_Pattern();
    FClose(INP);
    Sys_Error("",E_PATTERN_VALUE);
    return(ERROR);
   }
  }

  if(Pattern_Read[0]==RESET_CHAR)
  {
   if(Last==NONE)
   {
    PrintV2(Normal,"Valid Pattern:",Pattern_Read);
    Last=++Circuit.Num_Patterns;
   }
  }
  else
  {
   PrintV2(Normal,"Valid Pattern:",Pattern_Read);
   ++Circuit.Num_Patterns;
   Last=NONE;
  }
 }

 PrintSL(MESSAGE_COLOR,"Found Patterns      :",Circuit.Num_Patterns);

 /** AND NOW LOAD **/

 Rewind(INP);
 PrintSL(MESSAGE_COLOR,"Found Primary Inputs:",Circuit.Num_PI);
 PrintSL(MESSAGE_COLOR,"Found Patterns      :",Circuit.Num_Patterns);
 
 Alloc(Pattern,Circuit.Num_Patterns,_Pattern_Block,Circuit.Mem_Pattern);

 Last=NONE;

 for(Pattern_Index1=NONE;Pattern_Index1 < Circuit.Num_Patterns;)
 {
  Strset(Pattern_Read);
  
  fgets(Pattern_Read,Circuit.Num_PI+OVER,File[INP].Ptr);

  Read=strlen(Pattern_Read)-1;

  if(Read > NONE)
  {
   if(Pattern_Read[0]==RESET_CHAR)
   {
    if(Last==NONE)
    {
     PrintVSL(MESSAGE_COLOR,"Pattern Id:",Pattern_Index1);
     PrintV2( MESSAGE_COLOR,"Pattern   :",Pattern_Read);

     Alloc(Pattern[Pattern_Index1],Circuit.Num_PI,_Pattern,Circuit.Mem_Pattern);

     for(Pattern_Index2=NONE;Pattern_Index2 < Circuit.Num_PI;Pattern_Index2++)
     {
      Pattern[Pattern_Index1][Pattern_Index2]=PATTERN_RESET;
     }
     PrintV(MESSAGE_COLOR,"As RESET Pattern");

     Pattern_Index1++;
     HBarV(Normal);

     Last=OVER;
    }
   }
   else
   {
    PrintVSL(MESSAGE_COLOR,"Pattern Id:",Pattern_Index1);
    PrintV2( MESSAGE_COLOR,"Pattern   :",Pattern_Read);

    Alloc(Pattern[Pattern_Index1],Circuit.Num_PI,_Pattern,Circuit.Mem_Pattern);

    for(Pattern_Index2=NONE;Pattern_Index2 < Circuit.Num_PI;Pattern_Index2++)
    {
          if(Pattern_Read[Pattern_Index2]==ONE_CHAR) 
          {
           Pattern[Pattern_Index1][Pattern_Index2]=PATTERN_ONE;
           PrintVC(MESSAGE_COLOR,'1');
          }
     else if(Pattern_Read[Pattern_Index2]==ZERO_CHAR) 
          {
           Pattern[Pattern_Index1][Pattern_Index2]=PATTERN_ZERO;
           PrintVC(MESSAGE_COLOR,'0');
          }
     else
     {
      if((Pattern_Read[Pattern_Index2]==ICS0_CHAR) || 
         (Pattern_Read[Pattern_Index2]==ICS1_CHAR) || 
         (Pattern_Read[Pattern_Index2]==ICS2_CHAR) || 
         (Pattern_Read[Pattern_Index2]==ICS3_CHAR))
      {
       Pattern[Pattern_Index1][Pattern_Index2]=PATTERN_ICS;
       PrintVC(MESSAGE_COLOR,ICS0_CHAR);
      }
     }
    }
    VNL;
    Pattern_Index1++;
    HBarV(Normal);
    Last=NONE;
   }
  }
 }

 Free(Pattern_Read,DB_ROOT);
 
 strcpy(Circuit.Pat_Name,File[INP].Name);
 Log("Loaded Pattern",Circuit.Pat_Name);

 Print_Mem_Used(Pattern,Circuit.Mem_Pattern);

 Circuit.Num_Used_Patterns=NONE;

 Bar(BAR_COLOR);
 FClose(INP);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_Pattern                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_Pattern(void)
{
 PATTERN_ID Id=NONE;
 
 Circuit.Mem_Pattern =NONE; 

 if(Pattern==NULL) return; 

 for(Id=NONE;Id < Circuit.Num_Patterns;Id++)
 {
  Free(Pattern[Id],Id);
 }
 Free(Pattern,DB_ROOT);
 Circuit.Num_Patterns=NONE;
 Circuit.Num_Used_Patterns=NONE;
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
