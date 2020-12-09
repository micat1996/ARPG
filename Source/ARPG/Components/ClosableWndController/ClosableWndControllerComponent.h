#pragma once

#include "ARPG.h"
#include "Components/ActorComponent.h"

#include "Widgets/ClosableWnd/ClosableWnd.h"
#include "Widgets/CharacterWidget/HpableCharacterWidget/PlayerCharacterWidget/PlayerCharacterWidget.h"
#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "ClosableWndControllerComponent.generated.h"


UCLASS()
class ARPG_API UClosableWndControllerComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	UPROPERTY()
	ARPGPlayerController* PlayerController;

	// 할당된 창 객체들을 저장할 배열
	UPROPERTY()
	TArray<UClosableWnd*> AllocatedWnds;

public:	
	UClosableWndControllerComponent();

protected:
	virtual void BeginPlay() override;

public :
	// 창을 추가합니다.
	template<typename T>
	T* AddWnd(TSubclassOf<UClosableWnd> wndCls)
	{
		// 창 객체를 생성합니다.
		T * wndInst = Cast<T>(CreateWidget<UClosableWnd>(PlayerController, wndCls));

		wndInst->SetClosableWndContorller(this);

		// 배열에 추가합니다.
		AllocatedWnds.Add(wndInst);

		// 화면에 표시합니다.
		PlayerController->GetPlayerCharacterWidgetInstance()->AddClosableWnd(wndInst);

		// 창 객체를 반환합니다.
		return wndInst;
	}

	// 창을 닫습니다.
	/// - bAllClose : 모든 창을 닫도록 할 것인지를 지정합니다.
	/// - closableWndInstanceToClose : 어떤 창을 닫을 것인지 지정합니다.
	///   - 만약 nullptr 라면 마지막에 열었던 창이 닫힙니다.
	void CloseWnd(bool bAllClose = false, class UClosableWnd* closableWndInstanceToClose = nullptr);

	// 할당된 창 개수를 반환합니다.
	FORCEINLINE int32 GetAllocatedWndCount() const
	{ return AllocatedWnds.Num(); }

	// 창이 할당되어있는지 확인합니다.
	bool IsAllocated(UClosableWnd* closableWndInstance);

	// 창을 최상단으로 설정합니다.
	void SetHighestPriorityWnd(UClosableWnd* closableWndInstance);
		
};
