#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene(void);

public:
	CComponent*		Get_Component(const _tchar* pLayerTag,
								const OBJECT_ID pObjTag,
								const _tchar* pComponentTag,
								COMPONENTID eID);

	CGameObject*		Get_Object(const _tchar* pLayerTag,
		const OBJECT_ID pObjTag);


	CLayer* Get_Layer(const _tchar* pLayerTag)
	{
		auto		iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

		if (iter == m_mapLayer.end())
			return nullptr;


		return iter->second;
	}

public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void) = 0;
protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<const _tchar*, CLayer*>		m_mapLayer;




public:
	virtual void Free(void);
};

END
#endif // Scene_h__
