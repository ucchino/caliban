/*PPU************************************************************************\
*
*    $Source: caliban.c
*  $Revision: 0.7.5
*      $Date: Thu Jul  8 19:30:41 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "sim.h"
# include "caliban.h"
# include "options.h"

/****************************************************************************/

UINT Cols=NONE;

/****************************************************************************/

CHAR FF_Init_Mode[20];

/****************************************************************************/

CHAR Ansi_Mode=FALSE;
CHAR Silent_Mode=FALSE;
CHAR Verbose_Mode=FALSE;
CHAR Reset_Count_Mode=FALSE;
CHAR Multiple_Atpg_Mode=FALSE;
CHAR PatternOrdered_Mode=FALSE;

/****************************************************************************/

CHAR Command_Line_In_Use=TRUE;

/****************************************************************************/
/*                                                                          */
/* Fx Name: main                                                            */
/* Param. : argc, argv                                                      */
/* Return : void status                                                     */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

INT main(INT argc,CHAR *argv[])
{
 Unset_Ansi();
 
 Init_System();

#ifndef DEBUG
 Clear_Screen();
 Title();
 Copyright_Short_Notice();
 NL;
#endif

 Init_Info_Structures();
 Command_Line_In_Use=TRUE;
 
 if(argc==1)
 {
  Menu_Help();
  Command_Line_In_Use=FALSE;
 }
 Menu(argc,argv);
 return(OK);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Init_Info_Structures                                            */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Init_Info_Structures(void)
{
 Release_All_Memory();

 strcpy(Circuit.Name    ,"None");
 strcpy(Circuit.Cir_Name,"None");
 strcpy(Circuit.Fau_Name,"None");
 strcpy(Circuit.Pat_Name,"None");
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Init_System                                                     */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Init_System(void)
{
 Start_Irq();

 Init_Video();
 Clear_All_FName;
 
 strcpy(File[BAT].Name,_Program_Name);
 strcat(File[BAT].Name,File[BAT].Ext);

 if(Fopen(_Program_Name,CFG,CFG,OPEN_R  ,E_OPEN)  ==ERROR) Break(ERROR);
 if(Fopen(_Program_Name,CLF,CLF,CREATE_W,E_CREATE)==ERROR) Break(ERROR);
 
 strcpy(Circuit.Simulation_Stop_Time,"UnkNown");
 strcpy(Circuit.Simulation_Start_Time,"UnkNown");
 strcpy(Circuit.Simulation_Elapsed_Time,"UnkNown");

 PrintV(MESSAGE_COLOR,"-----------------------------------------");
 PrintV(MESSAGE_COLOR,"--- BASE TIME UNIT USED IS CLOCK_TICKS --");
 PrintV(MESSAGE_COLOR,"-----------------------------------------");
 NL;
 PrintVSL(MESSAGE_COLOR,"Setted [RefTime] CLOCK_TICKS Unit Is:",CLOCK_TICKS);

 clock();

 Start_Date(Program_Start_Date);

 Init_CLog();
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Init_Video                                                      */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Init_Video(void)
{
#ifdef __MSDOS__
 union REGS r;
 INT max_row=NONE;

 r.h.ah = 15;
 int86(0x10, &r, &r);

 if(r.h.ah == 80)
 {
  r.h.ah = 0x11;
  r.h.al = 0x12;
  r.h.bl = 0;
  int86(0x10, &r, &r);
  Cols=60;
  max_row=46;
 }
 else
 {
  Cols=30;
  max_row=25;
 }
#else
 Cols=70;
#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Is_Ansi                                                         */
/* Param. : void                                                            */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 08/07/99                                                        */
/*                                                                          */
/****************************************************************************/

INT Is_Ansi(void)
{
#ifdef __MSDOS__
 union REGS r;
 INT oldp;

 r.h.ah = 3;
 r.h.bh = 0;
 int86(0x10, &r, &r);
 oldp = r.x.dx;
 printf("\033[0m");
 r.h.ah = 3;
 r.h.bh = 0;
 int86(0x10, &r, &r);
 if (r.x.dx == oldp) return(TRUE);
 else
 {
  printf("\r                     \r");
 }
 return(FALSE);
#else
 return(TRUE);
#endif
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Program_Limits                                                  */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Program_Limits(void)
{
 Print(MESSAGE_COLOR,"Program/Bit Limits");
 Bar(BAR_COLOR);
 PrintSL(MESSAGE_COLOR,"Max Int_Bits ----------:",BYTE*sizeof(UINT));
 PrintSL(MESSAGE_COLOR,"Max Long_Bits ---------:",BYTE*sizeof(ULONG));
 Bar(BAR_COLOR);
 PrintSL(MESSAGE_COLOR,"Max Parallelism Bits --:",MAXBIT_PARALLELISM);
 PrintSL(MESSAGE_COLOR,"Max Descriptors Bits --:",MAXBIT_GATES);
 PrintSL(MESSAGE_COLOR,"Max Faults Bits -------:",MAXBIT_FAULTS);
 PrintSL(MESSAGE_COLOR,"Max Patterns Bits -----:",MAXBIT_PATTERNS);
 PrintSL(MESSAGE_COLOR,"Max Fanin/Fanout Bits -:",MAXBIT_FAN);
 PrintSL(MESSAGE_COLOR,"Max Genes Bits --------:",MAXBIT_GENES);
 PrintSL(MESSAGE_COLOR,"Max Levels Bits -------:",MAXBIT_LEVEL);
 PrintSL(MESSAGE_COLOR,"Max FF Bits -----------:",MAXBIT_FF);
 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Settings                                                        */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Ck_Settings(void)
{
 Bar(BAR_COLOR);

#ifdef FS32BIT
 Print(MESSAGE_COLOR,  "FS32BIT         <++Active>");
#else
 Print(MESSAGE_COLOR,  "FS32BIT         <--Inactive>");
#endif

#ifdef PPU_VISIT
 Print(MESSAGE_COLOR,  "PPU_VISIT       <++Active>");
#else
 Print(MESSAGE_COLOR,  "PPU_VISIT       <--Inactive>");
#endif

#ifdef TWO_LEVEL_VISIT
 Print(MESSAGE_COLOR,  "TWO_LEVEL_VISIT <++Active>");
#else
 Print(MESSAGE_COLOR,  "TWO_LEVEL_VISIT <--Inactive>");
#endif

#ifdef THRESHOLD
 Print(MESSAGE_COLOR,  "THRESHOLD       <++Active>");
 PrintSL(MESSAGE_COLOR,"THRESHOLD ======>",THRESHOLDVAL);
#else
 Print(MESSAGE_COLOR,  "THRESHOLD       <--Inactive>");
 PrintSL(MESSAGE_COLOR,"THRESHOLD ======>",THRESHOLDVAL);
#endif

#ifdef FAULT_QSORT
 Print(MESSAGE_COLOR,  "FAULT_QSORT     <++Active>");
#else
 Print(MESSAGE_COLOR,  "FAULT_QSORT     <--Inactive>");
#endif

#ifdef REVERSE_SORT
 Print(MESSAGE_COLOR,  "REVERSE_SORT    <++Active>");
#else
 Print(MESSAGE_COLOR,  "REVERSE_SORT    <--Inactive>");
#endif

#ifdef GOODFSIM
 Print(MESSAGE_COLOR,  "GOODFSIM        <++Active>");
#else
 Print(MESSAGE_COLOR,  "GOODFSIM        <--Inactive>");
#endif

#ifdef VERBOSESIM
 Print(MESSAGE_COLOR,  "VERBOSESIM      <++Active>");
#else
 Print(MESSAGE_COLOR,  "VERBOSESIM      <--Inactive>");
#endif

 NL;

 if(Is_Ansi()) {PrintO(MESSAGE_COLOR, "ANSI MODE SETTABLE");}
 else          {PrintO(MESSAGE_COLOR, "ANSI MODE UNSETTABLE");}

 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Circular                                                        */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Circular(void)
{
 CHAR Manifest[]=_Program_Name"(c) Is A PPU(c) Product... Call:"_Phone" To Order Your Own Copy Of "_Program_Name"(c)... An Integrated Parallel Event-Driven Fault Simulator With: Fault Dropping, 1-2 Level Fault Removal..............                                                        HAVE FUN......................                                              ";
 INT  index1=NONE,index2=NONE,index3=NONE,len;

 len=strlen(Manifest);
 
 unlink(File[BAT].Name);

 if((Silent_Mode==TRUE) || (Command_Line_In_Use==TRUE))
 {
  CallMe();
  SetColor(Normal);
  exit(OK);
 }

#ifdef DEBUG
 CallMe();
 SetColor(Normal);
 exit(OK);
#endif

#ifdef STILL_TESTING
 CallMe();
 SetColor(Normal);
 exit(OK); 
#endif

 ForEver()
 {
  OL;
  Rotate();
  index2=(++index2%len);
  for(index1=NONE;index1 < Cols;index1++) 
  {
   PrintC("",Manifest[(index2+index1)%len]);
   for(index3=NONE;index3 < 1000;index3++);
  }
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Rotate                                                          */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Rotate(void)
{
 # define MAXX_CIRCLE 4

 static INT index1=NONE;
 CHAR       rotate_vett[MAXX_CIRCLE]={'|','/','-','\\'};
  
 printf("(%c)",rotate_vett[index1++]);
 if(index1 == MAXX_CIRCLE) index1=NONE;
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Copyright_Short_Notice                                          */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Copyright_Short_Notice(void)
{
 Print(MESSAGE_COLOR,"******************************************************************");
 Print(MESSAGE_COLOR,"*                                                                *");
 Print(MESSAGE_COLOR,"* THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR *");
 Print(MESSAGE_COLOR,"* IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED *");
 Print(MESSAGE_COLOR,"* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR     *");
 Print(MESSAGE_COLOR,"* PURPOSE. THERE IS NOT ANY WARRANTY...!                         *");
 Print(MESSAGE_COLOR,"*                                                                *");
 Print(MESSAGE_COLOR,"******************************************************************");
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Copyright_Long_Notice                                           */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Copyright_Long_Notice(void)
{
 Print(MESSAGE_COLOR,"******************************************************************************");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"* All rights reserved.                                                       *");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND    *");
 Print(MESSAGE_COLOR,"* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE      *");
 Print(MESSAGE_COLOR,"* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE *");
 Print(MESSAGE_COLOR,"* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE   *");
 Print(MESSAGE_COLOR,"* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL *");
 Print(MESSAGE_COLOR,"* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    *");
 Print(MESSAGE_COLOR,"* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)      *");
 Print(MESSAGE_COLOR,"* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT *");
 Print(MESSAGE_COLOR,"* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY  *");
 Print(MESSAGE_COLOR,"* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF     *");
 Print(MESSAGE_COLOR,"* SUCH DAMAGE.                                                               *");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"******************************************************************************");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"* Redistribution and use in source and binary forms are permitted provided   *");
 Print(MESSAGE_COLOR,"* that:                                                                      *");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"* (1) Redistributions of source code must retain the above copyright         *");
 Print(MESSAGE_COLOR,"*     notice, this list of conditions and the following disclaimer.          *");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"* (2) Redistributions in binary form must reproduce the above copyright      *");
 Print(MESSAGE_COLOR,"*     notice, this list of conditions and the following disclaimer in the    *");
 Print(MESSAGE_COLOR,"*     documentation and/or other materials provided with the distribution.   *");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"* (3) All advertising materials mentioning features or use of this software  *");
 Print(MESSAGE_COLOR,"*     must display the following acknowledgement:                            *");
 Print(MESSAGE_COLOR,"*        This product includes software developed by                         *");
 Print(MESSAGE_COLOR,"*        Ucchino Giuseppe Pier Paolo <s73259@cclinf.polito.it>               *");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"* (4) Neither the name of the Author nor the names of its contributors       *");
 Print(MESSAGE_COLOR,"*     may be used to endorse or promote products derived from this software  *");
 Print(MESSAGE_COLOR,"*     without specific prior written permission.                             *");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"* (5) Distributions may not be sold for profit (WITHOUT EXPRESSED WRITTEN    *");
 Print(MESSAGE_COLOR,"*     AUTHOR PERMISSION) on any physical media  such as disks, tapes,        *");
 Print(MESSAGE_COLOR,"*     CD-ROMS, DVD, and other, and on any non-physical media as Lan,Wan, ecc.*");
 Print(MESSAGE_COLOR,"*                                                                            *");
 Print(MESSAGE_COLOR,"******************************************************************************");
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Init_Info                                                       */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Info(void)
{                                      
 Print(FBWhite,ALTERNATED_BAR);
 Print(FBGreen,"Product   : "); PrintTxt(BBRed     FWhite        ,_Program_Name"(c) - " _Version);
 Print(FBGreen,"Compiler  : "); PrintTxt(BBRed     FWhite        ,"Compiled With: " _Compiler);
 Print(FBGreen,"Link Dates: "); PrintTxt(BBRed     FWhite        ,"Compiled On "System " In "__DATE__" ---- " __TIME__);
 Print(FBGreen,"What's    : "); PrintTxt(BBRed     FWhite        ,_Description);
 Print(FBGreen,"Algorithm : "); PrintTxt(BBRed     FWhite        ,"Ideated By " _PPU2);
 Print(FBWhite,ALTERNATED_BAR);
 Print(FBGreen,"Who       : "); PrintTxt(Blink     BBlue FBWhite ,_Who);
 Print(FBWhite,ALTERNATED_BAR);
 Print(FBGreen,"Why       : "); PrintTxt(BBRed     FWhite        ,_Why);
 Print(FBGreen,"Where     : "); PrintTxt(BBRed     FWhite        ,_Where);
 Print(FBGreen,"Find Him  : "); PrintTxt(BBRed     FWhite        ,_PPUMail " Or "_PPUMail2 "---");
 Print(FBGreen,"Web Info  : "); PrintTxt(BBRed     FWhite        ,_PPUWww);
 Print(FBWhite,ALTERNATED_BAR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Author_Datas                                                    */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Author_Datas(void)
{
 CHAR* Color=FCyan;
 Print(Color,HEAVY_BAR);
 Print(Color,"###                                                                ###");
 Print(Color,"#                                        .~'~.                      ##");
 Print(Color,"#      The Author Is...                 |     |                      #");
 Print(Color,"#                                       |()-()|                      #");
 Print(Color,"#             ______________________oooo--(_)--oooo_______________   #");
 Print(Color,"#            /<___________________________________________________>  #");
 Print(Color,"#           / /                                                    > #");
 Print(Color,"#          / / _:   __  ____:  ____:  __   _:  __      __  ____:   / #");
 Print(Color,"#         / / / +- / / / ___+-/ ___+-/ /__/ +-/ / /;  / / /    +- /  #");
 Print(Color,"#        / / / /: / / / /   :/ /   :/ ___  /:/ / /  ;/ / /  /  : /   #");
 Print(Color,"#       / / / /__/ / / /___ / /___ / /  / / / / / /;  / /  /  / /    #");
 Print(Color,"#      / / <______/ <_____/<_____/ ;/   ;/  ;/  ;/  ;/ <_____/ /     #");
 Print(Color,"#     / / _____  _____  ______________________________________/      #");
 Print(Color,"#    / / / __ / / __ / __/                             <_____<       #");
 Print(Color,"#   / / / ___/ / ___/ __/   "); SetColor(BBRed);PrintTxt(FBYellow,_PPUMail); PrintTxt(Color,"    <_____<        #");
 Print(Color,"#  / / / /    / /    __/ "   ); SetColor(BBRed);PrintTxt(FBYellow,_PPUMail2);PrintTxt(Color,"   <_____<         #");
 Print(Color,"# < / <_/ @  <_/ @  __/_____________________________<_____<          #");
 Print(Color,"#  <                                                      /          #");
 Print(Color,"#   <____________________________________________________/           #");
 Print(Color,"#                                    .oooO   Oooo.                   #");
 Print(Color,"#                                    (   )   (   )                   #");
 Print(Color,"##                                    : (     ) /                   ##");
 Print(Color,"###                                    :_)   (_/                   ###");
 Print(Color,HEAVY_BAR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Call_Me                                                         */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void CallMe(void)
{
 CHAR* Color=FBMagenta;
 NL;
 Print(Color,SLASH_BAR);
 Print(Color,"/");
 Print(Color,"/  For Any Question, Suggestion, Comment, Problem.......");
 Print(Color,"/");
 Print(Color,"/  Contact Author At: ");SetColor(BBRed);PrintTxt(FBYellow,_PPUMail2);
 Print(Color,"/");
 Print(Color,SLASH_BAR);
 Print(Color," ");
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Title                                                           */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Title(void)
{
 CHAR* Color=FBWhite;
 Print(Color,SLASH_BAR);
 Print(Color,"/////                                                                /");
 Print(Color,"////                       :                                     :   /");
 Print(Color,"///     ______  _____   ___+    ___  ______   _____   _      ____+   /");
 Print(Color,"//     <  ___/ <  __ : <  /3+- <  / <  ___ : <  __ : < <    / __/3+- /");
 Print(Color,"/      / /     / / / / / / +   / /  / /__/ / / / / / /  :  / /   +   /");
 Print(Color,"/     / /     /  </ / / /  :  / /  / ___  / /  </ / / /-+-/ /    :   /");
 Print(Color,"/    / /     / /<  / / /     / /  / /  / / / /<  / / /  :  /         /");
 Print(Color,"/   / /___  / / / / / /___  / /  / /__/ / / / / / / /   / /          /");
 Print(Color,"/  <_____/ <_/ <_/ <_____/ <_/  <______/ <_/ <_/ <_/   <_/           /");
 Print(Color,"/                                                                    /");
 Print(Color,"/   An Event-Driven Fault Simulator For Large Sequential Circuits.  //");
 Print(Color,"/   Parallel 3Values-Logic Fault Dropping & 1-2 Level Fau Removal  ///");
 Print(Color,"/                                                                 ////");
 Print(Color,"/                                                                /////");
 Print(Color,SLASH_BAR);
 Print(Color," ");
 Print(FBRed ,_VersionP "\nLinked On: "__DATE__" - "__TIME__"\n");
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Algo                                                            */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Algo(void)
{
 UINT Tim=NONE;
 CHAR String[STRING_LEN];

 if(Fopen("caliban",ALG,ALG,OPEN_R,E_OPEN)==ERROR) 
 {
  Print(ERROR_COLOR,"Sorry, System Cannot Load Algorithm File!");
  Print(ERROR_COLOR,"Please,Check Its Presence In The Current Directory...");
  return;
 }
 
 Phase("Loading Algorithm File...");
 
 Bar(BAR_COLOR);
 NL;

 Tim=NONE;
 gets(String);
 while(!feof(File[ALG].Ptr))
 {
  Strset(String);
  fgets(String,STRING_LEN,File[ALG].Ptr);
  printf("%s",String);

  Tim++;
  if(Tim==20)
  {
   Tim=NONE;
   Strset(String);
   Print(FBWhite,"Press Return To Continue, [q/Q] To Stop:>");
   gets(String);
   if(Cmp(String,"q") || Cmp(String,"Q")) break;
  }
 }
 Bar(BAR_COLOR);
 FClose(ALG);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Man                                                             */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Man(void)
{
 UINT Tim=NONE;
 CHAR String[STRING_LEN];

 if(Fopen("caliban",MAN,MAN,OPEN_R,E_OPEN)==ERROR) 
 {
  Print(ERROR_COLOR,"Sorry, System Cannot Load Manual File!");
  Print(ERROR_COLOR,"Please,Check Its Presence In The Current Directory...");
  return;
 }
 
 Phase("Loading Manual File...");
 
 Bar(BAR_COLOR);
 NL;

 Tim=NONE;
 gets(String);
 while(!feof(File[MAN].Ptr))
 {
  Strset(String);
  fgets(String,STRING_LEN,File[MAN].Ptr);
  printf("%s",String);

  Tim++;
  if(Tim==20)
  {
   Tim=NONE;
   Strset(String);
   Print(FBWhite,"Press Return To Continue, [q/Q] To Stop:>");
   gets(String);
   if(Cmp(String,"q") || Cmp(String,"Q")) break;
  }
 }
 Bar(BAR_COLOR);
 FClose(MAN);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Mark                                                            */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 30/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Mark(void)
{
 /* Mark File While Debugging Code */
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
