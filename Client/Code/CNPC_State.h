#ifndef NPC_State_h__
#define NPC_State_h__

#include "GameObject.h"
#include "Export_Function.h"

class CNPC;
class NPCState_Battle;
class NPCState_Normal;
class NPCState_Mealtime;
class NPCState_GoHome;



class CNPC_State
{
public:
	explicit CNPC_State();
	explicit CNPC_State(const CNPC_State& rhs);
	virtual ~CNPC_State(void);

public:
	virtual HRESULT Ready_State(CNPC& pNPC) ;
	virtual _int Update_State(CNPC& pNPC, const _float& fTimeDelta) ;
	virtual _int Change_State(CNPC& pNPC);

	//static		NPCState_Battle		battle;
	//static		NPCState_Normal		normal;
	//static		NPCState_Mealtime	mealtime;
	//static		NPCState_GoHome		gohome;
	

protected:
	float			m_fTimeCycle = 5.f;
	float			m_fTime = 0.f;

//public:
//	static CNPC_State*		Create();

private:
	virtual void		Free(void);
};


#endif // Monster_h__
