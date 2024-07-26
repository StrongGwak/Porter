// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API APGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APGameModeBase();

	// GameState를 보기
	UPROPERTY()
	class APPlayer* PPlayer;
	
	// 지게 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	TArray<TSubclassOf<AActor>> PortClassArray;
	UFUNCTION()
	void SpawnPort(int32 PortTpye, FVector CurrentLocation, FVector ForwardV, FVector RightV);
	UFUNCTION()
	void DestroyPort(int32 Value);
	
	// 영웅 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	TArray<TSubclassOf<class APHero>> HeroClassArray;
	UFUNCTION()
	void SpawnHero(int32 HeroType);
	UFUNCTION()
	void DestroyHero(int32 Value);
};
