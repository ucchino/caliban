/*PPU************************************************************************\
*
*    $Source: gatetest.h
*  $Revision: 0.4.9
*      $Date: Thu Jul  8 19:30:50 1999 
*
\************************************************************************PPU*/

# define Gate_Error(Id,Message_Code) {Print2(ERROR_COLOR,SYSTEM_E,Sys_Error_Message_Code[Message_Code]);\
                                      Bar(BAR_COLOR);                                                         \
                                      PrintSL(ERROR_COLOR,"Descriptor Id:",Id);                               \
                                      Print2( ERROR_COLOR,"Type         :",GType[NetList[Id].Type&GATE_MASK]);\
                                      PrintSL(ERROR_COLOR,"Fanin        :",NetList[Id].Fanin);                \
                                      PrintSL(ERROR_COLOR,"Fanout       :",NetList[Id].Fanout);               \
                                      PrintSL(ERROR_COLOR,"Level        :",NetList[Id].Level);                \
                                      PrintSL(ERROR_COLOR,"EnQueued     :",NetList[Id].EnQueued);             \
                                      Bar(BAR_COLOR);}

/****************************************************************************/

# define GatePtr_Error(Ga,Message_Code) {Print2(ERROR_COLOR,SYSTEM_E,Sys_Error_Message_Code[Message_Code]); \
                                         Bar(BAR_COLOR);                                                    \
                                         Print(  ERROR_COLOR,"Descriptor Data");                            \
                                         Print2( ERROR_COLOR,"Type         :",GType[Ga->Type&GATE_MASK]);   \
                                         PrintSL(ERROR_COLOR,"Fanin        :",Ga->Fanin);                   \
                                         PrintSL(ERROR_COLOR,"Fanout       :",Ga->Fanout);                  \
                                         PrintSL(ERROR_COLOR,"Level        :",Ga->Level);                   \
                                         PrintSL(ERROR_COLOR,"EnQueued     :",Ga->EnQueued);                \
                                         Bar(BAR_COLOR);}

/****************************************************************************/

# define Print_Circuit_Pin() {ULONG index_macro1=NONE,index_macro2=NONE;                              \
                              for(index_macro1=NONE;index_macro1 < Circuit.Num_Descr;index_macro1++) \
                              {                                                                      \
                               Bar(BAR_COLOR);                                                       \
                               if((NetList[index_macro1].Type&GATE_MASK) == FF)                      \
                               {                                                                     \
                                if(Cell_Is_One( NetList[index_macro1].Pin_Value[FF_IN])) {Print2SL(MESSAGE_COLOR,"FFGateIn:",index_macro1,"Val:",1);}\
                                if(Cell_Is_Zero(NetList[index_macro1].Pin_Value[FF_IN])) {Print2SL(MESSAGE_COLOR,"FFGateIn:",index_macro1,"Val:",0);}\
                                else                                                                 \
                                {                                                                    \
                                 Print2SL(MESSAGE_COLOR,"FFGateIn:",index_macro1,"Val:","x");        \
                                 PrintSLXX(MESSAGE_COLOR,"As:",index_macro1,(NetList[index_macro1].Pin_Value[FF_IN]).Base,(NetList[index_macro1].Pin_Value[FF_IN]).Corrector);\
                                }                                                                    \
                               }                                                                     \
                               else                                                                  \
                               {                                                                     \
                                for(index_macro2=NONE;index_macro2 < NetList[index_macro1].Fanin;index_macro2++) \
                                {                                                                    \
                                 if(Cell_Is_One( NetList[index_macro1].Pin_Value[index_macro2])) {Print2SL(MESSAGE_COLOR,"FFGateIn:",index_macro1,"Val:",1);}\
                                 if(Cell_Is_Zero(NetList[index_macro1].Pin_Value[index_macro2])) {Print2SL(MESSAGE_COLOR,"FFGateIn:",index_macro1,"Val:",0);}\
                                 else                                                                \
                                 {                                                                   \
                                  Print2SL(MESSAGE_COLOR,"GateIn:",index_macro1,"Val:","x");         \
                                  PrintSLXX(MESSAGE_COLOR,"As:",index_macro1,(NetList[index_macro1].Pin_Value[index_macro2]).Base,(NetList[index_macro1].Pin_Value[index_macro2]).Corrector); \
                                 } \
                                } \
                               } \
                              } \
                             }

/****************************************************************************/

#ifdef FS32BIT

# define Cell_Is_Ics(Pin)  ((Cell_Is_Not_One(Pin)) && (Cell_Is_Not_Zero(Pin)))

# define CK_Pin(Gate)      {ULONG index_macro1=NONE;                                         \
                            if((Gate->Type&GATE_MASK)!=FF)                                   \
                            for(index_macro1=NONE;index_macro1 < Gate->Fanin;index_macro1++) \
                            {                                                                \
                             if((Cell_Is_One(Gate->Pin_Value[index_macro1]))  && (Gate->Pin_Value[index_macro1].RealMan!=QUICK_ONE))  {Print(Normal,"Different Good/Real Value 1");Break(ERROR);}\
                             if((Cell_Is_Zero(Gate->Pin_Value[index_macro1])) && (Gate->Pin_Value[index_macro1].RealMan!=QUICK_ZERO)) {Print(Normal,"Different Good/Real Value 0");Break(ERROR);}\
                             if((Cell_Is_Ics(Gate->Pin_Value[index_macro1]))  && (Gate->Pin_Value[index_macro1].RealMan!=QUICK_ICS))  {Print(Normal,"Different Good/Real Value x");Break(ERROR);}\
                            }                                                                \
                            else                                                             \
                            {                                                                \
                             if((Cell_Is_One(Gate->Pin_Value[0]))  && (Gate->Pin_Value[0].RealMan!=QUICK_ONE))  {Print(Normal,"Different Good/Real Value 1");Break(ERROR);}\
                             if((Cell_Is_Zero(Gate->Pin_Value[0])) && (Gate->Pin_Value[0].RealMan!=QUICK_ZERO)) {Print(Normal,"Different Good/Real Value 0");Break(ERROR);}\
                             if((Cell_Is_Ics(Gate->Pin_Value[0]))  && (Gate->Pin_Value[0].RealMan!=QUICK_ICS))  {Print(Normal,"Different Good/Real Value x");Break(ERROR);}\
                            }                                                                \
                           }
#else
# define CK_Pin(Gate)      ;
#endif

/****************************************************************************/

#ifdef DEBUG
# define About_Gate(Gate) {PrintSL(MESSAGE_COLOR,"Id       :",Gate->Id); \
                           Print2(MESSAGE_COLOR, "Type     :",GType[Gate->Type&GATE_MASK]);}

/****************************************************************************/

# define Check_For_Zombie_Events() {LONG index_macro1=NONE,index_macro2=NONE,index_macro3=NONE;                                \
                                    for(index_macro1=NONE; index_macro1 <= Num_Levels;index_macro1++){                         \
                                    for(index_macro2=NONE; index_macro2 <  Event_List[index_macro1].Last_Gate;index_macro2++){ \
                                    index_macro3++;Print4SL(MESSAGE_COLOR,"Found:",index_macro3,"Zombie Gate/Event(s):",Event_List[index_macro1].Scheduled_Gate[index_macro2]->Id,"At Level:",index_macro1,"On Total:",Event_List[index_macro1].Last_Gate); \
                                    Sys_Error("",E_ZOMBIE_EVENTS);}}if(index_macro3) Break(ERROR);}

/****************************************************************************/

# define Ck_Gate_Level(Gate,Dst) {LONG index_macros1=NONE;                                                                        \
                                  if((Gate->Level >= Gate->To_Gate[Dst]->Level) && ((Gate->To_Gate[Dst]->Type&GATE_MASK) != FF)){ \
                                  HBar(BAR_COLOR);About_Gate(Gate);Bar(BAR_COLOR);                                                \
                                  PrintSL(MESSAGE_COLOR,"==> Son Order Is:",Dst);                                                 \
                                  for(index_macros1=NONE; index_macros1 < Gate->Fanout;index_macros1++){                          \
                                  Bar(BAR_COLOR);About_Gate((Gate->To_Gate[index_macros1]));}                                     \
                                  Sys_Error("",E_EVENT_LEVEL);Print_Circuit();Break(ERROR);}}

/****************************************************************************/

#else
# define About_Gate(Gate)                 ;
# define Check_For_Zombie_Events()        ;
# define Ck_Gate_Level(Gate_Src,Gate_Dst) ;
#endif

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
