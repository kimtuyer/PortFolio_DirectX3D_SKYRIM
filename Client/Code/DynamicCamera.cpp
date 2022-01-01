#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;


	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	Set_HP(1);


	m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
	//_vec3 vPlayer_Pos = m_pTransformCom->m_vInfo[INFO::INFO_POS];
	//
	//_vec3 vPlayerDir = m_pTransformCom->m_vInfo[INFO::INFO_LOOK];
	//
	//D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);
	//
	//
	//m_vEye = vPlayer_Pos	- vPlayerDir;
	////
	//m_vEye.y += 2.5;


	return S_OK;
}

Engine::_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_iSetPos == 0)
	{
		m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
		_vec3 vPlayerDir = m_pTransformCom->m_vInfo[INFO::INFO_LOOK];
		_vec3 vPlayer_Pos = m_pTransformCom->m_vInfo[INFO::INFO_POS];


		m_vCameraPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];
		m_vEye = vPlayer_Pos - 2.5*vPlayerDir; 
		m_vEye.y += 2.75;;
		m_vAt = vPlayer_Pos + vPlayerDir;   
		m_vAt.y += 1.5;




		++m_iSetPos;
	}

	_bool quest6 = dynamic_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Get_Quest6();

	if (quest6)
	{
		m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_DRAGON, L"Com_Transform", ID_DYNAMIC));

		_vec3 vPlayer_Pos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

		_vec3 vPlayerDir = m_pTransformCom->m_vInfo[INFO::INFO_LOOK];

		D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);

		if (m_fTime >= 3.5f)
		{
		CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_flightroar_08.wav", CSoundMgr::Dragon);
		m_vEye = vPlayer_Pos + 15 * vPlayerDir;
		m_vAt = vPlayer_Pos - vPlayerDir; 
		m_vAt.y += 1.5;

		}
	}



	
	if(m_iPOV==OBJECT_PLAYER)
	Key_Input(fTimeDelta, 10.f);

	else if (m_iPOV == OBJECT_DRAGON)
	{

		_bool Pullon1 = dynamic_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Get_PullOn1();
		_bool Pullon2 = dynamic_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Get_PullOn2();
		_bool Pullon3 = dynamic_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Get_PullOn3();


		
		m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_DRAGON, L"Com_Transform", ID_DYNAMIC));

		_vec3 vPlayer_Pos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

		_vec3 vPlayerDir = m_pTransformCom->m_vInfo[INFO::INFO_LOOK];

		D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);


		if (Pullon1)
		{
			m_vEye = vPlayer_Pos + 15 * vPlayerDir;
			m_vAt = vPlayer_Pos - vPlayerDir;   
			m_vAt.y += 1.5;
		}
		if (Pullon2)
		{
			m_vEye = vPlayer_Pos + 7.5 * vPlayerDir;
			m_vEye.y += 10;;
			m_vAt = vPlayer_Pos - vPlayerDir; 
			m_vAt.y += 6;
		}
		if (Pullon3)
		{
			m_vEye = vPlayer_Pos + 7.5 * vPlayerDir;
			m_vEye.y += 10;;
			m_vAt = vPlayer_Pos - vPlayerDir; 
			m_vAt.y += 6;
		}




		//if (GetAsyncKeyState(VK_F4) & 0x0001)
		//{
		//
		//}




		if (Get_DIKeyState(DIK_TAB) & 0x80)
		{


			if (true == m_bClick)
				return 1;

			m_bClick = true;

			if (true == m_bFix)
				m_bFix = false;
			else
				m_bFix = true;

		}
		else
			m_bClick = false;



		if (false == m_bFix)
			return 1;

	}
	else if (m_iPOV == OBJECT_NPC)
	{

		list<CGameObject*> NPClist = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
		list<CGameObject*>::iterator iter = NPClist.begin();

		for (; iter != NPClist.end(); iter++)
		{

			
			if (dynamic_cast<CNPC*>(*iter)->Get_NPC_Type() == dynamic_cast<CDialog*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_Dialog))
				->Get_NPC_Type())
			{
				m_pTransformCom = dynamic_cast<CNPC*>(*iter)->Get_Transform();


				_vec3 vPlayer_Pos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

				_vec3 vPlayerDir = m_pTransformCom->m_vInfo[INFO::INFO_LOOK];

				D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);	
					

				m_vEye = vPlayer_Pos + 1 * vPlayerDir; //vCamWorld;//(vPlayer_Pos - (5 * vPlayerDir));
				m_vEye.y += 1;;
				m_vAt = vPlayer_Pos - vPlayerDir;   //vPlayerDir;		
				m_vAt.y += 1.5; //1.5;

			}

		}


		if (Get_DIKeyState(DIK_TAB) & 0x80)
		{


			if (true == m_bClick)
				return 1;

			m_bClick = true;

			if (true == m_bFix)
				m_bFix = false;
			else
				m_bFix = true;

		}
		else
			m_bClick = false;



		if (false == m_bFix)
			return 1;

		






	}




	if (true == m_bFix)
	{
		Mouse_Fix();
		Mouse_Move(fTimeDelta);
	}									
	//else
	//{
	//	Key_Input(fTimeDelta, 10.f);
	//
	//}


	_int	iExit = CCamera::Update_Object(fTimeDelta);

	

	return iExit;
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta, const _float& fSpeed)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_matrix		matLoclworld;
	D3DXMatrixInverse(&matLoclworld, NULL, &matCamWorld);

	//matCamWorld._41=
	//matCamWorld._42 =
	//matCamWorld._43 =




	m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));

	_vec3 vPlayer_Pos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

	_vec3 vPlayerDir = m_pTransformCom->m_vInfo[INFO::INFO_LOOK];

	D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);



	const _matrix* PWorldmatrix = m_pTransformCom->Get_WorldMatrix();

	_vec3 vPlayer_Pos2 = m_pTransformCom->m_vInfo[INFO::INFO_POS];


	//	_matrix		matPLocalWorld;
	//D3DXMatrixInverse(&matPLocalWorld, NULL, PWorldmatrix);
	//D3DXVec3TransformCoord(&vPlayer_Pos2, &vPlayer_Pos2, &matPLocalWorld);
	////플레이어 로컬 위치
	//
	//_vec3 CamPos = { matPLocalWorld._41 ,matPLocalWorld._42 ,matPLocalWorld._43 };
	//_vec3 LocDir = { matPLocalWorld._21,matPLocalWorld._22 ,matPLocalWorld._23 };
	//
	////카메라의 로컬영역 위치값을 플레이어 로컬위치값에 더해줌,.
	//vPlayer_Pos2.x+= matLoclworld._41;
	//	vPlayer_Pos2.y+= matLoclworld._42;
	//	vPlayer_Pos2.z+= matLoclworld._43;
	//
	//_vec3 vCamWorld;
	//
	////더해준 위치값을 다시 월드행렬을 곱해 카메라의 월드 행렬을 구한다.
	//D3DXVec3TransformCoord(&vCamWorld, &vPlayer_Pos2, &matCamWorld);

	_vec3 DragonDir =
		dynamic_cast<CTransform*>(CManagement::GetInstance()->Get_Component(L"GameLogic", OBJECT_DRAGON, L"Com_Transform", ID_DYNAMIC))
		->m_vInfo[INFO::INFO_POS]- vPlayer_Pos;

	D3DXVec3Normalize(&DragonDir, &DragonDir);

	if (!m_Y1key && !m_Y2key)
	{

	m_vEye = vPlayer_Pos- 2.5*vPlayerDir; //vCamWorld;//(vPlayer_Pos - (5 * vPlayerDir));
	m_vEye.y += 2.75;;
	m_vAt = vPlayer_Pos + vPlayerDir;   //vPlayerDir;		
	m_vAt.y += 1.5; //1.5;
	}

	if (m_Y1key)
	{

		m_vEye = vPlayer_Pos - 3.5*vPlayerDir; //vCamWorld;//(vPlayer_Pos - (5 * vPlayerDir));
		m_vEye.y += 2.75;;
		m_vAt = vPlayer_Pos + vPlayerDir;   //vPlayerDir;		
		m_vAt.y += 3; //1.5;
	}
	if (m_Y2key)
	{

		m_vEye = vPlayer_Pos - 3.5*vPlayerDir; //vCamWorld;//(vPlayer_Pos - (5 * vPlayerDir));
		m_vEye.y += 2.75;;
		m_vAt = vPlayer_Pos + vPlayerDir;   //vPlayerDir;		
		m_vAt.y += 6; //1.5;
	}


	if (!dynamic_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Get_DialogCheck() && GetAsyncKeyState(VK_F1) & 0x0001)
	{
		m_Y1key = true;

		m_vEye = vPlayer_Pos - 2.5*vPlayerDir;
		m_vEye.y += 2.75;;
		m_vAt = vPlayer_Pos + DragonDir;// 2.5*DragonDir;
		m_vAt.y += 4.5; 




	}
	if (GetAsyncKeyState(VK_F2) & 0x0001)
	{
		m_Y2key = true;

		m_vEye = vPlayer_Pos - 2.5*vPlayerDir;
		m_vEye.y += 2.75;;
		m_vAt = vPlayer_Pos + DragonDir;// 2.5*DragonDir;
		m_vAt.y += 6;
	}
	if (GetAsyncKeyState(VK_F3) & 0x0001)
	{
		m_Y1key = false;
		m_Y2key = false;

	}


	//if (Get_DIKeyState(DIK_W) & 0x80)
	//{
	//	_vec3		vLook;
	//	memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));
	//
	//	_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.f * fTimeDelta;
	//
	//	m_vEye += vLength;
	//	m_vAt += vLength;
	//}
	
	//if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))
	//	->Get_Wkey())
	//{
	//	_vec3	vLength = vPlayerDir * 5.f * fTimeDelta;
	//
	//	m_vEye += vLength;
	//	m_vAt += vLength;
	//
	//
	//}
	//if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))
	//	->Get_Skey())
	//{
	//	_vec3	vLength = vPlayerDir * 5.f * fTimeDelta;
	//
	//	m_vEye -= vLength;
	//	m_vAt -= vLength;
	//
	//
	//}
	//if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))
	//	->Get_Dkey())
	//{
	//	_vec3 fY = { 0,1,0 };
	//
	//	_vec3 vNormal;  D3DXVec3Cross(&vNormal, &vPlayerDir, &fY);
	//	_vec3	vLength = vNormal * 5.f * fTimeDelta;
	//
	//	m_vEye += vLength;
	//	m_vAt += vLength;
	//
	//
	//}
	//if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))
	//	->Get_Akey())
	//{
	//	_vec3 fY = { 0,1,0 };
	//
	//	_vec3 vNormal;  D3DXVec3Cross(&vNormal, &vPlayerDir, &fY);
	//	_vec3	vLength = vNormal * 5.f * fTimeDelta;
	//	m_vEye -= vLength;
	//	m_vAt -= vLength;
	//
	//
	//}
	




	_long	dwMouseMove = 0;


	if (dwMouseMove = Get_DIMouseMove(DIMS_Y))
	{

		_bDimY = true;
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		//m_vAt = m_vEye + vLook;


	}
	else
	{
		_bDimY = false;

	}


	if(m_bMoveState) //
	{
		m_vCameraPos = vPlayer_Pos;

		
		_vec3 vDir = { matCamWorld._31,matCamWorld._32, matCamWorld._33 };

		{

		//m_vEye +=(m_vCurDir * fTimeDelta * 5.f);//- 0.2*m_vCurDir;// +CamWorld.;
		m_vEye = vPlayer_Pos - 2.5 * vPlayerDir;
		m_vEye.y += 2.75;;
		
												////m_vEye.y += 2.5;;
		//m_vAt = vCamWorld * fTimeDelta * 5.f;//   (m_vCurDir* fTimeDelta * 5.f);// -(vPlayerDir); //- vPlayerDir);
		m_vAt = vPlayer_Pos + vPlayerDir;
		m_vAt.y += 1.5;


		}
	


		//if(_bDimY)
		//m_vAt += vLook;

	}
	//m_vAt = vPlayer_Pos + m_vCurDir;

	//위아래 시점이동은 잘됨,하지만
	//다시 at이 플레이어 바로 위에 위치하고 좌우시점이동이 제한,부자연,플레이어가 바라보는 방향과도 정확히 맞지 않게됨

	//카메라만 위치값을 저장하는 벡터를 생성해 플레이어위치 받아와 초기에 위치를 잡아준뒤
	//플레이어 이동및 이동방향 따라 카메라위치값 역시 일정한 플레이어방향벡터값 줘가면서 이동해야 할듯









	



	if (Get_DIKeyState(DIK_TAB) & 0x80)
	{


		if (true == m_bClick)
			return;

		m_bClick = true;

		if (true == m_bFix)
			m_bFix = false;
		else
			m_bFix = true;

	}
	else
		m_bClick = false;



	if (false == m_bFix)
		return;

}

void CDynamicCamera::Mouse_Move(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);
	_long	dwMouseMove = 0;

	
	if (dwMouseMove = Get_DIMouseMove(DIMS_Z))
	{
		
		//_bDimY = true;

			_vec3		vLook;
			memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

			_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 10.f * fTimeDelta;

			if (dwMouseMove > 0)
			{
			m_vEye += vLength;
			m_vAt += vLength;

			}
			else
			{
				m_vEye -= vLength;
				m_vAt -= vLength;
			}

	}



		//m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
		//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f) * fTimeDelta);
		////m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * -fTimeDelta);



	
	
	
	

}

void CDynamicCamera::Mouse_Fix(void)
{
	POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);


}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Free(void)
{
	CCamera::Free();
}
