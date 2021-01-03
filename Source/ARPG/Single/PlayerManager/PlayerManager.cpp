#include "PlayerManager.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"
#include "Single/GameInstance/ARPGGameInstance.h"

void UPlayerManager::InitManagerClass()
{

	Cast<UARPGGameInstance>(GetWorld()->GetGameInstance())->StartOpenLevel.AddLambda([this]() 
		{
			PlayerController = nullptr;
		});

}

FPlayerInfo* UPlayerManager::GetPlayerInfo()
{
	return &PlayerInfo;
}

ARPGPlayerController* UPlayerManager::GetPlayerController()
{

	if (!IsValid(PlayerController))
	{
		auto playerController = (UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (!IsValid(playerController)) return nullptr;
		else PlayerController = Cast<ARPGPlayerController>(playerController);
	}
		

	return PlayerController;
}

//void UPlayerManager::BeginDestroy()
//{
//	Super::BeginDestroy();
//
//	UE_LOG(LogTemp, Warning, TEXT("PlayerManager BeginDestroy()"));
//}
