// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPickupItem.generated.h"

// HeroType : 1~5, HeroTemp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHeroInfoDelegate, int32, HeroType, int32, HeroTemp);

UCLASS()
class PORTER_API APPickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 메모리 반납
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

public:
	UFUNCTION()
	void PickupItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MyCustomOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category="Event")
	FHeroInfoDelegate Heroinfo;
	
};
