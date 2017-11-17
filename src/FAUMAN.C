/*PPU************************************************************************\
*
*    $Source: fauman.c
*  $Revision: 0.7.7
*      $Date: Thu Jul  8 19:30:47 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "fauman.h"

/****************************************************************************/

ULONG Rate=NONE;
INT   Colla=NONE;

/****************************************************************************/
/*                                                                          */
/* Fx Name: Fauman                                                          */
/* Param. : char*,char*,char*,char*                                         */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Fauman(CHAR* Collapsed,CHAR* Fault_Model,CHAR* Sorting,CHAR* File_Name)
{
 INT  FSort=NONE;
 INT  Model=NONE;
 CHAR getting[STRING_LEN];

 if(NetList==NULL){Print(ERROR_COLOR,"Before Creating A FaultList You MUST Load A Circuit !");return(ERROR);}

 if(Fopen(File_Name,NEW,FAU,CREATE_W,E_CREATE)==ERROR) return(ERROR);

 if(Command_Line_In_Use==FALSE)
 {
  Strset(getting);
  while((Model < Model_1) || (Model > Model_4))
  {
   Print(MESSAGE_COLOR,"Choose A Fault Model:");
   Print2(Normal,"- Stuck On 1=>",MModel_1);
   Print2(Normal,"- Stuck On 2=>",MModel_2);
   Print2(Normal,"- Stuck On 3=>",MModel_3);
   Print2(Normal,"- Exit NOW 4=>",MModel_4);
   Print(Normal,"\nChoose:>");
   scanf("%s",getting);
   Model=atoi(getting);
   if(Model==Model_4) return(OK);
  }
  
  Strset(getting);
  while((FSort < Sort_1) || (FSort > Sort_3))
  {
   Print(MESSAGE_COLOR,"Choose A Sorting Method For Faults:");
   Print2(Normal,"- Sorted By 1=>",MSort_1);
   Print2(Normal,"- Sorted By 2=>",MSort_2);
   Print2(Normal,"- Sorted By 3=>",MSort_3);
   Print(Normal,"\nChoose:>");
   scanf("%s",getting);
   FSort=atoi(getting);
  }

  Strset(getting);
  while((Colla < Collapse_1) || (Colla > Collapse_2))
  {
   Print(MESSAGE_COLOR,"Choose If Collapse Fault List:");
   Print2(Normal,"-=>",MCollapse_1);
   Print2(Normal,"-=>",MCollapse_2);
   Print(Normal,"\nChoose:>");
   scanf("%s",getting);
   Colla=atoi(getting);
  }

 }
 else
 {
  Model=atoi(Fault_Model);
  if((Model < Model_1) || (Model > Model_3)) 
  {
   PrintSL(ERROR_COLOR,"Unknown Stk@ Model For Fault Manager:",Model);
   return(ERROR);
  }
  
  Colla=atoi(Collapsed);
  if((Colla < Collapse_1) || (Colla > Collapse_2)) 
  {
   PrintSL(ERROR_COLOR,"Unknown Collapsing Option For Fault Manager:",Colla);
   return(ERROR);
  }

  FSort=atoi(Sorting);
  if((FSort < Sort_1) || (FSort > Sort_3)) 
  {
   PrintSL(ERROR_COLOR,"Wrong Sort Option For Fault Manager",FSort);
   Print(ERROR_COLOR,  "KEEPING UNSORTED...");
   return(ERROR);
  }
 }
 
 Rate=NONE;

 if(Colla==Collapse_2)
 {
  Sys_Error("Fauman Collapsed Fault List",E_NOT_AVAIB);
  return(ERROR);
 }

 if(Create_Group_Fault(Model,FSort)==ERROR)
 {
  FClose(NEW);
  Log("Failed To Create FaultList To",File[NEW].Name);
  return(ERROR);
 }
 else
 {
  Log("Created FaultList To",File[NEW].Name);
 }
 FClose(NEW);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Group_Fault                                              */
/* Param. : int,int                                                         */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_Group_Fault(INT Fault_Model,INT Sorting)
{
 Phase("Creating The FaultList");NL;

 FPrint(NEW,"2.0A");
 
 if(Sorting==Sort_1) return(Sort_By_Gate(Fault_Model));
 else
 if(Sorting==Sort_2) return(Sort_By_Type(Fault_Model));
 else
 if(Sorting==Sort_3) return(Sort_By_Level(Fault_Model));

 Bar(BAR_COLOR);
 return(ERROR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Sort_By_Gate                                                    */
/* Param. : int                                                             */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Sort_By_Gate(INT Model)
{
 GATE_ID Id=NONE;
 
 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if(Create_Single_Gate_Fault(Model,Id)==ERROR) return(ERROR);
 }  
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Sort_By_Type                                                    */
/* Param. : int                                                             */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Sort_By_Type(INT Model)
{
 GATE_TYPE Gate_Type=NONE;
 GATE_ID   Id=NONE;
 
 for(Gate_Type=NONE;Gate_Type < GATE_TYPES;Gate_Type++)
 {
  for(Id=NONE;Id < Circuit.Num_Descr;Id++)
  {
   if((NetList[Id].Type&GATE_MASK)==Gate_Type)
   {
    if(Create_Single_Gate_Fault(Model,Id)==ERROR) return(ERROR);
   }
  }
 }
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Sort_By_Level                                                   */
/* Param. : int                                                             */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Sort_By_Level(INT Model)
{
 GATE_ID    Id=NONE;
 GATE_LEVEL Level=NONE;
 
 for(Level=NONE;Level < Circuit.Num_Levels;Level++)
 {
  for(Id=NONE;Id < Circuit.Num_Descr;Id++)
  {
   if(NetList[Id].Level==Level)
   {
    if(Create_Single_Gate_Fault(Model,Id)==ERROR) return(ERROR);
   }
  }
 }  
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Single_Gate_Fault                                        */
/* Param. : int,unsigned long                                               */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_Single_Gate_Fault(INT Model,GATE_ID Id)
{
 GATE_ID  Descr=NONE;
 GATE_FAN Fanout=NONE;

 if((Model==Model_1) || (Model==Model_3))
 {
  if(((NetList[Id].Type&GATE_MASK)!=PO) && ((NetList[Id].Type&GATE_MASK)!=CLOCK))
  {
   Rate++;
   Rate++;
   FPrintFault(NEW,128,Id,-1,1); /* OUT */
   FPrintFault(NEW,000,Id,-1,1); /* OUT */
  }
 }
 
 if((Model==Model_2) || (Model==Model_3))
 {
  for(Descr=NONE;Descr < Circuit.Num_Descr;Descr++)
  {
   if(((NetList[Descr].Type&GATE_MASK)!=PO) && ((NetList[Descr].Type&GATE_MASK)!=CLOCK))
   {
    for(Fanout=NONE;Fanout < NetList[Descr].Fanout;Fanout++)
    {
     if(NetList[Descr].To_Gate[Fanout]==&NetList[Id])
     {
      Rate++;
      Rate++;
      FPrintFault(NEW,128,Id,Descr,1);
      FPrintFault(NEW,000,Id,Descr,1);
     }
    }
   }
  }
 }

 if(Rate > MAX_FAULTS)
 {
  PrintSL(ERROR_COLOR,"Too Many Faults To Create (see Limits Before):",Rate);
  return(ERROR);
 }

 PrintOSL(MESSAGE_COLOR,"Created Faults:",Rate);

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
