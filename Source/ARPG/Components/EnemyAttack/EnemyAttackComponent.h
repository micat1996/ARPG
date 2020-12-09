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

	// ������ ���۵� �� ȣ��Ǵ� �븮��
	/// - OnAttackStarted �븮�ڰ� ȣ��Ǿ��� ���
	///   EnemyController �� LookAtPlayer �޼��尡 ȣ��ǵ��� �ϰ�,
	///   ���� ���۽� OnAttackStarted �븮�ڸ� ȣ�����ּ���!
	FOnAttackEventSignature OnAttackStarted;

	// ������ ���� �� �ִϸ��̼ǿ��� ȣ��Ǵ� �븮��
	FOnAttackEventSignature OnAttackFinished;



private :

	UPROPERTY()
	class AEnemyCharacter* EnemyCharacter;

	// ���ݿ� ���� �ִ� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackAnim;

	// ��ų�� �ߵ��� Ȯ���� �����մϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (UIMin = "0.0", UIMax = "90.0", ClampMin = "0.0", ClampMax = "90.0", AllowPrivateAccess = "true"))
	float SkillPercentage;

private :
	// ���� ���� ���¸� ��Ÿ���ϴ�.
	UPROPERTY()
	int32 AttackState;

public:	
	UEnemyAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	// ���� �ִϸ��̼��� ����մϴ�.
	void PlayAttackAnimation();

	// ���� ������ Ȱ��ȭ ��ŵ�ϴ�.
	void ActiveAttackRange();

public :
	// ������ ���¸� ��Ÿ���ϴ�.
	FORCEINLINE bool IsAttacking() const
	{ return AttackState != ATTACK_NONE; }
		

};
