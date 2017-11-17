/*PPU************************************************************************\
*
*    $Source: load_cir.c
*  $Revision: 0.7.2
*      $Date: Thu Jul  8 19:30:55 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "sim.h"

# include "modes.h"
# include "load_cir.h"
# include "load_fau.h"
# include "load_pat.h"

/****************************************************************************/

_Circuit  Circuit;

_Gate*    NetList=NULL;
          
CELL*     FF_Init=NULL;

Gate_Ptr* FF_Vect=NULL;
Gate_Ptr* PI_Vect=NULL;

/****************************************************************************/

CHAR       FF_Chain=FALSE;
GATE_ID    Check_Gates=NONE;
GATE_LEVEL Check_Levels=NONE;

/****************************************************************************/
/*                                                                          */
/* Fx Name: Give_Gates_For_Levels                                           */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Give_Gates_For_Levels(void)
{
 GATE_LEVEL Level=NONE;

 if(Circuit.Gates_For_Level==NULL)
 {
  Print(ERROR_COLOR,"Before You MUST Load A Circuit !");
  return;
 }
 else
 {
  for(Level=NONE;Level < Circuit.Num_Levels;Level++)
  {
   Print2SL(MESSAGE_COLOR,"Level:",Level,"Number Of Gates:",Circuit.Gates_For_Level[Level]);
  }
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_PI                                                       */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_PI(void)
{
 GATE_ID Id=NONE;
 GATE_ID Ip=NONE;
 GATE_ID PI_index=NONE;

 Phase("Creating PI Data Structures");
 
 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if(NetList[Id].Type==PI) 
  {
   if(NetList[Id].Fanin!=NONE) 
   {
    Gate_Error(Id,E_FAN_PI);
    return(ERROR);
   }
   
   for(Ip=NONE;Ip < NetList[Id].Fanout;Ip++)
   {
    if(NetList[Id].To_Gate[Ip]->Type==PI) 
    {
     Gate_Error(Id,E_OUT_TO_A_PI);
     return(ERROR);
    }
   }
   
   if(NetList[Id].Level!=NONE) 
   {
    Gate_Error(Id,E_LEVEL_PI);
    return(ERROR);
   }

   PI_index++;
  }
 }
 
 if(PI_index==NONE) 
 {
  Sys_Error("",E_NO_PI);
  return(ERROR);
 }
 
 if(PI_index!=Circuit.Num_PI) 
 {
  PrintSL(ERROR_COLOR,"Given Number Of PI:",Circuit.Num_PI);
  PrintSL(ERROR_COLOR,"Calculated        :",PI_index);
  Sys_Error("",E_PI_PI);
  return(ERROR);
 }
 
 Alloc(PI_Vect,Circuit.Num_PI,Gate_Ptr,Circuit.Mem_PI);

 for(Id=NONE,PI_index=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if(NetList[Id].Type==PI)
  {
   PI_Vect[PI_index]=&NetList[Id];
   PI_index++;
  }
 }

 Print(MESSAGE_COLOR,"PI Are OK!");
 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_FF                                                       */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_FF(void)
{
 GATE_ID Id=NONE;
 GATE_ID FF_index=NONE;
 
 Phase("Creating FF Data Structures");
 for(Id=NONE,FF_index=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if(NetList[Id].Type==FF)
  {
   if(NetList[Id].Fanin > 1)
   {
    Gate_Error(Id,E_SINGLE_IN);
    return(ERROR);
   }
   
   if(NetList[Id].Level!=NONE)
   {
    PrintSL(ERROR_COLOR,"[Flip Flop]:",FF_index);
    PrintSL(ERROR_COLOR,"With Level :",NetList[Id].Level);
    Sys_Error("",E_FF_LEVEL);
    return(ERROR);
   }

   FF_index++;
  }
 }

 if(FF_index==NONE) 
 {
  Sys_Error("",E_COMB_CIR);
  return(OK);
 }
 
 if(FF_index!=Circuit.Num_FF)
 {
  PrintSL(ERROR_COLOR,"Given Number Of FF:",Circuit.Num_FF);
  PrintSL(ERROR_COLOR,"Calculated        :",FF_index);
  Sys_Error("",E_FF_FF);
  return(ERROR);
 }
 
 if(FF_index  > MAX_FF)
 {
  PrintSL(ERROR_COLOR,"Max Permitted Number Of FF:",MAX_FF);
  PrintSL(ERROR_COLOR,"Calculated                :",FF_index);
  Sys_Error("",E_NUM_FF);
  return(ERROR);
 }
 
 Alloc(FF_Init,Circuit.Num_FF,CELL,Circuit.Mem_FF);
 Alloc(FF_Vect,Circuit.Num_FF,Gate_Ptr,Circuit.Mem_FF);
 
 for(Id=NONE,FF_index=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if(NetList[Id].Type==FF)
  {
   Circuit.Max_FF_Fanout=Max(Circuit.Max_FF_Fanout,NetList[Id].Fanout);
   
   FF_Vect[FF_index]=&NetList[Id];
   FF_index++;
   if((NetList[Id].Fanout==1) && (NetList[Id].To_Gate[0]->Type==PO)) PrintSL(MESSAGE_COLOR,"Gate/FF Found On A PO:",Id);
  }
 }
 
 Print(MESSAGE_COLOR,"FF Are OK!");
 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Gates_For_Level                                          */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_Gates_For_Level(void)
{
 GATE_ID    Id=NONE;
 GATE_LEVEL Level=NONE;

 Phase("Creating Level-Event Data Structures");
 if(Circuit.Num_Levels==NONE)Sys_Error("",E_CIRCUIT_LEVELS);

 Alloc(Circuit.Gates_For_Level,Circuit.Num_Levels,GATE_ID,Circuit.Mem_Circuit);
 
 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  Circuit.Gates_For_Level[NetList[Id].Level]++;
 }

 PrintV(MESSAGE_COLOR,"Gates For Level Report Follows:");
 for(Level=NONE;Level < Circuit.Num_Levels;Level++)
 {
  if(Circuit.Gates_For_Level[Level]==NONE)
  {
   PrintSL(ERROR_COLOR,"Faulty Level Is:",Level);
   Sys_Error("",E_NO_GATES_ON_LEVEL);
   return(ERROR);
  }
  
  PrintV2SL(MESSAGE_COLOR,"Level:",Level,"Number Of Gates:",Circuit.Gates_For_Level[Level]);
 }

 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: File_InitFFs                                                    */
/* Param. : char*                                                           */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void File_Init_FFs(CHAR* File_Name)
{
 FF_ID FF_Id=NONE;
 CHAR  String[STRING_LEN];

 if(NetList==NULL) {Print(ERROR_COLOR,"Before Load A FF Init File You MUST Load A Circuit !");return;}

 if(Fopen(File_Name,FFI,FFI,OPEN_R,E_OPEN)==ERROR) return;
 
 Phase("Loading Init FF File");
 PrintSL(MESSAGE_COLOR,"FF In This Circuit:",Circuit.Num_FF);
 
 for(FF_Id=NONE;FF_Id < Circuit.Num_FF;FF_Id++)   
 {
  Strset(String);
  fscanf(File[FFI].Ptr,"%s",&String);

  if(feof(File[FFI].Ptr))
  {
   Print(ERROR_COLOR,"Raised Premature Eof During Settings Of FFs");
   Init_FFs(DEFAULT_FF_INIT_MODE);
   Bar(BAR_COLOR);
   return;
  }

  if(Cmp(String,ONE_STRING)) 
  {
   PrintV2SL(MESSAGE_COLOR,"Setting FF:",FF_Id,"To "ONE_STRING,1);
   Set_Cell(FF_Init[FF_Id]);
  }
  else
  if((Cmp(String,ICS0_STRING)) || (Cmp(String,ICS1_STRING)) || (Cmp(String,ICS2_STRING)) || (Cmp(String,ICS3_STRING)))
  {
   PrintV2SL(MESSAGE_COLOR,"Setting FF:",FF_Id,"To "ICS0_STRING,1);
   Ics_Cell(FF_Init[FF_Id]);
  }
  else
  if(Cmp(String,ZERO_STRING)) 
  {
   PrintV2SL(MESSAGE_COLOR,"Setting FF:",FF_Id,"To "ZERO_STRING,1);
   Clear_Cell(FF_Init[FF_Id]);
  }
  else
  {
   Sys_Error("",E_LOGIC_LEVEL);
   Init_FFs(DEFAULT_FF_INIT_MODE);
   Bar(BAR_COLOR);
   return;
  }
 }
 
 strcpy(FF_Init_Mode,USER_FF_INIT_MODE);

 Phase("Done Dear Boz...");
 Bar(BAR_COLOR);
 FClose(FFI);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Init_FFs                                                        */
/* Param. : char*                                                           */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Init_FFs(CHAR* Mode)
{
 CELL   Value;
 FF_ID  FF_Id=NONE;

 if(NetList==NULL) {Print(ERROR_COLOR,"Before Set FFs You MUST Load A Circuit !");return;}

 Phase("Setting FF Initial Value");

 strcpy(FF_Init_Mode,Mode);

 if(Cmp(Mode,ONE_STRING))   {Set_Cell(Value);}  else
 if(Cmp(Mode,ICS0_STRING))  {Ics_Cell(Value);}  else
 if(Cmp(Mode,ICS1_STRING))  {Ics_Cell(Value);}  else
 if(Cmp(Mode,ICS2_STRING))  {Ics_Cell(Value);}  else
 if(Cmp(Mode,ICS3_STRING))  {Ics_Cell(Value);}  else
 if(Cmp(Mode,ZERO_STRING))  {Clear_Cell(Value);}
 else
 {
  Sys_Error("",E_LOGIC_LEVEL);

  if(DEFAULT_FF_INIT_VALUE==FF_ONE)  Set_Cell(Value);
  if(DEFAULT_FF_INIT_VALUE==FF_ZERO) Clear_Cell(Value);
  if(DEFAULT_FF_INIT_VALUE==FF_ICS)  Ics_Cell(Value);

  strcpy(FF_Init_Mode,DEFAULT_FF_INIT_MODE);
  Print2(MESSAGE_COLOR,"Setting FF To Logic Value:",DEFAULT_FF_INIT_MODE);
 }
   
 Print2(MESSAGE_COLOR,"Setting FF To Logic Value:",FF_Init_Mode);
 for(FF_Id=NONE;FF_Id < Circuit.Num_FF;FF_Id++)
 {
  FF_Init[FF_Id]=Value;
 }

 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Clear_Circuit                                                   */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Clear_Circuit(void)
{
 GATE_ID  Id=NONE;
 GATE_FAN Fanin=NONE;
 FF_ID    FF_Id=NONE;

 Phase("Clearing Circuit");
 for(Id=NONE,FF_Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  NetList[Id].EnQueued=FALSE;

  NetList[Id].Type&=FAULT_2_GOOD_IN;
  NetList[Id].Type&=FAULT_2_GOOD_OUT;

  Inactive(NetList[Id].Out_Value);
  Ics_Cell(NetList[Id].Out_Value);

  if(NetList[Id].Type==PO)
  {
   Active(NetList[Id].Pin_Value[0]);
  }
  else
  {
   if(NetList[Id].Type!=FF)
   {
    for(Fanin=NONE;Fanin < NetList[Id].Fanin;Fanin++)
    {
     Inactive(NetList[Id].Pin_Value[Fanin]);
     Ics_Cell(NetList[Id].Pin_Value[Fanin]);
    }
   }
  }

  if(NetList[Id].Type==FF)
  {
   NetList[Id].Fanin=FF_Id;
   FF_Id++;
   Active(NetList[Id].Pin_Value[FF_IN]);
   Ics_Cell(NetList[Id].Pin_Value[FF_IN]);
  }
 }
 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_To_Pin_Links                                             */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_To_Pin_Links(void)
{
 GATE_ID  Id=NONE;
 GATE_FAN In=NONE;
 GATE_FAN Fanout=NONE;

 Phase("Linking Connections");
 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 for(Fanout=NONE;Fanout < NetList[Id].Fanout;Fanout++)
 {
  Clear_Cell(NetList[Id].To_Gate[Fanout]->Pin_Value[0]);
 }

 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  for(Fanout=NONE;Fanout < NetList[Id].Fanout;Fanout++)
  {   
   In=Get_Cell_Base(NetList[Id].To_Gate[Fanout]->Pin_Value[0]);
   NetList[Id].To_Pin[Fanout]=&(NetList[Id].To_Gate[Fanout]->Pin_Value[In]);
   
#ifdef DEBUG
   PrintV4SL(MESSAGE_COLOR,"Gate:",Id,"With Pin:",Fanout,"Point To Pin:",Get_Cell_Base(NetList[Id].To_Gate[Fanout]->Pin_Value[0]),"Of Gate:",NetList[Id].To_Gate[Fanout]->Id);
#else
   PrintV3SL(MESSAGE_COLOR,"Gate:",Id,"With Pin:",Fanout,"Point To Pin:",Get_Cell_Base(NetList[Id].To_Gate[Fanout]->Pin_Value[0]));
#endif   

   Inc_Cell(NetList[Id].To_Gate[Fanout]->Pin_Value[0]);
   
   if(NetList[Id].To_Gate[Fanout]->Type!=FF)
   if(Get_Cell_Base(NetList[Id].To_Gate[Fanout]->Pin_Value[0]) > NetList[Id].To_Gate[Fanout]->Fanin)
   {
    PrintSL(ERROR_COLOR,"Gate         :",Id);
    PrintSL(ERROR_COLOR,"Link Position:",Fanout);
    PrintSL(ERROR_COLOR,"Fanin        :",NetList[Id].To_Gate[Fanout]->Fanin);
    Gate_Error(Id,E_WIRED_OR);
    return(ERROR);
   }
  }
 }
 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Keep_Circuit                                                    */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Keep_Circuit(void)
{
 FClose(CAL);
 Release_Circuit();
 Sys_Error("",E_CIRCUIT);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Cal_Circuit                                              */
/* Param. : char*                                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_Cal_Circuit(CHAR* File_Name)
{
 ULONG Type  =NONE;
 ULONG Fanin =NONE;
 ULONG Fanout=NONE;
 ULONG Level =NONE;
 ULONG Id    =NONE;
 ULONG Fan   =NONE;
 ULONG Pos   =NONE;

 if(Fopen(File_Name,CAL,CAL,OPEN_R,E_OPEN)==ERROR) return(ERROR);

 Release_Circuit();
 Release_Pattern();
 Release_Fault_List();

 Phase("Loading .CAL Circuit");

 Load_Cal_Header(Visible);

 if(Circuit.Num_Gates > MAX_GATES)
 {
  PrintSL(ERROR_COLOR,"Gates Found:",Circuit.Num_Gates);
  FClose(CAL);
  Sys_Error("",E_HIGH_GATES);
  return(ERROR);
 }

 Alloc(NetList,Circuit.Num_Descr,_Gate,Circuit.Mem_Circuit);

 Check_Gates=NONE;
 Check_Levels=NONE;

 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  fscanf(File[CAL].Ptr,"%lu %*s %lu %lu %lu %lu",&Pos,&Type,&Level,&Fanin,&Fanout);

  if(Pos!=Id)
  {
   Bar(BAR_COLOR);
   PrintSL(ERROR_COLOR,"Gate Id Read =",Pos);
   PrintSL(ERROR_COLOR,"File Position=",Id);
   Release_Circuit();
   Sys_Error("",E_CAL_VER);
   return(ERROR);
  }

  if((Type!=PI) && (Type!=PO) && (Type!=CLOCK))
  {
   Check_Gates++;
  }

  if(Fanin > MAX_FAN)
  {
   Bar(BAR_COLOR);
   PrintSL(ERROR_COLOR,"Gate :",Id);
   PrintSL(ERROR_COLOR,"Fanin:",Fanin);
   Print2(ERROR_COLOR, "Type :",GType[(NetList[Id].Type&GATE_MASK)]);
   Release_Circuit();
   Sys_Error("",E_HIGH_FAN);
   return(ERROR);
  }

  if(Fanout > MAX_FAN)
  {
   Bar(BAR_COLOR);
   PrintSL(ERROR_COLOR,"Gate  :",Id);
   PrintSL(ERROR_COLOR,"Fanout:",Fanout);
   Print2(ERROR_COLOR, "Type  :",GType[(NetList[Id].Type&GATE_MASK)]);
   Release_Circuit();
   Sys_Error("",E_HIGH_FAN);
   return(ERROR);
  }

  Circuit.Ports[Type&GATE_MASK]++;
  Circuit.Max_Fanin =Max(Circuit.Max_Fanin ,Fanin);
  Circuit.Max_Fanout=Max(Circuit.Max_Fanout,Fanout);

  Check_Levels=Max(Check_Levels,Level+1);

  NetList[Id].Type      =(GATE_TYPE)Type;
  NetList[Id].Level     =(GATE_LEVEL)Level;

  NetList[Id].EnQueued  =FALSE;

  Ics_Cell(NetList[Id].Out_Value);

  NetList[Id].To_Gate   =NULL;
  NetList[Id].To_Pin    =NULL;

  NetList[Id].Pin_Value =NULL;

  NetList[Id].Fanin     =(GATE_FAN)Fanin;
  NetList[Id].Fanout    =(GATE_FAN)Fanout;

#ifdef DEBUG
  NetList[Id].Id        =(GATE_ID)Id;
#endif

  if(NetList[Id].Type==PI)
  {
   Alloc(NetList[Id].Pin_Value,1,CELL,Circuit.Mem_Circuit);
  }
  else
  if(NetList[Id].Type==FF)
  {
   Alloc(NetList[Id].Pin_Value,1,CELL,Circuit.Mem_Circuit);
  }
  else
  if(Fanin)
  {
   Alloc(NetList[Id].Pin_Value,NetList[Id].Fanin,CELL,Circuit.Mem_Circuit);
  }
  
  if(Fanout)
  {
   Alloc(NetList[Id].To_Gate,NetList[Id].Fanout,Gate_Ptr,Circuit.Mem_Circuit);
   Alloc(NetList[Id].To_Pin,NetList[Id].Fanout,CELL_Ptr,Circuit.Mem_Circuit);

   for(Fan=NONE;Fan < Fanout;Fan++)
   {
    fscanf(File[CAL].Ptr,"%*s %lu",&Pos);

    NetList[Id].To_Gate[Fan]=&NetList[Pos];
    NetList[Id].To_Pin[Fan]=NULL;

    PrintV2SL(MESSAGE_COLOR,"Gate:",Id,"Point To:",Pos);
   }
   HBarV(BAR_COLOR);
  }
 }

 if(Check_Circuit()==ERROR)           {Keep_Circuit();return(ERROR);}

 if(Create_PI()==ERROR)               {Keep_Circuit();return(ERROR);}
 if(Create_FF()==ERROR)               {Keep_Circuit();return(ERROR);}
 if(Create_PO()==ERROR)               {Keep_Circuit();return(ERROR);}
 if(Create_LV()==ERROR)               {Keep_Circuit();return(ERROR);}
 if(Create_Gates_For_Level()==ERROR)  {Keep_Circuit();return(ERROR);}
 if(Create_To_Pin_Links()==ERROR)     {Keep_Circuit();return(ERROR);}

 Print_Scan_Chain_FF();
 Clear_Circuit();
 
 Init_FFs(DEFAULT_FF_INIT_MODE); 
 
 /**************************************************************/
 /**                                                          **/
 /** Qui stabilisco che tipo di simulazione di default voglio.**/
 /** Assegno il significato del reset. In tal modo dico che il**/
 /** reset porta i ff a 0 ogni volta che trovo il simbolo #,  **/
 /** se volessi altrimenti porrei Init_FFs("x"); ossia il     **/
 /** reset assume il significato di assegnazione del valore x **/
 /** FERMO RESTANDO CHE QUESTO ACCADE SOLO DI DEFAULT, NON    **/
 /** SEMPRE...                                                **/
 /**                                                          **/
 /**************************************************************/
 
 Print_Mem_Used(FF_Vect,Circuit.Mem_FF);
 Print_Mem_Used(PI_Vect,Circuit.Mem_PI);
 Print_Mem_Used(NetList,Circuit.Mem_Circuit);
 
 strcpy(Circuit.Cir_Name,File[CAL].Name);
 Log("Loaded Circuit",Circuit.Cir_Name);
 
 FClose(CAL);
 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Load_Cal_Header                                                 */
/* Param. : char                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Load_Cal_Header(CHAR Mode)
{
 ULONG Pro=NONE;
 CHAR  String[STRING_LEN];

 if(Mode==Visible)
 {
  fscanf(File[CAL].Ptr,"%*s %s %*s",String);
  Print2(MESSAGE_COLOR,"Cal File Version:",String);
  fscanf(File[CAL].Ptr,"%*s %s",String);
  Print2(MESSAGE_COLOR,"Building Circuit From Netlist:",String);
  
  fscanf(File[CAL].Ptr,"%*s %lu",  &Pro);
  Circuit.Num_Levels=Pro;
  PrintSL(MESSAGE_COLOR,"Circuit Levels :",Circuit.Num_Levels);
  
  fscanf(File[CAL].Ptr,"%*s %lu",  &Pro);
  Circuit.Num_Gates=Pro;
  PrintSL(MESSAGE_COLOR,"Total Gates    :",Circuit.Num_Gates);
  
  fscanf(File[CAL].Ptr,"%*s %lu",  &Pro);
  Circuit.Num_FF=Pro;
  PrintSL(MESSAGE_COLOR,"FlipFlops      :",Circuit.Num_FF);
  
  fscanf(File[CAL].Ptr,"%*s %lu",  &Pro);
  Circuit.Num_PI=Pro;
  PrintSL(MESSAGE_COLOR,"Primary Inputs :",Circuit.Num_PI);
  
  fscanf(File[CAL].Ptr,"%*s %lu",  &Pro);
  Circuit.Num_PO=Pro;
  PrintSL(MESSAGE_COLOR,"Primary Outputs:",Circuit.Num_PO);
  
  fscanf(File[CAL].Ptr,"%*s %lu",  &Pro);
  Circuit.Num_Descr=Pro;
  PrintSL(MESSAGE_COLOR,"Descriptors    :",Circuit.Num_Descr);

  fscanf(File[CAL].Ptr,"%*s %*s %*s %*s %*s %*s %*s");
 }
 else
 {
  fscanf(File[CAL].Ptr,"%*s %s %*s",String);
  fscanf(File[CAL].Ptr,"%*s %s",String);
  fscanf(File[CAL].Ptr,"%*s %lu",&Pro); Circuit.Num_Levels=Pro;
  fscanf(File[CAL].Ptr,"%*s %lu",&Pro); Circuit.Num_Gates=Pro;
  fscanf(File[CAL].Ptr,"%*s %lu",&Pro); Circuit.Num_FF=Pro;
  fscanf(File[CAL].Ptr,"%*s %lu",&Pro); Circuit.Num_PI=Pro;
  fscanf(File[CAL].Ptr,"%*s %lu",&Pro); Circuit.Num_PO=Pro;
  fscanf(File[CAL].Ptr,"%*s %*s %*s %*s %*s %*s %*s");
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_FF                                                      */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_FF(void)
{
 Free(FF_Init,DB_ROOT);
 Free(FF_Vect,DB_ROOT);

 Circuit.Num_FF =NONE;
 Circuit.Mem_FF =NONE;
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_PI                                                      */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_PI(void)
{
 Free(PI_Vect,DB_ROOT);

 Circuit.Num_PI=NONE;
 Circuit.Mem_PI=NONE;
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_PO                                                      */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_PO(void)
{
 Circuit.Num_PO=NONE;
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_Circuit                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_Circuit(void)
{
 GATE_ID Id=NONE;

 if(NetList==NULL) return;
 
 Release_PI();
 Release_PO();
 Release_FF();
 
 Circuit.Num_Gates    =NONE;
 Circuit.Num_Levels   =NONE;
 
 Circuit.Mem_Circuit  =NONE;

 Circuit.Max_Fanin    =NONE;
 Circuit.Max_Fanout   =NONE;
 Circuit.Max_FF_Fanout=NONE;

 for(Id=NONE;Id < PORTS;Id++) Circuit.Ports[Id]=NONE;
 
 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  Free(NetList[Id].To_Gate,Id);
  Free(NetList[Id].To_Pin,Id);
  Free(NetList[Id].Pin_Value,Id);
 }
 
 Circuit.Num_Descr=NONE;

 Free(NetList,DB_ROOT);
 Free(Circuit.Gates_For_Level,DB_ROOT);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Print_FF                                                        */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Print_FF(void)
{
 FF_ID FF_index=NONE;

 PrintV(MESSAGE_COLOR,"Flip Flop [To List] Follows:");
 BarV(BAR_COLOR);
 for(FF_index=NONE;FF_index < Circuit.Num_FF;FF_index++) 
 {
#ifdef DEBUG  
  PrintV2SL(MESSAGE_COLOR,"Flip Flop ID:",FF_index,"Destination:",FF_Vect[FF_index]->Id);
#else
  PrintVSL(MESSAGE_COLOR,"Flip Flop ID:",FF_index);
#endif
 }
 BarV(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Print_PI                                                        */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Print_PI(void)
{
 GATE_ID PI_index=NONE;

 PrintV(MESSAGE_COLOR,"Primary In [To List] Follows:");
 BarV(BAR_COLOR);

#ifdef DEBUG   
 for(PI_index=NONE;PI_index < Circuit.Num_PI;PI_index++) PrintV2SL(MESSAGE_COLOR,"Primary In ID:",PI_index,"Destination:",PI_Vect[PI_index]->Id);
#else
 for(PI_index=NONE;PI_index < Circuit.Num_PI;PI_index++) PrintVSL(MESSAGE_COLOR,"Primary In ID:",PI_index);
#endif
 BarV(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Print_Scan_Chain_FF                                             */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Print_Scan_Chain_FF(void)
{
 GATE_ID  Id=NONE;
 GATE_FAN Fanout=NONE;
 CHAR     Chain=NONE;

 Phase("Scannig For FF Chains");
 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if(NetList[Id].Type==FF)
  {
   for(Fanout=NONE;Fanout < NetList[Id].Fanout;Fanout++) 
   {
    if(NetList[Id].To_Gate[Fanout]->Type==FF) 
    {
     Chain++;
     PrintV2SL(MESSAGE_COLOR,"FF Connected Source:",Id,"With Fanout:",Fanout);
    }
   }
  }
 }
 if(Chain) 
  {
   FF_Chain=TRUE;
   PrintSL(MESSAGE_COLOR,"FF Chain-Rings Found:",Chain);
  }
 else
 {
  FF_Chain=FALSE;
  Print(MESSAGE_COLOR,"No Direct FF Chain-Ring Found");
 }
 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_PO                                                       */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_PO(void)
{
 GATE_ID Id=NONE;
 GATE_ID Check_PO=NONE;

 Phase("Checking PO");
 Check_PO=NONE;

 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if(NetList[Id].Type==PO)
  {
   if(NetList[Id].Fanout!=NONE)
   {
    Gate_Error(Id,E_FAN_PO);
    return(ERROR);
   }
   if(NetList[Id].Fanin!=1)
   {
    Gate_Error(Id,E_FAN_PO);
    return(ERROR);
   }
   Check_PO++;
  }
 }
 
 if((Check_PO!=Circuit.Num_PO) || (Circuit.Num_PO==NONE) || (Check_PO==NONE))
 {
  PrintSL(ERROR_COLOR,"Given Number Of PO:",Circuit.Num_PO);
  PrintSL(ERROR_COLOR,"Calculated        :",Check_PO);
  Sys_Error("",E_PO_PO);
  return(ERROR);
 }

 Print(MESSAGE_COLOR,"PO Are OK!");
 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_LV                                                       */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_LV(void)
{
 GATE_ID Id=NONE;

 Phase("Checking Levels");
 if((Check_Levels!=Circuit.Num_Levels) || (Circuit.Num_Levels==NONE) || (Check_Levels==NONE))
 {
  PrintSL(ERROR_COLOR,"Given Number Of Levels:",Circuit.Num_Levels);
  PrintSL(ERROR_COLOR,"Calculated            :",Check_Levels);
  Sys_Error("",E_LEVELS_LEVELS);
  return(ERROR);
 }

 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if((NetList[Id].Level > Circuit.Num_Levels))
  {
   Gate_Error(Id,E_CIRCUIT_LEVELS);
   return(ERROR);
  }

  if(NetList[Id].Level >= MAX_LEVEL)
  {
   Gate_Error(Id,E_HIGH_LEVEL);
   return(ERROR);
  }
 }

 Print(MESSAGE_COLOR,"Levels Are OK!");
 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Check_Circuit                                                   */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Check_Circuit(void)
{
 GATE_ID  Id=NONE;
 GATE_FAN Fanout=NONE;

 Phase("Checking Loaded .CAL Circuit...");
 if((Check_Gates !=Circuit.Num_Gates) || (Circuit.Num_Gates==NONE) || (Check_Gates==NONE))
 {
  PrintSL(ERROR_COLOR,"Given Number Of Gates:",Circuit.Num_Gates);
  PrintSL(ERROR_COLOR,"Calculated           :",Check_Gates);
  Sys_Error("",E_GATES_GATES);
  return(ERROR);
 }

 for(Id=NONE;Id < Circuit.Num_Descr;Id++)
 {
  if(Verbose_Mode) {Print_PercentV(MESSAGE_COLOR, "Check Progress:",(Id+1),Circuit.Num_Descr);}

  if((NetList[Id].Type==NOT)    || (NetList[Id].Type==BUFFER) ||
     (NetList[Id].Type==CLOCK)  || (NetList[Id].Type==PO))
     {
      if(NetList[Id].Fanin > 1)
      {
       Gate_Error(Id,E_SINGLE_IN);
       return(ERROR);
      }
     }

  if(((NetList[Id].Type&GATE_MASK)!=BUFFER) &&
     ((NetList[Id].Type&GATE_MASK)!=AND)    &&
     ((NetList[Id].Type&GATE_MASK)!=OR)     &&
     ((NetList[Id].Type&GATE_MASK)!=EXOR)   &&
     ((NetList[Id].Type&GATE_MASK)!=FF)     && /* FANIN IS EQUAL TO FFINDEX */
     ((NetList[Id].Type&GATE_MASK)!=PI)     &&
     ((NetList[Id].Type&GATE_MASK)!=NOT)    &&
     ((NetList[Id].Type&GATE_MASK)!=NAND)   &&
     ((NetList[Id].Type&GATE_MASK)!=NOR)    &&
     ((NetList[Id].Type&GATE_MASK)!=NEXOR)  &&
     ((NetList[Id].Type&GATE_MASK)!=PO)     &&
     ((NetList[Id].Type&GATE_MASK)!=CLOCK))
     {
      Gate_Error(Id,E_UNK_GATE);
      return(ERROR);
     }

  if(NetList[Id].Fanout==NONE) 
  {
   if((NetList[Id].Type!=PO) && (NetList[Id].Type!=CLOCK))
   {
    Bar(BAR_COLOR);
    PrintSL(ERROR_COLOR,"Gate :",Id);
    Print2(ERROR_COLOR, "Type :",GType[(NetList[Id].Type&GATE_MASK)]);
    Sys_Error("",E_NO_FAN);
    return(ERROR);
   }
  }

  if(NetList[Id].Fanin==NONE) 
  {
   if((NetList[Id].Type!=PI) && (NetList[Id].Type!=CLOCK))
   {
    Bar(BAR_COLOR);
    PrintSL(ERROR_COLOR,"Gate :",Id);
    Print2(ERROR_COLOR, "Type :",GType[(NetList[Id].Type&GATE_MASK)]);
    Sys_Error("",E_NO_FAN);
    return(ERROR);
   }
  }
  
  for(Fanout=NONE; Fanout < NetList[Id].Fanout;Fanout++)
  {
   if((NetList[Id].To_Gate[Fanout]->Type!=FF) &&
      (NetList[Id].To_Gate[Fanout]->Type!=PI) &&
      (NetList[Id].To_Gate[Fanout]->Type!=CLOCK))
   {
    if(NetList[Id].Level >= NetList[Id].To_Gate[Fanout]->Level)
    {
     Gate_Error(Id,E_GATE_LEVEL);
     PrintSL(ERROR_COLOR,"Destination Gate:",Fanout);
     PrintSL(ERROR_COLOR,"Level           :",NetList[Id].To_Gate[Fanout]->Level);
     return(ERROR);
    }

    if(NetList[Id].Level >= MAX_LEVEL)
    {
     Gate_Error(Id,E_HIGH_LEVEL);
     return(ERROR);
    }
   }
  }
 }

 Print(MESSAGE_COLOR,"Gates Are OK!");
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
