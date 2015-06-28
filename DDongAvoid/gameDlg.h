// gameDlg.h : header file
//

#if !defined(AFX_GAMEDLG_H__415FF11C_0BD7_4DBD_B9D3_869F65A14A6C__INCLUDED_)
#define AFX_GAMEDLG_H__415FF11C_0BD7_4DBD_B9D3_869F65A14A6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGameDlg dialog

class CGameDlg : public CDialog
{
// Construction
public:
	CString m_strScore;
	CString m_strCnt;
	void KeyDown(MSG pMsg);
	void initXY();
	void initXY2();
		
	CPoint m_ptBar; // 막대기 좌표값
	
	CPoint m_pt[20]; // 첫번째 별표 좌표값
	CPoint m_pt2[20]; // 두번째 별표 좌표값
	
	int m_nSpd[20]; // 첫번재 별표 속도
	int m_nSpd2[20]; // 두번재 별표 속도

	int m_nCnt; // 점수
	CGameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGameDlg)
	enum { IDD = IDD_GAME_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEDLG_H__415FF11C_0BD7_4DBD_B9D3_869F65A14A6C__INCLUDED_)
