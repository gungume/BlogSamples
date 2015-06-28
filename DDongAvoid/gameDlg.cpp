// gameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "game.h"
#include "gameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameDlg dialog

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialog)
	//{{AFX_MSG_MAP(CGameDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameDlg message handlers

BOOL CGameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	srand((unsigned int) time(NULL));
	
	// 별표 좌표값 초기화
	initXY();
	initXY2();
	
	// 막대기 좌표값 초기화
	m_ptBar.x=0;
	m_ptBar.y=500;

	// 점수 초기화
	m_nCnt=0;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	// 막대기 출력
	CClientDC dc(this);
	dc.TextOut(m_ptBar.x, m_ptBar.y, "▩▩");
	m_strScore.Format("점수 : %d점", m_nCnt);
	dc.TextOut(410, 0, m_strScore);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGameDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}




BOOL CGameDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		KeyDown(*pMsg);
	}
	if(pMsg->wParam == VK_RETURN)
	{
		SetTimer(0, 1, NULL);
		return 0;
	}
	

	return CDialog::PreTranslateMessage(pMsg);
}

void CGameDlg::initXY()
{
	for(int i=0; i<20; i++)
	{
		m_pt[i].x=rand()%490;
		m_pt[i].y=rand()%150;
		m_nSpd[i]=rand()%2+2;
	}

}

void CGameDlg::initXY2()
{
	for(int i=0; i<20; i++)
	{
		m_pt2[i].x=rand()%490;
		m_pt2[i].y=rand()%150;
		m_nSpd2[i]=rand()%2+2;
	}

}

void CGameDlg::KeyDown(MSG pMsg)
{
	switch(pMsg.wParam)
	{

		case VK_LEFT : 
			if(m_ptBar.x<=0)
				break;
			else
			{
				m_ptBar.x -= 10;
				break;
			}
	
		case VK_RIGHT : 
			if(m_ptBar.x>=470)
				break;
			else
			{
				m_ptBar.x += 10;
				break;
			}
	}
	Invalidate();
}

void CGameDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	// x=490, y=505
	CClientDC dc(this);	
	int i,j,k,min,max;
	i=j=k=min=max=0;

	
	switch(nIDEvent)
	{
		case 0:
			// 최소값
			for(j=0; j<19; j++)
			{
				if( m_pt[j].y <= m_pt[j+1].y )
					min=j;
				else
					min=j+1;

				if( m_pt[j].y >= m_pt[j+1].y )
					max=j;
				else
					max=j+1;
			}

			for(i=0; i<20; i++)
			{
				dc.TextOut(m_pt[i].x, m_pt[i].y, "*");
				if(i==min)
				{
					m_pt[min].y+=2;
					continue;
				}
				m_pt[i].y+=m_nSpd[i];
			}
	
			if(m_pt[min].y>=550)
			{
				initXY();
				m_nCnt+=20;
			}
			
			if( m_pt[max].y>=350)
			{
				SetTimer(1, 1, NULL);
			}

			
			for(k=0; k<20; k++)
			{
				if( (m_pt[k].x >= m_ptBar.x) && (m_pt[k].x <= m_ptBar.x+30) && (m_pt[k].y >= m_ptBar.y) && (m_pt[k].y <= m_ptBar.y+5) )
				{
					initXY();
					initXY2();
					KillTimer(0);
					KillTimer(1);
					m_strCnt.Format("다시시작 : Enter\n게임종료 : Esc", m_nCnt);
					MessageBox(m_strCnt, "The End");
					m_nCnt=0;
					Invalidate();
				}
			}
			
			break;
			
		/////////////////////////////////////////////////////////////////////////////////////
		case 1:
			// 최소값
			for(j=0; j<19; j++)
			{
				if( m_pt2[j].y <= m_pt2[j+1].y )
					min=j;
				else
					min=j+1;
			}

			for(i=0; i<20; i++)
			{
				dc.TextOut(m_pt2[i].x, m_pt2[i].y, "*");
				if(i==min)
				{
					m_pt2[min].y+=2;
					continue;
				}
				m_pt2[i].y+=m_nSpd[i];
			}
	
		

			if(m_pt2[min].y>=550)
			{
				initXY2();
				m_nCnt+=20;
			}
			
			for(k=0; k<20; k++)
			{
				if( (m_pt2[k].x >= m_ptBar.x) && (m_pt2[k].x <= m_ptBar.x+30) && (m_pt2[k].y >= m_ptBar.y) && (m_pt2[k].y <= m_ptBar.y+5) )
				{
					initXY();
					initXY2();
					KillTimer(0);
					KillTimer(1);
					m_strCnt.Format("다시시작 : Enter\n게임종료 : Esc", m_nCnt);
					MessageBox(m_strCnt, "The End");
					m_nCnt=0;
					Invalidate();
				}
			}
			break;		
	}
	
	CDialog::OnTimer(nIDEvent);
}


