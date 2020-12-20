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

	// â ũ�⸦ ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Size")
	FVector2D WndSize;

protected :
	// �θ� â�� ��Ÿ���ϴ�.
	/// - �θ� â�� nullptr �� �ƴ϶�� �θ� â�� ���� �� �ش� â�� ������ �˴ϴ�.
	UPROPERTY()
	UClosableWnd* ParentWnd;

	// �ڽ� â ��ü�� ������ �迭
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

	// �ڽ� â�� �����մϴ�.
	/// - ������ â�� ��ȯ�˴ϴ�.
	UFUNCTION(BlueprintCallable)
	UClosableWnd * CreateChildClosableWnd(TSubclassOf<UClosableWnd> closableWnd);

protected :
	// Ư���� â�� �Ҵ�Ǿ��ִ��� Ȯ���մϴ�.
	UFUNCTION(BlueprintCallable)
	bool IsAllocated(UClosableWnd* childwnd);

	// �ڽ� â�� �θ� â���� ���ܽ�ŵ�ϴ�.
	/// - �ش� �޼���� �ڽ� â�� �ݴ� ������ ���� ������, �迭 ChildWnds ���� ��Ҹ� �����ϱ⸸ �մϴ�.
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
