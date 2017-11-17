/*PPU************************************************************************\
*
*    $Source: gengis.c
*  $Revision: 0.5.6
*      $Date: Thu Jul  8 19:30:51 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "sim.h"
# include "gengis.h"
# include "load_fau.h"
# include "load_pat.h"

/****************************************************************************/

# define DEFAULT_COVERAGE       90.0
# define DEFAULT_LENGTH         (Circuit.Num_Levels)
# define MAX_COVERAGE           100.0

/****************************************************************************/

ULONG index1=NONE;
ULONG Wanted_Cycles=NONE;
ULONG Wanted_Length=NONE;
FLOAT Wanted_Coverage=NONE;
FLOAT Current_Coverage=NONE;

/****************************************************************************/

_Pattern_Block* Pattern_Trans=NULL;

/****************************************************************************/
/*                                                                          */
/* Fx Name: Gengis                                                          */
/* Param. : char*,char*,char*                                               */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Gengis(CHAR* File_Name,CHAR* Min_Coverage,CHAR* Max_Length,CHAR* Times)
{
 Log("Atpg Phase", "ATPG Building Phase... (PPU Self-Made)");

 if(NetList   ==NULL){Print(ERROR_COLOR,"Before ATPG PHASE You MUST Load A Circuit !");  return(ERROR);}
 if(Fault_List==NULL){Print(ERROR_COLOR,"Before ATPG PHASE You MUST Load A FaultList !");return(ERROR);}

 Phase("Creating .INP Test Pattern");
 NL;

 Release_Pattern();

 if(Command_Line_In_Use==FALSE)
 {
  Wanted_Cycles=NONE;
  Wanted_Length=NONE;
  Wanted_Coverage=NONE;

  while(Wanted_Coverage==NONE)
  {
   Print(Normal,"Choose Minimal Coverage [30%-100%]:>");
   scanf("%f",&Wanted_Coverage);
  }

  while(Wanted_Length==NONE)
  {
   Print(Normal,"Choose Maximal Pattern Length :>");
   scanf("%lu",&Wanted_Length);
  }

  while(Wanted_Cycles==NONE)
  {
   Print(Normal,"Choose Minimal Try-Times :>");
   scanf("%lu",&Wanted_Cycles);
  }

 }
 else
 {
  Wanted_Cycles=atoi(Times);
  Wanted_Length=atoi(Max_Length);
  Wanted_Coverage=atof(Min_Coverage);
 }

 if(Wanted_Coverage < DEFAULT_COVERAGE)
 {
  Wanted_Coverage=DEFAULT_COVERAGE;
  PrintSL(Normal,"Resampling This Pessimistic Prevision To:",DEFAULT_COVERAGE);
 }

 if(Wanted_Length <= 1)
 {
  Wanted_Length=DEFAULT_LENGTH;
  PrintSL(Normal,"Resampling This Pessimistic Prevision To:",DEFAULT_LENGTH);
 }

 if(Wanted_Coverage > MAX_COVERAGE)
 {
  Wanted_Coverage=MAX_COVERAGE;
  PrintSL(Normal,"Resampling This TOOOO Optimistic Prevision To:",MAX_COVERAGE);
 }

 Phase("ATPG Creation Phase... (PPU Self-Made)");

 Current_Coverage=NONE;
 Circuit.Num_Patterns=Wanted_Length;

 Alloc_Random_Pattern();

 unlink(File_Name);
 
 /**********************************************************************/
 /**********************************************************************/
 /**********************************************************************/
 /**********************************************************************/
 /**********************************************************************/
 /**********************************************************************/

 for(index1=NONE;index1 < Wanted_Cycles;index1++)
 {
  Circuit.Coverage=NONE;

  Create_Random_Pattern();

  Sim();

  Clear_Screen();

  PrintSLSD(MESSAGE_COLOR,"Pattern Id:",index1+1,"Best Coverage [%]:",Current_Coverage);
  PrintSLSD(MESSAGE_COLOR,"Pattern Id:",index1+1,"Last Coverage [%]:",Circuit.Coverage);

  if(Circuit.Coverage > Current_Coverage)
  {
   Current_Coverage=Circuit.Coverage;
   if(Save_Pattern(File_Name)==ERROR) return(ERROR);
  }

  if(Current_Coverage >= Wanted_Coverage) break;
 }

 Release_Pattern();
 
 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Alloc_Random_Pattern                                            */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Alloc_Random_Pattern(void)
{
 ULONG index2=NONE;

 rnd_init();

 Alloc(Pattern,Wanted_Length,_Pattern_Block,Circuit.Mem_Pattern);

 for(index2=NONE;index2 < Wanted_Length;index2++)
 {
  Alloc(Pattern[index2],Circuit.Num_PI,_Pattern,Circuit.Mem_Pattern);
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Random_Pattern                                           */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Create_Random_Pattern(void)
{
 UINT  Val=NONE;

 ULONG index1=NONE;
 ULONG index2=NONE;

 rnd_init();

 Pattern[0][0]=PATTERN_RESET;

 for(index2=1;index2 < Wanted_Length;index2++)
 {
  for(index1=NONE;index1 < Circuit.Num_PI;index1++)
  {
   Val=rnd_gen(2);

   if(Val==0) Pattern[index2][index1]=PATTERN_ZERO;
   if(Val==1) Pattern[index2][index1]=PATTERN_ONE;
  }
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Save_Pattern                                                    */
/* Param. : char*                                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Save_Pattern(CHAR* File_Name)
{
 ULONG   index1=NONE;
 GATE_ID PI_Index=NONE;

 if(Fopen(File_Name,NEW,INP,CREATE_W,E_CREATE)==ERROR) return(ERROR);

 for(index1=NONE;index1 < Wanted_Length;index1++)
 {
  for(PI_Index=NONE;PI_Index < Circuit.Num_PI;PI_Index++)
  {                                             
        if(Pattern[index1][PI_Index]==PATTERN_ONE)   {FPrintC(NEW,ONE_CHAR);}
   else if(Pattern[index1][PI_Index]==PATTERN_ZERO)  {FPrintC(NEW,ZERO_CHAR);}
   else if(Pattern[index1][PI_Index]==PATTERN_RESET) {FPrintC(NEW,RESET_CHAR);break;}
   else {Print(Normal,"GENGIS Cannot Handle X Values On TestPattern");Break(ERROR);}
  }
  FPrint(NEW,"");
 }

 FClose(NEW);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Append_Pattern                                                  */
/* Param. : char*                                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Append_Pattern(CHAR* File_Name)
{
 ULONG   index1=NONE;
 GATE_ID PI_Index=NONE;

 if(Fopen(File_Name,NEW,INP,APPEND_W,E_APPEND)==ERROR) return(ERROR);

 for(index1=NONE;index1 < Wanted_Length;index1++)
 {
  for(PI_Index=NONE;PI_Index < Circuit.Num_PI;PI_Index++)
  {                                             
        if(Pattern[index1][PI_Index]==PATTERN_ONE)   {FPrintC(NEW,ONE_CHAR);}
   else if(Pattern[index1][PI_Index]==PATTERN_ZERO)  {FPrintC(NEW,ZERO_CHAR);}
   else if(Pattern[index1][PI_Index]==PATTERN_RESET) {FPrintC(NEW,RESET_CHAR);break;}
   else {Print(Normal,"GENGIS Cannot Handle X Values On TestPattern");Break(ERROR);}
  }
  FPrint(NEW,"");
 }

 FClose(NEW);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Gengis_Multiple_Resets                                          */
/* Param. : char*,char*,char*,char*                                         */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Gengis_Multiple_Resets(CHAR* File_Name,CHAR* Min_Coverage,CHAR* Max_Length,CHAR* Times,CHAR* Resets)
{
 Sys_Error("Gengis_Multiple_Resets",E_NOT_AVAIB);
 return(ERROR);

 Log("Multiple Reset Atpg Phase", "ATPG Building Phase... (PPU Self-Made)");

 if(NetList   ==NULL){Print(ERROR_COLOR,"Before ATPG PHASE You MUST Load A Circuit !");  return(ERROR);}
 if(Fault_List==NULL){Print(ERROR_COLOR,"Before ATPG PHASE You MUST Load A FaultList !");return(ERROR);}

 Phase("Creating .INP Test Pattern");
 NL;

 /*****************************/
 Release_Pattern();
 Pattern=Pattern_Trans;
 Release_Pattern();
 Pattern_Trans=NULL;
 /*****************************/

 if(Command_Line_In_Use==FALSE)
 {
  Wanted_Cycles=NONE;
  Wanted_Length=NONE;
  Wanted_Coverage=NONE;

  while(Wanted_Coverage==NONE)
  {
   Print(Normal,"Choose Minimal Coverage [30%-100%]:>");
   scanf("%f",&Wanted_Coverage);
  }

  while(Wanted_Length==NONE)
  {
   Print(Normal,"Choose Maximal Pattern Length :>");
   scanf("%lu",&Wanted_Length);
  }

  while(Wanted_Cycles==NONE)
  {
   Print(Normal,"Choose Minimal Try-Times :>");
   scanf("%lu",&Wanted_Cycles);
  }

 }
 else
 {
  Wanted_Cycles=atoi(Times);
  Wanted_Length=atoi(Max_Length);
  Wanted_Coverage=atof(Min_Coverage);
 }

 if(Wanted_Coverage < DEFAULT_COVERAGE)
 {
  Wanted_Coverage=DEFAULT_COVERAGE;
  PrintSL(Normal,"Resampling This Pessimistic Prevision To:",DEFAULT_COVERAGE);
 }

 if(Wanted_Length <= 1)
 {
  Wanted_Length=DEFAULT_LENGTH;
  PrintSL(Normal,"Resampling This Pessimistic Prevision To:",DEFAULT_LENGTH);
 }

 if(Wanted_Coverage > MAX_COVERAGE)
 {
  Wanted_Coverage=MAX_COVERAGE;
  PrintSL(Normal,"Resampling This TOOOO Optimistic Prevision To:",MAX_COVERAGE);
 }

 Phase("ATPG Creation Phase... (PPU Self-Made)");

 Current_Coverage=NONE;
 Circuit.Num_Patterns=Wanted_Length;

 /*****************************/
 Alloc_Random_Pattern();
 Pattern_Trans=Pattern;
 Pattern=NULL;
 Alloc_Random_Pattern();
 /*****************************/

 unlink(File_Name);
 
 for(index1=NONE;index1 < Wanted_Cycles;index1++)
 {
  
  
  /**

 for()
 {

  Rendo vero la variabile perche' al prossimo passo e solo allora serve---
  Multiple_Atpg_Mode=TRUE;
  
  
 }

 distruzione memoria...

 
 FX_RET Append_Pattern(CHAR* File_Name) 
 
 **/
  


  Circuit.Coverage=NONE;

  Create_Random_Pattern();

  Sim();

  Clear_Screen();

  PrintSLSD(MESSAGE_COLOR,"Pattern Id:",index1+1,"Best Coverage [%]:",Current_Coverage);
  PrintSLSD(MESSAGE_COLOR,"Pattern Id:",index1+1,"Last Coverage [%]:",Circuit.Coverage);

  if(Circuit.Coverage > Current_Coverage)
  {
   Current_Coverage=Circuit.Coverage;
   if(Save_Pattern(File_Name)==ERROR) return(ERROR);
  }

  if(Current_Coverage >= Wanted_Coverage) break;
 }

 /*****************************/
 Release_Pattern();
 Pattern=Pattern_Trans;
 Release_Pattern();
 Pattern_Trans=NULL;
 /*****************************/
 
 Bar(BAR_COLOR);
 return(OK);
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
