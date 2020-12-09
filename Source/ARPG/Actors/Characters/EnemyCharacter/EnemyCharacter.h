#pragma once

#include "../BaseCharacter.h"
#include "../../../Structures/EnemyData/EnemyData.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ARPG_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
	class UPlayerDetectorComponent* PlayerDetector;

	// �� ���� ������Ʈ�� ��Ÿ���ϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
		class UEnemyWidgetComponent* EnemyWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
	class UEnemyAttackComponent* EnemyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
	FName EnemyCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", meta = (AllowPrivateAccess = "true"))
		float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", meta = (AllowPrivateAccess = "true"))
		float Hp;

	// GameInstance
	UPROPERTY()
	class UARPGGameInstance* GameInst;

	// �� ������ ��Ÿ���� ������ ���̺� �ּ�
	UPROPERTY()
	class UDataTable * EnemyDatatable;

	// �� ����
	FEnemyData EnemyData;




public :
	AEnemyCharacter();

#pragma region Overrides...
protected :
	virtual void BeginPlay() override;

	virtual void OnTakeDamage(
		AActor* DamagedActor,
		float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy,
		AActor* DamageCauser) override;

	virtual void OnCharacterDie() override;

#pragma endregion

private :
	// ������Ʈ�� �ʱ�ȭ
	void InitializeComponents();

	// ���̷�Ż �޽� ������ �ʱ�ȭ�մϴ�.
	void InitializeSkeletalMeshComponent();

public :
	// �� ���� �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
	void InitializeEnemyDataInConstructTime();

	// ������ �������� �����մϴ�.
	void Dash(FVector direction, float power);

	FORCEINLINE virtual float GetMaxHp() override
	{ return MaxHp; }
	FORCEINLINE virtual float GetHp() override
	{ return Hp; }
	FORCEINLINE virtual void SetHp(float value) override 
	{ Hp = value; }


public :
	// �̵� ���� ���θ� ��Ÿ���ϴ�.
	bool IsMovable() const;

	FORCEINLINE class UPlayerDetectorComponent* GetPlayerDetector() const
	{ return PlayerDetector; }

	FORCEINLINE class UEnemyAttackComponent* GetEnemyAttack() const
	{ return EnemyAttack; }

	FORCEINLINE FEnemyData* GetEnemyData()
	{ return &EnemyData; }


	
};
