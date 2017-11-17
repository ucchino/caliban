/*PPU************************************************************************\
*
*    $Source: iosys.c
*  $Revision: 0.7.6
*      $Date: Thu Jul  8 19:30:53 1999 
*
\************************************************************************PPU*/

# include "libs.h"

/****************************************************************************/

CHAR *Sys_Error_Message_Code[]={"System Failed To Open:"                           ,             /* E_OPEN              0  */
                          "System Failed To Create:"                         ,             /* E_CREATE            1  */
                          "System Failed To Append Data To The End Of:"      ,             /* E_APPEND            2  */
                          "System Failed To Close:"                          ,             /* E_CLOSE             3  */
                          "System Failed To Delete:"                         ,             /* E_DELETE            4  */

/****************************************************************************/

                          "System Cannot Rewind The File:"                   ,             /* E_FREW              5  */
                          "System Cannot ReSet File Pointer"                 ,             /* E_SET_FILE_PTR      6  */
                          "FSEEK File Pointer Fatal Error"                   ,             /* E_FSEEK_PTR         7  */
                          "System Attempt To Open an Already Opened File"    ,             /* E_ALREADYOPEN       8  */
                          "System Attempt To Operate On a Null File Pointer:",             /* E_NULL_FILE_PTR     9  */
                          "Wrong File Name"                                  ,             /* E_FILENAMELEN       10 */

/****************************************************************************/

                          "System Failed To Alloc Requested Memory For:"               ,   /* E_ALLOC             11 */
                          "System Attempt To Free a Null Pointer:"                     ,   /* E_FREE              12 */
                          "Passed a Zero/Negative Value To Memory-Alloc Manager For =>",   /* E_ALLOC_ZERO        13 */
                          "This Pointer is NULL:"                                      ,   /* E_CK_NULL_PTR       14 */
                          "System Attempt To Operate On A NOT-NULL Memory Pointer:"    ,   /* E_NOT_NULL_PTR      15 */  

/****************************************************************************/

                          "Is An Unsupported <DEE> File Version... Please Call Author",    /* E_DEE_VER           16 */
                          "Is An Unsupported <CAL> File Version... Please Call Author",    /* E_CAL_VER           17 */
                          "Is An Unsupported <FAU> File Version... Please Call Author",    /* E_FAU_VER           18 */
                          "Is An Unsupported <GEN> File Version... Please Call Author",    /* E_GEN_VER           19 */

/****************************************************************************/

                          "Difference Between Calculated And Given Number Of PO"         , /* E_PO_PO             20 */
                          "Difference Between Calculated And Given Number Of PI"         , /* E_PI_PI             21 */
                          "Difference Between Calculated And Given Number Of FF"         , /* E_FF_FF             22 */
                          "Difference Between Calculated And Given Number Of Gates"      , /* E_GATES_GATES       23 */
                          "Difference Between Calculated And Given Number Of Levels"     , /* E_LEVELS_LEVELS     24 */
                          "Difference Between Calculated And Given Number Of Descriptors", /* E_DES_DES           25 */ 

/****************************************************************************/

                          "Unknown Gate Type Found"                       ,                /* E_UNK_GATE          26 */
                          "Unable To Found Clock Descriptor In Dee File"  ,                /* E_UNK_CLOCK         27 */
                          "Combinatory Circuit To Simulate (NO FlipFlops)",                /* E_COMB_CIR          28 */

/****************************************************************************/

                          "FlipFlop With Wrong Level",                                     /* E_FF_LEVEL          29 */
                          "Too Many FF Descriptors"  ,                                     /* E_NUM_FF            30 */
                                                                                                                    
/****************************************************************************/

                          "Non-Zero Fanin On A Primary Input"             ,                /* E_FAN_PI            31 */
                          "Non-Zero Fanout On A Primary Output"           ,                /* E_FAN_PO            32 */
                          "Wrong Fanin On Single-Input Gate"              ,                /* E_SINGLE_IN         33 */
                          "Too Low Fanin"                                 ,                /* E_LOW_FANIN         34 */
                          "No Fanin And/Or No Fanout Found (Isolate Gate)",                /* E_NO_FAN            35 */
                          "Raised Max Permitted Fanin Or Fanout"          ,                /* E_HIGH_FAN          36 */
                          "Raised Max Permitted Number Of Gates"          ,                /* E_HIGH_GATES        37 */

/****************************************************************************/

                          "Non-Zero Level On a PI"                       ,                 /* E_LEVEL_PI          38 */
                          "Raised Max Permitted Level"                   ,                 /* E_HIGH_LEVEL        39 */
                          "Gate Input From Gate With A Minor Level"      ,                 /* E_GATE_LEVEL        40 */
                          "Gate Output To a Primary Input"               ,                 /* E_OUT_TO_A_PI       41 */
                          "Non Existing Circuit Level"                   ,                 /* E_CIRCUIT_LEVELS    42 */
                          "Wired-OR Logic, Two gates points to same pin" ,                 /* E_WIRED_OR          43 */
                          "No Gates On A Level"                          ,                 /* E_NO_GATES_ON_LEVEL 44 */

/****************************************************************************/

                          "Non Existing Gate ID"     ,                                     /* E_WRONG_ID          45 */
                          "WRONG CIRCUIT To Simulate",                                     /* E_CIRCUIT           46 */  

/****************************************************************************/

                          "UNAVAIBLE Function:",                                           /* E_NOT_AVAIB         47 */

/****************************************************************************/

                          "Invalid Pattern Value (Not 0/1/#)"                  ,           /* E_PATTERN_VALUE     48 */
                          "Invalid Pattern Lenght (Not Equal To Circuit Fanin)",           /* E_PATTERN_LENGHT    49 */
                          "Invalid Pattern Line. Empty Line"                   ,           /* E_PATTERN_EMPTY     50 */ 
                                                                                                  
/****************************************************************************/

                          "Bad Fault. Descriptors Are Not Connected"  ,                    /* E_FAU_CONNECTED     51 */
                          "No Faults - FAULT SIMULATION UNNECESSARY !",                    /* E_NO_FAULTS         52 */
                          "Fault On A PO Found -- UNTESTABLE"         ,                    /* E_FAU_ON_PO         53 */

/****************************************************************************/

                          "Wrong Number Of Arguments",                                     /* E_NUM_ARG           54 */

/****************************************************************************/

                          "Detected An Event On Clock Descriptor"              ,           /* E_EVENT_ON_CLOCK    55 */
                          "Unespected Events On A Clean Event List Found"      ,           /* E_ZOMBIE_EVENTS     56 */
                          "NOT HANDLED EVENT (Out_FF/Undefined Gate)"          ,           /* E_EVENT_UNK         57 */
                          "Scheduled Event With Wrong Level - OVERFLOW ALLOWED",           /* E_EVENT_LEVEL       58 */
                          "Event List OVERFLOW"                                ,           /* E_TOO_EVENTS        59 */

/****************************************************************************/

                          "NOT EXISTING PI IN THIS CIRCUIT",                               /* E_NO_PI             60 */

/****************************************************************************/

                          "Gate Which Type Is Not Equal To Requested Found",               /* E_PTR_NUM           61 */

/****************************************************************************/

                          "Fenice-Library Failed To Load EDF-Format Circuit",              /* E_EDF_LOAD          62 */
                          "Fenice-Found A Non-BUFFER Gate On A PI (STOP!) [PI MUST BE A BUFFER]",
                                                                                           /* E_EDF_PI            63 */
/****************************************************************************/

                          "Unknown Logic Level (1,0,X) Found...",                          /* E_LOGIC_LEVEL       64 */

/****************************************************************************/

                          "Tring To Store Too Many FFs, Enlarge Fault_List.FFs[]"};        /* E_FAU_STORE_FF      65 */

/****************************************************************************/
/*                                                                          */
/* Fx Name: Sys                                                             */
/* Param. : void                                                            */
/* Return : void                                                            */
/* Written: By PPU !                                                        */
/* LastMod: 31/12/98                                                        */
/*                                                                          */
/****************************************************************************/

void Sys(void)
{
 CHAR Command[STRING_LEN];

 Strset(Command);
 ForEver()
 {
  Print2(MESSAGE_COLOR,System,"[Type x to Exit]:>");
  gets(Command);
  if(Cmp(Command,""))  gets(Command);
  if(Cmp(Command,"x")) break;
  system(Command);
 }
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
