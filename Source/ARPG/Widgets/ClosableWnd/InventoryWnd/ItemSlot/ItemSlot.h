// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridSlot.h"

#include "ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
private :
	TSubclassOf<UUserWidget> ItemSlotWidgetClass;

private :
	// 부모 위젯
	class UInventoryWnd* InventoryWnd;

	// 인벤토리 슬롯 인덱스
	int32 InventorySlotIndex;

public :
	UItemSlot(const FObjectInitializer& ObjectInitializer);



protected :

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

	// 드래그중 위젯에 마우스가 들어왔을 때 호출되는 메서드
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 드래그가 끝났을 때 호출되는 메서드
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;




public :
	// Inventory WND 를 설정합니다.
	FORCEINLINE void SetInventoryWnd(class UInventoryWnd* inventoryWnd)
	{ InventoryWnd = inventoryWnd; }

	FORCEINLINE void SetInventorySlotIndex(int32 index)
	{ InventorySlotIndex = index; }

	FORCEINLINE int32 GetInventorySlotIndex() const
	{ return InventorySlotIndex; }

	FORCEINLINE UGridSlot* GetGridSlot() const
	{ return Cast<UGridSlot>(Slot); }


};
