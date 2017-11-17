/*PPU************************************************************************\
*
*    $Source: memory.h
*  $Revision: 0.7.2
*      $Date: Thu Jul  8 19:31:00 1999 
*
\************************************************************************PPU*/

# define DB_ROOT 0

/****************************************************************************/

#ifdef DEBUG
# define Free(Pointer,Nk) {CHAR Out[STRING_LEN];                  \
                       ifNPtr(Pointer) Sys_Error(#Pointer,E_FREE) \
                       else{                                      \
                       free(Pointer);Pointer=NULL;                \
                       if((Nk) >  0)       sprintf(Out,"Freed [%s] As [%ld]",#Pointer,(ULONG)Nk); \
                       if((Nk) == DB_ROOT) sprintf(Out,"Freed [%s] As Root Structure",#Pointer);  \
                       Print(DBG_COLOR,Out);}}
#else                       
# define Free(Pointer,Nk) {ifPtr(Pointer){free(Pointer);Pointer=NULL;}}
#endif              

/****************************************************************************/

# define Alloc(Pointer,Num,Type,Used) {ifPtr(Pointer){                             \
                                 Sys_Error(#Pointer,E_NOT_NULL_PTR);Break(ERROR);} \
                                 (Pointer=(Type*)malloc(((Num)*sizeof(Type))));    \
                                 Used+=Allocate_Memory(Pointer,sizeof(Type),(Num),#Pointer,#Type);}

/****************************************************************************/

# define ifPtr(Pointer)    if(Pointer != NULL)
# define ifNPtr(Pointer)   if(Pointer == NULL)
# define Ptr(Pointer)       ((Pointer == NULL) ? (TRUE) : (FALSE))
# define CkPtr(Pointer,Do) if(Pointer == NULL) {Sys_Error(#Pointer,E_CK_NULL_PTR);Do;}

/****************************************************************************/

# define Print_Mem_Used(ptr,mem) {CHAR Str1[STRING_LEN]; \
                            SPrint_Mem_Used(Str1,mem);   \
                            Print4(MESSAGE_COLOR, "Grabbed Mem:",Str1,"For",#ptr);}

/****************************************************************************/

void  Release_All_Memory(void);

void  SPrint_Mem_Used(CHAR* String,ULONG mem);

ULONG Allocate_Memory(void* Pointer,ULONG NumBytes,ULONG Num_Blocks,CHAR* Pointer_Name,CHAR* Pointer_Type);

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
