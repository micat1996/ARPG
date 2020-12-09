#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackComponent.generated.h"

#ifndef ATTACK_TYPE
#define ATTACK_TYPE

#define ATTACK_NONE			-1
#define ATTACK_REGULAR		 0
#define ATTACK_SKILL		 1
#endif


DECLARE_MULTICAST_DELEGATE(FOnAttackEventSignature)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UEnemyAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public :

	// 공격이 시작될 때 호출되는 대리자
	/// - OnAttackStarted 대리자가 호출되었을 경우
	///   EnemyController 의 LookAtPlayer 메서드가 호출되도록 하고,
	///   공격 시작시 OnAttackStarted 대리자를 호출해주세요!
	FOnAttackEventSignature OnAttackStarted;

	// 공격이 끝날 때 애니메이션에서 호출되는 대리자
	FOnAttackEventSignature OnAttackFinished;



private :

	UPROPERTY()
	class AEnemyCharacter* EnemyCharacter;

	// 공격에 사용될 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackAnim;

	// 스킬이 발동될 확률을 지정합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (UIMin = "0.0", UIMax = "90.0", ClampMin = "0.0", ClampMax = "90.0", AllowPrivateAccess = "true"))
	float SkillPercentage;

private :
	// 현재 공격 상태를 나타냅니다.
	UPROPERTY()
	int32 AttackState;

public:	
	UEnemyAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	// 공격 애니메이션을 재생합니다.
	void PlayAttackAnimation();

	// 공격 범위를 활성화 시킵니다.
	void ActiveAttackRange();

public :
	// 공격중 상태를 나타냅니다.
	FORCEINLINE bool IsAttacking() const
	{ return AttackState != ATTACK_NONE; }
		

};
