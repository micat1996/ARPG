#pragma once

#include "ARPG.h"

#include "Widgets/ClosableWnd/DraggableWnd/DraggableWnd.h"

#include "Structures/ShopItemInfo/ShopItemInfo.h"
#include "Structures/ItemInfo/ItemInfo.h"

#include "Enums/ShopItemType.h"

#include "ShopWnd.generated.h"

UCLASS()
class ARPG_API UShopWnd : public UDraggableWnd
{
	GENERATED_BODY()

private :
	class UDataTable* DT_ItemInfo;
	TSubclassOf<class USaleItem> SaleItemClass;
	TSubclassOf<class UShopTradeWnd> ShopTradeWndClass;


private :
	class UScrollBox* ScrollBox_SaleList;
	class UScrollBox* ScrollBox_InventoryItem;

	// 인벤토리 아이템을 표시하는 위젯 객체들
	TArray<class USaleItem*> InventoryItemSlots;

	// 물건 교환 창이 띄워져 있음을 나타냅니다.
	UPROPERTY()
	bool bIsTradeWndActivated;

public :
	UShopWnd(const FObjectInitializer& ObjInitializer);

protected :
	virtual void NativeConstruct() override;

private :
	void AddItem(EShopItemType shopItemType, FName itemCode, int32 costs, int32 itemCount = 1, int32 inventorySlotIndex = INDEX_NONE);

private :
	// 소지 아이템 목록을 초기화 합니다.
	void InitializeInventoryItem();

	void OnInventorySlotChanged();


public :
	// 상점에서 파는 물건 목록을 초기화합니다.
	void InitializeSaleList(TArray<FShopItemInfo> saleItems);

	// 물건 교환 창을 띄웁니다.
	class UShopTradeWnd* CreateTradeWnd(class USaleItem* saleItemWidget, EShopItemType shopItemType, FItemInfo* itemInfo, int32 costs);
	
	

};
