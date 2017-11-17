/*PPU************************************************************************\
*
*    $Source: struct.h
*  $Revision: 0.6.10
*      $Date: Thu Jul  8 19:31:06 1999 
*
\************************************************************************PPU*/

#ifdef FS32BIT
# define Bits               ((sizeof(CELL_UNIT)*BYTE))
#else
# define Bits               ((sizeof(CELL_UNIT)*BYTE)-1)
#endif

# define MAXBIT_PARALLELISM (Bits)

/****************************************************************************/

typedef struct THREE_VAL CELL;
struct THREE_VAL
     {
      CELL_UNIT Base;
      CELL_UNIT Corrector;

#ifdef PPU_VISIT
      CHAR      DontCare;
#endif

#ifdef FS32BIT
      CHAR      RealMan;
#endif
     };

typedef CELL*     CELL_Ptr;

/****************************************************************************/
/****************************************************************************/

typedef  struct Gate_Struct _Gate;
typedef _Gate*               Gate_Ptr;

/****************************************************************************/
/** "*" Identifica Sempre Un Vettore ****************************************/
/****************************************************************************/

struct Gate_Struct                  /**************************************/
     {                              /***                                ***/
      GATE_TYPE  Type;              /*** Evaluation                     ***/
      GATE_FAN   Fanin;             /*** Evaluation                     ***/
      GATE_FAN   Fanout;            /*** Propagation                    ***/
      GATE_LEVEL Level;             /*** Gate Level                     ***/
                                    /***                                ***/
      CELL       Out_Value;         /*** Evaluation  OUTPUT VALUE       ***/
                                    /***                                ***/
      CELL*      Pin_Value;         /*** Evaluation  INPUT VALUE        ***/
                                    /***                                ***/
      CELL_Ptr*  To_Pin;            /*** Propagation DESTINATION PIN    ***/
      Gate_Ptr*  To_Gate;           /*** Propagation DESTINATION GATE   ***/
                                    /***                                ***/
      CHAR       EnQueued;          /*** Propagation ALMOST ENQUEUED    ***/
                                    /***                                ***/
#ifdef DEBUG                        /***                                ***/
      GATE_ID    Id;                /*** Debug Only Dear Boz            ***/
#endif                              /***                                ***/
                                    /***                                ***/
     };                             /**************************************/

/****************************************************************************/

typedef struct FlipFlop_Struct _FF_List;
struct FlipFlop_Struct
     {
      CHAR     FF_Value;
      Gate_Ptr FF_Gate;
     };

/****************************************************************************/

typedef struct Event_Struct _Event_List;
struct Event_Struct
     {
      Gate_Ptr* Scheduled_Gate;
      GATE_ID   Last_Gate;
     };

/****************************************************************************/

typedef struct Circuit_Description  _Circuit;
struct Circuit_Description
     {
      CHAR Name[MAXFNLEN];

      CHAR Cir_Name[MAXFNLEN];
      CHAR Fau_Name[MAXFNLEN];
      CHAR Pat_Name[MAXFNLEN];

      GATE_ID Num_PI;
      GATE_ID Num_PO;
      GATE_ID Num_Descr;
      GATE_ID Num_Gates;

      GATE_LEVEL Num_Levels;
      
      FF_ID Num_FF;

      UINT Gain;
      UINT Event_Rate;
      
      ULONG Events;
      ULONG Good_Events;
      ULONG Evaluated_Gates;
      
      GATE_ID* Gates_For_Level;

      ULONG Ports[PORTS];

      FAULT_ID Num_Stk0;
      FAULT_ID Num_Stk1;
      FAULT_ID Num_Faults;
      FAULT_ID Found_Faults;
      
      DOUBLE Coverage;
      
      CHAR  Sort_Mode[STRING_LEN];

      GATE_FAN Max_Fanin;
      GATE_FAN Max_Fanout;
      GATE_FAN Max_FF_Fanout;
      
      PATTERN_ID Num_Patterns;
      PATTERN_ID Num_Used_Patterns;

      ULONG  Mem_FF;
      ULONG  Mem_PI;
      ULONG  Mem_Circuit;

      ULONG  Mem_Fault_List;
      ULONG  Mem_Faults_FF;

      ULONG  Mem_Pattern;

      ULONG  Mem_Simulation;

      CHAR  Simulation_Stop_Time[STRING_LEN];
      CHAR  Simulation_Start_Time[STRING_LEN];
      CHAR  Simulation_Elapsed_Time[STRING_LEN];
     };

/****************************************************************************/

extern _Circuit     Circuit;
extern _Gate*       NetList;

extern  CELL*       FF_Init;

extern  Gate_Ptr*   FF_Vect;
extern  Gate_Ptr*   PI_Vect;

extern _Event_List* Event_List;

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
