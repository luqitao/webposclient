#include "StdAfx.h"
#include "DuiEdit.h"

CDuiEdit::CDuiEdit(HWND hWnd, CDuiObject* pDuiObject)
		: CControlBaseFont(hWnd, pDuiObject)
{
	m_bTabStop = TRUE;	// ������Ӧtab��
	m_bDown = false;
	m_bDownTemp = false;
	m_buttonState = enBSNormal;
	m_EditState = enBSNormal;
	m_pLeftImage = NULL;
	m_pSmallImage = NULL;
	m_pEdit = NULL;
	m_strTitle = _T("");

	SetRect(CRect(0,0,0,0));
	SetBitmapCount(4);

	m_sizeLeftImage.SetSize(0,0);
	m_sizeSmallImage.SetSize(0,0);

	m_bPassWord = false;
	m_bMultiLine = false;
	m_bWantReturn = true;
	m_bAutoHScroll = false;
	m_bAutoVScroll = false;
	m_bNumber = false;
	m_bReadOnly = false;
	m_nMaxChar = -1;

	m_bIsSmallButton = FALSE;

	VERIFY(m_fontTemp.CreateFont(
		18,							// ����ĸ߶�  
		0,							// ����Ŀ��  
		0,							// ������ʾ�ĽǶ�
		0,							// ����ĽǶ�
		FW_DONTCARE,				// ����İ���
		FALSE,						// б������
		FALSE,						// ���»��ߵ�����
		0,							// ��ɾ���ߵ�����
		GB2312_CHARSET,				// ������ַ���
		OUT_DEFAULT_PRECIS,			// ����ľ���
		CLIP_DEFAULT_PRECIS,		// �ü��ľ���
		DEFAULT_QUALITY,			// �߼�����������豸��ʵ��
		DEFAULT_PITCH | FF_SWISS,	// ����������弯
		DuiSystem::GetDefaultFont()));					// ��������
}

CDuiEdit::CDuiEdit(HWND hWnd, CDuiObject* pDuiObject, UINT uControlID, CRect rc, CString strTitle/* = ""*/,
			BOOL bPassWord/* = FALSE*/, BOOL bIsVisible/* = TRUE*/, BOOL bIsDisable/* = FALSE*/ ,BOOL bIsPressDown/* = FALSE*/)
		: CControlBaseFont(hWnd, pDuiObject, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_bTabStop = TRUE;	// ������Ӧtab��
	m_bDown = false;
	m_bDownTemp = false;
	m_buttonState = enBSNormal;
	m_EditState = enBSNormal;
	m_pLeftImage = NULL;
	m_pSmallImage = NULL;
	m_pEdit = NULL;
	m_bPassWord = bPassWord;
	m_strTitle = strTitle;

	m_sizeLeftImage.SetSize(0,0);
	m_sizeSmallImage.SetSize(0,0);

	SetRect(rc);
	SetBitmapCount(4);

	m_bMultiLine = false;
	m_bAutoHScroll = false;
	m_bAutoVScroll = false;
	m_bNumber = false;
	m_bReadOnly = false;
	m_nMaxChar = -1;

	m_bIsSmallButton = FALSE;

	VERIFY(m_fontTemp.CreateFont(
		18,							// ����ĸ߶�  
		0,							// ����Ŀ��  
		0,							// ������ʾ�ĽǶ�
		0,							// ����ĽǶ�
		FW_DONTCARE,				// ����İ���
		FALSE,						// б������
		FALSE,						// ���»��ߵ�����
		0,							// ��ɾ���ߵ�����
		GB2312_CHARSET,				// ������ַ���
		OUT_DEFAULT_PRECIS,			// ����ľ���
		CLIP_DEFAULT_PRECIS,		// �ü��ľ���
		DEFAULT_QUALITY,			// �߼�����������豸��ʵ��
		DEFAULT_PITCH | FF_SWISS,	// ����������弯
		DuiSystem::GetDefaultFont()));					// ��������
}

CDuiEdit::~CDuiEdit(void)
{
	// ɾ��Windows�ؼ�
	HideEdit();

	if (m_fontTemp.m_hObject)
	{
		m_fontTemp.DeleteObject();
	}

	if(m_pLeftImage != NULL)
	{
		delete m_pLeftImage;
		m_pLeftImage = NULL;
	}

	if(m_pSmallImage != NULL)
	{
		delete m_pSmallImage;
		m_pSmallImage = NULL;
	}
}

bool CDuiEdit::SetLeftBitmap(UINT nResourceID, CString strType)
{
	if(LoadImageFromIDResource(nResourceID, strType, m_bImageUseECM, m_pLeftImage))
	{
		// �����߱ȴ��ڻ����3,����4��ͼƬ,�����տ�Ⱥ͸߶���ͬ�����ÿ�Ⱥ͸߶�
		// �����4��ͼƬ�����,����ΪСͼƬ��һ����ť
		if((m_pLeftImage->GetWidth() / m_pLeftImage->GetHeight()) >= 3)
		{
			m_sizeLeftImage.SetSize(m_pLeftImage->GetWidth() / 4, m_pLeftImage->GetHeight());
			m_nLeftImageCount = 4;
		}else
		{
			m_sizeLeftImage.SetSize(m_pLeftImage->GetHeight(), m_pLeftImage->GetHeight());
			m_nLeftImageCount = m_pLeftImage->GetWidth() / m_pLeftImage->GetHeight();
		}
		return true;
	}
	return false;
}

bool CDuiEdit::SetLeftBitmap(CString strImage)
{
	if(DuiSystem::Instance()->LoadImageFile(strImage, m_bImageUseECM, m_pLeftImage))
	{
		// �����߱ȴ��ڻ����3,����4��ͼƬ,�����տ�Ⱥ͸߶���ͬ�����ÿ�Ⱥ͸߶�
		// �����4��ͼƬ�����,����ΪСͼƬ��һ����ť
		if((m_pLeftImage->GetWidth() / m_pLeftImage->GetHeight()) >= 3)
		{
			m_sizeLeftImage.SetSize(m_pLeftImage->GetWidth() / 4, m_pLeftImage->GetHeight());
			m_nLeftImageCount = 4;
		}else
		{
			m_sizeLeftImage.SetSize(m_pLeftImage->GetHeight(), m_pLeftImage->GetHeight());
			m_nLeftImageCount = m_pLeftImage->GetWidth() / m_pLeftImage->GetHeight();
		}
		return true;
	}
	return false;
}

// ��XML����ͼƬ��Ϣ����
HRESULT CDuiEdit::OnAttributeLeftImage(const CString& strValue, BOOL bLoading)
{
	if (strValue.IsEmpty()) return E_FAIL;

	// ͨ��Skin��ȡ
	CString strSkin = _T("");
	if(strValue.Find(_T("skin:")) == 0)
	{
		strSkin = DuiSystem::Instance()->GetSkin(strValue);
		if (strSkin.IsEmpty()) return E_FAIL;
	}else
	{
		strSkin = strValue;
	}

	if(strSkin.Find(_T(".")) != -1)	// ����ͼƬ�ļ�
	{
		CString strImgFile = strSkin;
		if(strSkin.Find(_T(":")) != -1)
		{
			strImgFile = strSkin;
		}
		if(!SetLeftBitmap(strImgFile))
		{
			return E_FAIL;
		}
	}else	// ����ͼƬ��Դ
	{
		UINT nResourceID = _ttoi(strSkin);
		if(!SetLeftBitmap(nResourceID, TEXT("PNG")))
		{
			if(!SetLeftBitmap(nResourceID, TEXT("BMP")))
			{
				return E_FAIL;
			}
		}
	}

	return bLoading?S_FALSE:S_OK;
}

bool CDuiEdit::SetSmallBitmap(UINT nResourceID, CString strType)
{
	if(LoadImageFromIDResource(nResourceID, strType, m_bImageUseECM, m_pSmallImage))
	{
		// �����߱ȴ��ڻ����3,����4��ͼƬ,�����տ�Ⱥ͸߶���ͬ�����ÿ�Ⱥ͸߶�
		// �����4��ͼƬ�����,����ΪСͼƬ��һ����ť
		if((m_pSmallImage->GetWidth() / m_pSmallImage->GetHeight()) >= 3)
		{
			m_sizeSmallImage.SetSize(m_pSmallImage->GetWidth() / 4, m_pSmallImage->GetHeight());
			m_nSmallImageCount = 4;
			m_bIsSmallButton = TRUE;
		}else
		{
			m_sizeSmallImage.SetSize(m_pSmallImage->GetHeight(), m_pSmallImage->GetHeight());
			m_nSmallImageCount = m_pSmallImage->GetWidth() / m_pSmallImage->GetHeight();
			m_bIsSmallButton = FALSE;
		}
		return true;
	}
	return false;
}

bool CDuiEdit::SetSmallBitmap(CString strImage)
{
	if(DuiSystem::Instance()->LoadImageFile(strImage, m_bImageUseECM, m_pSmallImage))
	{
		// �����߱ȴ��ڻ����3,����4��ͼƬ,�����տ�Ⱥ͸߶���ͬ�����ÿ�Ⱥ͸߶�
		// �����4��ͼƬ�����,����ΪСͼƬ��һ����ť
		if((m_pSmallImage->GetWidth() / m_pSmallImage->GetHeight()) >= 3)
		{
			m_sizeSmallImage.SetSize(m_pSmallImage->GetWidth() / 4, m_pSmallImage->GetHeight());
			m_nSmallImageCount = 4;
			m_bIsSmallButton = TRUE;
		}else
		{
			m_sizeSmallImage.SetSize(m_pSmallImage->GetHeight(), m_pSmallImage->GetHeight());
			m_nSmallImageCount = m_pSmallImage->GetWidth() / m_pSmallImage->GetHeight();
			m_bIsSmallButton = FALSE;
		}
		return true;
	}
	return false;
}

// ��XML����ͼƬ��Ϣ����
HRESULT CDuiEdit::OnAttributeSmallImage(const CString& strValue, BOOL bLoading)
{
	if (strValue.IsEmpty()) return E_FAIL;

	// ͨ��Skin��ȡ
	CString strSkin = _T("");
	if(strValue.Find(_T("skin:")) == 0)
	{
		strSkin = DuiSystem::Instance()->GetSkin(strValue);
		if (strSkin.IsEmpty()) return E_FAIL;
	}else
	{
		strSkin = strValue;
	}

	if(strSkin.Find(_T(".")) != -1)	// ����ͼƬ�ļ�
	{
		CString strImgFile = strSkin;
		if(strSkin.Find(_T(":")) != -1)
		{
			strImgFile = strSkin;
		}
		if(!SetSmallBitmap(strImgFile))
		{
			return E_FAIL;
		}
	}else	// ����ͼƬ��Դ
	{
		UINT nResourceID = _ttoi(strSkin);
		if(!SetSmallBitmap(nResourceID, TEXT("PNG")))
		{
			if(!SetSmallBitmap(nResourceID, TEXT("BMP")))
			{
				return E_FAIL;
			}
		}
	}

	return bLoading?S_FALSE:S_OK;
}

void  CDuiEdit::SetControlRect(CRect rc) 
{
	m_rc = rc;
	m_rcText = m_rc;
	m_rcText.top += 4;
	m_rcText.left += (6 + m_sizeLeftImage.cx);
	m_rcText.bottom -= 4;
	m_rcText.right -= (3 + m_sizeSmallImage.cx);
}

BOOL CDuiEdit::IsDraw(CPoint point)
{
 	if(m_buttonState == enBSDown)
 	{
 		return false;
 	}
	CRect  rc;
	rc.left = m_rc.right - m_sizeSmallImage.cx - 2;
	rc.top = m_rc.top + (m_rc.Height() - m_sizeSmallImage.cy) / 2;
	rc.right = rc.left + m_sizeSmallImage.cx;
	rc.bottom = rc.top + m_sizeSmallImage.cy;

	if(rc.PtInRect(point))
	{
		if(m_buttonState == enBSNormal)
			return true;
	}
	else
	{
		if(m_buttonState != enBSNormal)
			return true;
	}

	return false;
}

// ���ÿؼ��е�Windowsԭ���ؼ��Ƿ�ɼ���״̬
void CDuiEdit::SetControlWndVisible(BOOL bIsVisible)
{
	if(bIsVisible)
	{
		//ShowEdit();
	}else
	{
		HideEdit();
	}
}

// ���ÿؼ��Ľ���״̬
void CDuiEdit::SetControlDisable(BOOL bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			m_EditState = enBSDisable;
			m_buttonState = enBSDisable;
			HideEdit();
		}
		else
		{
			if(m_bDown)
			{
				m_EditState = enBSDown;
				m_buttonState = enBSDown;
			}
			else
			{
				m_EditState = enBSNormal;
				m_buttonState = enBSNormal;
			}
		}
		::InvalidateRect(m_hWnd, &m_rc, true);
	}
}

// ���ÿؼ�title
void CDuiEdit::SetControlTitle(CString strTitle)
{
	__super::SetControlTitle(strTitle);

	// ���edit�ؼ��Ѿ�����,����Ҫ����edit�ؼ�������
	if(m_pEdit)
	{
		m_pEdit->SetWindowText(m_strTitle);	
		m_pEdit->SetSel(m_strTitle.GetLength(), -1);
	}
}

// ���ÿؼ��Ľ���
BOOL CDuiEdit::SetControlFocus(BOOL bFocus)
{
	__super::SetControlFocus(bFocus);

	enumButtonState buttonState = m_buttonState;
	enumButtonState editState = m_EditState;

	if(!bFocus)
	{
		m_bDown = false;
		m_buttonState = enBSNormal;
		m_EditState = enBSNormal;
		HideEdit();
	}else
	{
		m_bDown = true;
		m_buttonState = enBSDown;
		m_EditState = enBSDown;
		ShowEdit();
	}

	bool bIsDraw = buttonState != m_buttonState || editState != m_EditState;
	if(bIsDraw)
	{
		::InvalidateRect(m_hWnd, &m_rc, true);
	}

	return bIsDraw;
}

BOOL CDuiEdit::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_buttonState;
	enumButtonState editState = m_EditState;
	if(!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_EditState = enBSHover;
			if(m_buttonState != enBSDown)
			{
				CRect  rc;
				rc.left = m_rc.right - m_sizeSmallImage.cx - 2;
				rc.top = m_rc.top + (m_rc.Height() - m_sizeSmallImage.cy) / 2;
				rc.right = rc.left + m_sizeSmallImage.cx;
				rc.bottom = rc.top + m_sizeSmallImage.cy;

				if(rc.PtInRect(point))
				{
					m_buttonState = enBSHover;
				}
				else
				{
					//ShowEdit();
					m_buttonState = enBSNormal;
				}
			}
		}
		else
		{
			if(m_buttonState != enBSDown)
			{
				m_buttonState = enBSNormal;
				m_EditState = enBSNormal;
			}
		}
	}
	
	if(buttonState != m_buttonState || editState != m_EditState)
	{
		UpdateControl();
	}

	return buttonState != m_buttonState || editState != m_EditState;
}

BOOL CDuiEdit::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_buttonState;	
	enumButtonState editState = m_EditState;
	if(!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			CRect  rc;
			rc.left = m_rc.right - m_sizeSmallImage.cx - 2;
			rc.top = m_rc.top + (m_rc.Height() - m_sizeSmallImage.cy) / 2;
			rc.right = rc.left + m_sizeSmallImage.cx;
			rc.bottom = rc.top + m_sizeSmallImage.cy;

			if(rc.PtInRect(point))	// ������Ǳ༭���Ҳ���С��ťͼƬ
			{
				if(m_bIsSmallButton)	// СͼƬ��Ϊ��ť�Ž��д���
				{
					m_bDown = !m_bDown;
					m_bDownTemp = true;
					if(m_bDown)
					{
						m_buttonState = enBSDown;
					}
					else
					{				
						m_buttonState = enBSHover;
					}				
					SendMessage(m_uID, CONTROL_BUTTON, MSG_BUTTON_DOWN);
					HideEdit();
				}
			}
			else
			{
				if(m_bDown)
				{
					m_bDown = false;
					m_buttonState = enBSHover;
				}
				ShowEdit();
				
				SendMessage(m_uID, CONTROL_EDIT, MSG_BUTTON_DOWN);
			}		
		}
		else
		{
			m_buttonState = enBSNormal;
			m_EditState = enBSNormal;
		}
	}

	if(buttonState != m_buttonState || editState != m_EditState)
	{
		UpdateControl();
	}
	
	return buttonState != m_buttonState || editState != m_EditState;
}

BOOL CDuiEdit::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_buttonState;
	enumButtonState editState = m_EditState;
	if(!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_EditState = enBSHover;

			CRect  rc;
			rc.left = m_rc.right - m_sizeSmallImage.cx - 2;
			rc.top = m_rc.top + (m_rc.Height() - m_sizeSmallImage.cy) / 2;
			rc.right = rc.left + m_sizeSmallImage.cx;
			rc.bottom = rc.top + m_sizeSmallImage.cy;

			if(rc.PtInRect(point))
			{
				if(m_bIsSmallButton)	// СͼƬ��Ϊ��ť�Ž��д���
				{
					if(m_bDown)
					{
						m_buttonState = enBSDown;
					}
					else
					{
						m_buttonState = enBSHover;
					}	
					SendMessage(m_uID, CONTROL_BUTTON, MSG_BUTTON_UP);
				}
			}
			else
			{
				if(m_bDown)
				{
					m_buttonState = enBSDown;
				}
				else
				{
					m_buttonState = enBSNormal;
				}	
				SendMessage(m_uID, CONTROL_EDIT, MSG_BUTTON_UP);
			}			
		}
		else
		{
			if(!m_bDown)
			{
				m_buttonState = enBSNormal;
				m_EditState = enBSNormal;
			}
		}
	}
	
	m_bDownTemp = false;

	if(buttonState != m_buttonState || editState != m_EditState)
	{
		UpdateControl();
	}

	return buttonState != m_buttonState || editState != m_EditState;
}

// �����¼�����
BOOL CDuiEdit::OnControlKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// ����ǻس���,��ת��Ϊ�ַ��¼����ݸ�ԭ���ؼ�
	if((nChar == VK_RETURN) && m_pEdit && ::IsWindow(m_pEdit->GetSafeHwnd()))
	{
		m_pEdit->SendMessage(WM_CHAR, VK_RETURN, nFlags);
		return true;
	}

	return false;
}

void CDuiEdit::DrawControl(CDC &dc, CRect rcUpdate)
{
	Graphics graphics(dc);

	DrawImageFrame(graphics, m_pImage, m_rc, m_EditState * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, 4);

	if(m_pLeftImage)
	{
		CRect  rc;
		rc.left = m_rc.left + 2;
		rc.top = m_rc.top + (m_rc.Height() - m_sizeLeftImage.cy) / 2;
		rc.right = rc.left + m_sizeLeftImage.cx;
		rc.bottom = rc.top + m_sizeLeftImage.cy;
		
		if(m_nLeftImageCount > m_buttonState)
		{
			graphics.DrawImage(m_pLeftImage, RectF((Gdiplus::REAL)rc.left , (Gdiplus::REAL)rc.top, (Gdiplus::REAL)rc.Width(), (Gdiplus::REAL)rc.Height()),
				(Gdiplus::REAL)(m_buttonState * m_sizeLeftImage.cx), 0, (Gdiplus::REAL)m_sizeLeftImage.cx, (Gdiplus::REAL)m_sizeLeftImage.cy, UnitPixel);
		}else
		{
			graphics.DrawImage(m_pLeftImage, RectF((Gdiplus::REAL)rc.left , (Gdiplus::REAL)rc.top, (Gdiplus::REAL)rc.Width(), (Gdiplus::REAL)rc.Height()),
				0, 0, (Gdiplus::REAL)m_sizeLeftImage.cx, (Gdiplus::REAL)m_sizeLeftImage.cy, UnitPixel);
		}
	}

	if(m_pSmallImage)
	{
		CRect  rc;
		rc.left = m_rc.right - m_sizeSmallImage.cx - 2;
		rc.top = m_rc.top + (m_rc.Height() - m_sizeSmallImage.cy) / 2;
		rc.right = rc.left + m_sizeSmallImage.cx;
		rc.bottom = rc.top + m_sizeSmallImage.cy;
		
		if(m_nSmallImageCount > m_buttonState)
		{
			graphics.DrawImage(m_pSmallImage, RectF((Gdiplus::REAL)rc.left , (Gdiplus::REAL)rc.top, (Gdiplus::REAL)rc.Width(), (Gdiplus::REAL)rc.Height()),
				(Gdiplus::REAL)(m_buttonState * m_sizeSmallImage.cx), 0, (Gdiplus::REAL)m_sizeSmallImage.cx, (Gdiplus::REAL)m_sizeSmallImage.cy, UnitPixel);
		}else
		{
			graphics.DrawImage(m_pSmallImage, RectF((Gdiplus::REAL)rc.left , (Gdiplus::REAL)rc.top, (Gdiplus::REAL)rc.Width(), (Gdiplus::REAL)rc.Height()),
				0, 0, (Gdiplus::REAL)m_sizeSmallImage.cx, (Gdiplus::REAL)m_sizeSmallImage.cy, UnitPixel);
		}
	}

	BSTR bsFont = m_strFont.AllocSysString();
	FontFamily fontFamily(bsFont);
	Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
	SolidBrush solidBrush(m_clrText);
	SolidBrush solidBrushTip(m_clrTooltip);
	graphics.SetTextRenderingHint( TextRenderingHintClearTypeGridFit );
	::SysFreeString(bsFont);
	StringFormat strFormat;
	strFormat.SetAlignment(StringAlignmentNear);		// ˮƽ���������
	if(!m_bMultiLine)
	{
		// ��������
		strFormat.SetLineAlignment(StringAlignmentCenter);	// ��ֱ�����м����
		strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
	}else
	{
		strFormat.SetLineAlignment(StringAlignmentNear);	// ��ֱ�����϶���
	}

	RectF rect((Gdiplus::REAL)m_rcText.left, (Gdiplus::REAL)(m_rcText.top+2), (Gdiplus::REAL)m_rcText.Width(), (Gdiplus::REAL)(m_rcText.Height()-2));

	if(!m_strTitle.IsEmpty())
	{
		// ���ַǿ�
		CString strTitle = m_strTitle;
		if(m_bPassWord)
		{
			int nlen = strTitle.GetLength();
			strTitle = "";
			for(int i = 0; i < nlen; i++)
			{
				strTitle += '*';
			}
		}

		BSTR bsTitle = strTitle.AllocSysString();
		graphics.DrawString(bsTitle, (INT)wcslen(bsTitle), &font, rect, &strFormat, &solidBrush);
		::SysFreeString(bsTitle);
	}else
	if(!m_strTooltip.IsEmpty())
	{
		// ���û������,��������tooltip,����ʾtooltip
		BSTR bsTooltip = m_strTooltip.AllocSysString();
		graphics.DrawString(bsTooltip, (INT)wcslen(bsTooltip), &font, rect, &strFormat, &solidBrushTip);
		::SysFreeString(bsTooltip);
	}
}

BOOL CDuiEdit::GetLButtonDown()
{
	return m_buttonState == enBSDown;
}

BOOL CDuiEdit::GetLButtonUp()
{
	return FALSE;
}

CString CDuiEdit::GetEditText()
{
	if(m_pEdit && ::IsWindow(m_pEdit->GetSafeHwnd()))
	{
		m_pEdit->GetWindowText(m_strTitle);
	}
	return m_strTitle;
}

void CDuiEdit::ShowEdit()
{
	// �����ֻ�����ԣ����ô���edit�ؼ�
	if(m_bReadOnly || m_bIsDisable)
	{
		return;
	}

 	if(NULL == m_pEdit)
 	{
		TestMainThread();	// �����Ƿ������߳�

		CRect rc;
		rc = m_rcText;
		//rc.left--;
		if(!m_bMultiLine)
		{
			// ����Ե��б༭�����λ��
			rc.top += (m_rc.Height() - 18 - 6) / 2;	// ����windows�ؼ�����ʱ�����ʾλ��,�ͷǼ���״̬�¸��ӽ�һЩ
		}else
		{
			rc.top += 2;
		}
  		m_pEdit = new CEdit;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
		if(m_bMultiLine)
		{
			dwStyle |= ES_MULTILINE;
		}
		if(m_bWantReturn)
		{
			dwStyle |= ES_WANTRETURN;
		}
		if(m_bAutoHScroll)
		{
			dwStyle |= ES_AUTOHSCROLL;
		}
		if(m_bAutoVScroll)
		{
			dwStyle |= ES_AUTOVSCROLL;
		}
		if(m_bNumber)
		{
			dwStyle |= ES_NUMBER;
		}
		if(m_bReadOnly)
		{
			dwStyle |= ES_READONLY;
		}
  		m_pEdit->Create(dwStyle, rc, CWnd::FromHandle(m_hWnd), GetID());
  		m_pEdit->SetFont(&m_fontTemp);
		m_pEdit->SetWindowText(m_strTitle);	
		if(m_bPassWord)
		{
			m_pEdit->SetPasswordChar('*');
		}
		if(m_nMaxChar > 0)
		{
			m_pEdit->LimitText(m_nMaxChar);
		}
		m_pEdit->SetSel(m_strTitle.GetLength(), -1);
		m_pEdit->SetFocus();
 	}
}

void CDuiEdit::HideEdit()
{
	if(m_pEdit)
	{
		TestMainThread();	// �����Ƿ������߳�

		// ����Edit��Windows�ؼ��������жϴ����Ƿ���Ч
		if(::IsWindow(m_pEdit->GetSafeHwnd()))
		{
			// ��ȡ�༭������ݱ����ڿؼ��ı�����
			m_pEdit->GetWindowText(m_strTitle);
		}
		delete m_pEdit;
		m_pEdit = NULL;
	}
}

// ��Ϣ����
LRESULT CDuiEdit::OnMessage(UINT uID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::OnMessage(uID, uMsg, wParam, lParam);
}