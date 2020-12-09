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

	// ������ �κ��丮 ���� ��ü���� ������ �迭
	TArray<class UItemSlot*> InventorySlots;

	
public :
	UInventoryWnd(const FObjectInitializer& ObjInitializer);

protected :
	virtual void NativeOnInitialized() override;

private :
	// �� ������ �����մϴ�.
	void CreateItemSlot();

public :
	// �κ��丮 â�� �ʱ�ȭ�մϴ�.
	void InitializeInventoryWnd(class UPlayerInventoryComponent* playerInventory);

	// �κ��丮 ���Ե��� �����մϴ�.
	void UpdateInventorySlots();

public :
	FORCEINLINE class UPlayerInventoryComponent* GetPlayerInventoryComponent() const
	{ return PlayerInventory; }
};
