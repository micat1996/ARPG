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

	// 해당 컴포넌트를 소유하는 ARPGCharacter 객체를 저장합니다.
	PlayerCharacter = Cast<ARPGCharacter>(GetOwner());

	OnRegularAttackFinished.AddLambda([this]() -> void
		{
			// 기본 공격 상태를 종료하며,
			// 연계 공격 진행 상태로 초기화합니다.
			bRegularAttack = bNextRegularAttack = false;
		});
}


void UPlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bRegularAttack && PlayerCharacter->IsInAir())
		PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
	/// - StopMovementImmediately : 적용된 속도를 (0, 0, 0) 으로 설정하여 이동을 멈춥니다.

}

void UPlayerAttackComponent::LoadAsset()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ATTACK(
		TEXT("AnimMontage'/Game/Resources/GhostLady_S1/Animations/In-Place/UseMotions/AM_Attack.AM_Attack'"));
	if (AM_ATTACK.Succeeded()) AttackAnimMontageAsset = AM_ATTACK.Object;

}

void UPlayerAttackComponent::RegularAttack()
{

	// AttackAnimMontageAsset 유효성 검사
	if (!IsValid(AttackAnimMontageAsset))
	{
		UE_LOG(LogTemp, Error,
			TEXT("UPlayerAttackComponent.cpp :: %d LINE :: AttackAnimMontageAsset is not valid!"), 
			__LINE__);
		return;
	}

	// 이동 불가능한 상태이거나, 공중에 있을 경우 공격을 실행하지 않도록 합니다.
	if (!PlayerCharacter->IsMoveable() ||
		PlayerCharacter->IsInAir()) return;

	// 기본 공격중이라면 다음 공격도 진행되도록 합니다.
	if (bRegularAttack)
		bNextRegularAttack = true;

	// 기본 공격중이 아니라면
	else
	{
		// 기본 공격중 상태로 설정하며, 첫 번째 기본 공격 애니메이션이 재생되도록 합니다.
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

