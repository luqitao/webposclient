#pragma once
#include <vector>
using namespace std;

// ��ʱ����Ŀ�ṹ
struct TIMER_ITEM
{
	UINT	uTimerID;
	CString strTimerName;
	BOOL	bAppTimer;		// �Ƿ�Ӧ�õĶ�ʱ��
};

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);

	//����ʾ�����
	//vector<UINT>			m_vecTimer;				// ��ʱ��ID
	vector<TIMER_ITEM>		m_vecTimer;				// ��ʱ���б�
	static UINT				m_uAccuracy;			// ����ֱ��� 
	static BOOL				m_bIsTmierOK;			// �Ƿ���Զ�ʱ 

	BOOL GetTimerIsRun(UINT uTimerID);
	BOOL GetTimerIsRun(CString strTimerName);
	CString GetTimerName(UINT uTimerID);
	BOOL IsAppTimer(UINT uTimerID);
	int SetTimer(UINT nTimerResolution, CString strTimerName = _T(""), BOOL bAppTimer = FALSE);
	int KillTimer(UINT uTimerID);
	int KillTimer(CString strTimerName);
	void KillTimer();
	static void CALLBACK TimerCallProc(UINT uTimerID, UINT msg, DWORD dwData, DWORD dwa, DWORD dwb);
	virtual void OnTimer(UINT uTimerID) = 0;
	virtual void OnTimer(UINT uTimerID, CString strTimerName) {};
};
