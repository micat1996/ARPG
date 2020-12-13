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
	// �θ� ����
	class UInventoryWnd* InventoryWnd;

	class UImage* Image_ItemSprite;
	class UTextBlock* Text_ItemCount;

	// �κ��丮 ���� �ε���
	UPROPERTY()
	int32 InventorySlotIndex;

public :
	UItemSlot(const FObjectInitializer& ObjectInitializer);



protected :
	virtual void NativeOnInitialized() override;

	// �ش� ������ ���콺 Ŭ�� �Է��� ���� ��� ȣ��˴ϴ�.
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/// - InGeometry : ������ ��ġ, ũ�⸦ ��Ÿ���ϴ�.
	/// - InMouseEvent : �Է� �̺�Ʈ�� ����

	// �ش� ������ ���콺 Ŭ�� �Է��� ���� ��� ȣ��˴ϴ�.
	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry, 
		const FPointerEvent& InMouseEvent, 
		UDragDropOperation*& OutOperation) override;
	/// - OutOperation : ��¿� �Ű� �����̸�, ������ �巡�� �۾� ��ü�� ��ȯ�ؾ� �մϴ�.

	// �巡���� �巡�� ��ҽ� ȣ��Ǵ� �޼���
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// �巡�װ� ������ �� ȣ��Ǵ� �޼���
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


public :
	// ������ ������ �����մϴ�.
	void UpdateItemSlot();

public :
	// Inventory WND �� �����մϴ�.
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

	// �ش� ������ ��Ÿ���� ������ ������ ����ϴ�.
	FItemSlotInfo GetItemSlotInfo();

	// �ش� ������ ��Ÿ���� ������ ������ �����մϴ�.
	void SetItemSlotInfo(FItemSlotInfo itemInfo);


};
