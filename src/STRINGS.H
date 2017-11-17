/*PPU************************************************************************\
*
*    $Source: strings.h
*  $Revision: 0.6.10
*      $Date: Thu Jul  8 19:31:06 1999 
*
\************************************************************************PPU*/

# define STRING_LEN    400

# define S(Quantity)   ((Quantity > 1) ? "s" : "")

# define Cmp(a,b)      (!strcmp((a),(b)))
# define CmpN(a,b,c)   (!strncmp((a),(b),(c)))

# define NCmp(a,b)     (strcmp((a),(b)))
# define NCmpN(a,b,c)  (strncmp((a),(b),(c)))

# define ToLower(a)    {INT len=NONE; for(len=NONE;(len < strlen(a));len++) a[len]=tolower(a[len]);}
# define ToUpper(a)    {INT len=NONE; for(len=NONE;(len < strlen(a));len++) a[len]=toupper(a[len]);}

# define Strset(a)     {INT len=NONE; for(len=NONE;(len < strlen(a));len++) a[len]=' ';}

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
