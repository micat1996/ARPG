#include "BaseCharacter.h"

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::OnTakeDamage);
}

void ABaseCharacter::OnTakeDamage(
	AActor* DamagedActor,
	float Damage,
	const class UDamageType* DamageType,
	class AController* InstigatedBy,
	AActor* DamageCauser)
{
	// 남은 체력 계산
	SetHp(GetHp() - Damage);
	
	// 사망 확인
	if (GetHp() <= 0.0f)
	{
		SetHp(0.0f);
		bIsDie = true;

		OnCharacterDie();
	}
}

void ABaseCharacter::OnCharacterDie()
{
	GetMesh()->SetCollisionProfileName(TEXT("DiedCharacterMesh"));
	GetMesh()->SetSimulatePhysics(true);
}
