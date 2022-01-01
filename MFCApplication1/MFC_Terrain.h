#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;
class CRenderer;
class MFC_TerrainTex;

END


class MFC_Terrain : public CGameObject
{
private:
	explicit MFC_Terrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit MFC_Terrain(const MFC_Terrain& rhs);
	virtual ~MFC_Terrain(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	COptimization* Get_COptimization() {
		return m_pOptimizationCom
			;
	}
	MFC_TerrainTex* Get_Buffer() {
		return m_pBufferCom;
	}


	_ulong* Get_TriCnt() { return &m_dwTriCnt; }

	void	Set_index(_ulong index) { m_iIndex = index; }


private:
	HRESULT					Add_Component(void);
	HRESULT					SetUp_Material(void);

private:
	MFC_TerrainTex*					m_pBufferCom = nullptr;

	CTerrainTex*					m_pBufferCom2 = nullptr;


	CTexture*					m_pTextureCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	COptimization*				m_pOptimizationCom = nullptr;

	INDEX32*					m_pIndex = nullptr;
	_ulong						m_dwTriCnt = 0;
	_ulong						m_iIndex = 0;

public:
	static MFC_Terrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};

