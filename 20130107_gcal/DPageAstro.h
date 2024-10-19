#if !defined(AFX_DPAGEASTRO_H__3D1622B5_5E8B_4D6C_B921_5355D91AD2E8__INCLUDED_)
#define AFX_DPAGEASTRO_H__3D1622B5_5E8B_4D6C_B921_5355D91AD2E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPageAstro.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DPageAstro dialog

class DPageAstro : public CPropertyPage
{
	DECLARE_DYNCREATE(DPageAstro)

// Construction
public:
	DPageAstro();
	~DPageAstro();

// Dialog Data
	//{{AFX_DATA(DPageAstro)
	enum { IDD = IDD_PAGE_ASTRO };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DPageAstro)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DPageAstro)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DPAGEASTRO_H__3D1622B5_5E8B_4D6C_B921_5355D91AD2E8__INCLUDED_)
