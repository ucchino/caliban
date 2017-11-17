/*PPU************************************************************************\
*
*    $Source: sim.h
*  $Revision: 0.7.1
*      $Date: Thu Jul  8 19:31:06 1999 
*
\************************************************************************PPU*/

/****************************************************************************/
/* Se cerco di allocare solo una parte dello spazio necessario per allocare */
/* i FF nella faultlist devo porre il controllo sullo sforamento del # di   */
/* FF accodati nella faultlist a CHECK_COMPAQ_FF=1. Poiche' la lista accoda */
/* tutti i ff che sono modificati da un guasto nella simulazione del circuit*/
/* guasto.                                                                  */
/*                                                                          */
/* SE "ENQUEUABLE_FFS != (Circuit.Num_FF)" Allora ===> CHECK_COMPAQ_FF 1    */
/*                                                                          */
/****************************************************************************/

# define CHECK_COMPAQ_FF 0
# define ENQUEUABLE_FFS  (Circuit.Num_FF)

/****************************************************************************/

# define THRESHOLDVAL    5

/****************************************************************************/
/** CONDIZIONI DI FUNZIONAMENTO *********************************************/
/****************************************************************************/

#ifdef THRESHOLD
# undef FAULT_QSORT
#endif

/****************************************************************************/

#ifdef FS32BIT
# undef GOODFSIM
#endif

/****************************************************************************/

#ifdef FS32BIT
extern CELL_UNIT AMAM_B[3];
extern CELL_UNIT AMAM_C[3];
#endif

/****************************************************************************/

FX_RET    Sim(void);
void      Sim_First_Time(void);
void      Sim_Other_Time(void);

void      Release_FSim_Mem(void);

void      Reset_FF(void);
void      Precharge_Reset(void);
void      Create_FF_Queue(void);
void      Release_FF_Queue(void);

void      Load_Scheduled_FF(void);
void      Store_Scheduled_FF(void);

void      Drop_Out(void);

void      Inject_Fau(void);
FX_RET    Choose_Fault_Block_FSim(void);
FX_RET    Choose_Fault_Block_GoodSim(void);
CELL_UNIT Excitable_Fault_FSim(FAULT_ID Fau);

void      Inject_Pattern(void);

void      Clear_Event_List(void);
void      Create_Event_List(void);
void      Release_Event_List(void);

void      Clear_Fault_Block(void);
void      Create_Fault_Block(void);
void      Release_Fault_Block(void);

void      Detect_Event_On_PO(void);

void      Schedule_Gate(Gate_Ptr GateP);
void      Schedule_Sons(Gate_Ptr GateP,CELL Value);
void      Schedule_Sons_FSim(Gate_Ptr GateP,CELL Value);

void      Fresh_Sim_Stats(void);
void      Reset_Sim_Stats(void);

CELL      Eval_Boz(Gate_Ptr GateP);

void      QuickSort(void);
void      Create_QuickSort(void);
void      Release_QuickSort(void);

void      PPU_Visit(void);
void      Create_Level_List(void);
void      Release_Level_List(void);

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
