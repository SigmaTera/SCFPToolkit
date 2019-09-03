// BQDSCFPToolkit.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "BQDSCFPToolkit.h"
#include "utils.hpp"
#include "download.hpp"
#include "SystemTray.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
BOOL                InitInstance(HINSTANCE, int);
INT_PTR CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				Process(HWND);


CSystemTray tray;
BOOL		bFound = FALSE;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BQDSCFPTOOLKIT, szWindowClass, MAX_LOADSTRING);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, SW_HIDE))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BQDSCFPTOOLKIT));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}




//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   
   HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG_TOOLKIT), NULL, WndProc);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	

	switch (message)
    {
	case WM_INITDIALOG:
		tray.Create(hWnd,LoadIcon(hInst,MAKEINTRESOURCE(IDI_BQDSCFPTOOLKIT)),TEXT("小链助理"));
		Process(hWnd);
		SetTimer(hWnd, 1, 10000, NULL);
		break;
	case WM_SHOWWINDOW:
		
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			DestroyWindow(hWnd);
			break;
		default:
			break;
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
    case WM_DESTROY:
		tray.Destroy();
        PostQuitMessage(0);
        break;
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

using namespace Leorius;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	CString strTitle;
	CString strClassName;
	TCHAR szTemp[128];


	GetWindowText(hwnd, szTemp, 128);
	strTitle = szTemp;
	GetClassName(hwnd, szTemp, 128);
	strClassName = szTemp;
	
	if (strTitle.Find(TEXT("线上供应链融资平台 - Internet Explorer")) > -1 && (strClassName == TEXT("IEFrame")))
	{
		ShowWindow(hwnd, SW_MAXIMIZE);
		bFound = TRUE;
		return FALSE;
	}
	return TRUE;
}

BOOL Process(HWND hWnd)
{
	LHttpDownloader downloader;
	DWORD dwConnectFlag = 0;
	CString strDownloadPath = TEXT("C:\\BQDSCFPKit");
	
	if (InternetGetConnectedState(&dwConnectFlag, 0) == FALSE)
	{
		tray.ModifyBalloonText(TEXT("提示"), TEXT("您的电脑貌似未连网,请检查网络连接."));
		return FALSE;
	}
	tray.ModifyBalloonText(TEXT("Hi"), TEXT("欢迎使用青岛银行线上供应链融资平台."));
	if (IsDllComponentRegistered(TEXT("{20A2D8D1-5A6E-4B46-B3C1-3415678028F0}")) == FALSE)
	{
		tray.ModifyBalloonText(TEXT("提示"), TEXT("首次登录前，正在为您安装密码控件."));
		CreateDirectory(strDownloadPath, NULL);
		if (downloader.initial(TEXT("rz.qdccb.com"), 443) == 0)
		{
			downloader.renameFile(true);
			if (downloader.get(TEXT("/financingPlatform/ocx"), TEXT("BQDeditIE.exe"), (LPCTSTR)strDownloadPath, TEXT("BQDeditIE.exe")) == 0)
			{
				strDownloadPath += TEXT("\\BQDeditIE.exe");
				ShellExecute(strDownloadPath, TEXT("/S"));
				tray.ModifyBalloonText(TEXT("提示"), TEXT("安装成功."));
			}
			else
			{
				tray.ModifyBalloonText(TEXT("提示"), TEXT("控件下载失败了:("));
			}
			downloader.uninitial();
		}
	}
	if (IsDllComponentRegistered(TEXT("{91C74C59-177B-4F8F-82F6-26DCEAB9D7C9}")) == FALSE)
	{
		Sleep(1000);
		tray.ModifyBalloonText(TEXT("提示"), TEXT("正在为您安装软证书控件."));
		CreateDirectory(strDownloadPath, NULL);
		if (downloader.initial(TEXT("rz.qdccb.com"), 443) == 0)
		{
			downloader.renameFile(true);
			if (downloader.get(TEXT("/financingPlatform/ocx"), TEXT("CryptoKit.CertEnrollment.qdccb.x86.exe"), (LPCTSTR)strDownloadPath, TEXT("CryptoKit.CertEnrollment.qdccb.x86.exe")) == 0)
			{
				strDownloadPath += TEXT("\\CryptoKit.CertEnrollment.qdccb.x86.exe");
				ShellExecute(strDownloadPath, TEXT("/S"));
				tray.ModifyBalloonText(TEXT("提示"), TEXT("安装成功."));
			}
			else
			{
				tray.ModifyBalloonText(TEXT("提示"), TEXT("控件下载失败了:("));
			}
			downloader.uninitial();
		}
	}
	int i = 0;
	while (1)
	{
		if (IsDllComponentRegistered(TEXT("{20A2D8D1-5A6E-4B46-B3C1-3415678028F0}")) && IsDllComponentRegistered(TEXT("{91C74C59-177B-4F8F-82F6-26DCEAB9D7C9}")))
		{
			break;
		}
		if (i++ > 5)
		{
			break;
		}
		Sleep(1000);
	}
	EnumWindows(EnumWindowsProc, 0);
	if (!bFound)
		ShellExecute(TEXT("iexplore.exe"), TEXT("https://rz.qdccb.com/financingPlatform/"), SW_MAXIMIZE, FALSE);
	return TRUE;
}