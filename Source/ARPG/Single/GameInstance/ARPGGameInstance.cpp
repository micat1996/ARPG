#include "ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"


void UARPGGameInstance::Init()
{
	Super::Init();

	ManagerClasses.Empty();

	// PlayerManager °´Ã¼ µî·Ï
	//RegisterManagerClass<UPlayerManager>();
	RegisterManagerClass(UPlayerManager::StaticClass());
}

void UARPGGameInstance::RegisterManagerClass(TSubclassOf<UManagerClass> managerClass)
{
	UManagerClass* managerClassInstance = NewObject<UManagerClass>(this, managerClass);
	managerClassInstance->InitManagerClass();
	ManagerClasses.Add(managerClassInstance);
}