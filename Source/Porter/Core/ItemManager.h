// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API UItemManager : public UObject
{
	GENERATED_BODY()

public:
	void SetGameInstance(class UPGameInstance* PGameInstance);

private:
	UPROPERTY()
	TArray<int32> DTIndex;

public:
	UPROPERTY()
	class UPGameInstance* GI;
};
