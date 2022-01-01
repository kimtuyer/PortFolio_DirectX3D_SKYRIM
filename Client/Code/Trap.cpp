#include "stdafx.h"
#include "Trap.h"
#include "Breath.h"
#include "Export_Function.h"

CTrap::CTrap(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CTrap::CTrap(const CTrap& rhs)
	: CGameObject(rhs)
{

}

CTrap::~CTrap(void)
{
	//m_pBreath->Free();
}

HRESULT CTrap::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01, 0.01, 0.01);
	m_pTransformCom->Set_Pos(63, 11.5, 67.95);

//	m_pTransformCom->Set_Pos(61.8, 19.f, 65.85f);
	//m_pTransformCom->Set_Pos(61.8, 12.f, 65.85f);
	//m_pBodyCollider->Set_Radius(100);	
	//m_pHeadCollider->Set_Radius(100);
	//m_pLWingCollider->Set_Radius(100);
	//m_pRWingCollider->Set_Radius(100);
	//m_pTailCollider ->Set_Radius(100);

	m_iHP = 1;
	//m_pNaviCom->Set_CellIndex(1);
	//m_pMeshCom->Set_AnimationIndex(4);

	

	return S_OK;
}

Engine::_int CTrap::Update_Object(const _float& fTimeDelta)
{
	if (m_ID == 0)
	{
		m_pTransformCom->Set_Pos(63, 11.5, 67.95);

	}
	else //1번째 트랩은 Mesh400까지만 사용!
	{
		m_pTransformCom->Set_Pos(63, 11.f, 60.95);
		//m_pMeshCom1000 = m_pMeshCom400;
	}

	{
		CGameObject* pDragon = CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON);

		if (dynamic_cast<CDragon*>(pDragon)->Get_ActionStage() >= 32)
		{
			m_fTime += fTimeDelta;

			

			//트랩 메쉬 변경!
			
			if (m_ID == 0)	
			{
				m_pMeshCom1000 = m_pMeshCom200;
				if (m_fTime >= 1.5 && !height600)
				{

					m_pMeshCom1000 = m_pMeshCom600;
					height600 = true;
					CSoundMgr::Get_Instance()->PlaySound(L"qst_dragontrap.wav", CSoundMgr::EFFECT_TRAP);

				}
				if (m_fTime > 1.5  && m_fTime <=3.f && !height400)
				{		

					m_pMeshCom1000 = m_pMeshCom400;
					height400 = true;


				}
				if (m_fTime > 3  && m_fTime <= 4.5f && !height200)
				{

					m_pMeshCom1000 = m_pMeshCom200;
					height200 = true;


				}

					


			}
			if (m_ID == 1)
			{
				m_pMeshCom1000 = m_pMeshCom400;

				if (m_fTime >= 1.5 && !height600)
				{

					m_pMeshCom1000 = m_pMeshCom600;
					height600 = true;
					
				}
				if (m_fTime > 1.5  && m_fTime <= 3.f && !height400)
				{

					m_pMeshCom1000 = m_pMeshCom400;
					height400 = true;


				}
				if (m_fTime > 3 && m_fTime <= 4.5f && !height200)
				{

					m_pMeshCom1000 = m_pMeshCom400;

					height200 = true;
				}


			}


		
		}



		CGameObject::Update_Object(fTimeDelta);

		Update_Collider(fTimeDelta);


		//SetUp_OnTerrain();

		//NPC_Tongue2 bone 위치에서 브레스 발사!

		//충돌체 부착위치
		//NPC_LHand

		//NPC_RHand
		//>>좌우 날개공격위해 따로 충돌체 배치!

		//NPC_TailHub >>꼬리 공격 bone

		//NPC_Head_MagicNode__Hmag_ >>머리 공격 위해 충돌체 배치

		//NPC_Hub01 >>중심 잡고 몸통 충돌체 배치






		Add_RenderGroup(RENDER_NONALPHA, this);

	}
	return 0;
}

void CTrap::Render_Object(void)
{
	//m_pBodyCollider->Render_ColliderSphere(COLLTYPE(false), m_pBodyCollider->Get_CollWorldMatrix());

	//if (m_bRenderOn)
	{



		//m_pHeadCollider->Render_ColliderSphere(COLLTYPE(false), m_pHeadCollider->Get_CollWorldMatrix());
		//m_pLWingCollider->Render_ColliderSphere(COLLTYPE(false), m_pLWingCollider->Get_CollWorldMatrix());
		//m_pRWingCollider->Render_ColliderSphere(COLLTYPE(false), m_pRWingCollider->Get_CollWorldMatrix());
		//m_pTailCollider->Render_ColliderSphere(COLLTYPE(false), m_pTailCollider->Get_CollWorldMatrix());




		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	


		m_pMeshCom1000->Render_Meshes();

	}
}

_int CTrap::Update_Collider(const _float & fTimeDelta)
{

	_vec3 vDir, vPos;


	
	return _int();
}

HRESULT CTrap::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom1000 = dynamic_cast<CStaticMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Trap1000"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshCom800 = dynamic_cast<CStaticMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Trap800"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshCom600 = dynamic_cast<CStaticMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Trap600"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshCom400 = dynamic_cast<CStaticMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Trap400"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshCom200 = dynamic_cast<CStaticMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Trap200"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);


	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);


	pComponent = m_pHeadCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_HeadCollider", pComponent);

	pComponent = m_pBodyCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BodyCollider", pComponent);

	pComponent = m_pTailCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_TailCollider", pComponent);

	pComponent = m_pLWingCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_LWingCollider", pComponent);

	pComponent = m_pRWingCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_RWingCollider", pComponent);






	return S_OK;

}


void CTrap::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_vec3 CTrap::PickUp_OnTerrain(void)
{
	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

CTrap* CTrap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrap*	pInstance = new CTrap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTrap::Free(void)
{
	CGameObject::Free();
}

