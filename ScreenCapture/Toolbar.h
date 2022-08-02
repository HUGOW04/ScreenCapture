#ifndef  TOOLBARWND_H
#define  TOOLBARWND_H

#include "WndImpl.hpp"

class CToolBarWnd : public CWndImpl
{
public:
	CToolBarWnd(TBBUTTON* pTbBtn, int nBtnCnt,int nSepCnt,int cxImage,int cyImage);
	~CToolBarWnd();
public:
	LPCTSTR GetWindowClassName() const;
	void Initialize(void);
	BOOL SetTbButtonImage(HBITMAP* phImage,UINT uMsgSetImage = TB_SETIMAGELIST,COLORREF crMask = RGB(255,255,255));

private:
	TBBUTTON* m_pTbButton;          

	int m_nTbButtonCnt;             
	int m_nSepCnt;                  

	int m_cxImage;
	int m_cyImage;
};

#endif