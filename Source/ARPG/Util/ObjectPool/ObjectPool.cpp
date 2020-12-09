#include "ObjectPool.h"


IObjectPoolable* UObjectPool::GetRecycledObject()
{
	// 요소의 개수가 0 이라면 nullptr 리턴
	if (PoolObjects.Num() == 0) return nullptr;

	IObjectPoolable* poolableObj = nullptr;

	for (auto elem : PoolObjects)
	{
		// 재사용 가능한 객체를 찾았다면 poolableObj 에 저장합니다.
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
