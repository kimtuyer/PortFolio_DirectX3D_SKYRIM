#ifndef UI_h__
#define UI_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUI : public CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);
	explicit CUI(const CUI& rhs);
	virtual ~CUI(void);

public:
	HRESULT Ready_CUI_Object(wstring Mesh_proto);


	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	void Set_fX(float x) { m_fX += x; }
	void Set_fX2(float x) { m_fX = x; }

	void Set_fY(float Y) { m_fY = Y; }

	float Get_fY() { return m_fY; }


	void Set_scaleX(float x) { m_fScalex = x; }
	void Set_scaleY(float Y) { m_fScaley = Y; }



	void Set_SizeX(float x) { m_fSizeX = x; }
	void Set_SizeY(float Y) { m_fSizeY = Y; }


	void Set_RenderOn(_bool state) { m_bRenderOn = state; }
	_bool Get_RenderOn() {
		return m_bRenderOn
			;
	}

	void	Set_ItemType(int i) { m_iItemType = i; }
	int		Get_ItemType() { return m_iItemType; }


	void	Set_TexIndex(int i) { m_iTexIndex = i; }



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

	int						m_iTexIndex = 0;


	float					m_fScalex, m_fScaley;

	_bool					m_bRenderOn = true;


	int						m_iItemType = ITEM_Type::ITEM_END; // 해당 ui 가 아이템장착창 가리키는 텍스처일 경우, 아이템타입 부위 식별

public:
	static CUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);

private:
	virtual void		Free(void);
};


#endif // UI_h__
