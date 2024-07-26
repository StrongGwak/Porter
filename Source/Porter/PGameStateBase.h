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

	// 정말 주의해서 바꿀 것 : 모든 배열의 사이즈
	int32 MaximumArraySize = 15;

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
	
	// 공통
	UPROPERTY()
	TArray<FVector> OffsetArray;
	float OffsetX = -100;

	
	// 지게 관련
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
	int32 CheckPortNum();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Port")
	int32 MaxPortNum = 15;

	// 영웅 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Hero")
	TArray<class APHero*> HeroArray;
	UPROPERTY()
	TArray<int32> HeroIndexArray;
	UPROPERTY()
	float HeroWeight = 0.8;
	UPROPERTY()
	float HeroOffset = 50;
	UFUNCTION()
	int32 CheckHeroNum();

	
};
