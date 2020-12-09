// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PlayerDetectorComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UPlayerDetectorComponent : public USphereComponent
{
	GENERATED_BODY()

private :
	// 플레이어 감지 여부를 나타냅니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bDetectPlayer;

public :
	UPlayerDetectorComponent();

	virtual void BeginPlay() override;


private :
	UFUNCTION()
	void OnPlayerDetected(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnPlayerLost(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

public :
	// 플레이어 감지 여부를 반환합니다.
	FORCEINLINE bool IsPlayerDetected() const
	{ return bDetectPlayer; }

};
