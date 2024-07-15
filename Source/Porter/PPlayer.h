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

	void Up();
	void Down();
	void UpdateSpringArmTargetLength();
	void FObjectFinderInputManager();

public:
	UPROPERTY(EditAnywhere)
	float mouseSpeed = 30;

	// 용병을 NPC로 표현했지만, 나중에 고쳐야 할 듯
	UPROPERTY()
	float CurrentHero;

	// 용병의 종류를 숫자로 표현 : 0은 없음, 종류는 한 5가지 : 1~5 숫자로 표현
	UPROPERTY()
	TArray<int32> HeroArray;

	UPROPERTY()
	float CameraLevel;

	UPROPERTY(EditAnywhere)
	float MaxHero = 15;

private:
	int32 TempNum;

};
