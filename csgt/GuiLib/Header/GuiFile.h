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


#if !defined(AFX_GUIFILE_H__314F6DAD_DA53_4E03_B656_E5DF4D4155E9__INCLUDED_)
#define AFX_GUIFILE_H__314F6DAD_DA53_4E03_B656_E5DF4D4155E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Guilib.h"
class GUILIBDLLEXPORT  CGuiFile  
{
public:
	CGuiFile();
	virtual ~CGuiFile();
	

	enum TypeSeek  {   FSEEK_CUR=1,
					   FSEEK_END=2,
					   FSEEK_SET=0};
public:
	char* ReadLine();
	BOOL  WriteLine(char *cadena);
	BOOL  IsOpen();
	BOOL  Open(char *szFile,char* mode);
	BOOL  Seek(long offset, TypeSeek tps );
	char* Read(int Size);
	BOOL  Eof();
	void Close();
protected: 
	FILE* fp;
};

#endif // !defined(AFX_GUIFILE_H__314F6DAD_DA53_4E03_B656_E5DF4D4155E9__INCLUDED_)

