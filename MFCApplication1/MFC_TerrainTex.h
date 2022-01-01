#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

class MFC_TerrainTex : public CVIBuffer
{
private:
	explicit MFC_TerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit MFC_TerrainTex(const MFC_TerrainTex& rhs);
	virtual ~MFC_TerrainTex(void);

public:
	const _vec3*			Get_VtxPos(void) const { return m_pPos; }
	 _vec3*			MFC_Get_VtxPos(void)  { return m_pPos; }
	 void			MFC_Set_VtxPos(_vec3* vPos) {



		 int index = (m_iH.biHeight - 1)*m_iH.biWidth + (m_iH.biWidth - 1);
		// memcpy(m_pPos, vPos, sizeof(_vec3)*index);
	
		 for (int i = 0; i < index; i++)
		 {
		
		 m_pPos[i] = vPos[i];				}
		
		 }


	 LPDIRECT3DVERTEXBUFFER9	Get_VB() {
		 return m_pVB
			 ;
	 }



	// 이 함수는 높이맵의 해상도와 정점의 개수가 일치하다는 전제 하에 사용하는 코드
	_ulong			Get_VtxCntX(void) const { return m_iH.biWidth; }
	_ulong			Get_VtxCntZ(void) const { return m_iH.biHeight; }

	void			Copy_Index(INDEX32* pIndex, const _ulong& dwTriCnt);

	void Set_VtxCntX(_ulong cntx) { m_iH.biWidth = cntx; }
	void Set_VtxCntZ(_ulong cntz) { m_iH.biHeight = cntz; }

	//나
	
	BITMAPFILEHEADER Get_fH() {
		return m_fH
			;
	}

	BITMAPINFOHEADER Get_iH()
	{
		return m_iH;
	}
	_ulong*		Get_Pixel() {
		return pPixel
			;
	}
	
		//int Update(_ulong cntx, _ulong cntz)
	//{
	//
	//}

public:
	HRESULT Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void Render_Buffer(void) override;

private:
	HANDLE					m_hFile;

	_ulong*		pPixel; //나 수정
	BITMAPFILEHEADER		m_fH;
	BITMAPINFOHEADER		m_iH;
	_vec3*					m_pPos = nullptr;

public:
	static MFC_TerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv = 1);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void) override;

};

END
