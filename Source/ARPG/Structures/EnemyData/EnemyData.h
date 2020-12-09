#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyData.generated.h"

// �� ������ ��Ÿ���� ����ü
USTRUCT()
struct ARPG_API FEnemyData : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public :
	// �� �ڵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EnemyCode;

	// �� SkeletalMesh ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath SkeletalMeshPath;

	// �� �ִ� �������Ʈ Ŭ���� ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath AnimInstanceClassPath;

	// �� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText EnemyName;

	// �ִ� �̵� �ӷ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "1000.0", ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxSpeed;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "100000.0", ClampMin = "0.0", ClampMax = "100000.0"))
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "1000.0", ClampMin = "0.0", ClampMax = "1000.0"))
	float Atk;
};
