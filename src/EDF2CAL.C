/*PPU************************************************************************\
*
*    $Source: edf2cal.c
*  $Revision: 0.4.3
*      $Date: Thu Jul  8 19:30:46 1999 
*
\************************************************************************PPU*/

                       /*****************************************************/
                       /*                                                   */
# include "data2.h"    /* DONT' MOVE FROM THIS POSITION. MUST BE PLACED     */
                       /* BEFORE GATES.H INCLUDED BY LIBS.H -----------     */
                       /*                                                   */
                       /*****************************************************/
# include "libs.h"
# include "edf2cal.h"

/****************************************************************************/
/*** Giovanni's Global Data --->  squillero@polito.it (ask him...) **********/
/****************************************************************************/

int n_pi        =NONE;
int n_po        =NONE;
int n_ff        =NONE;
int n_descr     =NONE;
int n_gates     =NONE;
int max_level   =NONE;

/****************************************************************************/
/****************************************************************************/

int more_po     =NONE;

int ck_n_pi     =NONE;
int ck_n_po     =NONE;
int ck_n_ff     =NONE;
int ck_n_descr  =NONE;
int ck_max_level=NONE;

/****************************************************************************/

GATE_TYPE   Type=NONE;

/****************************************************************************/

DESCRIPTOR *descr;

int *pi_array;
int *po_array;
int *ppi_array;
int *ppo_array;

/****************************************************************************/

LONG Clock_Id=NONE;

LONG Num_Of_Levels=NONE;

/****************************************************************************/

CHAR* Edf_Type[]={"And",        /* 0  */
                  "Nand",       /* 1  */
                  "Or",         /* 2  */
                  "Nor",        /* 3  */
                  "Buffer",     /* 4  */
                  "Not",        /* 5  */
                  "Exor",       /* 6  */
                  "Nexor",      /* 7  */
                  "Logic0",     /* 8  */
                  "Logic1",     /* 9  */
                  "FF",         /* 10 */
                  "CallBack"};  /* 11 */

/****************************************************************************/
/* GATES ********************************************************************/
/****************************************************************************/

# define EDF_AND       0
# define EDF_NAND      1
# define EDF_OR        2
# define EDF_NOR       3
# define EDF_BUFFER    4
# define EDF_NOT       5
# define EDF_EXOR      6
# define EDF_NEXOR     7
# define EDF_LOGIC0    8
# define EDF_LOGIC1    9
# define EDF_FF        10
# define EDF_CALLBACK  11

/****************************************************************************/
/* ATTRIBUTES ***************************************************************/
/****************************************************************************/

# define EDF_INTERNAL  0
# define EDF_PI        1 
# define EDF_PO        2

/****************************************************************************/
/* FF ATTRIBUTES ************************************************************/
/****************************************************************************/

# define EDF_FFD       0
# define EDF_FFDR      1
# define EDF_FFDP      2
# define EDF_FFDRP     3

/****************************************************************************/

CHAR* Edf_Attr[]={"Internal Gate","Pi","Po"};

/****************************************************************************/
/*** PPU's Bright Code -> ppu@cclinf.polito.it (for any question...)      ***/
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* Fx Name: Edf2Cal                                                         */
/* Param. : char*                                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

FX_RET Edf2Cal(CHAR* File_Name)
{
 LONG Fin=NONE;
 LONG Fout=NONE;
 LONG index1=NONE;
 
 if(Fopen(File_Name,EDF,EDF,OPEN_R,E_OPEN)==ERROR)     {return(ERROR);}
 if(Fopen(File_Name,GEN,GEN,CREATE_W,E_CREATE)==ERROR) {FClose(EDF);return(ERROR);}
 if(Fopen(File_Name,CAL,CAL,CREATE_W,E_CREATE)==ERROR) {FClose(EDF);FClose(GEN);return(ERROR);}

 Phase("Loading EDF To Convert To CAL Format\n");
 
 create(File[EDF].Name);
 
 if(descr==NULL)
 {
  Sys_Error("",E_EDF_LOAD);
  FClose(EDF);
  FClose(GEN);
  FClose(CAL);
  return(ERROR);
 }

 for(index1=NONE;index1 < n_descr;index1++)
 {
  BarV(BAR_COLOR);
  PrintV2(Normal, "Gate NetId :",descr[index1].to_name);
  PrintV2(Normal, "Gate Name  :",descr[index1].name);
  PrintVSL(Normal,"Gate Index :",index1);
  PrintVSL(Normal,"Gate Id    :",descr[index1].gate_id);
  PrintV2(Normal, "Gate Type  :",Edf_Type[(SCHAR)descr[index1].type]);
  PrintV2(Normal, "Gate Attr  :",Edf_Attr[(SCHAR)descr[index1].attr]);
  PrintVSL(Normal,"Gate Fanin :",descr[index1].fanin);
  PrintVSL(Normal,"Gate Fanout:",descr[index1].fanout);

  for(Fout=NONE;Fout < descr[index1].fanout;Fout++)
  {
   PrintV2SL(Normal,"To Index:",Fout,"Fanout To :",descr[index1].to[Fout]);
  }
  for(Fin=NONE;Fin < descr[index1].fanin;Fin++)
  {
   PrintV2SL(Normal,"Fr Index:",Fin,"Fanin  Fr :",descr[index1].from[Fin]);
  }
 }

 n_pi--;
 Num_Of_Levels=(max_level+1);
 n_gates=(n_descr-(n_po+n_pi));

 FPrint3(CAL,"Edf2Cal",Edf2Cal_Version,"Package_Utility");
 FPrint2(CAL,"File_Name:",File[CAL].Name);
 FPrintSL(CAL,"Total_Levels:",Num_Of_Levels+1);
 FPrintSL(CAL,"Total_Gates:",n_gates+n_po-1);
 FPrintSL(CAL,"Total_FF:",n_ff);
 FPrintSL(CAL,"Total_PI:",n_pi);
 FPrintSL(CAL,"Total_PO:",n_po);
 FPrintSL(CAL,"Total_Descriptors:",(n_descr+n_po));

 FPrint3(GEN,"Edf2Cal",Edf2Cal_Version,"Package_Utility");
 FPrint2(GEN,"File_Name:",File[GEN].Name);
 FPrintSL(GEN,"Total_Levels:",Num_Of_Levels+1);
 FPrintSL(GEN,"Total_Gates:",n_gates+n_po-1);
 FPrintSL(GEN,"Total_FF:",n_ff);
 FPrintSL(GEN,"Total_PI:",n_pi);
 FPrintSL(GEN,"Total_PO:",n_po);
 FPrintSL(GEN,"Total_Descriptors:",(n_descr+n_po));

 FPrint(GEN,"Id# SType NType Level Fanin Fanout Fanin_List Fanout_List");
 FPrint(CAL,"Id# SType NType Level Fanin Fanout Fanout_List");
 
 Phase("Checking Clock...");
 
 more_po=NONE;
 
 Clock_Id=Find_Clock();

 if(Clock_Id > n_descr) 
 {
  Release_Edf2Cal();
  Sys_Error("",E_UNK_CLOCK);
  FClose(EDF);
  FClose(GEN);
  FClose(CAL);
  return(ERROR);
 }

 Phase("Resampling Datas...");

 for(index1=NONE;index1 < n_descr;index1++)
 {
  if(ck_max_level < descr[index1].level) ck_max_level=descr[index1].level;
  
  if(Verbose_Mode) {Print_PercentV(MESSAGE_COLOR,"Coded Descriptors:",(index1+1),n_descr);}
  else             {Print_Percent( MESSAGE_COLOR,"Coded Descriptors:",(index1+1),n_descr);}


  if((descr[index1].attr==EDF_INTERNAL) || (descr[index1].attr==EDF_PO) || (descr[index1].attr==EDF_PI))
  {
   switch(descr[index1].type)
   {
    case(EDF_AND)     :Type=AND;
    break;
    case(EDF_NAND)    :Type=NAND;
    break;
    case(EDF_OR)      :Type=OR;
    break;
    case(EDF_NOR)     :Type=NOR;
    break;
    case(EDF_BUFFER)  :Type=BUFFER;
    break;
    case(EDF_NOT)     :Type=NOT;
    break;
    case(EDF_EXOR)    :Type=EXOR;
    break;
    case(EDF_NEXOR)   :Type=NEXOR;
    break;
    case(EDF_LOGIC0)  :Type=UNKNOWN0;
    break;
    case(EDF_LOGIC1)  :Type=UNKNOWN1;
    break;
    case(EDF_FF)      :Type=FF;
                       descr[index1].fanin=1;
    break;
    case(EDF_CALLBACK):Type=UNKNOWN2;
    break;
    default           :Type=UNKNOWN3;
    break;
   }
  }

  if(descr[index1].attr==EDF_PO)
  {
   if(descr[index1].fanout)
   {
    Release_Edf2Cal();
    Sys_Error("",E_FAN_PO);
    FClose(EDF);
    FClose(GEN);
    FClose(CAL);
    return(ERROR);
   }
   ck_n_po++;
   descr[index1].fanout=1;
  }

  if(descr[index1].attr==EDF_PI)
  {
   if((descr[index1].type!=EDF_BUFFER) || (descr[index1].fanin!=NONE))
   {
    Release_Edf2Cal();
    Sys_Error("",E_EDF_PI);
    FClose(EDF);
    FClose(GEN);
    FClose(CAL);
    return(ERROR);
   }
   Type=PI;
   ck_n_pi++;
   descr[index1].fanin=NONE;
  }

  if(index1==Clock_Id)
  {
   Type=CLOCK;
   descr[index1].fanin=NONE;
   descr[index1].fanout=NONE;
  }

  if(descr[index1].fanin==0) (Type |= FAN0);
  if(descr[index1].fanin==1) (Type |= FAN1);
  if(descr[index1].fanin==2) (Type |= FAN2);
  if(descr[index1].fanin==3) (Type |= FAN3);
  if(descr[index1].fanin==4) (Type |= FAN4);
  if(descr[index1].fanin >4) (Type |= FANG);

  FPrint4Gen(CAL,index1,GType[Type&GATE_MASK],Type,descr[index1].level,descr[index1].fanin,descr[index1].fanout);
  FPrint4Gen(GEN,index1,GType[Type&GATE_MASK],Type,descr[index1].level,descr[index1].fanin,descr[index1].fanout);
  
  for(Fin=NONE;Fin < descr[index1].fanin;Fin++)
  {
   FPrintOSL(GEN," From",descr[index1].from[Fin]);
  }

  for(Fout=NONE;Fout < descr[index1].fanout;Fout++)
  {
   if(descr[index1].attr==EDF_PO)
   {
    FPrintOSL(GEN," To",n_descr+more_po);
    FPrintOSL(CAL," To",n_descr+more_po);
    more_po++;
   }
   else
   { 
    FPrintOSL(GEN," To",descr[index1].to[Fout]);
    FPrintOSL(CAL," To",descr[index1].to[Fout]);
   } 
  }
  FNL(CAL);
  FNL(GEN);
 }

 ck_n_pi--;
 ck_n_descr=n_po+n_pi+n_gates;

/****************************************************************************/

 Phase("Checking Loaded .EDF Circuit...");

 if((ck_n_po!=n_po) || (n_po!=more_po))
 {
  Release_Edf2Cal();
  PrintSL(ERROR_COLOR,"Given Number Of PO:",n_po);
  PrintSL(ERROR_COLOR,"Calculated Check  :",ck_n_po);
  PrintSL(ERROR_COLOR,"Calculated More   :",more_po);
  Sys_Error("",E_PO_PO);
  return(ERROR);
 }
 Print(MESSAGE_COLOR,"PO Are OK!");

/****************************************************************************/

 if(ck_n_pi!=n_pi)
 {
  Release_Edf2Cal();
  PrintSL(ERROR_COLOR,"Given Number Of PI:",n_pi);
  PrintSL(ERROR_COLOR,"Calculated        :",ck_n_pi);
  Sys_Error("",E_PI_PI);
  return(ERROR);
 }
 Print(MESSAGE_COLOR,"PI Are OK!");

/****************************************************************************/

 if(ck_n_descr!=n_descr)
 {
  Release_Edf2Cal();
  PrintSL(ERROR_COLOR,"Given Number Of Descr:",n_descr);
  PrintSL(ERROR_COLOR,"Calculated           :",ck_n_descr);
  Sys_Error("",E_GATES_GATES);
  return(ERROR);
 }
 Print(MESSAGE_COLOR,"Gates Are OK!");

/****************************************************************************/

 if(ck_max_level!=Num_Of_Levels-1)
 {
  Release_Edf2Cal();
  PrintSL(ERROR_COLOR,"Given Number Of Levels:",Num_Of_Levels);
  PrintSL(ERROR_COLOR,"Calculated            :",ck_max_level);
  Sys_Error("",E_LEVELS_LEVELS);
  return(ERROR);
 }
 Print(MESSAGE_COLOR,"Levels Are OK!");

/****************************************************************************/

 Phase("Creating True Gate Primary Outputs");

 for(index1=NONE,more_po=NONE;index1 < n_descr;index1++)
 {
  if(descr[index1].attr==EDF_PO)
  {
   Type =PO;
   Type|=FAN1;

   FPrint4Gen(CAL,(n_descr+more_po),GType[Type&GATE_MASK],Type,Num_Of_Levels,1,0);
   FPrint4Gen(GEN,(n_descr+more_po),GType[Type&GATE_MASK],Type,Num_Of_Levels,1,0);

   FPrintOSL(GEN," From",index1);
   
   more_po++;

   FNL(CAL);
   FNL(GEN);
  }
 }

 Release_Edf2Cal();
 
 Bar(BAR_COLOR);
 PrintSL(MESSAGE_COLOR,"Number  Of Descriptors :",(n_descr+n_po));
 PrintSL(MESSAGE_COLOR,"Number  Of Gates       :",n_gates);
 PrintSL(MESSAGE_COLOR,"Number  Of Pi          :",n_pi);
 PrintSL(MESSAGE_COLOR,"Number  Of Po          :",n_po);
 PrintSL(MESSAGE_COLOR,"Number  Of FF          :",n_ff);
 PrintSL(MESSAGE_COLOR,"Maximum Level          :",(Num_Of_Levels+1));
 Bar(BAR_COLOR);

 FClose(EDF);
 FClose(GEN);
 FClose(CAL);

 Bar(BAR_COLOR);
 return(OK);
}
 
/****************************************************************************/
/*                                                                          */
/* Fx Name: Find_Clock                                                      */
/* Param. : void                                                            */
/* Return : long                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

LONG Find_Clock(void)
{
 LONG index1=NONE;

 Phase("Searching Clock Descriptor...");

 for(index1=NONE;index1 < n_descr;index1++)
 {
  if((descr[index1].type==EDF_FF) && (descr[descr[index1].from[1]].attr==EDF_PI))
  {
   descr[descr[index1].from[1]].fanin=NONE;
   descr[descr[index1].from[1]].fanout=NONE;
   PrintSL(MESSAGE_COLOR,"Clock Found As Descriptor:",descr[index1].from[1]);
   return(descr[index1].from[1]);
  }
 }
 Sys_Error("",E_COMB_CIR);
 Bar(BAR_COLOR);
 return(n_descr+10);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Release_Edf2Cal                                                 */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Release_Edf2Cal(void)
{
 LONG index1=NONE;

 if(descr==NULL) return;

 for(index1=NONE;index1 < n_descr;index1++)
 {
  Free(descr[index1].to,index1);
  Free(descr[index1].from,index1);
 }
 Free(descr,DB_ROOT);
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
