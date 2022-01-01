#ifndef Monster_draugr_h__
#define Monster_draugr_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;

END
class CMonster_draugr : public CGameObject
{
private:
	explicit CMonster_draugr(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster_draugr(const CMonster_draugr& rhs);
	virtual ~CMonster_draugr(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);
	void					Key_Input(const _float& fTimeDelta);
	void					SetUp_OnTerrain(void);
	_vec3					PickUp_OnTerrain(void);

private:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	_vec3					m_vDir;

public:
	static CMonster_draugr*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Monster_draugr_h__
