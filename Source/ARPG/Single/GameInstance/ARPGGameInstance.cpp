#include "ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"


void UARPGGameInstance::Init()
{
	Super::Init();

	ManagerClasses.Empty();

	// PlayerManager ��ü ���
	//RegisterManagerClass<UPlayerManager>();
	RegisterManagerClass(UPlayerManager::StaticClass());

	bIsLevelLoaded = true;
}

void UARPGGameInstance::Shutdown()
{
	for (int32 i = 0; i < ManagerClasses.Num(); ++i)
	{
		// nullptr ���� Ȯ��
		if (!ManagerClasses[i]) return;

		// �ش� ��ü�� GC�� ���� ������ ��ü���� Ȯ��
		if (!ManagerClasses[i]->IsValidLowLevel()) return;
		/// - IsValidLowLevel() : GC �� ���� �������� �ʾҴٸ� true ����

		// ��ü �Ҹ�
		ManagerClasses[i]->ConditionalBeginDestroy();
		/// - ConditionalBeginDestroy() : �ش� ��ü�� �Ҹ�ǵ��� �մϴ�.

		// ��۸� �����Ͱ� ���� �ʵ��� ��.
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