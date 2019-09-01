#include "SystemTray.h"
#include <tchar.h>

CSystemTray::CSystemTray(void) :m_hParent(NULL), m_hIconTray(NULL), m_hIconBalloon(NULL)
{
	::ZeroMemory(&m_nidSystemTray, sizeof(NOTIFYICONDATA));
}

CSystemTray::~CSystemTray(void)
{
	Destroy();
}

BOOL CSystemTray::Create(HWND hParent, HICON hIcon, LPCTSTR szToolTip)
{
	m_hParent = hParent;



	m_nidSystemTray.cbSize = sizeof(NOTIFYICONDATA);
	m_nidSystemTray.hWnd = m_hParent;
	m_nidSystemTray.hIcon = m_hIconTray = hIcon;

	m_nidSystemTray.uFlags = NIF_ICON | NIF_MESSAGE;
	m_nidSystemTray.uCallbackMessage = WM_TRAY_MSG;
	if (szToolTip)
	{
		m_nidSystemTray.uFlags |= NIF_TIP;
		_tcscpy_s(m_nidSystemTray.szTip, szToolTip);
	}
	return Shell_NotifyIcon(NIM_ADD, &m_nidSystemTray);
}

BOOL CSystemTray::ModifyIcon(HICON hIcon)
{
	
	m_nidSystemTray.hIcon = hIcon;
	m_nidSystemTray.uFlags = NIF_ICON;
	return Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray);
}

BOOL CSystemTray::ModifyToolTipText(LPCTSTR szToolTip)
{
	
	_tcscpy_s(m_nidSystemTray.szTip, szToolTip);
	m_nidSystemTray.uFlags = NIF_TIP;
	return Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray);
}

BOOL CSystemTray::ModifyBalloonText(LPCTSTR szTitle, LPCTSTR szInfo)
{
	
	_tcscpy_s(m_nidSystemTray.szInfoTitle, szTitle);
	_tcscpy_s(m_nidSystemTray.szInfo, szInfo);
	m_nidSystemTray.hIcon = m_hIconBalloon;
	m_nidSystemTray.uFlags = NIF_INFO | NIF_ICON;
	BOOL bRet = Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray);

	if (bRet)
	{
		m_nidSystemTray.hIcon = m_hIconTray;
		m_nidSystemTray.uFlags = NIF_ICON;
		return Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray);
	}
	else return FALSE;
}

BOOL CSystemTray::ModifyBalloonIcon(HICON hIcon)
{
	m_nidSystemTray.dwInfoFlags = NIIF_USER;

	m_nidSystemTray.hIcon = m_hIconBalloon = hIcon;

	m_nidSystemTray.uFlags = NIF_INFO | NIF_ICON;
	BOOL bRet = FALSE;
	bRet = Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray);
	if (bRet)
	{
		m_nidSystemTray.hIcon = m_hIconTray;

		m_nidSystemTray.uFlags = NIF_ICON;

		return Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray);
	}
	else return FALSE;
}
BOOL CSystemTray::SetFocus()
{
	return Shell_NotifyIcon(NIM_SETFOCUS, &m_nidSystemTray);
}
void CSystemTray::Destroy()
{
	if (m_nidSystemTray.hWnd != NULL)
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nidSystemTray);
		::ZeroMemory(&m_nidSystemTray, sizeof(NOTIFYICONDATA));
	}
	m_hParent = NULL;
}