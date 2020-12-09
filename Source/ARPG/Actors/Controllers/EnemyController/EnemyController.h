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
	// 조종중인 적 캐릭터를 나타냅니다.
	UPROPERTY()
	class AEnemyCharacter* EnemyCharacter;


	// 자극 감지 객체에서 사용될 시각 자극 객체를 나타냅니다.
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight * AISightConfig;

	// 사용될 비헤이비어 트리 애셋을 나타냅니다.
	UPROPERTY()
	class UBehaviorTree * EnemyBehaviorTree;

	// 추적할 액터
	UPROPERTY()
	class AActor * TrackingTargetActor;

public :
	AEnemyController();

public :
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float dt) override;

private :
	void InitializeEnemyController();

	// 플레이어 캐릭터가 가까운 곳에 있는지 확인합니다.
	bool IsPlayerNearby();

	// 추적 이동을 수행합니다.
	void TrackingPlayer();

private :
	// 시각 센스에 감지된 액터가 존재할 경우 호출될 메서드입니다.
	UFUNCTION()
	void OnSightDetected(AActor* Actor, FAIStimulus Stimulus);

public :
	// 플레이어를 바라봅니다.
	void LookAtPlayer();

public :
	FORCEINLINE class AEnemyCharacter* GetEnemyCharacter() const
	{ return EnemyCharacter; }
	
};
