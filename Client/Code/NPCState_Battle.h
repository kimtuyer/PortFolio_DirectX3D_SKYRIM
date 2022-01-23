#ifndef NPCState_Battle_h__
#define NPCState_Battle_h__

#include "CNPC_State.h"

BEGIN(Engine)

class CTriCol;
class CTransform;

END
class NPCState_Battle: public CNPC_State
{
private:
	explicit NPCState_Battle();
	explicit NPCState_Battle(const NPCState_Battle& rhs);
	virtual ~NPCState_Battle(void);

public:
	virtual HRESULT Ready_State(CNPC& pNPC) ;
	virtual _int Update_State(CNPC& pNPC, const _float& fTimeDelta) ;
	virtual _int Change_State(CNPC& pNPC);



private:
	

public:
	static NPCState_Battle*		Create();

private:
	virtual void		Free(void);
};


#endif // Monster_h__
