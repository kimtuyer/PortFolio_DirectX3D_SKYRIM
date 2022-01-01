#ifndef Base_h__
#define Base_h__

// ���۷��� ī��Ʈ(���� ī��Ʈ)

// ���θ� ������ ���, ī��Ʈ�� �ø���, ������ �����͸� �� ������� ��� ī��Ʈ�� ������ ����
// ���۷��� ī��Ʈ�� 0�� ���� ��쿡�� �����͸� ����(delete)

// Ŭ���� ������(dll ������Ʈ���� ���� ����� Ŭ������ �ڵ� ������ �����ϱ� ���� �뵵)
class _declspec(dllexport) CBase 
{
protected:
	inline explicit CBase(void);
	inline virtual ~CBase(void);

public:
	inline	unsigned long	AddRef(void);
	inline	unsigned long	Release(void);

private:
	unsigned long			m_dwRefCnt;

public:
	inline virtual void Free(void) = 0; // ���� �����Լ�
};

#include "Base.inl"

#endif // Base_h__


// ��������  : �ܺο����� ���ٰ����� ���� ����  (���� ���� : ���� �ϳ��� ����)
// ��������(static) : ���ο����� ���ٰ����� ���� ����(���� ���� : Ŭ����, �Լ� ����)



