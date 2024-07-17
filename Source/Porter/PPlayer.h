// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerStatStruct.h"
#include "PPlayer.generated.h"

UCLASS()
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

public:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float mouseSpeed = 30;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float AddCameraLength = 300;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	int32 HeroNum;

	// 용병의 종류를 숫자로 표현 : 0은 없음, 종류는 한 5가지 : 1~5 숫자로 표현
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<ACharacter*> HeroBoxArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<float> PorterFloorArray;
\
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<FVector> OffsetArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float PorterHeight = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float PorterWidth = 100;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* HeroSpawnLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> HeroBoxSpawner;

public:
	// Player가 가지고 있는 스텟들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	FPlayerStatsStruct PlayerAndHeroStats;

	UFUNCTION(BlueprintCallable, Category="Stats")
	void UpdateStats(FPlayerStatsStruct UpdateStat);
	
	UFUNCTION(Category="Stats")
	void PlusHP(int32 Heal);

	UFUNCTION(Category="Stats")
	void MinusHP(int32 Damage);
	
	// HP
	UPROPERTY(EditAnywhere)
	int32 MaxHp = 5;
	UPROPERTY(EditAnywhere)
	int32 CurrentHP = 5;
	UPROPERTY(EditAnywhere)
	int32 MaxHeroHP = 5;
	UPROPERTY(EditAnywhere)
	int32 LastHeroHP = 2;	// 항상 1~5의 int값  

	// 스태미나
	UPROPERTY(EditAnywhere)
	float MaxStamina = 100;
	UPROPERTY(EditAnywhere)
	float DecreaseStamina = 33; // 속력
	UPROPERTY(EditAnywhere)
	float IncreaseStamina = 20; // 속력
	UPROPERTY()
	float ZeroToHundredIncreaseSteamin = 10; // 속력
	UPROPERTY(EditAnywhere)
	float CurrentStamina = 100;
	UPROPERTY()
	bool bIsRun = false;
	UPROPERTY()
	bool bCanRun = true;
	

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Run();
	
	UFUNCTION()
	void StopRun();

	UFUNCTION()
	void Up();

	UFUNCTION()
	void Down();

	UFUNCTION()
	void MakeArrays();

	UFUNCTION()
	void FObjectFinderInputManager();

	UFUNCTION()
	void Die();
};
