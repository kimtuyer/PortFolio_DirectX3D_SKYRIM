#ifndef MonsterHP_h__
#define MonsterHP_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CMonsterHP : public CGameObject
{
private:
	explicit CMonsterHP(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterHP(const CMonsterHP& rhs);
	virtual ~CMonsterHP(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void Set_fX(float x) { m_fX += x; }
	void Set_fX2(float x) { m_fX = x; }

	void	Set_SizeX(float x) { m_fSizeX += x; }
	void Set_RenderOn(_bool state) { m_bRenderOn = state; }

	void	Set_ID(int i) { m_ObjectID = i; }

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

	_bool					m_bRenderOn = false;
	int						m_ObjectID = 0;

public:
	static CMonsterHP*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // MonsterHP_h__
