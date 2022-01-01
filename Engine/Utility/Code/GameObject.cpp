#include "GameObject.h"

//#include "MFC_Calculator.h"
//#include "MFC_TerrainTex.h"
//#include "Export_Function.h"
//#include "Calculator.h"
//#include "Optimization.h"
//#include "Collider.h"
//#include "Renderer.h"
//#include "Transform.h"
//#include "StaticMesh.h"
//#include "ProtoMgr.h"
//#include "TerrainTex.h"
USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{

}

HRESULT Engine::CGameObject::Ready_Object(void)
{
	return S_OK;
}

Engine::_int Engine::CGameObject::Update_Object(const _float& fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
	{
		iter.second->Update_Component(fTimeDelta);
	}

	return 0;
}

void Engine::CGameObject::Render_Object(void)
{

}

void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}

//HRESULT CGameObject::Add_Component(void)
//{
//	CComponent*			pComponent = nullptr;
//
//	//// StaticMesh
//	//pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Stone"));
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);
//	//
//	//// Transform
//	//pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
//	//
//	//// renderer
//	//pComponent = m_pRendererCom = Engine::Get_Renderer();
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//pComponent->AddRef();
//	//m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);
//	//
//	//// Calculator
//	//pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);
//	//
//	//// Collider
//	//pComponent = m_pColliderCom = CCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);
//	//
//	//// Optimization
//	//pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(Engine::Clone_Proto(L"Proto_Optimization"));
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_STATIC].emplace(L"Com_Optimization", pComponent);
//	//
//	//return S_OK;
//	//// StaticMesh
//	//pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(CProtoMgr::GetInstance()->Clone_Proto(L"Proto_Mesh_Stone"));
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);
//	//
//	//// Transform
//	//pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CProtoMgr::GetInstance()->Clone_Proto(L"Proto_Transform"));
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
//	//
//	//// renderer
//	//pComponent = m_pRendererCom = CRenderer::GetInstance();
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//pComponent->AddRef();
//	//m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);
//	//
//	//// Calculator
//	//pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(CProtoMgr::GetInstance()->Clone_Proto(L"Proto_Calculator"));
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);
//	//
//	//// Collider
//	//pComponent = m_pColliderCom = CCollider::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);
//	//
//	//// Optimization
//	//pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(CProtoMgr::GetInstance()->Clone_Proto(L"Proto_Optimization"));
//	//NULL_CHECK_RETURN(pComponent, E_FAIL);
//	//m_mapComponent[ID_STATIC].emplace(L"Com_Optimization", pComponent);
//	//
//	return S_OK;
//}

//void CGameObject::SetUp_OnTerrain(void)
//{
//	//_vec3	vPos;
//	//m_pTransformCom->Get_Info(INFO_POS, &vPos);
//	//
//	//CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_DYNAMIC));
//	//NULL_CHECK(pTerrainBufferCom);
//	//
//	//const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();
//	//
//	//
//	//_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);
//	//
//	//m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
//}

//_bool CGameObject::Collision_ToObject(const _tchar * pLayerTag, const OBJECT_ID pObjTag)
//{
//	//CCollider*		pPlayerColliderCom = dynamic_cast<CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
//	//NULL_CHECK_RETURN(pPlayerColliderCom, false);
//	//
//	///*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
//	//										m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/
//	//
//	//return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
//	//	m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());
//}

void CGameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vCamPos;

	memcpy(&vCamPos, &matView._41, sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

CComponent* Engine::CGameObject::Find_Component(const _tchar* pComponentTag, COMPONENTID eID)
{

	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

CComponent* Engine::CGameObject::Get_Component(const _tchar* pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

