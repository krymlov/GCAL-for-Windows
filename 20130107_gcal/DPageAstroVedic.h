#if !defined(AFX_DPageToday_H__1CB0C774_9CAD_4E8F_8147_D9D9C270708C__INCLUDED_)
#define AFX_DPageToday_H__1CB0C774_9CAD_4E8F_8147_D9D9C270708C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPageToday.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DPageToday dialog

class DPageToday : public CPropertyPage
{
	DECLARE_DYNCREATE(DPageToday)

// Construction
public:
	DPageToday();
	~DPageToday();

// Dialog Data
	//{{AFX_DATA(DPageToday)
	enum { IDD = IDD_PAGE_ASTROV };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPageToday)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPageToday)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DPageToday_H__1CB0C774_9CAD_4E8F_8147_D9D9C270708C__INCLUDED_)
