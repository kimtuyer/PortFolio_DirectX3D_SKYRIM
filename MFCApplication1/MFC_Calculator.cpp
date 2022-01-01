#include "MFC_Calculator.h"
#include "TerrainTex.h"
#include "MFC_TerrainTex.h"
#include "Transform.h"

USING(Engine)

Engine::CMFC_Calculator::CMFC_Calculator(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{

}

Engine::CMFC_Calculator::CMFC_Calculator(const CMFC_Calculator& rhs)
	:CComponent(rhs)
{

}

Engine::CMFC_Calculator::~CMFC_Calculator(void)
{

}

HRESULT Engine::CMFC_Calculator::Ready_MFC_Calculator(void)
{
	//D3DXPLANE		Plane;

	//// 평면을 이루는 정점의 위치를 넣어주면 평면의 ABCD를 만들어주는 함수
	// D3DXPlaneFromPoints()

	 // 평면의 방정식

	
	

	 // x,y,z는 평면 위에 놓여있는 정점의 좌표
	 // a, b, c, d는 평면과 직각을 이루는 법선의 방향성

	return S_OK;
}

Engine::_float Engine::CMFC_Calculator::Compute_HeightOnTerrain(const _vec3* pPos, 
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

Engine::CMFC_Calculator* Engine::CMFC_Calculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMFC_Calculator*	pInstance = new CMFC_Calculator(pGraphicDev);

	if (FAILED(pInstance->Ready_MFC_Calculator()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CMFC_Calculator::Free(void)
{
	CComponent::Free();
}

Engine::CComponent* Engine::CMFC_Calculator::Clone(void)
{
	return new CMFC_Calculator(*this);
}

Engine::_vec3 Engine::CMFC_Calculator::Picking_OnTerrain(HWND hWnd,
	const MFC_TerrainTex* pTerrainBufferCom,
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

void CMFC_Calculator::Set_HeightOnTerrain(_vec3 vMouse, _ulong & dwHeight, _ulong & dwSize, MFC_TerrainTex* pTerrainBufferCom)
{

	_vec3*		pTerrainVtx = pTerrainBufferCom->MFC_Get_VtxPos();

	_ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();
	_ulong		dwByte = 0;


	HANDLE hFile= CreateFile(L"../Resource/Texture/Terrain/TestHeight.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER		m_fH;
	BITMAPINFOHEADER		m_iH;

	ReadFile(hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, NULL);

	CloseHandle(hFile);



	VTXTEX*			pVertex = NULL;


	pTerrainBufferCom->Get_VB()->Lock(0, 0, (void**)&pVertex, 0);

	//m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	//
	//
	_ulong		dwIndex = 0;
	
	for (_ulong i = 0; i < dwVtxCntZ; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX; ++j)
		{
			dwIndex = i * dwVtxCntX + j;
	

			
			if (m_pVertex == nullptr)
			{
				pVertex[dwIndex].vPos = _vec3(_float(j * 1), //간격 일단 1로 고정
					(pPixel[dwIndex] & 0x000000ff) / 20.f,
					_float(i * 1));

				pVertex[dwIndex].vTexUV = _vec2((_float(j) / (dwVtxCntX - 1)) * 20.f,
					(_float(i) / (dwVtxCntZ - 1)) * 20.f);


			}

			
			
			if ((vMouse.x - pTerrainVtx[dwIndex].x)*(vMouse.x - pTerrainVtx[dwIndex].x) +
				(vMouse.z - pTerrainVtx[dwIndex].z)*(vMouse.z - pTerrainVtx[dwIndex].z) <=dwSize*dwSize  )
			{ //마우스 피킹점 중심으로 반지름 거리내에 속할 경우

				float fDist = sqrtf((vMouse.x - pTerrainVtx[dwIndex].x)*(vMouse.x - pTerrainVtx[dwIndex].x) +
					(vMouse.z - pTerrainVtx[dwIndex].z)*(vMouse.z - pTerrainVtx[dwIndex].z));
				

				for (int i = 0; i < 100; ++i)
				
				{
				
					if (fDist >= dwSize - 0.1-(i*0.1) && fDist <= dwSize -(i*0.1))
					{
				
						pVertex[dwIndex].vPos.y = dwHeight - dwSize + (i*0.1)+0.1;
						pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;


						if (fDist <= 2)
						{
							pVertex[dwIndex].vPos.y = dwHeight- 0.01;
							pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
						}

				
				
					}
				
				
				
				
				}

				//if (fDist >= dwSize - 1 && fDist <= dwSize)  // 9~10
				//{
				//	pVertex[dwIndex].vPos.y = dwHeight-9;
				//	pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//
				//}
				//else if (fDist >= dwSize - 2 && fDist <= dwSize - 1)
				//{
				//
				//	pVertex[dwIndex].vPos.y = dwHeight - 8;
				//	pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//}
				//
				//else if (fDist >= dwSize - 3 && fDist <= dwSize - 2)
				//{
				//	pVertex[dwIndex].vPos.y = dwHeight - 7;
				//	pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//
				//}
				//
				//else if (fDist >= dwSize - 4 && fDist <= dwSize - 3)
				//{
				//pVertex[dwIndex].vPos.y = dwHeight - 6;
				//pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//
				//}
				//else if (fDist >= dwSize - 5 && fDist <= dwSize-4){
				//	pVertex[dwIndex].vPos.y = dwHeight - 5;
				//	pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//}
				//
				//else if (fDist >= dwSize - 6 && fDist <= dwSize-5){
				//	pVertex[dwIndex].vPos.y = dwHeight - 4;
				//	pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//}
				//
				//else if (fDist >= dwSize - 7 && fDist <= dwSize-6){
				//	pVertex[dwIndex].vPos.y = dwHeight - 3;
				//	pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//}
				//
				//else if (fDist >= dwSize - 8 && fDist <= dwSize-7){
				//	pVertex[dwIndex].vPos.y = dwHeight - 2;
				//	pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//}
				//
				//else if (fDist >= dwSize - 9 && fDist <= dwSize-8){
				//	pVertex[dwIndex].vPos.y = dwHeight - 1;
				//	pTerrainVtx[dwIndex] = pVertex[dwIndex].vPos;
				//}
				
				
				
				
				//pVertex[dwIndex].vPos.y = dwHeight;


			}
			


		}
	}

	//memcpy(pTerrainVtx, pVertex,sizeof(_ulong))

	m_pVertex = pVertex;
	


	pTerrainBufferCom->MFC_Set_VtxPos(pTerrainVtx);
	pTerrainBufferCom->Get_VB()->Unlock();









	_float		fU, fV, fDist;

	_ulong		dwVtxIdx[3];



	//for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	//{
	//	for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
	//	{
	//		_ulong	dwIndex = i * dwVtxCntX + j;
	//
	//		// 오른쪽 위
	//
	//		dwVtxIdx[0] = dwIndex + dwVtxCntX;
	//		dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
	//		dwVtxIdx[2] = dwIndex + 1;
	//
	//		//if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
	//		//	&pTerrainVtx[dwVtxIdx[2]],
	//		//	&pTerrainVtx[dwVtxIdx[0]],
	//		//	&vRayPos,
	//		//	&vRayDir,
	//		//	&fU, &fV, &fDist))
	//		//{
	//		//	return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
	//		//		0.f,
	//		//		pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
	//		//}
	//
	//		// 왼쪽 아래
	//		dwVtxIdx[0] = dwIndex + dwVtxCntX;
	//		dwVtxIdx[1] = dwIndex + 1;
	//		dwVtxIdx[2] = dwIndex;
	//
	//		//if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
	//		//	&pTerrainVtx[dwVtxIdx[0]],
	//		//	&pTerrainVtx[dwVtxIdx[1]],
	//		//	&vRayPos,
	//		//	&vRayDir,
	//		//	&fU, &fV, &fDist))
	//		//{
	//		//	return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
	//		//		0.f,
	//		//		pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
	//		//}
	//	}
	//}







}

Engine::_bool Engine::CMFC_Calculator::Collision_AABB(const _vec3* pDestMin,
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

_bool CMFC_Calculator::Collision_OBB(const _vec3 * pDestMin, const _vec3 * pDestMax, 
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

_bool CMFC_Calculator::Collision_Sphere(float & pDestRadius, _vec3 * pDestCenter, const _matrix * pDestWorld, float & pSourRadius, _vec3 * pSourCenter, const _matrix * pSourWorld)
{
	_vec3		 vDestMax, vSourMax;
	_float		fMin, fMax;



	D3DXVec3TransformCoord(pDestCenter, pDestCenter, pDestWorld);
	D3DXVec3TransformCoord(pSourCenter, pSourCenter, pSourWorld);

	D3DXVECTOR3*		vDestCenter = pDestCenter;
	D3DXVECTOR3*		vSourCenter = pSourCenter;



	float fDist;
	D3DXVECTOR3 vDif;

	vDif = ((*vDestCenter) - (*vSourCenter));
	fDist = D3DXVec3Length(&vDif);

	if (pDestRadius + pSourRadius >= fDist)
		return true;
	else
		return false;
}

void CMFC_Calculator::Set_Point(OBB * pObb, const _vec3 * pMin, const _vec3 * pMax)
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

void CMFC_Calculator::Set_Axis(OBB* pObb)
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

