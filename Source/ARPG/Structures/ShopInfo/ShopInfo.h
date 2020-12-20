#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Structures/ShopItemInfo/ShopItemInfo.h"

#include "ShopInfo.generated.h"

// ���� ������ ��Ÿ���� ���� ����ü
USTRUCT()
struct ARPG_API FShopInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// �Ǹ� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FShopItemInfo> SaleItems;
};
