#include "stdafx.h"
#include "StaticCamera.h"

#include "Export_Function.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CStaticCamera::~CStaticCamera(void)
{

}

HRESULT CStaticCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;


	//float w = 2.f / WINCX;
	//float h = 2.f / WINCY;
	//float a = 1.f;
	//float b = 0.f;
	//
	//_matrix A{
	//w,0,0,0,
	//0,h,0,0,
	//0,0,a,0,
	//0,0,b,1
	//
	//};
	//
	//D3DXMatrixPerspectiveFovLH(&A, WINCX, WINCY, m_fNear, m_fFar);



	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	//D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	//
	//D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;



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

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	if (m_iSetPos == 0)
	{
		m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
		_vec3 vPlayerDir = m_pTransformCom->m_vInfo[INFO::INFO_LOOK];
		_vec3 vPlayer_Pos = m_pTransformCom->m_vInfo[INFO::INFO_POS];


		m_vCameraPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

		//m_vEye = (vPlayer_Pos - (5*vPlayerDir));
		//m_vEye.y += 2.5;;
		//m_vAt = vPlayer_Pos + vPlayerDir;
		//m_vAt.y += 2;

		++m_iSetPos;
	}



	

	Key_Input(fTimeDelta, 10.f);


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
	//D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	

	return iExit;
}

void CStaticCamera::Key_Input(const _float& fTimeDelta, const _float& fSpeed)
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


		_matrix		matPLocalWorld;
	D3DXMatrixInverse(&matPLocalWorld, NULL, PWorldmatrix);
	D3DXVec3TransformCoord(&vPlayer_Pos2, &vPlayer_Pos2, &matPLocalWorld);
	//플레이어 로컬 위치

	_vec3 CamPos = { matPLocalWorld._41 ,matPLocalWorld._42 ,matPLocalWorld._43 };
	_vec3 LocDir = { matPLocalWorld._21,matPLocalWorld._22 ,matPLocalWorld._23 };

	//카메라의 로컬영역 위치값을 플레이어 로컬위치값에 더해줌,.
	vPlayer_Pos2.x+= matLoclworld._41;
		vPlayer_Pos2.y+= matLoclworld._42;
		vPlayer_Pos2.z+= matLoclworld._43;

	_vec3 vCamWorld;

	//더해준 위치값을 다시 월드행렬을 곱해 카메라의 월드 행렬을 구한다.
	D3DXVec3TransformCoord(&vCamWorld, &vPlayer_Pos2, &matCamWorld);


	//m_vEye = vPlayer_Pos- 2.5*vPlayerDir; //vCamWorld;//(vPlayer_Pos - (5 * vPlayerDir));
	//m_vEye.y += 2.75;;
	//m_vAt = vPlayer_Pos+ vPlayerDir;//vCamWorld;// vPlayer_Pos + vPlayerDir;
	//m_vAt.y += 1.5;

	//matCamWorld=
	//_vec3 SetComPos = { 0,0,50 };
	//_vec3 LocCamPos = vPlayer_Pos + SetComPos;


	//D3DXVec3TransformCoord(&LocCamPos, &LocCamPos, &matCamWorld);






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

void CStaticCamera::Mouse_Move(const _float& fTimeDelta)
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

void CStaticCamera::Mouse_Fix(void)
{
	POINT		ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);


}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CStaticCamera*	pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CStaticCamera::Free(void)
{
	CCamera::Free();
}
