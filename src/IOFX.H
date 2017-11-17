/*PPU************************************************************************\
*
*    $Source: iofx.h
*  $Revision: 0.7.6
*      $Date: Thu Jul  8 19:30:52 1999 
*
\************************************************************************PPU*/

# define NL              printf("\n");
# define OL              printf("\r");
# define VNL             if(Verbose_Mode==TRUE) NL;
# define NN              ;
                       
# define FNL(FP)         fprintf(File[FP].Ptr,"\n");

/****************************************************************************/

# define Clear_Screen() {SetColor(Reset);SetColor(Ansi_Clear);}

#ifdef __MSDOS__
#ifdef BORLAND
# undef  Clear_Screen()
# define Clear_Screen()  clrscr()
#endif
#endif

/****************************************************************************/

# define GoodBye()       {GoodBye_CLog();NL;                \
                          Release_All_Memory();FCloseall(); \
                          Circular();exit(OK);}

/****************************************************************************/

# define Break(mode)     {Break_CLog();                                \
                          Break_DLog();                                \
                          Break_SLog();                                \
                          Break_FLog();                                \
                          Release_All_Memory();FCloseall();            \
                          NL;Print(ERROR_COLOR,"System AUTO Break...");\
                          NL;exit(mode);}

/****************************************************************************/

INT Dummy_Ansi(CHAR* Color);
INT Print_Ansi(CHAR* Color);

void Activate_Ansi(void);
void Deactivate_Ansi(void);

/****************************************************************************/

extern INT (*Apply_Color)(CHAR* Color);

/****************************************************************************/

#ifdef ANSI
# define Apply(Color)  ((*Apply_Color)(Color))
# define Set_Ansi()    {Activate_Ansi();Apply_Color=Print_Ansi;}
# define Unset_Ansi()  {Deactivate_Ansi();Apply_Color=Dummy_Ansi;}
#else
# define Apply(Color)  ;
# define Set_Ansi()    {Activate_Ansi();}
# define Unset_Ansi()  {Deactivate_Ansi();}
#endif

/****************************************************************************/

# define BAR            "----------------------------------------------------------------------"
# define CLOSE_BAR      "---------------------------------------------------------------------|"
# define SLASH_BAR      "//////////////////////////////////////////////////////////////////////"
# define ALTERNATED_BAR "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="
# define HEAVY_BAR      "######################################################################"
# define LOW_BAR        "______________________________________________________________________"
# define DEBUG_BAR      "--------------------------------------------------"

# define Bar(Color)     {Print(Color,BAR);}
# define BarV(Color)    {PrintV(Color,BAR);}

/****************************************************************************/
/** BAR PRINTER *************************************************************/
/****************************************************************************/

# define HBar(Color)    {Print(Color,HEAVY_BAR);}
# define ABar(Color)    {Print(Color,ALTERNATED_BAR);}

# define HBarV(Color)   {PrintV(Color,HEAVY_BAR);}
# define ABarV(Color)   {PrintV(Color,ALTERNATED_BAR);}

/****************************************************************************/
/** PRINT  ******************************************************************/
/** COLORS ******************************************************************/
/****************************************************************************/

# define SetColor(Color)              {Apply(Color);}
# define SetColor2(Attr,Color)        {Apply(Attr);Apply(Color);}
# define SetColor3(Attr,Bk,Color)     {Apply(Attr);Apply(Bk);Apply(Color);}
# define SetColor4(Attr,Bk,Att,Color) {Apply(Attr);Apply(Bk);Apply(Att);Apply(Color);}

/****************************************************************************/
/** PRINT  ******************************************************************/
/** NORMAL ******************************************************************/
/****************************************************************************/

# define Phase(a)                  {Bar(BAR_COLOR);NL;Apply(PHASE_COLOR);printf("%s",a);Apply(Reset);}

# define PrintTxt(Color,a)         {Apply(Color);printf("%s",a);Apply(Reset);}

# define PrintC(Color,a)           {NN;Apply(Color);printf("%c",a);}
# define PrintD(Color,a)           {NN;Apply(Color);printf("%g",a);}
# define PrintSC(Color,a,b)        {NL;Apply(Color);printf("%s %c",a,b);Apply(Reset);}
# define PrintSD(Color,a,D)        {NL;Apply(Color);printf("%s %g",a,(FLOAT)D);Apply(Reset);}

# define Print(Color,a)            {NL;Apply(Color);printf("%s",a);Apply(Reset);}
# define Print2(Color,a,b)         {NL;Apply(Color);printf("%s %s",a,b);Apply(Reset);}
# define Print3(Color,a,b,c)       {NL;Apply(Color);printf("%s %s %s",a,b,c);Apply(Reset);}
# define Print4(Color,a,b,c,d)     {NL;Apply(Color);printf("%s %s %s %s",a,b,c,d);Apply(Reset);}
# define Print5(Color,a,b,c,d,e)   {NL;Apply(Color);printf("%s %s %s %s %s",a,b,c,d,e);Apply(Reset);}
# define Print6(Color,a,b,c,d,e,f) {NL;Apply(Color);printf("%s %s %s %s %s %s",a,b,c,d,e,f);Apply(Reset);}

/****************************************************************************/
/** PRINT           *********************************************************/
/** STRING AND LONG *********************************************************/
/****************************************************************************/

# define PrintSX(Color,a,L1)                           {NL;Apply(Color);printf("%s %lx",a,(ULONG)L1);Apply(Reset);}
# define PrintSXX(Color,a,L1,L2)                       {NL;Apply(Color);printf("%s %lx %lx",a,(ULONG)L1,(ULONG)L2);Apply(Reset);}
# define PrintSLX(Color,a,L1,L2)                       {NL;Apply(Color);printf("%s %lu %lx",a,(ULONG)L1,(ULONG)L2);Apply(Reset);}
# define PrintSLXX(Color,a,L1,L2,L3)                   {NL;Apply(Color);printf("%s %lu %lx %lx",a,(ULONG)L1,(ULONG)L2,(ULONG)L3);Apply(Reset);}

# define PrintSL(Color,a,L1)                           {NL;Apply(Color);printf("%s %lu",a,(ULONG)L1);Apply(Reset);}
# define Print2SL(Color,a,L1,b,L2)                     {NL;Apply(Color);printf("%s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2);Apply(Reset);}
# define Print3SL(Color,a,L1,b,L2,c,L3)                {NL;Apply(Color);printf("%s %lu %s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3);Apply(Reset);}
# define Print4SL(Color,a,L1,b,L2,c,L3,d,L4)           {NL;Apply(Color);printf("%s %lu %s %lu %s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3,d,(ULONG)L4);Apply(Reset);}
# define Print5SL(Color,a,L1,b,L2,c,L3,d,L4,e,L5)      {NL;Apply(Color);printf("%s %lu %s %lu %s %lu %s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3,d,(ULONG)L4,e,(ULONG)L5);Apply(Reset);}
# define Print6SL(Color,a,L1,b,L2,c,L3,d,L4,e,L5,f,L6) {NL;Apply(Color);printf("%s %lu %s %lu %s %lu %s %lu %s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3,d,(ULONG)L4,e,(ULONG)L5,f,(ULONG)L6);Apply(Reset);}

# define PrintSLSD(Color,a,L1,b,L2)                    {NL;Apply(Color);printf("%s %lu %s %g",a,(ULONG)L1,b,(FLOAT)L2);Apply(Reset);}

/****************************************************************************/
/** PRINT        ************************************************************/
/** PERCENT MODE ************************************************************/
/****************************************************************************/

# define Print_Percent(Color,a,L1,L2)       {OL;Apply(Color);printf("%s [%3d%%] as [%lu] of [%lu]",a,(UINT)(100*L1/L2),(ULONG)L1,(ULONG)L2);Apply(Reset);}
# define Print_PercentV(Color,a,L1,L2)      {if(Verbose_Mode==TRUE){NL;Apply(Color);printf("%s [%3d%%] as [%lu] of [%lu]",a,(UINT)(100*L1/L2),(ULONG)L1,(ULONG)L2);Apply(Reset);}}
# define Print_Percent_Short(Color,a,L1,L2)  {OL;Apply(Color);printf("%s [%3d%%]",a,(UINT)(100*L1/L2));Apply(Reset);}

/****************************************************************************/
/** PRINT  ******************************************************************/
/** ONLINE ******************************************************************/
/****************************************************************************/

# define PrintO(Color,a)                 {OL;Apply(Color);printf("%s",a);Apply(Reset);}
# define PrintO2(Color,a,b)              {OL;Apply(Color);printf("%s %s",a,b);Apply(Reset);}

# define PrintOSLSD(Color,a,L1,b,L2)     {OL;Apply(Color);printf("%s %lu %s %g",a,(ULONG)L1,b,(FLOAT)L2);Apply(Reset);}

/****************************************************************************/
/** PRINT ONLINE    *********************************************************/
/** STRING AND LONG *********************************************************/
/****************************************************************************/

# define PrintOSL(Color,a,L1)            {OL;Apply(Color);printf("%s %lu",a,(ULONG)L1);Apply(Reset);}
# define PrintO2SL(Color,a,L1,b,L2)      {OL;Apply(Color);printf("%s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2);Apply(Reset);}
# define PrintO3SL(Color,a,L1,b,L2,c,L3) {OL;Apply(Color);printf("%s %lu %s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3);Apply(Reset);}

/****************************************************************************/
/** PRINT        ************************************************************/
/** VERBOSE MODE ************************************************************/
/****************************************************************************/

# define PrintVSX(Color,a,L1)         {if(Verbose_Mode==TRUE) {NL;Apply(Color);printf("%s %lx",a,(ULONG)L1);Apply(Reset);}}

# define PrintVC(Color,a)             {if(Verbose_Mode==TRUE) {   Apply(Color);printf("%c",a);}}

# define PrintV(Color,a)              {if(Verbose_Mode==TRUE) {NL;Apply(Color);printf("%s",a);Apply(Reset);}}
# define PrintV2(Color,a,b)           {if(Verbose_Mode==TRUE) {NL;Apply(Color);printf("%s %s",a,b);Apply(Reset);}}
# define PrintV4(Color,a,b,c,d)       {if(Verbose_Mode==TRUE) {NL;Apply(Color);printf("%s %s %s %s",a,b,c,d);Apply(Reset);}}

/****************************************************************************/
/** PRINT VERBOSE MODE ******************************************************/
/** STRING AND LONG    ******************************************************/
/****************************************************************************/

# define PrintVSL(Color,a,L1)                 {if(Verbose_Mode==TRUE) {NL;Apply(Color);printf("%s %lu",a,(ULONG)L1);Apply(Reset);}}
# define PrintV2SL(Color,a,L1,b,L2)           {if(Verbose_Mode==TRUE) {NL;Apply(Color);printf("%s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2);Apply(Reset);}}
# define PrintV3SL(Color,a,L1,b,L2,c,L3)      {if(Verbose_Mode==TRUE) {NL;Apply(Color);printf("%s %lu %s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3);Apply(Reset);}}
# define PrintV4SL(Color,a,L1,b,L2,c,L3,d,L4) {if(Verbose_Mode==TRUE) {NL;Apply(Color);printf("%s %lu %s %lu %s %lu %s %lu",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3,d,(ULONG)L4);Apply(Reset);}}

/****************************************************************************/
/** PRINT *******************************************************************/
/** FILES *******************************************************************/
/****************************************************************************/

# define FPrint(FP,a)                      {fprintf(File[FP].Ptr,"%s\n",a);}
# define FPrintC(FP,a)                     {fprintf(File[FP].Ptr,"%c",a);}
# define FPrint2(FP,a,b)                   {fprintf(File[FP].Ptr,"%s %s\n",a,b);}
# define FPrint3(FP,a,b,c)                 {fprintf(File[FP].Ptr,"%s %s %s\n",a,b,c);}

# define FPrintSL(FP,a,L1)                 {fprintf(File[FP].Ptr,"%s %lu\n",a,(ULONG)L1);}
# define FPrint1SL(FP,a,L1)                {fprintf(File[FP].Ptr,"%s %lu\n",a,(ULONG)L1);}
# define FPrint2SL(FP,a,L1,b,L2)           {fprintf(File[FP].Ptr,"%s %lu %s %lu\n",a,(ULONG)L1,b,(ULONG)L2);}
# define FPrint3SL(FP,a,L1,b,L2,c,L3)      {fprintf(File[FP].Ptr,"%s %lu %s %lu %s %lu\n",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3);}
# define FPrint4SL(FP,a,L1,b,L2,c,L3,d,L4) {fprintf(File[FP].Ptr,"%s %lu %s %lu %s %lu %s %lu\n",a,(ULONG)L1,b,(ULONG)L2,c,(ULONG)L3,d,(ULONG)L4);}

# define FPrintL(FP,L1)                    {fprintf(File[FP].Ptr,"%lu\n",(ULONG)L1);}
# define FPrintFault(FP,L1,L2,L3,L4)       {fprintf(File[FP].Ptr,"%lu %lu %ld %ld\n",(ULONG)L1,(ULONG)L2,(LONG)L3,(LONG)L4);}

# define FPrintO(FP,a)                     {fprintf(File[FP].Ptr,"%s",a);}
# define FPrintOSL(FP,a,L1)                {fprintf(File[FP].Ptr,"%s %lu",a,(ULONG)L1);}

# define FPrintUndetect(FP,a,L1,b,c)       {fprintf(File[FP].Ptr,"%s %6lu %s %s\n",a,(ULONG)L1,b,c);}
# define FPrintDetect(FP,a,L1,b,L2,c,d)    {fprintf(File[FP].Ptr,"%s %6lu %s %6lu %s %s\n",a,(ULONG)L1,b,(ULONG)L2,c,d);}
# define FPrint2SLFormatted(FP,a,L1,b,L2)  {fprintf(File[FP].Ptr,"%s %6lu %s %6lu\n",a,(ULONG)L1,b,(ULONG)L2);}

/****************************************************************************/
/** PRINT     ***************************************************************/
/** DEE FILES ***************************************************************/
/****************************************************************************/

# define FPrint4Dee(FP,L1,a,L2,L3,L4,L5); {fprintf(File[FP].Ptr,"%lu %s %lu %lu %lu %lu",(ULONG)L1,a,(ULONG)L2,(ULONG)L3,(ULONG)L4,(ULONG)L5);}
# define FPrint4Gen(FP,L1,a,L2,L3,L4,L5); {fprintf(File[FP].Ptr,"%lu %s %lu %lu %lu %lu",(ULONG)L1,a,(ULONG)L2,(ULONG)L3,(ULONG)L4,(ULONG)L5);}

/****************************************************************************/
/** SLFS ********************************************************************/
/** SLFS ********************************************************************/
/****************************************************************************/

# define Init_SLog()  {fprintf(File[SLF].Ptr,ALTERNATED_BAR"\n");                  \
                   fprintf(File[SLF].Ptr,"o %s [%s]\n",_Program_Name,_Version);\
                   fprintf(File[SLF].Ptr,"o Ideated By :> %s\n",_PPU2);        \
                   fprintf(File[SLF].Ptr,"o Simulation LogFile\n");            \
                   fprintf(File[SLF].Ptr,ALTERNATED_BAR"\n");} 

/****************************************************************************/
/** FLFS ********************************************************************/
/** FLFS ********************************************************************/
/****************************************************************************/

# define Init_FLog()  {fprintf(File[FLF].Ptr,ALTERNATED_BAR"\n");                  \
                   fprintf(File[FLF].Ptr,"o %s [%s]\n",_Program_Name,_Version);\
                   fprintf(File[FLF].Ptr,"o Ideated By :> %s\n",_PPU2);        \
                   fprintf(File[FLF].Ptr,"o Fault Detection LogFile\n");            \
                   fprintf(File[FLF].Ptr,ALTERNATED_BAR"\n");} 

/****************************************************************************/
/** CLFS ********************************************************************/
/** CLFS ********************************************************************/
/****************************************************************************/

# define Init_CLog()  {fprintf(File[CLF].Ptr,ALTERNATED_BAR"\n");                  \
                   fprintf(File[CLF].Ptr,"o %s [%s]\n",_Program_Name,_Version);\
                   fprintf(File[CLF].Ptr,"o Ideated By :> %s\n",_PPU2);        \
                   fprintf(File[CLF].Ptr,"o Command LogFile\n");               \
                   fprintf(File[CLF].Ptr,ALTERNATED_BAR"\n");} 

/****************************************************************************/
/** DLFS ********************************************************************/
/** DLFS ********************************************************************/
/****************************************************************************/

# define Init_DLog()  {fprintf(File[DLF].Ptr,ALTERNATED_BAR"\n");                  \
                   fprintf(File[DLF].Ptr,"o %s [%s]\n",_Program_Name,_Version);\
                   fprintf(File[DLF].Ptr,"o Ideated By :> %s\n",_PPU2);        \
                   fprintf(File[DLF].Ptr,"o PatternOrdered LogFile\n");            \
                   fprintf(File[DLF].Ptr,ALTERNATED_BAR"\n");}

/****************************************************************************/
/** SLFS ********************************************************************/
/** SLFS ********************************************************************/
/****************************************************************************/

# define Save_SLog()  {CHAR CString[STRING_LEN];                                                                \
                   fprintf(File[SLF].Ptr,"Circuit File Name ----------:%s\n",       Circuit.Cir_Name);          \
                   fprintf(File[SLF].Ptr,"Faults  File Name ----------:%s\n",       Circuit.Fau_Name);          \
                   fprintf(File[SLF].Ptr,"Pattern File Name ----------:%s\n",       Circuit.Pat_Name);          \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Primary Inputs -------------:%lu\n",(ULONG)Circuit.Num_PI);           \
                   fprintf(File[SLF].Ptr,"Primary Outputs ------------:%lu\n",(ULONG)Circuit.Num_PO);           \
                   fprintf(File[SLF].Ptr,"Total Flip Flops -----------:%lu\n",(ULONG)Circuit.Num_FF);           \
                   fprintf(File[SLF].Ptr,"Total Gates ----------------:%lu\n",(ULONG)Circuit.Num_Gates);        \
                   fprintf(File[SLF].Ptr,"Total Levels ---------------:%lu\n",(ULONG)Circuit.Num_Levels);       \
                   fprintf(File[SLF].Ptr,"Total Descriptors-----------:%lu\n",(ULONG)Circuit.Num_Descr);        \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Buffer ---------------------:%lu\n",(ULONG)Circuit.Ports[BUFFER]);    \
                   fprintf(File[SLF].Ptr,"And ------------------------:%lu\n",(ULONG)Circuit.Ports[AND]);       \
                   fprintf(File[SLF].Ptr,"Or -------------------------:%lu\n",(ULONG)Circuit.Ports[OR]);        \
                   fprintf(File[SLF].Ptr,"Exor -----------------------:%lu\n",(ULONG)Circuit.Ports[EXOR]);      \
                   fprintf(File[SLF].Ptr,"Not ------------------------:%lu\n",(ULONG)Circuit.Ports[NOT]);       \
                   fprintf(File[SLF].Ptr,"Nand -----------------------:%lu\n",(ULONG)Circuit.Ports[NAND]);      \
                   fprintf(File[SLF].Ptr,"Nor ------------------------:%lu\n",(ULONG)Circuit.Ports[NOR]);       \
                   fprintf(File[SLF].Ptr,"Nexor ----------------------:%lu\n",(ULONG)Circuit.Ports[NEXOR]);     \
                   fprintf(File[SLF].Ptr,"FF -------------------------:%lu\n",(ULONG)Circuit.Ports[FF]);        \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Flip Flop Precharge At -----:%s\n",FF_Init_Mode);                     \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Max Fanin ------------------:%lu\n",(ULONG)Circuit.Max_Fanin);        \
                   fprintf(File[SLF].Ptr,"Max Fanout -----------------:%lu\n",(ULONG)Circuit.Max_Fanout);       \
                   fprintf(File[SLF].Ptr,"Max FF Fanout --------------:%lu\n",(ULONG)Circuit.Max_FF_Fanout);    \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Gain -----------------------:%lu\n",(ULONG)Circuit.Gain);             \
                   fprintf(File[SLF].Ptr,"Events ---------------------:%lu\n",(ULONG)Circuit.Events);           \
                   fprintf(File[SLF].Ptr,"Good Events ----------------:%lu\n",(ULONG)Circuit.Good_Events);      \
                   fprintf(File[SLF].Ptr,"Evaluated Gates ------------:%lu\n",(ULONG)Circuit.Evaluated_Gates);  \
                   fprintf(File[SLF].Ptr,"Evaluated Gates For Second -:%lu\n",(ULONG)Circuit.Event_Rate);       \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Sort Method ----------------:%s\n",       Circuit.Sort_Mode);         \
                   fprintf(File[SLF].Ptr,"Stk@0 ----------------------:%lu\n",(ULONG)Circuit.Num_Stk0);         \
                   fprintf(File[SLF].Ptr,"Stk@1 ----------------------:%lu\n",(ULONG)Circuit.Num_Stk1);         \
                   fprintf(File[SLF].Ptr,"Faults (Min. 1) ------------:%lu\n",(ULONG)Circuit.Num_Faults);       \
                   fprintf(File[SLF].Ptr,"Found Faults ---------------:%lu\n",(ULONG)Circuit.Found_Faults);     \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Fault Coverage -------------:%g%%\n",(Circuit.Coverage));             \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Number Of Patterns ---------:%lu\n",(ULONG)Circuit.Num_Patterns);     \
                   fprintf(File[SLF].Ptr,"Number Of Used Patterns ----:%lu\n",(ULONG)Circuit.Num_Used_Patterns);\
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   SPrint_Mem_Used(CString,Circuit.Mem_FF);                                                     \
                   fprintf(File[SLF].Ptr,"Memory Used For FF ---------:%s\n",       CString);                   \
                   SPrint_Mem_Used(CString,Circuit.Mem_PI);                                                     \
                   fprintf(File[SLF].Ptr,"Memory Used For PI ---------:%s\n",       CString);                   \
                   SPrint_Mem_Used(CString,Circuit.Mem_Circuit);                                                \
                   fprintf(File[SLF].Ptr,"Memory Used For Circuit ----:%s\n",       CString);                   \
                   SPrint_Mem_Used(CString,Circuit.Mem_Fault_List);                                             \
                   fprintf(File[SLF].Ptr,"Memory Used For Fault List -:%s\n",       CString);                   \
                   SPrint_Mem_Used(CString,Circuit.Mem_Faults_FF);                                              \
                   fprintf(File[SLF].Ptr,"Memory Used For Fau-FF List-:%s\n",       CString);                   \
                   SPrint_Mem_Used(CString,Circuit.Mem_Pattern);                                                \
                   fprintf(File[SLF].Ptr,"Memory Used For Pattern ----:%s\n",       CString);                   \
                   SPrint_Mem_Used(CString,Circuit.Mem_Simulation);                                             \
                   fprintf(File[SLF].Ptr,"Memory Used For Simulation -:%s\n",       CString);                   \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                              \
                   fprintf(File[SLF].Ptr,"Start Simulation Time ------:%s\n",       Circuit.Simulation_Start_Time);  \
                   fprintf(File[SLF].Ptr,"End Simulation Time --------:%s\n",       Circuit.Simulation_Stop_Time);   \
                   fprintf(File[SLF].Ptr,BAR"\n");                                                                   \
                   fprintf(File[SLF].Ptr,"Elapsed Simulation Time ----:%s\n",       Circuit.Simulation_Elapsed_Time);\
                   fprintf(File[SLF].Ptr,ALTERNATED_BAR"\n");}

/****************************************************************************/
/** LOGS ********************************************************************/
/** LOGS ********************************************************************/
/****************************************************************************/

# define Log(a,b)         fprintf(File[CLF].Ptr,">%s %s\n",a,b);

#define GoodBye_CLog()   {CHAR    Stringon[STRING_LEN];                                      \
                      clock_t Current_Clock;                                             \
                      time_t  Current_Date;                                              \
                      if(File[CLF].Ptr!=NULL){                                           \
                      Get_Date(Stringon,Program_Start_Date);                             \
                      fprintf(File[CLF].Ptr,ALTERNATED_BAR"\n");                         \
                      fprintf(File[CLF].Ptr,"o Program Started         :%s\n",Stringon); \
                      Start_Date(Current_Date);                                          \
                      Get_Date(Stringon,Current_Date);                                   \
                      fprintf(File[CLF].Ptr,"o Actual Time Is          :%s\n",Stringon); \
                      Get_Clock(Current_Clock);                                          \
                      SPrint_Time(Stringon,Current_Clock);                               \
                      fprintf(File[CLF].Ptr,"o Elapsed Time Since Start:%s\n",Stringon); \
                      fprintf(File[CLF].Ptr,ALTERNATED_BAR"\n");                         \
                      fprintf(File[CLF].Ptr,"o End Of Command Log.\n");                  \
                      fprintf(File[CLF].Ptr,ALTERNATED_BAR"\n");}}

/****************************************************************************/      
/** LOGS ********************************************************************/
/** LOGS ********************************************************************/
/****************************************************************************/

# define Break_FLog()        {if(File[FLF].Ptr!=NULL) fprintf(File[FLF].Ptr,ALTERNATED_BAR"\no Fatal Error Blocking Fault Log.\n"ALTERNATED_BAR);}
# define Break_CLog()        {if(File[CLF].Ptr!=NULL) fprintf(File[CLF].Ptr,ALTERNATED_BAR"\no Fatal Error Blocking Command Log.\n"ALTERNATED_BAR);}
# define Break_DLog()        {if(File[DLF].Ptr!=NULL) fprintf(File[DLF].Ptr,ALTERNATED_BAR"\no Fatal Error Blocking PatternOrdered Log.\n"ALTERNATED_BAR);}
# define Break_SLog()        {if(File[SLF].Ptr!=NULL) fprintf(File[SLF].Ptr,ALTERNATED_BAR"\no Fatal Error Blocking Simulation Log.\n"ALTERNATED_BAR);}

/****************************************************************************/      
/** LOGS ********************************************************************/
/** LOGS ********************************************************************/
/****************************************************************************/

# define User_Quit_Program_CLog() {if(File[CLF].Ptr!=NULL) fprintf(File[CLF].Ptr,ALTERNATED_BAR"\no Program Interrupted By User\n"ALTERNATED_BAR);}
# define User_Quit_Program_FLog() {if(File[FLF].Ptr!=NULL) fprintf(File[FLF].Ptr,ALTERNATED_BAR"\no Program Interrupted By User\n"ALTERNATED_BAR);}
# define User_Quit_Program_DLog() {if(File[DLF].Ptr!=NULL) fprintf(File[DLF].Ptr,ALTERNATED_BAR"\no Program Interrupted By User\n"ALTERNATED_BAR);}
# define User_Quit_Program_SLog() {if(File[SLF].Ptr!=NULL) fprintf(File[SLF].Ptr,ALTERNATED_BAR"\no Program Interrupted By User\n"ALTERNATED_BAR);}

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
