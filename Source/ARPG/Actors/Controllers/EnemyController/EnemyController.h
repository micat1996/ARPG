#pragma once

#include "ARPG.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

UCLASS()
class ARPG_API AEnemyController : public AAIController
{
	GENERATED_BODY()

private :
	// �������� �� ĳ���͸� ��Ÿ���ϴ�.
	UPROPERTY()
	class AEnemyCharacter* EnemyCharacter;


	// �ڱ� ���� ��ü���� ���� �ð� �ڱ� ��ü�� ��Ÿ���ϴ�.
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight * AISightConfig;

	// ���� �����̺�� Ʈ�� �ּ��� ��Ÿ���ϴ�.
	UPROPERTY()
	class UBehaviorTree * EnemyBehaviorTree;

	// ������ ����
	UPROPERTY()
	class AActor * TrackingTargetActor;

public :
	AEnemyController();

public :
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float dt) override;

private :
	void InitializeEnemyController();

	// �÷��̾� ĳ���Ͱ� ����� ���� �ִ��� Ȯ���մϴ�.
	bool IsPlayerNearby();

	// ���� �̵��� �����մϴ�.
	void TrackingPlayer();

private :
	// �ð� ������ ������ ���Ͱ� ������ ��� ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void OnSightDetected(AActor* Actor, FAIStimulus Stimulus);

public :
	// �÷��̾ �ٶ󺾴ϴ�.
	void LookAtPlayer();

public :
	FORCEINLINE class AEnemyCharacter* GetEnemyCharacter() const
	{ return EnemyCharacter; }
	
};
