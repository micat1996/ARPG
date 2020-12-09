#include "ObjectPool.h"


IObjectPoolable* UObjectPool::GetRecycledObject()
{
	// ����� ������ 0 �̶�� nullptr ����
	if (PoolObjects.Num() == 0) return nullptr;

	IObjectPoolable* poolableObj = nullptr;

	for (auto elem : PoolObjects)
	{
		// ���� ������ ��ü�� ã�Ҵٸ� poolableObj �� �����մϴ�.
		if (elem->GetCanRecyclable())
		{
			poolableObj = elem;
			break;
		}
	}

	if (poolableObj == nullptr)
		return nullptr;

	poolableObj->SetCanRecyclable(false);
	poolableObj->OnRecycleStart();

	return poolableObj;
}
