; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGameDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "game.h"

ClassCount=3
Class1=CGameApp
Class2=CGameDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_GAME_DIALOG

[CLS:CGameApp]
Type=0
HeaderFile=game.h
ImplementationFile=game.cpp
Filter=N

[CLS:CGameDlg]
Type=0
HeaderFile=gameDlg.h
ImplementationFile=gameDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CGameDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=gameDlg.h
ImplementationFile=gameDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_GAME_DIALOG]
Type=1
Class=CGameDlg
ControlCount=0

