// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class AEnemyCharacter * Owner;

	// 속도 길이를 나타냅니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float VelocityLength;

public :
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private :
	UFUNCTION()
	void AnimNotify_EnemyAttackFin();

	UFUNCTION()
	void AnimNotify_UpdateRotation();

	UFUNCTION()
	void AnimNotify_ActiveAttackRange();

	UFUNCTION()
	void AnimNotify_DashForward();


};
