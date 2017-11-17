/*PPU************************************************************************\
*
*    $Source: fauman.h
*  $Revision: 0.7.4
*      $Date: Thu Jul  8 19:30:48 1999 
*
\************************************************************************PPU*/

FX_RET Sort_By_Gate(INT Model);
FX_RET Sort_By_Type(INT Model);
FX_RET Sort_By_Level(INT Model);

FX_RET Create_Single_Gate_Fault(INT Model,GATE_ID Id);

FX_RET Create_Group_Fault(INT Fault_Model,INT Sorting);

FX_RET Fauman(CHAR* Collapse,CHAR* Fault_Model,CHAR* Sorting,CHAR *New_File);

/****************************************************************************/

# define MSort_1  "Gate#"   
# define MSort_2  "Type "
# define MSort_3  "Level"

# define Sort_1    1
# define Sort_2    2
# define Sort_3    3

# define MModel_1 "Out  "
# define MModel_2 "In   "
# define MModel_3 "InOut"
# define MModel_4 "<BYE>"

# define Model_1   1
# define Model_2   2
# define Model_3   3
# define Model_4   4


# define MCollapse_1 "UnCollapse"
# define MCollapse_2 "Collapse  "

# define Collapse_1  1
# define Collapse_2  2

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
