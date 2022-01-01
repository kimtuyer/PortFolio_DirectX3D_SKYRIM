#include "pch.h"
#include "MFC_TerrainTex.h"
USING(Engine)

MFC_TerrainTex::MFC_TerrainTex(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

MFC_TerrainTex::MFC_TerrainTex(const MFC_TerrainTex & rhs) : CVIBuffer(rhs)
, m_hFile(rhs.m_hFile)
, m_fH(rhs.m_fH)
, m_iH(rhs.m_iH)
, m_pPos(rhs.m_pPos)
{
}

MFC_TerrainTex::~MFC_TerrainTex(void)
{
}

void MFC_TerrainTex::Copy_Index(INDEX32 * pIndex, const _ulong & dwTriCnt)
{
	INDEX32*		pOriginIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriginIndex, 0);

	memcpy(pOriginIndex, pIndex, sizeof(INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

HRESULT MFC_TerrainTex::Ready_Buffer(const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwFVF = FVF_TEX;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	m_pPos = new _vec3[m_dwVtxCnt];


	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	_ulong		dwByte = 0;

	m_hFile = CreateFile(L"../Resource/Texture/Terrain/TestHeight.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, NULL);

	CloseHandle(m_hFile);

	VTXTEX*			pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong		dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPos = _vec3(_float(j * dwVtxItv),
				(pPixel[dwIndex] & 0x000000ff) / 20.f,
				_float(i * dwVtxItv));

			//pVertex[dwIndex].vPos.y = 5.f;		


			pVertex[dwIndex].vTexUV = _vec2((_float(j) / (dwCntX - 1)) * 20.f,
				(_float(i) / (dwCntZ - 1)) * 20.f);		

			m_pPos[dwIndex] = pVertex[dwIndex].vPos;
		}
	}


	m_pVB->Unlock();


	INDEX32*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong	dwTriIdx = 0;

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;
			dwTriIdx++;

			// 왼쪽 아래
			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;
			dwTriIdx++;
		}
	}

	m_pIB->Unlock();


	return S_OK;
}

void MFC_TerrainTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

MFC_TerrainTex * MFC_TerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	MFC_TerrainTex*		pInstance = new MFC_TerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * MFC_TerrainTex::Clone(void)
{
	return new MFC_TerrainTex(*this);
}

void MFC_TerrainTex::Free(void)
{
	CVIBuffer::Free();

	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);
}
