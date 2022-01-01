#include "stdafx.h"
#include "PlayerArmor.h"

#include "ColliderSphere.h"
#include "Export_Function.h"

CPlayerArmor::CPlayerArmor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPlayerArmor::CPlayerArmor(const CPlayerArmor& rhs)
	: CGameObject(rhs)
{

}

CPlayerArmor::~CPlayerArmor(void)
{

}

HRESULT CPlayerArmor::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(0.f));


	//m_pColliderCom->Set_RadiusScale(0.1);
	m_pMeshCom->Set_AnimationIndex(12);
	Set_HP(1);

	return S_OK;
}

Engine::_int CPlayerArmor::Update_Object(const _float& fTimeDelta)
{
	_vec3 vDir, vPos;

	m_iPlayerAniIndex= dynamic_cast<CPlayer*>(m_pPlayer)->Get_AniIndex();


	if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_ArmorEquip() && !dynamic_cast<CPlayer*>(m_pPlayer)->Get_ArmorFullEquip())
	{




		m_pMeshCom->Set_AnimationIndex(m_iPlayerAniIndex);



		//m_pColliderSphereCom->Set_RadiusScale(0.15);
		CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_ID::OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vDir);

		//vPos = pPlayerTransCom->m_vInfo[INFO_POS] +vDir;
		//m_pColliderSphereCom->Set_Center(vPos);

		//m_pColliderSphereCom->Set_Center_Trans(20 * vDir);
		//if (m_pParentBoneMatrix)
		{

			CDynamicMesh*		pPlayerMeshCom = dynamic_cast<CDynamicMesh*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_MeshNude", ID_STATIC));
			NULL_CHECK_RETURN(pPlayerMeshCom, -1);

			const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("MaleUnderwearBodyHeavyArmor002");

			m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

			CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
			NULL_CHECK_RETURN(pPlayerTransCom, -1);




			m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();
		}

		CGameObject::Update_Object(fTimeDelta);
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix* *m_pParentWorldMatrix));

		if (m_iPlayerAniIndex == 12)//  idle
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;


		}

		if (m_iPlayerAniIndex == 0)//  무기 든 채로 idle
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

		}




		if (m_iPlayerAniIndex == 13)// 그냥 달리기
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 8;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_forwardStop())//전방 충돌시
			{
				m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;

			}



		}
		if (m_iPlayerAniIndex == 15)// 무기든채 달리기
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 8;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_forwardStop())//전방 충돌시
			{
				m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;

			}


		}

		if (m_iPlayerAniIndex == 17)// 무기 장착+후진 달리기
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = -11;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_bacwardStop())//전방 충돌시
			{
				m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;

			}



		}
		if (m_iPlayerAniIndex == 14)// 무기 없이 후진
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = -11;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_bacwardStop())//전방 충돌시
			{
				m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;

			}


		}

		if (m_iPlayerAniIndex == 2)// 무기 없이 우측이동
		{

			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 8;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;


			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_RightStop())//전방 충돌시
			{
				m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

			}


		}
		if (m_iPlayerAniIndex == 4)// 무기 장착 우측이동
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 8;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;

			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_RightStop())//전방 충돌시
			{
				m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

			}

		}

		if (m_iPlayerAniIndex == 5)// 무기 장착 좌측이동
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = -8;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;


			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_LeftStop())//전방 충돌시
			{
				m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

			}


		}
		if (m_iPlayerAniIndex == 3)// 무기 없이 좌측이동
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].x = -8;;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z = 0;;

			if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_LeftStop())//전방 충돌시
			{
				m_pTransformCom->m_vInfo[INFO::INFO_POS].x = 0;;

			}

		}
		if (m_iPlayerAniIndex == 10)// 무기 해제
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;

		}

		if (m_iPlayerAniIndex == 11)// 무기 착용
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;

		}


		if (m_iPlayerAniIndex == 8)// 우단 베기 공격
		{
			m_pTransformCom->m_vInfo[INFO::INFO_POS].y = -120;;

		}







		_vec3 vDir2;



		// vDir2 = { 0,0,10 };	
		// vTrans = vTrans + (7.5*vDir2);





		//vTrans.x += -70;
		////vTrans.y += ;
		//vTrans.z += -70;

		//vTrans.x += -70;
		//vTrans.y += 10;
		//vTrans.z += -70;


		//_matrix matScale, matRev, matTrans, matRot, m_matCollWorld;
		//const _matrix* p_matCollWorld;
		//
		//{
		//
		//	D3DXMatrixScaling(&matScale, 0.2, 0.2, 0.2);
		//	D3DXMatrixRotationY(&matRot, m_pTransformCom->m_vAngle.y);
		//	D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);
		//	D3DXMatrixRotationAxis(&matRev, &m_pTransformCom->m_vInfo[INFO::INFO_UP], m_pTransformCom->m_vAngle.y);
		//	m_matCollWorld = matScale * matRot*matTrans*matRev * *m_pTransformCom->Get_WorldMatrix();
		//	//matTrans*matRev;//* matTrans;//matScale * matRot*matTrans  * matRev; //*pPlayerTransCom->Get_WorldMatrix();
		//
		//
		//
		//}



		//_vec3 vPos = { 1,0,1 };
		//m_pTransformCom->Move_Pos(&vPos,5.f,fTimeDelta);

		//m_pColliderCom->Set_Matrix(m_matCollWorld);

		//m_bColl = Collision_ToObject(L"GameLogic", OBJECT_GuardMan);

		if (m_bColl)
		{
			int i = 0;
		}
		else
		{

		}


		m_pMeshCom->Play_Animation(fTimeDelta);


		Add_RenderGroup(RENDER_NONALPHA, this);

	}
	return 0;
}

void CPlayerArmor::Render_Object(void)
{
	if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_ArmorEquip() && !dynamic_cast<CPlayer*>(m_pPlayer)->Get_ArmorFullEquip())
	{


		CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pMeshCom->Render_Meshes();

		//m_pColliderCom->Render_ColliderSphere(COLLTYPE(false), m_pColliderCom->Get_CollWorldMatrix());

		//m_pColliderCom->Render_Collider(COLLTYPE(m_bColl), m_pTransformCom->Get_NRotWorldMatrix());
	}
}

HRESULT CPlayerArmor::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_HideArmor"));
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



Engine::_bool CPlayerArmor::Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag)
{
	CColliderSphere*		pPlayerColliderCom = dynamic_cast<CColliderSphere*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	return m_pCalculatorCom->Collision_Sphere(pPlayerColliderCom->Get_Radius(), pPlayerColliderCom->Get_Center(),
		pPlayerColliderCom->Get_CollWorldMatrix(), 0.01, m_pColliderCom->Get_Radius(), m_pColliderCom->Get_Center(), m_pColliderCom->Get_CollWorldMatrix(), 0.01);


}

CPlayerArmor* CPlayerArmor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerArmor*	pInstance = new CPlayerArmor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayerArmor::Free(void)
{
	CGameObject::Free();
}

