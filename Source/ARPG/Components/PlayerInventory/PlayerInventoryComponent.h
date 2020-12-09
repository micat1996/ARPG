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
	// �κ��丮 â ���� �������Ʈ Ŭ������ ��Ÿ���ϴ�.
	TSubclassOf<class UInventoryWnd> BPInventoryWndClass;

private :
	// �κ��丮 â ��ü�� ��Ÿ���ϴ�.
	UPROPERTY()
	class UInventoryWnd* InventoryWnd;


private :
	// �������� �����۵��� ��Ÿ���ϴ�.
	TMap<EEquipItemType, FName> EquipItems;

	// ��� ������ ������ �����ϴ� DataTable Asset
	class UDataTable* DTEquipItemInfo;

	// ���� �� �ִ� â�� �����ϴ� ��ü�� ��Ÿ���ϴ�.
	class UClosableWndControllerComponent* ClosableWndController;

	// �������� �����۵��� ��Ÿ���ϴ�.
	TArray<FItemSlotInfo> InventoryItems;


public:	
	UPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// ��� �������� �ʱ�ȭ�մϴ�.
	void InitializeEquipItems();

	// �κ��丮 �������� �ʱ�ȭ�մϴ�.
	void InitializeInventoryItems();

	// �κ��丮 â�� ���ϴ�.
	void OpenInventory();

	// �κ��丮 â�� �ݽ��ϴ�.
	void CloseInventory();
public :
	// �κ��丮 â ���� ���¿� ���� �κ��丮 â�� ���ų� �ݽ��ϴ�.
	FORCEINLINE void ToggleInventory()
	{
		if (IsInventoryWndOpend())	CloseInventory();
		else						OpenInventory();
	}

	// ��� ������ Mesh �� �����մϴ�.
	void UpdateEquipItemMesh(EEquipItemType equipitemType,
		class USkeletalMeshComponent* refSkMeshToUpdate,
		class USkeletalMeshComponent* refSetSkMeshToUpdate = nullptr);

	// ������ ��� ������ �ڵ带 ����ϴ�.
	FORCEINLINE FName GetEquippedItemCode(EEquipItemType equipType) const
	{ return EquipItems[equipType]; }

	// ������ ��ȭ�� �õ��մϴ�.
	EReinforceResult TryReinforce(EEquipItemType itemType);

	// �κ��丮 �������� �����մϴ�.
	FORCEINLINE void SetItemSlotInfo(int32 slotIndex, FItemSlotInfo slotInfo)
	{ InventoryItems[slotIndex] = slotInfo; }

	// �κ��丮 �������� ��ü�մϴ�.
	void SwapItem(class UItemSlot* firstItemSlot, class UItemSlot* secondItemSlot);

public :
	// �κ��丮 â�� ���ȴ����� ��Ÿ���ϴ�.
	FORCEINLINE bool IsInventoryWndOpend() const
	{ return InventoryWnd != nullptr; }

	FORCEINLINE const TArray<FItemSlotInfo>& GetInventoryItems() const
	{ return InventoryItems; }

};
