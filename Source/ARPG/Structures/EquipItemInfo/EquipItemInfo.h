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
    // ��� ������ �ڵ�
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ItemCode;

    // ��� ������ �̸�
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ItemName;

    // ��� ������ Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EEquipItemType ItemType;

    // ��� ������ SkeletalMesh ���
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSoftObjectPath SKMeshPath;

    // ��Ʈ ��� ������
    /// - Hand ������ ��� ������ SkeletalMesh ��θ� ��Ÿ���ϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSoftObjectPath SKMeshSetPath;



    // �߰� ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AdditionalDefensiveForce;

    // �߰� ���ݷ�
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AdditionalOffensiveForce;

    // ������ ���� �ּ� ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinLevel;

    // �߰� ü��
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AdditionalHp;

    // �߰� ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AdditionalMp;



    // �߰� �κ��丮 ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 AdditionalSlotCount;

    // ��ȭ ���
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ReinforcementCosts;

    // ��ȭ ���� Ȯ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ReinforcementProbability;

    // �ش� �������� ������ ��Ÿ���ϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ItemLevel;

    // ���� ��ȭ ������ �ڵ�
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName NextReinforcementItemCode;

    // ���� ���������� ��Ÿ���ϴ�.
    /// - �ش� ���� ���� ��� �� �̻� ��ȭ�� �Ұ����� ���������� �νĵ˴ϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bMaximumLevel;
};