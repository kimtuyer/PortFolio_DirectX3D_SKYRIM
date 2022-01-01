#include "Calculator.h"
#include "TerrainTex.h"
#include "Transform.h"

USING(Engine)

Engine::CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{

}

Engine::CCalculator::CCalculator(const CCalculator& rhs)
	:CComponent(rhs)
{

}

Engine::CCalculator::~CCalculator(void)
{

}

HRESULT Engine::CCalculator::Ready_Calculator(void)
{
	//D3DXPLANE		Plane;

	//// 평면을 이루는 정점의 위치를 넣어주면 평면의 ABCD를 만들어주는 함수
	// D3DXPlaneFromPoints()

	 // 평면의 방정식

	
	

	 // x,y,z는 평면 위에 놓여있는 정점의 좌표
	 // a, b, c, d는 평면과 직각을 이루는 법선의 방향성

	return S_OK;
}

Engine::_float Engine::CCalculator::Compute_HeightOnTerrain(const _vec3* pPos, 
															const _vec3* pTerrainVtxPos, 
															const _ulong& dwCntX, 
															const _ulong& dwCntZ, 
															const _ulong& dwVtxItv)
{
	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	_float	fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;;


	D3DXPLANE		Plane;


	// 오른쪽 위
	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane, &pTerrainVtxPos[dwIndex + dwCntX], 
									&pTerrainVtxPos[dwIndex + dwCntX + 1], 
									&pTerrainVtxPos[dwIndex + 1]);
	}
	else
	{
		D3DXPlaneFromPoints(&Plane, &pTerrainVtxPos[dwIndex + dwCntX],
									&pTerrainVtxPos[dwIndex + 1],
									&pTerrainVtxPos[dwIndex]);
	}
	// ax + by + cz + d = 0->y = ?
	// by = -ax - cz - d
	// y =(-ax - cz - d) / b

	return ((-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b);
}

_float CCalculator::Compute_HeightOnNavi(const _vec3 * pPos, const _vec3 A, const _vec3 B, const _vec3 C)
{
	D3DXPLANE		Plane;

	D3DXPlaneFromPoints(&Plane, &A,
		&B,
		&C);

	return ((-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b);


}

Engine::CCalculator* Engine::CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCalculator::Free(void)
{
	CComponent::Free();
}

Engine::CComponent* Engine::CCalculator::Clone(void)
{
	return new CCalculator(*this);
}

Engine::_vec3 Engine::CCalculator::Picking_OnTerrain(HWND hWnd,
	const CTerrainTex* pTerrainBufferCom,
	const CTransform* pTerrainTransCom)
{
	POINT	ptMouse = {};

	GetCursorPos(&ptMouse);

	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 영역의 좌표를 투영 영역의 좌표로 변환
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 상태의 마우스를 뷰스페이스 상태로 변환
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰스페이스 상태의 마우스를 월드 영역으로 변환
	_vec3		vRayPos, vRayDir;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 상태에서 로컬 영역까지 변환
	_matrix		matWorld;
	pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	const _vec3*		pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_float		fU, fV, fDist;

	_ulong		dwVtxIdx[3];

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if(D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]], 
								&pTerrainVtx[dwVtxIdx[2]],
								&pTerrainVtx[dwVtxIdx[0]], 
								&vRayPos, 
								&vRayDir,
								&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
							0.f,
							pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}
			
			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
								&pTerrainVtx[dwVtxIdx[0]],
								&pTerrainVtx[dwVtxIdx[1]],
								&vRayPos,
								&vRayDir,
								&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
							0.f,
							pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _vec3();
}

Engine::_bool Engine::CCalculator::Collision_AABB(const _vec3* pDestMin,
													const _vec3* pDestMax, 
													const _matrix* pDestWorld, 
													const _vec3* pSourMin, 
													const _vec3* pSourMax, 
													const _matrix* pSourWorld)
{
	_vec3		vDestMin, vDestMax, vSourMin, vSourMax;
	_float		fMin, fMax;

	D3DXVec3TransformCoord(&vDestMin, pDestMin, pDestWorld);
	D3DXVec3TransformCoord(&vDestMax, pDestMax, pDestWorld);

	D3DXVec3TransformCoord(&vSourMin, pSourMin, pSourWorld);
	D3DXVec3TransformCoord(&vSourMax, pSourMax, pSourWorld);

	// x축에서 바라봤을 때
	
	fMin = max(vDestMin.x, vSourMin.x);
	fMax = min(vDestMax.x, vSourMax.x);

	if (fMax < fMin)
		return false;

	// y축에서 바라봤을 때

	fMin = max(vDestMin.y, vSourMin.y);
	fMax = min(vDestMax.y, vSourMax.y);

	if (fMax < fMin)
		return false;
	
	// z축에서 바라봤을 때

	fMin = max(vDestMin.z, vSourMin.z);
	fMax = min(vDestMax.z, vSourMax.z);

	if (fMax < fMin)
		return false;

	return true;
}

_bool CCalculator::Collision_OBB(const _vec3 * pDestMin, const _vec3 * pDestMax, 
									const _matrix * pDestWorld, 
									const _vec3 * pSourMin, const _vec3 * pSourMax, 
									const _matrix * pSourWorld)
{
	OBB		tObb[2];
	ZeroMemory(tObb, sizeof(OBB) * 2);

	Set_Point(&tObb[0], pDestMin, pDestMax);
	Set_Point(&tObb[1], pSourMin, pSourMax);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&tObb[0].vPoint[i], &tObb[0].vPoint[i], pDestWorld);
		D3DXVec3TransformCoord(&tObb[1].vPoint[i], &tObb[1].vPoint[i], pSourWorld);
	}

	D3DXVec3TransformCoord(&tObb[0].vCenter, &tObb[0].vCenter, pDestWorld);
	D3DXVec3TransformCoord(&tObb[1].vCenter, &tObb[1].vCenter, pSourWorld);

	for (_uint i = 0; i < 2; ++i)
		Set_Axis(&tObb[i]);

	_float	fDistance[3];		// 0번 : 첫 번째 obb의 중점에서 각 면을 향해 뻗어 나가는 벡터들을 각 축의 투영한 길이의 합
								// 1번 : 두 번째 obb의 중점에서 각 면을 향해 뻗어 나가는 벡터들을 각 축의 투영한 길이의 합
								// 2번 : 0번 obb의 중점에서 1번 obb의 중점으로 향하는 방향 벡터를 각 축의 투영한 길이의 합

	for (_int i = 0; i < 2; ++i)
	{
		for (_int j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&tObb[0].vProjAxis[0], &tObb[i].vAxis[j])) +
						   fabs(D3DXVec3Dot(&tObb[0].vProjAxis[1], &tObb[i].vAxis[j])) +
						   fabs(D3DXVec3Dot(&tObb[0].vProjAxis[2], &tObb[i].vAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&tObb[1].vProjAxis[0], &tObb[i].vAxis[j])) +
							fabs(D3DXVec3Dot(&tObb[1].vProjAxis[1], &tObb[i].vAxis[j])) +
							fabs(D3DXVec3Dot(&tObb[1].vProjAxis[2], &tObb[i].vAxis[j]));

			fDistance[2] = fabs(D3DXVec3Dot(&(tObb[1].vCenter - tObb[0].vCenter), &tObb[i].vAxis[j]));
				
			if (fDistance[2] > fDistance[0] + fDistance[1])
				return false;
		}
	}
	
	return true;
}

_bool CCalculator::Collision_Sphere(float & pDestRadius, _vec3 * pDestCenter, const _matrix * pDestWorld, float DestScale, 
	float & pSourRadius, _vec3 * pSourCenter, const _matrix * pSourWorld, float SourScale)
{
	_vec3		 vDestMax, vSourMax;
	_float		fMin, fMax, fDist, fDist2;
	D3DXVECTOR3 vDif;

	D3DXVec3TransformCoord(pDestCenter, pDestCenter, pDestWorld);
	D3DXVec3TransformCoord(pSourCenter, pSourCenter, pSourWorld);

	D3DXVECTOR3*		vDestCenter = pDestCenter;
	D3DXVECTOR3*		vSourCenter = pSourCenter;
	
	if (vDestCenter->x < 0 || vDestCenter->y < 0)
	{
		return false;
	}

	fDist = sqrtf((vDestCenter->x - vSourCenter->x)*(vDestCenter->x - vSourCenter->x) +

		(vDestCenter->y - vSourCenter->y) + (vDestCenter->y - vSourCenter->y) +
		(vDestCenter->z - vSourCenter->z) + (vDestCenter->z - vSourCenter->z));


	vDif = ((*vDestCenter) - (*vSourCenter));
	fDist2 = D3DXVec3Length(&vDif);

	if (pDestRadius*DestScale + pSourRadius * SourScale >= fDist)
	{

		if (m_iColliderCount == 0) //첫 충돌체크후 충돌 안할대까지 해당문 돌지않게
		{
			ColliderCheck = true;
		}
		m_iColliderCount++;
		return true;

	}
	else
	{
		m_iColliderCount = 0;
		ColliderCheck = false;

		return false;

	}

}

void CCalculator::Set_Point(OBB * pObb, const _vec3 * pMin, const _vec3 * pMax)
{
	pObb->vPoint[0] = _vec3(pMin->x, pMax->y, pMin->z);
	pObb->vPoint[1] = _vec3(pMax->x, pMax->y, pMin->z);
	pObb->vPoint[2] = _vec3(pMax->x, pMin->y, pMin->z);
	pObb->vPoint[3] = _vec3(pMin->x, pMin->y, pMin->z);
		  	
	pObb->vPoint[4] = _vec3(pMin->x, pMax->y, pMax->z);
	pObb->vPoint[5] = _vec3(pMax->x, pMax->y, pMax->z);
	pObb->vPoint[6] = _vec3(pMax->x, pMin->y, pMax->z);
	pObb->vPoint[7] = _vec3(pMin->x, pMin->y, pMax->z);

	pObb->vCenter = (*pMin + *pMax) * 0.5f;
}

void CCalculator::Set_Axis(OBB* pObb)
{
	pObb->vProjAxis[0] = (pObb->vPoint[2] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
	pObb->vProjAxis[1] = (pObb->vPoint[0] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
	pObb->vProjAxis[2] = (pObb->vPoint[7] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;

	pObb->vAxis[0] = pObb->vPoint[2] - pObb->vPoint[3];
	pObb->vAxis[1] = pObb->vPoint[0] - pObb->vPoint[3];
	pObb->vAxis[2] = pObb->vPoint[7] - pObb->vPoint[3];

	for (_uint i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pObb->vAxis[i], &pObb->vAxis[i]);

}

