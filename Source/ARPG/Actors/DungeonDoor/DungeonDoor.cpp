#include "DungeonDoor.h"

#include "Single/PlayerManager/PlayerManager.h"
#include "Single/GameInstance/ARPGGameInstance.h"

#include "Actors/Characters/RPGCharacter/RPGCharacter.h"
#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

ADungeonDoor::ADungeonDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX_TRIGGER"));
	SetRootComponent(Box);

	Box->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ADungeonDoor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

	Box->OnComponentBeginOverlap.AddDynamic(this, &ADungeonDoor::OnDoorOverlap);
	
}

void ADungeonDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonDoor::OnDoorOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		ARPGCharacter* playerCharacter = Cast<ARPGCharacter>(GetManager(UPlayerManager)->
			GetPlayerController()->GetPawn());

		if (playerCharacter->GetCapsuleComponent() == OtherComp)
		{

			OtherActor->Destroy();


			//Cast<UARPGGameInstance>(GetGameInstance())->BroadcastOpenLevelEvent();
			Cast<UARPGGameInstance>(GetGameInstance())->bIsLevelLoaded = false;
			Cast<UARPGGameInstance>(GetGameInstance())->SetNextLevelName(FName(TEXT("TestDungeon")));
			UGameplayStatics::OpenLevel(this, FName(TEXT("LoadingLevel")));
		}
	}
}
