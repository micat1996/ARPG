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
		// BehaviorTree �� �����ŵ�ϴ�.
		RunBehaviorTree(EnemyBehaviorTree);

		// �ð� ������ ������ ȣ���� �޼��� ���
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(
			this, &AEnemyController::OnSightDetected);
	}
	else
	{ UE_LOG(LogTemp, Error, TEXT("EnemyController.cpp :: %d LINE :: EnemyBehaviorTree is not loaded!"), __LINE__); }

	// ���� ���۽� �÷��̾ �ٶ󺸵��� �մϴ�.
	EnemyCharacter->GetEnemyAttack()->OnAttackStarted.AddUObject(
		this, &AEnemyController::LookAtPlayer);

	// ���� ���۽� �̵��� ���ߵ��� �մϴ�.
	EnemyCharacter->GetEnemyAttack()->OnAttackStarted.AddLambda([this]() -> void
		{ MoveToActor(EnemyCharacter); });

}

void AEnemyController::Tick(float dt)
{
	Super::Tick(dt);

	// �����ϴ� ĳ���Ͱ� �̵��� �� �ִٸ�
	if (GetEnemyCharacter()->IsMovable())
	{
		// ��ó�� ���� �������� ���� ���
		if (!IsPlayerNearby())
			// ���� �̵��� �����ϵ��� �մϴ�.
			TrackingPlayer();
	}
}

void AEnemyController::InitializeEnemyController()
{
	// AI Controller �� �ڱ� ���� ������Ʈ�� �����մϴ�.
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_PERCEPTION")));
	/// - UAIPerceptionComponent : AI �� Ư���� �ڱ��� �����ϴ� ������Ʈ�� ��Ÿ���ϴ�.
	/// - AI Controller ���ο��� PerceptionComponent ��ü�� ����Ű�� �ʵ尡 �����ϸ�,
	///   �ش� �ʵ忡 ���� �Ҵ��մϴ�.
	/// - GetPerceptionComponent() �� �̿��Ͽ� �ش� ��ü�� �ٽ� ���� �� �ֽ��ϴ�.

	// AI �� ���� �����մϴ�.
	SetGenericTeamId(FGenericTeamId(TEAM_ENEMY));

	// �þ� �ڱ� ��ü ����
	{
		// �þ� �ڱ� ��ü�� �����մϴ�.
		AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_CONFIG"));

		// �þ� �Ÿ��� �����մϴ�.
		AISightConfig->SightRadius = 500.0f;

		// �þ� ��� �Ÿ��� �����մϴ�.
		AISightConfig->LoseSightRadius = 650.0f;

		// ���� ���� �þ߰��� �����մϴ�.
		/// - 180' �� ��� 360' �� �����ϰ� �˴ϴ�.
		AISightConfig->PeripheralVisionAngleDegrees = 180.0f;

		// �ڱ��� �Ҹ�Ǳ���� �ɸ��� �ð��� �����մϴ�.
		AISightConfig->SetMaxAge(2.0f);

		// ���� �����ǵ��� �մϴ�.
		AISightConfig->DetectionByAffiliation.bDetectEnemies = true;

		// ���� ���� �������� �ʵ��� �մϴ�.
		AISightConfig->DetectionByAffiliation.bDetectFriendlies = false;

		// �߸� ���� �������� �ʵ��� �մϴ�.
		AISightConfig->DetectionByAffiliation.bDetectNeutrals = false;

		// �ش� ������ ����ϴ� �ð� �ڱ� ��ü�� �����ŵ�ϴ�.
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

	// �� ������ PlayerIsNearby Ű�� ���� �����մϴ�.
	GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerIsNearby"), nearby);

	return nearby;
}

void AEnemyController::TrackingPlayer()
{
	// ���� ��ǥ ���Ͱ� �������� �ʴ´ٸ� �������� �ʵ��� �մϴ�.
	if (!IsValid(TrackingTargetActor)) return;

	// �� ������ IsTracking Ű�� ���� ������ ���·� �����մϴ�.
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsTracking"), true);

	// �÷��̾� ĳ���͸� ���� �̵���ŵ�ϴ�.
	MoveToActor(TrackingTargetActor);
}

void AEnemyController::OnSightDetected(AActor* Actor, FAIStimulus Stimulus)
{
	// �þ߿� ������ ��ü�� �����Ѵٸ� 
	if (Stimulus.WasSuccessfullySensed())
	{
		// ���� Ÿ���� �������� �ʴ´ٸ�
		if (!IsValid(TrackingTargetActor))
		{ 
			// ���� Ÿ���� �����մϴ�.
			TrackingTargetActor = Actor;
		}
	}
	// �þ߿� �����ƴ� ��ü�� ������ٸ�
	else TrackingTargetActor = nullptr;

}

void AEnemyController::LookAtPlayer()
{
	if (!IsValid(TrackingTargetActor)) return;

	// ���� ��ǥ�� ��ġ�� �����մϴ�.
	FVector trackingLocation = TrackingTargetActor->GetActorLocation();

	// �������� �� ĳ������ ��ġ�� �����մϴ�.
	FVector enemyCharacterLocation = EnemyCharacter->GetActorLocation();

	// �÷��̾ ���� ������ �����մϴ�.
	FVector direction = trackingLocation - enemyCharacterLocation;

	// Yaw ȸ������ ��� ���Ͽ� Z �� ���� ���ܽ�ŵ�ϴ�.
	direction.Z = 0.0f;

	// ���� ���ͷ� ����ϴ�.
	direction = direction.GetSafeNormal();

	// ������� ���Ⱚ�� �̿��Ͽ� ȸ������ ����ϴ�.
	EnemyCharacter->SetActorRotation(direction.Rotation());

}