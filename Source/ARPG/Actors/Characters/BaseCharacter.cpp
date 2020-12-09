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
	// ���� ü�� ���
	SetHp(GetHp() - Damage);
	
	// ��� Ȯ��
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
