/*PPU************************************************************************\
*
*    $Source: types.h
*  $Revision: 0.6.8
*      $Date: Thu Jul  8 19:31:08 1999 
*
\************************************************************************PPU*/

# define NONE           0

# define TRUE           1
# define FALSE          NONE

# define BIT            1

# define BYTE           8

/****************************************************************************/

typedef  signed         int     INT;
typedef                 char    CHAR;
typedef  signed         char    SCHAR;
typedef  signed   long  int     LONG;
typedef                 float   FLOAT;
typedef                 double  DOUBLE;

/****************************************************************************/

typedef  unsigned       int     UINT;
typedef  unsigned       char    UCHAR;
typedef  unsigned long  int     ULONG;

/****************************************************************************/

typedef  signed         int     FX_RET;

/****************************************************************************/

typedef  LONG           CELL_UNIT;

# define C(Val)         ((CELL_UNIT)(Val))

/****************************************************************************/

# undef  ONE
# undef  ZERO

# define ZERO           C(0L)
# define ONE            C(~(ZERO))
# define RESET          C(0x55555555)

/****************************************************************************/

# define FF_ZERO        0
# define FF_ONE         1
# define FF_ICS         2

/****************************************************************************/

# define PATTERN_ZERO   0
# define PATTERN_ONE    1
# define PATTERN_ICS    2
# define PATTERN_RESET  3

/****************************************************************************/

# define ICS0_STRING   "X"
# define ICS1_STRING   "x"
# define ICS2_STRING   "2"
# define ICS3_STRING   "-"
# define ONE_STRING    "1"
# define ZERO_STRING   "0"
# define RESET_STRING  "#"

# define ICS0_CHAR     'X'
# define ICS1_CHAR     'x'
# define ICS2_CHAR     '2'
# define ICS3_CHAR     '-'
# define ONE_CHAR      '1'
# define ZERO_CHAR     '0'
# define RESET_CHAR    '#'

/****************************************************************************/

# define UNKNOWN       ONE
# define   KNOWN       ZERO

/****************************************************************************/

# define DEFAULT_FF_INIT_VALUE FF_ZERO
# define DEFAULT_FF_INIT_MODE  ZERO_STRING
# define USER_FF_INIT_MODE     "User_Defined"

/****************************************************************************/

typedef UCHAR GATE_TYPE;
typedef UINT  GATE_FAN;
typedef UINT  GATE_LEVEL;

/****************************************************************************/

                          /***************************************/
typedef GATE_FAN FF_ID;   /* 0..2^n Same Type as Gate Fan Why... */
                          /* Memorizzo nel Fanout del FF, che e' */
                          /* infatti sempre 1,il # d'ordine del  */
                          /* ff stesso per kasini miei           */
                          /***************************************/

/****************************************************************************/

typedef ULONG GATE_ID;
typedef ULONG FAULT_ID;
typedef ULONG PATTERN_ID;

/****************************************************************************/

# define SIGNED     1
# define UNSIGNED   0

/****************************************************************************/

# define MAX_FF             (((FF_ID)~0) >> UNSIGNED)
# define MAXBIT_FF          ((sizeof(FF_ID)*BYTE)-UNSIGNED)

# define MAX_FAN            (((GATE_FAN)~0) >> UNSIGNED)
# define MAXBIT_FAN         ((sizeof(GATE_FAN)*BYTE)-UNSIGNED)

# define MAX_GENES          (((ULONG)~0) >> UNSIGNED)
# define MAXBIT_GENES       ((sizeof(ULONG)*BYTE)-UNSIGNED)

# define MAX_GATES          (((GATE_ID)~0) >> UNSIGNED)
# define MAXBIT_GATES       ((sizeof(GATE_ID)*BYTE)-UNSIGNED)

# define MAX_LEVEL          (((GATE_LEVEL)~0) >> UNSIGNED)
# define MAXBIT_LEVEL       ((sizeof(GATE_LEVEL)*BYTE)-UNSIGNED)

# define MAX_FAULTS         (((FAULT_ID)~0) >> UNSIGNED)
# define MAXBIT_FAULTS      ((sizeof(FAULT_ID)*BYTE)-UNSIGNED)

# define MAX_PATTERNS       (((PATTERN_ID)~0) >> UNSIGNED)
# define MAXBIT_PATTERNS    ((sizeof(PATTERN_ID)*BYTE)-UNSIGNED)

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
