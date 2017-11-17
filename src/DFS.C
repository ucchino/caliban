/*PPU************************************************************************\
*
*    $Source: dfs.c
*  $Revision: 0.0.7
*      $Date: Wed Nov 25 23:48:39 1998
*
\************************************************************************PPU*/

# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# define LEN 255

void main(int argc,char* argv[])
{
 char Gtype[LEN];

 char filenam[LEN];

 unsigned int i;

 long int fs_fau;
 long int flf_fau;
 long int fs_pattern;
 long int flf_pattern;

 int failed;
 int fs_detection;
 int flf_detection;

 char fs_detect[LEN];
 char flf_detect[LEN];

 FILE* flf;
 FILE* fs;

 if(argc < 2)
 {
  printf("\nManca il nome del file .FLF da confrontare\n");
  exit(0);
 }

 failed=0;

 strcpy(filenam,argv[1]);
 strcat(filenam,".flf");

 fs=fopen("molokh.fs","r");
 if(fs==NULL)
 {
  printf("\nNon posso aprire il file fs\n");
  exit(0);
 }

 flf=fopen(filenam,"r");
 if(flf==NULL)
 {
  fclose(fs);
  printf("\nNon posso aprire il file flf\n");
  exit(0);
 }

 /* Read FLF Header */
 fgets(filenam,LEN,flf);
 fgets(filenam,LEN,flf);
 fgets(filenam,LEN,flf);
 fgets(filenam,LEN,flf);
 fgets(filenam,LEN,flf);

 while((feof(fs)==0) && (feof(flf)==0))
 {
  fs_detection=0;
  flf_detection=0;

  fs_pattern=0;
  flf_pattern=0;

  fs_fau=0;
  flf_fau=0;

  for(i=0;i < LEN;i++)
  {
   fs_detect[i]=' ';
   flf_detect[i]=' ';
  }

  fscanf(fs,"Fault n. %ld %s",&fs_fau,fs_detect);
  if(!strcmp(fs_detect,"detected"))
  {
   fs_detection=1;
   fscanf(fs," at pattern n. %ld\n",&fs_pattern);
  }
  else
   fscanf(fs,"\n");


  strcpy(Gtype,"                ");
  fscanf(flf,"%s",flf_detect);

  if(!strcmp(flf_detect,"Detected"))
  {
   flf_detection=1;
   fscanf(flf,"%*s %*s %ld %*s %ld",&flf_fau,&flf_pattern);
   fscanf(flf,"%*s %s\n",Gtype);
  }
  else
   fscanf(flf,"%*s %ld %*s %s",&flf_fau,Gtype);


  if(!strcmp(flf_detect,"Detected")) flf_detection=1;

  if((fs_pattern!=flf_pattern) || (fs_fau!=flf_fau) || (fs_detection!=flf_detection))
  {
   failed=1;
   printf("\nConfronto fallito\n");
   printf("\nFault    FS:%10ld  FLF:%10ld",fs_fau,flf_fau);
   printf("\nGateType   :%s",Gtype);
   printf("\nPattern  FS:%10ld  FLF:%10ld ---> %ld",fs_pattern,flf_pattern,(fs_pattern-flf_pattern));
   printf("\nDetect   FS:%10d  FLF:%10d",fs_detection,flf_detection);
   printf("\n--------------------------------------------------------------------");
  }
 }

 fscanf(flf,"%s%*s",filenam);

 if(feof(flf)==0 || feof(fs)==0)
 {
  printf("\nUno dei due file e' piu' piccolo\n");
  exit(0);
 }
 
 fclose(fs);
 fclose(flf);
 
 if(!failed)
 {
  printf("\nConfronto riuscito file di log uguali\n");
 }
}
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
