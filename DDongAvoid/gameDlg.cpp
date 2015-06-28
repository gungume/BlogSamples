// gameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "game.h"
#include "gameDlg.h"

//#pragma comment(lib,"winmm.lib")


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
	ON_WM_CLOSE()
	ON_WM_DESTROY()
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
	
	MessageBox("시작 = Enter\n\n종료 = Esc\n\n일시정지 = Pause\n\n볼륨업 = PageUp\n\n볼륨다운 = PageDown", "짝퉁 똥피하기");
	srand((unsigned int) time(NULL));
	
	// 별표 좌표값 초기화
	initXY();
	initXY2();
	
	// 막대기 좌표값 초기화
	m_ptBar.x=240;
	m_ptBar.y=500;

	// 점수, 단계 초기화
	m_nCnt=0;
	m_nStage=1;

	m_nCnt2=9;		// 단계별 별표 y좌표 우선순위
	m_nCnt3=10;		// 단계별 별표 갯수
	m_nNum=100;		// 단계 계산값

	// 볼륨값 초기화 및 게임시작시 볼륨 최대화
	m_fVolume=0.5;
	SetMusicVolume(1);
	
	// wav 파일로 배경음악
	//PlaySound("tet.wav",NULL,SND_ASYNC| SND_LOOP);
	PlaySound((LPCSTR)MAKEINTRESOURCE(IDR_WAVE_BG),NULL,SND_RESOURCE | SND_ASYNC | SND_LOOP);
	
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
	dc.TextOut(400, 0, m_strScore);

	m_strStage.Format("난이도 : %d단계", m_nStage);
	dc.TextOut(400, 20, m_strStage);
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
	if(pMsg->wParam == VK_ESCAPE)
	{
		SetMusicVolume(1);
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
		case VK_PAUSE :
			KillTimer(0);
			KillTimer(1);
			if(	MessageBox("일시정지", "Pause") == IDOK )
			{
				SetTimer(0, 1, NULL);
				SetTimer(1, 1, NULL);				
			}
			break;

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
		case VK_PRIOR :
			if(m_fVolume < 0.9)
			{
				m_fVolume+=0.1;
				SetMusicVolume(m_fVolume);
			}
			break;
				
		case VK_NEXT :
			if(m_fVolume > 0.1)
			{
				m_fVolume-=0.1;
				SetMusicVolume(m_fVolume);
			}
			break;
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
			for(j=0; j<m_nCnt2; j++)
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

			for(i=0; i<m_nCnt3; i++)
			{
				dc.TextOut(m_pt[i].x, m_pt[i].y, "★");
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
				
				if( (m_nNum+100 == m_nCnt) && (m_nNum <= 1000) )
				{
					m_nCnt2++;
					m_nCnt3++;
					m_nStage++;
					m_nNum=m_nCnt;
				}
			}
			
			if( m_pt[max].y>=350)
			{
				SetTimer(1, 1, NULL);
			}

			
			for(k=0; k<m_nCnt3; k++)
			{
				if( (m_pt[k].x+15 >= m_ptBar.x) && (m_pt[k].x <= m_ptBar.x+30) && (m_pt[k].y >= m_ptBar.y) && (m_pt[k].y <= m_ptBar.y+5) )
				{
					initXY();
					initXY2();
					KillTimer(0);
					KillTimer(1);
					m_strCnt.Format("다시시작 : Enter\n게임종료 : Esc", m_nCnt);
					MessageBox(m_strCnt, "The End");
					m_nCnt=0;
					m_nCnt2=9;
					m_nCnt3=10;
					m_nStage=1;
					m_nNum=100;					
					Invalidate();
				}
			}
			
							
			break;
		
		/////////////////////////////////////////////////////////////////////////////////////
		case 1:
			// 최소값
			for(j=0; j<m_nCnt2; j++)
			{
				if( m_pt2[j].y <= m_pt2[j+1].y )
					min=j;
				else
					min=j+1;
			}

			for(i=0; i<m_nCnt3; i++)
			{
				dc.TextOut(m_pt2[i].x, m_pt2[i].y, "★");
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
				if( (m_nNum+100 == m_nCnt) && (m_nNum < 1200) )
				{
					m_nCnt2++;
					m_nCnt3++;
					m_nStage++;
					m_nNum=m_nCnt;
				}
			}
		
			for(k=0; k<m_nCnt3; k++)
			{
				if( (m_pt2[k].x+15 >= m_ptBar.x) && (m_pt2[k].x <= m_ptBar.x+30) && (m_pt2[k].y >= m_ptBar.y) && (m_pt2[k].y <= m_ptBar.y+5) )
				{
					initXY();
					initXY2();
					KillTimer(0);
					KillTimer(1);
					m_strCnt.Format("다시시작 : Enter\n게임종료 : Esc", m_nCnt);
					MessageBox(m_strCnt, "The End");
					m_nCnt=0;
					m_nCnt2=9;
					m_nCnt3=10;
					m_nStage=1;
					m_nNum=100;
					Invalidate();
				}
			}
			break;
		
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CGameDlg::SetMusicVolume(double fPercent)
{
	DWORD    dVolume=0xffffffff;
    WORD    left=HIWORD(dVolume),right=LOWORD(dVolume);
     
	right=UINT(right*fPercent);
    left =UINT(left *fPercent);
    dVolume=left;
    dVolume=dVolume<<16;
    dVolume=dVolume+ right;
	waveOutSetVolume(NULL,dVolume);
}

void CGameDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SetMusicVolume(1);
	CDialog::OnClose();
}

void CGameDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SetMusicVolume(1);
}
