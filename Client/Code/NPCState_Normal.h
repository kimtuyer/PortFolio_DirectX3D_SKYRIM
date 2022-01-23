#ifndef NPCState_Normal_h__
#define NPCState_Normal_h__

#include "CNPC_State.h"

BEGIN(Engine)

class CTriCol;
class CTransform;

END
class NPCState_Normal: public CNPC_State
{
private:
	explicit NPCState_Normal();
	explicit NPCState_Normal(const NPCState_Normal& rhs);
	virtual ~NPCState_Normal(void);

public:
	virtual HRESULT Ready_State(CNPC& pNPC) ;
	virtual _int Update_State(CNPC& pNPC, const _float& fTimeDelta) ;
	virtual _int Change_State(CNPC& pPlayer);


private:

public:
	static NPCState_Normal*		Create();

private:
	virtual void		Free(void);
};


#endif // Monster_h__
