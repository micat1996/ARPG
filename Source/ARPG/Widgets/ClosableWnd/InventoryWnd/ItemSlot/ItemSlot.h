#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridSlot.h"

#include "Structures/ItemSlotInfo/ItemSlotInfo.h"

#include "ItemSlot.generated.h"


UCLASS()
class ARPG_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
private :
	TSubclassOf<UUserWidget> ItemSlotWidgetClass;

	class UDataTable* DT_ItemInfo;

	class UTexture2D* EmptyTexture;

	FLinearColor NormalSlotColor;
	FLinearColor DraggingSlotColor;

private :
	// 부모 위젯
	class UInventoryWnd* InventoryWnd;

	class UImage* Image_ItemSprite;
	class UTextBlock* Text_ItemCount;

	// 인벤토리 슬롯 인덱스
	UPROPERTY()
	int32 InventorySlotIndex;

public :
	UItemSlot(const FObjectInitializer& ObjectInitializer);



protected :
	virtual void NativeOnInitialized() override;

	// 해당 위젯에 마우스 클릭 입력이 있을 경우 호출됩니다.
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/// - InGeometry : 위젯의 위치, 크기를 나타냅니다.
	/// - InMouseEvent : 입력 이벤트의 종류

	// 해당 위젯에 마우스 클릭 입력이 있을 경우 호출됩니다.
	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry, 
		const FPointerEvent& InMouseEvent, 
		UDragDropOperation*& OutOperation) override;
	/// - OutOperation : 출력용 매개 변수이며, 감지된 드래그 작업 객체를 반환해야 합니다.

	// 드래그중 드래그 취소시 호출되는 메서드
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 드래그가 끝났을 때 호출되는 메서드
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


public :
	// 아이템 슬롯을 갱신합니다.
	void UpdateItemSlot();

public :
	// Inventory WND 를 설정합니다.
	FORCEINLINE void SetInventoryWnd(class UInventoryWnd* inventoryWnd)
	{ InventoryWnd = inventoryWnd; }

	FORCEINLINE void SetInventorySlotIndex(int32 index)
	{
		UE_LOG(LogTemp, Warning, TEXT("index = %d"), index);
		InventorySlotIndex = index; }

	FORCEINLINE int32 GetInventorySlotIndex() const
	{ return InventorySlotIndex; }

	FORCEINLINE UGridSlot* GetGridSlot() const
	{ return Cast<UGridSlot>(Slot); }

	// 해당 슬롯이 나타내는 아이템 정보를 얻습니다.
	FItemSlotInfo GetItemSlotInfo();

	// 해당 슬롯이 나타내는 아이템 정보를 설정합니다.
	void SetItemSlotInfo(FItemSlotInfo itemInfo);


};
