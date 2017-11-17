/*PPU************************************************************************\
*
*    $Source: load_fau.h
*  $Revision: 0.7.1
*      $Date: Thu Jul  8 19:30:57 1999 
*
\************************************************************************PPU*/

typedef CHAR STUCK_AT;

/****************************************************************************/

# define STUCK_AT0  0
# define STUCK_AT1  1

# define STUCK0     STUCK_AT0
# define STUCK1     STUCK_AT1

/****************************************************************************/

void   UpDate_FList(void);

void   Clear_Fault_List(void);
void   Release_Fault_List(void);

FX_RET Create_Fault_List(CHAR* Fault_List_File_Name);

/****************************************************************************/

typedef struct Fault_Des _Fault_Des;
struct Fault_Des
     {
      PATTERN_ID Pattern_Id;

      STUCK_AT   Stk_Value;
      
      CELL_Ptr   Pin;
      Gate_Ptr   Faulty_Gate;
      
     _FF_List*   FFs;
     
      FF_ID      Num_Enqueued_FFs;
     };

/****************************************************************************/

typedef _Fault_Des* Fault_Ptr;

/****************************************************************************/

extern  _Fault_Des* Fault_Descriptor;

extern   Fault_Ptr* Fault_List;  

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
