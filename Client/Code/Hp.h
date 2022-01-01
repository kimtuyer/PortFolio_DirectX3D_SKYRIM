#ifndef Hp_h__
#define Hp_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CHp : public CGameObject
{
private:
	explicit CHp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHp(const CHp& rhs);
	virtual ~CHp(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void Set_fX(float x) { m_fX += x; }
	void Set_RenderOn(_bool check) { m_bRender = check; }
	_bool	Get_RenderOn() { return m_bRender; }
private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_matrix					m_matProj;
	_bool					m_bRender = true;

	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

public:
	static CHp*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Hp_h__
