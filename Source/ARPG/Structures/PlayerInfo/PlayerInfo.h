#pragma once

#include "PlayerInfo.generated.h"

USTRUCT()
struct ARPG_API FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float Hp;

	UPROPERTY()
	float Atk;

	UPROPERTY()
	int32 InventorySlotCount;


	FPlayerInfo();

};
