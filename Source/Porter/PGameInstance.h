// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API UPGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPGameInstance();

	void SpawnPort(class APPlayer* PlayerCharacter);

private:
	TArray<FVector> OffsetArray;
	float OffsetX = -100;

	UPROPERTY()
	TArray<TSubclassOf<AActor>> PortClassArray;
	UPROPERTY(EditAnywhere, Category="Stats|Port")
	TArray<AActor*> PortArray;
	UPROPERTY()
	TArray<int32> PortFloorArray;
	UPROPERTY()
	float PortHeight = 100;
	UPROPERTY()
	float PortWidth = 100;
	UPROPERTY()
	float PortWeight = 0.2;
	UFUNCTION()
	int32 CheckPortNum();
	UPROPERTY(EditAnywhere, Category="Stats|Port")
	int32 MaxPortNum = 15;
	
};
