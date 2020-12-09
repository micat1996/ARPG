#include "RPGCharacterAnimInstance.h"
#include "Actors/Characters/RPGCharacter/RPGCharacter.h"
#include "Components/PlayerAttack/PlayerAttackComponent.h"

void URPGCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Owner))
	{
		Owner = Cast<ARPGCharacter>(TryGetPawnOwner());
		return;
	}

	VelocityLength = Owner->GetVelocity().Size();
	bIsInAir = Owner->IsInAir();
	bIsDoubleJump = Owner->IsDoubleJump();
	bIsDoubleJumpCrouching = Owner->IsDoubleJumpCrouching();
}

void URPGCharacterAnimInstance::AnimNotify_DBJumpCrouching()
{
	if (!IsValid(Owner)) return;

	Owner->StopMove();
}

void URPGCharacterAnimInstance::AnimNotify_DBJumpCrouchingFin()
{
	if (!IsValid(Owner)) return;

	Owner->AllowMove();
	Owner->FinishDoubleJumpCrouching();
}

void URPGCharacterAnimInstance::AnimNotify_RollingMoveFin()
{
	Owner->OnRollinngMoveFinished.ExecuteIfBound();
}

void URPGCharacterAnimInstance::AnimNotify_NextRegularAttack()
{

	// 연계 공격이 가능한 상태라면
	if (Owner->GetPlayerAttackComponent()->IsNextRegularAttackable())

		// 다음 기본 공격 애니메이션을 재생합니다.
		Montage_JumpToSection(TEXT("RegularAttack2"));
		
}

void URPGCharacterAnimInstance::AnimNotify_RegularAttackFin()
{
	if (Owner->GetPlayerAttackComponent()->OnRegularAttackFinished.IsBound())
		Owner->GetPlayerAttackComponent()->OnRegularAttackFinished.Broadcast();
}

void URPGCharacterAnimInstance::AnimNotify_LookAtControlDirection()
{
	Owner->LookAtControlDirection();
}

void URPGCharacterAnimInstance::AnimNotify_RegularAttackFirstStep()
{
	if (!Owner->IsInAir())
		Owner->Dash(Owner->GetActorForwardVector(), 6500.0f);
}

void URPGCharacterAnimInstance::AnimNotify_RegularAttackFinalStep()
{
	if (!Owner->IsInAir())
		Owner->Dash(Owner->GetActorForwardVector() * -1.0f, 4000.0f);
}

void URPGCharacterAnimInstance::AnimNotify_ActiveRegularAttackRange()
{
	Owner->GetPlayerAttackComponent()->ActiveAttackRange(120.0f, 230.0f);
}
