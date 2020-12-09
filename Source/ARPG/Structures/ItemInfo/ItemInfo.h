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
	// 아이템 코드
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemCode;

	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType;

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;

	// 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemDescription;

	// 최대 소지 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCount;

	// 아이템 판매 가격
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SalePrice;

	// 아이템 스프라이트 이미지 경로
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSoftObjectPath ItemSpritePath;

	// 기타 수치들
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value3;
};
