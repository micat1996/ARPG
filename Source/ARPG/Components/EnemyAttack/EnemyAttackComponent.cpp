#include "EnemyAttackComponent.h"

#include "../../Actors/Characters/EnemyCharacter/EnemyCharacter.h"

UEnemyAttackComponent::UEnemyAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	AttackState = ATTACK_NONE;
	SkillPercentage = 30.0f;

}


void UEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyCharacter = Cast<AEnemyCharacter>(GetOwner());

	// ������ ������ ��� ������ ������ �����մϴ�.
	OnAttackFinished.AddLambda([this]()->void
		{
			AttackState = ATTACK_NONE;
		});

}


void UEnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UEnemyAttackComponent::PlayAttackAnimation()
{
	if (AttackState != ATTACK_NONE) return;
	if (!IsValid(AttackAnim)) return;

	// ���� ���� �̺�Ʈ ����.
	if (OnAttackStarted.IsBound())
		OnAttackStarted.Broadcast();


	// Ȯ���� ���� ��ų, �⺻ ������ �����մϴ�.
	if (FMath::FRandRange(1.0f, 100.0f) <= SkillPercentage)
	{
		AttackState = ATTACK_SKILL;
		EnemyCharacter->PlayAnimMontage(AttackAnim, 1.0f, TEXT("SkillAttack"));
	}
	else
	{
		AttackState = ATTACK_REGULAR;
		EnemyCharacter->PlayAnimMontage(AttackAnim, 1.0f, TEXT("BasicAttack"));
	}
}

void UEnemyAttackComponent::ActiveAttackRange()
{
	float attackRange = 100.0f;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(EnemyCharacter);

	TArray<FHitResult> outHits;

	UKismetSystemLibrary::SphereTraceMultiByProfile(
		GetWorld(),
		EnemyCharacter->GetActorLocation(),
		EnemyCharacter->GetActorLocation() + (EnemyCharacter->GetActorForwardVector() * attackRange),
		100.0f,
		TEXT("AttackRange"),
		true,
		actorsToIgnore,
		EDrawDebugTrace::Type::ForDuration,
		outHits,
		true,
		FLinearColor::Red,
		FLinearColor::Red,
		2.0f);

	for (auto hit : outHits)
	{
		if (!hit.GetActor()->ActorHasTag(TEXT("Enemy")))
		{
			hit.GetActor()->TakeDamage(
				EnemyCharacter->GetEnemyData()->Atk,
				FDamageEvent(),
				EnemyCharacter->GetController(),
				EnemyCharacter);
		}
	}


}
