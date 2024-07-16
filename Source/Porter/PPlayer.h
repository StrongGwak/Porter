// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
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

public:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Up();

	UFUNCTION()
	void Down();

	UFUNCTION()
	void MakeArrays();

	UFUNCTION()
	void FObjectFinderInputManager();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float mouseSpeed = 30;

	// 용병을 NPC로 표현했지만, 나중에 고쳐야 할 듯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float HeroNum;

	// 용병의 종류를 숫자로 표현 : 0은 없음, 종류는 한 5가지 : 1~5 숫자로 표현
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<ACharacter*> HeroBoxArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<float> PorterFloorArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Options")
	float AddCameraLength = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float MaxHero = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float PorterHeight = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	float PorterWidth = 100;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* HeroSpawnLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> HeroBoxSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hero")
	TArray<FVector> OffsetArr;

	
	
};
