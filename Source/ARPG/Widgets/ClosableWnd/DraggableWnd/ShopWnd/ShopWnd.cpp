#include "ShopWnd.h"


#include "Widgets/ClosableWnd/DraggableWnd/ShopWnd/SaleItem/SaleItem.h"

#include "Structures/ShopInfo/ShopInfo.h"

#include "Components/ScrollBox.h"


UShopWnd::UShopWnd(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SHOP_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ShopInfo.DT_ShopInfo'"));
	if (DT_SHOP_INFO.Succeeded()) DT_ShopInfo = DT_SHOP_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("ShopWnd.cpp :: %d LINE :: DT_SHOP_INFO is not loaded!"), __LINE__); }

	static ConstructorHelpers::FClassFinder<USaleItem> BP_SALE_ITEM(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/ShopWnd/BP_SaleItem.BP_SaleItem_C'"));
	if (BP_SALE_ITEM.Succeeded()) SaleItemClass = BP_SALE_ITEM.Class;
	else { UE_LOG(LogTemp, Error, TEXT("ShopWnd.cpp :: %d LINE :: BP_SALE_ITEM is not loaded!"), __LINE__); }
}

void UShopWnd::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateWndSize(800.0f, 900.0f);

	ScrollBox_SaleList = Cast<UScrollBox>(GetWidgetFromName("ScrollBox_SaleList"));

	InitializeSaleList();
}

void UShopWnd::InitializeSaleList()
{
	AddItem(EShopItemType::SI_SaleItem, FName(TEXT("")));
}

void UShopWnd::AddItem(EShopItemType shopItemType, FName itemCode, int32 itemCount)
{
	USaleItem * saleItem = CreateWidget<USaleItem>(this, SaleItemClass);
	ScrollBox_SaleList->AddChild(saleItem);

	saleItem->InitializeSaleItem(EShopItemType::SI_SaleItem, itemCode, itemCount);
}
