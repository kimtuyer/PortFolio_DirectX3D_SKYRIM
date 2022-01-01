#ifndef NPC_h__
#define NPC_h__

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
class CNPC : public CGameObject
{
private:
	explicit CNPC(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh);
	explicit CNPC(const CNPC& rhs);
	virtual ~CNPC(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void	Set_Player(CGameObject* pObj) { m_pPlayer = pObj; }
	CGameObject* Get_Player() { return m_pPlayer; }

	void	Set_NPC_Type(int type) { m_iNPC_type = type; }
	int		Get_NPC_Type() { return m_iNPC_type; }


	void	Set_NPC_Name(wstring name) { m_wNPC_Name = name; }
	wstring	Get_NPC_Name() { return m_wNPC_Name; }


	vector<ItemInfo> Get_Inventory() { return m_vecInventory; }
	void			Set_Inventory(vector<ItemInfo> vecInven) {
		m_vecInventory = vecInven
			;
	}

	CColliderSphere*	Get_Collider() { return m_pColliderCom; }
	CTransform*			Get_Transform() {
		return m_pTransformCom
			;
	}
	CNaviMesh*			Get_NaviCom() {
		return  m_pNaviCom
			;
	}
	void				Set_RenderOn(bool check) { m_bRenderOn = check; }
	_bool				Get_RenderOn() { return m_bRenderOn; }

private:
	HRESULT					Add_Component(wstring Mesh);
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

	vector<ItemInfo>		m_vecInventory;

	int				m_iNPC_type = NPC_Type::NPC_END;
	wstring			m_wNPC_Name;
	_bool					m_bRenderOn = false;


public:
	static CNPC*		Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring Mesh);

private:
	virtual void		Free(void);
};


#endif // NPC_h__
