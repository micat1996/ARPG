// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()


private :
	// 닫을 수 있는 창을 관리하는 컴포넌트입니다.
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UClosableWndControllerComponent * ClosableWndController;

	// 게임 위젯에 대한 UClass 와 인스턴스를 나타냅니다.
	TSubclassOf<class UPlayerCharacterWidget> GameWidgetClass;
	class UPlayerCharacterWidget* PlayerCharacterWidgetInstance;


public :
	ARPGPlayerController();

protected :
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;



private :
	void InputMouseX(float axis);
	void InputMouseY(float axis);

public :
	class UPlayerCharacterWidget* GetPlayerCharacterWidgetInstance();
	
	FORCEINLINE class UClosableWndControllerComponent* GetClosableWndControllerComponent() const
	{ return ClosableWndController; }

	// 뷰 타깃을 설정합니다.
	/// - nullptr 가 전달될 경우 기존에 조작하던 플레이어 카메라로 설정합니다.
	void ChangeViewTarget(class AActor* newViewTarget);

};
