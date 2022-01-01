#ifndef Sp_h__
#define Sp_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CSp : public CGameObject
{
private:
	explicit CSp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSp(const CSp& rhs);
	virtual ~CSp(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void Set_fX(float x) { m_fX += x; }
	void Set_RenderOn(_bool check) { m_bRender = check; }
	_bool	Get_RenderOn() { return m_bRender; }

	void	Set_Id(int i) { m_Id = i; }
	int		Get_ID() { return m_Id; }
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

	_bool					m_bRender = true;
	int						m_Id = 0;

public:
	static CSp*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Sp_h__
