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
	// 등록한 매니저 클래스 인스턴스를 저장할 배열
	TArray<UManagerClass*> ManagerClasses;

	// 다음으로 로드할 레벨 이름을 저장합니다.
	UPROPERTY()
	FName NextLevelName;

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLevelLoaded;

private :
	// ManagerClass 를 등록시킵니다.
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

	// 등록한 ManagerClass 를 얻습니다.
	template<typename ManagerClassType>
	ManagerClassType* GetManagerClass()
	{
		// 배열에 추가된 요소들을 확인합니다.
		for (int32 i = 0; i < ManagerClasses.Num(); ++i)
		{
			// 배열 요소의 UClass 가 전달된 ManagerClassType 과 일치하다면
			if (ManagerClasses[i]->GetClass() == ManagerClassType::StaticClass())

				// 찾은 요소를 반환합니다.
				return Cast<ManagerClassType>(ManagerClasses[i]);
		}

		// 찾지 못했다면 nullptr 반환
		return nullptr;
	}

public :
	// 애셋 로딩을 위한 StreamableManager 를 얻는 메서드
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
