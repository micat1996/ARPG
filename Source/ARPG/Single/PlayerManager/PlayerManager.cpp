#include "PlayerManager.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

void UPlayerManager::InitManagerClass()
{

}

ARPGPlayerController* UPlayerManager::GetPlayerController()
{
	if (!IsValid(PlayerController))
		PlayerController = Cast<ARPGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	return PlayerController;
}
