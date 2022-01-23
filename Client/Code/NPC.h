#ifndef NPC_h__
#define NPC_h__

#include "GameObject.h"
#include "Define.h"
#include "Player.h"
#include "CNPC_State.h"
#include "NPCState_Mealtime.h"
#include "NPCState_Normal.h"
#include "NPCState_GoHome.h"
#include "NPCState_Battle.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CColliderSphere;
class COptimization;

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

	void	Set_PathList(_vec3 vDestPos, const _float& fTimeDelta);

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

	
	 _vec3				Get_HomeLoc() { return m_vHomeLoc; }
	 _vec3				Get_BarLoc() { return m_vBarLoc; }
	 _vec3			   Get_StartLoc() { return m_vStartLoc; }

	void				Set_HomeLoc(float x, float y, float z) { m_vHomeLoc = {x,y,z}; }
	void				Set_HomeIndex(int index) { m_iHomeIndex = index; }

	void				Set_StartLoc(float x, float y, float z) { m_vStartLoc = { x,y,z }; }
	void				Set_StartIndex(int index) { m_iStartIndex = index; }



	CNPC_State*			m_pState = nullptr;
	void				ChangeState(CNPC_State* new_state);

	queue<_vec3>*		Get_PathList() { return &m_sPathlist; }
	void				Insert_Pathlist(_vec3& vPos) { m_sPathlist.push(vPos); }

	bool				Get_FindPath() { return m_bPath; }
	void				Set_FindPath() { m_bPath = false; }
	_float					SetUp_OnTerrain(void);





private:
	HRESULT					Add_Component(wstring Mesh);
	void					Key_Input(const _float& fTimeDelta);
	_vec3					PickUp_OnTerrain(void);

private:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;
	COptimization*			m_pOptimizationCom = nullptr;

	_vec3					m_vDir;
	CGameObject*			m_pPlayer;

	const _matrix*			m_pBoneMatrix = nullptr;

	vector<ItemInfo>		m_vecInventory;

	int				m_iNPC_type = NPC_Type::NPC_END;
	wstring			m_wNPC_Name;

	_bool					m_bRenderOn = false;
	_bool					m_bDraw = false;


	bool					m_bPath = false;
	_vec3					m_vBarLoc = { 67.15f,7.f,30.4f };
	_int					m_iBarIndex = 35;

	_vec3					m_vHomeLoc = {};
	_int					m_iHomeIndex = 0;

	_vec3					m_vStartLoc = {};
	_int					m_iStartIndex = 0;

	float					m_fAniTime =0.f;

	queue<_vec3>			m_sPathlist;

public:
	static CNPC*		Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring Mesh);

private:
	virtual void		Free(void);
};


#endif // NPC_h__
