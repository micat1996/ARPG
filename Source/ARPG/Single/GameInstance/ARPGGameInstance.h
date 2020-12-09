#pragma once

#include "ARPG.h"
#include "Engine/GameInstance.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

//#include "Interfaces/ManagerClassBase.h"
#include "Single/ManagerClass/ManagerClass.h"

#include "ARPGGameInstance.generated.h"

UCLASS()
class ARPG_API UARPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

private :
	// ����� �Ŵ��� Ŭ���� �ν��Ͻ��� ������ �迭
	TArray<UManagerClass*> ManagerClasses;


private :
	// ManagerClass �� ��Ͻ�ŵ�ϴ�.
	void RegisterManagerClass(TSubclassOf<UManagerClass> managerClass);

	//template<typename T>
	//T* RegisterManagerClass()
	//{
	//	IManagerClassBase* managerClassInstance = NewObject<T>(this);
	//	managerClassInstance->InitManagerClass();
	//	ManagerClasses.Add(managerClassInstance);
	//
	//	return Cast<T>(managerClassInstance);
	//}

public :
	virtual void Init() override;

	// ����� ManagerClass �� ����ϴ�.
	template<typename ManagerClassType>
	ManagerClassType* GetManagerClass()
	{
		// �迭�� �߰��� ��ҵ��� Ȯ���մϴ�.
		for (int32 i = 0; i < ManagerClasses.Num(); ++i)
		{
			// �迭 ����� UClass �� ���޵� ManagerClassType �� ��ġ�ϴٸ�
			if (ManagerClasses[i]->GetClass() == ManagerClassType::StaticClass())

				// ã�� ��Ҹ� ��ȯ�մϴ�.
				return Cast<ManagerClassType>(ManagerClasses[i]);
		}

		// ã�� ���ߴٸ� nullptr ��ȯ
		return nullptr;
	}

public :
	// �ּ� �ε��� ���� StreamableManager �� ��� �޼���
	FORCEINLINE FStreamableManager* GetStreamableManager()
	{ return &UAssetManager::GetStreamableManager(); }


};
