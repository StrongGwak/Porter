// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerManager.h"
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

	UPlayerManager* GetPlayerManager() const;

private:
	UPROPERTY()
	UPlayerManager* PlayerManager;

public:
	// 지게 관련
	UPROPERTY(EditAnywhere, Category="Stats|Port")
	TArray<TSubclassOf<AActor>> PortTypeArray;

	// 영웅 관련
	UPROPERTY(EditAnywhere, Category="Stats|Hero")
	TArray<TSubclassOf<class APHero>> HeroTypeArray;
};
