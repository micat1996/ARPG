// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlotInfo.generated.h"

USTRUCT()
struct ARPG_API FItemSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// ������ �ڵ�
	UPROPERTY()
	FName ItemCode;

	// ������ ����
	UPROPERTY()
	int32 ItemCount;

public :
	FItemSlotInfo();
	FItemSlotInfo(FName itemCode, int32 itemCount);

	// �ش� ������ ����ִ��� Ȯ���մϴ�.
	FORCEINLINE bool IsEmpty() const
	{ return ItemCode == FName(TEXT("None")); }

};
