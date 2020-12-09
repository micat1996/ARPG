#include "EnemyController.h"

#include "Actors/Characters/EnemyCharacter/EnemyCharacter.h"
#include "Components/PlayerDetector/PlayerDetectorComponent.h"
#include "Components/EnemyAttack/EnemyAttackComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyController::AEnemyController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_ENEMY(
		TEXT("BehaviorTree'/Game/Resources/AI/BT_Enemy.BT_Enemy'"));
	if (BT_ENEMY.Succeeded()) EnemyBehaviorTree = BT_ENEMY.Object;

	InitializeEnemyController();
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyCharacter = Cast<AEnemyCharacter>(InPawn);

	if (IsValid(EnemyBehaviorTree))
	{
		// BehaviorTree 를 실행시킵니다.
		RunBehaviorTree(EnemyBehaviorTree);

		// 시각 센스로 감지시 호출할 메서드 등록
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(
			this, &AEnemyController::OnSightDetected);
	}
	else
	{ UE_LOG(LogTemp, Error, TEXT("EnemyController.cpp :: %d LINE :: EnemyBehaviorTree is not loaded!"), __LINE__); }

	// 공격 시작시 플레이어를 바라보도록 합니다.
	EnemyCharacter->GetEnemyAttack()->OnAttackStarted.AddUObject(
		this, &AEnemyController::LookAtPlayer);

	// 공격 시작시 이동을 멈추도록 합니다.
	EnemyCharacter->GetEnemyAttack()->OnAttackStarted.AddLambda([this]() -> void
		{ MoveToActor(EnemyCharacter); });

}

void AEnemyController::Tick(float dt)
{
	Super::Tick(dt);

	// 조종하는 캐릭터가 이동할 수 있다면
	if (GetEnemyCharacter()->IsMovable())
	{
		// 근처에 적이 존재하지 않을 경우
		if (!IsPlayerNearby())
			// 추적 이동을 수행하도록 합니다.
			TrackingPlayer();
	}
}

void AEnemyController::InitializeEnemyController()
{
	// AI Controller 의 자극 감지 컴포넌트를 설정합니다.
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_PERCEPTION")));
	/// - UAIPerceptionComponent : AI 의 특정한 자극을 감지하는 컴포넌트를 나타냅니다.
	/// - AI Controller 내부에는 PerceptionComponent 객체를 가리키는 필드가 존재하며,
	///   해당 필드에 값을 할당합니다.
	/// - GetPerceptionComponent() 를 이용하여 해당 객체를 다시 얻을 수 있습니다.

	// AI 의 팀을 설정합니다.
	SetGenericTeamId(FGenericTeamId(TEAM_ENEMY));

	// 시야 자극 객체 설정
	{
		// 시야 자극 객체를 생성합니다.
		AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_CONFIG"));

		// 시야 거리를 설정합니다.
		AISightConfig->SightRadius = 500.0f;

		// 시야 상실 거리를 설정합니다.
		AISightConfig->LoseSightRadius = 650.0f;

		// 한쪽 눈의 시야각을 설정합니다.
		/// - 180' 일 경우 360' 를 감지하게 됩니다.
		AISightConfig->PeripheralVisionAngleDegrees = 180.0f;

		// 자극이 소멸되기까지 걸리는 시간을 설정합니다.
		AISightConfig->SetMaxAge(2.0f);

		// 적이 감지되도록 합니다.
		AISightConfig->DetectionByAffiliation.bDetectEnemies = true;

		// 같은 팀이 감지되지 않도록 합니다.
		AISightConfig->DetectionByAffiliation.bDetectFriendlies = false;

		// 중립 팀이 감지되지 않도록 합니다.
		AISightConfig->DetectionByAffiliation.bDetectNeutrals = false;

		// 해당 설정을 사용하는 시각 자극 객체를 적용시킵니다.
		GetPerceptionComponent()->ConfigureSense(*AISightConfig);
	}



}

bool AEnemyController::IsPlayerNearby()
{ 
	bool nearby = false;

	if (IsValid(EnemyCharacter))
	{
		nearby = EnemyCharacter->GetPlayerDetector()->IsPlayerDetected();
	}

	// 블랙 보드의 PlayerIsNearby 키의 값을 설정합니다.
	GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerIsNearby"), nearby);

	return nearby;
}

void AEnemyController::TrackingPlayer()
{
	// 만약 목표 액터가 존재하지 않는다면 실행하지 않도록 합니다.
	if (!IsValid(TrackingTargetActor)) return;

	// 블랙 보드의 IsTracking 키의 값을 추적중 상태로 설정합니다.
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsTracking"), true);

	// 플레이어 캐릭터를 향해 이동시킵니다.
	MoveToActor(TrackingTargetActor);
}

void AEnemyController::OnSightDetected(AActor* Actor, FAIStimulus Stimulus)
{
	// 시야에 감지된 객체가 존재한다면 
	if (Stimulus.WasSuccessfullySensed())
	{
		// 추적 타깃이 존재하지 않는다면
		if (!IsValid(TrackingTargetActor))
		{ 
			// 추적 타깃을 설정합니다.
			TrackingTargetActor = Actor;
		}
	}
	// 시야에 감지됐던 객체가 사라진다면
	else TrackingTargetActor = nullptr;

}

void AEnemyController::LookAtPlayer()
{
	if (!IsValid(TrackingTargetActor)) return;

	// 추적 목표의 위치를 저장합니다.
	FVector trackingLocation = TrackingTargetActor->GetActorLocation();

	// 조종중인 적 캐릭터의 위치를 저장합니다.
	FVector enemyCharacterLocation = EnemyCharacter->GetActorLocation();

	// 플레이어를 향한 방향을 저장합니다.
	FVector direction = trackingLocation - enemyCharacterLocation;

	// Yaw 회전값을 얻기 위하여 Z 축 값을 제외시킵니다.
	direction.Z = 0.0f;

	// 단위 벡터로 만듭니다.
	direction = direction.GetSafeNormal();

	// 만들어진 방향값을 이용하여 회전값을 얻습니다.
	EnemyCharacter->SetActorRotation(direction.Rotation());

}