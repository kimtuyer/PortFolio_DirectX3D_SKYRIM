
// MFCApplication1View.cpp: CMFCApplication1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif



#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"
#include "MainFrm.h"
#include "GraphicDev.h"
#include "CForm.h"


#include "MFC_Calculator.h"
#include "MFC_TerrainTex.h"
#include "MFC_Terrain.h"
#include "Scene1.h"


#include "MFC_ColliderSphere.h"
#include "ColliderSphere.h"
#include "Monster_draugr.h"
#include "CStatic_Object.h"

#include "Stone.h"
#include "Player.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCToolView
HWND g_hWnd;

// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1View::OnBnClickedButton_TerrainTex)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()


// CMFCApplication1View 생성/소멸

CMFCApplication1View::CMFCApplication1View() 
	//: m_pGraphic_Device(CGraphicDev::GetInstance())
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_XCount = 129;
	m_ZCount = 129;


	//_CrtSetBreakAlloc(22294);

}

CMFCApplication1View::~CMFCApplication1View()
{

	//m_pGraphic_Device->DestroyInstance();
	//m_pNaviMesh->Release();

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pManagementClass);



	Engine::Release_Utility();
	Engine::Release_System();

	
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 그리기

HRESULT CMFCApplication1View::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagementClass)
{
	CScene*		pScene = nullptr;

	if(FAILED(Create_Management(ppManagementClass), E_FAIL))
		return E_FAIL;

	(*ppManagementClass)->AddRef();

	pScene = Scene1::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	if (FAILED((*ppManagementClass)->Set_Scene(pScene)))
		return E_FAIL;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	CComponent*			pComponent = nullptr;
	pComponent = m_pCalculatorCom = dynamic_cast<CMFC_Calculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);



	return S_OK;
}

_int CMFCApplication1View::Update_Scene(const _float & fTimeDelta)
{
	m_fTime += fTimeDelta;
	Update_InputDev();
	m_pManagementClass->Update_Scene(fTimeDelta);


	


	



	//int Tbtn = TerrainBtn;
	//
	//switch (Tbtn)
	//{
	//case TBtn_Vertex:
	//	break;
	//case TBtn_Transform:
	//	break;
	//case TBtn_Texture:
	//	break;
	//case TBtn_End:
	//	break;
	//default:
	//	break;
	//}


	Invalidate(false);

	return _int();
}

_vec3 CMFCApplication1View::PickUp_OnTerrain(void)
{
	MFC_TerrainTex*		pTerrainBufferCom = dynamic_cast<MFC_TerrainTex*>(m_pManagementClass->Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	//CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_DYNAMIC));
	//NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>(m_pManagementClass->Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Transform", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

void CMFCApplication1View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	++m_dwRenderCnt;
	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	Render_Font(L"Font_Jinji", m_szFPS, &_vec2(400.f, 20.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));


	m_pGraphic_Device->Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
	m_pManagementClass->Render_Scene(m_pGraphicDev);

	///_matrix SphereMatrix;
	///D3DXMatrixIdentity(&SphereMatrix);
	////if (!m_vecSphere.empty())
	////	for (auto &p : m_vecSphere)
	////		p->Render_MFC_ColliderSphere(COLLTYPE(false), &SphereMatrix);
   	//

	



	m_pNaviMesh->Render_NaviMesh();
	
	//m_pColliderSphereCom->Render_ColliderSphere(COLLTYPE(false), &SphereMatrix);


	



	m_pGraphic_Device->Render_End();
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFCApplication1View 인쇄

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCApplication1View 진단

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 메시지 처리기


void CMFCApplication1View::OnInitialUpdate()
{

	CView::OnInitialUpdate();

	//SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILECY >> 1) * TILEY));
	g_hWnd = m_hWnd;
	//GetScrollPos(); 
	// 창의 크기를 조절할껀데.. 창의 크기를 조절하려면 ? MainFrame 이 아이를 얻어와야 해요. 
	// 그것도 고민을 해봐야겠네요 ? 
	// CMainFrame, CView, CDocument 
	// CWinApp 
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	RECT rcMain{};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);
	// 	rcMain.right = rcMain.right - rcMain.left; 
	// 	rcMain.left = 0; 
	// 	rcMain.bottom = rcMain.bottom - rcMain.top; 
	// 	rcMain.top = 0;
	RECT rcView{};
	GetClientRect(&rcView);
	int iGapX = rcMain.right - rcView.right; //1440 - 1420 = 20
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iGapX + 1, WINCY + iGapY + 1, SWP_NOMOVE);



	//FAILED_CHECK_RETURN(m_pGraphic_DeviceReady_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pGraphic_Device), E_FAIL);


	if (FAILED(m_pGraphic_Device->GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pGraphic_Device)))
	{
		ERR_MSG(L"Ready_Graphic_Device");
		return;

	}
	m_pGraphic_Device->AddRef();

	(m_pGraphicDev) = m_pGraphic_Device->Get_Device();
	(m_pGraphicDev)->AddRef();


	HINSTANCE hInst = AfxGetInstanceHandle();

	if (FAILED(Ready_InputDev(hInst, g_hWnd)))
		ERR_MSG(L"Ready_InputDev");


	m_pNaviMesh = CNaviMesh::MFC_Create(m_pGraphicDev);


	//if(	FAILED(Ready_Proto(L"Proto_Mesh_Navi", CNaviMesh::Create(m_pGraphicDev))))
	//	ERR_MSG(L"Proto_Mesh_Navi");
	if(FAILED(Ready_Font((m_pGraphicDev), L"Font_Default", L"바탕", 15, 20, FW_NORMAL)))
		ERR_MSG(L"Font_Default");


	if(FAILED(Ready_Font((m_pGraphicDev), L"Font_Jinji", L"궁서", 30, 30, FW_HEAVY)))
		ERR_MSG(L"Font_Jinji");








	if (FAILED(Ready_Proto(L"Proto_Calculator", CMFC_Calculator::Create(m_pGraphicDev))))
		ERR_MSG(L"Proto_Calculator");


	if (FAILED(Ready_Proto(L"Proto_Optimization", COptimization::Create(m_pGraphicDev, true, VTXCNTX, VTXCNTZ))))
		ERR_MSG(L"Proto_Optimization");


	// 버퍼
	//MFC_TerrainTex* pTerTex= MFC_TerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV);
	//CTerrainTex* pTerrainTex= CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV);
	//CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV);

	//if (FAILED(Engine::Ready_Proto(L"Proto_Buffer_CTerrainTex", CTerrainTex::Create(m_pGraphicDev, m_XCount, m_ZCount, VTXITV))))
	//	ERR_MSG(L"Proto_Buffer_CTerrainTex");


	if (FAILED(Engine::Ready_Proto(L"Proto_Buffer_TerrainTex", MFC_TerrainTex::Create(m_pGraphicDev, m_XCount, m_ZCount, VTXITV))))
		ERR_MSG(L"Proto_Buffer_TerrainTex");

	//if(FAILED(Ready_Proto(L"Proto_Buffer_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL))
	//	ERR_MSG(L"Proto_Buffer_CubeTex");


	//CGameObject*			pGameObject = nullptr;
	// Terrain
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
	// 텍스쳐
	if (FAILED(Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, L"../Resource/Texture/Terrain/%d.dds", TYPE_NORMAL, 3)))) //wrfieldHighgrass01.dds, wrfieldgrass0%d.dds //wrbasedirt01 //GrassRock_0.tga
		ERR_MSG(L"Proto_Texture_Terrain");

	//if (FAILED(Ready_Proto(L"Proto_Texture_Terrain2", CTexture::Create(m_pGraphicDev, L"../Resource/Texture/Terrain/%d.dds", TYPE_NORMAL, 1)))) //wrfieldHighgrass01.dds, wrfieldgrass0%d.dds //wrbasedirt01 //GrassRock_0.tga
	//	ERR_MSG(L"Proto_Texture_Terrain");
	//
	//if (FAILED(Ready_Proto(L"Proto_Texture_Terrain3", CTexture::Create(m_pGraphicDev, L"../Resource/Texture/Terrain/GrassRock_0.dds", TYPE_NORMAL, 1)))) //wrfieldHighgrass01.dds, wrfieldgrass0%d.dds //wrbasedirt01 //GrassRock_0.tga
	//	ERR_MSG(L"Proto_Texture_Terrain");


	if (FAILED(Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev))))
		ERR_MSG(L"Proto_Transform");

	




	if (FAILED(Ready_Proto(L"Proto_Mesh_Stone", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/bridge/", L"DragonBridge.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Draugr", CDynamicMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/DynamicMesh/Giant/", L"Giant.X"))))
		ERR_MSG(L"Proto_Mesh_Draugr");




	if (FAILED(Ready_Scene(m_pGraphicDev, &m_pManagementClass)))
		ERR_MSG(L"Ready_Scene");




	//  House


	if (FAILED(Ready_Proto(L"Proto_Mesh_House0", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/House/", L"House_0.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_House1", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/House/", L"House_1.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");



	if (FAILED(Ready_Proto(L"Proto_Mesh_House2", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/House/", L"House_2.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");





	//Rock


	if (FAILED(Ready_Proto(L"Proto_Mesh_Rock0", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Rock/", L"Rock_0.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Rock1", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Rock/", L"Rock_1.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Rock2", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Rock/", L"Rock_2.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Rock3", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Rock/", L"Rock_3.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Rock4", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Rock/", L"Rock_4.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");



	if (FAILED(Ready_Proto(L"Proto_Mesh_Rock5", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Rock/", L"Rock_5.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");



	//Stairs


	if (FAILED(Ready_Proto(L"Proto_Mesh_Stairs0", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Stairs/", L"Stairs_0.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Stairs1", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Stairs/", L"Stairs_1.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Stairs2", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Stairs/", L"Stairs_2.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Stairs3", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Stairs/", L"Stairs_3.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");




	if (FAILED(Ready_Proto(L"Proto_Mesh_Stairs4", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Stairs/", L"Stairs_4.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	//Tree



	if (FAILED(Ready_Proto(L"Proto_Mesh_Tree0", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Tree/", L"Tree_0.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Tree1", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Tree/", L"Tree_1.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	if (FAILED(Ready_Proto(L"Proto_Mesh_Tree2", CStaticMesh::Create(m_pGraphicDev, L"../Client/Bin/Resource/Mesh/StaticMesh/Tree/", L"Tree_2.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");

	//화이트런 Terrain


		//if (FAILED(Ready_Proto(L"wrarcadiabase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/", L"wrarcadiabase01.X"))))
		//ERR_MSG(L"Proto_Mesh_Stone");
	CStdioFile tip_file;

	CString Proto_name,str , Xname;

	//if (tip_file.Open(L"Whiterun.txt", CFile::modeRead | CFile::typeText))
	//{
	//
	//	while (tip_file.ReadString(str))
	//	{
	//		str.Replace((L"\r\n"),(L""));
	//		Proto_name += str;
	//
	//		Xname += Proto_name + '.'+'X';
	//		if (FAILED(Ready_Proto(Proto_name, CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", Xname))))
	//		{
	//			int i = 0;
	//			ERR_MSG(Proto_name);
	//
	//		}
	//
	//		Proto_name.Empty();
	//		str.Empty();
	//		Xname.Empty();
	//		
	//	}
	//
	//	tip_file.Close();
	//}
	if (FAILED(Ready_Proto(L"WhiterunMainload", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"WhiterunMainload.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");//


	if (FAILED(Ready_Proto(L"wrmainroadplains", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmainroadplains.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
		
	if (FAILED(Ready_Proto(L"wrarcadiabase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrarcadiabase01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	
	
	if (FAILED(Ready_Proto(L"wrbridgestone01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbridgestone01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	
	if (FAILED(Ready_Proto(L"wrbanneredmarebase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbanneredmarebase01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrblacksmithbase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrblacksmithbase.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrbreezebase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbreezebase.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrcarlottaplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcarlottaplatform01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");

	if (FAILED(Ready_Proto(L"wrcommonhouseplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcommonhouseplatform01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");

	if (FAILED(Ready_Proto(L"wrcastlestairs01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestairs01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	
	
	if (FAILED(Ready_Proto(L"wrcompanionbase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcompanionbase01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrdoormaingate01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrdoormaingate01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrdrunkenhuntsmanbase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrdrunkenhuntsmanbase.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrdrygoodsbase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrdrygoodsbase.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrgreathouseplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrgreathouseplatform01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrgreathouseplatform02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrgreathouseplatform02.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrgreymanebase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrgreymanebase.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrguardhouse01arch01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrguardhouse01arch01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrhallofdeadplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhallofdeadplatform01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrmainroadmarket", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmainroadmarket.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrmainroadplains02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmainroadplains02.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrpondstairs01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondstairs01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrpondstairs02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondstairs02.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrpondwallcor01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondwallcor01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrpondwallstr01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondwallstr01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrpondwallstr02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondwallstr02.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrpondwallstr03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondwallstr03.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrshacksbase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrshacksbase01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrshacksbase02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrshacksbase02.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrstairsplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrstairsplatform01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	if (FAILED(Ready_Proto(L"wrulfberhplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrulfberhplatform01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");


	//화이트런 wall
	if (FAILED(Ready_Proto(L"wrwallcap01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcap01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	
		
		if (FAILED(Ready_Proto(L"wrwallcap01nohighcol", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcap01nohighcol.X"))))
			ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallcurve30up128", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve30up128.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallcurve30up128a", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve30up128a.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallcurve45", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve45.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallcurve45up128", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve45up128.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallcurve60", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve60.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallmaingate01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallmaingate01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallmaingate01ext", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallmaingate01ext.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallstr01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallstr01nohighcoll", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01nohighcoll.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallstrrubbled01free", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrrubbled01free.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallstrrubbled01short", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrrubbled01short.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallstrrubbled02free", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrrubbled02free.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallstrup128", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrup128.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallstrup128shift128", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrup128shift128.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallstrup128shift128nohighcol", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrup128shift128nohighcol.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwalltierdivide01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltierdivide01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwalltotem01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltotem01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwalltower01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltower01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwalltowercap01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltowercap01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
			if (FAILED(Ready_Proto(L"wrwallwatergrate01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallwatergrate01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");

	if (FAILED(Ready_Proto(L"wrstairswater01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrstairswater01.X"))))
		ERR_MSG(L"Proto_Mesh_Stone");
	
	
		if (FAILED(Ready_Proto(L"wrwalltowerfree01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltowerfree01.X"))))
			ERR_MSG(L"Proto_Mesh_Stone");
		
		if (FAILED(Ready_Proto(L"wrwalltower01collpiece", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltower01collpiece.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
		if (FAILED(Ready_Proto(L"wrguardhouse01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrguardhouse01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
		if (FAILED(Ready_Proto(L"wrbridge02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbridge02.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
		if (FAILED(Ready_Proto(L"wrbridge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbridge01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");

		if (FAILED(Ready_Proto(L"wrtreecircle01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtreecircle01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
		
		if (FAILED(Ready_Proto(L"wrterwind01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrterwind01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
			
		if (FAILED(Ready_Proto(L"wrtercloud01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtercloud01.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
		if (FAILED(Ready_Proto(L"wrtercloud02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtercloud02.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
			
		if (FAILED(Ready_Proto(L"wrtercloud03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtercloud03.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
		

		if (FAILED(Ready_Proto(L"sovmeadhall01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/Sovengarde/", L"sovmeadhall01.X"))))
			ERR_MSG(L"Proto_Mesh_Stone");

	
		
			if (FAILED(Ready_Proto(L"wrcastlewoodtower01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlewoodtower01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
			
			if (FAILED(Ready_Proto(L"wrcastlestonetowertop01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestonetowertop01.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
			
			if (FAILED(Ready_Proto(L"wrcastlestonetower01porchint", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestonetower01porchint.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
			
			if (FAILED(Ready_Proto(L"wrcastlestonetower01porch", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestonetower01porch.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
			
			if (FAILED(Ready_Proto(L"wrcastlestonetower01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestonetower01.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
			
			if (FAILED(Ready_Proto(L"wrcastlemainbuilding01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlemainbuilding01.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
			
			if (FAILED(Ready_Proto(L"wrcastleentrance01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastleentrance01.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");
			
			if (FAILED(Ready_Proto(L"wrcastlebridge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlebridge01.X"))))
					ERR_MSG(L"Proto_Mesh_Stone");




			
			if (FAILED(Ready_Proto(L"wrskyforge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrskyforge01.X"))))
				ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrskyforge01door01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrskyforge01door01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrwallstr01stockades01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01stockades01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					
				
					if (FAILED(Ready_Proto(L"wrwallstr01stockades02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01stockades02.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrwallstr01stockades03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01stockades03.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrguardtowerentrance01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrguardtowerentrance01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrwallruinsfree01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallruinsfree01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrwallruinsfree01entance01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallruinsfree01entance01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrwalltowerchunk01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltowerchunk01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrwalltowerrubbled01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltowerrubbled01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					
					if (FAILED(Ready_Proto(L"wrscafguardtower01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrscafguardtower01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrscafguardtowerbase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrscafguardtowerbase01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrscafguardtowerlarge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrscafguardtowerlarge01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrscafguardtowerspikes01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrscafguardtowerspikes01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");
				
					if (FAILED(Ready_Proto(L"wrwoodbridge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwoodbridge01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");

					if (FAILED(Ready_Proto(L"rockcliff04", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"rockcliff04.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");

					if (FAILED(Ready_Proto(L"wrtempleofk01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtempleofk01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");

					if (FAILED(Ready_Proto(L"wrhallofthedead01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhallofthedead01.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");

					if (FAILED(Ready_Proto(L"rockcliff07", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"rockcliff07.X"))))
						ERR_MSG(L"Proto_Mesh_Stone");




						
							if (FAILED(Ready_Proto(L"wrbreezehome01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbreezehome01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhouse02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhouse02.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousebanneredmare01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousebanneredmare01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhouseblacksmith01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhouseblacksmith01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousegreymane01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousegreymane01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousemeadery01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousemeadery01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousemeaderywarehouse01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousemeaderywarehouse01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
					
							if (FAILED(Ready_Proto(L"wrhouseshack01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhouseshack01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhouseshack02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhouseshack02.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousestables01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousestables01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousestores01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousestores01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousestores02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousestores02.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousewind01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousewind01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousewind02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousewind02.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousewind03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousewind03.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrhousewind04", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousewind04.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrjorvaskr01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrjorvaskr01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrmarketstand01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmarketstand01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrmarketstand02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmarketstand02.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");

					
							if (FAILED(Ready_Proto(L"wrmarketstand03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmarketstand03.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrtrellisarch01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtrellisarch01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
					
							if (FAILED(Ready_Proto(L"wrtrelliscolumn01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtrelliscolumn01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"wrtrelliscomplete01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtrelliscomplete01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"mountainpeak01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountainpeak01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
					
							if (FAILED(Ready_Proto(L"mountainpeak02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountainpeak02.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"mountaincliff01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliff01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"mountaincliff02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliff02.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"mountaincliff03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliff03.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
						
							if (FAILED(Ready_Proto(L"mountaincliff04", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliff04.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
					
							if (FAILED(Ready_Proto(L"mountaincliffslope", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliffslope.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");

							if (FAILED(Ready_Proto(L"wrjorvaskr01backporch01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrjorvaskr01backporch01.X"))))
								ERR_MSG(L"Proto_Mesh_Stone");
					
	

	//pGameObject = MFC_Terrain::Create(m_pGraphicDev);
	//if(FAILED(Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, L"../Client/Bin/Resource/Texture/Terrain/Grass_%d.tga", TYPE_NORMAL, 2)), E_FAIL))
	//	ERR_MSG(L"Proto_Texture_Terrain");

	//if(FAILED(Ready_Proto(L"Proto_Texture_Terrain2", CTexture::Create(m_pGraphicDev, L"../Client/Bin/Resource/Texture/Terrain/Terrain0.png", TYPE_NORMAL, 1)), E_FAIL))
	//	ERR_MSG(L"Proto_Texture_Terrain2");

	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TYPE_CUBE, 4)), E_FAIL);



	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CMFCApplication1View::OnBnClickedButton_TerrainTex()
{
	CString str = _T("");
	GetDlgItemText(IDC_EDIT2, str);
	m_XCount = _ttoi(str);

	CString str2 = _T("");
	GetDlgItemText(IDC_EDIT3, str2);
	m_ZCount = _ttoi(str2);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCApplication1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	CForm* pForm2 = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));


	_vec3	vPos = PickUp_OnTerrain();
	m_vecMousePos.push_back(vPos);


	//CMFC_ColliderSphere* pSphere = CMFC_ColliderSphere::Create(m_pGraphicDev, &vPos, 100, 100);
	//pSphere->Set_Pos(vPos);
	//m_vecSphere.emplace_back(pSphere);


	if (m_iCellindex != 0) //0번 이후 1번째 삼각형
	{
		int num = pForm2->m_pNaviTab.m_iRadio;
		if (pForm2->m_pNaviTab.m_Radio_WP.GetCheck() == BST_CHECKED) //Way_Point선택
		{


			_vec3 A = m_vecMousePos.front() - m_vecPoint[0]->vPoint[CCell::POINT_A];
			_vec3 B = m_vecMousePos.front() - m_vecPoint[0]->vPoint[CCell::POINT_B];
			_vec3 C = m_vecMousePos.front() - m_vecPoint[0]->vPoint[CCell::POINT_C];
			float MinDist= D3DXVec3Length(&A) + D3DXVec3Length(&B) + D3DXVec3Length(&C);
			int		iIndex = 0;
			for (int i = 0; i < m_vecPoint.size(); i++)
			{

				_vec3 A = m_vecMousePos.front() - m_vecPoint[i]->vPoint[CCell::POINT_A];
				_vec3 B = m_vecMousePos.front() - m_vecPoint[i]->vPoint[CCell::POINT_B];
				_vec3 C = m_vecMousePos.front() - m_vecPoint[i]->vPoint[CCell::POINT_C];

				float Dist_A = D3DXVec3Length(&A)+ D3DXVec3Length(&B)+ D3DXVec3Length(&C);
				if (i > 0 && MinDist > Dist_A)
				{
					MinDist = Dist_A;
					iIndex = i;
				}

			}
			m_pCell = m_pNaviMesh->Get_Cell(iIndex);

			A = m_pCell->MFC_Get_Point(CCell::POINT_A);
			B = m_pCell->MFC_Get_Point(CCell::POINT_B);
			C = m_pCell->MFC_Get_Point(CCell::POINT_C);

			_vec3 MidPoint = { A.x + B.x + C.x / 3  , A.y + B.y + C.y / 3  , A.z + B.z + C.z / 3 };

			m_vecWayPoint.push_back(WayPointInfo(MidPoint, m_iWayPointIndex++, iIndex));		

			//pView->m_pNaviMesh->Insert_WayPointSell(m_pCell);
			pView->m_pNaviMesh->Get_Cell(iIndex)->Set_color({ 255,0,255,1 });
			//m_vecWayPointIndex.push_back(iIndex);

		
			//찾은 인덱스값 트리에 넘기고, 해당 셀 색깔 표시
			CString b;
			b.Format(_T("%d"), iIndex);

			HTREEITEM hRoot2;
			hRoot2 = pForm2->m_pNaviTab.m_ctrl_WayPointTree.InsertItem(b, TVI_ROOT, TVI_LAST);


			//HTREEITEM hChild1;
			//hChild1 = pForm2->m_pNaviTab.m_ctrl_WayPointTree.InsertItem(L"0", hRoot2, TVI_LAST);
			////HTREEITEM hStatic;
			//hChild1 = pForm2->m_pNaviTab.m_ctrl_WayPointTree.InsertItem(L"1", hRoot2, TVI_LAST);
			//hChild1 = pForm2->m_pNaviTab.m_ctrl_WayPointTree.InsertItem(L"2", hRoot2, TVI_LAST);




		}
		
		else if (pForm2->m_pNaviTab.m_Radio1.GetCheck() == BST_CHECKED) //Cell_Point

		{





			m_pCell = m_pNaviMesh->Get_Cell(m_iCellindex - 1);

			_vec3 CellPos = m_pCell->MFC_Get_Point(CCell::POINT_A);

			_vec3 A = m_vecMousePos.front() - m_pCell->MFC_Get_Point(CCell::POINT_A);
			_vec3 B = m_vecMousePos.front() - m_pCell->MFC_Get_Point(CCell::POINT_B);
			_vec3 C = m_vecMousePos.front() - m_pCell->MFC_Get_Point(CCell::POINT_C);

			float Dist_A = D3DXVec3Length(&A);
			float Dist_B = D3DXVec3Length(&B);
			float Dist_C = D3DXVec3Length(&C);

			pair<float, float> mm = minmax({ Dist_A ,Dist_B ,Dist_C });

			if (mm.second == Dist_A)
			{

				float mm = min(Dist_B, Dist_C);

				m_pCell = CCell::Create(m_pGraphicDev, m_iCellindex, m_pCell->Get_Point(CCell::POINT_B), &m_vecMousePos.front(), m_pCell->Get_Point(CCell::POINT_C));
				m_pNaviMesh->Insert_Sell(m_pCell);

				m_vecPoint.emplace_back(new CellInfo(m_pCell->MFC_Get_Point(CCell::POINT_A), m_pCell->MFC_Get_Point(CCell::POINT_B),
					m_pCell->MFC_Get_Point(CCell::POINT_C), m_iCellindex));


				//else
				//{
				//	m_pCell = CCell::Create(m_pGraphicDev, m_iCellindex, m_pCell->Get_Point(CCell::POINT_C), &m_vecMousePos.front(), m_pCell->Get_Point(CCell::POINT_B));
				//	m_pNaviMesh->Insert_Sell(m_pCell);
				//
				//	//CellInfo CInfo;
				//	//CInfo.vPoint[CLine::POINT_START] = m_pCell->MFC_Get_Point(CCell::POINT_C);
				//	//CInfo.vPoint[CLine::POINT_FINISH] = m_vecMousePos.front();
				//	//CInfo.vPoint[CLine::POINT_END] = m_pCell->MFC_Get_Point(CCell::POINT_B);
				//	//CInfo.CellIndex = m_iCellindex;
				//	//
				//	//m_vecPoint.push_back(&CInfo);
				//	m_vecPoint.emplace_back(new CellInfo(m_pCell->MFC_Get_Point(CCell::POINT_A), m_pCell->MFC_Get_Point(CCell::POINT_B),
				//		m_pCell->MFC_Get_Point(CCell::POINT_C), m_iCellindex));
				//
				//}


			}

			else if (mm.second == Dist_B)
			{
				float mm = min(Dist_A, Dist_C);

				//if (mm == Dist_A)
				//{
				//	m_pCell = CCell::Create(m_pGraphicDev, m_iCellindex, m_pCell->Get_Point(CCell::POINT_A), &m_vecMousePos.front(), m_pCell->Get_Point(CCell::POINT_C));
				//	m_pNaviMesh->Insert_Sell(m_pCell);
				//
				//	//CellInfo CInfo;
				//	//CInfo.vPoint[CLine::POINT_START] = m_pCell->MFC_Get_Point(CCell::POINT_A);
				//	//CInfo.vPoint[CLine::POINT_FINISH] = m_vecMousePos.front();
				//	//CInfo.vPoint[CLine::POINT_END] = m_pCell->MFC_Get_Point(CCell::POINT_C);
				//	//CInfo.CellIndex = m_iCellindex;
				//	//
				//	//m_vecPoint.push_back(&CInfo);
				//	m_vecPoint.emplace_back(new CellInfo(m_pCell->MFC_Get_Point(CCell::POINT_A), m_pCell->MFC_Get_Point(CCell::POINT_B),
				//		m_pCell->MFC_Get_Point(CCell::POINT_C), m_iCellindex));
				//
				//}
				//else
				{
					m_pCell = CCell::Create(m_pGraphicDev, m_iCellindex, m_pCell->Get_Point(CCell::POINT_C), &m_vecMousePos.front(), m_pCell->Get_Point(CCell::POINT_A));
					m_pNaviMesh->Insert_Sell(m_pCell);

					//CellInfo CInfo;
					//CInfo.vPoint[CLine::POINT_START] = m_pCell->MFC_Get_Point(CCell::POINT_C);
					//CInfo.vPoint[CLine::POINT_FINISH] = m_vecMousePos.front();
					//CInfo.vPoint[CLine::POINT_END] = m_pCell->MFC_Get_Point(CCell::POINT_A);
					//CInfo.CellIndex = m_iCellindex;
					//
					//m_vecPoint.push_back(&CInfo);
					m_vecPoint.emplace_back(new CellInfo(m_pCell->MFC_Get_Point(CCell::POINT_A), m_pCell->MFC_Get_Point(CCell::POINT_B),
						m_pCell->MFC_Get_Point(CCell::POINT_C), m_iCellindex));
				}

			}
			else if (mm.second == Dist_C)
			{
				float mm = min(Dist_A, Dist_B);

				//if (mm == Dist_A)
				{
					m_pCell = CCell::Create(m_pGraphicDev, m_iCellindex, m_pCell->Get_Point(CCell::POINT_A), &m_vecMousePos.front(), m_pCell->Get_Point(CCell::POINT_B));
					m_pNaviMesh->Insert_Sell(m_pCell);

					//CellInfo CInfo;
					//CInfo.vPoint[CLine::POINT_START] = m_pCell->MFC_Get_Point(CCell::POINT_A);
					//CInfo.vPoint[CLine::POINT_FINISH] = m_vecMousePos.front();
					//CInfo.vPoint[CLine::POINT_END] = m_pCell->MFC_Get_Point(CCell::POINT_B);
					//CInfo.CellIndex = m_iCellindex;
					//
					//m_vecPoint.push_back(&CInfo);
					m_vecPoint.emplace_back(new CellInfo(m_pCell->MFC_Get_Point(CCell::POINT_A), m_pCell->MFC_Get_Point(CCell::POINT_B),
						m_pCell->MFC_Get_Point(CCell::POINT_C), m_iCellindex));
				}
				//else
				//{
				//	m_pCell = CCell::Create(m_pGraphicDev, m_iCellindex, m_pCell->Get_Point(CCell::POINT_B), &m_vecMousePos.front(), m_pCell->Get_Point(CCell::POINT_A));
				//	m_pNaviMesh->Insert_Sell(m_pCell);
				//
				//	//CellInfo CInfo;
				//	//CInfo.vPoint[CLine::POINT_START] = m_pCell->MFC_Get_Point(CCell::POINT_B);
				//	//CInfo.vPoint[CLine::POINT_FINISH] = m_vecMousePos.front();
				//	//CInfo.vPoint[CLine::POINT_END] = m_pCell->MFC_Get_Point(CCell::POINT_A);
				//	//CInfo.CellIndex = m_iCellindex;
				//	//
				//	//m_vecPoint.push_back(&CInfo);
				//	m_vecPoint.emplace_back(new CellInfo(m_pCell->MFC_Get_Point(CCell::POINT_A), m_pCell->MFC_Get_Point(CCell::POINT_B),
				//		m_pCell->MFC_Get_Point(CCell::POINT_C), m_iCellindex));
				//}

			}


			CString a;
			a.Format(_T("%d"), m_iCellindex);

			HTREEITEM hRoot;
			hRoot = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(a, TVI_ROOT, TVI_LAST);


			HTREEITEM hChild;
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"0", hRoot, TVI_LAST);
			//HTREEITEM hStatic;
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"1", hRoot, TVI_LAST);
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"2", hRoot, TVI_LAST);

			m_iCellindex++;


		}

		m_vecMousePos.clear();



	}
	else //0번째 삼각형
	{
		if (m_vecMousePos.size() >= 3)
		{

			m_pCell = CCell::Create(m_pGraphicDev, m_iCellindex, &m_vecMousePos[0], &m_vecMousePos[1], &m_vecMousePos[2]);
			m_pNaviMesh->Insert_Sell(m_pCell);

			//CellInfo* CInfo=new CellInfo;
			//CInfo->vPoint[CLine::POINT_START] = m_vecMousePos[CLine::POINT_START];
			//CInfo->vPoint[CLine::POINT_FINISH] = m_vecMousePos[CLine::POINT_FINISH];
			//CInfo->vPoint[CLine::POINT_END] = m_vecMousePos[CLine::POINT_END];
			//CInfo->CellIndex = m_iCellindex;

			m_vecPoint.emplace_back(new CellInfo(m_vecMousePos[CLine::POINT_START], m_vecMousePos[CLine::POINT_FINISH], m_vecMousePos[CLine::POINT_END], m_iCellindex));


			CString a;
			a.Format(_T("%d"), m_iCellindex);

			HTREEITEM hRoot;
			hRoot = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(a, TVI_ROOT, TVI_LAST);


			HTREEITEM hChild;
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"0", hRoot, TVI_LAST);
			//HTREEITEM hStatic;
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"1", hRoot, TVI_LAST);
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"2", hRoot, TVI_LAST);

			m_iCellindex++;

			m_vecMousePos.clear();
			//delete CInfo;


		}



	}
	//m_pLine = CLine::Create(m_vecMousePos[0], m_vecMousePos[1]);

	//for (auto &p : m_vecMousePos)
		//Safe_Release(p);







	
	Invalidate(FALSE);

	CView::OnLButtonDown(nFlags, point);
}


void CMFCApplication1View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//D3DXVECTOR3 vMouse{ float(point.x) + GetScrollPos(SB_HORZ), float(point.y) + GetScrollPos(SB_VERT), 0.f };

	_vec3	vPos = PickUp_OnTerrain();
	_vec3	vTemp = {};
	
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	CForm* pForm2 = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	CString str_Mesh;

	if (pForm2->m_pMeshTab.m_Radio_Object == 0) //맵 오브젝트 선택
	{
		int index = pForm2->m_pMeshTab.m_StaticList.GetCurSel();
		pForm2->m_pMeshTab.m_StaticList.GetText(index, str_Mesh);

		CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"Environment");
		list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_BRIDGE);

		list<CGameObject*>::iterator iter = pList.begin();

		{

			int i = 0;
		

			for (; iter != pList.end(); ++i)
				if (i == index)
				{
					dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Get_Info(INFO::INFO_POS, &vTemp);
					vPos.y = vTemp.y;
					dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Pos(&vPos);
					pForm2->m_pMeshTab.m_vecObject[i]->vPos = vPos;
				}
				else
					++iter;




		}

		//if (str_Mesh == "House0")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, STATIC_OBJECT_ID::House0);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::House0));
		//
		//	m_StaticList.AddString(L"House0");
		//}
		//if (str_Mesh == "House1")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, House1);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::House1));
		//
		//	m_StaticList.AddString(L"House1");
		//}
		//
		//if (str_Mesh == "House2")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, House2);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::House2));
		//
		//	m_StaticList.AddString(L"House2");
		//}
		//
		//
		//
		//if (str_Mesh == "Rock0")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock0);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock0));
		//
		//	m_StaticList.AddString(L"Rock0");
		//}
		//
		//if (str_Mesh == "Rock1")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock1);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock1));
		//
		//	m_StaticList.AddString(L"Rock1");
		//}
		//
		//if (str_Mesh == "Rock2")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock2);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock2));
		//
		//	m_StaticList.AddString(L"Rock2");
		//}
		//
		//
		//
		//if (str_Mesh == "Rock3")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock3);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock3));
		//
		//	m_StaticList.AddString(L"Rock3");
		//}
		//if (str_Mesh == "Rock4")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock4);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock4));
		//
		//	m_StaticList.AddString(L"Rock4");
		//}
		//
		//if (str_Mesh == "Rock5")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock5);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock5));
		//
		//	m_StaticList.AddString(L"Rock5");
		//}
		//
		//
		//
		//if (str_Mesh == "Staris0")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris0);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris0));
		//
		//	m_StaticList.AddString(L"Staris0");
		//}
		//
		//if (str_Mesh == "Staris1")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris1);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris1));
		//
		//	m_StaticList.AddString(L"Staris1");
		//}
		//
		//if (str_Mesh == "Staris2")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris2);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris2));
		//
		//	m_StaticList.AddString(L"Staris2");
		//}
		//
		//if (str_Mesh == "Staris3")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris3);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris3));
		//
		//	m_StaticList.AddString(L"Staris3");
		//}
		//
		//if (str_Mesh == "Staris4")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris4);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris4));
		//
		//	m_StaticList.AddString(L"Staris4");
		//}
		//
		//
		//
		//if (str_Mesh == "Tree0")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Tree0);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Tree0));
		//
		//	m_StaticList.AddString(L"Tree0");
		//}
		//
		//if (str_Mesh == "Tree1")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Tree1);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Tree1));
		//
		//	m_StaticList.AddString(L"Tree1");
		//}
		//
		//if (str_Mesh == "Tree2")
		//{
		//
		//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Tree2);
		//
		//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Tree2));
		//
		//	m_StaticList.AddString(L"Tree2");
		//}



















	}
	else
	{
		int index = pForm2->m_pMeshTab.m_DynamicList.GetCurSel();
		pForm2->m_pMeshTab.m_DynamicList.GetText(index, str_Mesh);

		if (str_Mesh == "Draugr")

		{

			int i = 0;
			CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"GameLogic");
			list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_DRAUGR);

			list<CGameObject*>::iterator iter = pList.begin();

			for (; iter != pList.end(); ++i)
				if (i == index)
					dynamic_cast<CMonster_draugr*>(*iter)->Get_Transform()->Set_Pos(&vPos);
				else
					++iter;


			//dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(OBJECT_DRAUGR);


		}

	}
	

	


	Invalidate(FALSE);

	CView::OnRButtonDown(nFlags, point);
}


void CMFCApplication1View::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	_vec3	vPos = PickUp_OnTerrain();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	CForm* pForm2 = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));



	CString cHeight;

	CString cSize;

	_ulong dwheight = 5;

	_ulong size = 5;

	pForm2->m_pTerrainTab.m_Edit_Height.GetWindowTextW(cHeight);
	pForm2->m_pTerrainTab.m_VtxSize.GetWindowTextW(cSize);

	//int size = _ttoi(cSize);
	//
	//float cHeight = _ttof(cHeight);



	CComponent* pTerrainTex=
	m_pManagementClass->Get_Component(L"GameLogic",OBJECT_ID::OBJECT_TERRAIN, L"Com_Buffer", COMPONENTID::ID_DYNAMIC);

	m_pCalculatorCom->Set_HeightOnTerrain(vPos, dwheight, size, dynamic_cast<MFC_TerrainTex*>(pTerrainTex));

	CView::OnMButtonDown(nFlags, point);
}
