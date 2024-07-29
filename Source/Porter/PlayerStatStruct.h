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
// 플레이어 스탯 관련
public:
	// 평상시 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerWalkSpeed = 400;
	// 달릴 때 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerRunSpeed = 1000;
	// 스태미나 최댓값		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerMaxStamina = 100;
	// 달릴 때 초당 스태미나 소모값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerDecreaseStamina = 33;
	// 0까지 가지 않았을 때 초당 스태미나 충전값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerIncreaseStamina = 20;
	// 0까지 가고 나서 초당 스태미나 충전값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerZeroToHundredIncreaseStamina = 10;	
	// 현재 플레이어의 최대 지게 수. 절대로 15를 넘기지 말 것 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxPortNum = 15;
	// 힘 스텟 : 따로 뺌
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWeight = 5;

	// 용병 스탯 관련
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroAttackSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroMeleeDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroCasterDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroCriticalChance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeroCriticalDamage = 0;
};
