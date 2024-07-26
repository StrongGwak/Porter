// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API APGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	APGameStateBase();

	// 공통
	UPROPERTY()
	TArray<FVector> OffsetArray;
	
	// 지게 관련
	UPROPERTY()
	TArray<AActor*> PortArray;
	UPROPERTY()
	TArray<int32> PortFloorArray;
	UPROPERTY()
	float PortHeight = 100;
	UPROPERTY()
	float PortWidth = 100;
	UPROPERTY()
	float PortWeight = 0.2;

	// 영웅 관련
	UPROPERTY()
	TArray<class APHero*> HeroArray;
	UPROPERTY()
	float HeroWeight = 0.8;
	UPROPERTY()
	float HeroOffset = 50;
	
};
