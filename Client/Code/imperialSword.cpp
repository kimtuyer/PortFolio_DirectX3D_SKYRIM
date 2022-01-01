#include "stdafx.h"
#include "imperialSword.h"

#include "ColliderSphere.h"
#include "Export_Function.h"

CimperialSword::CimperialSword(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CimperialSword::CimperialSword(const CimperialSword& rhs)
	: CGameObject(rhs)
{

}

CimperialSword::~CimperialSword(void)
{

}

HRESULT CimperialSword::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(0.f));


	//m_pColliderCom->Set_RadiusScale(0.1);
	Set_HP(1);

	return S_OK;
}

Engine::_int CimperialSword::Update_Object(const _float& fTimeDelta)
{
	_vec3 vDir, vPos;

	//m_pColliderSphereCom->Set_RadiusScale(0.15);
	if (dynamic_cast<CGuardMan*>(m_pGuardMan)->Get_RenderOn())
	{




		CTransform*		pPlayerTransCom = dynamic_cast<CGuardMan*>(m_pGuardMan)->Get_Transform();
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vDir);

		//vPos = pPlayerTransCom->m_vInfo[INFO_POS] +vDir;
		//m_pColliderSphereCom->Set_Center(vPos);

		//m_pColliderSphereCom->Set_Center_Trans(20 * vDir);
		if (nullptr == m_pParentBoneMatrix)
		{
			CDynamicMesh*		pPlayerMeshCom = dynamic_cast<CGuardMan*>(m_pGuardMan)->Get_MeshCom();
			NULL_CHECK_RETURN(pPlayerMeshCom, -1);

			const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("NPC_R_Finger02__RF02_");

			m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

			CTransform*		pPlayerTransCom = dynamic_cast<CGuardMan*>(m_pGuardMan)->Get_Transform();
			NULL_CHECK_RETURN(pPlayerTransCom, -1);




			m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
		}

		CGameObject::Update_Object(fTimeDelta);

		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix* *m_pParentWorldMatrix));


		_vec3 vTrans =
			m_pTransformCom->m_vInfo[INFO::INFO_POS];


		_vec3 vDir2 = { 0,10,0 };



		vTrans = vTrans - (5.5*vDir2);
		//vTrans.x += -70;
		////vTrans.y += ;
		//vTrans.z += -70;

		//vTrans.x += -70;
		//vTrans.y += 10;
		//vTrans.z += -70;


		_matrix matScale, matRev, matTrans, matRot, m_matCollWorld;
		const _matrix* p_matCollWorld;

		{

			D3DXMatrixScaling(&matScale, 0.02, 0.02, 0.02);
			D3DXMatrixRotationY(&matRot, m_pTransformCom->m_vAngle.y);
			D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);
			D3DXMatrixRotationAxis(&matRev, &m_pTransformCom->m_vInfo[INFO::INFO_UP], m_pTransformCom->m_vAngle.y);
			m_matCollWorld = matScale * matRot*matTrans*matRev * *m_pTransformCom->Get_WorldMatrix();
			//matTrans*matRev;//* matTrans;//matScale * matRot*matTrans  * matRev; //*pPlayerTransCom->Get_WorldMatrix();



		}


		//_vec3 vPos = { 1,0,1 };
		//m_pTransformCom->Move_Pos(&vPos,5.f,fTimeDelta);

		m_pColliderCom->Set_Matrix(m_matCollWorld);

		//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

		Add_RenderGroup(RENDER_NONALPHA, this);
	}
	return 0;
}

void CimperialSword::Render_Object(void)
{
	if (dynamic_cast<CGuardMan*>(m_pGuardMan)->Get_RenderOn())
	{
		//CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pMeshCom->Render_Meshes();

		m_pColliderCom->Render_ColliderSphere(COLLTYPE(false), m_pColliderCom->Get_CollWorldMatrix());


	}//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

HRESULT CimperialSword::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(L"Proto_Mesh_imperialSword"));
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
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom =CColliderSphere::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_NumVtx(), m_pMeshCom->Get_VtxSize());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;

}



Engine::_bool CimperialSword::Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag)
{
	CCollider*		pPlayerColliderCom = dynamic_cast<CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	//return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
	//	m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());

}

CimperialSword* CimperialSword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CimperialSword*	pInstance = new CimperialSword(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CimperialSword::Free(void)
{
	CGameObject::Free();
}

