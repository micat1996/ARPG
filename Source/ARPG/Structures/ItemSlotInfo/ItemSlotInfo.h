// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlotInfo.generated.h"

USTRUCT()
struct ARPG_API FItemSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// 아이템 코드
	UPROPERTY()
	FName ItemCode;

	// 아이템 개수
	UPROPERTY()
	int32 ItemCount;

public :
	FItemSlotInfo();
	FItemSlotInfo(FName itemCode, int32 itemCount);

	// 해당 슬롯이 비어있는지 확인합니다.
	FORCEINLINE bool IsEmpty() const
	{ return ItemCode == FName(TEXT("None")); }

};
