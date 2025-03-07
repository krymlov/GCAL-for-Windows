// SimpleDataFile.h: interface for the TFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEDATAFILE_H__00F7817C_BBDC_488E_A2C9_EC9846776216__INCLUDED_)
#define AFX_SIMPLEDATAFILE_H__00F7817C_BBDC_488E_A2C9_EC9846776216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "platform.h"
#define TFLL 4
// PORTABLE

class TFile  
{
public:
	int m_cend;
	int WriteString(const char * str);
	int ReadString(TString &str);
	static char m_szLine[TFLL];
	FILE * m_fHandle;
	int Open(const char * szname, const char *mode);
	void Close();
	int ReadPropertyLine(TString &strA, TString &strB);
	BOOL WriteString2(int i, const char * psz);
	BOOL WriteString2(int i, int n);
	BOOL WriteString2(int i, double a);
	BOOL ReadString2(TString &strA, TString &strB);
	TFile();
	virtual ~TFile();

};

#endif // !defined(AFX_SIMPLEDATAFILE_H__00F7817C_BBDC_488E_A2C9_EC9846776216__INCLUDED_)
