#include "EnemyAnimInstance.h"
#include "Actors/Characters/EnemyCharacter/EnemyCharacter.h"
#include "Actors/Controllers/EnemyController/EnemyController.h"
#include "Components/EnemyAttack/EnemyAttackComponent.h"

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Owner))
	{
		Owner = Cast<AEnemyCharacter>(TryGetPawnOwner());
		return;
	}

	VelocityLength = Owner->GetVelocity().Size();
}


void UEnemyAnimInstance::AnimNotify_EnemyAttackFin()
{
	if (Owner->GetEnemyAttack()->OnAttackFinished.IsBound())
		/// - IsBound() : 대리자에 메서드가 바인딩 되어있는지 확인합니다.

		Owner->GetEnemyAttack()->OnAttackFinished.Broadcast();

}

void UEnemyAnimInstance::AnimNotify_UpdateRotation()
{
	Cast<AEnemyController>(Owner->GetController())->LookAtPlayer();
}

void UEnemyAnimInstance::AnimNotify_ActiveAttackRange()
{
	Owner->GetEnemyAttack()->ActiveAttackRange();
}

void UEnemyAnimInstance::AnimNotify_DashForward()
{
	Owner->Dash(Owner->GetActorForwardVector(), 3000.0f);
}
