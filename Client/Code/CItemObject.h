#pragma once
//#include "Engine_Struct.h"

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CTexture;
class CRcTex;
END
		



class CItemObject : public CGameObject
{


private:
	explicit CItemObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);
	//explicit CItemObject(LPDIRECT3DDEVICE9 pGraphicDev);

	explicit CItemObject(const CItemObject& rhs);
	virtual ~CItemObject(void);

public:
	 HRESULT Ready_CItemObject(wstring Mesh_proto);
	virtual HRESULT Ready_Object(void) override;

	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	HRESULT Add_Component(wstring Mesh_proto);
	void Release_CItemObject();


	void Set_fX(float x) { m_fX += x; }

	void Set_fX_Item(float x) { m_fX = x; }

	void Set_fY(float Y) { m_fY = Y; }

	float Get_fY() { return m_fY; }


	void Set_scaleX(float x) { m_fScalex = x; }
	void Set_scaleY(float Y) { m_fScaley = Y; }



	void Set_SizeX(float x) { m_fSizeX = x; }
	void Set_SizeY(float Y) { m_fSizeY = Y; }

	void Set_TextureIndex(int i) {
		m_iTextureIndex = i;
	}
	int	Get_TextureIndex() { return m_iTextureIndex; }

	void Set_RenderOn(_bool state) { m_bRenderOn = state; }

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	//CGameObject*			m_pPlayer;
	_matrix					m_matProj;

	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

	float					m_fScalex, m_fScaley;

	int					m_iTextureIndex=0;

	_bool				m_bRenderOn = false;



private:
	virtual void		Free(void);

public:

	 static CItemObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);


private:


	//vector<ItemObjectInfo>	m_vecItemObjectList;
};

