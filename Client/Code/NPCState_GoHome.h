#ifndef NPCState_GoHome_h__
#define NPCState_GoHome_h__

#include "CNPC_State.h"

BEGIN(Engine)

class CTriCol;
class CTransform;

END
class NPCState_GoHome: public CNPC_State
{
private:
	explicit NPCState_GoHome();
	explicit NPCState_GoHome(const NPCState_GoHome& rhs);
	virtual ~NPCState_GoHome(void);

public:
	virtual HRESULT Ready_State(CNPC& pNPC) ;
	virtual _int Update_State(CNPC& pNPC, const _float& fTimeDelta) ;
	virtual _int Change_State(CNPC& pPlayer);

	static NPCState_GoHome*		Create();

private:
	

private:
	virtual void		Free(void);
};


#endif // Monster_h__
