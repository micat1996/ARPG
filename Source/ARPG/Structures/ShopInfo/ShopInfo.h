// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Structures/ShopItemInfo/ShopItemInfo.h"

#include "ShopInfo.generated.h"

// 상점 정보를 나타내기 위한 구조체
USTRUCT()
struct ARPG_API FShopInfo : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()
public:
    // 판매 아이템
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FShopItemInfo> SaleItems;
};
