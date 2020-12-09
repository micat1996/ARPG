// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API URPGCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner Properties")
	class ARPGCharacter * Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner Properties")
	float VelocityLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner Properties")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner Properties")
	bool bIsDoubleJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner Properties")
	bool bIsDoubleJumpCrouching;

public :
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private :
	UFUNCTION()
	void AnimNotify_DBJumpCrouching();

	UFUNCTION()
	void AnimNotify_DBJumpCrouchingFin();

	UFUNCTION()
	void AnimNotify_RollingMoveFin();

	UFUNCTION()
	void AnimNotify_NextRegularAttack();

	UFUNCTION()
	void AnimNotify_RegularAttackFin();

	UFUNCTION()
	void AnimNotify_LookAtControlDirection();

	UFUNCTION()
	void AnimNotify_RegularAttackFirstStep();

	UFUNCTION()
	void AnimNotify_RegularAttackFinalStep();

	UFUNCTION()
	void AnimNotify_ActiveRegularAttackRange();


};
