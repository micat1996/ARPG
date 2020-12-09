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

	// �Ҽӵ� ���� �����մϴ�.
	SetGenericTeamId(TEAM_PLAYER);

	// ��Ʈ�ѷ� Yaw ȸ���� ������� �ʵ��� ����
	bUseControllerRotationYaw = false;

	// �̵� �������� ȸ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// �ʱ� �̵� �ӵ� ����
	GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;

	// ���� �ʱ� ���ӷ� ����
	GetCharacterMovement()->JumpZVelocity = 1200.0f;

	// ĳ���Ͱ� �޴� �߷� ����
	GetCharacterMovement()->GravityScale = 2.5f;

	// ���߿��� ĳ���͸� 80% ���� �����ϵ��� ����
	GetCharacterMovement()->AirControl = 0.8f;

	// �ִ� ���� ī��Ʈ ����
	RemainingJumpCount = JumpMaxCount = 2;

	// �Ѹ� �̵� ���� ���·� ����
	bIsRollingMovable = true;

	// ���Ǵ� �ּµ��� �ε��մϴ�.
	LoadAsset();

	// �̵� ���� ���·� �����մϴ�.
	AllowMove();

	// ������Ʈ �ʱ�ȭ
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

	// ��� ���� ����
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

	// ��Ʈ�ѷ� ȸ���� Yaw ȸ������ �����մϴ�.
	FRotator yawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	// ��Ʈ�ѷ� ���� ������ ������ �����մϴ�.
	FVector rightVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	// �������̶�� �̵���Ű�� �ʽ��ϴ�.
	if (PlayerAttack->IsAttacking()) return;
	AddMovementInput(rightVector, axis);
}

void ARPGCharacter::InputVertical(float axis)
{
	if (!IsMoveable()) return;

	// ��Ʈ�ѷ� ȸ���� Yaw ȸ������ �����մϴ�.
	FRotator yawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	// ��Ʈ�ѷ� ���� ���� ������ �����մϴ�.
	FVector forwardVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

	// �������̶�� �̵���Ű�� �ʽ��ϴ�.
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

	// ���� �������̾��� ��� ���� ���·� �����մϴ�.
	if (IsDoubleJump())
		bIsDoubleJumpCrouching = true;

	// ���� ī��Ʈ �ʱ�ȭ
	RemainingJumpCount = JumpMaxCount;

	// �Ѹ� �̵� ���� ���·� ����
	bIsRollingMovable = true;

	// �޸��� ���¿� ���� �̵� �ӷ��� �����մϴ�.
	GetCharacterMovement()->MaxWalkSpeed = (bIsRun) ? RUN_SPEED : WALK_SPEED;
		
}

void ARPGCharacter::Run()
{
	bIsRun = true;

	if (!IsInAir())
		// �̵� �ӷ��� �����մϴ�.
		GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;
}

void ARPGCharacter::Walk()
{
	bIsRun = false;

	if (!IsInAir())
		// �̵� �ӷ��� �����մϴ�.
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

	// ��Ʈ�ѷ� �������� ȸ��
	LookAtControlDirection();

	bIsRollingMovable = false;
	bRollingMove = true;


	// �̵� ������ ������ ����
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
	// ��� ������ �����Ѵٸ�
	if (bUpdateAll)
	{
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Head, HeadMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Mask, MaskMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Top, TopMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Bottom, BottomMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Hand, LeftHandMesh, RightHandMesh);
		PlayerInventory->UpdateEquipItemMesh(EEquipItemType::EI_Foot, FootMesh);
	}

	// Ư���� ������ �����Ѵٸ�
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

		// �⺻ �̵��� ������ �����̸�
		(bCanMove)

		// ���� ���� �������� �ƴϸ�
		&& (!bIsDoubleJumpCrouching) 

		// �Ѹ� �̵����� �ƴϸ�
		&& (!bRollingMove);

	// �̵� �����ϵ��� �մϴ�.
}


UPlayerManager* ARPGCharacter::GetPlayerManager()
{
	return PlayerManager = (IsValid(PlayerManager)) ? PlayerManager :
		Cast<UARPGGameInstance>(GetGameInstance())->
		GetManagerClass<UPlayerManager>();
}