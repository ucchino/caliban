/*PPU************************************************************************\
*
*    $Source: 3values.h
*  $Revision: 0.4.4
*      $Date: Thu Jul  8 19:30:40 1999 
*
\************************************************************************PPU*/

# define SHIFT_BASE                     (Bits-2)
# define SHIFT_CORRECTOR                (Bits-1)

/****************************************************************************/
/********* DIRECT CELL MODIFICATION *****************************************/
/****************************************************************************/

#ifdef FS32BIT

# define QUICK_ZERO                     1 /* 01b */
# define QUICK_ONE                      2 /* 10b */
# define QUICK_ICS                      0 /* 00b */

/****************************************************************************/

# define Restore_Cell(Pin)              {(Pin.Base)      =((((CELL_UNIT)Pin.RealMan)<<SHIFT_BASE)     >>(Bits-1));\
                                         (Pin.Corrector) =((((CELL_UNIT)Pin.RealMan)<<SHIFT_CORRECTOR)>>(Bits-1));}

# define Copy_Cell(Dest,Src)            {(Dest.Base)     =(Src.Base);     \
                                         (Dest.Corrector)=(Src.Corrector);\
                                         (Dest.RealMan)  =(Src.RealMan);}

# define Copy_Part_Of_Cell(Dest,Src)    {(Dest.Base)     =(Src.Base);     \
                                         (Dest.Corrector)=(Src.Corrector);}

# define Different_Good_Cell(Pin)       (((Pin.Base)     ^((((CELL_UNIT)Pin.RealMan)<<SHIFT_BASE)     >>(Bits-1)))| \
                                         ((Pin.Corrector)^((((CELL_UNIT)Pin.RealMan)<<SHIFT_CORRECTOR)>>(Bits-1))))

# define Set_Cell(Pin)                  {(Pin.Base)     =(ONE) ;\
                                         (Pin.Corrector)=(ZERO);\
                                         (Pin.RealMan)  =(QUICK_ONE);}

# define Ics_Cell(Pin)                  {(Pin.Base)     =(ZERO);\
                                         (Pin.Corrector)=(ZERO);\
                                         (Pin.RealMan)  =(QUICK_ICS);}

# define Clear_Cell(Pin)                {(Pin.Base)     =(ZERO);\
                                         (Pin.Corrector)=(ONE); \
                                         (Pin.RealMan)  =(QUICK_ZERO);}
#else

# define Restore_Cell(Pin)              {(Pin.Base)     >>=(Bits);\
                                         (Pin.Corrector)>>=(Bits);}

# define Copy_Cell(Dest,Src)            {(Dest.Base)     =(Src.Base);\
                                         (Dest.Corrector)=(Src.Corrector);}

# define Copy_Part_Of_Cell(Dest,Src)    {(Dest.Base)     =(Src.Base);\
                                         (Dest.Corrector)=(Src.Corrector);}

# define Different_Good_Cell(Pin)       (((Pin.Base)     ^((Pin.Base)     >>Bits)) | \
                                         ((Pin.Corrector)^((Pin.Corrector)>>Bits)))

# define Set_Cell(Pin)                  {(Pin.Base)=(ONE);\
                                         (Pin.Corrector)=(ZERO);}

# define Ics_Cell(Pin)                  {(Pin.Base)=(ZERO);\
                                         (Pin.Corrector)=(ZERO);}

# define Clear_Cell(Pin)                {(Pin.Base)=(ZERO);\
                                         (Pin.Corrector)=(ONE) ;}
#endif

/****************************************************************************/
/********* MASKS ************************************************************/
/****************************************************************************/

# define Mask_Bit1(Pos)                 (C(1) << (Pos))
# define Mask_Bit0(Pos)                 (~(Mask_Bit1(Pos)))

/****************************************************************************/
/********* BIT OPERATIONS ***************************************************/
/****************************************************************************/

# define SetBit(Pin,Pos)                ((Pin)|=(Mask_Bit1(Pos)))
# define ClearBit(Pin,Pos)              ((Pin)&=(Mask_Bit0(Pos)))

# define SetBit_Cell(Pin,Pos)           {SetBit(Pin.Base,Pos)  ;ClearBit(Pin.Corrector,Pos);}
# define IcsBit_Cell(Pin,Pos)           {ClearBit(Pin.Base,Pos);ClearBit(Pin.Corrector,Pos);}
# define ClearBit_Cell(Pin,Pos)         {ClearBit(Pin.Base,Pos);SetBit(Pin.Corrector,Pos);}

/****************************************************************************/
/********* QUERY ************************************************************/
/****************************************************************************/

# define Bit_Is_One(Pin,Pos)            ((Pin)&(Mask_Bit1(Pos)))

# define Cell_Is_One(Pin)               (((Pin.Base)==ONE))
# define Cell_Is_Zero(Pin)              (((Pin.Corrector)==(ONE)))

# define Cell_Is_Not_One(Pin)           (((Pin.Base)==(ZERO)))
# define Cell_Is_Not_Zero(Pin)          (((Pin.Corrector)==(ZERO)))

# define Cell_Is_Ics(Pin)               ((Cell_Is_Not_One(Pin)) && (Cell_Is_Not_Zero(Pin)))

# define Cell_Bit_Is_One(Pin,Pos)       ((Pin.Base)&(Mask_Bit1(Pos)))
# define Cell_Bit_Is_Zero(Pin,Pos)      ((Pin.Corrector)&(Mask_Bit1(Pos)))

# define Different_Cells(PinA,PinB)     (((PinA.Base)^(PinB.Base)) | ((PinA.Corrector)^(PinB.Corrector)))

/****************************************************************************/
/********* PPU VISIT ********************************************************/
/****************************************************************************/

#ifdef PPU_VISIT
# define Active(Pin)                    ((Pin.DontCare)=(TRUE))
# define Is_Active(Pin)                 ((Pin.DontCare)==(TRUE))
# define Is_Active_Ptr(Pin)             ((Pin->DontCare)==(TRUE))

# define Inactive(Pin)                  ((Pin.DontCare)=(FALSE))
# define Is_Inactive(Pin)               ((Pin.DontCare)==(FALSE))
# define Is_Inactive_Ptr(Pin)           ((Pin->DontCare)==(FALSE))
#else
# define Is_Inactive(Pin)               ;
# define Is_Inactive_Ptr(Pin)           ;
# define Is_Active(Pin)                 ;
# define Is_Active_Ptr(Pin)             ;
# define Inactive(Pin)                  ;
# define Active(Pin)                    ;
#endif

/****************************************************************************/
/********* SPARE OPERATION ON CELLS *****************************************/
/****************************************************************************/

# define Get_Cell_Base(Pin)             ((Pin.Base))
# define Inc_Cell(Pin)                  {(Pin.Base)++;(Pin.Corrector)++;}

/****************************************************************************/
/********* LOGICAL OPERATION ON CELLS ***************************************/
/****************************************************************************/

# define AND_OP(PinA,PinB)              {(PinA.Base)&=(PinB.Base),((PinA.Corrector)|=(PinB.Corrector));}

# define OR_OP(PinA,PinB)               {(PinA.Base)|=(PinB.Base),((PinA.Corrector)&=(PinB.Corrector));}

# define EXOR_OP(PinA,PinB)             {register CELL_UNIT Verifier;                                             \
                                         Verifier=(((PinA.Base)|(PinA.Corrector))&((PinB.Base)|(PinB.Corrector)));\
                                         (PinA.Base)^=(PinB.Base);                                                \
                                         (PinA.Corrector)^=(PinB.Corrector);                                      \
                                         (PinA.Corrector)=~(PinA.Corrector);                                      \
                                         (PinA.Base)&=(Verifier),(PinA.Corrector)&=(Verifier);}

# define NOT_OP(Pin)                    {register CELL_UNIT Aux; (Aux)=(Pin.Base),(Pin.Base)=(Pin.Corrector),(Pin.Corrector)=(Aux);}

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
