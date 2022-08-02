#ifndef SCRNCAP_H
#define SCRNCAP_H

#include "WndImpl.hpp"
#include "DrawGraph.hpp"
#include "ToolWnd.h"

typedef enum
{
	ACTION_CHOOSING,
	ACTION_ADJUST,
	ACTION_RECT,
	ACTION_ELLIPSE,
	ACTION_ARROW,
	ACTION_SCRAWL,
	ACTION_HIGHLIGHT,
	ACTION_SAVE,
	ACTION_UNDO,
	ACTION_REDO
}ACTION;


typedef enum
{
	STRETCH_NO,
	STRETCH_L,
	STRETCH_U,
	STRETCH_R,
	STRETCH_D,
	STRETCH_LU,
	STRETCH_RU,
	STRETCH_LD,
	STRETCH_RD
}STRETCH;

class CScrnCapWnd: public CWndImpl
{
public:
	CScrnCapWnd();
	~CScrnCapWnd();

public:
	LPCTSTR   GetWindowClassName() const;
	void Initialize(void);

	LRESULT ProcessMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//sys msg processing
	LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDbClk(WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);

	//user msg processing
	LRESULT OnRecap(WPARAM wParam, LPARAM lParam);
	LRESULT OnSaveImage(WPARAM wParam, LPARAM lParam);
	LRESULT OnRectangle(WPARAM wParam, LPARAM lParam);
	LRESULT OnEllipse(WPARAM wParam, LPARAM lParam);
	LRESULT OnArrowLine(WPARAM wParam, LPARAM lParam);
	LRESULT OnScrawl(WPARAM wParam, LPARAM lParam);
	LRESULT OnHighlight(WPARAM wParam, LPARAM lParam);
	LRESULT OnUndo(WPARAM wParam, LPARAM lParam);
	LRESULT OnRedo(WPARAM wParam, LPARAM lParam);
	LRESULT OnCancel(WPARAM wParam, LPARAM lParam);
	LRESULT OnFinish(WPARAM wParam, LPARAM lParam);

	LRESULT OnPenSizeChange(WPARAM wParam, LPARAM lParam);
	LRESULT OnPenColorChange(WPARAM wParam, LPARAM lParam);

public:
	void InitializeDC(void);


	void CreateToolWnd(void);

	void CreateColorWnd(BOOL bShow);

	BOOL AutoRecognizeWnd(const POINT& ptMousePos);


	void Stretch_Adjust(const STRETCH& emStretch,RectX& rcStretch,int cxOffset,int cyOffset);
	void Drag_Adjust(RectX& rcDrag,int cxOffset,int cyOffset);
	BOOL Adjust(int cxOffset,int cyOffset);


	void AdjustToolPos(void);

	void Enlarge(RectX& rcEnlarge, int iSpeed);


	void PaintSelRgn(void);   

	void Undo(void);
	void Redo(void);

	BOOL Save(void);

	void Finish(void);

	void ClearUndoStack(void);
	void ClearRedoStack(void);

	//@brief  API rewriting
	static BOOL BitBltEx(HDC hDestDC,RectX rc,HDC hSrcDC, POINT ptStart, DWORD dwRop);
	static int  DrawTextEx(HDC hDC, const CString& strTx, RectX rc, UINT uFormat);

	static BOOL RectangleX(HDC hDC, RectX rc); 


	//Painting


	static void DrawAdjustSquare(HDC hDC,const RectX& rc, const int iSide); 
	static void DrawRect(HDC hDC, const RectX& rcDraw, const int& iPenWidth, const int& iPenStyle);
	static void DrawSizeTx(HDC hDC, const RectX& rcSel);

	//@brief  枚举所有窗口
	static BOOL CALLBACK EnumGrandChildWindowsProc(HWND hwnd,LPARAM lParam);
	static BOOL CALLBACK EnumChildWindowsProc(HWND hwnd,LPARAM lParam);
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);
	static BOOL InsertWndRect(HWND hwnd);
	static std::vector<RectX> GetAllWndRect();

	static void ConvertToGrayBitmap(HBITMAP hSourceBmp,HDC sourceDC);



	void SetScrnCursor(HWND hWnd, const RectX& rcCursorLie, const BOOL& bLButtonDown, const ACTION& m_emAction, STRETCH& emStretch);
	static void SetStretchCursor(const RectX& rc,const POINT& ptPos, STRETCH& emStretch);

	static STRETCH GetStrechDrct(const RectX& rc,const POINT& ptPos);



	static BOOL GetSaveFilePath(HINSTANCE hInstance,HWND hWnd, TCHAR* szFilePath, TCHAR* szFileTitle,int& nFilterIndex);
	static void GetCurTimeString(CString& strCurDateTime);
	static void SetDefFileName(CString& strDefName);

	static BOOL GetEncoderClsid(const WCHAR* format, CLSID* pClsid);


	static BOOL SaveAsPic(HBITMAP hImage, const CString& pFileName, const CString& strFormat);


	static BOOL CopyBMP2Clipboard(HBITMAP hBMP,HWND hWnd);



private:

	HDC  m_hClientDC;     
	HDC  m_hMemPaintDC;    
	HDC  m_hMemDC;          
	HDC  m_hMemCurScrnDC;   

	HBITMAP m_hScrnBMP, m_hOldScrnBMP;     
	HBITMAP m_hGrayBMP, m_hOldGrayBMP;
	HBITMAP m_hGrayBMP2, m_hOldGrayBMP2;
	HBITMAP m_hCurScrnBMP, m_hOldCurScrnBMP; 

	HBITMAP m_hGraphBMP, m_hOldGraphBMP;     


	HBITMAP m_hCurBMP, m_hOldCurBMP;         
	std::stack<HBITMAP> m_stackUndoGraph;    
	std::stack<HBITMAP> m_stackRedoGraph;    


	POINT m_ptStart;
	POINT m_ptMoving;

	BOOL m_bLBtnDown;    
	BOOL m_bStretching;  

	ACTION m_emAction;    
	STRETCH m_emStrech;   

	RectX m_rcSel;       
	RectX m_rcChoosing;  
	RectX m_rcSelRedo;   

	IGraph *m_pGraph;
	CToolWnd* m_pToolWnd;
	CColorWnd* m_pColorWnd;

	int m_nPenWidth;
	COLORREF m_dwPenColor;

private:
	static HCURSOR m_hCursor;
	static std::vector<RectX> m_vecAllWndRect;
};

#endif