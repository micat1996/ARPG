#pragma once

#include "CoreMinimal.h"
#include "ShopItemInfo.generated.h"

// 상점ㅈ 아이템을 나타내기 위한 구조체
USTRUCT(BlueprintType)
struct ARPG_API FShopItemInfo
{
	GENERATED_USTRUCT_BODY()

public :
	// 아이템 코드
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemCode;

	// 아이템 가격
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Costs;

};
