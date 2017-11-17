/*PPU************************************************************************\
*
*    $Source: load_fau.c
*  $Revision: 0.7.7
*      $Date: Thu Jul  8 19:30:57 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "sim.h"
# include "load_fau.h"

/****************************************************************************/

 Fault_Ptr*  Fault_List=NULL;
_Fault_Des*  Fault_Descriptor=NULL;

/****************************************************************************/
/*                                                                          */
/* Fx Name: Clear_Fault_List                                                */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Clear_Fault_List(void)
{
 FF_ID    Flp=NONE;
 FAULT_ID Fault=NONE;

 Phase("Clearing FaultList");

 for(Fault=NONE;Fault < Circuit.Num_Faults;Fault++)
 {
  Fault_Descriptor[Fault].Num_Enqueued_FFs=NONE;
  Fault_Descriptor[Fault].Pattern_Id      =UNKNOWN;

  for(Flp=NONE;Flp < ENQUEUABLE_FFS;Flp++)
  {
   Fault_Descriptor[Fault].FFs[Flp].FF_Gate =NULL;
   Fault_Descriptor[Fault].FFs[Flp].FF_Value=FF_ICS;
  }

  Fault_List[Fault]=&(Fault_Descriptor[Fault]);
 }
 
 Fault_List[Circuit.Num_Faults]=NULL;
 
 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_FAult_List                                               */
/* Param. : char*                                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_Fault_List(CHAR* File_Name)
{
 CHAR Connected=TRUE;

 FAULT_ID Fault=NONE;
 GATE_FAN Fanout=NONE;

 ULONG    n1,n2,n4;
 LONG     n3;

 CHAR String[STRING_LEN];

 if(NetList==NULL) {Print(ERROR_COLOR,"Before Load A FaultList You MUST Load A Circuit !");return(ERROR);}

 if(Fopen(File_Name,FAU,FAU,OPEN_R,E_OPEN)==ERROR) return(ERROR);
 Release_Fault_List();

 Phase("Loading FaultList");

 fgets(String,STRING_LEN,File[FAU].Ptr);

 if(NCmpN("2.0A",String,4))
 {
  FClose(FAU);
  Sys_Error("",E_FAU_VER);
  return(ERROR);
 }

 Circuit.Num_Faults    =NONE;
 Circuit.Mem_Fault_List=NONE;
 Circuit.Mem_Faults_FF =NONE;

 while(fgets(String,STRING_LEN,File[FAU].Ptr)) ++Circuit.Num_Faults;

 PrintSL(MESSAGE_COLOR,"Found Faults:",Circuit.Num_Faults);
 
 if(Circuit.Num_Faults==NONE)
 {
  FClose(FAU);
  Sys_Error("",E_NO_FAULTS);
  return(ERROR);
 }

 Alloc(Fault_Descriptor,Circuit.Num_Faults,_Fault_Des,Circuit.Mem_Fault_List);

 Print_Mem_Used(Fault_Descriptor,Circuit.Mem_Fault_List);

 Rewind(FAU);

 fgets(String,STRING_LEN,File[FAU].Ptr);
 if(NCmpN("2.0A",String,4))
 {
  FClose(FAU);
  Release_Fault_List();
  Sys_Error("",E_FAU_VER);
  return(ERROR);
 }

 for(Fault=NONE;Fault < Circuit.Num_Faults;Fault++)
 {
  Fault_Descriptor[Fault].Pin             =NULL;
  Fault_Descriptor[Fault].Stk_Value       =UNKNOWN;
  Fault_Descriptor[Fault].Faulty_Gate     =NULL;
  Fault_Descriptor[Fault].Num_Enqueued_FFs=NONE;
  Fault_Descriptor[Fault].Pattern_Id      =UNKNOWN;

  Alloc(Fault_Descriptor[Fault].FFs,(ENQUEUABLE_FFS),_FF_List,Circuit.Mem_Faults_FF);
 }

 Print_Mem_Used(Fault_Descriptor[Fault].FFs,Circuit.Mem_Faults_FF);

 for(Fault=NONE;fgets(String,STRING_LEN,File[FAU].Ptr);)
 {
  sscanf(String,"%lu %lu %ld %lu",&n1,&n2,&n3,&n4);

  /********************************************\
  *                                            *
  *   N1 = Stk@                                *
  *   N2 = Gate Guasto                         *
  *   N3 = Gate Guastante Se Ingresso Di N2    *
  *   N4 = Size of Corresp. UNCOLLAPSED Faults *
  *        fauls covered by fault#k=n4         *
  *                                            *
  \********************************************/

  if(n3==-1)
  {
   Fault_Descriptor[Fault].Pin=NULL;
   PrintV2SL(MESSAGE_COLOR,"Loaded Out Fault:",Fault,"For Gate:",n2);
  }
  else
  {
   Connected=FALSE;
   for(Fanout=NONE;Fanout < NetList[n3].Fanout;Fanout++)
   {
    if(NetList[n3].To_Gate[Fanout]==(&NetList[n2]))
    {
     Connected=TRUE;

     Fault_Descriptor[Fault].Pin=(NetList[n3].To_Pin[Fanout]);

     PrintV4SL(MESSAGE_COLOR,"Fault# :",Fault,"Faulty_Gate:",n2,"Faulter_Gate:",n3,"With Pin:",Fanout);

     if(NetList[n2].Type==PO)
     {
      PrintSL(ERROR_COLOR,"Faulty Gate Desc Is:",n2);
      PrintSL(ERROR_COLOR,"Father Is          :",n3);
      Release_Fault_List();
      FClose(FAU);
      Sys_Error("",E_FAU_ON_PO);
      return(ERROR);
     }
     break;
    }
   }

   if(Connected==FALSE)
   {
    PrintSL(ERROR_COLOR,"Faulty Gate Desc Is:",n2);
    PrintSL(ERROR_COLOR,"Father Is          :",n3);
    Release_Fault_List();
    FClose(FAU);
    Sys_Error("",E_FAU_CONNECTED);
    return(ERROR);
   }
  }

  n1=((n1 > 127) ? STUCK1 : STUCK0);
  Fault_Descriptor[Fault].Stk_Value  =n1;
  Fault_Descriptor[Fault].Faulty_Gate=(&NetList[n2]);

  if(Fault_Descriptor[Fault].Stk_Value==STUCK1) Circuit.Num_Stk1++;
  else                                          Circuit.Num_Stk0++;
  Fault++;
 }

 strcpy(Circuit.Fau_Name,File[FAU].Name);
 Log("Loaded FaultList",Circuit.Fau_Name);

 Phase("Updating FaultList");

 UpDate_FList();

 PrintSL(MESSAGE_COLOR,"Found Faults At Last Scan:",Circuit.Num_Faults);
 
 Bar(BAR_COLOR);
 FClose(FAU);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Update_FList                                                    */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void UpDate_FList(void)
{
 FAULT_ID Fault=NONE;

 Alloc(Fault_List,(Circuit.Num_Faults+1),Fault_Ptr,Circuit.Mem_Fault_List);

 Print_Mem_Used(Fault_List,Circuit.Mem_Fault_List);

 for(Fault=NONE;Fault < Circuit.Num_Faults;Fault++)
 {
  Fault_List[Fault]=&(Fault_Descriptor[Fault]);
 }
 Fault_List[Circuit.Num_Faults]=NULL;
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_Fault_List                                              */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_Fault_List(void)
{
 FAULT_ID Num_Fau=NONE;
 
 if(Fault_List != NULL)
 {
  for(Num_Fau=NONE;Num_Fau < Circuit.Num_Faults;Num_Fau++)
  {
   Free(Fault_Descriptor[Num_Fau].FFs,DB_ROOT);
  }
 }

 Free(Fault_List,DB_ROOT);
 Free(Fault_Descriptor,DB_ROOT);

 Circuit.Found_Faults  =NONE;
 Circuit.Num_Stk0      =NONE;
 Circuit.Num_Stk1      =NONE;
 Circuit.Mem_Fault_List=NONE;
 Circuit.Mem_Faults_FF =NONE;
 Circuit.Num_Faults    =1;

 strcpy(Circuit.Sort_Mode,"None");
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
