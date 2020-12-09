#include "PlayerDetectorComponent.h"
#include "../../Actors/Characters/RPGCharacter/RPGCharacter.h"

UPlayerDetectorComponent::UPlayerDetectorComponent()
{
	SphereRadius = 200.0f;
}

void UPlayerDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPlayerDetectorComponent::OnPlayerDetected);
	OnComponentEndOverlap.AddDynamic(this, &UPlayerDetectorComponent::OnPlayerLost);
}

void UPlayerDetectorComponent::OnPlayerDetected(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (Cast<ARPGCharacter>(OtherActor))
		bDetectPlayer = true;
}

void UPlayerDetectorComponent::OnPlayerLost(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	if (Cast<ARPGCharacter>(OtherActor))
		bDetectPlayer = false;
}
