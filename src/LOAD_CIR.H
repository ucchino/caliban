/*PPU************************************************************************\
*
*    $Source: load_cir.h
*  $Revision: 0.7.7
*      $Date: Thu Jul  8 19:30:56 1999 
*
\************************************************************************PPU*/

# define Visible   TRUE
# define InVisible FALSE

/****************************************************************************/

FX_RET Create_PI(void);
void   Release_PI(void);

FX_RET Create_PO(void);
FX_RET Create_LV(void);

FX_RET Create_FF(void);
void   Release_FF(void);

void   Init_FFs(CHAR* FF_Init_Mode);
void   File_Init_FFs(CHAR* FF_Init_File_Name);

void   Keep_Circuit(void);
void   Clear_Circuit(void);
FX_RET Check_Circuit(void);
void   Release_Circuit(void);
FX_RET Create_Cal_Circuit(CHAR* NetList_File_Name);

FX_RET Create_To_Pin_Links(void);
void   Give_Gates_For_Levels(void);
FX_RET Create_Gates_For_Level(void);

void   Load_Cal_Header(CHAR Mode);

void   Print_PI(void);
void   Print_FF(void);
void   Print_Scan_Chain_FF(void);

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
