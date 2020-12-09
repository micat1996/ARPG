#pragma once

#include "ARPG.h"
#include "Components/ActorComponent.h"

#include "Structures/ItemSlotInfo/ItemSlotInfo.h"

#include "Enums/EquipItemType.h"
#include "Enums/ReinforceResult.h"

#include "PlayerInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	// 인벤토리 창 위젯 블루프린트 클래스를 나타냅니다.
	TSubclassOf<class UInventoryWnd> BPInventoryWndClass;

private :
	// 인벤토리 창 객체를 나타냅니다.
	UPROPERTY()
	class UInventoryWnd* InventoryWnd;


private :
	// 장착중인 아이템들을 나타냅니다.
	TMap<EEquipItemType, FName> EquipItems;

	// 장비 아이템 정보를 저장하는 DataTable Asset
	class UDataTable* DTEquipItemInfo;

	// 닫을 수 있는 창을 관리하는 객체를 나타냅니다.
	class UClosableWndControllerComponent* ClosableWndController;

	// 소지중인 아이템들을 나타냅니다.
	TArray<FItemSlotInfo> InventoryItems;


public:	
	UPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// 장비 아이템을 초기화합니다.
	void InitializeEquipItems();

	// 인벤토리 아이템을 초기화합니다.
	void InitializeInventoryItems();

	// 인벤토리 창을 엽니다.
	void OpenInventory();

	// 인벤토리 창을 닫습니다.
	void CloseInventory();
public :
	// 인벤토리 창 열림 상태에 따라 인벤토리 창을 열거나 닫습니다.
	FORCEINLINE void ToggleInventory()
	{
		if (IsInventoryWndOpend())	CloseInventory();
		else						OpenInventory();
	}

	// 장비 아이템 Mesh 를 갱신합니다.
	void UpdateEquipItemMesh(EEquipItemType equipitemType,
		class USkeletalMeshComponent* refSkMeshToUpdate,
		class USkeletalMeshComponent* refSetSkMeshToUpdate = nullptr);

	// 장착된 장비 아이템 코드를 얻습니다.
	FORCEINLINE FName GetEquippedItemCode(EEquipItemType equipType) const
	{ return EquipItems[equipType]; }

	// 아이템 강화를 시도합니다.
	EReinforceResult TryReinforce(EEquipItemType itemType);

	// 인벤토리 아이템을 설정합니다.
	FORCEINLINE void SetItemSlotInfo(int32 slotIndex, FItemSlotInfo slotInfo)
	{ InventoryItems[slotIndex] = slotInfo; }

	// 인벤토리 아이템을 교체합니다.
	void SwapItem(class UItemSlot* firstItemSlot, class UItemSlot* secondItemSlot);

public :
	// 인벤토리 창이 열렸는지를 나타냅니다.
	FORCEINLINE bool IsInventoryWndOpend() const
	{ return InventoryWnd != nullptr; }

	FORCEINLINE const TArray<FItemSlotInfo>& GetInventoryItems() const
	{ return InventoryItems; }

};
