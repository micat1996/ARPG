#include "ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"


void UARPGGameInstance::Init()
{
	Super::Init();

	ManagerClasses.Empty();

	// PlayerManager 객체 등록
	//RegisterManagerClass<UPlayerManager>();
	RegisterManagerClass(UPlayerManager::StaticClass());

	bIsLevelLoaded = true;
}

void UARPGGameInstance::Shutdown()
{
	for (int32 i = 0; i < ManagerClasses.Num(); ++i)
	{
		// nullptr 인지 확인
		if (!ManagerClasses[i]) return;

		// 해당 객체가 GC에 의해 해제된 객체인지 확인
		if (!ManagerClasses[i]->IsValidLowLevel()) return;
		/// - IsValidLowLevel() : GC 에 의해 해제되지 않았다면 true 리턴

		// 객체 소멸
		ManagerClasses[i]->ConditionalBeginDestroy();
		/// - ConditionalBeginDestroy() : 해당 객체가 소멸되도록 합니다.

		// 댕글링 포인터가 되지 않도록 함.
		ManagerClasses[i] = nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("Shut Down"));
	Super::Shutdown();

}

void UARPGGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
}

void UARPGGameInstance::BroadcastOpenLevelEvent()
{
	if (StartOpenLevel.IsBound())
	{
		StartOpenLevel.Broadcast();
	}
}


void UARPGGameInstance::RegisterManagerClass(TSubclassOf<UManagerClass> managerClass)
{
	UManagerClass* managerClassInstance = NewObject<UManagerClass>(
		this, managerClass, managerClass->GetClass()->GetFName(), EObjectFlags::RF_MarkAsRootSet);

	managerClassInstance->InitManagerClass();
	ManagerClasses.Add(managerClassInstance);
}