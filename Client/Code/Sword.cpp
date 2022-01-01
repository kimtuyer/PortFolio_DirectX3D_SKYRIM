#include "stdafx.h"
#include "Sword.h"

#include "ColliderSphere.h"
#include "Export_Function.h"

CSword::CSword(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CSword::CSword(const CSword& rhs)
	: CGameObject(rhs)
{

}

CSword::~CSword(void)
{

}

HRESULT CSword::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(0.f));


	//m_pColliderCom->Set_RadiusScale(0.1);
	Set_HP(1);

	return S_OK;
}

Engine::_int CSword::Update_Object(const _float& fTimeDelta)
{
	_vec3 vDir, vPos;


	if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_bWeaponEquip())
	{


		CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_ID::OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vDir);

	
		//if (m_pParentBoneMatrix)
		{
			if (!dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_ArmorFullEquip())
			{

				pPlayerMeshCom = dynamic_cast<CDynamicMesh*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_MeshNude", ID_STATIC));
			}
			else
			{
				pPlayerMeshCom = dynamic_cast<CDynamicMesh*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_MeshFull", ID_STATIC));

			}

			NULL_CHECK_RETURN(pPlayerMeshCom, -1);
			const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName(m_wFrameBoneName);

			m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

			CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
			NULL_CHECK_RETURN(pPlayerTransCom, -1);




			m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
		}

		CGameObject::Update_Object(fTimeDelta);

		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix* *m_pParentWorldMatrix));


		_vec3 vTrans =
			m_pTransformCom->m_vInfo[INFO::INFO_POS];

		_vec3 vDir2;
		if (m_wFrameBoneName == "NPC_R_Finger02__RF02_")
		{

			vDir2 = { 0,0,10 };
			vTrans = vTrans + (7.5*vDir2);
		}
		else
		{
			//vDir2 = { 0,10,0 };
		//	vTrans = vTrans - (5.5*vDir2);

		}



		//vTrans.x += -70;
		////vTrans.y += ;
		//vTrans.z += -70;

		//vTrans.x += -70;
		//vTrans.y += 10;
		//vTrans.z += -70;


		_matrix matScale, matRev, matTrans, matRot, m_matCollWorld;
		const _matrix* p_matCollWorld;

		{

			D3DXMatrixScaling(&matScale, 0.2, 0.2, 0.2);
			D3DXMatrixRotationY(&matRot, m_pTransformCom->m_vAngle.y);
			D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);
			D3DXMatrixRotationAxis(&matRev, &m_pTransformCom->m_vInfo[INFO::INFO_UP], m_pTransformCom->m_vAngle.y);
			m_matCollWorld = matScale * matRot*matTrans*matRev * *m_pTransformCom->Get_WorldMatrix();
			//matTrans*matRev;//* matTrans;//matScale * matRot*matTrans  * matRev; //*pPlayerTransCom->Get_WorldMatrix();



		}


		m_pColliderCom->Set_Matrix(m_matCollWorld);


		



		Add_RenderGroup(RENDER_NONALPHA, this);
	}
	return 0;
}

void CSword::Render_Object(void)
{
	if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_bWeaponEquip())
	{



		CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pMeshCom->Render_Meshes();

		//m_pColliderCom->Render_ColliderSphere(COLLTYPE(false), m_pColliderCom->Get_CollWorldMatrix());
	}
	//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
}

HRESULT CSword::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Proto(L"Proto_Mesh_Sword"));
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



Engine::_bool CSword::Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag)
{
	CColliderSphere*		pPlayerColliderCom = dynamic_cast<CColliderSphere*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	return m_pCalculatorCom->Collision_Sphere(pPlayerColliderCom->Get_Radius(), pPlayerColliderCom->Get_Center(),
		pPlayerColliderCom->Get_CollWorldMatrix(), 0.01, m_pColliderCom->Get_Radius(), m_pColliderCom->Get_Center(), m_pColliderCom->Get_CollWorldMatrix(), 0.01);


}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSword*	pInstance = new CSword(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSword::Free(void)
{
	CGameObject::Free();
}

