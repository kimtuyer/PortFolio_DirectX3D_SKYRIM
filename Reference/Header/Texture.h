#ifndef Texture_h__
#define Texture_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture(void);

public:
	HRESULT			Ready_Texture(const _tchar* pPath, TEXTUREID eType, const _uint& iCnt);
	void			Render_Texture(const _uint& iIndex = 0);

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;	

	int									m_TextureIndex = 0;

public:
	void Set_TextureNumber(int index) { m_TextureIndex = index; }
	int Get_TextureIndex() { return m_TextureIndex; }

public:
	virtual CComponent*		Clone(void);
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _tchar* pPath, 
									TEXTUREID eType, 
									const _uint& iCnt = 1);
	virtual void			Free(void);
};

END
#endif // Texture_h__
