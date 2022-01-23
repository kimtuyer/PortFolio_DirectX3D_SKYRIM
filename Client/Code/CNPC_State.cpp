#include "stdafx.h"
#include "CNPC_State.h"
#include "NPC.h"
//#include "Export_Function.h"

CNPC_State::CNPC_State()
{

}

CNPC_State::CNPC_State(const CNPC_State& rhs)
	
{

}

CNPC_State::~CNPC_State(void)
{

}

HRESULT CNPC_State::Ready_State(CNPC& pNPC)
{




	return S_OK;
}

Engine::_int CNPC_State::Update_State(CNPC& pNPC, const _float& fTimeDelta)
{

	return 0;
}

_int CNPC_State::Change_State(CNPC& pNPC)
{
	return _int();
}



//CNPC_State* CNPC_State::Create()
//{
//	CNPC_State*	pInstance = new CNPC_State();
//
//	if (FAILED(pInstance->Ready_State()))
//		Safe_Release(pInstance);
//
//	return pInstance;
//}

void CNPC_State::Free(void)
{
	//CGameObject::Free();
}

