#include "SaleItem.h"


USaleItem::USaleItem(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DT_ItemInfo = DT_ITEM_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("SaleItem.cpp :: %d LINE :: DT_ITEM_INFO is not loaded!"), __LINE__); }


}

void USaleItem::NativeConstruct()
{
	Super::NativeConstruct();

}

void USaleItem::InitializeSaleItem(EShopItemType shopItemType, FName ItemCode, int32 itemCount)
{
	// 아이템 정보를 얻습니다.
	FString contextString;
	ItemInfo = DT_ItemInfo->FindRow<FItemInfo>(ItemCode, contextString);


}
