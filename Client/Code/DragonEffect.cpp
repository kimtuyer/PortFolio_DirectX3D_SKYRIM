#include "stdafx.h"
#include "DragonEffect.h"

#include "Export_Function.h"

CDragonEffect::CDragonEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CDragonEffect::CDragonEffect(const CDragonEffect& rhs)
	: CGameObject(rhs)
{

}

CDragonEffect::~CDragonEffect(void)
{

}

HRESULT CDragonEffect::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iHP = 1;

	//m_pTransformCom->Set_Pos(&_vec3(rand() % 20, 0.f, rand() % 20));
	m_pTransformCom->Set_Scale(0.4, 0.4, 0.4);
	return S_OK;
}

Engine::_int CDragonEffect::Update_Object(const _float& fTimeDelta)
{
	_matrix		matWorld, matView, matBill, matScale, matTrans, matRot;


	if (m_bRenderOn)
	{
		if (m_iCount == 0)
		{


			pDragonCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_DRAGON, L"Com_Transform", ID_DYNAMIC));
			pDragonMesh = dynamic_cast<CDynamicMesh*>(Engine::Get_Component(L"GameLogic", OBJECT_DRAGON, L"Com_Mesh", ID_STATIC));

			const D3DXFRAME_DERIVED*		pFrame = pDragonMesh->Get_FrameByName("NPC_Tongue2");

			m_pBoneMatrix = &pFrame->CombinedTransformMatrix;

			worldbone = { m_pBoneMatrix->_41,m_pBoneMatrix->_42,m_pBoneMatrix->_43 };
			D3DXVec3TransformCoord(&worldbone, &worldbone, pDragonCom->Get_WorldMatrix());

			m_pTransformCom->Set_Pos(&worldbone);

			m_vOrgin = worldbone;

			//m_iCount++;


			//dynamic_cast<CDragon*>(m_pDragon)->Set_ShotFull(true);
		}
		CGameObject::Update_Object(fTimeDelta);
		_vec3 vDir;
		pDragonCom->Get_Info(INFO::INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		 if (m_iBreathType == DraonBreath_Effect::DraonBreath_Beam)

		 {

			 if (dynamic_cast<CDragon*>(m_pDragon)->Get_ShorFull())
			 {
					

				 _vec3 vDir = m_pTargetPos - worldbone;
				 D3DXVec3Normalize(&vDir, &vDir);


				 if (m_IndexCount == 0)
				 {

					 if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))
						 ->Get_ActionState() == CURRENT_BLOCK)
					 {

						 worldbone = m_vOrgin + (vDir*m_iIndex * 0.5);
						 m_vPos = worldbone;
						 //m_vPos = worldbone;

						 m_fDist = sqrtf((m_pTargetPos.x - m_vPos.x) *(m_pTargetPos.x - m_vPos.x) +
							 (m_pTargetPos.y - m_vPos.y)*(m_pTargetPos.y - m_vPos.y) +
							 (m_pTargetPos.z - m_vPos.z)*(m_pTargetPos.z - m_vPos.z));

						 _vec3 DirBlock = { 1,0,1 };

						 if (m_fDist < 1.2)
						 {
							 if (m_iSetIndexCount == 0)	
							 {
								 m_iSetBlockIndex = m_iIndex;
								 m_iSetIndexCount++;
							 }
							 DirBlock = vDir;	

						 }
						 if (m_iSetBlockIndex!=0 &&  m_iSetBlockIndex <= m_iIndex)
						 {


							// if (m_BlockCount % 2 == 0)
							 {
								// DirBlock.x = vDir.z+1;
								 //vDir.x += 1;
								// vDir.z += 1;		

							 }

							// else if (m_BlockCount % 2 == 1)
							 {
								// DirBlock.x = vDir.x-1;

								 //vDir.x -= 1;
								 //vDir.z -= 1;
							 }
							 m_BlockCount++;

							 worldbone = m_vOrgin + (DirBlock*m_iIndex * 0.5);
							 m_vPos = worldbone;

						 }
						 else
						 {
							worldbone = m_vOrgin + (vDir*m_iIndex * 0.5);
							m_vPos = worldbone;



						 }

						 int scaleWeight = dynamic_cast<CDragon*>(m_pDragon)->Get_BreathRange() - m_fDist;

						 if (m_fDist >= 18 && m_fDist <= 19)
						 {
							 //m_fScale = 1;
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;

						 }

						 if (m_fDist >= 17 && m_fDist <= 18)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;

						 }


						 if (m_fDist >= 16 && m_fDist <= 17)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;

						 }


						 if (m_fDist >= 15 && m_fDist <= 16)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;

						 }


						 if (m_fDist >= 14 && m_fDist <= 15)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;

						 }


						 if (m_fDist >= 13 && m_fDist <= 14)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }



						 if (m_fDist >= 12 && m_fDist <= 13)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }



						 if (m_fDist >= 11 && m_fDist <= 12)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }

						 if (m_fDist >= 10 && m_fDist <= 11)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;




						 }
						 if (m_fDist >= 9 && m_fDist <= 10)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }
						 if (m_fDist >= 8 && m_fDist <= 9)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }
						 if (m_fDist >= 7 && m_fDist <= 8)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }
						 if (m_fDist >= 6 && m_fDist <= 7)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }
						 if (m_fDist >= 5 && m_fDist <= 6)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }
						 if (m_fDist >= 4 && m_fDist <= 5)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }
						 if (m_fDist >= 3 && m_fDist <= 4)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }
						 if (m_fDist >= 2 && m_fDist <= 3)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;


						 }
						 if (m_fDist >= 1 && m_fDist <= 2)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;

						 }
						 if (m_fDist <= 1)
						 {
							 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
							 m_fScale += m_fScaleWeight * scaleWeight;

						 }

						 m_fScaleSave = m_fScale;






					 }
					 else
					 {
					 

					 worldbone = m_vOrgin + (vDir*m_iIndex * 0.5);
					 m_vPos = worldbone;

					 m_fDist = sqrtf((m_pTargetPos.x - m_vPos.x) *(m_pTargetPos.x - m_vPos.x) +
						 (m_pTargetPos.y - m_vPos.y)*(m_pTargetPos.y - m_vPos.y) +
						 (m_pTargetPos.z - m_vPos.z)*(m_pTargetPos.z - m_vPos.z));

					 if (m_iIndex == 0)
					 {
						 dynamic_cast<CDragon*>(m_pDragon)->Set_BreathRange(m_fDist);

					 }
					 //if (m_iIndex == 39) //이때 위치값 저장,플레이어 위치와 비교해 플레이어에게 데미지전달하게끔!
					 {
						 m_vCollider = worldbone;
					 }


					 int scaleWeight = dynamic_cast<CDragon*>(m_pDragon)->Get_BreathRange() - m_fDist;

					 if (m_fDist >= 18 && m_fDist <= 19)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;

					 }

					 if (m_fDist >= 17 && m_fDist <= 18)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;

					 }


					 if (m_fDist >= 16 && m_fDist <= 17)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;

					 }


					 if (m_fDist >= 15 && m_fDist <= 16)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;

					 }


					 if (m_fDist >= 14 && m_fDist <= 15)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;

					 }


					 if (m_fDist >= 13 && m_fDist <= 14)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }



					 if (m_fDist >= 12 && m_fDist <= 13)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }



					 if (m_fDist >= 11 && m_fDist <= 12)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }

					 if (m_fDist >= 10 && m_fDist <= 11)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;




					 }
					 if (m_fDist >= 9 && m_fDist <= 10)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }
					 if (m_fDist >= 8 && m_fDist <= 9)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }
					 if (m_fDist >= 7 && m_fDist <= 8)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }
					 if (m_fDist >= 6 && m_fDist <= 7)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }
					 if (m_fDist >= 5 && m_fDist <= 6)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }
					 if (m_fDist >= 4 && m_fDist <= 5)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }
					 if (m_fDist >= 3 && m_fDist <= 4)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }
					 if (m_fDist >= 2 && m_fDist <= 3)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;


					 }
					 if (m_fDist >= 1 && m_fDist <= 2)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;

					 }
					 if (m_fDist <= 1)
					 {
						 D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
						 m_fScale += m_fScaleWeight * scaleWeight;

					 }

					 m_fScaleSave = m_fScale;

					 }



			 }

				m_IndexCount++;
				
				


		
				//if (m_fDist <= 0.5 && !dynamic_cast<CDragon*>(m_pDragon)->Get_EndIndexcheck())
				//{
				//	dynamic_cast<CDragon*>(m_pDragon)->Set_EndIndex(m_iIndex);
				//	//m_bRenderOn = false;
				//}

				

				//나증엔 타겟이 바라보는 방향으로 더해가면서
				//발사되게 수정해야함!

				//D3DXMatrixScaling(&matScale, 1, 1, 1);

					

			}
			else
			{
					m_iSetIndexCount = 0;
				m_IndexCount = 0;

				return 0;
			
			}


		}

		m_pTransformCom->Get_WorldMatrix(&matWorld);
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixIdentity(&matBill);

		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, NULL, &matBill);
		D3DXMatrixScaling(&matScale, m_fScaleSave, m_fScaleSave, m_fScaleSave);
		//D3DXMatrixRotationY(&matRot, D3DXToRadian(90));
		//D3DXMatrixRotationY(&matRot, D3DXToRadian(90));

		//if (m_vecPos.size() >=30)
		D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);




		matWorld = matScale *matTrans;

		// 현재 이 코드는 문제의 소지가 있다.
		// 월드 행렬 = 스 * 자 * 이 * 공 * 부
		// 월 = 빌 * 월(스(I)) -> 빌(자^-1) * 스 * 자 

		m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
		//m_pTransformCom->Set_WorldMatrix(&matWorld);


		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		Compute_ViewZ(&vPos);




		Add_RenderGroup(RENDER_ALPHA, this);
	}


	return 0;
}

void CDragonEffect::Render_Object(void)
{
	if (m_bRenderOn)
	{
		int i = dynamic_cast<CDragon*>(m_pDragon)->Get_EndIndex();
		if (dynamic_cast<CDragon*>(m_pDragon)->Get_EndIndexcheck() && dynamic_cast<CDragon*>(m_pDragon)->Get_EndIndex() <= m_iIndex)
		{
			//m_bRenderOn = false;
			return;
		}



		int num = 0;

		srand((unsigned int)time(NULL));
		for (int i = 0; i < m_itexIndex+1; ++i)
		{
			 num = rand();

		}


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (m_iBreathType == DraonBreath_Effect::DraonBreath_Beam)
	{
		m_itexIndex++;

		if (m_itexIndex == 16)
			m_itexIndex = 0;

		m_pTextureCom->Render_Texture(15-m_itexIndex);

	}
	//m_pTextureCom->Render_Texture(m_itexIndex);
	
	
	m_pBufferCom->Render_Buffer();
	}
}

HRESULT CDragonEffect::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Breath"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;

}

CDragonEffect* CDragonEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDragonEffect*	pInstance = new CDragonEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CDragonEffect::Free(void)
{
	CGameObject::Free();
}

