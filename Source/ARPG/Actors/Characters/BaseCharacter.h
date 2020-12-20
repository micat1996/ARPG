#pragma once

#include "ARPG.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class ARPG_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected :
	UPROPERTY()
	bool bIsDie;

protected:
	virtual void BeginPlay() override;

public :
	virtual float GetMaxHp()		PURE_VIRTUAL(ABaseCharacter::GetMaxHp, return 0.0f;);
	virtual float GetHp()			PURE_VIRTUAL(ABaseCharacter::GetHp, return 0.0f;);
	virtual void SetHp(float value) PURE_VIRTUAL(ABaseCharacter::SetHp, );

protected :
	// ���ظ� �Ծ��� ��� ȣ��� �޼���
	UFUNCTION()
	virtual void OnTakeDamage(
		AActor* DamagedActor, 
		float Damage, 
		const class UDamageType* DamageType, 
		class AController* InstigatedBy, 
		AActor* DamageCauser);

	// ĳ���Ͱ� ����� ȣ��� �޼���
	virtual void OnCharacterDie();


public :
	FORCEINLINE bool IsDie() const
	{ return bIsDie; }
};
