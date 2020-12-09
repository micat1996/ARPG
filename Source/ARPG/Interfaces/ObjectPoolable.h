// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_API IObjectPoolable
{
	GENERATED_BODY()

public:
	// 재사용 가능 여부를 나타내는 변수에 대한 접근자 / 설정자
	virtual bool GetCanRecyclable() PURE_VIRTUAL(IObjectPoolable::GetCanRecyclable, return true;);
	virtual void SetCanRecyclable(bool canRecyclable) PURE_VIRTUAL(IObjectPoolable::SetCanRecyclable, );

	// 재활용이 되는 시점에서 호출되는 메서드
	virtual void OnRecycleStart() PURE_VIRTUAL(IObjectPoolable::OnRecycleStart, );
};
