#include "pch.h"

#include "CStatic_Object.h"

#include "MFC_Calculator.h"
#include "MFC_TerrainTex.h"
#include "Export_Function.h"

CStatic_Object::CStatic_Object(LPDIRECT3DDEVICE9 pGraphicDev, CString Mesh_proto) : CGameObject(pGraphicDev)
{
	Ready_CStatic_Object(Mesh_proto);
}


CStatic_Object::CStatic_Object(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{

}

CStatic_Object::CStatic_Object(const CStatic_Object& rhs)
	: CGameObject(rhs)
{

}

CStatic_Object::~CStatic_Object()
{
}

HRESULT CStatic_Object::Ready_CStatic_Object(CString Mesh_proto)
{
	
	FAILED_CHECK_RETURN(Add_Component(Mesh_proto), E_FAIL);

	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	Set_HP(1);

	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));

}

HRESULT CStatic_Object::Ready_Object(void)
{
	return S_OK;
}

_int CStatic_Object::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	//SetUp_OnTerrain();

	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	//m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;

}

void CStatic_Object::Render_Object(void)
{
	//if (false == m_bDraw)
	//	return;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pMeshCom->Render_Meshes();

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_WorldMatrix());

	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}



void CStatic_Object::Release_CStatic_Object()
{
}

HRESULT CStatic_Object::Add_Component(CString Mesh_proto)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(Mesh_proto));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);
	
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
	//pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);
	
	pComponent = m_pCalculatorCom2 = dynamic_cast<CMFC_Calculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);
	


	// Collider
	pComponent = m_pColliderCom = CCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);
	
	// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(Clone_Proto(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Optimization", pComponent);
	
	
	

	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);

	return S_OK		;
}


void CStatic_Object::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	MFC_TerrainTex*		pTerrainBufferCom = dynamic_cast<MFC_TerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_DYNAMIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom2->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);

}

_bool CStatic_Object::Collision_ToObject(const _tchar * pLayerTag, const OBJECT_ID pObjTag)
{
	CCollider*		pPlayerColliderCom = dynamic_cast<CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	return m_pCalculatorCom2->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
		m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());
}

void CStatic_Object::Change_CStatic_Object(const D3DXVECTOR3 & vPos, const BYTE & byDrawID, const BYTE & byOption)
{
}

int CStatic_Object::Get_Index(const D3DXVECTOR3 & vPos)
{
	return 0;
}

bool CStatic_Object::IsPicking(const D3DXVECTOR3 & vPos, const int iIndex)
{
	return false;
}

void CStatic_Object::Free(void)
{
}

CStatic_Object * CStatic_Object::Create(LPDIRECT3DDEVICE9 pGraphicDev, CString Mesh_proto)
{
	CStatic_Object*	pInstance = new CStatic_Object(pGraphicDev, Mesh_proto);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	
	return pInstance;

}
