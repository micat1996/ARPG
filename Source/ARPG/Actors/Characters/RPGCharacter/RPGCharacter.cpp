#include "RPGCharacter.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "Widgets/CharacterWidget/HpableCharacterWidget/PlayerCHaracterWidget/PlayerCharacterWidget.h"

#include "Components/ZoomableSpringArm/ZoomableSpringArmComponent.h"
#include "Components/PlayerAttack/PlayerAttackComponent.h"
#include "Components/PlayerInteraction/PlayerInteractionComponent.h"
#include "Components/PlayerInventory/PlayerInventoryComponent.h"

#include "Structures/PlayerInfo/PlayerInfo.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"


ARPGCharacter::ARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 소속될 팀을 설정합니다.
	SetGenericTeamId(TEAM_PLAYER);

	// 컨트롤러 Yaw 회전을 사용하지 않도록 설정
	bUseControllerRotationYaw = false;

	// 이동 방향으로 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// 초기 이동 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;

	// 점프 초기 가속력 설정
	GetCharacterMovement()->JumpZVelocity = 1200.0f;

	// 캐릭터가 받는 중력 설정
	GetCharacterMovement()->GravityScale = 2.5f;

	// 공중에서 캐릭터를 80% 제어 가능하도록 설정
	GetCharacterMovement()->AirControl = 0.8f;

	// 최대 점프 카운트 설정
	RemainingJumpCount = JumpMaxCount = 2;

	// 롤링 이동 가능 상태로 설정
	bIsRollingMovable = true;

	// 사용되는 애셋들을 로드합니다.
	LoadAsset();

	// 이동 가능 상태로 설정합니다.
	AllowMove();

	// 컴포넌트 초기화
	InitializeComponents();
}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	OnRollinngMoveFinished.BindLambda([this]() -> void
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bRollingMove = false;
		});

	TopMesh->SetMasterPoseComponent(GetMesh());
	BottomMesh->SetMasterPoseComponent(GetMesh());
	LeftHandMesh->SetMasterPoseComponent(GetMesh());
	RightHandMesh->SetMasterPoseComponent(GetMesh());
	FootMesh->SetMasterPoseComponent(GetMesh());

	// 모든 파츠 갱신
	UpdateEquipItemMesh(true);
}

void ARPGCharacter::OnTakeDamage(
	AActor* DamagedActor, 
	float Damage, 
	const UDamageType* DamageType, 
	AController* InstigatedBy, 
	AActor* DamageCauser)
{
	Super::OnTakeDamage(
		DamagedActor, 
		Damage, 
		DamageType, 
		InstigatedBy, 
		DamageCauser);

	PlayerController->GetPlayerCharacterWidgetInstance()->UpdateHp();
}

void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Action
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &ARPGCharacter::Run);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &ARPGCharacter::Walk);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ARPGCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ARPGCharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, PlayerInteraction, &UPlayerInteractionComponent::TryInteraction);
	PlayerInputComponent->BindAction(TEXT("OpenInventory"), EInputEvent::IE_Pressed, PlayerInventory, &UPlayerInventoryComponent::ToggleInventory);


	PlayerInputComponent->BindAction(TEXT("RollingForward"), EInputEvent::IE_DoubleClick, this, &ARPGCharacter::RollingForward);
	PlayerInputComponent->BindAction(TEXT("RollingBackward"), EInputEvent::IE_DoubleClick, this, &ARPGCharacter::RollingBackward);
	PlayerInputComponent->BindAction(TEXT("RollingRight"), EInputEvent::IE_DoubleClick, this, &ARPGCharacter::RollingRight);
	PlayerInputComponent->BindAction(TEXT("RollingLeft"), EInputEvent::IE_DoubleClick, this, &ARPGCharacter::RollingLeft);

	PlayerInputComponent->BindAction(TEXT("RegularAttack"), EInputEvent::IE_Pressed, 
		PlayerAttack, &UPlayerAttackComponent::RegularAttack);

	// Bind Axis
	PlayerInputComponent->BindAxis(TEXT("MouseWheel"), SpringArm,
		&UZoomableSpringArmComponent::ZoomCamera);

}

void ARPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerController = Cast<ARPGPlayerController>(NewController);
}

void ARPGCharacter::LoadAsset()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASE_BODY(
		TEXT("SkeletalMesh'/Game/Resources/GhostLady_S1/Meshes/Characters/Combines/SK_AssassinA.SK_AssassinA'"));
	if (SK_BASE_BODY.Succeeded()) BodyMeshAsset = SK_BASE_BODY.Object;
	else { UE_LOG(LogTemp, Error, TEXT("RPGCharacter.cpp :: %d LINE :: SK_BASE_BODY is not loaded!"), __LINE__); }

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ROLLING_MOVE(
		TEXT("AnimMontage'/Game/Resources/GhostLady_S1/Animations/In-Place/UseMotions/RollingMove/AM_RollingMove.AM_RollingMove'"));
	if (AM_ROLLING_MOVE.Succeeded())
		AnimMontageRollingMoveAsset = AM_ROLLING_MOVE.Object;
	else { UE_LOG(LogTemp, Error, TEXT("RPGCharacter.cpp :: %d LINE :: AM_ROLLING_MOVE is not loadad!"), __LINE__); }


}

void ARPGCharacter::InitializeComponents()
{
#pragma region Mesh Settings
	GetMesh()->SetSkeletalMesh(BodyMeshAsset);
#pragma endregion

#pragma region Add Components
	SpringArm = CreateDefaultSubobject<UZoomableSpringArmComponent>(TEXT("SPRING_ARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	PlayerAttack = CreateDefaultSubobject<UPlayerAttackComponent>(TEXT("PLAYER_ATTACK"));
	PlayerInteraction = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("INTERACTION_COMPONENT"));
	PlayerInventory = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("PLAYER_INVENTORY"));

	HeadMesh = GetMesh();
	MaskMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_MASK"));
	TopMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_TOP"));
	BottomMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_BOTTOM"));
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_LHAND"));
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_RHAND"));
	FootMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_FOOT"));


	MaskMesh->SetupAttachment(GetMesh(), FName(TEXT("Socket_Mask")));
	TopMesh->SetupAttachment(GetMesh());
	BottomMesh->SetupAttachment(GetMesh());
	LeftHandMesh->SetupAttachment(GetMesh());
	RightHandMesh->SetupAttachment(GetMesh());
	FootMesh->SetupAttachment(GetMesh());

	SpringArm->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(SpringArm);
	PlayerInteraction->SetupAttachment(GetRootComponent());
#pragma endregion
}

void ARPGCharacter::InputHorizontal(float axis)
{
	if (!IsMoveable()) return;

	// 컨트롤러 회전중 Yaw 회전만을 저장합니다.
	FRotator yawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	// 컨트롤러 기준 오른쪽 방향을 저장합니다.
	FVector rightVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	// 공격중이라면 이동시키지 않습니다.
	if (PlayerAttack->IsAttacking()) return;
	AddMovementInput(rightVector, axis);
}

void ARPGCharacter::InputVertical(float axis)
{
	if (!IsMoveable()) return;

	// 컨트롤러 회전중 Yaw 회전만을 저장합니다.
	FRotator yawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	// 컨트롤러 기준 앞쪽 방향을 저장합니다.
	FVector forwardVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

	// 공격중이라면 이동시키지 않습니다.
	if (PlayerAttack->IsAttacking()) return;
	AddMovementInput(forwardVector, axis);
}

void ARPGCharacter::Jump()
{
	if	(!IsMoveable()
	||	(RemainingJumpCount < 0)
	||	(PlayerAttack->IsAttacking()))
		return;

	if (RemainingJumpCount > 0)
	{
		if (RemainingJumpCount == 1)
		{
			FVector velocity = GetCharacterMovement()->Velocity;
			velocity.Z = GetCharacterMovement()->JumpZVelocity;
			GetCharacterMovement()->Velocity = velocity;
		}

		--RemainingJumpCount;
	}

	Super::Jump();
}

void ARPGCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// 더블 점프중이었을 경우 착지 상태로 설정합니다.
	if (IsDoubleJump())
		bIsDoubleJumpCrouching = true;

	// 점프 카운트 초기화
	RemainingJumpCount = JumpMaxCount;

	// 롤링 이동 가능 상태로 설정
	bIsRollingMovable = true;

	// 달리기 상태에 따라 이동 속력을 조절합니다.
	GetCharacterMovement()->MaxWalkSpeed = (bIsRun) ? RUN_SPEED : WALK_SPEED;
		
}

void ARPGCharacter::Run()
{
	bIsRun = true;

	if (!IsInAir())
		// 이동 속력을 조절합니다.
		GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;
}

void ARPGCharacter::Walk()
{
	bIsRun = false;

	if (!IsInAir())
		// 이동 속력을 조절합니다.
		GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
}


void ARPGCharacter::StartRollingMove(FName montageSectionName)
{
	if ((RemainingJumpCount < 1)
	|| (!IsMoveable())
	|| (!bIsRollingMovable)
	|| (PlayerAttack->IsAttacking())) 
		return;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 컨트롤러 방향으로 회전
	LookAtControlDirection();

	bIsRollingMovable = false;
	bRollingMove = true;


	// 이동 방향을 저장할 변수
	FVector direction = FVector::UpVector;

	if		(montageSectionName == TEXT("Backward"))	direction += GetActorForwardVector() * -1.0f;
	else if (montageSectionName == TEXT("Forward"))		direction += GetActorForwardVector();
	else if (montageSectionName == TEXT("Right"))		direction += GetActorRightVector();
	else if (montageSectionName == TEXT("Left"))		direction += GetActorRightVector() * -1.0f;

	direction = direction.GetSafeNormal();

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->AddImpulse(direction * 1500.0f, true);

	PlayAnimMontage(AnimMontageRollingMoveAsset, 1.0f, montageSectionName);
}

void ARPGCharacter::Dash(FVector direction, float power)
{
	GetCharacterMovement()->AddImpulse(direction * power, true);
}

void ARPGCharacter::LookAtControlDirection()
{
	FRotator controlRotation = GetControlRotation();
	controlRotation.Roll = controlRotation.Pitch = 0.0f;

	SetActorRotation(controlRotation);
}

void ARPGCharacter::UpdateEquipItemMesh(bool bUpdateAll, EEquipItemType updateItemType)
{
	// 모든 파츠를 갱신한다면
	if (bUpdateAll)
	{
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Head, HeadMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Mask, MaskMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Top, TopMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Bottom, BottomMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Hand, LeftHandMesh, RightHandMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Foot, FootMesh);
	}

	// 특정한 파츠만 갱신한다면
	else
	{
		switch (updateItemType)
		{
		case EEquipItemType::EI_Head:
			PlayerInventory->UpdateEquipItemMesh(updateItemType, HeadMesh);
			break;
		case EEquipItemType::EI_Mask:
			PlayerInventory->UpdateEquipItemMesh(updateItemType, MaskMesh);
			break;
		case EEquipItemType::EI_Top:
			PlayerInventory->UpdateEquipItemMesh(updateItemType, TopMesh);
			break;
		case EEquipItemType::EI_Bottom:
			PlayerInventory->UpdateEquipItemMesh(updateItemType, BottomMesh);
			break;
		case EEquipItemType::EI_Hand:
			PlayerInventory->UpdateEquipItemMesh(updateItemType, LeftHandMesh, RightHandMesh);
			break;
		case EEquipItemType::EI_Foot:
			PlayerInventory->UpdateEquipItemMesh(updateItemType, FootMesh);
			break;
		case EEquipItemType::EI_Weapon:
			break;
		}
	}


}

float ARPGCharacter::GetMaxHp()
{ return GetPlayerManager()->GetPlayerInfo()->MaxHp; }

float ARPGCharacter::GetHp()
{ return GetPlayerManager()->GetPlayerInfo()->Hp; }

void ARPGCharacter::SetHp(float value)
{ GetPlayerManager()->GetPlayerInfo()->Hp = value; }

bool ARPGCharacter::IsMoveable() const
{
	return 

		// 기본 이동이 가능한 상태이며
		(bCanMove)

		// 더블 점프 착지중이 아니며
		&& (!bIsDoubleJumpCrouching) 

		// 롤링 이동중이 아니면
		&& (!bRollingMove);

	// 이동 가능하도록 합니다.
}


UPlayerManager* ARPGCharacter::GetPlayerManager()
{
	return PlayerManager = (IsValid(PlayerManager)) ? PlayerManager :
		Cast<UARPGGameInstance>(GetGameInstance())->
		GetManagerClass<UPlayerManager>();
}