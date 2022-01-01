#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{

}

//HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pInstance)
//{
//	if (nullptr == pInstance)
//		return E_FAIL;
//
//	m_mapObject.emplace(pObjTag, pInstance);
//
//	return S_OK;
//}

HRESULT CLayer::Add_UIInvenObject(const UI_INVENTORY_ID pObjTag, CGameObject * pInstance)
{
	if (pObjTag >= UI_INVENTORY_END || nullptr == pInstance)
		return E_FAIL;

	m_listUI_Inventory[pObjTag].emplace_back(pInstance);
	return S_OK;
}

HRESULT Engine::CLayer::Add_GameObject(const OBJECT_ID pObjTag, CGameObject * pInstance)
{
	if (pObjTag >= OBJECT_END || nullptr == pInstance)
		return E_FAIL;

	m_listGameObject[pObjTag].emplace_back(pInstance);
	return S_OK;
}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult = 0;

	//for (auto& iter : m_mapObject)
	//{
	//	iResult = iter.second->Update_Object(fTimeDelta);
	//
	//	if (iResult & 0x8000000)
	//		return iResult;
	//}

	for (int i = 0; i < OBJECT_END; ++i)
	{
		for (auto& iter = m_listGameObject[i].begin(); iter != m_listGameObject[i].end();)
		{
			if ((*iter)->Get_State() == 1 || (*iter)->Get_HP()==0)
			{
				Safe_Release(*iter);
				m_listGameObject[i].erase(iter++);
			}
			else
			{
			iResult = (*iter)->Update_Object(fTimeDelta);



			
			if (iResult == -1)
			{
				Safe_Release(*iter);
				m_listGameObject[i].erase(iter++);

			}
			else
			{
			++iter;

			}


			}


			//{

			//if (OBJ_DEAD == iEvent)
			//{

		}
	}







	return iResult;
}

_int Engine:: CLayer::Update_UI_Inven_Layer(const _float & fTimeDelta)
{
	_int	iResult = 0;

	

	for (int i = 0; i < UI_INVENTORY_END; ++i)
	{
		for (auto& iter = m_listUI_Inventory[i].begin(); iter != m_listUI_Inventory[i].end();)
		{
			if ((*iter)->Get_State() == 1 || (*iter)->Get_HP() == 0)
			{
				Safe_Release(*iter);
				m_listUI_Inventory[i].erase(iter++);
			}
			else
			{
				iResult = (*iter)->Update_Object(fTimeDelta);

				if (iResult == -1)
				{
					Safe_Release(*iter);
					m_listUI_Inventory[i].erase(iter++);

				}
				else
				{
					++iter;

				}


			}


			//{

			//if (OBJ_DEAD == iEvent)
			//{

		}
	}







	return iResult;
}

CLayer* Engine::CLayer::Create(void)
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}

CComponent* Engine::CLayer::Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);


	

}

CComponent * CLayer::Get_Component(const OBJECT_ID pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	return m_listGameObject[pObjTag].front()->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_Object(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

CGameObject * CLayer::Get_Object(const OBJECT_ID pObjTag)
{
	if(!m_listGameObject[pObjTag].empty())
		return m_listGameObject[pObjTag].front();

}

CGameObject * CLayer::Get_UI_InvenObject(const int UI_INVENTORY_Index)
{
	if (!m_listUI_Inventory[UI_INVENTORY_Index].empty())
		return m_listUI_Inventory[UI_INVENTORY_Index].front();
}


