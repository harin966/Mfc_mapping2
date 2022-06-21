
// MfcBasicView.cpp: CMfcBasicView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MfcBasic.h"
#endif

#include "MfcBasicDoc.h"
#include "MfcBasicView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMfcBasicView

IMPLEMENT_DYNCREATE(CMfcBasicView, CView)

BEGIN_MESSAGE_MAP(CMfcBasicView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
//	ON_WM_PAINT()
END_MESSAGE_MAP()

// CMfcBasicView 생성/소멸

CMfcBasicView::CMfcBasicView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMfcBasicView::~CMfcBasicView()
{
}

BOOL CMfcBasicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.lpszClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		LoadIcon(NULL, IDI_APPLICATION));

	return CView::PreCreateWindow(cs);
}

// CMfcBasicView 그리기

void CMfcBasicView::OnDraw(CDC* pDC)
{
	CMfcBasicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!(pDoc->m_imgMat.empty())) {

		//pDC->SetMapMode(MM_ISOTROPIC);
		//pDC->SetWindowExt(100,100);
		////pDC->SetWindowOrg((long)&pt.x, (long)&pt.y);
		//pDC->SetViewportExt(m_nZoomRate, m_nZoomRate);
		//TRACE("m_nZoomRate = %d/n", m_nZoomRate);
		//pDC->SetViewportOrg(0,0);

		Graphics g(pDC->m_hDC);
		// 비율 조정
		CRect Rect;
		GetClientRect(&Rect);
		float width = Rect.right - Rect.left, height = Rect.bottom - Rect.top;
		float rectRate = width / height, imgRate = (float)pDoc->m_imgMat.size().width / (float)pDoc->m_imgMat.size().height;

		int priority_range = ((imgRate > rectRate && imgRate < 1) || (imgRate < rectRate&& imgRate >= 1)) ? 1 : 0;
		if (imgRate < 1) priority_range = 1 - priority_range;

		if (priority_range) width = height * imgRate;
		else height = width / imgRate;

		int interval_x = (Rect.right - (int)width) / 2, interval_y = (Rect.bottom - (int)height) / 2;

		if (priority_range) {
			Rect.left = interval_x; Rect.right = (int)width; Rect.top = 0; Rect.bottom = (int)height;
		}
		else {
			Rect.left = 0; Rect.right = (int)width; Rect.top = interval_y; Rect.bottom = (int)height;
		}

		pDC->SetMapMode(MM_ISOTROPIC);
		pDC->SetWindowExt(100,100);
		//pDC->SetWindowOrg((long)&pt.x, (long)&pt.y);
		pDC->SetViewportExt(m_nZoomRate, m_nZoomRate);
		TRACE("m_nZoomRate = %d\n", m_nZoomRate);
		TRACE("pt.x = %d\n pt.y = %d", &pt.x, &pt.y);
		pDC->SetViewportOrg(0,0);
		
		//pDC->SetViewportOrg(0, 0);
		//Graphics g(pDC->m_hDC);


		// cv로 그리기
		cvtColor(pDoc->m_imgMat, pDoc->m_imgMat, COLOR_BGR2BGRA);

		Bitmap bitmap((INT)pDoc->m_imgMat.size().width, (INT)pDoc->m_imgMat.size().height, (INT)pDoc->m_imgMat.step,
			PixelFormat32bppARGB, pDoc->m_imgMat.data);

		//g.DrawImage(&bitmap, Rect.left, Rect.top, Rect.right, Rect.bottom);
		g.DrawImage(&bitmap, Rect.left, Rect.top, (int)width, (int)height);
		

		//gdi+ 회전
		/*Gdiplus::Matrix matrix;
		matrix.RotateAt(90,
			Gdiplus::PointF((float)pDoc->m_img->GetWidth(), (float)pDoc->m_img->GetHeight())
		);
		g.SetTransform(&matrix);*/
		//g.DrawImage(pDoc->m_img, 0, 0);

		// cv 플립
		/*Mat dst;
		int flipCode[] = { 1,0,-1 };
		for (int i = 0; i < 1; i++) {
		flip(pDoc->m_imgMat, dst, flipCode[i]);

			String desc = format("flip");
			putText(dst, desc, cv::Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0,
				Scalar(255, 0, 0), 1, LINE_AA);

			Bitmap bitmap((INT)dst.size().width, (INT)dst.size().height, (INT)dst.step,
				PixelFormat32bppARGB, dst.data);
			
			g.DrawImage(&bitmap, Rect.left, Rect.top, Rect.right, Rect.bottom);
		}*/

	}

	
	/*pDoc->m_img = Image::FromFile(pDoc->m_ImgPath);
	g.DrawImage(pDoc->m_img,300,200, pDoc->m_img->GetWidth(), pDoc->m_img->GetHeight());*/

	//pDC->SetMapMode(MM_ISOTROPIC);
	//pDC->SetWindowExt(100, 100);
	//pDC->SetViewportExt(m_nZoomRate, m_nZoomRate);
	//TRACE("m_nZoomRate = %d/n", m_nZoomRate);
	//pDC->SetViewportOrg(300, 300);

	//g.ScaleTransform(10, 10);
	//g.ScaleTransform(m_nZoomRate, m_nZoomRate);

	//g.ScaleTransform(1, 1);
	//Invalidate();

}


// CMfcBasicView 인쇄

BOOL CMfcBasicView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMfcBasicView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMfcBasicView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMfcBasicView 진단

#ifdef _DEBUG
void CMfcBasicView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcBasicView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcBasicDoc* CMfcBasicView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcBasicDoc)));
	return (CMfcBasicDoc*)m_pDocument;
}
#endif //_DEBUG


// CMfcBasicView 메시지 처리기



//void CMfcBasicView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
//	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	if ((nFlags & MK_CONTROL) != MK_CONTROL)
//		CView::OnMouseHWheel(nFlags, zDelta, pt);
//
//	if (zDelta < 0) {
//		m_nZoomRate += 10;
//		if (m_nZoomRate > 300) m_nZoomRate = 300;
//	}
//	else {
//		m_nZoomRate -= 10;
//		if (m_nZoomRate < 1) m_nZoomRate = 1;
//	}
//	//RedrawWindow();	
//	Invalidate();
//
//	CView::OnMouseHWheel(nFlags, zDelta, pt);
//}


BOOL CMfcBasicView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((nFlags & MK_CONTROL) == MK_CONTROL) {

		//if ((nFlags & MK_CONTROL) != MK_CONTROL)
		//CView::OnMouseHWheel(nFlags, zDelta, pt);

	if (zDelta < 0) {
		m_nZoomRate -= 5;
		if (m_nZoomRate > 300) m_nZoomRate = 300;
	}
	else {
		m_nZoomRate += 5;
		if (m_nZoomRate < 1) m_nZoomRate = 1;
	}

	/*CPaintDC dc(this);


	dc.SetMapMode(MM_ISOTROPIC);
	dc.SetWindowExt(100, 100);
	dc.SetWindowOrg((long)&pt.x, (long)&pt.y);
	dc.SetViewportExt(200, 200);
	dc.SetViewportOrg(0, 0);*/

	RedrawWindow();	
	//Invalidate();

	}
	//CView::OnMouseHWheel(nFlags, zDelta, pt);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


//void CMfcBasicView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
//					   // 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.
//
//
//
//}


void CMfcBasicView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	m_nZoomRate = 100;
	pt = CPoint(0, 0);



	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
