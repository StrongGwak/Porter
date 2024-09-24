// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void Initialize();
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
	TArray<APHero*> HeroArray;

public:
	UFUNCTION()
	APHero* SpawnHero(FPHeroStruct HeroStruct);
	UFUNCTION()
	APHero* SpawnHeroUsingOpen(FPHeroStruct HeroStruct, ACharacter* PlayerCharacter);
	UFUNCTION()
	void DestroyHero(int32 HeroIndex);
	UFUNCTION()
	TArray<APHero*> SwapHeroes(TArray<int32> IndexArray);
	UFUNCTION()
	int32 LastHeroNum();
	UFUNCTION()
	int32 CheckHeroNum();

	UFUNCTION()
	APHero* FindHero(FName RowName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	int32 MaximumArraySize = 15;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<class APHero>> HeroTypeArray;
	
	UPROPERTY()
	float HeroWeight = 0.8;
	UPROPERTY()
	FVector HeroOffset = FVector(-40, 0, 30);

public:
	// GI
	UPROPERTY()
	class UPGameInstance* GI;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APHero> HeroClass;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* HeroDataTable;
};
