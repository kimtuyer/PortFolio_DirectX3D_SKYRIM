#include "stdafx.h"
#include "Stone.h"

#include "Export_Function.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto)
	: CGameObject(pGraphicDev)
{
	Ready_Static_Object(Mesh_proto);
}

CStone::CStone(const CStone& rhs)
	: CGameObject(rhs)
{

}

CStone::~CStone(void)
{

}

HRESULT CStone::Ready_Static_Object(wstring Mesh_proto)
{
	FAILED_CHECK_RETURN(Add_Component(Mesh_proto), E_FAIL);
	m_iHP = 1;
	return S_OK;
}

HRESULT CStone::Ready_Object(void)
{
	//FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));

	return S_OK;
}

Engine::_int CStone::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	if (m_id == OBJECT_WALL)
		m_pMeshCom = m_pMeshLevelCom;

	//SetUp_OnTerrain();
	//m_pTransformCom->m_vInfo[INFO::INFO_POS].y = 13;


	m_pColliderCom->Set_constMatrix(m_pTransformCom->Get_WorldMatrix());
	//m_pColliderCom->Set_Center(m_pTransformCom->m_vInfo[INFO::INFO_POS]);


	//m_bColl = Collision_ToObject(L"GameLogic",OBJECT_Collider);
	//if (m_bColl)
	//{
	//	--m_iHP;
	//	if(m_iHP==0)
	//	return -1;
	//}



	//_vec3	vPos;
	//m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//
	//m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CStone::Render_Object(void)
{
	//if (false == m_bDraw)
	//	return;		
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pMeshCom->Render_Meshes();
	
	//m_pColliderCom->Render_ColliderSphere(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

HRESULT CStone::Add_Component(wstring Mesh_proto)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(Mesh_proto.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);


	pComponent = m_pMeshLevelCom= dynamic_cast<CStaticMesh*>(Clone_Proto(L"Proto_Mesh_leverdown"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//m_pMeshLevelCom

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(Clone_Proto(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Optimization", pComponent);
	
	return S_OK;

}

void CStone::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_bool CStone::Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag)
{
	CColliderSphere*		pPlayerColliderCom = dynamic_cast<CColliderSphere*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	return m_pCalculatorCom->Collision_Sphere(pPlayerColliderCom->Get_Radius(), pPlayerColliderCom->Get_Center(),
		pPlayerColliderCom->Get_CollWorldMatrix(), 0.01, m_pColliderCom->Get_Radius(),&m_pTransformCom->m_vInfo[INFO::INFO_POS], m_pColliderCom->Get_CollWorldMatrix(), 1);

}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto)
{
	CStone*	pInstance = new CStone(pGraphicDev, Mesh_proto);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStone::Free(void)
{
	CGameObject::Free();
}

