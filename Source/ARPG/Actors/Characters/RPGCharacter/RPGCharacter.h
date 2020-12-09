#pragma once

#include "../BaseCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "Enums/EquipItemType.h"
#include "RPGCharacter.generated.h"

#ifndef GROUND_MOVE_SPEED
#define GROUND_MOVE_SPEED
#define WALK_SPEED			150.0f
#define RUN_SPEED			600.0f
#endif

DECLARE_DELEGATE(FOnRollingMoveFinishedSignature)

UCLASS()
class ARPG_API ARPGCharacter : public ABaseCharacter, 
	public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public :
	FOnRollingMoveFinishedSignature OnRollinngMoveFinished;

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
	class UZoomableSpringArmComponent * SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UPlayerAttackComponent * PlayerAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	class UPlayerInteractionComponent * PlayerInteraction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UPlayerInventoryComponent * PlayerInventory;


	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parts", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent * HeadMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parts", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent * MaskMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parts", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent * TopMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parts", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent * BottomMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parts", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent * LeftHandMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parts", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent * RightHandMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parts", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent * FootMesh;

		
	// PlayerController
	class ARPGPlayerController* PlayerController;

	// ĳ���� SK Mesh
	class USkeletalMesh* BodyMeshAsset;

	// �Ҽӵ� ���� ��Ÿ���ϴ�.
	FGenericTeamId TeamId;

	// �Ѹ� �̵� �ִ� ��Ÿ��
	UPROPERTY()
	class UAnimMontage* AnimMontageRollingMoveAsset;

	UPROPERTY()
	class UPlayerManager * PlayerManager;

private :
	// �޸��� ���¸� ��Ÿ���ϴ�.
	UPROPERTY()
	bool bIsRun;

	// �̵� ���� ���¸� ��Ÿ���ϴ�.
	UPROPERTY()
	bool bCanMove;

	// �Ѹ� �̵� ���� ���¸� ��Ÿ���ϴ�.
	UPROPERTY()
	bool bIsRollingMovable;

	// �Ѹ� �̵� ���¸� ��Ÿ���ϴ�.
	UPROPERTY()
	bool bRollingMove;


	// ���� ���� ���� ī��Ʈ�� ��Ÿ���ϴ�.
	UPROPERTY()
	int32 RemainingJumpCount;

	// ���� ����  �� ������������ ��Ÿ���ϴ�.
	UPROPERTY()
	bool bIsDoubleJumpCrouching;

public:
	ARPGCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void OnTakeDamage(
		AActor* DamagedActor,
		float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy,
		AActor* DamageCauser) override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

private :
	void LoadAsset();
	void InitializeComponents();

protected :
	UFUNCTION(BlueprintCallable, Category = "CharacterAction")
	void InputHorizontal(float axis);

	UFUNCTION(BlueprintCallable, Category = "CharacterAction")
	void InputVertical(float axis);

public :
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

private :
	void Run();
	void Walk();

	FORCEINLINE void RollingForward()
	{ StartRollingMove(FName(TEXT("Forward"))); }
	FORCEINLINE void RollingBackward()
	{ StartRollingMove(FName(TEXT("Backward"))); }
	FORCEINLINE void RollingRight()
	{ StartRollingMove(FName(TEXT("Right"))); }
	FORCEINLINE void RollingLeft()
	{ StartRollingMove(FName(TEXT("Left"))); }

	// �Ѹ� �̵��� �����մϴ�.
	void StartRollingMove(FName montageSectionName);



public :
	// �̵��� ���ߵ��� �մϴ�.
	FORCEINLINE void StopMove()
	{ bCanMove = false; }

	// �̵��� ����մϴ�.
	FORCEINLINE void AllowMove()
	{ bCanMove = true; }

	// ���� ���� ���� ���¸� �����մϴ�.
	FORCEINLINE void FinishDoubleJumpCrouching()
	{ bIsDoubleJumpCrouching = false; }

	// ������ �������� �����մϴ�.
	void Dash(FVector direction, float power);

	// ��Ʈ�ѷ� �������� ȸ����ŵ�ϴ�.
	void LookAtControlDirection();

	// ��� ������ Mesh �� �����մϴ�.
	/// - bUpdateAll : ��� ������ ������ �������� �����մϴ�.
	/// - updateItemType : � ������ ������ �������� �����մϴ�.
	void UpdateEquipItemMesh(bool bUpdateAll, EEquipItemType updateItemType = EEquipItemType::EI_Head);

	
	virtual float GetMaxHp() override;
	virtual float GetHp() override;
	virtual void SetHp(float value) override;

public :

	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override
	{ this->TeamId = TeamID; }

	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override
	{ return TeamId; }

	FORCEINLINE class UPlayerAttackComponent* GetPlayerAttackComponent() const
	{ return PlayerAttack; }

	FORCEINLINE class UCameraComponent* GetCameraComponent() const
	{ return Camera; }


	FORCEINLINE bool IsInAir() const
	{ return GetCharacterMovement()->IsFalling(); }

	FORCEINLINE bool IsDoubleJump() const
	{ return RemainingJumpCount == 0; }

	// �̵� ���� ���¸� ��ȯ�մϴ�.
	bool IsMoveable() const;

	FORCEINLINE bool IsDoubleJumpCrouching() const
	{ return bIsDoubleJumpCrouching; }

	class UPlayerManager* GetPlayerManager();

	FORCEINLINE class ARPGPlayerController* GetPlayerController() const
	{ return PlayerController; }

	FORCEINLINE class UPlayerInventoryComponent* GetPlayerInventory() const
	{ return PlayerInventory; }


};
