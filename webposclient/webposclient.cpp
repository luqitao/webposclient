// webposclient.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "webposclient.h"
#include "DuiHandlerMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwebposclientApp

BEGIN_MESSAGE_MAP(CwebposclientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CwebposclientApp 构造

CwebposclientApp::CwebposclientApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CwebposclientApp 对象

CwebposclientApp theApp;


// CwebposclientApp 初始化

BOOL CwebposclientApp::InitInstance()
{
	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("webposclient"));

	// 初始化DuiVision界面库,可以指定语言,dwLangID为0表示自动判断当前语言
	// 11160是应用程序ID,每个DUI应用程序应该使用不同的ID,ID主要用于进程间通信传递命令行时候区分应用
	DWORD dwLangID = 0;
	new DuiSystem(m_hInstance, dwLangID, _T("webposclient.ui"), 11160, IDD_DUIVISIONAPP_DIALOG, _T(""));

	// 检查是否已经有进程在运行
	CString strAppMutex = DuiSystem::Instance()->GetConfig(_T("appMutex"));	// 从配置文件中获取互斥量名字
	if(!strAppMutex.IsEmpty())
	{
		::CreateMutex(NULL,TRUE, _T("Global\\") + strAppMutex);
		if(ERROR_ALREADY_EXISTS == GetLastError() || ERROR_ACCESS_DENIED == GetLastError())
		{
			// 读取命令行参数,如果不需要处理命令行可以直接退出
			CString strCmd = L"";
			if(__argc > 0)
			{
				strCmd = __targv[0];
				DuiSystem::LogEvent(LOG_LEVEL_DEBUG, L"Command line:%s", strCmd);
			}

			// 发送进程间消息(lParam为1表示不显示界面,appMutex作为应用名,信息参数传递命令行参数)
			CString strAppName = DuiSystem::Instance()->GetConfig(_T("appMutex"));
			DuiSystem::Instance()->SendInterprocessMessage(0, DuiSystem::Instance()->GetAppID(), 1, strAppName, strCmd);

			return FALSE; // Here we quit this application
		}
	}


	// 创建主窗口
	CDlgBase* pMainDlg = DuiSystem::CreateDuiDialog(_T("dlg_main"), NULL, _T(""), TRUE);
	// 给主窗口注册事件处理对象
	CDuiHandlerMain* pHandler = new CDuiHandlerMain();
	pHandler->SetDialog(pMainDlg);
	DuiSystem::RegisterHandler(pMainDlg, pHandler);

	// 初始化提示信息窗口
	DuiSystem::Instance()->CreateNotifyMsgBox(_T("dlg_notifymsg"));

	// 按照非模式对话框创建主窗口,可以默认隐藏
	pMainDlg->Create(pMainDlg->GetIDTemplate(), NULL);
	//pMainDlg->ShowWindow(SW_HIDE);
	INT_PTR nResponse = pMainDlg->RunModalLoop();

	// 如果是按照模式对话框运行主窗口,只要改为如下代码就可以
	//INT_PTR nResponse = pMainDlg->DoModal();

	// 释放DuiVision界面库的资源
	DuiSystem::Release();

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
