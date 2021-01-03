#pragma once

#include "ARPG.h"
#include "Engine/GameInstance.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

//#include "Interfaces/ManagerClassBase.h"
#include "Single/ManagerClass/ManagerClass.h"

#include "ARPGGameInstance.generated.h"

#ifndef GAME_INSTANCE
#define GAME_INSTANCE
#define GetGameInst() (Cast<UARPGGameInstance>(GetWorld()->GetGameInstance()))
#endif

DECLARE_MULTICAST_DELEGATE(FOpenLevelEvent)


UCLASS()
class ARPG_API UARPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public :
	FOpenLevelEvent StartOpenLevel;

private :
	// ����� �Ŵ��� Ŭ���� �ν��Ͻ��� ������ �迭
	TArray<UManagerClass*> ManagerClasses;

	// �������� �ε��� ���� �̸��� �����մϴ�.
	UPROPERTY()
	FName NextLevelName;

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLevelLoaded;

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

	virtual void Shutdown() override;

	virtual void BeginDestroy() override;

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

	FORCEINLINE void SetNextLevelName(FName nextLevelName)
	{ NextLevelName = nextLevelName; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetNextLevelName() const
	{ return NextLevelName; }

	UFUNCTION(BlueprintCallable)
	void BroadcastOpenLevelEvent();
};
