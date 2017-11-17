/*PPU************************************************************************\
*
*    $Source: gates.h
*  $Revision: 0.7.5
*      $Date: Thu Jul  8 19:30:50 1999 
*
\************************************************************************PPU*/

/****************************************************************************\
*                                                                            *
*                  |---------------|---------------|                         *
*                  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |                         *
*                  |---------------|---------------|                         *
*                    ^   ^   ^   ^   ^   ^       ^                           *
*                    |   |    \_/    |    \_____/                            *
*                    |   |     |     |       |                               *
*                    |   |     |     |       |                               *
*                   Fau  |    Fan    |       |                               *
*                   Out Fau   Inp    |   Gate Type                           *
*                       Inp         Not                                      *
*                                                                            *
\****************************************************************************/
                                  
# define FANG              (0x00)
# define FAN0              (0x00)
# define FAN1              (0x00)

# define FAN2              (0x10)
# define FAN3              (0x20)
# define FAN4              (0x30)

/****************************************************************************/

# define NUM_FAULTS        (0x04)

/****************************************************************************/

# define GATE_MASK         (0x0f)

/****************************************************************************/

# define FAULT_ON_IN       (0x40)
# define FAULT_2_GOOD_IN   (0xbf) /* ~(FAULT_ON_IN)  */

/****************************************************************************/

# define FAULT_ON_OUT      (0x80)
# define FAULT_2_GOOD_OUT  (0x7f) /* ~(FAULT_ON_OUT) */

/****************************************************************************/

# define FAULTY_GATE       (0xc0) /* (FAULT_ON_IN)|(FAULT_ON_OUT)  */
# define GOOD_GATE         (0x3f) /* ~(FAULTY_GATE)                */

/****************************************************************************/

# define GATE_TYPES        (0x0f)

/****************************************************************************/

#undef PI     /** data2.h **/
#undef PO     /** data2.h **/
#undef BUFFER /** data2.h **/
#undef AND    /** data2.h **/
#undef OR     /** data2.h **/
#undef EXOR   /** data2.h **/
#undef FF     /** data2.h **/
#undef NOT    /** data2.h **/
#undef NAND   /** data2.h **/
#undef NOR    /** data2.h **/
#undef NEXOR  /** data2.h **/
#undef CLOCK  /** data2.h **/

/****************************************************************************/

# define FF_IN  0
# define FFF_IN            (FF | FAULT_ON_IN)  /* Fast Fault Dropping Use */

/****************************************************************************/

# define BUFFER            (0x00)       /* 00 */
# define AND               (0x01)       /* 01 */
# define OR                (0x02)       /* 02 */
# define EXOR              (0x03)       /* 03 */
# define FF                (0x04)       /* 04 */
# define PI                (0x05)       /* 05 */

# define UNKNOWN0          (0x06)       /* For Future Use */
# define UNKNOWN1          (0x07)       /* For Future Use */
# define UNKNOWN2          (UNKNOWN0)   /* For Future Use */
# define UNKNOWN3          (UNKNOWN1)   /* For Future Use */

# define NOT               (0x08)       /* 08 */
# define NAND              (AND  | NOT) /* 09 */
# define NOR               (OR   | NOT) /* 10 */
# define NEXOR             (EXOR | NOT) /* 11 */
# define CLOCK             (FF   | NOT) /* 12 */
# define PO                (PI   | NOT) /* 13 */

/****************************************************************************/

# define ANDG              (AND  |FANG)  /* 01 | 00 | 00 = 01 */
# define ORG               (OR   |FANG)  /* 02 | 00 | 00 = 02 */
# define EXORG             (EXOR |FANG)  /* 03 | 00 | 00 = 03 */

# define NANDG             (NAND |FANG)  /* 01 | 00 | 08 = 09 */
# define NORG              (NOR  |FANG)  /* 02 | 00 | 08 = 0A */
# define NEXORG            (NEXOR|FANG)  /* 03 | 00 | 08 = 0B */

# define AND2              (AND  |FAN2)  /* 01 | 10 | 00 = 11 */
# define OR2               (OR   |FAN2)  /* 02 | 10 | 00 = 12 */
# define EXOR2             (EXOR |FAN2)  /* 03 | 10 | 00 = 13 */

# define NAND2             (NAND |FAN2)  /* 01 | 10 | 08 = 19 */
# define NOR2              (NOR  |FAN2)  /* 02 | 10 | 08 = 1A */
# define NEXOR2            (NEXOR|FAN2)  /* 03 | 10 | 08 = 1B */

# define AND3              (AND  |FAN3)  /* 01 | 20 | 00 = 20 */
# define OR3               (OR   |FAN3)  /* 02 | 20 | 00 = 21 */
# define EXOR3             (EXOR |FAN3)  /* 03 | 20 | 00 = 22 */

# define NAND3             (NAND |FAN3)  /* 01 | 20 | 08 = 29 */
# define NOR3              (NOR  |FAN3)  /* 02 | 20 | 08 = 2A */
# define NEXOR3            (NEXOR|FAN3)  /* 03 | 20 | 08 = 2B */

# define AND4              (AND  |FAN4)  /* 01 | 30 | 00 = 30 */
# define OR4               (OR   |FAN4)  /* 02 | 30 | 00 = 31 */
# define EXOR4             (EXOR |FAN4)  /* 03 | 30 | 00 = 32 */
                                             
# define NAND4             (NAND |FAN4)  /* 01 | 30 | 08 = 39 */
# define NOR4              (NOR  |FAN4)  /* 02 | 30 | 08 = 3A */
# define NEXOR4            (NEXOR|FAN4)  /* 03 | 30 | 08 = 3B */

/****************************************************************************/

# define PORTS             (GATE_MASK+1)

/****************************************************************************/

extern CHAR* GType[PORTS];
extern CHAR* Attr[NUM_FAULTS];

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
