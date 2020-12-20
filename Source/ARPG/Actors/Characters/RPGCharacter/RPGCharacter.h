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

	// 캐릭터 SK Mesh
	class USkeletalMesh* BodyMeshAsset;

	// 소속된 팀을 나타냅니다.
	FGenericTeamId TeamId;

	// 롤링 이동 애님 몽타주
	UPROPERTY()
	class UAnimMontage* AnimMontageRollingMoveAsset;

	UPROPERTY()
	class UPlayerManager * PlayerManager;

private :
	// 달리는 상태를 나타냅니다.
	UPROPERTY()
	bool bIsRun;

	// 이동 가능 상태를 나타냅니다.
	UPROPERTY()
	bool bCanMove;

	// 롤링 이동 가능 상태를 나타냅니다.
	UPROPERTY()
	bool bIsRollingMovable;

	// 롤링 이동 상태를 나타냅니다.
	UPROPERTY()
	bool bRollingMove;


	// 현재 남은 점프 카운트를 나타냅니다.
	UPROPERTY()
	int32 RemainingJumpCount;

	// 더블 점프  후 착지중인지를 나타냅니다.
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

	// 롤링 이동을 시작합니다.
	void StartRollingMove(FName montageSectionName);



public :
	// 이동을 멈추도록 합니다.
	FORCEINLINE void StopMove()
	{ bCanMove = false; }

	// 이동을 허용합니다.
	FORCEINLINE void AllowMove()
	{ bCanMove = true; }

	// 더블 점프 착지 상태를 해제합니다.
	FORCEINLINE void FinishDoubleJumpCrouching()
	{ bIsDoubleJumpCrouching = false; }

	// 지정한 방향으로 돌진합니다.
	void Dash(FVector direction, float power);

	// 컨트롤러 방향으로 회전시킵니다.
	void LookAtControlDirection();

	// 장비 아이템 Mesh 를 갱신합니다.
	/// - bUpdateAll : 모든 파츠를 갱신할 것인지를 결정합니다.
	/// - updateItemType : 어떤 파츠를 갱신할 것인지를 결정합니다.
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

	// 이동 가능 상태를 반환합니다.
	bool IsMoveable() const;

	FORCEINLINE bool IsDoubleJumpCrouching() const
	{ return bIsDoubleJumpCrouching; }

	class UPlayerManager* GetPlayerManager();

	FORCEINLINE class ARPGPlayerController* GetPlayerController() const
	{ return PlayerController; }

	FORCEINLINE class UPlayerInventoryComponent* GetPlayerInventory() const
	{ return PlayerInventory; }


};
