// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStatStruct.generated.h"

/**
 * 
 */
class PORTER_API PlayerStatStruct
{
public:
	PlayerStatStruct();
	~PlayerStatStruct();
};

// 초기화 값들 모아두기
USTRUCT(Atomic, BlueprintType)
struct FPlayerStatsStruct
{
	GENERATED_USTRUCT_BODY()

	// 주석은 변수 위에 써야 bp 노드상에서 보임
public:
	// 플레이어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float mouseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float AddCameraLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	int32 MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float DecreaseStamina; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float IncreaseStamina; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float ZeroToHundredIncreaseStamina; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float PlayerWalkSpeed ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Player")
	float PlayerBoostSpeed;

	UPROPERTY(EditAnywhere, Category="Stats|Port")
	int32 MaxPortNum = 10;
	UPROPERTY(EditAnywhere, Category="Stats|Port")
	int32 MaxWeight = 10;
	
	FPlayerStatsStruct()
		: mouseSpeed(30), AddCameraLength(300), MaxHp(5), MaxStamina(100), DecreaseStamina(33), IncreaseStamina(20), \
			ZeroToHundredIncreaseStamina(10), PlayerWalkSpeed(400), PlayerBoostSpeed(1000) {}
};
