#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)


class CMFC_Calculator;
class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class COptimization;






class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject(void);

public:
	CComponent*		Get_Component(const _tchar* pComponentTag, COMPONENTID eID);

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		Render_Object(void);

	int Get_State() { return m_State; }

	void Set_State(int i) { m_State = i; }


	int Get_ActionState() { return m_Action_State; }

	void Set_ActionState(int Current_State) { m_Action_State = Current_State; }

	int Get_HP() { return m_iHP; }

	void Set_HP(int i) { m_iHP = i; }
		
	void Set_HPtoDamage(int iAttack) {

		m_iHP -= iAttack;

		if (m_iHP < 0)
			m_iHP = 0;

	}


	int Get_AttackStat() { return m_iAttack; }

	void Set_AttackStat(int i) { m_iAttack = i; }

	void Set_AttackStat_Change(int i) { m_iAttack += i; }


protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];

	int m_State = 0;

	int m_Action_State = CURRENT_IDLE;
	int	m_iHP = 0;
	int m_iAttack = 0;

	_float								m_fViewZ;

public:

//	HRESULT					Add_Component(void);
//	void					SetUp_OnTerrain(void);
//	_bool					Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag);
public:

	_float			Get_ViewZ(void) { return m_fViewZ; }

	void			Compute_ViewZ(const _vec3* pPos);



private:
	CComponent*			Find_Component(const _tchar* pComponentTag, COMPONENTID eID);



	//CStaticMesh*			m_pMeshCom = nullptr;
	//CTransform*				m_pTransformCom = nullptr;
	//CRenderer*				m_pRendererCom = nullptr;
	//CCalculator*		m_pCalculatorCom = nullptr;
	//CCollider*				m_pColliderCom = nullptr;
	//COptimization*			m_pOptimizationCom = nullptr;
	//
	//_bool					m_bColl = false;
	//_uint					m_iFlag;
	//_bool					m_bDraw = false;

public:
	virtual void	Free(void);

};

END
#endif // GameObject_h__
