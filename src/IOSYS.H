/*PPU************************************************************************\
*
*    $Source: iosys.h
*  $Revision: 0.7.5
*      $Date: Thu Jul  8 19:33:31 1999 
*
\************************************************************************PPU*/

# define ERROR              -1
# define OK                  0

/****************************************************************************/

# define E_OPEN              0
# define E_CREATE            1
# define E_APPEND            2
# define E_CLOSE             3

/****************************************************************************/

# define E_DELETE            4
# define E_REW               5
# define E_SET_FILE_PTR      6
# define E_FSEEK             7
# define E_ALREADYOPEN       8
# define E_NULL_FILE_PTR     9
# define E_FILENAMELEN       10

/****************************************************************************/

# define E_ALLOC             11
# define E_FREE              12
# define E_ALLOC_ZERO        13
# define E_CK_NULL_PTR       14
# define E_NOT_NULL_PTR      15

/****************************************************************************/

# define E_DEE_VER           16
# define E_CAL_VER           17
# define E_FAU_VER           18
# define E_GEN_VER           19

/****************************************************************************/

# define E_PO_PO             20
# define E_PI_PI             21
# define E_FF_FF             22
# define E_GATES_GATES       23
# define E_LEVELS_LEVELS     24
# define E_DES_DES           25

/****************************************************************************/

# define E_UNK_GATE          26
# define E_UNK_CLOCK         27
# define E_COMB_CIR          28

/****************************************************************************/

# define E_FF_LEVEL          29
# define E_NUM_FF            30

/****************************************************************************/

# define E_FAN_PI            31
# define E_FAN_PO            32
# define E_SINGLE_IN         33
# define E_LOW_FANIN         34
# define E_NO_FAN            35
# define E_HIGH_FAN          36
# define E_HIGH_GATES        37

/****************************************************************************/

# define E_LEVEL_PI          38
# define E_HIGH_LEVEL        39
# define E_GATE_LEVEL        40
# define E_OUT_TO_A_PI       41
# define E_CIRCUIT_LEVELS    42
# define E_WIRED_OR          43
# define E_NO_GATES_ON_LEVEL 44

/****************************************************************************/

# define E_WRONG_ID          45
# define E_CIRCUIT           46

/****************************************************************************/

# define E_NOT_AVAIB         47

/****************************************************************************/

# define E_PATTERN_VALUE     48
# define E_PATTERN_LENGHT    49
# define E_PATTERN_EMPTY     50

/****************************************************************************/

# define E_FAU_CONNECTED     51
# define E_NO_FAULTS         52
# define E_FAU_ON_PO         53

/****************************************************************************/

# define E_NUM_ARG           54

/****************************************************************************/

# define E_EVENT_ON_CLOCK    55
# define E_ZOMBIE_EVENTS     56
# define E_EVENT_UNK         57
# define E_EVENT_LEVEL       58
# define E_TOO_EVENTS        59

/****************************************************************************/

# define E_NO_PI             60

/****************************************************************************/

# define E_PTR_NUM           61

/****************************************************************************/

# define E_EDF_LOAD          62
# define E_EDF_PI            63

/****************************************************************************/

# define E_LOGIC_LEVEL       64

/****************************************************************************/

# define E_FAU_STORE_FF      65

/****************************************************************************/

# define SYSTEM_E         "(Sys Error)==>"
# define SYSTEM_M         "(Sys Message)==>"

# define Sys_Error(Who,Message_Code) {Bar(BAR_COLOR);Print3(ERROR_COLOR,SYSTEM_E,Sys_Error_Message_Code[Message_Code],Who);Bar(BAR_COLOR);}

/****************************************************************************/

void Sys(void);

/****************************************************************************/

extern CHAR* Sys_Error_Message_Code[];

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
