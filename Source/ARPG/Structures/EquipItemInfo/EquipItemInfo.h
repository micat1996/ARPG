#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/EquipItemType.h"
#include "EquipItemInfo.generated.h"

/**
Head    10000
Mask    11000
Top     12000
Bottom  13000
Hand    14000
Foot    15000
Weapon  16000
 */

USTRUCT()
struct ARPG_API FEquipItemInfo : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    // 장비 아이템 코드
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ItemCode;

    // 장비 아이템 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ItemName;

    // 장비 아이템 타입
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EEquipItemType ItemType;

    // 장비 아이템 SkeletalMesh 경로
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSoftObjectPath SKMeshPath;

    // 세트 장비 아이템
    /// - Hand 파츠의 경우 오른손 SkeletalMesh 경로를 나타냅니다.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSoftObjectPath SKMeshSetPath;



    // 추가 방어력
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AdditionalDefensiveForce;

    // 추가 공격력
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AdditionalOffensiveForce;

    // 아이템 장착 최소 레벨
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinLevel;

    // 추가 체력
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AdditionalHp;

    // 추가 마나
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AdditionalMp;



    // 추가 인벤토리 슬롯
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 AdditionalSlotCount;

    // 강화 비용
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ReinforcementCosts;

    // 강화 성공 확률
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ReinforcementProbability;

    // 해당 아이템의 레벨을 나타냅니다.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ItemLevel;

    // 다음 강화 아이템 코드
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName NextReinforcementItemCode;

    // 최종 아이템임을 나타냅니다.
    /// - 해당 값이 참일 경우 더 이상 강화가 불가능한 아이템으로 인식됩니다.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bMaximumLevel;
};