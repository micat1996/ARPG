// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClosableWnd.generated.h"

DECLARE_MULTICAST_DELEGATE(FWndEventSignature)

UCLASS()
class ARPG_API UClosableWnd : public UUserWidget
{
	GENERATED_BODY()
	
public :
	FWndEventSignature onWndClosed;

	// 창 크기를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Size")
	FVector2D WndSize;

protected :
	// 부모 창을 나타냅니다.
	/// - 부모 창이 nullptr 가 아니라면 부모 창이 닫힐 때 해당 창도 닫히게 됩니다.
	UPROPERTY()
	UClosableWnd* ParentWnd;

	// 자식 창 객체를 저장할 배열
	UPROPERTY()
	TArray<UClosableWnd *> ChildWnds;

	UPROPERTY()
	class UButton * CloseButton;

	UPROPERTY()
	class UClosableWndControllerComponent* ClosableWndController;

protected :
	virtual void NativeConstruct() override;

public :
	void SetCloseButton(class UButton* closeButton);

	// 자식 창을 생성합니다.
	/// - 생성된 창을 반환됩니다.
	UFUNCTION(BlueprintCallable)
	UClosableWnd * CreateChildClosableWnd(TSubclassOf<UClosableWnd> closableWnd);

protected :
	// 특정한 창이 할당되어있는지 확인합니다.
	UFUNCTION(BlueprintCallable)
	bool IsAllocated(UClosableWnd* childwnd);

	// 자식 창을 부모 창에서 제외시킵니다.
	/// - 해당 메서드는 자식 창을 닫는 역할을 하지 않으며, 배열 ChildWnds 에서 요소를 제거하기만 합니다.
	void RemoveFromParentWnd(UClosableWnd* childWnd);


public :
	UFUNCTION()
	void CloseClosableWnd();

public :
	FORCEINLINE void SetClosableWndContorllerComponent(class UClosableWndControllerComponent* closableWndController)
	{ ClosableWndController = closableWndController; }

	FORCEINLINE class UClosableWndControllerComponent * GetClosableWndControllerComponent() const
	{ return ClosableWndController; }

	UFUNCTION(BlueprintCallable)
	void UpdateWndSize(float width, float height);

};
