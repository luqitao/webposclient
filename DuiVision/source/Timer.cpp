#include "StdAfx.h"
#include <mmsystem.h> 
#include "Timer.h"

#pragma comment(lib,"Winmm.lib")  

UINT				CTimer::m_uAccuracy = 1;				// ����ֱ��� 
BOOL				CTimer::m_bIsTmierOK = false;			// �Ƿ���Զ�ʱ 

CTimer::CTimer(void)
{
	m_bIsTmierOK = false;
}

CTimer::~CTimer(void)
{
	KillTimer();

	if (m_bIsTmierOK)
	{
		timeEndPeriod(m_uAccuracy);
	}
}

void CALLBACK CTimer::TimerCallProc(UINT uTimerID, UINT msg, DWORD dwData, DWORD dwa, DWORD dwb)
{
	CTimer *pTimer = (CTimer *)dwData;
	if (pTimer)
	{
		pTimer->OnTimer(uTimerID);
		if(pTimer->IsAppTimer(uTimerID))
		{
			// �����Ӧ�õĶ�ʱ��,����ô����������Ķ�ʱ����
			CString strTimerName = pTimer->GetTimerName(uTimerID);
			pTimer->OnTimer(uTimerID, strTimerName);
		}
	}
}

int CTimer::SetTimer(UINT nTimerResolution, CString strTimerName, BOOL bAppTimer)
{
	if(!m_bIsTmierOK)
	{
		TIMECAPS tc;
		//���ú���timeGetDevCapsȡ��ϵͳ�ֱ��ʵ�ȡֵ��Χ������޴������ 
		if(timeGetDevCaps(&tc,sizeof(TIMECAPS))==TIMERR_NOERROR) 	
		{ 
			// �ֱ��ʵ�ֵ���ܳ���ϵͳ��ȡֵ��Χ  
			m_uAccuracy=min(max(tc.wPeriodMin,1),tc.wPeriodMax);
			//����timeBeginPeriod�������ö�ʱ���ķֱ��� 
			timeBeginPeriod(m_uAccuracy); 
			m_bIsTmierOK = true;
		} 
	}

	UINT uTimerID = timeSetEvent(nTimerResolution, m_uAccuracy,(LPTIMECALLBACK) TimerCallProc, (DWORD)this, TIME_PERIODIC);
	TIMER_ITEM item;
	item.uTimerID = uTimerID;
	item.strTimerName = strTimerName;
	item.bAppTimer = bAppTimer;
	m_vecTimer.push_back(item);
	return uTimerID;
}

CString CTimer::GetTimerName(UINT uTimerID)
{
	for(size_t i = 0; i < m_vecTimer.size(); i++)
	{
		if(uTimerID == m_vecTimer[i].uTimerID)
		{
			return m_vecTimer[i].strTimerName;
		}
	}
	return _T("");
}

BOOL CTimer::IsAppTimer(UINT uTimerID)
{
	for(size_t i = 0; i < m_vecTimer.size(); i++)
	{
		if(uTimerID == m_vecTimer[i].uTimerID)
		{
			return m_vecTimer[i].bAppTimer;
		}
	}
	return FALSE;
}

int CTimer::KillTimer(UINT uTimerID)
{
	for(size_t i = 0; i < m_vecTimer.size(); i++)
	{
		if(uTimerID == m_vecTimer[i].uTimerID)
		{
			timeKillEvent(uTimerID);
			m_vecTimer.erase(m_vecTimer.begin() + i);
			break;
		}
	}
	return m_vecTimer.size();
}

int CTimer::KillTimer(CString strTimerName)
{
	for(size_t i = 0; i < m_vecTimer.size(); i++)
	{
		if(strTimerName == m_vecTimer[i].strTimerName)
		{
			timeKillEvent(m_vecTimer[i].uTimerID);
			m_vecTimer.erase(m_vecTimer.begin() + i);
			break;
		}
	}
	return m_vecTimer.size();
}

void CTimer::KillTimer()
{
	for(size_t i = 0; i < m_vecTimer.size(); i++)
	{
		timeKillEvent(m_vecTimer[i].uTimerID);
	}
	m_vecTimer.clear();
}

BOOL CTimer::GetTimerIsRun(UINT uTimerID) 
{
	for(size_t i = 0; i < m_vecTimer.size(); i++)
	{
		if(uTimerID == m_vecTimer[i].uTimerID)
		{
			return TRUE;
		}
	}
	return FALSE; 
}

BOOL CTimer::GetTimerIsRun(CString strTimerName) 
{
	for(size_t i = 0; i < m_vecTimer.size(); i++)
	{
		if(strTimerName == m_vecTimer[i].strTimerName)
		{
			return TRUE;
		}
	}
	return FALSE; 
}