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

public:
	UPGameInstance();


	// offset array를 얼만큼 만들어 둘 것인지. 항상 MaxPortNum보다 커야함.
	// MaximumArraySize는 정말 신중하게 바꿀 것. 
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
	UPROPERTY(EditAnywhere, Category="Stats|Port")
	TArray<TSubclassOf<AActor>> PortTypeArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Port")
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
	int32 SpawnPort(int32 PortType);
	UFUNCTION()
	int32 DestroyPort();
	UFUNCTION()
	int32 CheckPortNum();
	UPROPERTY(EditAnywhere, Category="Stats|Port")
	int32 MaxPortNum = 10;

public:
	// 영웅 관련
	UPROPERTY(EditAnywhere, Category="Stats|Hero")
	TArray<TSubclassOf<class APHero>> HeroTypeArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Hero")
	TArray<class APHero*> HeroArray;
	UPROPERTY()
	TArray<int32> HeroIndexArray;
	UPROPERTY()
	float HeroWeight = 0.8;
	UPROPERTY()
	float HeroOffset = 50;
	UFUNCTION()
	void SpawnHero(int32 HeroType);
	UFUNCTION()
	int32 CheckHeroNum();
	
};
