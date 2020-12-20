#pragma once

#include "CoreMinimal.h"
#include "ShopItemInfo.generated.h"

// ������ �������� ��Ÿ���� ���� ����ü
USTRUCT(BlueprintType)
struct ARPG_API FShopItemInfo
{
	GENERATED_USTRUCT_BODY()

public :
	// ������ �ڵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemCode;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Costs;

};
