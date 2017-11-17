/*PPU************************************************************************\
*
*    $Source: times.h
*  $Revision: 0.6.10
*      $Date: Thu Jul  8 19:31:08 1999 
*
\************************************************************************PPU*/

extern time_t  Program_Start_Date;

extern clock_t Simulation_Stop_Time;
extern clock_t Simulation_Start_Time;
extern clock_t Simulation_Elapsed_Time;

/****************************************************************************/

# define DATE_TIME_STRING_LENGHT   27

/****************************************************************************/

#if defined CLOCKS_PER_SEC
# define CLOCK_TICKS CLOCKS_PER_SEC
# define CK          1
#elif defined CLOCK_TICKS
# define CLOCK_TICKS CLK_TCK
# define CK          2
#elif defined HZ
# define CLOCK_TICKS HZ
# define CK          3
#else
# define CLOCK_TICKS 60
# define CK          4
#endif

/****************************************************************************/

# define Sec(TotalTime)            ((TotalTime)/(DOUBLE)CLOCK_TICKS)

#ifndef __MSDOS__
# define Get_Clock(Clock)          {struct tms t;time(&t);Clock=(t.tms_utime+t.tms_stime);}
#else
# define Get_Clock(Clock)          {Clock=clock();}
#endif

/****************************************************************************/

# define Start_Date(Date)          {Date=time(NULL);}
# define Set_Date_NL(Curr)         {Curr[DATE_TIME_STRING_LENGHT-3]=' ';Curr[DATE_TIME_STRING_LENGHT-3]=' ';}
# define Get_Date(String,Date)     {sprintf(String,"%s",ctime(&Date));Set_Date_NL(String);}

/****************************************************************************/

# define SPrint_Time(String,Time)  {DOUBLE Secs=Sec(Time);INT Aux=Sec(Time);Strset(String);sprintf(String,"[%02d:%02d:%02d][%03.0f Clock Ticks]",(Aux/3600),((Aux/60)%60),(Aux%60),(1000*(modf(Secs,&Secs))));}
                                                                                                                                                                               
/****************************************************************************/

void Print_Time(void);                                                                                            
                                                                                                                                                                                           
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
