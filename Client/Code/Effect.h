#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"
#include"Player.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CEffect : public CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	CTransform*		Get_Transform() { return m_pTransformCom; }

	void		Set_RenderOn(_bool check) { m_bRenderOn = check; }
	void		Set_FrameSize(float size) { m_fFrameSize = size; }

	void		Set_ID(int i) { m_ID = i; }	
	void		Set_Orgin(float x, float y, float z) { vOrgin.x = x;
	
	vOrgin.y = y; vOrgin.z = z;
	}
private:
	HRESULT					Add_Component(wstring Mesh);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_float					m_fFrame = 0.f;
	_float					m_fFrameSize = 4.f;
	_bool					m_bRenderOn = false;
	_bool					m_bRender2 = false;

	int						m_ID = 0;
	_vec3					vOrgin;

public:
	static CEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh);

private:
	virtual void		Free(void);
};


#endif // Effect_h__
