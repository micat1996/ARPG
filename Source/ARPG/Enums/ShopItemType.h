#pragma once

#include "CoreMinimal.h"
#include "ShopItemType.generated.h"

UENUM(BlueprintType)
enum class EShopItemType : uint8
{
	SI_SaleItem		 UMETA(DisplaName = Sale Item),
	SI_InventoryItem UMETA(DisplaName = Inventory Item)
};
