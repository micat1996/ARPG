// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PlayerInteractionComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UPlayerInteractionComponent : public USphereComponent
{
	GENERATED_BODY()
	
private :
	UPROPERTY()
	class ARPGCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<class AInteractableActor*> InteractableActors;

	// 상호작용 상태를 나타냅니다.
	UPROPERTY()
	bool bIsInteracting;

public :
	UPlayerInteractionComponent();

protected :
	virtual void BeginPlay() override;

public :
	virtual void TickComponent(float dt, ELevelTick tickType, FActorComponentTickFunction* thisTickFunc) override;

public :
	// 상호작용을 시도합니다.
	void TryInteraction();

private :
	// 상호작용 가능한 액터를 거리에 따라 정렬합니다.
	void SortByDistance();

	UFUNCTION()
	void OnInteractableActorDetected(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractableActorLose(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

};
