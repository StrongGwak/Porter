// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PChangeLevel.generated.h"

UCLASS()
class PORTER_API APChangeLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APChangeLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* CollisionBox;

	// 지금 맵 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapTransition")
	FName CurrentLevel;

	// 넘어갈 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapTransition")
	FName NextLevel;

	// 나중에 위젯으로 바꿔야할지도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapTransition")
	FName LoadingLevel;

	// 플레이어가 충돌했을 때 호출되는 함수
	UFUNCTION(Blueprintable, BlueprintCallable)
	void OnPlayerBeginOverlap(AActor* ThisActor, AActor* PPlayer);

public:
	bool bIsLoading;

	UFUNCTION(Blueprintable, BlueprintCallable)
	void OpenOthers();
	
	void TransitionToNewMap();

	class UPGameInstance* GI;
};
