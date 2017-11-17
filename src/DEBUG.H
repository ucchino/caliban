/*PPU************************************************************************\
*
*    $Source: debug.h
*  $Revision: 0.7.1
*      $Date: Thu Jul  8 19:30:44 1999 
*
\************************************************************************PPU*/

#ifdef DEBUG
# define DEBUG_S                    "(Debugging Mode)"
# define BarD(BAR_COLOR);            Bar(BAR_COLOR);
# define MARK_FX()                  {Mark();}
#ifdef WHEREIS
# define WHERE(Fx)                  {CHAR _Fx[]="<B<"__FILE__">B> Begin FX:("Fx")";BarD(BAR_COLOR);Print2(Normal,_Fx,DEBUG_S);}
# define OUTHERE(Fx)                {CHAR _Fx[]="<E<"__FILE__">E> End   FX:("Fx")";                Print2(Normal,_Fx,DEBUG_S);BarD(BAR_COLOR);}
# define NWHERE(Fx)                 ;                                         
#endif
#else
# define BarD(BAR_COLOR);           ;
# define MARK_FX()                  ;
# define WHERE(Fx)                  ;
# define OUTHERE(Fx)                ;
# define NWHERE(Fx)                 ;
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
