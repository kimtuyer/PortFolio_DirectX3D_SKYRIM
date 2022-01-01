#ifndef Mp_h__
#define Mp_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CMp : public CGameObject
{
private:
	explicit CMp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMp(const CMp& rhs);
	virtual ~CMp(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void Set_fX(float x) { m_fX += x; }

private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_matrix					m_matProj;

	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

public:
	static CMp*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Mp_h__
