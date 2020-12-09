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
	// 기본 공격중 상태
	UPROPERTY()
	bool bRegularAttack;

	// 연계 공격시 다음 공격이 입력되었음을 나타내는 변수
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
	// 기본 공격을 합니다.
	void RegularAttack();

	// 공격 범위를 활성화 시킵니다.
	void ActiveAttackRange(float radius, float length);

	// 기본 공격중 상태에서 다음 연계 공격 진행 가능 여부를 나타냅니다.
	FORCEINLINE bool IsNextRegularAttackable() const
	{ return bNextRegularAttack; }

	// 공격중 상태를 반환합니다.
	FORCEINLINE bool IsAttacking() const
	{ return bRegularAttack; }

		
};
