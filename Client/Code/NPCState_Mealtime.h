#ifndef NPCState_Mealtime_h__
#define NPCState_Mealtime_h__

#include "CNPC_State.h"


class NPCState_Mealtime: public CNPC_State
{
private:
	explicit NPCState_Mealtime();
	explicit NPCState_Mealtime(const NPCState_Mealtime& rhs);
	virtual ~NPCState_Mealtime(void);

public:
	virtual HRESULT Ready_State(CNPC& pNPC) ;
	virtual _int Update_State(CNPC& pNPC, const _float& fTimeDelta) ;
	virtual _int Change_State(CNPC& pPlayer);


private:
	

public:
	static NPCState_Mealtime*		Create();

private:
	virtual void		Free(void);
};


#endif // Monster_h__
