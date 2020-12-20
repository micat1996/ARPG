#include "ItemSlotInfo.h"

FItemSlotInfo::FItemSlotInfo()// : FItemSlotInfo(FName(TEXT("")), 0)
{
	ItemCode = FName(TEXT(""));
	ItemCount = 0;
}

FItemSlotInfo::FItemSlotInfo(FName itemCode, int32 itemCount)
{
	ItemCode = itemCode;
	ItemCount = itemCount;
}
