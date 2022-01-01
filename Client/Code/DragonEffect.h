#ifndef DragonEffect_h__
#define DragonEffect_h__

#include "GameObject.h"
#include "Dragon.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CDynamicMesh;
END
class CDragonEffect : public CGameObject
{
private:
	explicit CDragonEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDragonEffect(const CDragonEffect& rhs);
	virtual ~CDragonEffect(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	CTransform*		Get_Transform() { return m_pTransformCom; }
	void			Set_DragonObject(CGameObject* p) { m_pDragon = p; }


	void		Set_RenderOn(_bool check) { m_bRenderOn = check; }
	void		Set_iIndex(int i) {m_iIndex = i; }
	int			Get_iIndex() { return m_iIndex; }


	void		Set_TextureIndex(int i) { m_itexIndex = i; }
	void		Set_Speed(float speed) { m_fSpeed = speed; }
	void		Set_BreathType(int i) { m_iBreathType = i; }
	void		Set_TargetPos(_vec3 vPos) { m_pTargetPos = vPos; }

	_vec3		Get_Collider() { return m_vCollider; }

	void		Set_m_IndexCount() { m_IndexCount = 0; }




private:
	HRESULT					Add_Component(void);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_float					m_fFrame = 0.f;
	_bool					m_bRenderOn = false;

	CGameObject*			m_pDragon = nullptr;
	CTransform*				pDragonCom = nullptr;
	CDynamicMesh*			pDragonMesh = nullptr;




	const _matrix*			m_pBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;
	int						m_iCount = 0;
	int						m_itexIndex = 0; //텍스처 번호
	_vec3					worldbone;
	_vec3					m_vOrgin;
	_vec3					m_vCollider;
	float					m_fDist;
	_bool					m_bReset = false;


	float					m_fScale = 1;
	float					m_fScaleSave = 0;
	float					m_fScaleWeight = 0.04;
	float					m_fSpeed=10.f;


	int						m_iBreathType = 0;
	int						m_iIndex = 0;//각 생성된이펙트 들의 인덱스!

	_vec3					m_pTargetPos;
	_vec3					m_vPos; //위치값 저장
	int						m_IndexCount = 0; //이펙트 초기 위치값 저장
	int						m_BlockCount = 0;
	int						m_iSetBlockIndex = 0;
	int						m_iSetIndexCount = 0;

public:
	static CDragonEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // DragonEffect_h__
