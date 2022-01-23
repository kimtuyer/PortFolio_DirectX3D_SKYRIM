#include "stdafx.h"
#include "NPCState_Battle.h"

#include "Export_Function.h"
#include "NPCState_Battle.h"

NPCState_Battle::NPCState_Battle()
{

}

NPCState_Battle::NPCState_Battle(const NPCState_Battle& rhs)
	
{

}

NPCState_Battle::~NPCState_Battle(void)
{

}

HRESULT NPCState_Battle::Ready_State(CNPC& pNPC)
{

	return S_OK;
}






NPCState_Battle* NPCState_Battle::Create()
{
	static NPCState_Battle	pInstance;

	

	return &pInstance;
}

void NPCState_Battle::Free(void)
{

	//CGameObject::Free();
}

_int NPCState_Battle::Update_State(CNPC& pNPC, const _float& fTimeDelta)
{
	return _int();
}

_int NPCState_Battle::Change_State(CNPC& pNPC)
{
	return _int();
}
