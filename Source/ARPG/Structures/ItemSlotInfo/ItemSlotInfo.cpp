#include "ItemSlotInfo.h"

FItemSlotInfo::FItemSlotInfo()
{
	ItemCode = FName(TEXT(""));
	ItemCount = 0;
}

FItemSlotInfo::FItemSlotInfo(FName itemCode, int32 itemCount)
{
	ItemCode = itemCode;
	ItemCount = itemCount;

}