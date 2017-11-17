/*PPU************************************************************************\
*
*    $Source: options.h
*  $Revision: 0.3.3
*      $Date: Thu Jul  8 19:31:02 1999 
*
\************************************************************************PPU*/
/****************************************************************************/

INT  Set_Option(CHAR* Opt);

/****************************************************************************/

void Menu(INT arg_number,CHAR *arg_vector[]);

/****************************************************************************/

void Command_Line_Long_Help(void);
void Command_Line_Short_Help(void);

/****************************************************************************/
/* a b c d e f g h i j k l m n o p q r s t u v x y w z $                    */
/* + - + + - + + + + - - + + - + + + - + - + + + + + - +                    */
/*                                                                          */
/* A B C D E F G H I J K L M N O P Q R S T U V X Y W Z !                    */
/* + + + + + + + - + - - - + - - - - + + - - - + - + - +                    */
/*                                                                          */
/****************************************************************************/

# define About_Option               1
# define About_Args                 0
# define About                      "a"
# define Short_About                "-a"
# define Long_About                 "--about"

# define Load_Circuit_Option        2
# define Load_Circuit_Args          1
# define Load_Circuit               "c"
# define Short_Load_Circuit         "-c"
# define Long_Load_Circuit          "--load-circuit"

# define PatternOrdered_Option      3
# define PatternOrdered_Args        0
# define PatternOrdered             "o"
# define Short_PatternOrdered       "-o"
# define Long_PatternOrdered        "--pattern-ordered"

# define Load_Faults_Option         4
# define Load_Faults_Args           1
# define Load_Faults                "f"
# define Short_Load_Faults          "-f"
# define Long_Load_Faults           "--load-faults"

# define Create_New_Circuit_Option  5
# define Create_New_Circuit_Args    1
# define Create_New_Circuit         "C"
# define Short_Create_New_Circuit   "-C"
# define Long_Create_New_Circuit    "--create-circuit"

# define Short_Help_Option          6
# define Short_Help_Args            0
# define Short_Help                 "h"
# define Short_Short_Help           "-h"

# define Long_Help_Option           7
# define Long_Help_Args             0
# define Long_Help                  "h"
# define Long_Long_Help             "--help"

# define Simulation_Info_Option     8
# define Simulation_Info_Args       0
# define Simulation_Info            "I"
# define Short_Simulation_Info      "-I"
# define Long_Simulation_Info       "--sim-info"

# define Algorithm_Option           9
# define Algorithm_Args             0
# define Algorithm                  "A"
# define Short_Algorithm            "-A"
# define Long_Algorithm             "--algorithm"

# define Convert_Dee_Option         10
# define Convert_Dee_Args           1
# define Convert_Dee                "D"
# define Short_Convert_Dee          "-D"
# define Long_Convert_Dee           "--convert-dee"

# define Limits_Option              11
# define Limits_Args                0
# define Limits                     "l"
# define Short_Limits               "-l"
# define Long_Limits                "--limits"

# define Memory_Status_Option       12
# define Memory_Status_Args         0
# define Memory_Status              "m"
# define Short_Memory_Status        "-m"
# define Long_Memory_Status         "--memory-status"

# define Manual_Option              13
# define Manual_Args                0
# define Manual                     "M"
# define Short_Manual               "-M"
# define Long_Manual                "--manual"

# define Dir_List_Option            14
# define Dir_List_Args              0
# define Dir_List                   "d"
# define Short_Dir_List             "-d"
# define Long_Dir_List              "-dir"

# define Load_Pattern_Option        15
# define Load_Pattern_Args          1
# define Load_Pattern               "p"
# define Short_Load_Pattern         "-p"
# define Long_Load_Pattern          "--load-pattern"

# define Quit_Option                16
# define Quit_Args                  0
# define Quit                       "q"
# define Short_Quit                 "-q"
# define Long_Quit                  "--quit"

# define Run_Option                 17
# define Run_Args                   0
# define Run                        "X"
# define Short_Run                  "-X"
# define Long_Run                   "--exec"

# define Simulate_Option            18
# define Simulate_Args              0
# define Simulate                   "s"
# define Short_Simulate             "-s"
# define Long_Simulate              "--simulate"

# define Command_File_Option        19
# define Command_File_Args          1
# define Command_File               "B"
# define Short_Command_File         "-B"
# define Long_Command_File          "--command-file"

# define Faumanager_Option          20
# define Faumanager_Args            4
# define Faumanager                 "u"
# define Short_Faumanager           "-u"
# define Long_Faumanager            "--faumanager"

# define Verbose_Option             21
# define Verbose_Args               0
# define Verbose                    "v"
# define Short_Verbose              "-v"
# define Long_Verbose               "--verbose"

# define Author_Option              22
# define Author_Args                0
# define Author                     "w"
# define Short_Author               "-w"
# define Long_Author                "--author"

# define Atpg1_Option               23
# define Atpg1_Args                 4
# define Atpg1                      "g"
# define Short_Atpg1                "-g"
# define Long_Atpg1                 "--atpg1"

# define Atpg2_Option               24
# define Atpg2_Args                 5
# define Atpg2                      "G"
# define Short_Atpg2                "-G"
# define Long_Atpg2                 "--atpg2"

# define Load_All_Option            25
# define Load_All_Args              1
# define Load_All                   "y"
# define Short_Load_All             "-y"
# define Long_Load_All              "--load"

# define Copyright_Notice_Option    26
# define Copyright_Notice_Args      0
# define Copyright_Notice           "R"
# define Short_Copyright_Notice     "-R"
# define Long_Copyright_Notice      "--copyright"

# define Circuit_Levels_Option      27
# define Circuit_Levels_Args        0
# define Circuit_Levels             "N"
# define Short_Circuit_Levels       "-N"
# define Long_Circuit_Levels        "--gates-levels"

# define Convert_Edf_Option         28
# define Convert_Edf_Args           1
# define Convert_Edf                "E"
# define Short_Convert_Edf          "-E"
# define Long_Convert_Edf           "--convert-edf"

# define Silent_Option              29
# define Silent_Args                0
# define Silent                     "S"
# define Short_Silent               "-S"
# define Long_Silent                "--silent"

# define Ansi_Option                30
# define Ansi_Args                  0
# define Ansi                       "$"
# define Short_Ansi                 "-$"
# define Long_Ansi                  "--ansi"

# define File_Init_FF_Option        31
# define File_Init_FF_Args          1
# define File_Init_FF               "F"
# define Short_File_Init_FF         "-F"
# define Long_File_Init_FF          "--ff-file"

# define Init_FF_Option             32
# define Init_FF_Args               1
# define Init_FF                    "i"
# define Short_Init_FF              "-i"
# define Long_Init_FF               "--ff-init"

# define Settings_Option            33
# define Settings_Args              0
# define Settings                   "k"
# define Short_Settings             "-k"
# define Long_Settings              "--settings"

# define Reset_Count_Option         34
# define Reset_Count_Args           0
# define Reset_Count                "!"
# define Short_Reset_Count          "-!"
# define Long_Reset_Count           "--reset-count"

/****************************************************************************/

extern CHAR *Menu_Help_[];

# define Menu_Help() {INT macro_var;Clear_Screen();for(macro_var=NONE;*Menu_Help_[macro_var];macro_var++) Print(FCyan,Menu_Help_[macro_var]);}

/****************************************************************************/

void    Results(void);
void    Get_Simulation_Info(void);

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
