#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyData.generated.h"

// 적 정보를 나타내는 구조체
USTRUCT()
struct ARPG_API FEnemyData : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public :
	// 적 코드
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EnemyCode;

	// 적 SkeletalMesh 경로
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath SkeletalMeshPath;

	// 적 애님 블루프린트 클래스 경로
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath AnimInstanceClassPath;

	// 적 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText EnemyName;

	// 최대 이동 속력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "1000.0", ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxSpeed;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "100000.0", ClampMin = "0.0", ClampMax = "100000.0"))
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = "0.0", UIMax = "1000.0", ClampMin = "0.0", ClampMax = "1000.0"))
	float Atk;
};
