/*PPU************************************************************************\
*
*    $Source: dee2cal.c
*  $Revision: 0.7.5
*      $Date: Thu Jul  8 19:30:44 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "caliban.h"
# include "dee2cal.h"

/****************************************************************************/

static _GateDes* Gate_Dee2Cal;

static LONG a,b,c,d,e,f,g,i,j,n1,n4,n5,n6;

static LONG Num_Pi     =NONE;
static LONG Num_Po     =NONE;
static LONG Num_FF     =NONE;
static LONG Num_Gates  =NONE;
static LONG Num_Descr  =NONE;
static LONG Max_level  =NONE;
static LONG Max_fanin  =NONE;
static LONG Max_fanout =NONE;
static LONG Max_givout =NONE;
static LONG Max_fanoutD=NONE;
static LONG first_pout =NONE;
static LONG Clock_Descr=NONE;

/****************************************************************************/
/** RICORDARE BENE CHE I PO SONO GATES PER I FILES DEE MENTRE PER ME NO *****/
/** QUINDI NUM_GATES=NUM_PO+PORTE VARIE *************************************/
/** *************************************************************************/
/****************************************************************************/

static ULONG FileLine                     =NONE;
static ULONG Mem_Used                     =NONE;
static ULONG Allocated_Dee2Cal_Descriptors=NONE;

/****************************************************************************/

static INT  flg_31B;
 
static CHAR version[STRING_LEN];

/****************************************************************************/

void Clear_Dee2Cal_Vars(void)
{
 a=b=c=d=e=f=g=i=j=n1=n4=n5=n6=NONE;
 Allocated_Dee2Cal_Descriptors=NONE;
 Num_Pi     =NONE;
 Num_Po     =NONE;
 Num_FF     =NONE;
 Num_Gates  =NONE;
 Num_Descr  =NONE;
 FileLine   =NONE;
 Max_level  =NONE;
 Max_fanin  =NONE;
 Max_fanout =NONE;
 Max_givout =NONE;
 first_pout =NONE;
 Clock_Descr=NONE;
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Dee2Cal                                                         */
/* Param. : char*                                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Dee2Cal(CHAR* File_Name)
{
 if(Fopen(File_Name,DEE,DEE,OPEN_R  ,E_OPEN)  ==ERROR) return(ERROR);
 if(Fopen(File_Name,CAL,CAL,CREATE_W,E_CREATE)==ERROR) {FClose(DEE);return(ERROR);}
 if(Fopen(File_Name,GEN,GEN,CREATE_W,E_CREATE)==ERROR) {FClose(DEE);FClose(CAL);return(ERROR);}

 Clear_Dee2Cal_Vars();
 Release_Dee2Cal();
 
 if(Read_Rumenta()==ERROR) 
 {
  Bye();
  return(ERROR);
 }

 Phase("Loading DEE To Convert To CAL Format");

 Allocated_Dee2Cal_Descriptors=(Num_Descr+Num_Po);
 Alloc(Gate_Dee2Cal,Allocated_Dee2Cal_Descriptors,_GateDes,Mem_Used);
 
 for(i=NONE; i < Allocated_Dee2Cal_Descriptors; i++)
 {
  Gate_Dee2Cal[i].Fanout=NONE;
  Gate_Dee2Cal[i].Fanin =NONE;
  Gate_Dee2Cal[i].From  =NULL;
  Gate_Dee2Cal[i].To    =NULL;
  Gate_Dee2Cal[i].Type  =UNKNOWN0;
 }

 /********* ALLOCATION DATA  **********/

 for(i=NONE; i < Num_Descr; i++)
 {
  BarV(BAR_COLOR);
  fscanf(File[DEE].Ptr,"%ld %ld %*s %ld",&n1,&d,&n4);       /* n4 = level     */
  FileLine++;                                               /*  d = wire/gate */
                                                            /* n1 = internal  */
  Gate_Dee2Cal[i].Attr=n1;
  
  if(n4 > MAX_LEVEL)
  {
   Bye();
   PrintSL(ERROR_COLOR,"Found Level  :",n4);
   PrintSL(ERROR_COLOR,"On Descriptor:",i);
   Sys_Error("",E_HIGH_LEVEL);
   return(ERROR);
  }
  
  Gate_Dee2Cal[i].Level=n4;

  if(Gate_Dee2Cal[i].Attr==MMCLOCK)    PrintV2SL(MESSAGE_COLOR,"Clock Found In Position:",i,"Of:",Num_Descr-1);
  if(Gate_Dee2Cal[i].Attr==MMINT)      PrintV2SL(MESSAGE_COLOR,"Gate  Found In Position:",i,"Of:",Num_Descr-1);
  if(Gate_Dee2Cal[i].Attr==MMPI)       PrintV2SL(MESSAGE_COLOR,"PIN   Found In Position:",i,"Of:",Num_Descr-1);
  if(Gate_Dee2Cal[i].Attr==MMPO)       PrintV2SL(MESSAGE_COLOR,"POUT  Found In Position:",i,"Of:",Num_Descr-1);
  
  if(Max_level < Gate_Dee2Cal[i].Level) Max_level=Gate_Dee2Cal[i].Level;

  fscanf(File[DEE].Ptr, "%ld %*s %*s %ld %ld %ld", &n1, &n4, &n5, &n6);
  FileLine++;

  switch (d)                                  /* n5 = fanin  */
  {                                           /* n4 = fanout */
   case MMWIRE:if(n5 > NONE)
               {
                Bye();
                PrintSL(ERROR_COLOR,"Found Fanin  :",n5);
                PrintSL(ERROR_COLOR,"Primary Input:",i)
                Sys_Error("",E_FAN_PI);
                return(ERROR);
               }

               Gate_Dee2Cal[i].Fanout=n4;
               Gate_Dee2Cal[i].Fanin =n5;
               Gate_Dee2Cal[i].From  =NULL;
               Gate_Dee2Cal[i].To    =NULL;
               Gate_Dee2Cal[i].Type  =PI;
   break;
   case MMGATE:if(n5 > MAX_FAN)                        /* n5 = fanout */
               {                                       /* n6 = fanin  */
                Bye();                                 /* n1 = type   */
                PrintSL(ERROR_COLOR,"Gate   :",i);
                PrintSL(ERROR_COLOR,"Fanout :",n5);
                Print2(ERROR_COLOR, "Type Is:",GType[Gate_Dee2Cal[i].Type&GATE_MASK]);
                Sys_Error("",E_HIGH_FAN);
                return(ERROR);
               }
               if(n6 > MAX_FAN)
               {
                Bye();
                PrintSL(ERROR_COLOR,"Gate   :",i);
                PrintSL(ERROR_COLOR,"Fanin  :",n6);
                Print2(ERROR_COLOR, "Type Is:",GType[Gate_Dee2Cal[i].Type&GATE_MASK]);
                Sys_Error("",E_HIGH_FAN);
                return(ERROR);
               }
   
               Gate_Dee2Cal[i].Fanout=n5;
               Gate_Dee2Cal[i].Fanin =n6;
               Gate_Dee2Cal[i].From  =NULL;
               Gate_Dee2Cal[i].To    =NULL;
               
               switch (n1)
               {
                case MMAND:  Gate_Dee2Cal[i].Type=AND;
                break;
                case MMNAND: Gate_Dee2Cal[i].Type=NAND;
                break;
                case MMOR:   Gate_Dee2Cal[i].Type=OR;
                break;
                case MMNOR:  Gate_Dee2Cal[i].Type=NOR;
                break;
                case MMBUF:  Gate_Dee2Cal[i].Type=BUFFER;
                break;
                case MMNOT:  Gate_Dee2Cal[i].Type=NOT;
                break;
                case MMEXOR: Gate_Dee2Cal[i].Type=EXOR;
                break;
                case MMEXNOR:Gate_Dee2Cal[i].Type=NEXOR;
                break;
                case MMFF:   Gate_Dee2Cal[i].Type=FF;
                Num_FF++;
                break;
                default:
                {
                 Bye();
                 PrintSL(ERROR_COLOR,"On Gate           :",i);
                 PrintSL(ERROR_COLOR,"Found Unknown Code:",n1);
                 Sys_Error("",E_UNK_GATE);
                 return(ERROR);
                }
               }

               if((n1!=MMNOT) && (n1!=MMBUF) && (Gate_Dee2Cal[i].Fanin < 2))
               {
                Bye();
                PrintSL(ERROR_COLOR,"Descriptor:",i);
                PrintSL(ERROR_COLOR,"Type Id   :",n1);
                Sys_Error("",E_LOW_FANIN);
                return(ERROR);
               }
   break;
   default:Bye();
           PrintSL(ERROR_COLOR,"Descriptor        :",i);
           PrintSL(ERROR_COLOR,"Found Unknown Code:",d);
           PrintSL(ERROR_COLOR,"Total Descriptors :",Num_Descr);
           Sys_Error("",E_UNK_GATE);
           return(ERROR);
  }

  if((Gate_Dee2Cal[i].Fanout==NONE) && (Gate_Dee2Cal[i].Fanin==NONE))
  {
   Bye();
   PrintSL(ERROR_COLOR,"Gate:",i);
   Print2(ERROR_COLOR, "Type:",GType[Gate_Dee2Cal[i].Type&GATE_MASK]);
   Sys_Error("",E_NO_FAN);
   return(ERROR);
  }

  PrintVSL(MESSAGE_COLOR,"Descriptor:",i);
  PrintV2( MESSAGE_COLOR,"Type      :",GType[Gate_Dee2Cal[i].Type&GATE_MASK]);
  PrintVSL(MESSAGE_COLOR,"Fanin     :",Gate_Dee2Cal[i].Fanin);
  PrintVSL(MESSAGE_COLOR,"Fanout    :",Gate_Dee2Cal[i].Fanout);

  if((Max_fanin  < Gate_Dee2Cal[i].Fanin)) Max_fanin=Gate_Dee2Cal[i].Fanin;

  if(Gate_Dee2Cal[i].Fanout)
  {
   Alloc(Gate_Dee2Cal[i].To,Gate_Dee2Cal[i].Fanout,LONG,Mem_Used);
   for(j=NONE; j < Gate_Dee2Cal[i].Fanout; j++)
   {
    fscanf(File[DEE].Ptr,"%*s %ld %*s %*s",&f);  
    FileLine++;
    Gate_Dee2Cal[i].To[j]=f;
   }                                                 /* f = destination */
  }

  if(Gate_Dee2Cal[i].Fanin)
  {
   Alloc(Gate_Dee2Cal[i].From,Gate_Dee2Cal[i].Fanin,LONG,Mem_Used);
   
   if(n1==MMFF)
   {
    if(Gate_Dee2Cal[i].Fanin!=2)
    {
     Bye();
     PrintSL(ERROR_COLOR,"Flip-Flop:",i);
     PrintSL(ERROR_COLOR,"Fanin    :",Gate_Dee2Cal[i].Fanin);
     Sys_Error("",E_SINGLE_IN);
     return(ERROR);
    }
   }

   for(j=NONE; j < Gate_Dee2Cal[i].Fanin; j++)
   {
    fscanf(File[DEE].Ptr,"%*s %ld %*s %*s",&f);
    FileLine++;                                      /* f = from */
    Gate_Dee2Cal[i].From[j]=f;
   }

   if(n1==MMFF) Gate_Dee2Cal[i].Fanin=1; /* second read link is the clock */
  }                                        /* first  read link is real from */
  else Gate_Dee2Cal[i].From=NULL;
 }

 if(Num_FF > MAX_FF)
 {
  Bye();
  Sys_Error("",E_NUM_FF);
  return(ERROR);
 }

 if(Num_Descr!=(Num_Gates+Num_Pi))
 {
  Bye();
  Print2SL(ERROR_COLOR,"Incongruent Circuit Descriptors -> Given:",Num_Descr,"Found:",(Num_Gates+Num_Pi));
  Sys_Error("",E_DES_DES);
  return(ERROR);
 }

 Max_level++;
 
 Clock_Descr=Search_First_FF();

 if(Clock_Descr > Num_Descr) 
 {
  Sys_Error("",E_UNK_CLOCK);
 }

 for(i=NONE;i < Num_Descr;i++)
 {
  if((Max_fanout < Gate_Dee2Cal[i].Fanout) && (i!=Clock_Descr)) 
  {
   Max_fanout=Gate_Dee2Cal[i].Fanout;
   Max_fanoutD=i;
  }
  
  if((Gate_Dee2Cal[i].Fanout > MAX_FAN) && (i!=Clock_Descr))
  {
   Bye();
   PrintSL(ERROR_COLOR,"Found Fanout:",n4);
   PrintSL(ERROR_COLOR,"On Wire     :",i);
   Sys_Error("",E_HIGH_FAN);
   return(ERROR);
  }
 }
 
 if(Create_Po()==ERROR) 
 {
  Bye();
  return(ERROR);
 }
 
 Encode();
 Get_It_Out_All();

 Log("Converted NetList From DEE File",File_Name);

 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Create_Po                                                       */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Create_Po(void)
{
 Phase("Creating True Gate Primary Outputs");
 for(i=NONE,a=NONE,first_pout=NONE;i < Num_Descr;i++)
 {
  if(Gate_Dee2Cal[i].Attr==MMPO)
  {
   PrintVSL(MESSAGE_COLOR,"Resampling POUT-Gate:",i);
   PrintVSL(MESSAGE_COLOR,"Fanout              :",Gate_Dee2Cal[i].Fanout);
   
   if(Gate_Dee2Cal[i].Fanout==NONE) /* An Out Gate */
   {
    Gate_Dee2Cal[i].Fanout=1;
    Alloc(Gate_Dee2Cal[i].To,Gate_Dee2Cal[i].Fanout,LONG,Mem_Used);
    Gate_Dee2Cal[i].To[0]=(Num_Descr+first_pout);
    PrintVSL(MESSAGE_COLOR,"New (To) Pin       :",Gate_Dee2Cal[i].To[0]);
   }
   else /* A Multiple Out Gate */
   {
    Sys_Error("",E_FAN_PO);
    return(ERROR);
   }

   /* For Last Created PO */
   Gate_Dee2Cal[Num_Descr+first_pout].Type   =PO; 
   Gate_Dee2Cal[Num_Descr+first_pout].Attr   =MMPO;
   Gate_Dee2Cal[Num_Descr+first_pout].Fanout =NONE;
   Gate_Dee2Cal[Num_Descr+first_pout].Level  =Max_level;
   Gate_Dee2Cal[Num_Descr+first_pout].Fanin  =1;
   Alloc(Gate_Dee2Cal[Num_Descr+first_pout].From,1,LONG,Mem_Used);
   Gate_Dee2Cal[Num_Descr+first_pout].From[0]=i;
   first_pout++;
   a++;
  }
 }
 Print_Mem_Used(Gate_Dee2Cal,Mem_Used);
 if(a!=Num_Po)
 {
  Print2SL(ERROR_COLOR,"Incongruent Circuit Primary OUT -> Given:",Num_Po,"Found:",a);
  Sys_Error("",E_PO_PO);
  return(ERROR);
 }
 Bar(BAR_COLOR);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_Dee2Cal                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_Dee2Cal(void)
{
 LONG index1=NONE;
 
 if(Gate_Dee2Cal==NULL) return;
 
 for(index1=NONE;index1 < Allocated_Dee2Cal_Descriptors;index1++)
 {
  Free(Gate_Dee2Cal[index1].From,index1);
  Free(Gate_Dee2Cal[index1].To,index1);
 }

 Mem_Used=NONE;
 Free(Gate_Dee2Cal,DB_ROOT);
 Allocated_Dee2Cal_Descriptors=NONE;
 
 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Search_First_FF                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

LONG Search_First_FF(void)
{
 Phase("Searching Clock Descriptor...");
 for(i=NONE;i < Num_Descr; i++)
 {
  if((Gate_Dee2Cal[i].Type==FF) && (Gate_Dee2Cal[Gate_Dee2Cal[i].From[1]].Type==PI))
  {
   Gate_Dee2Cal[Gate_Dee2Cal[i].From[1]].Type=CLOCK;
   Gate_Dee2Cal[Gate_Dee2Cal[i].From[1]].Fanin=NONE;
   Gate_Dee2Cal[Gate_Dee2Cal[i].From[1]].Fanout=NONE;
   PrintSL(MESSAGE_COLOR,"Clock Found As Descriptor:",Gate_Dee2Cal[i].From[1]);
   return(Gate_Dee2Cal[i].From[1]);
  }
 }
 Sys_Error("",E_COMB_CIR);
 Bar(BAR_COLOR);
 return(Num_Descr+10);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Read_Rumenta                                                    */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Read_Rumenta(void)
{
 CHAR String[STRING_LEN];
 
 FileLine=NONE;

 fscanf(File[DEE].Ptr,"%s",version);
 FileLine++;

      if(Cmp(version, "2.0A")) flg_31B=NONE;
 else if(Cmp(version, "3.1B")) flg_31B=TRUE;
 else if(Cmp(version, "4-0"))  flg_31B=TRUE;
 else 
 {
  Sys_Error("",E_DEE_VER);
  return(ERROR);
 }
 
 fscanf(File[DEE].Ptr,"%*s");
 FileLine++;
 fscanf(File[DEE].Ptr,"%*s"); /* 3 */
 FileLine++;
 fscanf(File[DEE].Ptr,"%*s"); /* 4 */
 FileLine++;
 fscanf(File[DEE].Ptr,"%*s"); /* 5 */
 FileLine++;
 fscanf(File[DEE].Ptr,"%ld %*s",&Num_Pi); /* 6 */
 FileLine++;
 fscanf(File[DEE].Ptr,"%ld %*s",&Num_Po); /* 7 */
 FileLine++;
 c=(flg_31B ? 8 : 7);
 for(i=NONE; i < c; i++) 
 {
  fgets(String,STRING_LEN,File[DEE].Ptr);
  FileLine++;
 }
 fscanf(File[DEE].Ptr,"%ld",&Num_Gates);              /* 14 */
 FileLine++;                                   
 c=(flg_31B ? 8 : 5);
 for(i=NONE; i < c; i++) 
 {
  fgets(String,STRING_LEN,File[DEE].Ptr);
  FileLine++;
 }
 fscanf(File[DEE].Ptr,"%ld %*s %*s %*s", &g);       /* 19 */
 FileLine++;
 fscanf(File[DEE].Ptr,"%ld", &e);                   /* 20 */
 FileLine++;
 fscanf(File[DEE].Ptr,"%ld %ld", &a, &b);           /* 21 */
 FileLine++;
 if(flg_31B) {fscanf(File[DEE].Ptr, "%*s %*s");FileLine++;}
 fscanf(File[DEE].Ptr,"%*s %*s");                   /* 22 */
 FileLine++;
 fscanf(File[DEE].Ptr,"%ld",&Max_givout);           /* 23 */
 FileLine++;
 for(i=NONE; i < 7; i++) 
 {
  fgets(String,STRING_LEN,File[DEE].Ptr);
  FileLine++;
 }
 fscanf(File[DEE].Ptr,"%ld %*s %*s %*s", &Num_Descr); /* 30 */
 FileLine++;
 for(i=NONE; i < 4; i++) 
 {
  fgets(String,STRING_LEN,File[DEE].Ptr);
  FileLine++;
 }
 fscanf(File[DEE].Ptr,"%*s %*s");                   /* 34 */
 FileLine++;
 fgets(String,STRING_LEN,File[DEE].Ptr);            /* 34 */
 FileLine++;                              
 fgets(String,STRING_LEN,File[DEE].Ptr);            /* 35 */
 FileLine++;
 for(i=NONE; i < (g+e+a+b+1); i++) 
 {
  fgets(String,STRING_LEN,File[DEE].Ptr);
  FileLine++;
 }
 return(OK);
}
 
/****************************************************************************/
/*                                                                          */
/* Fx Name: Get_It_Out_File                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Get_It_Out_File(void)
{
 FPrint3(CAL,"Dee2Cal",Dee2Cal_Version,"Package_Utility");
 FPrint2(CAL,"File_Name:",File[CAL].Name);
 
 FPrintSL(CAL,"Total_Levels:",(Max_level+1));
 FPrintSL(CAL,"Total_Gates:",Num_Gates);
 FPrintSL(CAL,"Total_FF:",Num_FF);
 FPrintSL(CAL,"Total_PI:",(Num_Pi-1));
 FPrintSL(CAL,"Total_PO:",Num_Po);  
 FPrintSL(CAL,"Total_Descriptors:",Num_Po+Num_Pi+Num_Gates);

 FPrint3(GEN,"Dee2Cal",Dee2Cal_Version,"Package_Utility");
 FPrint2(GEN,"File_Name:",File[GEN].Name);
 
 FPrintSL(GEN,"Total_Levels:",(Max_level+1));
 FPrintSL(GEN,"Total_Gates:",Num_Gates);
 FPrintSL(GEN,"Total_FF:",Num_FF);
 FPrintSL(GEN,"Total_PI:",(Num_Pi-1));
 FPrintSL(GEN,"Total_PO:",Num_Po);  
 FPrintSL(GEN,"Total_Descriptors:",Num_Po+Num_Pi+Num_Gates);

 FPrint(GEN,"Id# SType NType Level Fanin Fanout Fanin_List Fanout_List");
 FPrint(CAL,"Id# SType NType Level Fanin Fanout Fanout_List");
 
 b=NONE;
 
 while(b < Allocated_Dee2Cal_Descriptors)
 {
  FPrint4Dee(GEN,b,GType[Gate_Dee2Cal[b].Type&GATE_MASK],Gate_Dee2Cal[b].Type,Gate_Dee2Cal[b].Level,Gate_Dee2Cal[b].Fanin,Gate_Dee2Cal[b].Fanout);
  FPrint4Dee(CAL,b,GType[Gate_Dee2Cal[b].Type&GATE_MASK],Gate_Dee2Cal[b].Type,Gate_Dee2Cal[b].Level,Gate_Dee2Cal[b].Fanin,Gate_Dee2Cal[b].Fanout);

  for(c=NONE;c < Gate_Dee2Cal[b].Fanin;c++)  
  {
   FPrintOSL(GEN," From",Gate_Dee2Cal[b].From[c]);
  }
  
  for(c=NONE;c < Gate_Dee2Cal[b].Fanout;c++) 
  {
   FPrintOSL(GEN," To",Gate_Dee2Cal[b].To[c]);
   FPrintOSL(CAL," To",Gate_Dee2Cal[b].To[c]);
  }

  FNL(CAL);
  FNL(GEN);
  b++;
 }
} 

/****************************************************************************/
/*                                                                          */
/* Fx Name: Get_It_Out_All                                                  */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Get_It_Out_All(void)
{
 Phase("Saving Datas...");
 Get_It_Out_File();
 
 Release_Dee2Cal();
 Allocated_Dee2Cal_Descriptors=NONE;
 
 Bar(BAR_COLOR);
 PrintSL(MESSAGE_COLOR,"Number  Of Descriptors :",Num_Descr);
 PrintSL(MESSAGE_COLOR,"Number  Of Gates       :",Num_Gates);
 PrintSL(MESSAGE_COLOR,"Number  Of Pi          :",Num_Pi-1);
 PrintSL(MESSAGE_COLOR,"Number  Of Po          :",Num_Po);
 PrintSL(MESSAGE_COLOR,"Number  Of FF          :",Num_FF);
 PrintSL(MESSAGE_COLOR,"Maximum Level          :",Max_level+1);
 PrintSL(MESSAGE_COLOR,"Maximum Fanin          :",Max_fanin);
 PrintSL(MESSAGE_COLOR,"Maximum Fanout         :",Max_fanout);
 PrintSL(MESSAGE_COLOR,"Maximum Fanout On Gate :",Max_fanoutD);
 PrintSL(MESSAGE_COLOR,"Maximum Given Fanout   :",Max_givout);
 PrintSL(MESSAGE_COLOR,"Clock   Descriptor     :",Clock_Descr);
 Bar(BAR_COLOR);
 
 FClose(DEE);
 FClose(CAL);
 FClose(GEN);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Encode                                                          */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Encode(void)
{
 Phase("Clearing And Setting New Type Field");
 PrintSL(MESSAGE_COLOR,"Descriptors To Code:",Allocated_Dee2Cal_Descriptors);

 for(i=NONE;i < Allocated_Dee2Cal_Descriptors;i++)
 {
  Print_Percent(MESSAGE_COLOR,"Coded Descriptors:",(i+1),Allocated_Dee2Cal_Descriptors);
  
  Gate_Dee2Cal[i].Type&=GATE_MASK;
  
  if(Gate_Dee2Cal[i].Fanin==0) (Gate_Dee2Cal[i].Type |= FAN0);
  if(Gate_Dee2Cal[i].Fanin==1) (Gate_Dee2Cal[i].Type |= FAN1);
  if(Gate_Dee2Cal[i].Fanin==2) (Gate_Dee2Cal[i].Type |= FAN2);
  if(Gate_Dee2Cal[i].Fanin==3) (Gate_Dee2Cal[i].Type |= FAN3);
  if(Gate_Dee2Cal[i].Fanin==4) (Gate_Dee2Cal[i].Type |= FAN4);
  if(Gate_Dee2Cal[i].Fanin >4) (Gate_Dee2Cal[i].Type |= FANG);
 }
 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Bye                                                             */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Bye(void)
{
 PrintSL(ERROR_COLOR,"Corrispondent .DEE File Line:",FileLine++);
 
 Release_Dee2Cal();
 FClose(DEE);
 FClose(CAL);
 FClose(GEN);
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
