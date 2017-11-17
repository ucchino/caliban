/*PPU************************************************************************\
*
*    $Source: sim.c
*  $Revision: 0.9.3
*      $Date: Thu Jul  8 19:31:04 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "sim.h"
# include "caliban.h"
# include "load_cir.h"
# include "load_fau.h"
# include "load_pat.h"

/*************************\
*                         *
* !!!= NOTA               *
* ^^^= MODIFICA APPORTATA *
*                         *
\*************************/

/**********************

se usassi un char per indicare il pin risparmierei dei byte (visto che adesso
uso un puntatore al pin da modificare) che potrei impiegare come fa giova, visto
che un gate puo' essere guastato da piu' di un guasto mi serve una lista... un indice
non basta...!!!

hmmm...una word di 32 bit garantiti in cui azzerare il bit relativo
al guasto nella fault list... bit a 0 indica posizione guasto...

una matrice di simulazione 32x32 BitsxBits in cui accodare la lista dei guasti
su di un solo gate invece di usare la lista lineare di 32 guasti...
accorpare insieme i guasti sullo stesso gate 

**********************/


/****************************************************************************/
/*                                                                          */
/* MY SYMBOL-VARIABLE... Ooops LA MIA VARIABILE SIMBOLO ISPIRATA DALLA      */
/* FAMOSISSIMA BUNZO-VARIABILE DI ANDREA MONTOBBIO LATINOFILO-ITALIANOFILO  */
/* STRENUO ASSERTORE DELL'IDIOZIA DELL'USO DELLA LINGUA INGLESE DA PARTE    */
/* DEI PROGRAMMATORI ITALIANI TRA CUI IL SOTTOSCRITTO PPU...                */
/*                                                                          */
/****************************************************************************/

ULONG          Bringo           =FALSE;

/****************************************************************************/

FF_ID          Num_FF           =NONE;

GATE_ID        Num_PI           =NONE;
GATE_ID        Num_PO           =NONE;
GATE_ID        Num_Gates        =NONE;
GATE_ID        Num_Descr        =NONE;

/****************************************************************************/

FAULT_ID       Num_Faults       =NONE;
GATE_LEVEL     Num_Levels       =NONE;
PATTERN_ID     Num_Patterns     =NONE;

ULONG          Events           =NONE;
ULONG          Good_Events      =NONE;
ULONG          Evaluated_Gates  =NONE;

DOUBLE         Coverage         =NONE;

/****************************************************************************/

FAULT_ID       Found_Faults     ;
FAULT_ID       Remaining_Faults ;

/****************************************************************************/

FAULT_ID       Current_Fault    =NONE;

PATTERN_ID     Current_Pattern  =NONE;

PATTERN_ID     Current_Resets   =NONE;

/****************************************************************************/

ULONG          Gain             =NONE;

/****************************************************************************/

_Event_List*   Event_List       =NULL;

 Fault_Ptr*    Fault_Block      =NULL;

 CHAR*         FF_Queue         =NULL;

/****************************************************************************/

#ifdef FAULT_QSORT
 Fault_Ptr*    Dest             =NULL;
 FAULT_ID*     Quantity         =NULL;
 FAULT_ID*     StartFrom        =NULL;
#endif

/****************************************************************************/

#ifdef THRESHOLD
 Fault_Ptr*    DestS            =NULL;
#endif

/****************************************************************************/

#ifdef PPU_VISIT
 Gate_Ptr*     Level_List       =NULL;
#endif

/****************************************************************************/
/*                                                                          */
/* Fx Name: Fresh_Sim_Stats                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Fresh_Sim_Stats(void)
{
 ULONG Rate;

 Circuit.Gain             =Gain;
 Circuit.Events           =Events;
 Circuit.Good_Events      =Good_Events;
 Circuit.Found_Faults     =Found_Faults;
 Circuit.Evaluated_Gates  =Evaluated_Gates;
 Circuit.Num_Used_Patterns=Current_Pattern;
 
 Rate=Sec(Simulation_Elapsed_Time);
 Circuit.Coverage=((100*((FLOAT)Found_Faults))/((FLOAT)Num_Faults));
 
 if(Rate > 0) Circuit.Event_Rate=(Evaluated_Gates/Rate);
 else         Circuit.Event_Rate=(Evaluated_Gates);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Reset_Sim_Stats                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Reset_Sim_Stats(void)
{
 Gain                     =NONE;
 Num_FF                   =Circuit.Num_FF;
 Num_PI                   =Circuit.Num_PI;
 Num_PO                   =Circuit.Num_PO;
 Events                   =NONE;
 Coverage                 =NONE;
 Num_Gates                =Circuit.Num_Gates;
 Num_Descr                =Circuit.Num_Descr;
 Num_Faults               =Circuit.Num_Faults;
 Num_Levels               =Circuit.Num_Levels;
 Good_Events              =NONE;
 Num_Patterns             =Circuit.Num_Patterns;
 Circuit.Gain             =NONE;
 Current_Fault            =NONE;
 Current_Resets           =NONE;
 Current_Pattern          =NONE;
 Evaluated_Gates          =NONE;
 Circuit.Event_Rate       =NONE;
 Circuit.Found_Faults     =NONE;
 Circuit.Mem_Simulation   =NONE;
 Circuit.Evaluated_Gates  =NONE;
 Circuit.Num_Used_Patterns=NONE;

 if(Multiple_Atpg_Mode==FALSE)
 {
  Found_Faults            =NONE;
  Remaining_Faults        =Circuit.Num_Faults;
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Event_List                                               */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Create_Event_List(void)
{
 GATE_LEVEL Level=NONE;

 Alloc(Event_List,(Num_Levels+1),_Event_List,Circuit.Mem_Simulation);

 for(Level=NONE;Level < Num_Levels;Level++)
 {
  if(Circuit.Gates_For_Level[Level]==NONE)
  {
   PrintSL(ERROR_COLOR,"Faulty Level Is:",Level);
   Sys_Error("",E_NO_GATES_ON_LEVEL);
  }
  Alloc(Event_List[Level].Scheduled_Gate,Circuit.Gates_For_Level[Level],Gate_Ptr,Circuit.Mem_Simulation);
 }
 Alloc(Event_List[Num_Levels].Scheduled_Gate,Num_FF,Gate_Ptr,Circuit.Mem_Simulation);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Clear_Event_List                                                */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Clear_Event_List(void)
{
 GATE_LEVEL Level=NONE;

 for(Level=NONE;Level <= Num_Levels;Level++)
 {
  Event_List[Level].Last_Gate=NONE;
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_FSim_Mem                                                */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_FSim_Mem(void)
{
 Circuit.Mem_Simulation=NONE;
 
 Release_FF_Queue();
 Release_QuickSort();
 Release_Event_List();
 Release_Level_List();
 Release_Fault_Block();
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_Event_List                                              */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_Event_List(void)
{
 GATE_LEVEL index1=NONE;

 if(Event_List==NULL) return;

 for(index1=NONE;index1 <= Num_Levels;index1++)
 {
  Free(Event_List[index1].Scheduled_Gate,index1);
 }
 Free(Event_List,DB_ROOT);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Detect_Event_On_PO                                              */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Detect_Event_On_PO(void)
{
 PATTERN_ID Pd=NONE;
 FAULT_ID   Id=NONE;

 for(Id=NONE;Id < Num_Faults;Id++)
 {
  if(Fault_Descriptor[Id].Pattern_Id!=UNKNOWN)
  {
   FPrintDetect(FLF,  "Detected Fault#  :",Id+1,"Pattern:",Fault_Descriptor[Id].Pattern_Id+1,"GateType:",GType[Fault_Descriptor[Id].Faulty_Gate->Type&GATE_MASK]);
  }
  else
  {
   FPrintUndetect(FLF,"Undetected Fault#:",Id+1,"GateType:",GType[Fault_Descriptor[Id].Faulty_Gate->Type&GATE_MASK]);
  }
 }
 FPrint(FLF,BAR);

 if(PatternOrdered_Mode==TRUE)
 {
  for(Pd=NONE;Pd < Num_Patterns;Pd++)
  {
   for(Id=NONE;Id < Num_Faults;Id++)
   {
    if(Pd==Fault_Descriptor[Id].Pattern_Id)
    {
     FPrint2SLFormatted(DLF,"Fault Id:",Id,"Detected At Pattern#:",Pd+1);
    }
   }
  }
  FPrint(DLF,BAR);
 }

 for(Id=NONE;Id < Num_Faults;Id++) Fault_Descriptor[Id].Pattern_Id=UNKNOWN;
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Precharge_Reset                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Precharge_Reset(void)
{
 FF_ID FF_Index=NONE;

 for(FF_Index=NONE;FF_Index < Num_FF;FF_Index++)
 {
  FF_Queue[FF_Index]=TRUE;
  Copy_Cell(FF_Vect[FF_Index]->Out_Value,FF_Init[FF_Index]);
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Sim                                                             */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Sim(void)
{
 if(NetList   ==NULL){Print(ERROR_COLOR,"Before SIM You MUST Load A Circuit !");  return(ERROR);}
 if(Fault_List==NULL){Print(ERROR_COLOR,"Before SIM You MUST Load A FaultList !");return(ERROR);}
 if(Pattern   ==NULL){Print(ERROR_COLOR,"Before SIM You MUST Load A Pattern !");  return(ERROR);}

 if(Fopen(Circuit.Name,SLF,SLF,CREATE_W,E_CREATE)==ERROR) return(ERROR);
 if(Fopen(Circuit.Name,FLF,FLF,CREATE_W,E_CREATE)==ERROR) {FClose(SLF);return(ERROR);}

 Init_SLog();
 Init_FLog();

 if(PatternOrdered_Mode==TRUE)
 {
  if(Fopen(Circuit.Name,DLF,DLF,CREATE_W,E_CREATE)==ERROR)
  {
   FClose(SLF);
   FClose(FLF);
   return(ERROR);
  }
  Init_DLog();
 }

 Reset_Sim_Stats();

 /******************************* SIMULATION *********/

 if(Multiple_Atpg_Mode==FALSE)
 {
  Clear_Circuit();
  Clear_Fault_List();
  Release_FSim_Mem();
  Create_FF_Queue();
  Create_Event_List();
  Clear_Event_List();
  Create_Fault_Block();
  Clear_Fault_Block();
  Create_QuickSort();
 }

 /******************************* SIMULATION *********/

 for(Bringo=NONE;Bringo < Num_FF;Bringo++) FF_Vect[Bringo]->Level=Num_Levels;

 Create_Level_List();

 fflush(stdout);
 fprintf(stderr,"\nDone:0%%");
 fflush(stderr);

 Get_Clock(Simulation_Start_Time);
 Inject_Pattern();
 Get_Clock(Simulation_Stop_Time);

 for(Bringo=NONE;Bringo < Num_FF;Bringo++) FF_Vect[Bringo]->Level=NONE;

 Simulation_Elapsed_Time=(Simulation_Stop_Time-Simulation_Start_Time);

 SPrint_Time(Circuit.Simulation_Stop_Time,Simulation_Stop_Time);
 SPrint_Time(Circuit.Simulation_Start_Time,Simulation_Start_Time);
 SPrint_Time(Circuit.Simulation_Elapsed_Time,Simulation_Elapsed_Time);

 Detect_Event_On_PO();

 Fresh_Sim_Stats();
 Save_SLog();

 if(PatternOrdered_Mode==TRUE) FClose(DLF);
 FClose(SLF);
 FClose(FLF);

 Log("Done Simulation Based On",Circuit.Name);

 Init_FFs(DEFAULT_FF_INIT_MODE);

 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Clear_Fault_Block                                               */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Clear_Fault_Block(void)
{
 FAULT_ID Id=NONE;

 for(Id=NONE;Id <= Bits;Id++)
 {
  Fault_Block[Id]=NULL;
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Fault_Block                                              */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Create_Fault_Block(void)
{
 Alloc(Fault_Block,(Bits+1),Fault_Ptr,Circuit.Mem_Simulation);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_QuickSort                                                */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 03/02/98                                                        */
/*                                                                          */
/****************************************************************************/

void Create_QuickSort(void)
{
 FAULT_ID Id=NONE;

#ifdef THRESHOLD
 Alloc(DestS,    (Num_Faults+1),Fault_Ptr,Circuit.Mem_Simulation);
 for(Id=NONE;Id <=Num_Faults;Id++) DestS[Id]=NULL;
#endif
 
#ifdef FAULT_QSORT 
 Alloc(Dest,     (Num_Faults+1),Fault_Ptr,Circuit.Mem_Simulation);
 Alloc(Quantity, (Num_FF+1)    ,FAULT_ID ,Circuit.Mem_Simulation);
 Alloc(StartFrom,(Num_FF+1)    ,FAULT_ID ,Circuit.Mem_Simulation);
 for(Id=NONE;Id <=Num_Faults;Id++) Dest[Id]=NULL;
 for(Id=NONE;Id <=Num_FF;Id++)    {Quantity[Id]=NONE;StartFrom[Id]=NONE;}
#endif 
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_QuickSort                                               */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 03/02/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_QuickSort(void)
{
#ifdef THRESHOLD 
 Free(DestS,DB_ROOT);
#endif
 
#ifdef FAULT_QSORT 
 Free(Dest,DB_ROOT);
 Free(Quantity,DB_ROOT);
 Free(StartFrom,DB_ROOT);
#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_Fault_Block                                             */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_Fault_Block(void)
{
 Free(Fault_Block,DB_ROOT);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_Level_List                                              */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 04/02/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_Level_List(void)
{
#ifdef PPU_VISIT 
 Free(Level_List,DB_ROOT);
#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_FF_Queue                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Create_FF_Queue(void)
{
 FF_ID FF_Index=NONE;

 Alloc(FF_Queue,Num_FF,CHAR,Circuit.Mem_Simulation);

 for(FF_Index=NONE;FF_Index < Num_FF;FF_Index++)
 {
  FF_Queue[FF_Index]=FALSE;
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_FF_Queue                                                */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_FF_Queue(void)
{
 Free(FF_Queue,DB_ROOT);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Schedule_Gate                                                   */
/* Param. : struct gate*                                                    */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Schedule_Gate(Gate_Ptr Gate)
{
 if(Gate->EnQueued==FALSE)
 {
  Gate->EnQueued=TRUE;
  Event_List[Gate->Level].Scheduled_Gate[Event_List[Gate->Level].Last_Gate++]=Gate;
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Schedule_Sons                                                   */
/* Param. : struct gate*,struct cell                                        */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Schedule_Sons(Gate_Ptr Gate,CELL Value)
{
 GATE_FAN Fanout=NONE;

 for(Fanout=NONE;Fanout < Gate->Fanout;Fanout++)
 {
  Copy_Cell((*(Gate->To_Pin[Fanout])),Value);

  if(Gate->To_Gate[Fanout]->EnQueued==FALSE)
  {
   Gate->To_Gate[Fanout]->EnQueued=TRUE;
   Event_List[Gate->To_Gate[Fanout]->Level].Scheduled_Gate[Event_List[Gate->To_Gate[Fanout]->Level].Last_Gate++]=Gate->To_Gate[Fanout];
  }
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Schedule_Sons_FSim                                              */
/* Param. : struct gate*,struct cell                                        */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Schedule_Sons_FSim(Gate_Ptr Gate,CELL Value)
{
 GATE_FAN Fanout=NONE;

 for(Fanout=NONE;Fanout < Gate->Fanout;Fanout++)
 {
  Copy_Part_Of_Cell((*(Gate->To_Pin[Fanout])),Value);

  if(Gate->To_Gate[Fanout]->EnQueued==FALSE)
  {
   Gate->To_Gate[Fanout]->EnQueued=TRUE;
   Event_List[Gate->To_Gate[Fanout]->Level].Scheduled_Gate[Event_List[Gate->To_Gate[Fanout]->Level].Last_Gate++]=Gate->To_Gate[Fanout];
  }
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Inject_Fau                                                      */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Inject_Fau(void)
{
 ULONG index1=NONE;

 for(index1=NONE;(Fault_Block[index1]!=NULL);index1++)
 {
  if(Fault_Block[index1]->Pin!=NULL)
  {
   Fault_Block[index1]->Faulty_Gate->Type|=FAULT_ON_IN;
   Schedule_Gate(Fault_Block[index1]->Faulty_Gate);
   continue;
  }

  if((Fault_Block[index1]->Faulty_Gate->Type&GATE_MASK)!=FF)
  {
   Fault_Block[index1]->Faulty_Gate->Type|=FAULT_ON_OUT;
   Schedule_Gate(Fault_Block[index1]->Faulty_Gate);
   continue;
  }

  FF_Queue[(Fault_Block[index1]->Faulty_Gate->Fanin)]=TRUE;

  if(Fault_Block[index1]->Stk_Value==STUCK0)
  {
   ClearBit_Cell(Fault_Block[index1]->Faulty_Gate->Out_Value,index1);
   continue;
  }
  else
  {
   SetBit_Cell(Fault_Block[index1]->Faulty_Gate->Out_Value,index1);
   continue;
  }
  Break(ERROR);
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Load_Scheduled_FF                                               */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Load_Scheduled_FF(void)
{
 ULONG index1=NONE;
 FF_ID FF_Index=NONE;

 for(index1=NONE;(Fault_Block[index1]!=NULL);index1++)
 {
  for(FF_Index=NONE;FF_Index < Fault_Block[index1]->Num_Enqueued_FFs;FF_Index++)
  {
   FF_Queue[(Fault_Block[index1]->FFs[FF_Index].FF_Gate->Fanin)]=TRUE;

   switch(Fault_Block[index1]->FFs[FF_Index].FF_Value)
   {
    case(FF_ZERO):ClearBit_Cell(Fault_Block[index1]->FFs[FF_Index].FF_Gate->Out_Value,index1);
    break;
    case(FF_ONE) :SetBit_Cell(Fault_Block[index1]->FFs[FF_Index].FF_Gate->Out_Value,index1);
    break;
    case(FF_ICS) :IcsBit_Cell(Fault_Block[index1]->FFs[FF_Index].FF_Gate->Out_Value,index1);
    break;
    default      :Sys_Error("",E_LOGIC_LEVEL);Break(ERROR);
   }
  }
  Fault_Block[index1]->Num_Enqueued_FFs=NONE;
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Sim_Other_Time                                                  */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Sim_Other_Time(void)
{
 Gate_Ptr   Gate=NULL;

 CELL       Value;
 GATE_ID    Id=NONE;
 GATE_LEVEL Level=NONE;

 for(Level=NONE;Level < Num_Levels;Level++)
 {
  for(Id=NONE;Id < Event_List[Level].Last_Gate;Id++)
  {
   Gate=Event_List[Level].Scheduled_Gate[Id];
   Value=Eval_Boz(Gate);

   if(Gate->Type!=PO) ++Evaluated_Gates;

   if(Different_Good_Cell(Value))
   {
    if(Gate->Type!=PO) ++Events;
    Schedule_Sons_FSim(Gate,Value);
   }
  }
  Event_List[Level].Last_Gate=NONE;
 }
 Store_Scheduled_FF();
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Reset_FF                                                        */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Reset_FF(void)
{
 ULONG index=NONE;

 for(index=NONE;index < Remaining_Faults;index++)
 {
  Fault_List[index]->Num_Enqueued_FFs=NONE;
 }
 
 for(index=NONE;index < Num_FF;index++)
 {
  /********************************************************************\
   * NB: Sveltisce di poco l'eliminazione della riga if sotto, ma per *
   *     contro ho molti meno eventi... Quale soluzione migliore?     *
  \********************************************************************/

  if(Different_Cells(FF_Vect[index]->Out_Value,FF_Init[index]))
  {
   FF_Queue[index]=TRUE;
   Copy_Cell(FF_Vect[index]->Out_Value,FF_Init[index]);
  }
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Eval_Boz                                                        */
/* Param. : struct gate*                                                    */
/* Return : struct cell                                                     */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

CELL Eval_Boz(Gate_Ptr Gate)
{
 CELL  Value;
 ULONG index1;

 Gate->EnQueued=FALSE;

 switch(Gate->Type)
 {
  case(BUFFER)  : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                return(Value);

  case(ANDG)    : Set_Cell(Value);
                for(index1=NONE;index1 < Gate->Fanin;index1++) {
                AND_OP(Value,Gate->Pin_Value[index1]);Restore_Cell(Gate->Pin_Value[index1]);}
                return(Value);

  case(ORG)     : Clear_Cell(Value);
                for(index1=NONE;index1 < Gate->Fanin;index1++) {
                OR_OP(Value,Gate->Pin_Value[index1]);Restore_Cell(Gate->Pin_Value[index1]);}
                return(Value);

  case(EXORG)   : Clear_Cell(Value);
                for(index1=NONE;index1 < Gate->Fanin;index1++) {
                EXOR_OP(Value,Gate->Pin_Value[index1]);Restore_Cell(Gate->Pin_Value[index1]);}
                return(Value);

  case(FF)      : Copy_Cell(Value,Gate->Pin_Value[FF_IN]);
                Restore_Cell(Gate->Pin_Value[FF_IN]);
                return(Value);

  case(PI)      : Copy_Cell(Value,Gate->Out_Value);
                Restore_Cell(Gate->Out_Value);
                return(Value);

  case(NOT)     : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                NOT_OP(Value);
                return(Value);

  case(NANDG)   : Set_Cell(Value);
                for(index1=NONE;index1 < Gate->Fanin;index1++) {
                AND_OP(Value,Gate->Pin_Value[index1]);Restore_Cell(Gate->Pin_Value[index1]);}
                NOT_OP(Value);
                return(Value);

  case(NORG)    : Clear_Cell(Value);
                for(index1=NONE;index1 < Gate->Fanin;index1++) {
                OR_OP(Value,Gate->Pin_Value[index1]);Restore_Cell(Gate->Pin_Value[index1]);}
                NOT_OP(Value);
                return(Value);

  case(NEXORG)  : Clear_Cell(Value);
                for(index1=NONE;index1 < Gate->Fanin;index1++) {
                EXOR_OP(Value,Gate->Pin_Value[index1]);Restore_Cell(Gate->Pin_Value[index1]);}
                NOT_OP(Value);
                return(Value);

  case(PO)      : {CELL_UNIT Exor_Fau=Different_Good_Cell(Gate->Pin_Value[0]);
                for(index1=NONE; (Fault_Block[index1]!=NULL); index1++)
                {
                 if((Bit_Is_One(Exor_Fau,index1)) && (Fault_Block[index1]->Pattern_Id==UNKNOWN))
                 {
                  Found_Faults++;
                  Fault_Block[index1]->Pattern_Id=(Current_Pattern-Current_Resets);
                 }
                }}

                Restore_Cell(Gate->Pin_Value[0]);
                return(Value);

  case(AND2)    : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                AND_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                return(Value);

  case(OR2)     : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                OR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                return(Value);

  case(EXOR2)   : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                EXOR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                return(Value);

  case(NAND2)   : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                AND_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                NOT_OP(Value);
                return(Value);

  case(NOR2)    : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                OR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                NOT_OP(Value);
                return(Value);

  case(NEXOR2)  : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                EXOR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                NOT_OP(Value);
                return(Value);

  case(AND3)    : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                AND_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                AND_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                return(Value);

  case(OR3)     : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                OR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                OR_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                return(Value);

  case(EXOR3)   : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                EXOR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                EXOR_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                return(Value);

  case(NAND3)   : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                AND_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                AND_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                NOT_OP(Value);
                return(Value);

  case(NOR3)    : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                OR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                OR_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                NOT_OP(Value);
                return(Value);

  case(NEXOR3)  : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                EXOR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                EXOR_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                NOT_OP(Value);
                return(Value);

  case(AND4)    : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                AND_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                AND_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                AND_OP(Value,Gate->Pin_Value[3]);Restore_Cell(Gate->Pin_Value[3]);
                return(Value);

  case(OR4)     : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                OR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                OR_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                OR_OP(Value,Gate->Pin_Value[3]);Restore_Cell(Gate->Pin_Value[3]);
                return(Value);

  case(EXOR4)   : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                EXOR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                EXOR_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                EXOR_OP(Value,Gate->Pin_Value[3]);Restore_Cell(Gate->Pin_Value[3]);
                return(Value);

  case(NAND4)   : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                AND_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                AND_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                AND_OP(Value,Gate->Pin_Value[3]);Restore_Cell(Gate->Pin_Value[3]);
                NOT_OP(Value);
                return(Value);

  case(NOR4)    : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                OR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                OR_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                OR_OP(Value,Gate->Pin_Value[3]);Restore_Cell(Gate->Pin_Value[3]);
                NOT_OP(Value);
                return(Value);

  case(NEXOR4)  : Copy_Cell(Value,Gate->Pin_Value[0]);Restore_Cell(Gate->Pin_Value[0]);
                EXOR_OP(Value,Gate->Pin_Value[1]);Restore_Cell(Gate->Pin_Value[1]);
                EXOR_OP(Value,Gate->Pin_Value[2]);Restore_Cell(Gate->Pin_Value[2]);
                EXOR_OP(Value,Gate->Pin_Value[3]);Restore_Cell(Gate->Pin_Value[3]);
                NOT_OP(Value);
                return(Value);

  case(FFF_IN)  : Gate->Type&=FAULT_2_GOOD_IN;
                for(index1=NONE;(Fault_Block[index1]!=NULL);index1++)
                {
                 if((Fault_Block[index1]->Faulty_Gate==Gate) && (Fault_Block[index1]->Pin!=NULL))
                 {
                  if(Fault_Block[index1]->Stk_Value==STUCK1)
                  {
                   SetBit_Cell((*(Fault_Block[index1]->Pin)),index1);
                  }
                  else
                  {
                   ClearBit_Cell((*(Fault_Block[index1]->Pin)),index1);
                  }
                 }
                }

                Value=Gate->Pin_Value[FF_IN];

                Restore_Cell(Gate->Pin_Value[FF_IN]);
                return(Value);

  default       : if(Gate->Type&FAULT_ON_OUT)
                {
                 Gate->Type&=FAULT_2_GOOD_OUT;
                 Value=Eval_Boz(Gate);

                 for(index1=NONE;(Fault_Block[index1]!=NULL);index1++)
                 {
                  if((Fault_Block[index1]->Faulty_Gate==Gate)&&(Fault_Block[index1]->Pin==NULL))
                  {
                   if(Fault_Block[index1]->Stk_Value==STUCK1)
                   {
                    SetBit_Cell(Value,index1);
                   }
                   else
                   {
                    ClearBit_Cell(Value,index1);
                   }
                  }
                 }
                 return(Value);
                }
                else
                {
                 Gate->Type&=FAULT_2_GOOD_IN;
                 for(index1=NONE;(Fault_Block[index1]!=NULL);index1++)
                 {
                  if((Fault_Block[index1]->Faulty_Gate==Gate) && (Fault_Block[index1]->Pin!=NULL))
                  {
                   if(Fault_Block[index1]->Stk_Value==STUCK1)
                   {
                    SetBit_Cell((*(Fault_Block[index1]->Pin)),index1);
                   }
                   else
                   {
                    ClearBit_Cell((*(Fault_Block[index1]->Pin)),index1);
                   }
                  }
                 }

                 Value=Eval_Boz(Gate);
                 return(Value);
                }
               }
              }

/****************************************************************************/
/*                                                                          */
/* Fx Name: Choose_Fault_Block_FSim                                         */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Choose_Fault_Block_FSim(void)
{
 FAULT_ID Choosed_Fault=NONE;

 for(Choosed_Fault=NONE;(Fault_List[Current_Fault]!=NULL);Current_Fault++)
 {
  if(Excitable_Fault_FSim(Current_Fault))
  {
   Fault_Block[Choosed_Fault]=Fault_List[Current_Fault];
   Choosed_Fault++;

   if(Choosed_Fault >= Bits)
   {
    Current_Fault++;
    break;
   }
  }
 }
 
 Fault_Block[Choosed_Fault]=NULL;
 return(Choosed_Fault);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Choose_Fault_Block_GoodSim                                      */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Choose_Fault_Block_GoodSim(void)
{
 FAULT_ID Choosed_Fault=NONE;

 for(Choosed_Fault=NONE;(Fault_List[Current_Fault]!=NULL);Choosed_Fault++,Current_Fault++)
 {
  Fault_Block[Choosed_Fault]=Fault_List[Current_Fault];

  if(Choosed_Fault >= Bits) break;
 }
 Fault_Block[Choosed_Fault]=NULL;
 return(Choosed_Fault);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: QuickSort                                                       */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 03/02/98                                                        */
/*                                                                          */
/****************************************************************************/

void QuickSort(void)
{
#ifdef FAULT_QSORT
 UINT i=NONE,j=NONE;

 StartFrom[0]=NONE;
 for(i=1; i <= Num_FF ;i++)
 {
  StartFrom[i]=StartFrom[i-1]+Quantity[i-1];
  Quantity[i-1]=0;
 }

 for(i=NONE; Fault_List[i]!=NULL ;i++)
 {
  j=Fault_List[i]->Num_Enqueued_FFs;
  Dest[ StartFrom[j] ]=Fault_List[i];
  StartFrom[j]++;
 }

 Dest[i]=NULL;
 memcpy(Fault_List,Dest,i*sizeof(Fault_Ptr));
#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Drop_Out                                                        */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Drop_Out(void)
{
 FAULT_ID Fau=NONE;
 FAULT_ID Fad=NONE;
 FAULT_ID Des=NONE;

 for(Fau=Fad=NONE;Fault_List[Fau]!=NULL;Fau++)
 {
  if(Fault_List[Fau]->Pattern_Id==UNKNOWN)
  {

#ifdef FAULT_QSORT
   Quantity[Fault_List[Fau]->Num_Enqueued_FFs]++;
#endif   
   
#ifdef THRESHOLD
   if(Fault_List[Fau]->Num_Enqueued_FFs > THRESHOLDVAL) DestS[Des++]=Fault_List[Fau];
   else
#endif

   Fault_List[Fad++]=Fault_List[Fau];
  }
 }
 
#ifdef THRESHOLD
#ifdef REVERSE_SORT 
 for(;Des > NONE;) Fault_List[Fad++]=DestS[--Des];
#else
 DestS[Des]=NULL;
 for(Des=NONE;Dest[Des]!=NULL;Des++) Fault_List[Fad++]=Dest[Des];
#endif
#endif
 
 Remaining_Faults=Fad;
 Fault_List[Fad]=NULL;
 
#ifdef FAULT_QSORT 
 QuickSort();
#endif
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* Fx Name: Inject_Pattern                                                  */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Inject_Pattern(void)
{
 UINT       OPatt_Percent=NONE;
 UINT       NPatt_Percent=NONE;
 PATTERN_ID Pattern_Index=NONE;

 CELL       Real_Value[3];
 FF_ID      FF_Index=NONE;
 
 Set_Cell(Real_Value[PATTERN_ONE]);
 Ics_Cell(Real_Value[PATTERN_ICS]);
 Clear_Cell(Real_Value[PATTERN_ZERO]);
 
 Found_Faults=NONE;
   
 Precharge_Reset();

 for(Current_Pattern=NONE;Current_Pattern < Num_Patterns;Current_Pattern++)
 {
  Current_Fault=NONE;

  NPatt_Percent=(100*(Current_Pattern+1)/Num_Patterns);

  if(OPatt_Percent!=NPatt_Percent)
  {
   OPatt_Percent=NPatt_Percent;

#ifdef VERBOSESIM
   UINT Cov=NONE;
   UINT Rat=NONE;
   UINT Res=NONE;

   Cov=(100*(Found_Faults+1)/Num_Faults);
   Get_Clock(Simulation_Stop_Time);
   Simulation_Elapsed_Time=(Simulation_Stop_Time-Simulation_Start_Time);
   Rat=Sec(Simulation_Elapsed_Time);
   if(Rat > 0) Res=(Evaluated_Gates/Rat);
   else        Res=(Evaluated_Gates);

   Circuit.Coverage=((100*((FLOAT)Found_Faults))/((FLOAT)Num_Faults));

   fprintf(stderr,"\rDone:%d%% Coverage:%d%% Gate/Sec:%d",NPatt_Percent,Cov,Res);
   fflush(stderr);

#else
   fprintf(stderr,"\rDone:%d%%",NPatt_Percent);
#endif
   
  }

  if(Pattern[Current_Pattern][0]==PATTERN_RESET)
  {
   Reset_FF();

   Current_Pattern++;

   /********************************************/
   /* Eseguo il reset e salto subito al        */
   /* pattern successivo per non perdere tempo */
   /********************************************/

   if(Reset_Count_Mode==TRUE) Current_Resets++;
  }
  else
  {
   for(FF_Index=NONE;FF_Index < Num_FF;FF_Index++)
   {
    if(Different_Cells(FF_Vect[FF_Index]->Out_Value,FF_Vect[FF_Index]->Pin_Value[FF_IN]))
    {
     FF_Queue[FF_Index]=TRUE;
     Copy_Cell(FF_Vect[FF_Index]->Out_Value,FF_Vect[FF_Index]->Pin_Value[FF_IN]);
    }
   }
  }

  for(Pattern_Index=NONE;Pattern_Index < Num_PI;Pattern_Index++)
  {
   if(Different_Cells(PI_Vect[Pattern_Index]->Out_Value,Real_Value[Pattern[Current_Pattern][Pattern_Index]]))
   {
    Copy_Cell(PI_Vect[Pattern_Index]->Out_Value,Real_Value[Pattern[Current_Pattern][Pattern_Index]]);
    Schedule_Sons(PI_Vect[Pattern_Index],Real_Value[Pattern[Current_Pattern][Pattern_Index]]);
   }
  }

#ifdef GOODFSIM
  if(Choose_Fault_Block_GoodSim()==NONE) return;
  Load_Scheduled_FF();
  Inject_Fau();
#endif

  /* !!! FF_Queue Memorizza i FF che sono per qualche ragione stati modificati */
  /* !!! invece di usare una lista seq di ff potrei usare una semplice coda e */
  /* !!! tenere dentro questa l'indirizzo o valore del ff modificato... */
  /* !!! senza cosi' dover scandire magari una luuunga  lista di FF */
  /* !!! non usare + if(FF_Queue[FF_Index]==TRUE) ma semplicemente agire */

  for(FF_Index=NONE;FF_Index < Num_FF;FF_Index++)
  {
   if(FF_Queue[FF_Index]==TRUE)
   {
    Schedule_Sons(FF_Vect[FF_Index],FF_Vect[FF_Index]->Out_Value);
    Restore_Cell(FF_Vect[FF_Index]->Out_Value);
    FF_Queue[FF_Index]=FALSE;
   }
  }

  Sim_First_Time();

#ifdef PPU_VISIT
  PPU_Visit();
#endif

#ifdef GOODFSIM

  /* !!! lo droppo se e' alla fine della lista perche' i guasti */
  /* !!! trovati sono messi in coda... verificare la cosa... perche' */
  /* !!! tanto il dropping elimina il guasto dalla flist (nella */

  if(Current_Fault >= Remaining_Faults)
  {
   Drop_Out();
   continue;
  }

  if(Found_Faults  >= Num_Faults) return;
#endif

  ForEver()
  {
   if(Choose_Fault_Block_FSim()==NONE) break;

   Load_Scheduled_FF();
   Inject_Fau();

   for(FF_Index=NONE;FF_Index < Num_FF;FF_Index++)
   {
    if(FF_Queue[FF_Index]==TRUE)
    {
     Schedule_Sons(FF_Vect[FF_Index],FF_Vect[FF_Index]->Out_Value);
     Restore_Cell(FF_Vect[FF_Index]->Out_Value);
     FF_Queue[FF_Index]=FALSE;
    }
   }

   Sim_Other_Time();

   if(Current_Fault >= Remaining_Faults) break;
   if(Found_Faults  >= Num_Faults)       return;
  }

  Drop_Out();
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Sim_First_Time                                                  */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Sim_First_Time(void)
{
 Gate_Ptr Gate;

 GATE_ID    Id=NONE;
 GATE_LEVEL Level=NONE;
 CELL       Pre_Value;
 CELL       Post_Value;

 for(Level=NONE;Level < Num_Levels;Level++)
 {
  for(Id=NONE;Id < Event_List[Level].Last_Gate;Id++)
  {
   Gate=Event_List[Level].Scheduled_Gate[Id];
   Copy_Cell(Pre_Value,Gate->Out_Value);
   Post_Value=Eval_Boz(Gate);

#ifdef FS32BIT
         if(Cell_Is_Zero(Post_Value)) Post_Value.RealMan=QUICK_ZERO;
    else if(Cell_Is_One(Post_Value))  Post_Value.RealMan=QUICK_ONE;
    else                              Post_Value.RealMan=QUICK_ICS;    
#endif

   if(Gate->Type!=PO) ++Evaluated_Gates;
   if(Different_Cells(Post_Value,Pre_Value))
   {
    if(Gate->Type!=PO) ++Good_Events;
    Schedule_Sons(Gate,Post_Value);

#ifdef GOODFSIM
    Restore_Cell(Post_Value);
#endif

    Copy_Cell(Gate->Out_Value,Post_Value);
   }
  }
  Event_List[Level].Last_Gate=NONE;
 }

#ifdef GOODFSIM
 Store_Scheduled_FF();
#else
 Event_List[Num_Levels].Last_Gate=NONE;
#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Store_Scheduled_FF                                              */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Store_Scheduled_FF(void)
{
 CELL      Value;
 CELL_UNIT Diff=NONE;
 FF_ID     FF_Index=NONE;

 for(FF_Index=NONE;FF_Index < Event_List[Num_Levels].Last_Gate;FF_Index++)
 {
  Value=Eval_Boz(Event_List[Num_Levels].Scheduled_Gate[FF_Index]);
  Diff=(Different_Good_Cell(Value));

  if(Diff)
  {
   ULONG index1=NONE;
   for(index1=NONE;(Fault_Block[index1]!=NULL);index1++)

              /* !!! qui posso mettere Bits se sono sicuro che
i bit in sovrappiu' (se sto lavorando con un block di per esempio Bits-13=32-13, guasti
ha su quei bit solo degli zeri >> "Diff " e' zero su quei bit...***/

   {
    if(Bit_Is_One(Diff,index1))
    {
     FF_ID FFs=NONE;
     FFs=Fault_Block[index1]->Num_Enqueued_FFs++;

#if CHECK_COMPAQ_FF
     if(FFs >= ENQUEUABLE_FFS)
     {
      Print(Normal,  "Pre Dropp");
      PrintSL(Normal,"FoundFau:",Found_Faults);
      PrintSL(Normal,"Accodati:",Event_List[Num_Levels].Last_Gate);
      PrintSL(Normal,"Max     :",ENQUEUABLE_FFS);
      PrintSL(Normal,"Pattern :",Current_Pattern);
      
      Sys_Error("PANIK!",E_FAU_STORE_FF);Break(ERROR);
     }
#endif     

     Fault_Block[index1]->FFs[FFs].FF_Gate=Event_List[Num_Levels].Scheduled_Gate[FF_Index];

          if(Cell_Bit_Is_One(Value,index1)) {Fault_Block[index1]->FFs[FFs].FF_Value=FF_ONE;}
     else if(Cell_Bit_Is_Zero(Value,index1)){Fault_Block[index1]->FFs[FFs].FF_Value=FF_ZERO;}
     else                                   {Fault_Block[index1]->FFs[FFs].FF_Value=FF_ICS;}
    }
   }
  }
 }

 Event_List[Num_Levels].Last_Gate=NONE;
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Excitable_Fault                                                 */
/* Param. : unsigned long                                                   */
/* Return : signed long                                                     */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

CELL_UNIT Excitable_Fault_FSim(FAULT_ID Fau)
{
 CELL Check;

 if(Fault_List[Fau]->Num_Enqueued_FFs) return(TRUE);

 if(Fault_List[Fau]->Pin!=NULL)
 {
  if(Fault_List[Fau]->Faulty_Gate->Type==FF) return(TRUE);

#ifdef PPU_VISIT
  if(Is_Inactive_Ptr(Fault_List[Fau]->Pin))  return(FALSE);
#endif

/* !!! Se PPU_VISIT attivo questa condizione sembra superflua quindi dovrei mettere return(TRUE) */

  if((Fault_List[Fau]->Stk_Value==STUCK0) && (Cell_Is_Zero((*Fault_List[Fau]->Pin)))) return(FALSE);
  if((Fault_List[Fau]->Stk_Value==STUCK1) && (Cell_Is_One((*Fault_List[Fau]->Pin))))  return(FALSE);

  if(Fault_List[Fau]->Stk_Value==STUCK0) {ClearBit_Cell((*(Fault_List[Fau]->Pin)),0);}
  else                                   {SetBit_Cell((*(Fault_List[Fau]->Pin)),0);}

  Check=Eval_Boz(Fault_List[Fau]->Faulty_Gate);

#ifdef TWO_LEVEL_VISIT
  if(Different_Good_Cell(Check))
  {
   GATE_FAN Fanout;
   CELL     RetVal;
   Gate_Ptr Gate=Fault_List[Fau]->Faulty_Gate;

   for(Fanout=NONE;Fanout < Gate->Fanout;Fanout++)
   {
    if((Gate->To_Gate[Fanout]->Type)==PO) return(TRUE); /** ??????**/

    Copy_Part_Of_Cell((*Gate->To_Pin[Fanout]),Check);
    
    RetVal=Eval_Boz(Gate->To_Gate[Fanout]);
    
    if(Different_Good_Cell(RetVal))
    {
     return(TRUE);
    }
   }
  }
  return(FALSE);
#else
  if(Different_Good_Cell(Check))
  {
       return(TRUE);
  }
  else return(FALSE);
#endif
 }

 if((Fault_List[Fau]->Stk_Value==STUCK0) && (Cell_Is_Zero(Fault_List[Fau]->Faulty_Gate->Out_Value))) return(FALSE);
 if((Fault_List[Fau]->Stk_Value==STUCK1) && (Cell_Is_One(Fault_List[Fau]->Faulty_Gate->Out_Value)))  return(FALSE);

#ifdef PPU_VISIT
 if(Is_Inactive(Fault_List[Fau]->Faulty_Gate->Out_Value)) return(FALSE);
#endif

#ifdef TWO_LEVEL_VISIT
 else
 {
  GATE_FAN Fanout;
  CELL     RetVal;
  Gate_Ptr Gate=Fault_List[Fau]->Faulty_Gate;

  Copy_Cell(Check,Gate->Out_Value);
  
  if(Fault_List[Fau]->Stk_Value==STUCK0) {ClearBit_Cell(Check,0);}
  else                                   {SetBit_Cell(Check,0);}

  for(Fanout=NONE;Fanout < Gate->Fanout;Fanout++)
  {
   if((Gate->To_Gate[Fanout]->Type)==PO) return(TRUE);

   Copy_Part_Of_Cell((*Gate->To_Pin[Fanout]),Check);
   
   RetVal=Eval_Boz(Gate->To_Gate[Fanout]);
   
   if(Different_Good_Cell(RetVal))
   {
    return(TRUE);
   }
  }
 }
 return(FALSE);
#else
 return(TRUE);
#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Level_List                                               */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 04/02/98                                                        */
/*                                                                          */
/****************************************************************************/

void Create_Level_List(void)
{
#ifdef PPU_VISIT
 GATE_ID    Ip=NONE;
 GATE_LEVEL Level=NONE;
 GATE_ID    index1=NONE;

 Alloc(Level_List,(Num_Descr-(Num_PO)+1),Gate_Ptr,Circuit.Mem_Simulation);

 for(Level=Num_Levels; Level >= NONE;--Level)
 {
  for(Ip=NONE;Ip < Num_Descr;Ip++)
  {
   if(NetList[Ip].Level==Level)
   {
    if(NetList[Ip].Type==PO) continue;

/* !!! PROVA ... VVVVVVVVVV  **/

   if((NetList[Ip].Type==FF) && (NetList[Ip].Level!=NONE)) Break(ERROR);


    Level_List[index1++]=&(NetList[Ip]);
   }
  }
 }

 Level_List[index1]=NULL;

#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: PPU_Visit                                                       */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 04/02/98                                                        */
/*                                                                          */
/****************************************************************************/

void PPU_Visit(void)
{
#ifdef PPU_VISIT
 GATE_FAN Fanin=NONE;
 GATE_FAN Fanout=NONE;
 GATE_ID  index1=NONE;

 CHAR     BackPropagation=FALSE;
/*

   LOGICA NEGATA DA RIVEDERE !!!!!!!!!!!!!!!!!!

Vorrei perdere meno tempo nella stesura della mappa durante la visit...
Vorrei evitare di dover toccare dei pin per portarli allo stato di default.

Sveltiamo? Bene!!!!


HP Idea figa...

tutti i pin sono inattivi... spiegazione:coni di influenza...

e poi...

3xX locazioni...

1 default (ATTIVO)
2 non attivo
3 di passaggio...


durante la visit porto tutti i non attivi a puntare a 2
alla fine della sim per il dato pattern cambio il valore contenuto in 2 in attivo
--->reset globale ad attivo

al pattern successivo i non attivi punteranno a 3...

cambiando quanto puntato dai pin non attivi ottengo un reset sopraluce....


PROBLEMATICA DEI FF E DEI PO...............

  **/


 for(index1=NONE;Level_List[index1]!=NULL;index1++)
 {
  for(Fanout=NONE;Fanout < Level_List[index1]->Fanout;Fanout++)
  {
   if(Is_Active_Ptr(Level_List[index1]->To_Pin[Fanout]))
   {
    Active(Level_List[index1]->Out_Value);
    break;
   }
  }

  if(BackPropagation==FALSE)
  {
   Inactive(Level_List[index1]->Out_Value);
   continue;
  }

  switch(Level_List[index1]->Type&GATE_MASK)
  {
   case(ANDG) : if(Cell_Is_Not_Zero(Level_List[index1]->Out_Value))
                {
                 for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++)
                 {
                  Active(Level_List[index1]->Pin_Value[Fanin]);
                 }
                }
                else
                for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++)
                {
                 if(Cell_Is_Not_Zero(Level_List[index1]->Pin_Value[Fanin]))
                 {
                  Inactive(Level_List[index1]->Pin_Value[Fanin]);
                 }
                 else Active(Level_List[index1]->Pin_Value[Fanin]);
                }
   break;
   case(ORG)  : if(Cell_Is_Not_One(Level_List[index1]->Out_Value))
                {
                 for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++)
                 {
                  Active(Level_List[index1]->Pin_Value[Fanin]);
                 }
                }
                else
                for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++)
                {
                 if(Cell_Is_Not_One(Level_List[index1]->Pin_Value[Fanin]))
                 {
                  Inactive(Level_List[index1]->Pin_Value[Fanin]);
                 }
                 else Active(Level_List[index1]->Pin_Value[Fanin]);
                }

   break;
   case(NANDG): if(Cell_Is_Not_One(Level_List[index1]->Out_Value))
                {
                 for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++)
                 {
                  Active(Level_List[index1]->Pin_Value[Fanin]);
                 }
                }
                else
                for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++)
                {
                 if(Cell_Is_Not_Zero(Level_List[index1]->Pin_Value[Fanin]))
                 {    
                  Inactive(Level_List[index1]->Pin_Value[Fanin]);
                 }
                 else Active(Level_List[index1]->Pin_Value[Fanin]);
                }

   break;
   case(NORG) : if(Cell_Is_Not_Zero(Level_List[index1]->Out_Value))
                {
                 for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++)
                 {
                  Active(Level_List[index1]->Pin_Value[Fanin]);
                 }
                }
                else
                for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++)
                {
                 if(Cell_Is_Not_One(Level_List[index1]->Pin_Value[Fanin]))
                 {    
                  Inactive(Level_List[index1]->Pin_Value[Fanin]);
                 }
                 else Active(Level_List[index1]->Pin_Value[Fanin]);
                }

   break;
   case(FF)   : break;
   default    : for(Fanin=NONE;Fanin < Level_List[index1]->Fanin;Fanin++) 
                Active(Level_List[index1]->Pin_Value[Fanin]);
  }
 }
#endif
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
