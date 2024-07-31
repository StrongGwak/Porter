// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerStatStruct.h"
#include "PPlayer.generated.h"

UCLASS()

/*
 1. Up과 Down 함수는 테스트 용이며, 결국 Damage에 따라 종속적으로 실행되어야 한다.
 - Up과 Down에 있는 HP최신화를 Damage에 옮기는 작업 필요
 2. 배열 바꾸는 함수를 만들어 두자 : 
 */
class PORTER_API APPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPlayer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// 입력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputMappingContext* IMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* TestUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* TestDownAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* SwapAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* TestHeroUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* TestHeroKill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* TestDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* TestSave;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess=true))
	class UInputAction* TestOpen;
	

public:
	UPROPERTY()
	class UPGameInstance* GI;
	
	// 스프링 암과 카메라
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

public:
	// 용병 소환 및 지게 -> 행렬로 만들기.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float mouseSpeed = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float AddCameraLength = 300;

	// 용병의 종류를 숫자로 표현 : 0은 없음, 종류는 한 5가지 : 1~5 숫자로 표현
	// 아래는 용병이 들어간 TArray. 예시 : [nullptr, 1번 용병 주소, 4번 용병 주소, 2번 용병 주소, 3번 용병 주소, 1번 용병 주소, 1번 용병 주소]

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<class APHero*> HeroArray;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<AActor*> PortArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	int32 HeroIndex;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float HeroWeight = 0.8;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float HeroOffset = 50;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float PortWeight = 0.2;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<int32> PortFloorArray;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<FVector> OffsetArray;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float PortHeight = 100;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float PortWidth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float EntireWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float MaxWeight;
	
	// 배열로 대체 - 에디터 상에서 결정  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<TSubclassOf<class APHero>> HeroType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<TSubclassOf<AActor>> PortType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<int32> HeroIndexArray;
	//UFUNCTION(BlueprintCallable, Category="Hero")
	//void SwapHeroesByArray(TArray<int32> SwapArray);
	
public:
	// Player가 가지고 있는 스텟들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	FPlayerStatsStruct PlayerAndHeroStats;
	UFUNCTION(BlueprintCallable, Category="Stats")
	void UpdateStats(FPlayerStatsStruct UpdateStat);

	// 나중에 옮기기
	UFUNCTION(Category="Stats")
	void PlusHP(int32 Heal);
	UFUNCTION(Category="Stats")
	void MinusHP(int32 Damage);
	UFUNCTION()
	void Boost();
	UFUNCTION()
	void StopBoost();
	UFUNCTION()
	void UpdateBoost();
	//UFUNCTION()
	//void SpawnPort(int32 Value);
	//UFUNCTION()
	//void SpawnHero(int32 Value);
	
	// HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int32 MaxHp = 5;
	// 얘는 플레이어가 가지고 있어야지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int32 CurrentHP = 5;

	// 스태미나
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float MaxStamina = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float DecreaseStamina = 33; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float IncreaseStamina = 20; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float ZeroToHundredIncreaseStamina = 10; // 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float CurrentStamina = 100;
	UPROPERTY()
	float TempStamina = 100;
	UPROPERTY()
	bool bIsBoost = false;
	UPROPERTY()
	bool bCanBoost = true;
	UPROPERTY()
	FTimerHandle BoostTimeHandle;
	UPROPERTY()
	FTimerHandle RestTimeHandle;
	UPROPERTY()
	float StartBoostTime;
	UPROPERTY()
	float RestTime;
	
// 이동 관련 함수거나, 초기화거나, 나중에 버려야할 함수(테스트용 함수)들
public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void UpPort();
	UFUNCTION()
	void DownPort();
	UFUNCTION()
	void UpHeroesFromArray();
	UFUNCTION()
	void FObjectFinderInputManager();
	UFUNCTION()
	void Die();
	UFUNCTION()
	void PlaySwap();
	UFUNCTION()
	bool CheckCondition();
	UPROPERTY()
	int32 TempIndex;
	void GetDamage();
	UFUNCTION()
	void MakeHeroHPZero();
	UFUNCTION()
	void SaveSpawn();
	UFUNCTION()
	void OpenSpawn();
	

};
