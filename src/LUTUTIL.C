/*--------------------------------------------------------------------------
**
**   System.....: ColorFlex Data System (CDS)
**
**   Application: Enhanced Imaging Editor (Equiped w/ JVC TK-F7300U).
**
**   Module.....: LUTUTIL.C - Utility to translate LUT data from binary
**                            tables to comma-delimited tables & back.
**
**   Author.....: Ronald D. Redmer
**
**   History....:
**                01/24/94 RDR Designed & Programmed.
**
**------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <fcntl.h>
#include <io.h>
#include <dos.h>
#include <sys\stat.h>

//--- Constants ------------------------------------------------------------
#define	true	1
#define	false	0
// #define DEBUG	1
//--- Structures -----------------------------------------------------------
typedef int Boolean;
typedef	unsigned int Word;
typedef	unsigned char Byte;

union LUT
{
	struct _rgba { Byte r,g,b,a; } RGBA;
	Byte    rgba[4];
};
typedef union LUT LUT;                  	// Define LUT data type
LUT    LutTb[256];		                // Standard LUT Table
char   LutFile[80];     	                // LUT file name
char   TxtFile[80];				// Text file name
char   CmdSwitch[10];				// Command line switch
//--- Prototypes -----------------------------------------------------------
int main(int argcnt, char **args);		// Process command line
int LoadLutFile(LUT *LutTb, char *file);	// Load BrookTree LUT table
int SaveLutFile(LUT *LutTb, char *file);	// Save BrookTree LUT table
int SaveTxtFile(LUT *LutTb, char *file);	// Save Text file
int LoadTxtFile(LUT *LutTb, char *file);        // Load Text file
void MakeLinearLut(LUT *LutTb);         	// Set LUT to linear curve

/*--------------------------------------------------------------------------
**
**  Function...: main
**
**  Description: Process BrookTree LUT files.
**
**------------------------------------------------------------------------*/
int main(int argcnt, char **args)
{
int i=0;
//--- Display Application Info ---------------------------------------------
printf("\n\nLUTUTIL - Lookup Table Translator Version 1.00");
printf("\n(c) 1994 Hicks Equipment, Inc.");
printf("\nAll Rights Reserved.\n");

//--- Process Command Line Arguments ---------------------------------------
if (argcnt < 3)
	{
	printf("%c", (char) 7);		// Beep
	printf("\nUsage: LUTUTIL lut_file text_file <switches>\n");
	printf("\n               Switches:");
	printf("\n                  /l  Translate text file to LUT");
	printf("\n                  /t  Translate LUT file to text\n\n");
	return(-1);
	}
#ifdef DEBUG
	for (i=0;i<argcnt;i++)
		printf("\n Args are: %s", *(args+i));
#endif
strcpy(LutFile, *(args+1));		// Name of LUT file
strcpy(TxtFile, *(args+2));		// Name of Text file
strcpy(CmdSwitch, *(args+3));		// Command line switch

//--- Translate LUT to text file -------------------------------------------
if (!strncmp(CmdSwitch, "/t", 2))		// Translate LUT to text
	{
	printf("\nTranslating [%s] to [%s].", LutFile, TxtFile);
	if (LoadLutFile(LutTb, LutFile) < 0)	// Load the LUT file
		{
		printf("%c", (char) 7);		// Beep
		printf("\nError loading LUT file [%s].\n\n", LutFile);
		return(-1);
		}
	if (SaveTxtFile(LutTb, TxtFile) < 0)	// Save the text file
		{
		printf("%c", (char) 7);		// Beep
		printf("\nError Saving text file [%s].\n\n", TxtFile);
		return(-1);
		}
	printf("\nCompleted with no errors.\n\n");
	}
//--- Translate text file to LUT -------------------------------------------
if (!strncmp(CmdSwitch, "/l", 2))		// Translate text to LUT
	{
	printf("\nTranslating [%s] to [%s].", TxtFile, LutFile);
	if (LoadTxtFile(LutTb, TxtFile) < 0)	// Load the text file
		{
		printf("%c", (char) 7);		// Beep
		printf("\nError loading text file [%s].\n\n", TxtFile);
		return(-1);
		}
	if (SaveLutFile(LutTb, LutFile) < 0)	// Save the text file
		{
		printf("%c", (char) 7);		// Beep
		printf("\nError Saving LUT file [%s].\n\n", LutFile);
		return(-1);
		}
	printf("\nCompleted with no errors.\n\n");
	}
return(0);
}
/*--------------------------------------------------------------------------
**
**  Function...: LoadLutFile
**
**  Description: Load a LUT structure with information stored in standard
**               BrookTree LUT file.
**
**------------------------------------------------------------------------*/
int LoadLutFile(LUT *LutTb, char *file)
{
int fh;
if ((fh = open(file, O_RDONLY| O_BINARY)) == -1)
	{
	return -1;              /* open error */
	}
if (read(fh,(char *)LutTb,sizeof(LUT)*256) != sizeof(LUT)*256)
	{
	return -2;              /* read error */
	}
close(fh);
return 0;
}
/*--------------------------------------------------------------------------
**
**  Function...: SaveLutFile
**
**  Description: Save a LUT structure with information stored in standard
**               BrookTree LUT table.
**
**------------------------------------------------------------------------*/
int SaveLutFile(LUT *LutTb, char *file)
{
int fh;
if ((fh = open(file, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE)) == -1)
	{
	return -1;              /* open error */
	}
if ((write(fh,(char *)LutTb,sizeof(LUT)*256)) != sizeof(LUT)*256)
	{
	return -2;              /* write error */
	}
close(fh);
return(0);
}
/*--------------------------------------------------------------------------
**
**  Function...: LoadTxtFile
**
**  Description: Load a LUT structure with information stored in standard
**               comma-delimited text file.
**
**------------------------------------------------------------------------*/
int LoadTxtFile(LUT *LutTb, char *file)
{
FILE *fh;
int i=0;
int r=0, g=0, b=0, a=0;
if ((fh = fopen(file,"rt")) == NULL)
	{
	return -1;              /* open error */
	}
for (i=0; i<256; i++)
	{
	fscanf(fh, "%d,%d,%d,%d", &r, &g, &b, &a);
	LutTb[i].RGBA.r = (char) r;
	LutTb[i].RGBA.g = (char) g;
	LutTb[i].RGBA.b = (char) b;
	LutTb[i].RGBA.a = (char) a;
	#ifdef DEBUG
		printf("\n%d,%d,%d,%d", (int) LutTb[i].RGBA.r, (int) LutTb[i].RGBA.g, (int) LutTb[i].RGBA.b, (int) LutTb[i].RGBA.a);
	#endif
	}
fclose(fh);
return 0;
}
/*--------------------------------------------------------------------------
**
**  Function...: SaveTxtFile
**
**  Description: Save text file with LUT information.
**
**------------------------------------------------------------------------*/
int SaveTxtFile(LUT *LutTb, char *file)
{
int fh;
int i=0;
int numbytes=0;
char outbuf[50];
//--- Open Text file for output --------------------------------------------
if ((fh = open(file, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE)) == -1)
	{
	return(-1);
	}
//--- Output each LUT value ------------------------------------------------
for (i=0; i<256; i++)
	{
	#ifdef DEBUG
		printf("\n%d,%d,%d,%d", (int) LutTb[i].RGBA.r, (int) LutTb[i].RGBA.g, (int) LutTb[i].RGBA.b, (int) LutTb[i].RGBA.a);
	#endif
	sprintf(outbuf, "%d,%d,%d,%d\n", (int) LutTb[i].RGBA.r, (int) LutTb[i].RGBA.g, (int) LutTb[i].RGBA.b, (int) LutTb[i].RGBA.a);
	if ((numbytes = write(fh, outbuf, strlen(outbuf))) != strlen(outbuf))
		{
		return(-2);
		}
	}
close(fh);
return(0);
}
/*--------------------------------------------------------------------------
**
**  Function...: MakeLinearLut
**
**  Description: Set LUT table to linear curve.
**
**------------------------------------------------------------------------*/
void MakeLinearLut(LUT *LutTb)
{
int i;
for (i=0; i<256; i++) 
	{
	LutTb->RGBA.r = LutTb->RGBA.g = LutTb->RGBA.b = i;
	LutTb->RGBA.a = 0;
	LutTb++;
	}
}
/* END OF FILE */
