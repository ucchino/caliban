/*PPU************************************************************************\
*
*    $Source: options.c
*  $Revision: 0.2.10
*      $Date: Thu Jul  8 19:31:01 1999 
*
\************************************************************************PPU*/

# include "libs.h"

# include "sim.h"
# include "fauman.h"
# include "gengis.h"
# include "caliban.h"
# include "dee2cal.h"
# include "edf2cal.h"
# include "options.h"
# include "load_cir.h"
# include "load_fau.h"
# include "load_pat.h"

/****************************************************************************/

# define Dummy_Option  "                 "
# define Option_Lenght (sizeof(CHAR)*strlen(Dummy_Option))

# define Print_Option(Vector,Option_String) {INT Lenght=(Option_Lenght-strlen(Option_String)); if(Lenght < 0) Lenght=0;Strset(Vector);strcpy(Vector,Option_String);strncat(Vector,Dummy_Option,(sizeof(CHAR)*Lenght));}

/****************************************************************************/

CHAR *Menu_Help_[]=
{"_____________________________________________________________",
 "|                                                           |",
 "| -=-=-=-=-=-=-=-=-=-=-=()=-=-=-=()=-=-=-=-=-=-=--=-=-=-=-  |",
 "|                                                           |",
 "| An Event-Driven Fault Simulator & ATPG For Large Sequent. |",
 "| Sincronous Circuits With Fault Dropping & PPU's Algorithm |",
 "|___________________________________________________________;",
 "|                                                            ",
 "| " Load_All          ")= Load    All Necessary For Sim      ",
 "| " Load_Circuit      ")= Load    Circuit To Simulate (.CAL) ",
 "| " Load_Faults       ")= Load    Gman    FaultList   (.FAU) ",
 "| " Load_Pattern      ")= Load    Pattern To Test     (.INP) ",
 "|------------------------------------------------------------",
 "| " Faumanager        ")= Create  a Fault List               ",
 "| " Create_New_Circuit")= Create  a Sequential Circuit       ",
 "|------------------------------------------------------------",
 "| " Circuit_Levels    ")= Gates   For Levels                 ",
 "| " Convert_Dee       ")= Convert Dee To Cal format          ",
 "| " Convert_Edf       ")= Convert Edf To Cal format          ",
 "|------------------------------------------------------------",
 "| " Atpg1             ")= Start   ATPG With 1 Reset          ",
 "| " Atpg2             ")= Start   ATPG With Multiple Resets  ",
 "| " Simulate          ")= Start   Fault Simulation           ",
 "|------------------------------------------------------------", 
 "| " Init_FF           ")= InitFF  With Specified Value       ",
 "| " File_Init_FF      ")= InitFF  With Specified File (.FFI) ",
 "|------------------------------------------------------------",
 "| " Simulation_Info   ")= System  General Simulation Info    ",
 "| " Reset_Count       ")= Counts  Reset Signal as Pattern (!)",
 "| " PatternOrdered    ")= Pattern Ordered Mode (.DLF)        ",
 "| " Verbose           ")= Verbose Mode (OutPut Is Very Long) ",
 "|------------------------------------------------------------",
 "| " Memory_Status     ")= Memory  And Files Use              ",
 "| " Ansi              ")= Ansi    Mode (Frigo...)            ",
 "| " Run               ")= Command To Operative System        ",
 "| " Dir_List          ")= Show    Files In Current Directory ",
 "|------------------------------------------------------------",
 "| " Author            ")= Who is  Author                     ",
 "| " About             ")= About   Program                    ",
 "| " Limits            ")= Limits  For Program                ",
 "| " Settings          ")= Current Compilation Settings       ",
 "| " Copyright_Notice  ")= About   CopyRights <<READ FIRST>>  ",
 "| " Algorithm         ")= About   Program's Features         ",
 "| " Manual            ")= About   Caliban Options/Use        ",
 "|------------------------------------------------------------",
 "| " Short_Help        ")= User    Help                       ",
 "| " Quit              ")= Quit    Program                    ",
 "|____________________________________________________________",
 "                                                             ",""};

/****************************************************************************/
/*                                                                          */
/* Fx Name: Set_Option                                                      */
/* Param. : char*                                                           */
/* Return : int                                                             */
/* Written: By PPU !                                                        */
/* LastMod: 29/06/99                                                        */
/*                                                                          */
/****************************************************************************/

INT Set_Option(CHAR* Opt)
{
       if(Cmp(Opt,About)             || Cmp(Opt,Short_About)              || Cmp(Opt,Long_About              )) return(About_Option);
  else if(Cmp(Opt,Silent)            || Cmp(Opt,Short_Silent)             || Cmp(Opt,Long_Silent             )) return(Silent_Option);
  else if(Cmp(Opt,Load_Circuit)      || Cmp(Opt,Short_Load_Circuit)       || Cmp(Opt,Long_Load_Circuit       )) return(Load_Circuit_Option);
  else if(Cmp(Opt,Load_Pattern)      || Cmp(Opt,Short_Load_Pattern)       || Cmp(Opt,Long_Load_Pattern       )) return(Load_Pattern_Option);
  else if(Cmp(Opt,Load_Faults)       || Cmp(Opt,Short_Load_Faults)        || Cmp(Opt,Long_Load_Faults        )) return(Load_Faults_Option);
  else if(Cmp(Opt,Load_All)          || Cmp(Opt,Short_Load_All)           || Cmp(Opt,Long_Load_All           )) return(Load_All_Option);
  else if(Cmp(Opt,PatternOrdered)    || Cmp(Opt,Short_PatternOrdered)     || Cmp(Opt,Long_PatternOrdered     )) return(PatternOrdered_Option);
  else if(Cmp(Opt,Create_New_Circuit)|| Cmp(Opt,Short_Create_New_Circuit) || Cmp(Opt,Long_Create_New_Circuit )) return(Create_New_Circuit_Option);
  else if(Cmp(Opt,Short_Help)        || Cmp(Opt,Short_Short_Help))                                              return(Short_Help_Option);
  else if(Cmp(Opt,Long_Help)         ||                                      Cmp(Opt,Long_Long_Help          )) return(Long_Help_Option);
  else if(Cmp(Opt,Simulation_Info)   || Cmp(Opt,Short_Simulation_Info)    || Cmp(Opt,Long_Simulation_Info    )) return(Simulation_Info_Option);
  else if(Cmp(Opt,Init_FF)           || Cmp(Opt,Short_Init_FF)            || Cmp(Opt,Long_Init_FF            )) return(Init_FF_Option);
  else if(Cmp(Opt,File_Init_FF)      || Cmp(Opt,Short_File_Init_FF)       || Cmp(Opt,Long_File_Init_FF       )) return(File_Init_FF_Option);
  else if(Cmp(Opt,Algorithm)         || Cmp(Opt,Short_Algorithm)          || Cmp(Opt,Long_Algorithm          )) return(Algorithm_Option);
  else if(Cmp(Opt,Manual)            || Cmp(Opt,Short_Manual)             || Cmp(Opt,Long_Manual             )) return(Manual_Option);
  else if(Cmp(Opt,Convert_Dee)       || Cmp(Opt,Short_Convert_Dee)        || Cmp(Opt,Long_Convert_Dee        )) return(Convert_Dee_Option);
  else if(Cmp(Opt,Convert_Edf)       || Cmp(Opt,Short_Convert_Edf)        || Cmp(Opt,Long_Convert_Edf        )) return(Convert_Edf_Option);
  else if(Cmp(Opt,Limits)            || Cmp(Opt,Short_Limits)             || Cmp(Opt,Long_Limits             )) return(Limits_Option);
  else if(Cmp(Opt,Memory_Status)     || Cmp(Opt,Short_Memory_Status)      || Cmp(Opt,Long_Memory_Status      )) return(Memory_Status_Option);
  else if(Cmp(Opt,Dir_List)          || Cmp(Opt,Short_Dir_List)           || Cmp(Opt,Long_Dir_List           )) return(Dir_List_Option);
  else if(Cmp(Opt,Quit)              || Cmp(Opt,Short_Quit)               || Cmp(Opt,Long_Quit               )) return(Quit_Option);
  else if(Cmp(Opt,Run)               || Cmp(Opt,Short_Run)                || Cmp(Opt,Long_Run                )) return(Run_Option);
  else if(Cmp(Opt,Simulate)          || Cmp(Opt,Short_Simulate)           || Cmp(Opt,Long_Simulate           )) return(Simulate_Option);
  else if(Cmp(Opt,Atpg1)             || Cmp(Opt,Short_Atpg1)              || Cmp(Opt,Long_Atpg1              )) return(Atpg1_Option);
  else if(Cmp(Opt,Atpg2)             || Cmp(Opt,Short_Atpg2)              || Cmp(Opt,Long_Atpg2              )) return(Atpg2_Option);
  else if(Cmp(Opt,Command_File)      || Cmp(Opt,Short_Command_File)       || Cmp(Opt,Long_Command_File       )) return(Command_File_Option);
  else if(Cmp(Opt,Faumanager)        || Cmp(Opt,Short_Faumanager)         || Cmp(Opt,Long_Faumanager         )) return(Faumanager_Option);
  else if(Cmp(Opt,Verbose)           || Cmp(Opt,Short_Verbose)            || Cmp(Opt,Long_Verbose            )) return(Verbose_Option);
  else if(Cmp(Opt,Author)            || Cmp(Opt,Short_Author)             || Cmp(Opt,Long_Author             )) return(Author_Option);
  else if(Cmp(Opt,Copyright_Notice)  || Cmp(Opt,Short_Copyright_Notice)   || Cmp(Opt,Long_Copyright_Notice   )) return(Copyright_Notice_Option);
  else if(Cmp(Opt,Circuit_Levels)    || Cmp(Opt,Short_Circuit_Levels)     || Cmp(Opt,Long_Circuit_Levels     )) return(Circuit_Levels_Option);
  else if(Cmp(Opt,Settings)          || Cmp(Opt,Short_Settings)           || Cmp(Opt,Long_Settings           )) return(Settings_Option);
  else if(Cmp(Opt,Reset_Count)       || Cmp(Opt,Short_Reset_Count)        || Cmp(Opt,Long_Reset_Count        )) return(Reset_Count_Option);
  else if(Cmp(Opt,Ansi)              || Cmp(Opt,Short_Ansi)               || Cmp(Opt,Long_Ansi               )) return(Ansi_Option);
  return(UNKNOWN);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Command_Line_Short_Help                                         */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 29/06/99                                                        */
/*                                                                          */
/****************************************************************************/
                      
void Command_Line_Short_Help(void)
{
 Print(FYellow,                          "General Options:");
 
 Bar(FBYellow);
 Print(FBCyan,                           "ATPG:");
 Bar(FBYellow);
 
 Print(FBYellow,Short_Atpg1              " {file}{coverage}{lenght}{try}");
 Print(FBYellow,Short_Atpg2              " {file}{coverage}{lenght}{try}{max resets}");
 Print(FBYellow,"  "                     "  {lenght}   Pattern Maximum Length");
 Print(FBYellow,"  "                     "  {file}     Specify New INP Test Pattern");
 Print(FBYellow,"  "                     "  {coverage} Specify Minimal Requested Coverage");
 Print(FBYellow,"  "                     "  {try}      How Many Times Loops Fsim Foreach Block");
 
 Print(FBYellow,"  "                     " :START A.T.P.G.");
 
 Bar(FBYellow);
 Print(FBCyan,                           "Fault Simulation:");
 Bar(FBYellow);
 
 Print(FBYellow,Short_Simulate           "        :START FAULT SIMULATION                   (.SLF)");
 Print(FBYellow,Short_Init_FF            " {type} :Init FF Using type (1/0/X=2)             ------");
 Print(FBYellow,Short_File_Init_FF       " {file} :Init FF Using A Init File                (.FFI)");
 Print(FBYellow,Short_Load_Circuit       " {file} :Specify CAL Circuit For Simulation       (.CAL)");
 Print(FBYellow,Short_Load_Faults        " {file} :Specify FAU FaultList For Simulation     (.FAU)");
 Print(FBYellow,Short_Load_Pattern       " {file} :Specify Another INP Test Pattern For Sim (.INP)");
 Print(FBYellow,Short_Load_All           " {file} :Load Specified Circuit/Pattern/FaultList");
 
 Bar(FBYellow);
 Print(FBCyan,                           "Fault Management:");
 Bar(FBYellow);

 Print(FBYellow,Short_Faumanager         " {Collapsing}{Stuck@}{Sorting}{file}");
 Print(FBYellow,"  "                     "  {file}       There Create a Complete FaultList");
 Print(FBYellow,"  "                     "  {Stuck@}     1="MModel_1" 2="MModel_2" 3="MModel_3);
 Print(FBYellow,"  "                     "  {Sorting}    1="MSort_1" 2="MSort_2" 3="MSort_3);
 Print(FBYellow,"  "                     "  {Collapsing} 1="MCollapse_1"    2="MCollapse_2);
 
 Bar(FBYellow);
 Print(FBCyan,                           "Circuit Management:");
 Bar(FBYellow);

 Print(FBYellow,Short_Create_New_Circuit " {file} :Create a New Sequential Circuit  (.CAL)");
 Print(FBYellow,Short_Convert_Dee        " {file} :Convert a DEE file To Cal Format (.CAL)");
 Print(FBYellow,Short_Convert_Edf        " {file} :Convert a EDF file To Cal Format (.CAL)");
 Print(FBYellow,Short_Circuit_Levels     "        :Give Number Of Gates For Each Level");
 
 Bar(FBYellow);
 Print(FBCyan,                           "Simulation Stuff:");
 Bar(FBYellow);

 Print(FBYellow,Short_Simulation_Info    " :General Simulation Info");
 Print(FBYellow,Short_Memory_Status      " :Memory-Files       Info");
 Print(FBYellow,Short_Reset_Count        " :Counts Reset Signals as Valid Patterns");
 Print(FBYellow,Short_PatternOrdered     " :Save PatternOrdered Sim Info (.DLF)");
 
 Bar(FBYellow);
 Print(FBCyan,                           "Various Stuff:");
 Bar(FBYellow);

 Print(FBYellow,Short_Ansi               " :Enable Ansi");
 Print(FBYellow,Short_Author             " :WhoIs Author");
 Print(FBYellow,Short_About              " :About Program");
 Print(FBYellow,Short_Algorithm          " :About Algorithm");
 Print(FBYellow,Short_Short_Help         " :This Short Help");
 Print(FBYellow,Short_Limits             " :Limits Of Program");
 Print(FBYellow,Short_Settings           " :Settings Of Program");
 Print(FBYellow,Short_Manual             " :Caliban User Manual");
 Print(FBYellow,Short_Silent             " :Active Silent Mode (No Output)");
 Print(FBYellow,Short_Copyright_Notice   " :COPYRIGHT NOTICE - READ AS FIRST");
 Print(FBYellow,Short_Verbose            " :Active Verbose Mode (Output [VERY] Long)");
 Print(FBYellow,Short_Command_File       " :Active Command File (Other Commands Will Be Ignored)");
 Bar(FBYellow);
 Print(FBCyan,                           "NB: {file} Must Be Specified Without Extension");
 Bar(FBYellow);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Command_Line_Long_Help                                          */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 29/06/99                                                        */
/*                                                                          */
/****************************************************************************/

void Command_Line_Long_Help(void)
{
 CHAR Option[STRING_LEN];

 Print(FYellow,"General Options:");
 
 Bar(FBYellow);
 Print(FBCyan,"ATPG:");
 Bar(FBYellow);

 Print_Option(Option,Long_Atpg1);              Print2(FBYellow,Option," {file}{coverage}{lenght}{try}");
 Print_Option(Option,Long_Atpg2);              Print2(FBYellow,Option," {file}{coverage}{lenght}{try}{max resets}");
 Print_Option(Option," ");                     Print2(FBYellow,Option,"  {lenght}   Pattern Maximum Length");
 Print_Option(Option," ");                     Print2(FBYellow,Option,"  {try}      How Many Times Loops Fsim");
 Print_Option(Option," ");                     Print2(FBYellow,Option,"  {file}     Specify New INP Test Pattern");
 Print_Option(Option," ");                     Print2(FBYellow,Option,"  {coverage} Specify Minimal Requested Coverage");
 Print_Option(Option," ");                     Print2(FBYellow,Option," :START A.T.P.G.");

 Bar(FBYellow);
 Print(FBCyan,"Fault Simulation:");
 Bar(FBYellow);


 Print_Option(Option,Long_Simulate);           Print2(FBYellow,Option,"        :Start FAULT SIMULATION");
 Print_Option(Option,Long_Init_FF);            Print2(FBYellow,Option," {type} :Init FF Using Type (1/0/X=2)");
 Print_Option(Option,Long_File_Init_FF);       Print2(FBYellow,Option," {file} :Init FF Using a Init File");
 Print_Option(Option,Long_Load_Circuit);       Print2(FBYellow,Option," {file} :Specify CAL Circuit For Simulation");
 Print_Option(Option,Long_Load_Faults);        Print2(FBYellow,Option," {file} :Specify FAU FaultList For Simulation");
 Print_Option(Option,Long_Load_Pattern);       Print2(FBYellow,Option," {file} :Specify INP Test Pattern For Simulation");
 Print_Option(Option,Long_Load_All);           Print2(FBYellow,Option," {file} :Load Common-Named Circuit/Pattern/FauList");
 
 Bar(FBYellow);
 Print(FBCyan,"Fault Management:");
 Bar(FBYellow);

 Print_Option(Option,Long_Faumanager);         Print2(FBYellow,Option," {Collapsing}{Stuck@}{Sorting}{file}");
 Print_Option(Option," ");                     Print2(FBYellow,Option," >{file}       There Create a Complete FaultList");
 Print_Option(Option," ");                     Print2(FBYellow,Option," >{Stuck@}     1="MModel_1" 2="MModel_2" 3="MModel_3);
 Print_Option(Option," ");                     Print2(FBYellow,Option," >{Sorting}    1="MSort_1" 2="MSort_2" 3="MSort_3);
 Print_Option(Option," ");                     Print2(FBYellow,Option," >{Collapsing} 1="MCollapse_1"    2="MCollapse_2);

 Bar(FBYellow);
 Print(FBCyan,"Circuit Management:");
 Bar(FBYellow);

 Print_Option(Option,Long_Create_New_Circuit); Print2(FBYellow,Option," {file} :Create a New Sequential Circuit");
 Print_Option(Option,Long_Convert_Dee);        Print2(FBYellow,Option," {file} :Convert a DEE file To Cal Format");
 Print_Option(Option,Long_Convert_Edf);        Print2(FBYellow,Option," {file} :Convert a EDF file To Cal Format");
 Print_Option(Option,Long_Circuit_Levels);     Print2(FBYellow,Option,"        :Give Number Of Gates For Each Level");

 Bar(FBYellow);
 Print(FBCyan,"Simulation Stuff:");
 Bar(FBYellow);

 Print_Option(Option,Long_Simulation_Info);    Print2(FBYellow,Option," :General Simulation Info");
 Print_Option(Option,Long_Reset_Count);        Print2(FBYellow,Option," :Counts Reset Signals as Valid Patterns"); 
 Print_Option(Option,Long_PatternOrdered);     Print2(FBYellow,Option," :PatternOrdered Simulation Info");
 Print_Option(Option,Long_Memory_Status);      Print2(FBYellow,Option," :Memory-Files Info");
 
 Bar(FBYellow);
 Print(FBCyan,"Various Stuff:");   
 Bar(FBYellow);

 Print_Option(Option,Long_Ansi);               Print2(FBYellow,Option," :Enable Ansi");
 Print_Option(Option,Long_Author);             Print2(FBYellow,Option," :WhoIs Author");
 Print_Option(Option,Long_About);              Print2(FBYellow,Option," :About Program");
 Print_Option(Option,Long_Algorithm);          Print2(FBYellow,Option," :About Algorithm");
 Print_Option(Option,Long_Long_Help);          Print2(FBYellow,Option," :This Long Help");
 Print_Option(Option,Long_Limits);             Print2(FBYellow,Option," :Limits Of Program");
 Print_Option(Option,Long_Settings);           Print2(FBYellow,Option," :Settings Of Program");
 Print_Option(Option,Long_Manual)              Print2(FBYellow,Option," :Caliban User Manual");
 Print_Option(Option,Long_Silent);             Print2(FBYellow,Option," :Active Silent Mode");
 Print_Option(Option,Long_Copyright_Notice);   Print2(FBYellow,Option," :Copyright Notice READ FIRST");
 Print_Option(Option,Long_Verbose);            Print2(FBYellow,Option," :Active Verbose Mode (Output [VERY] Long)");
 Print_Option(Option,Long_Command_File);       Print2(FBYellow,Option," :Active Command File (Other Commands Will Be Ignored)");
 Bar(FBYellow);
 Print(FBCyan,"NB: {file} Must Be Specified Without Extension");
 Bar(FBYellow);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Menu                                                            */
/* Param. : int, char*[]                                                    */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 29/06/99                                                        */
/*                                                                          */
/****************************************************************************/

void Menu(INT Number_Of_Args,CHAR *List_Of_Args[])
{
 CHAR   String[STRING_LEN];

 INT    index1=NONE;
 INT    Option=NONE;
 INT    Menu_Mem=NONE;
 INT    Command_Filez=FALSE;
 INT    Current_Argument=NONE;
 INT    Num_Of_Args=Number_Of_Args;

 CHAR* *Command=NULL;

 if(Command_Line_In_Use==TRUE)
 {  
  Command=List_Of_Args;
  for(index1=NONE;index1 < Num_Of_Args;index1++)
  {
   if((Cmp(List_Of_Args[index1],Short_Command_File)) || (Cmp(List_Of_Args[index1],Long_Command_File)))
   {
    Command=NULL;
    Command_Filez=TRUE;
    break;
   }
  }
  Num_Of_Args--;
 }
 else
 {
  Alloc(Command   ,1,CHAR*,Menu_Mem);
  Alloc(Command[0],STRING_LEN,CHAR,Menu_Mem);
 }

 if(Command_Filez==TRUE)
 {
  Num_Of_Args=NONE;
  while(!feof(File[CFG].Ptr))
  {
   Num_Of_Args++;
   fscanf(File[CFG].Ptr,"%*s");
   if(feof(File[CFG].Ptr))
   {
    FClose(CFG);
    break;
   }
  }

  Alloc(Command,Num_Of_Args,CHAR*,Menu_Mem);

  if(Fopen(_Program_Name,CFG,CFG,OPEN_R,E_OPEN)==ERROR) Break(ERROR);

  for(index1=1;index1 < Num_Of_Args;index1++)
  {
   Alloc(Command[index1],STRING_LEN,CHAR,Menu_Mem);
   fscanf(File[CFG].Ptr,"%s",Command[index1]);
  }
  Num_Of_Args--;
  FClose(CFG);
 }

 /****************************************************************************/

 Current_Argument=NONE;
 ForEver()
 {
  if(Command_Line_In_Use==TRUE)
  {
   if(++Current_Argument > Num_Of_Args)
   {
    GoodBye();
   }
  }
  else  
  {
   Prompt();
   Current_Argument=NONE;
   scanf("%s",Command[Current_Argument]);
  }

  FPrint2(CLF,"=>",Command[Current_Argument]);

  Option=Set_Option(Command[Current_Argument]);

  switch(Option)
  {
   case(Load_All_Option):if(Command_Line_In_Use==TRUE) 
                         {
                          if(Current_Argument+Load_All_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                          ++Current_Argument;
                         }
                         else
                         {
                          Print(MESSAGE_COLOR,"Get Simulation File Name (WithOut Ext):");
                          scanf("%s",Command[Current_Argument]);
                         }

                         strcpy(Circuit.Name,Command[Current_Argument]);
                         if(Create_Cal_Circuit(Command[Current_Argument])==OK)
                         {
                          Create_Pattern(Command[Current_Argument]);
                          Create_Fault_List(Command[Current_Argument]);
                         }
   break;
   /******** --- ********/
   case(Load_Circuit_Option):if(Command_Line_In_Use==TRUE)
                             {
                              if(Current_Argument+Load_Circuit_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                              Create_Cal_Circuit(Command[++Current_Argument]);
                             }
                             else
                             {
                              Print(MESSAGE_COLOR,"Get .CAL Circuit File Name (WithOut Ext):");
                              scanf("%s",Command[Current_Argument]);
                              strcpy(Circuit.Name,Command[Current_Argument]);
                              Create_Cal_Circuit(Command[Current_Argument]);
                             }
   break;
   /******** --- ********/
   case(Load_Faults_Option):if(Command_Line_In_Use==TRUE)
                            {
                             if(Current_Argument+Load_Faults_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                             Create_Fault_List(Command[++Current_Argument]);
                            }
                            else
                            {
                             Print(MESSAGE_COLOR,"Get .FAU FaultList File Name (WithOut Ext):");
                             scanf("%s",Command[Current_Argument]);
                             Create_Fault_List(Command[Current_Argument]);
                            }
   break;
   /******** --- ********/
   case(Load_Pattern_Option):if(Command_Line_In_Use==TRUE)
                             {
                              if(Current_Argument+Load_Pattern_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                              Create_Pattern(Command[++Current_Argument]);
                             }
                             else
                             {
                              Print(MESSAGE_COLOR,"Get .INP Pattern File Name (WithOut Ext):");
                              scanf("%s",Command[Current_Argument]);
                              Create_Pattern(Command[Current_Argument]);
                             }
   break;
   /******** --- ********/
   case(Convert_Dee_Option):if(Command_Line_In_Use==TRUE) 
                            { 
                             if(Current_Argument+Convert_Dee_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                             Dee2Cal(Command[++Current_Argument]);
                            }
                            else
                            {
                             Print(MESSAGE_COLOR,"Get .DEE Circuit File Name (WithOut Ext):");
                             scanf("%s",Command[Current_Argument]);
                             Dee2Cal(Command[Current_Argument]);
                            }
   break;
   /******** --- ********/
   case(Faumanager_Option):if(Command_Line_In_Use==TRUE)
                           {
                            if(Current_Argument+Faumanager_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                            {

                             Fauman(Command[Current_Argument+1],Command[Current_Argument+2],Command[Current_Argument+3],Command[Current_Argument+4]);
                             Current_Argument+=Faumanager_Args;
                            }
                           }
                           else
                           {
                            Print(MESSAGE_COLOR,"Get .FAU NEW FaultList File Name (WithOut Ext):");
                            scanf("%s",Command[Current_Argument]);
                            Fauman(Command[Current_Argument],"","","");
                           }
   break;
   /******** --- ********/
   case(Circuit_Levels_Option):Give_Gates_For_Levels();
   break;
   /******** --- ********/
   case(Command_File_Option):;
   break;
   /******** --- ********/
   case(Silent_Option):if((File[BAT].Ptr==NULL) && (Command_Line_In_Use==TRUE))
                       { 
                        File[BAT].Ptr=freopen(File[BAT].Name,"w",stdout);
                        if(File[BAT].Ptr==NULL) 
                        {
                         Print(MESSAGE_COLOR,"System Cannot Active Silent Mode");
                        }
                        else Silent_Mode=TRUE;
                       }

                       if(Command_Line_In_Use==FALSE) Print(MESSAGE_COLOR,"Option Avaible Only In Command_Line Mode");
   break;
   /******** --- ********/
   case(Verbose_Option):if(Verbose_Mode==TRUE)
                        {
                         Print(MESSAGE_COLOR,"[Verbose Mode] Is Inactive NOW");
                         Verbose_Mode=FALSE;
                        }
                        else
                        {
                         Print(MESSAGE_COLOR,"[Verbose Mode] Is Active NOW");
                         Verbose_Mode=TRUE;
                        }
   break;
   /******** --- ********/
   case(Reset_Count_Option):if(Reset_Count_Mode==TRUE)
                        {
                         Print(MESSAGE_COLOR,"[Reset Count Mode] Is Inactive NOW");
                         Reset_Count_Mode=FALSE;
                        }
                        else
                        {
                         Print(MESSAGE_COLOR,"[Reset Count Mode] Is Active NOW");
                         Reset_Count_Mode=TRUE;
                        }
   break;
   /******** --- ********/
   case(Simulate_Option):Sim();
                         Results();
   break;
   /******** --- ********/
   case(File_Init_FF_Option):if(Command_Line_In_Use==TRUE)
                             { 
                              if(Current_Argument+File_Init_FF_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                              File_Init_FFs(Command[++Current_Argument]);
                             }
                             else
                             {
                              Print(MESSAGE_COLOR,"Get .FFI FF INit File Name (WithOut Ext):");
                              scanf("%s",Command[Current_Argument]);
                              File_Init_FFs(Command[Current_Argument]);
                             }
   break;
   /******** --- ********/
   case(Init_FF_Option):if(Command_Line_In_Use==TRUE)
                        { 
                         if(Current_Argument+Init_FF_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                         Init_FFs(Command[++Current_Argument]);
                        }
                        else
                        {
                         Print(MESSAGE_COLOR,"Get An Init Value (1/0/X=2):");
                         scanf("%s",Command[Current_Argument]);
                         Init_FFs(Command[Current_Argument]);
                        }
   break;
   /******** --- ********/
   case(Atpg1_Option):if(Command_Line_In_Use==TRUE)
                      {
                       if(Current_Argument+Atpg1_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                       {
                        Gengis(Command[Current_Argument+1],Command[Current_Argument+2],Command[Current_Argument+3],Command[Current_Argument+4]);
                        Current_Argument+=Atpg1_Args;
                       }
                      }
                      else
                      {
                       Print(MESSAGE_COLOR,"Get .INP Pattern File Name (WithOut Ext):");
                       scanf("%s",Command[Current_Argument]);
                       Gengis(Command[Current_Argument],"","","");
                      }
   break;
   /******** --- ********/
   case(Atpg2_Option):if(Command_Line_In_Use==TRUE)
                      {
                       if(Current_Argument+Atpg2_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                       {
                        Gengis_Multiple_Resets(Command[Current_Argument+1],Command[Current_Argument+2],Command[Current_Argument+3],Command[Current_Argument+4],Command[Current_Argument+5]);
                        Current_Argument+=Atpg2_Args;
                       }
                      }
                      else
                      {
                       Print(MESSAGE_COLOR,"Get .INP Pattern File Name (WithOut Ext):");
                       scanf("%s",Command[Current_Argument]);
                       Gengis_Multiple_Resets(Command[Current_Argument],"","","","");
                      }
   break;
   /******** --- ********/
   case(Short_Help_Option):if(Command_Line_In_Use==FALSE) Menu_Help();
               if(Command_Line_In_Use==TRUE ) Command_Line_Short_Help();
   break;
   /******** --- ********/
   case(Long_Help_Option):if(Command_Line_In_Use==FALSE) Menu_Help();
              if(Command_Line_In_Use==TRUE ) Command_Line_Long_Help();
   break;
   /******** --- ********/
   case(About_Option):Info();
   break;
   /******** --- ********/
   case(Settings_Option):Ck_Settings();
   break;
   /******** --- ********/
   case(Limits_Option):Program_Limits();
   break;
   /******** --- ********/
   case(Copyright_Notice_Option):Copyright_Long_Notice();
   break;
   /******** --- ********/
   case(Author_Option):Author_Datas();
   break;
   /******** --- ********/
   case(Memory_Status_Option):Print_Time();
                              Bar(BAR_COLOR);
                              SPrint_Mem_Used(String,Circuit.Mem_PI);
                              Print2(Normal,"Pi List    :",String);
                              SPrint_Mem_Used(String,Circuit.Mem_FF);
                              Print2(Normal,"FF List    :",String);
                              SPrint_Mem_Used(String,Circuit.Mem_Circuit);
                              Print2(Normal,"Netlist    :",String);
                              Bar(BAR_COLOR);
                              SPrint_Mem_Used(String,Circuit.Mem_Fault_List);
                              Print2(Normal,"Fault List :",String);
                              SPrint_Mem_Used(String,Circuit.Mem_Faults_FF);
                              Print2(Normal,"Fau-FF List:",String);
                              Bar(BAR_COLOR);
                              SPrint_Mem_Used(String,Circuit.Mem_Pattern);
                              Print2(Normal,"Pattern    :",String);
                              Bar(BAR_COLOR);
                              SPrint_Mem_Used(String,Circuit.Mem_Simulation);
                              Print2(Normal,"Simulation :",String);
                              Bar(BAR_COLOR);
                              Get_Files_Info();
                              Bar(BAR_COLOR);
                              if(Verbose_Mode       ==TRUE) {Print(MESSAGE_COLOR,"Verbose        Mode= Active");} else {Print(MESSAGE_COLOR,"Verbose         Mode= Inactive");}
                              if(Ansi_Mode          ==TRUE) {Print(MESSAGE_COLOR,"Ansi           Mode= Active");} else {Print(MESSAGE_COLOR,"Ansi            Mode= Inactive");}
                              if(Silent_Mode        ==TRUE) {Print(MESSAGE_COLOR,"Silent         Mode= Active");} else {Print(MESSAGE_COLOR,"Silent          Mode= Inactive");}
                              if(Verbose_Mode       ==TRUE) {Print(MESSAGE_COLOR,"Verbose        Mode= Active");} else {Print(MESSAGE_COLOR,"Verbose         Mode= Inactive");}
                              if(Reset_Count_Mode   ==TRUE) {Print(MESSAGE_COLOR,"Reset Count    Mode= Active");} else {Print(MESSAGE_COLOR,"Reset Count     Mode= Inactive");}
                              if(PatternOrdered_Mode==TRUE) {Print(MESSAGE_COLOR,"PatternOrdered Mode= Active");} else {Print(MESSAGE_COLOR,"Pattern Ordered Mode= Inactive");}
                              if(Multiple_Atpg_Mode ==TRUE) {Print(MESSAGE_COLOR,"Multiple Atpg  Mode= Active");} else {Print(MESSAGE_COLOR,"Multiple Atpg   Mode= Inactive");}
                              Bar(BAR_COLOR);
   break;
   /******** --- ********/
   case(Simulation_Info_Option):Get_Simulation_Info();
   break;
   /******** --- ********/
   case(PatternOrdered_Option):if(PatternOrdered_Mode==FALSE)
                           {
                            Print(MESSAGE_COLOR,"Pattern Ordered Mode Is Active NOW");
                            PatternOrdered_Mode=TRUE;
                           }
                           else
                           {
                            Print(MESSAGE_COLOR,"Pattern Ordered Mode Is Inactive NOW");
                            PatternOrdered_Mode=FALSE;
                           }
   break;
   /******** --- ********/
   case(Dir_List_Option):if(Command_Line_In_Use==FALSE){NL;Directory();}
   break;
   /******** --- ********/
   case(Ansi_Option):if(Is_Ansi())
                     {
                      if(Ansi_Mode==TRUE)
                      {             
                       Print(MESSAGE_COLOR,"Ansi Mode Is Inactive NOW");
                       Unset_Ansi();
                      }
                      else
                      {
                       Print(MESSAGE_COLOR,"Ansi Mode Is Active NOW");
                       Set_Ansi();
                      }
                     }
                     else
                      Print(MESSAGE_COLOR,"System Does Not Support Ansi Modes");
   break;
   /******** --- ********/
   case(Algorithm_Option):Algo();
   break;
   /******** --- ********/
   case(Manual_Option):Man();
   break;
   /******** --- ********/
   case(Run_Option):if(Command_Line_In_Use==FALSE) Sys();
   break;
   /******** --- ********/
   case(Quit_Option):if(File[BAT].Ptr!=NULL) unlink(File[BAT].Name);
   GoodBye();
   break;
   /******** --- ********/
   case(Create_New_Circuit_Option):
   Sys_Error("Create New Circuit",E_NOT_AVAIB);
   break;
   /******** --- ********/
   case(Convert_Edf_Option):if(Command_Line_In_Use==TRUE)
                            { 
                             if(Current_Argument+Convert_Edf_Args > Num_Of_Args){Sys_Error("",E_NUM_ARG);Break(ERROR);}
                             Edf2Cal(Command[++Current_Argument]);
                            }
                            else
                            {
                             Print(MESSAGE_COLOR,"Get .EDF Circuit File Name (WithOut Ext):");
                             scanf("%s",Command[Current_Argument]);
                             Edf2Cal(Command[Current_Argument]);
                            }
   break;
   /******** --- ********/
   default:if(Command_Line_In_Use==FALSE) Print3(ERROR_COLOR,"[",Command[Current_Argument],"]=> Unknown Option. Type <h> For Help Screen...");
           if(Command_Line_In_Use==TRUE)  Print2(ERROR_COLOR, "Unrecognized Option:",Command[Current_Argument]);
  }
 }
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Results                                                         */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 29/06/99                                                        */
/*                                                                          */
/****************************************************************************/

void Results(void)     
{
 Bar(BAR_COLOR);
 PrintSL(Normal, "Evaluated Gates ------------:",Circuit.Evaluated_Gates);
 PrintSL(Normal, "Evaluated Gates For Second -:",Circuit.Event_Rate);
 Bar(BAR_COLOR);
 PrintSL(Normal, "Faults (Min. 1) ------------:",Circuit.Num_Faults);
 PrintSL(Normal, "Found Faults ---------------:",Circuit.Found_Faults);
 Bar(BAR_COLOR);
 PrintSD(FBWhite,"Fault Coverage% ------------:",Circuit.Coverage);
 Bar(BAR_COLOR);
 PrintSL(Normal, "Number Of Patterns ---------:",Circuit.Num_Patterns);
 PrintSL(Normal, "Number Of Used Patterns ----:",Circuit.Num_Used_Patterns);
 Bar(BAR_COLOR);
 Print_Time();
 Print2(Normal,  "Start Simulation Time ------:",Circuit.Simulation_Start_Time);
 Print2(Normal,  "End Simulation Time --------:",Circuit.Simulation_Stop_Time);
 Bar(BAR_COLOR);
 Print2(Normal,  "Elapsed Simulation Time ----:",Circuit.Simulation_Elapsed_Time);
 Bar(BAR_COLOR);
}

/****************************************************************************/
/*                                                                          */
/* Fx Name: Get_Simulation_Info                                             */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 29/06/99                                                        */
/*                                                                          */
/****************************************************************************/

void Get_Simulation_Info(void) 
{
 CHAR String[STRING_LEN];

 Print(BAR_COLOR,CLOSE_BAR);
 Print2(Normal, "Circuit File Name ----------:",Circuit.Cir_Name);
 Print2(Normal, "Faults  File Name ----------:",Circuit.Fau_Name);
 Print2(Normal, "Pattern File Name ----------:",Circuit.Pat_Name);
 Print(BAR_COLOR,CLOSE_BAR);
 PrintSL(Normal,"Primary Inputs -------------:",Circuit.Num_PI);
 PrintSL(Normal,"Primary Outputs ------------:",Circuit.Num_PO);
 PrintSL(Normal,"Total Flip Flops -----------:",Circuit.Num_FF);
 PrintSL(Normal,"Total Gates ----------------:",Circuit.Num_Gates);
 PrintSL(Normal,"Total Levels ---------------:",Circuit.Num_Levels);
 PrintSL(Normal,"Total Descriptors-----------:",Circuit.Num_Descr);
 Print(BAR_COLOR,CLOSE_BAR);
 PrintSL(Normal,"Buffer ---------------------:",Circuit.Ports[BUFFER]);
 PrintSL(Normal,"And ------------------------:",Circuit.Ports[AND]);
 PrintSL(Normal,"Or -------------------------:",Circuit.Ports[OR]);
 PrintSL(Normal,"Exor -----------------------:",Circuit.Ports[EXOR]);
 PrintSL(Normal,"Not ------------------------:",Circuit.Ports[NOT]);
 PrintSL(Normal,"Nand -----------------------:",Circuit.Ports[NAND]);
 PrintSL(Normal,"Nor ------------------------:",Circuit.Ports[NOR]);
 PrintSL(Normal,"Nexor ----------------------:",Circuit.Ports[NEXOR]);
 PrintSL(Normal,"FF -------------------------:",Circuit.Ports[FF]);
 Print(BAR_COLOR,CLOSE_BAR);
 Print2(Normal, "Flip Flop Precharge At -----:",FF_Init_Mode);
 Print(BAR_COLOR,CLOSE_BAR);
 PrintSL(Normal,"Max Fanin ------------------:",Circuit.Max_Fanin);
 PrintSL(Normal,"Max Fanout -----------------:",Circuit.Max_Fanout);
 PrintSL(Normal,"Max FF Fanout --------------:",Circuit.Max_FF_Fanout);
 Print(BAR_COLOR,CLOSE_BAR);
 PrintSL(Normal,"Gain -----------------------:",Circuit.Gain);
 PrintSL(Normal,"Events ---------------------:",Circuit.Events);
 PrintSL(Normal,"Good Events ----------------:",Circuit.Good_Events);
 PrintSL(Normal,"Evaluated Gates ------------:",Circuit.Evaluated_Gates);
 PrintSL(Normal,"Evaluated Gates For Second -:",Circuit.Event_Rate);
 Print(BAR_COLOR,CLOSE_BAR);
 Print2(Normal, "Sort Method ----------------:",Circuit.Sort_Mode);
 PrintSL(Normal,"Stk@0 ----------------------:",Circuit.Num_Stk0);
 PrintSL(Normal,"Stk@1 ----------------------:",Circuit.Num_Stk1);
 PrintSL(Normal,"Faults (Min. 1) ------------:",Circuit.Num_Faults);
 PrintSL(Normal,"Found Faults ---------------:",Circuit.Found_Faults);
 Print(BAR_COLOR,CLOSE_BAR);
 PrintSD(Normal,"Fault Coverage% ------------:",Circuit.Coverage);
 Print(BAR_COLOR,CLOSE_BAR);
 PrintSL(Normal,"Number Of Patterns ---------:",Circuit.Num_Patterns);
 PrintSL(Normal,"Number Of Used Patterns ----:",Circuit.Num_Used_Patterns);
 Print(BAR_COLOR,CLOSE_BAR);
 SPrint_Mem_Used(String,Circuit.Mem_FF);
 Print2(Normal, "Memory Used For FF ---------:",String);
 SPrint_Mem_Used(String,Circuit.Mem_PI);
 Print2(Normal, "Memory Used For PI ---------:",String);
 SPrint_Mem_Used(String,Circuit.Mem_Fault_List);
 Print2(Normal, "Memory Used For Faults -----:",String);
 SPrint_Mem_Used(String,Circuit.Mem_Faults_FF);
 Print2(Normal, "Memory Used For Fau-FF List-:",String);
 SPrint_Mem_Used(String,Circuit.Mem_Pattern);
 Print2(Normal, "Memory Used For Pattern ----:",String);
 SPrint_Mem_Used(String,Circuit.Mem_Circuit);
 Print2(Normal, "Memory Used For Circuit ----:",String);
 SPrint_Mem_Used(String,Circuit.Mem_Simulation);
 Print2(Normal, "Memory Used For Simulation -:",String);
 Print(BAR_COLOR,CLOSE_BAR);
 Print2(Normal, "Start Simulation Time ------:",Circuit.Simulation_Start_Time);
 Print2(Normal, "End Simulation Time --------:",Circuit.Simulation_Stop_Time);
 Print2(Normal, "Elapsed Simulation Time ----:",Circuit.Simulation_Elapsed_Time);
 Print(BAR_COLOR,CLOSE_BAR);
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
