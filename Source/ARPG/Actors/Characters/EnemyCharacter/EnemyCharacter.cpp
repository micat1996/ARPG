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
	// DT_EnemyData 애셋 로드
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_DATA(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EnemyData.DT_EnemyData'"));

	if (DT_ENEMY_DATA.Succeeded()) EnemyDatatable = DT_ENEMY_DATA.Object;

	// 컴포넌트 정보 초기화
	InitializeComponents();

	// AI Controller 를 설정합니다.
	AIControllerClass = AEnemyController::StaticClass();
}

void AEnemyCharacter::BeginPlay()
{ 
	Super::BeginPlay();

	GameInst = Cast<UARPGGameInstance>(GetGameInstance());

	// 적 정보 초기화
	InitializeEnemyDataInConstructTime();

	// 위젯 초기화
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
	// 적 SkeletalMesh 동기 로드
	auto enemySKMesh = Cast<USkeletalMesh>(
		GameInst->GetStreamableManager()->LoadSynchronous(EnemyData.SkeletalMeshPath));

	// 애님 블루프린트 애셋 로드
	UBlueprint* enemyAnimInstClass = Cast<UBlueprint>(
		GameInst->GetStreamableManager()->LoadSynchronous(EnemyData.AnimInstanceClassPath));

	// SkeletalMesh 가 유효한 경우
	if (IsValid(enemySKMesh))
	{
		// 로드한 Skeletal Mesh 를 적용시킵니다.
		GetMesh()->SetSkeletalMesh(enemySKMesh);

		// 위치 회전 설정
		GetMesh()->SetRelativeLocationAndRotation(
			FVector(0.0f, 0.0f, -(GetCapsuleComponent()->GetScaledCapsuleHalfHeight())),
			FRotator(0.0f, -90.0f, 0.0f));
	}
	else
	{ UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter.cpp :: %d LINE :: enemySKMesh is not loaded!"), __LINE__); }

	// 애님 블루프린트 애셋이 유효한 경우
	if (IsValid(enemyAnimInstClass))
	{
		// 애님 인스턴스 클래스로 캐스팅합니다.
		TSubclassOf<UEnemyAnimInstance> bpAnimInstClass =
			static_cast<TSubclassOf<UEnemyAnimInstance>>(enemyAnimInstClass->GeneratedClass);

		// 애님 인스턴스 클래스를 적용합니다.
		GetMesh()->SetAnimClass(bpAnimInstClass);
	}
	else 
	{ UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter.cpp :: %d LINE :: enemyAnimInstClass is not loaded!"), __LINE__); }
}

void AEnemyCharacter::InitializeEnemyDataInConstructTime()
{
	// enemyCode 와 일치하는 열 정보를 저장합니다.
	FString contextString;
	FEnemyData* enemyData = EnemyDatatable->FindRow<FEnemyData>(
		EnemyCode, contextString);

	// 적 정보를 찾았는지 검사합니다.
	if (enemyData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter.cpp :: %d LINE :: enemyData is not Valid"), __LINE__);
		return;
	}

	// 적 정보를 저장합니다.
	EnemyData = (*enemyData);

	UE_LOG(LogTemp, Warning, TEXT("InitializeEnemyDataInConstructTime :: name = %s"),
		*GetEnemyData()->EnemyName.ToString());

	// 체력 설정
	Hp = MaxHp = enemyData->MaxHp;

	// 최대 이동 속력 설정
	GetCharacterMovement()->MaxWalkSpeed = EnemyData.MaxSpeed;

	// 스켈레탈 메시 초기화
	InitializeSkeletalMeshComponent();

	// 위젯 위치 설정
	EnemyWidget->SetWidgetHeight(
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 30.0f);
}

void AEnemyCharacter::Dash(FVector direction, float power)
{
	GetCharacterMovement()->AddImpulse(direction * power, true);
}

bool AEnemyCharacter::IsMovable() const
{
	// 적이 공격중이 아니며,
	return !GetEnemyAttack()->IsAttacking() 

		// 사망 상태가 아닐 경우
		&& !bIsDie;

	// 이동할 수 있도록 합니다.
}