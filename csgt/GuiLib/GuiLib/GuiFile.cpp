//-----------------------------------------------------------------------//
// This is a part of the GuiLib MFC Extention.							 //	
// Modified by  :  Francisco Campos										 //
// (C) 2002 Francisco Campos <www.beyondata.com> All rights reserved     //
// This code is provided "as is", with absolutely no warranty expressed  //
// or implied. Any use is at your own risk.								 //		
// You must obtain the author's consent before you can include this code //
// in a software library.												 //
// If the source code in  this file is used in any application			 //
// then acknowledgement must be made to the author of this program		 //	
// fcampos@tutopia.com													 //
//-----------------------------------------------------------------------//

#include "stdafx.h"
#include "GuiFile.h"
#include <malloc.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuiFile::CGuiFile()
{
}

CGuiFile::~CGuiFile()
{
//	Close();
}


BOOL  CGuiFile::Open(char *szFile,char* mode)
{
	fp=fopen(szFile,mode);
	return IsOpen();
}

void CGuiFile::Close()
{
	if (IsOpen())
	{
		fclose(fp);
		fp=NULL;
	}
}

BOOL  CGuiFile::Seek(long offset, TypeSeek tps )
{
	int result = fseek( fp, offset, tps);
    if( result )
        return FALSE;
    else
		return TRUE;
}
      
char* CGuiFile::Read(int Size)
{
	char* cad=(char*) malloc(sizeof(char)*Size);
	memset(cad,0x00,Size);
	int numread = fread( cad, sizeof( char ), Size,fp);
	return cad;
}

BOOL  CGuiFile::Eof()
{
	if (feof(fp))
		return TRUE;
	return FALSE;
}

char* CGuiFile::ReadLine()
{
	int c;
	int pos;
	int Entro;
	char *cadena;
	cadena=(char*) malloc(sizeof(char)* 1000);
	memset(cadena,0x00,1000);
	pos=0;
	Entro=0;
	while (!feof(fp))
	{
		c=fgetc(fp);
		if (c == 10 || c == 13 || c == -1)
		  break;
		else
		{
			if (pos > (int)_msize( cadena ))
			{
				size_t size=_msize( cadena );
				char* aux= (char*)malloc(sizeof(char)* size);
				strcpy(aux,cadena);
				if( (cadena =(char*)realloc( cadena, size + (100 * sizeof( char )) )) 
			        ==  NULL )
				{
					*(aux+pos)='\0';
					return aux;
				}
				memset(cadena+size,0x00,size+100);
			}

			*(cadena+(pos++))=(char)c;
			Entro=1;
		}
	}
	if (Entro==1)
		*(cadena+pos)='\n';
	else
		*(cadena+pos)='\0';
	return cadena;
}

BOOL  CGuiFile::IsOpen()
{
	if (fp == NULL)
		return FALSE;
	return TRUE;
}

BOOL  CGuiFile::WriteLine(char *cadena)
{
	int numwritten =fwrite(cadena,sizeof(char),strlen(cadena),fp);
	if (numwritten != (int)strlen(cadena))
		return FALSE;
	return TRUE;
}


