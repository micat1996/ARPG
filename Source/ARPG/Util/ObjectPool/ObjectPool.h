// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Interfaces/ObjectPoolable.h"
#include "ObjectPool.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UObjectPool : public UObject
{
	GENERATED_BODY()

private:
	// Ǯ���� ������Ʈ���� ������ �迭
	TArray<IObjectPoolable*> PoolObjects;

public:
	// Ǯ���� ���ο� ������Ʈ�� ����մϴ�.
	template<typename T>
	T* RegisterRecyclableObject(T* newRecyclableObject)
	{
		PoolObjects.Add((IObjectPoolable*)newRecyclableObject);
		return newRecyclableObject;
	}

	// ��Ȱ��� ��ü�� ����ϴ�.
	IObjectPoolable* GetRecycledObject();
};
