/*PPU************************************************************************\
*
*    $Source: files.h
*  $Revision: 0.7.4
*      $Date: Thu Jul  8 19:30:49 1999 
*
\************************************************************************PPU*/

FX_RET Fopen(CHAR* Name,INT Id,INT Ext,INT Mode,INT Error);

/****************************************************************************/

# define OPEN_R    0
# define CREATE_W  1
# define APPEND_W  2

/****************************************************************************/

# define SEEK_FROM_START 0
# define SEEK_FROM_CURR  1
# define SEEK_FROM_END   2

/****************************************************************************/
                                                                                         
# define FClose(Code)                  {if(File[Code].Ptr!=NULL){                  \
                                        fflush(File[Code].Ptr);                    \
                                        fclose(File[Code].Ptr);                    \
                                        File[Code].Ptr=NULL;}                      \
                                        else                                       \
                                        Sys_Error(File[Code].Name,E_NULL_FILE_PTR) \
                                        Strset(File[Code].Name);}

/****************************************************************************/

# define FCloseall()                   {INT macro_var;                                            \
                                        for(macro_var=NONE; macro_var < WORK_FILES;macro_var++){ \
                                        if(File[macro_var].Ptr!=NULL){                           \
                                        FClose(macro_var);}}}

/****************************************************************************/

# define Clear_All_FName               {INT macro_var;                                           \
                                        for(macro_var=NONE; macro_var < WORK_FILES;macro_var++) \
                                        Strset(File[macro_var].Name);}

/****************************************************************************/
                                                                                                             
# define Get_Files_Info()              {INT macro_var;                                                   \
                                        for(macro_var=NONE; macro_var < WORK_FILES;macro_var++)         \
                                        if(File[macro_var].Ptr!=NULL){                                  \
                                        Print3(MESSAGE_COLOR,"File",File[macro_var].Name,"Is In Use");}}

/****************************************************************************/                               

# define Rewind(Code)                  rewind(File[Code].Ptr)

/****************************************************************************/                               

#ifdef __MSDOS__ 
# define FNLEN    12
# define MAXFNLEN STRING_LEN
#else
# define FNLEN    20
# define MAXFNLEN STRING_LEN
#endif

/****************************************************************************/

# define CLF        0      /* Caliban LogFile    */
# define SLF        1      /* Simulation LogFile */
# define DLF        2      /* PatternOrdered Fau */
# define FLF        3      /* GateOrdered Faults */

# define CFG        4      /* Caliban Config     */
# define NEW        5      /* New GeneralPurpose */
# define INP        6      /* Pattern            */

# define CAL        7      /* NetList            */
# define EDF        8      /* NetList EDF Format */
# define DEE        9      /* NetList DEE Format */
# define FAU        10     /* FaultList          */
# define GEN        11

# define FFI        12     /* FF Init File       */
# define BAT        13     /* Caliban CommandFile*/

# define MAN        14     /* Manual             */
# define ALG        15     /* About Algorithm    */

# define WORK_FILES 16

/****************************************************************************/

struct file_block
    {
     FILE* Ptr;
     CHAR* Ext;
     CHAR  Name[MAXFNLEN];
    };

/****************************************************************************/

typedef struct file_block FILE_BLOCK;

/****************************************************************************/

extern FILE_BLOCK File[];
extern CHAR*      Open_Mode[];

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
