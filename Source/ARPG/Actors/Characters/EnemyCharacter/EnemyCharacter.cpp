#include "EnemyCharacter.h"
#include "Actors/Controllers/EnemyController/EnemyController.h"

#include "Components/PlayerDetector/PlayerDetectorComponent.h"
#include "Components/EnemyAttack/EnemyAttackComponent.h"
#include "Components/CharacterWidget/EnemyWidget/EnemyWidgetComponent.h"

#include "BrainComponent.h"

#include "Widgets/CharacterWidget/HpableCharacterWidget/HpableCharacterWidget.h"

#include "Single/GameInstance/ARPGGameInstance.h"

#include "AnimInstances/EnemyCharacter/EnemyAnimInstance.h"


AEnemyCharacter::AEnemyCharacter()
{
	// DT_EnemyData �ּ� �ε�
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_DATA(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EnemyData.DT_EnemyData'"));

	if (DT_ENEMY_DATA.Succeeded()) EnemyDatatable = DT_ENEMY_DATA.Object;

	// ������Ʈ ���� �ʱ�ȭ
	InitializeComponents();

	// AI Controller �� �����մϴ�.
	AIControllerClass = AEnemyController::StaticClass();
}

void AEnemyCharacter::BeginPlay()
{ 
	Super::BeginPlay();

	GameInst = Cast<UARPGGameInstance>(GetGameInstance());

	// �� ���� �ʱ�ȭ
	InitializeEnemyDataInConstructTime();

	// ���� �ʱ�ȭ
	EnemyWidget->GetEnemyWidgetInstance()->InitializeWidget(this);
	EnemyWidget->GetEnemyWidgetInstance()->SetNameText(GetEnemyData()->EnemyName);
	EnemyWidget->GetEnemyWidgetInstance()->UpdateHp();


	OnTakeAnyDamage.AddDynamic(this, &AEnemyCharacter::OnTakeDamage);

	Tags.Add(TEXT("Enemy"));
}

void AEnemyCharacter::OnTakeDamage(
	AActor* DamagedActor, 
	float Damage, 
	const UDamageType* DamageType, 
	AController* InstigatedBy, 
	AActor* DamageCauser)
{

	Super::OnTakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	EnemyWidget->GetEnemyWidgetInstance()->UpdateHp();

}

void AEnemyCharacter::OnCharacterDie()
{
	Super::OnCharacterDie();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	FString reason;
	Cast<AEnemyController>(GetController())->GetBrainComponent()->StopLogic(reason);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		[this]() { Destroy(); },
		3.0f,
		false);
}

void AEnemyCharacter::InitializeComponents()
{
	PlayerDetector = CreateDefaultSubobject<UPlayerDetectorComponent>(TEXT("PLAYER_DETECTOR"));
	PlayerDetector->SetupAttachment(GetRootComponent());

	EnemyAttack = CreateDefaultSubobject<UEnemyAttackComponent>(TEXT("ENEMY_ATTACK"));

	EnemyWidget = CreateDefaultSubobject<UEnemyWidgetComponent>(TEXT("ENEMY_WIDGET"));
	EnemyWidget->SetupAttachment(GetRootComponent());

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}

void AEnemyCharacter::InitializeSkeletalMeshComponent()
{
	// �� SkeletalMesh ���� �ε�
	auto enemySKMesh = Cast<USkeletalMesh>(
		GameInst->GetStreamableManager()->LoadSynchronous(EnemyData.SkeletalMeshPath));

	// �ִ� �������Ʈ �ּ� �ε�
	UBlueprint* enemyAnimInstClass = Cast<UBlueprint>(
		GameInst->GetStreamableManager()->LoadSynchronous(EnemyData.AnimInstanceClassPath));

	// SkeletalMesh �� ��ȿ�� ���
	if (IsValid(enemySKMesh))
	{
		// �ε��� Skeletal Mesh �� �����ŵ�ϴ�.
		GetMesh()->SetSkeletalMesh(enemySKMesh);

		// ��ġ ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(
			FVector(0.0f, 0.0f, -(GetCapsuleComponent()->GetScaledCapsuleHalfHeight())),
			FRotator(0.0f, -90.0f, 0.0f));
	}
	else
	{ UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter.cpp :: %d LINE :: enemySKMesh is not loaded!"), __LINE__); }

	// �ִ� �������Ʈ �ּ��� ��ȿ�� ���
	if (IsValid(enemyAnimInstClass))
	{
		// �ִ� �ν��Ͻ� Ŭ������ ĳ�����մϴ�.
		TSubclassOf<UEnemyAnimInstance> bpAnimInstClass =
			static_cast<TSubclassOf<UEnemyAnimInstance>>(enemyAnimInstClass->GeneratedClass);

		// �ִ� �ν��Ͻ� Ŭ������ �����մϴ�.
		GetMesh()->SetAnimClass(bpAnimInstClass);
	}
	else 
	{ UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter.cpp :: %d LINE :: enemyAnimInstClass is not loaded!"), __LINE__); }
}

void AEnemyCharacter::InitializeEnemyDataInConstructTime()
{
	// enemyCode �� ��ġ�ϴ� �� ������ �����մϴ�.
	FString contextString;
	FEnemyData* enemyData = EnemyDatatable->FindRow<FEnemyData>(
		EnemyCode, contextString);

	// �� ������ ã�Ҵ��� �˻��մϴ�.
	if (enemyData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter.cpp :: %d LINE :: enemyData is not Valid"), __LINE__);
		return;
	}

	// �� ������ �����մϴ�.
	EnemyData = (*enemyData);

	UE_LOG(LogTemp, Warning, TEXT("InitializeEnemyDataInConstructTime :: name = %s"),
		*GetEnemyData()->EnemyName.ToString());

	// ü�� ����
	Hp = MaxHp = enemyData->MaxHp;

	// �ִ� �̵� �ӷ� ����
	GetCharacterMovement()->MaxWalkSpeed = EnemyData.MaxSpeed;

	// ���̷�Ż �޽� �ʱ�ȭ
	InitializeSkeletalMeshComponent();

	// ���� ��ġ ����
	EnemyWidget->SetWidgetHeight(
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 30.0f);
}

void AEnemyCharacter::Dash(FVector direction, float power)
{
	GetCharacterMovement()->AddImpulse(direction * power, true);
}

bool AEnemyCharacter::IsMovable() const
{
	// ���� �������� �ƴϸ�,
	return !GetEnemyAttack()->IsAttacking() 

		// ��� ���°� �ƴ� ���
		&& !bIsDie;

	// �̵��� �� �ֵ��� �մϴ�.
}