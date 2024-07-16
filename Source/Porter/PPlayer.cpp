// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"
#include "TestHeroBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APPlayer::APPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	HeroSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("HeroSpawnLocation"));
	HeroSpawnLocation->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldLocation(FVector(0, 0, 70));
	SpringArm->TargetArmLength = 400;
	SpringArm->bUsePawnControlRotation=true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = true;

	FObjectFinderInputManager();
	MakeArrays();
	
}


// Called when the game starts or when spawned
void APPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subSystem->AddMappingContext(IMC, 0);
		}
	}

	
	
}

// Called every frame
void APPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APPlayer::Look);
	EnhancedInputComponent->BindAction(TestUpAction, ETriggerEvent::Started, this, &APPlayer::Up);
	EnhancedInputComponent->BindAction(TestDownAction, ETriggerEvent::Started, this, &APPlayer::Down);

}

void APPlayer::Move(const FInputActionValue& Value)
{
	const FVector2D movementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, movementVector.Y);
	AddMovementInput(RightDirection, movementVector.X);
}

void APPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y * GetWorld()->DeltaTimeSeconds * mouseSpeed * -1);
	AddControllerYawInput(LookAxisVector.X * GetWorld()->DeltaTimeSeconds * mouseSpeed);
}


void APPlayer::Up()
{
	if (CurrentHero < MaxHero)
	{
		CurrentHero++;
		//UpdateSpringArmTargetLength();
		SpringArm->TargetArmLength = 400 + CameraLevelArr[CurrentHero] * 300;
		//UpdateOffset();

		ACharacter* TestHeroBox = GetWorld()->SpawnActor<ACharacter>(HeroBoxSpawner, HeroSpawnLocation->GetComponentLocation() + OffsetArr[CurrentHero], HeroSpawnLocation->GetComponentRotation());

		if(TestHeroBox)
		{
			HeroBoxArray.Add(TestHeroBox);
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHero));
	}

	
}

void APPlayer::Down()
{
	if (CurrentHero > 0)
	{
		CurrentHero--;
		SpringArm->TargetArmLength = 400 + CameraLevelArr[CurrentHero] * 300;

		if(HeroBoxArray.Num() > 0)
		{
			ACharacter* LastHeroBox = HeroBoxArray.Last();

			if(LastHeroBox)
			{
				LastHeroBox->Destroy();
				HeroBoxArray.Pop();
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHero));
	}

	
}

void APPlayer::MakeArrays()
{
	int TempCameraLevel = 0;
	for(int HeroNum=0; HeroNum<MaxHero+1; HeroNum++)
	{
		// Hero 수에 따른 카메라 레벨 == 쌓은 층 수
		TempCameraLevel = 0;
		while (true)
		{
			if (HeroNum <= TempCameraLevel * (TempCameraLevel + 1) / 2)
			{
				break;
			}
			else
			{
				TempCameraLevel++;
			}
		}
		// 배열 추가
		CameraLevelArr.Add(TempCameraLevel);

		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CameraLevelArr[HeroNum]));


		
		// 층수로 계산하는 스폰 위치 Offset
		float OffsetY = 0;
		float CurrentSpawnNum = HeroNum - (TempCameraLevel-1)*TempCameraLevel / 2;
		int ForCount = round(TempCameraLevel);
		for(int i = 0; i<round(CurrentSpawnNum); i++)
		{
			if(i%2 == 1)
			{
				OffsetY = -1 * OffsetY - 200;
			}
			else
			{
				OffsetY *= -1;
			}
		}
		if (ForCount%2 == 0)
		{
			OffsetY += 100;
		}
		// 배열 추가
		OffsetArr.Add(FVector(-100, OffsetY, TempCameraLevel*100));

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(OffsetArr[HeroNum].Y));
	}
}

void APPlayer::FObjectFinderInputManager()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>InputMappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Inputs/IMC_Player.IMC_Player'"));
	if (InputMappingContext.Object != nullptr)
	{
		IMC = InputMappingContext.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Move.IA_Move'"));
	if (InputMove.Object != nullptr)
	{
		MoveAction = InputMove.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputLook(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Look.IA_Look'"));
	if (InputLook.Object != nullptr)
	{
		LookAction = InputLook.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_TestUp.IA_TestUp'"));
	if (InputLook.Object != nullptr)
	{
		TestUpAction = InputUp.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputDown(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_TestDown.IA_TestDown'"));
	if (InputLook.Object != nullptr)
	{
		TestDownAction = InputDown.Object;
	}
}

