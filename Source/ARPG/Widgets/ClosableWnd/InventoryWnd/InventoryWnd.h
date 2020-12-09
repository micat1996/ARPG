#pragma once

#include "ARPG.h"
#include "Widgets/ClosableWnd/DraggableWnd/DraggableWnd.h"
#include "InventoryWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UInventoryWnd : public UDraggableWnd
{
	GENERATED_BODY()

private :
	TSubclassOf<class UItemSlot> BP_ItemSlotClass;

private :
	struct FPlayerInfo* PlayerInfo;

	class UPlayerInventoryComponent* PlayerInventory;

private :
	class UGridPanel* GridPanel_Content;

	// 생성된 인벤토리 슬롯 객체들을 저장할 배열
	TArray<class UItemSlot*> InventorySlots;

	
public :
	UInventoryWnd(const FObjectInitializer& ObjInitializer);

protected :
	virtual void NativeOnInitialized() override;

private :
	// 빈 슬롯을 생성합니다.
	void CreateItemSlot();

public :
	// 인벤토리 창을 초기화합니다.
	void InitializeInventoryWnd(class UPlayerInventoryComponent* playerInventory);

	// 인벤토리 슬롯들을 갱신합니다.
	void UpdateInventorySlots();

public :
	FORCEINLINE class UPlayerInventoryComponent* GetPlayerInventoryComponent() const
	{ return PlayerInventory; }
};
