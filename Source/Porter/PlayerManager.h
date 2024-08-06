// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Hero/PHeroStruct.h"
#include "PlayerManager.generated.h"

class APHero;
/**
 * 
 */
UCLASS()
class PORTER_API UPlayerManager : public UObject
{
	GENERATED_BODY()

	// offset array를 얼만큼 만들어 둘 것인지. 항상 MaxPortNum보다 커야함.
	// MaximumArraySize는 정말 신중하게 바꿀 것.
public:
	UPlayerManager();

	// 초기값만 존재
	void Initialize(TArray<TSubclassOf<AActor>> Port, TArray<TSubclassOf<APHero>> Hero);
	
	UFUNCTION(BlueprintCallable, Blueprintable)
	void SaveSpawnInformation();
	UFUNCTION(BlueprintCallable, Blueprintable)
	void OpenSpawnInformation(ACharacter* PlayerCharacter);

	int32 SavedPortNum;
	
	UPROPERTY()
	TArray<FPHeroStruct> HeroStructArray;
	
private:
	UPROPERTY()
	TArray<AActor*> PortArray;

	UPROPERTY()
	TArray<class APHero*> HeroArray;


public:
	// 지게 관련
	UFUNCTION()
	int32 SpawnPort(int32 PortType, ACharacter* PlayerCharacter);
	UFUNCTION()
	int32 DestroyPort();
	UFUNCTION()
	int32 CheckPortNum();

	// 영웅 관련
	UFUNCTION()
	void SpawnHero(int32 HeroType, ACharacter* PlayerCharacter, bool bUseHeroIndex = false, int32 HeroIndex = -1);
	UFUNCTION()
	void DestroyHero(int32 HeroIndex);
	UFUNCTION()
	void SwapHeroes(TArray<int32> HeroIndexArray, ACharacter* PlayerCharacter);
	UFUNCTION()
	int32 LastHeroNum();
	UFUNCTION()
	int32 CheckHeroNum();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	int32 MaximumArraySize = 15;

public:
	// 플레이어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float mouseSpeed = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float AddCameraLength = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	int32 MaxHp = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float MaxStamina = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float DecreaseStamina = 33; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float IncreaseStamina = 20; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float ZeroToHundredIncreaseStamina = 10; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float PlayerWalkSpeed = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float PlayerBoostSpeed = 1000;

	TArray<FVector> OffsetArray;
	float OffsetX = -100;

public:
	// 지게 관련
	UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<AActor>> PortTypeArray;
	UPROPERTY()
	TArray<int32> PortFloorArray;
	UPROPERTY()
	float PortHeight = 100;
	UPROPERTY()
	float PortWidth = 100;
	UPROPERTY()
	float PortWeight = 0.2;

	UPROPERTY(EditAnywhere, Category="Stats|Port")
	int32 MaxPortNum = 10;
	UPROPERTY(EditAnywhere, Category="Stats|Port")
	int32 MaxWeight = 10;
	
public:
	// 영웅 관련
	UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<class APHero>> HeroTypeArray;
	
	UPROPERTY()
	float HeroWeight = 0.8;
	UPROPERTY()
	float HeroOffset = 50;

};
