// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/ItemType.h"
#include "ItemInfo.generated.h"

USTRUCT()
struct ARPG_API FItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// ������ �ڵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemCode;

	// ������ Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType;

	// ������ �̸�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemDescription;

	// �ִ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCount;

	// ������ �Ǹ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SalePrice;

	// ������ ��������Ʈ �̹��� ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath ItemSpritePath;

	// ��Ÿ ��ġ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value3;
};
