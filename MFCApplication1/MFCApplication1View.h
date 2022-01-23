
// MFCApplication1View.h: CMFCApplication1View 클래스의 인터페이스
//

#pragma once
#include "Export_Function.h"
#include "Engine_Define.h"




class CMFCApplication1Doc;

BEGIN(Engine)

class CMFC_ColliderSphere;

END

BEGIN(Engine)

class CMFC_Calculator;

END

BEGIN(Engine)

class CGraphicDev;
END

BEGIN(Engine)

class CLine;


END

BEGIN(Engine)

class CCell;


END


BEGIN(Engine)

class CNaviMesh;


END


//BEGIN(Engine)
//
//typedef struct _tagCellInfo
//{
//	_vec3 vPoint[3];
//	int CellIndex = 0;
//
//	_tagCellInfo() {};
//
//	_tagCellInfo(_vec3 A, _vec3 B, _vec3 C, int index)
//	{
//		vPoint[0] = A;
//		vPoint[1] = B;
//		vPoint[2] = C;
//		CellIndex = index;
//
//	};
//
//}CellInfo;
//
//
//END


BEGIN(Engine)

typedef struct _tagObjectnfo
{
	int ObjectType;
	int Object_ID;
	_vec3 vPos[3];

	_tagObjectnfo() {};

	_tagObjectnfo(_vec3 A, _vec3 B, _vec3 C, int type,int ID)
	{
		vPos[0] = A;
		vPos[1] = B;
		vPos[2] = C;
		ObjectType = type;
		Object_ID = ID;

	};

}Objectnfo;


END









class CMFCApplication1View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCApplication1View();
	DECLARE_DYNCREATE(CMFCApplication1View)

// 특성입니다.
public:
	CMFCApplication1Doc* GetDocument() const;
	//CGraphic_Device* m_pGraphic_Device;
	Engine::CGraphicDev* m_pGraphic_Device;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;

	CManagement*			m_pManagementClass = nullptr;
	CMFC_Calculator*			m_pCalculatorCom = nullptr;
	CMFC_ColliderSphere*		m_pColliderSphereCom = nullptr;


	//vector<CMFC_ColliderSphere*> m_vecSphere;


	int m_iCellindex = 0;
	vector<_vec3> m_vecMousePos;
	_vec3 m_vMousePos[CLine::POINT_END];

	vector<Engine::CellInfo*> m_vecPoint;
	//vector<Engine::CellInfo*> m_vecWayPoint;
	vector<int> m_vecWayPointIndex;
	vector<Engine::WayPointInfo>m_vecWayPoint;
	int m_iWayPointIndex = 0;



	CLine* m_pLine=nullptr;
	CCell* m_pCell=nullptr;
	CNaviMesh* m_pNaviMesh=nullptr;


	_ulong			m_dwRenderCnt = 0;
	_tchar			m_szFPS[256];
	_float			m_fTime = 0.f;





public:
	int m_XCount, m_ZCount=0;




// 작업입니다.
public:
	HRESULT					Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagementClass);
	 _int Update_Scene(const _float& fTimeDelta) ;

	 _vec3					PickUp_OnTerrain(void);







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
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButton_TerrainTex();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCApplication1View.cpp의 디버그 버전
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

