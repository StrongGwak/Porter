// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HeroManager.generated.h"

class UPlayerManager;
struct FPHeroStruct;
class APHero;
/**
 * 
 */
UCLASS()
class PORTER_API UHeroManager : public UObject
{
	GENERATED_BODY()

public:
	UHeroManager();

	void Initialize(TArray<TSubclassOf<APHero>> Hero);
	void SetGameInstance(class UPGameInstance* PGameInstance);

	UFUNCTION(BlueprintCallable, Blueprintable)
	void SaveSpawnInformation();
	UFUNCTION(BlueprintCallable, Blueprintable)
	void OpenSpawnInformation(ACharacter* PlayerCharacter);
	UFUNCTION()
	TArray<class APHero*> GetHeroArray();
	UFUNCTION()
	void SetHeroArray(TArray<class APHero*> Heroes);
	
	UPROPERTY()
	TArray<FPHeroStruct> HeroStructArray;
	
private:
	UPROPERTY()
	TArray<class APHero*> HeroArray;

public:
	UFUNCTION()
	void SpawnHero(int32 HeroType, ACharacter* PlayerCharacter, bool bUseHeroIndex = false, int32 HeroIndex = -1);
	UFUNCTION()
	void DestroyHero(int32 HeroIndex);
	UFUNCTION()
	void SwapHeroes(TArray<int32> IndexArray, ACharacter* PlayerCharacter);
	UFUNCTION()
	int32 LastHeroNum();
	UFUNCTION()
	int32 CheckHeroNum();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	int32 MaximumArraySize = 15;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<class APHero>> HeroTypeArray;
	
	UPROPERTY()
	float HeroWeight = 0.8;
	UPROPERTY()
	float HeroOffset = 50;

public:
	// GI
	UPROPERTY()
	class UPGameInstance* GI;
};
