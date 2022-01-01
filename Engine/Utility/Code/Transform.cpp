#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{

}

Engine::CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_matWorld(rhs.m_matWorld)
	, m_matNRotWorld(rhs.m_matNRotWorld)
{
	for(_uint i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];
}

Engine::CTransform::~CTransform(void)
{

}

void CTransform::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vInfo[INFO_POS].x = fX;
	m_vInfo[INFO_POS].y = fY;
	m_vInfo[INFO_POS].z = fZ;
}

void CTransform::Set_Pos(_vec3 * pPos)
{
	//memcpy(&m_matWorld._41, pPos, sizeof(_vec3));

	m_vInfo[INFO_POS] = *pPos;
}

const _matrix* Engine::CTransform::Get_NRotWorldMatrix(void)
{
	return &m_matNRotWorld;
}

void Engine::CTransform::Get_WorldMatrix(_matrix* pWorld) const
{
	*pWorld = m_matWorld;
}

const _matrix* Engine::CTransform::Get_WorldMatrix(void)
{
	return &m_matWorld;
}

void Engine::CTransform::Rotation(ROTATION eType, const _float& fAngle)
{
	*(((_float*)&m_vAngle) + eType) += fAngle;
}


void Engine::CTransform::Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
{
	m_vInfo[INFO_POS] += *pDir * fSpeed *fTimeDelta;
}

void Engine::CTransform::Get_Info(INFO eType, _vec3* pInfo)
{
	memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
}

HRESULT Engine::CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	
	return S_OK;
}

Engine::_int Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	for (_int i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// ���� ��� : ũ�� * ���� * �̵� (* ���� * �θ�)

	// ũ�� ��ȯ
	for (_int i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}
	
	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_matNRotWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));


			
	/*right *= m_vScale.x
	up    *= m_vScale.y
	look  *= m_vScale.z*/

	// ȸ�� ��ȯ
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_int i = 0; i < INFO_POS; ++i)
	{
		for (_int j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}
	
	
	// ������ ������ ���� ���͵��� �̿��Ͽ� ���� ��� ����
	for(_int i = 0; i < INFO_END; ++i)
		memcpy( &m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

CTransform* Engine::CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform*	pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTransform::Free(void)
{
	CComponent::Free();
}

CComponent* Engine::CTransform::Clone(void)
{
	return  new CTransform(*this);
}

void Engine::CTransform::Move_PickingPos(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	vDir = *pPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
}

void CTransform::Chase_Target(const _vec3 * pTargetPos, const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matRot = *Compute_LookAtTarget(pTargetPos);

	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x,
									m_vInfo[INFO_POS].y,
									m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix * CTransform::Compute_LookAtTarget(const _vec3 * pTargetPos)
{
	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];
	float fRad = acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
		D3DXVec3Normalize(&m_vInfo[INFO_LOOK], &m_vInfo[INFO_LOOK])));



	_vec3	vAxis =	*D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);

	_matrix		matRot;
	_vec3		vUp = {0,1,0};

	// ������ ��ȸ�� ��� ������ִ� �Լ�
	if (fRad <1)
	return D3DXMatrixRotationAxis(&matRot, &vUp,
											acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
															  D3DXVec3Normalize(&m_vInfo[INFO_LOOK], &m_vInfo[INFO_LOOK]))));
	else
		return  D3DXMatrixRotationAxis(&matRot, &vUp,
			-acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
				D3DXVec3Normalize(&m_vInfo[INFO_LOOK], &m_vInfo[INFO_LOOK]))));
}

void CTransform::Chase_Target_Navi(const _vec3 * pTargetPos, _vec3 vPos)
{

	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];

	


	m_vInfo[INFO_POS] = vPos;

	_matrix		matRot = *Compute_LookAtTarget(pTargetPos);

	_matrix		matScale, matTrans, matRot1;

	if (vDir.x > 0 && vDir.z > 0)
	{
	D3DXMatrixRotationY(&matRot1, acosf(D3DXVec3Dot(D3DXVec3Normalize(&-vDir, &-vDir),
	D3DXVec3Normalize(&m_vInfo[INFO_UP], &m_vInfo[INFO_UP]))));

	}	
	else
	{
		D3DXMatrixRotationY(&matRot1, acosf(D3DXVec3Dot(D3DXVec3Normalize(&-vDir, &-vDir),
			D3DXVec3Normalize(&m_vInfo[INFO_UP], &m_vInfo[INFO_UP]))));
	}

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
	
		m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot* matTrans;//matTrans;// matRot * matTrans;

}

void Engine::CTransform::Set_ParentMatrix(const _matrix* pParentMatrix)
{
	m_matWorld *= *pParentMatrix;
}

void CTransform::Set_WorldMatrix(const _matrix * pWorld)
{
	m_matWorld = *pWorld;

}

void Engine::CTransform::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_vScale.z = fZ;
}
