
// MfcBasicView.h: CMfcBasicView 클래스의 인터페이스
//

#pragma once


class CMfcBasicView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMfcBasicView() noexcept;
	DECLARE_DYNCREATE(CMfcBasicView)

// 특성입니다.
public:
	CMfcBasicDoc* GetDocument() const;

// 작업입니다.
public:
	int m_nZoomRate;
	float _fViewportScale;
	float _iViewportWidth;
	float _iViewportHeight;
	CPoint pt;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMfcBasicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // MfcBasicView.cpp의 디버그 버전
inline CMfcBasicDoc* CMfcBasicView::GetDocument() const
   { return reinterpret_cast<CMfcBasicDoc*>(m_pDocument); }
#endif

