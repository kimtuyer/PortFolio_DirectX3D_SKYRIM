#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);

public:
	CComponent*		Get_Component(const _tchar* pObjTag,
								  const _tchar* pComponentTag,
								  COMPONENTID eID);

	CComponent*		Get_Component(const OBJECT_ID pObjTag,
		const _tchar* pComponentTag,
		COMPONENTID eID);


	CGameObject* Get_Object(const _tchar* pObjTag);

	CGameObject* Get_Object(const OBJECT_ID pObjTag);

	CGameObject* Get_UI_InvenObject(const int UI_INVENTORY_Index);


	list<CGameObject*> Get_ObjectList(const OBJECT_ID pObjTag)
	{
		return m_listGameObject[pObjTag];
	}

	list<CGameObject*> Get_UI_InvenList(const UI_INVENTORY_ID pObjTag)
	{
		return m_listUI_Inventory[pObjTag];
	}

	

public:
	HRESULT		Add_UIInvenObject(const UI_INVENTORY_ID pObjTag, CGameObject* pInstance);
	HRESULT		Add_GameObject(const OBJECT_ID pObjTag, CGameObject* pInstance);


	HRESULT		Ready_Layer(void);
	_int		Update_Layer(const _float& fTimeDelta);
	_int		Update_UI_Inven_Layer(const _float& fTimeDelta);




	void Set_LayerName(wstring name) { layerName = name; }
	wstring	Get_LayerName() { return layerName; }

private:
	multimap<const _tchar*, CGameObject*>		m_mapObject;


	list<CGameObject*> m_listGameObject[OBJECT_END];

	list<CGameObject*> m_listUI_Inventory[UI_INVENTORY_END];


	
	vector<CGameObject*> m_vecObject;
	
	wstring	layerName;

	//map<const _tchar*, vector<CGameObject*>>		m_mapObject;


public:
	static CLayer*		Create(void);
	virtual void		Free(void);

};


END
#endif // Layer_h__
