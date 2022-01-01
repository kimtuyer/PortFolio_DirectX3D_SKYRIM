#ifndef BarkeeperNPC_h__
#define BarkeeperNPC_h__

#include "GameObject.h"
#include "Define.h"
#include "Player.h"
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CColliderSphere;

END
class CBarkeeperNPC : public CGameObject
{
private:
	explicit CBarkeeperNPC(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBarkeeperNPC(const CBarkeeperNPC& rhs);
	virtual ~CBarkeeperNPC(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void	Set_Player(CGameObject* pObj) { m_pPlayer = pObj; }
	CGameObject* Get_Player() { return m_pPlayer; }
private:
	HRESULT					Add_Component(void);
	void					Key_Input(const _float& fTimeDelta);
	_float					SetUp_OnTerrain(void);
	_vec3					PickUp_OnTerrain(void);

private:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;

	_vec3					m_vDir;
	CGameObject*			m_pPlayer;

	const _matrix*			m_pBoneMatrix = nullptr;



public:
	static CBarkeeperNPC*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // BarkeeperNPC_h__
