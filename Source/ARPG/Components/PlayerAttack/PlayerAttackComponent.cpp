#include "PlayerAttackComponent.h"
#include "Actors/Characters/RPGCharacter/RPGCharacter.h"

UPlayerAttackComponent::UPlayerAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LoadAsset();
}


void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// �ش� ������Ʈ�� �����ϴ� ARPGCharacter ��ü�� �����մϴ�.
	PlayerCharacter = Cast<ARPGCharacter>(GetOwner());

	OnRegularAttackFinished.AddLambda([this]() -> void
		{
			// �⺻ ���� ���¸� �����ϸ�,
			// ���� ���� ���� ���·� �ʱ�ȭ�մϴ�.
			bRegularAttack = bNextRegularAttack = false;
		});
}


void UPlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bRegularAttack && PlayerCharacter->IsInAir())
		PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
	/// - StopMovementImmediately : ����� �ӵ��� (0, 0, 0) ���� �����Ͽ� �̵��� ����ϴ�.

}

void UPlayerAttackComponent::LoadAsset()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ATTACK(
		TEXT("AnimMontage'/Game/Resources/GhostLady_S1/Animations/In-Place/UseMotions/AM_Attack.AM_Attack'"));
	if (AM_ATTACK.Succeeded()) AttackAnimMontageAsset = AM_ATTACK.Object;

}

void UPlayerAttackComponent::RegularAttack()
{

	// AttackAnimMontageAsset ��ȿ�� �˻�
	if (!IsValid(AttackAnimMontageAsset))
	{
		UE_LOG(LogTemp, Error,
			TEXT("UPlayerAttackComponent.cpp :: %d LINE :: AttackAnimMontageAsset is not valid!"), 
			__LINE__);
		return;
	}

	// �̵� �Ұ����� �����̰ų�, ���߿� ���� ��� ������ �������� �ʵ��� �մϴ�.
	if (!PlayerCharacter->IsMoveable() ||
		PlayerCharacter->IsInAir()) return;

	// �⺻ �������̶�� ���� ���ݵ� ����ǵ��� �մϴ�.
	if (bRegularAttack)
		bNextRegularAttack = true;

	// �⺻ �������� �ƴ϶��
	else
	{
		// �⺻ ������ ���·� �����ϸ�, ù ��° �⺻ ���� �ִϸ��̼��� ����ǵ��� �մϴ�.
		bRegularAttack = true;
		PlayerCharacter->PlayAnimMontage(AttackAnimMontageAsset);
	}
}

void UPlayerAttackComponent::ActiveAttackRange(float radius, float length)
{
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(PlayerCharacter);

	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(
		GetWorld(),
		PlayerCharacter->GetActorLocation(),
		PlayerCharacter->GetActorLocation() + (PlayerCharacter->GetActorForwardVector() * length),
		radius,
		TEXT("AttackRange"),
		true,
		actorsToIgnore,
		EDrawDebugTrace::Type::ForDuration,
		hitResults,
		true);

	for (auto hit : hitResults)
	{

		hit.GetActor()->TakeDamage(
			20.0f,
			FDamageEvent(),
			PlayerCharacter->GetController(),
			PlayerCharacter);
	}

}

