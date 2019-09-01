#pragma once
#include <windows.h>
#include <shellapi.h>

#define WM_TRAY_MSG WM_USER + 0x01                  //自定义消息，用以接收来自托盘的反馈

class CSystemTray
{
public:
	CSystemTray(void);
public:
	~CSystemTray(void);
	void Destroy();
	BOOL Create(HWND hParent, HICON hIcon, LPCTSTR szToolTip);  //hParent 是接收WM_TRAY_MSG消息的窗口
	BOOL ModifyIcon(HICON hIcon);
	BOOL ModifyToolTipText(LPCTSTR szToolTip);
	BOOL ModifyBalloonText(LPCTSTR szTitle, LPCTSTR szInfo);
	BOOL ModifyBalloonIcon(HICON hIcon);
	BOOL SetFocus();
	//BOOL AnimateIcon(HICON *pIconArray, UINT nSize, DWORD dwTimeInter);
private:
	void Wait(int nSeconds);
	HWND m_hParent;
	HICON m_hIconTray, m_hIconBalloon;
	NOTIFYICONDATA m_nidSystemTray;
};