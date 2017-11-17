/*PPU************************************************************************\
*
*    $Source: gengis.h
*  $Revision: 0.6.6
*      $Date: Thu Jul  8 19:30:51 1999 
*
\************************************************************************PPU*/

FX_RET Save_Pattern(CHAR* Pattern_File_Name);
FX_RET Append_Pattern(CHAR* Pattern_File_Name);

FX_RET Gengis(CHAR* Pattern_File_Name,CHAR* Coverage,CHAR* Gengis_Length,CHAR* Gengis_Cycle_Times);
FX_RET Gengis_Multiple_Resets(CHAR* Pattern_File_Name,CHAR* Coverage,CHAR* Gengis_Length,CHAR* Gengis_Cycle_Times,CHAR* Gengis_Resets);

void   Create_Random_Pattern(void);

void   Alloc_Random_Pattern(void);

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
