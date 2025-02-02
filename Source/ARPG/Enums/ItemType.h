#pragma once
#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_EtCetera			UMETA(DisplayName = EtCetera Item),
	IT_Consumption		UMETA(DisplayName = Consumption Item)
};
