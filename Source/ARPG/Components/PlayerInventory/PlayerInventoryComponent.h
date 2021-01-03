#pragma once

#include "ARPG.h"

#include "Components/ActorComponent.h"

#include "Enums/EquipItemType.h"
#include "Enums/ReinforceResult.h"

#include "Structures/ItemSlotInfo/ItemSlotInfo.h"

#include "PlayerInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventorySlotChangedSignature)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public :
	// 인벤토리 슬롯에 변경이 있을 경우 호출될 대리자
	FOnInventorySlotChangedSignature OnInventorySlotChanged;

private :
	// 인벤토리 창 위젯 블루프린트 클래스를 나타냅니다.
	TSubclassOf<class UInventoryWnd> BPInventoryWndClass;

	// 장비 아이템 정보를 저장하는 DataTable Asset
	class UDataTable* DTEquipItemInfo;

	// 아이템 정보를 저장하는 DawtaTable Asset
	class UDataTable* DTItemInfo;

private :
	// 인벤토리 창 객체를 나타냅니다.
	UPROPERTY()
	class UInventoryWnd* InventoryWnd;

private :
	// 장착중인 아이템들을 나타냅니다.
	TMap<EEquipItemType, FName> EquipItems;


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

	// 인벤토리에 아이템을 추가합니다.
	/// - itemCode : 인벤토리에 추가할 아이템 코드
	/// - itemCount : 추가할 아이템 개수
	/// - return : 추가 성공 여부
	bool AddItem(FName itemCode, int32 itemCount = 1);

	// 인벤토리에 있는 아이템을 제거합니다.
	/// - slotIndex : 제거할 아이템을 갖는 슬롯 인덱스
	/// - removeCount : 제거 개수
	/// - return : 제거 성공 여부
	bool RemoveItem(int32 slotIndex, int32 removeCount);

	// 아이템이 담긴 슬롯 인덱스를 반환합니다.
	int32 GetItemIndex(FItemSlotInfo itemSlotInfo);

public :
	// 인벤토리 창이 열렸는지를 나타냅니다.
	FORCEINLINE bool IsInventoryWndOpend() const
	{ return InventoryWnd != nullptr; }

	FORCEINLINE const TArray<FItemSlotInfo>& GetInventoryItems() const
	{ return InventoryItems; }
};
