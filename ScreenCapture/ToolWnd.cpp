#include "stdafx.h"
#include "ToolWnd.h"


#define  TBBTN_CNT 13  
#define  TBSEP_CNT 2   


const LPWSTR  BUTTONNAMEARR[] = { _T("Redo F1"),_T("Save F2"),_T("Square F3"),
	                              _T("Circle F4"),_T("Arrow F5"),_T("Pen F6"),
								  _T("Pencil F7"),_T("Back Ctrl+Z"),_T("Forward Ctrl+Y"),
								  _T("Exit"),_T("Ok") };
	
CToolWnd::CToolWnd():CWndImpl()
{
	m_pToolBar = NULL;
	m_pTbBtn = NULL;
	m_pTbNormalImage = NULL;
	m_nTbBtnCnt = TBBTN_CNT;

	m_hBrushBk = NULL;

	m_bsChecked.reset();
}

CToolWnd::~CToolWnd()
{
	if(NULL != m_pToolBar)
	{
		delete m_pToolBar;
		m_pToolBar = NULL;
	}

	if(NULL != m_pTbBtn)
	{
		delete[] m_pTbBtn;
		m_pTbBtn = NULL;
	}
	if(NULL != m_pTbNormalImage)
	{
		for(int iDelBMP = 0;iDelBMP < m_nTbBtnCnt - TBSEP_CNT; iDelBMP++)
		{
			DeleteObject(m_pTbNormalImage[iDelBMP]);
		}

		delete[] m_pTbNormalImage;
		m_pTbNormalImage = NULL;
	}

	if(NULL != m_pTbHotImage)
	{
		for(int iDelBMP = 0;iDelBMP < m_nTbBtnCnt - TBSEP_CNT; iDelBMP++)
		{
			DeleteObject(m_pTbHotImage[iDelBMP]);
		}

		delete[] m_pTbHotImage;
		m_pTbHotImage = NULL;
	}


	if(NULL != m_hBrushBk)
	{
		DeleteObject(m_hBrushBk);
		SetWindowLong(GetSafeHwnd(),GCL_HBRBACKGROUND,(LONG)GetStockObject(WHITE_BRUSH));
	}
}

LPCTSTR CToolWnd::GetWindowClassName() const
{
	return _T("CToolWnd");
}

BOOL CToolWnd::SetWndBkColor(COLORREF dwColor)
{
	m_hBrushBk = CreateSolidBrush(dwColor);
	SetClassLong(GetSafeHwnd(),GCL_HBRBACKGROUND,(LONG)m_hBrushBk);

	return TRUE;
}

BOOL CToolWnd::InitTbButtonArr(void)
{
	if(m_nTbBtnCnt <= 0)
		return FALSE;

	m_pTbBtn = new TBBUTTON[m_nTbBtnCnt];

	for(int iID = 0,iCmd = 0; iID < m_nTbBtnCnt; iID++)
	{
		//此处插入分割线
		if((iID == 2) || (iID == 8))
		{
			TBBUTTON tbTemp = {0,0,TBSTATE_ENABLED,TBSTYLE_SEP};
			m_pTbBtn[iID] = tbTemp;
		}
		else
		{
			TBBUTTON tbTemp = {iCmd,IDM_RECAP + iCmd,TBSTATE_ENABLED,TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE};
			m_pTbBtn[iID] = tbTemp;
			if((iID > 2)&&(iID < 8))
			{
				m_pTbBtn[iID].fsStyle = TBSTYLE_CHECK|TBSTYLE_AUTOSIZE;
			}
			else
			{
				m_pTbBtn[iID].fsStyle = TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE;
			}

			iCmd++;
		}

	}

	return TRUE;
}

BOOL CToolWnd::InitTbNormalImageArr(void)
{
	if(m_nTbBtnCnt <= 0)
		return FALSE;

	m_pTbNormalImage = new HBITMAP[m_nTbBtnCnt - TBSEP_CNT];

	for(int iBMP = 0; iBMP < m_nTbBtnCnt - TBSEP_CNT; iBMP++)
	{
		m_pTbNormalImage[iBMP] = LoadBitmap(GetInstance(),MAKEINTRESOURCE(IDB_RECAP + iBMP));		
	}

	return TRUE;
}

BOOL CToolWnd::InitTbHotImageArr(void)
{
	if(m_nTbBtnCnt <= 0)
		return FALSE;

	m_pTbHotImage = new HBITMAP[m_nTbBtnCnt - TBSEP_CNT];

	for(int iBMP = 0; iBMP < m_nTbBtnCnt - TBSEP_CNT; iBMP++)
	{
		HBITMAP hTest = LoadBitmap(GetInstance(),MAKEINTRESOURCE(IDB_RECAP_HOT + iBMP));
		m_pTbHotImage[iBMP] = LoadBitmap(GetInstance(),MAKEINTRESOURCE(IDB_RECAP_HOT + iBMP));		
	}

	return TRUE;
}

void CToolWnd::Initialize()
{
	InitTbButtonArr();
	InitTbNormalImageArr();
	InitTbHotImageArr();
	CreateToolbar();
}

void CToolWnd::CreateToolbar()
{
	m_pToolBar = new CToolBarWnd(m_pTbBtn,m_nTbBtnCnt,TBSEP_CNT,20,20);
	m_pToolBar->Create(GetSafeHwnd(),_T("toolwnd"),WS_CHILD|WS_VISIBLE|CCS_NORESIZE|TBSTYLE_TOOLTIPS|TBSTYLE_FLAT|TBSTYLE_TRANSPARENT,
		WS_EX_TOPMOST|WS_EX_TOOLWINDOW,TOOLBAR_RC);

	m_pToolBar->SetTbButtonImage(m_pTbNormalImage);
	m_pToolBar->SetTbButtonImage(m_pTbHotImage,TB_SETHOTIMAGELIST);
	
	SetWndBkColor(RGB(222,238,254));
}

LRESULT CToolWnd::ProcessMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	default:
		break;
	case WM_PAINT:
		OnPaint(wParam, lParam);
		break;
	case WM_KEYDOWN:
		OnKeyDown(wParam,lParam);
		break;
	case WM_COMMAND:
		OnCommand(wParam, lParam);
		break;
	case WM_NOTIFY:
		OnNotify(wParam, lParam);
		break;
	}

	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

LRESULT  CToolWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg = HIWORD(wParam);
	int iCmd = LOWORD(wParam);

	if(BN_CLICKED == uMsg)
	{
		switch(iCmd)
		{
		default:
			break;
		case IDM_RECAP:
		case IDM_SAVE:
		case IDM_UNDO:
		case IDM_REDO:
		case IDM_CANCEL:
		case IDM_FINISH:
			::SendMessage(GetParent(m_hWnd),SCMSG_RECAP + iCmd,0L,0L);
			break;
		case IDM_RECT:
		case IDM_ELLIPSE:
		case IDM_ARROW:
		case IDM_SCRAWL:
		case IDM_HIGHLIGHT:
			{
				m_bsChecked.flip(iCmd - IDM_RECT);
				for(int iGraphCmd = 0; iGraphCmd < N_GRAPH; ++iGraphCmd )
				{
					if(iCmd - IDM_RECT != iGraphCmd)
					{
						::SendMessage(m_pToolBar->GetSafeHwnd(),TB_SETSTATE,IDM_RECT + iGraphCmd,TBSTATE_ENABLED);
					}
				}
				UINT uState = m_bsChecked.test(iCmd - IDM_RECT) ? TBSTATE_PRESSED|TBSTATE_ENABLED : TBSTATE_ENABLED;
				::SendMessage(m_pToolBar->GetSafeHwnd(),TB_SETSTATE,iCmd,(LPARAM)uState);

				::SendMessage(GetParent(m_hWnd),SCMSG_RECAP + iCmd,WPARAM(m_bsChecked[iCmd - IDM_RECT]),0L);


				std::bitset<N_GRAPH> bsChecked;			
				bsChecked[iCmd - IDM_RECT] = m_bsChecked[iCmd - IDM_RECT];
				m_bsChecked = bsChecked;
			}
			break;

		}//end of switch

	}

	return 0;
}

LRESULT CToolWnd::OnNotify(WPARAM wParam, LPARAM lParam)
{
	int IDBtn = (int)wParam;
	LPNMHDR pNeedText = (LPNMHDR)lParam;;
	LPTOOLTIPTEXT pToolTipText;

	if(TTN_NEEDTEXT == pNeedText->code)
	{
		pToolTipText = (LPTOOLTIPTEXT)lParam;
		pToolTipText->lpszText = BUTTONNAMEARR[IDBtn];
	}

	return 0;
}

LRESULT CToolWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	default:
		break;
	case VK_ESCAPE:
		::SendMessage(GetParent(GetSafeHwnd()),WM_CLOSE,0L,0L);
		break;
	case VK_RETURN:
		::SendMessage(GetParent(GetSafeHwnd()),SCMSG_FINISH,0L,0L);
		break;
	}

	return 0;
}

LRESULT CToolWnd::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paint_;
	HDC hDC = BeginPaint(GetSafeHwnd(),&paint_);

	LOGBRUSH  LogBrush;
	LogBrush.lbStyle = BS_NULL;
	HBRUSH hBrush = CreateBrushIndirect(&LogBrush);
	HBRUSH hOldbrush = (HBRUSH)::SelectObject(hDC, hBrush);

	HPEN hPen = CreatePen(PS_SOLID, 1, COLOR_RCSEL);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	Rectangle(hDC,0,0,GetWindowSize().cx,GetWindowSize().cy);

	::SelectObject(hDC,hOldbrush);
	::SelectObject(hDC,hOldPen);

	DeleteObject(hBrush);
	DeleteObject(hPen);

	EndPaint(GetSafeHwnd(),&paint_);

	return 0;
}


