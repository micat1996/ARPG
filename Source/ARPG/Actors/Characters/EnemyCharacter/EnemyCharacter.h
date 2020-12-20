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

	// 적 위젯 컴포넌트를 나타냅니다.
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

	// 적 정보를 나타내는 데이터 테이블 애셋
	UPROPERTY()
	class UDataTable * EnemyDatatable;

	// 적 정보
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
	// 컴포넌트를 초기화
	void InitializeComponents();

	// 스켈레탈 메시 정보를 초기화합니다.
	void InitializeSkeletalMeshComponent();

public :
	// 적 정보 초기화
	UFUNCTION(BlueprintCallable)
	void InitializeEnemyDataInConstructTime();

	// 지정한 방향으로 돌진합니다.
	void Dash(FVector direction, float power);

	FORCEINLINE virtual float GetMaxHp() override
	{ return MaxHp; }
	FORCEINLINE virtual float GetHp() override
	{ return Hp; }
	FORCEINLINE virtual void SetHp(float value) override 
	{ Hp = value; }


public :
	// 이동 가능 여부를 나타냅니다.
	bool IsMovable() const;

	FORCEINLINE class UPlayerDetectorComponent* GetPlayerDetector() const
	{ return PlayerDetector; }

	FORCEINLINE class UEnemyAttackComponent* GetEnemyAttack() const
	{ return EnemyAttack; }

	FORCEINLINE FEnemyData* GetEnemyData()
	{ return &EnemyData; }


	
};
