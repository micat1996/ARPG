#pragma once

#include "ARPG.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnRegularAttackEventSignature)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UPlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public :
	FOnRegularAttackEventSignature OnRegularAttackStarted;
	FOnRegularAttackEventSignature OnRegularAttackFinished;

private :
	UPROPERTY()
	class ARPGCharacter* PlayerCharacter;


	UPROPERTY()
	class UAnimMontage* AttackAnimMontageAsset;

#pragma region Regular Attack
	// �⺻ ������ ����
	UPROPERTY()
	bool bRegularAttack;

	// ���� ���ݽ� ���� ������ �ԷµǾ����� ��Ÿ���� ����
	UPROPERTY()
	bool bNextRegularAttack;
#pragma endregion

public:	
	UPlayerAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	void LoadAsset();

public :
	// �⺻ ������ �մϴ�.
	void RegularAttack();

	// ���� ������ Ȱ��ȭ ��ŵ�ϴ�.
	void ActiveAttackRange(float radius, float length);

	// �⺻ ������ ���¿��� ���� ���� ���� ���� ���� ���θ� ��Ÿ���ϴ�.
	FORCEINLINE bool IsNextRegularAttackable() const
	{ return bNextRegularAttack; }

	// ������ ���¸� ��ȯ�մϴ�.
	FORCEINLINE bool IsAttacking() const
	{ return bRegularAttack; }

		
};
