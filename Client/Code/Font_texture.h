#ifndef Font_Texture_h__
#define Font_Texture_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CFont_Texture : public CGameObject
{
private:
	explicit CFont_Texture(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);
	explicit CFont_Texture(const CFont_Texture& rhs);
	virtual ~CFont_Texture(void);

public:
	HRESULT Ready_CFont_Texture_Object(wstring Mesh_proto);


	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	void Set_fX(float x) { m_fX += x; }
	void Set_fY(float Y) { m_fY = Y; }

	float Get_fY() { return m_fY; }


	void Set_scaleX(float x) { m_fScalex = x; }
	void Set_scaleY(float Y) { m_fScaley = Y; }



	void Set_SizeX(float x) { m_fSizeX = x; }
	void Set_SizeY(float Y) { m_fSizeY = Y; }

	CTexture*	Get_Texture() { return m_pTextureCom; }
	
	void Set_TextureIndex(int i) {
		m_iTextureIndex = i;
	}
	int	Get_TextureIndex() { return m_iTextureIndex; }

	void Set_RenderOn(_bool state) { m_bRenderOn = state; }

private:
	HRESULT					Add_Component(wstring Mesh);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_matrix					m_matProj;

	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

	float					m_fScalex, m_fScaley;
	int						m_iTextureIndex = 0;


	_bool					m_bRenderOn = true;

public:
	static CFont_Texture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);

private:
	virtual void		Free(void);
};


#endif // Font_Texture_h__
