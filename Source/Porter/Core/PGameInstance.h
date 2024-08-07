// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerManager.h"
#include "HeroManager.h"
#include "ItemManager.h"
#include "PGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTER_API UPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Blueprintable)
	UPlayerManager* GetPlayerManager();

	UFUNCTION(BlueprintCallable, Blueprintable)
	UHeroManager* GetHeroManager();

	UFUNCTION(BlueprintCallable, Blueprintable)
	UItemManager* GetItemManager();

private:
	UPROPERTY()
	UPlayerManager* PlayerManager;

	UPROPERTY()
	UHeroManager* HeroManager;

	UPROPERTY()
	UItemManager* ItemManager;

public:
	// 지게 관련
	UPROPERTY(EditAnywhere, Category="Stats|Port")
	TArray<TSubclassOf<AActor>> PortTypeArray;

	// 영웅 관련
	UPROPERTY(EditAnywhere, Category="Stats|Hero")
	TArray<TSubclassOf<class APHero>> HeroTypeArray;
};
