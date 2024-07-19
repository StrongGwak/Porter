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
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APPlayer::APPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
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
	UpdateStats(PlayerAndHeroStats);
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

	// 달릴 수 없으면, 달리지 못한다 -> !끼리 묶는건 의미 x
	if (bIsRun && bCanRun)	// 달리고 있고, 달리기 누를 수 있는 상태
	{
		CurrentStamina -= DecreaseStamina * DeltaTime;
		if (CurrentStamina <= 0)
		{
			bCanRun = false;
			StopRun();
			CurrentStamina = 0;
		}
	}
	else if (!bIsRun && bCanRun && CurrentStamina < MaxStamina) // 달리고 있지 않은데, 달리기 누를 수는 있는 상태 == 휴식타임
	{
		CurrentStamina += IncreaseStamina * DeltaTime;
		if (CurrentStamina > MaxStamina)
		{
			CurrentStamina = MaxStamina;
		}
	}
	else if (!bCanRun)
	{
		CurrentStamina += ZeroToHundredIncreaseSteamin * DeltaTime;
		if (CurrentStamina > MaxStamina)
		{
			CurrentStamina = MaxStamina;
			bCanRun = true;
		}
	}
	
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
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APPlayer::Run);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APPlayer::StopRun);
}

// 블루프린트에서 업데이트 할 사항
void APPlayer::UpdateStats(FPlayerStatsStruct UpdateStat)
{
	PlayerAndHeroStats = UpdateStat;
	
	// 바로 적용해야 할 것들 나중에 따로 정리 필요하긴 함
	// 틱에서 검사하는게 아니니까 이 과정이 필요하다. 
	GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerWalkSpeed;

	// 현재 용병 수가 max를 넘게 될 경우 없애버리기
	while (HeroNum > PlayerAndHeroStats.MaxHeroNum)
	{
		Down();
	}

	MaxHeroHP += PlayerAndHeroStats.PlusMaxHeroHP;
	MaxStamina = PlayerAndHeroStats.PlayerMaxStamina;
	DecreaseStamina = PlayerAndHeroStats.PlayerDecreaseStamina;
	IncreaseStamina = PlayerAndHeroStats.PlayerIncreaseStamina;
	ZeroToHundredIncreaseSteamin = PlayerAndHeroStats.PlayerZeroToHundredIncreaseSteamin;
}

void APPlayer::PlusHP(int32 Heal)
{
	if (LastHeroHP == MaxHeroHP) LastHeroHP = MaxHeroHP; 
	else if (Heal + LastHeroHP >= MaxHeroHP) LastHeroHP = MaxHeroHP;
	else LastHeroHP += Heal;
	CurrentHP = MaxHeroHP * (HeroNum - 1) + LastHeroHP;
}

void APPlayer::MinusHP(int32 Damage)
{
	if (Damage > MaxHp)
	{
		MaxHp = 0;
		CurrentHP = 0;
		Die();
		return;
	}
	int32 NumHeroDie = Damage / MaxHeroHP;
	int32 UpdateLastHeroHP = Damage % MaxHeroHP;
	
	if (LastHeroHP > Damage)
	{
		LastHeroHP -= Damage;
		CurrentHP = MaxHeroHP * (HeroNum - 1) + LastHeroHP;
	}
	else if (LastHeroHP > UpdateLastHeroHP)
	{
		LastHeroHP -= UpdateLastHeroHP;
		for(int i=0; i<NumHeroDie; i++)
		{
			Down();
		}
	}
	else
	{
		LastHeroHP = LastHeroHP + MaxHeroHP - UpdateLastHeroHP;
		for(int i=0; i<NumHeroDie+1; i++)
		{
			Down();
		}
	}

	// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHP));
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

void APPlayer::Run()
{
	if(bCanRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerRunSpeed;
		bIsRun = true;
	}
}

void APPlayer::StopRun()
{
	bIsRun = false;
	GetCharacterMovement()->MaxWalkSpeed = PlayerAndHeroStats.PlayerWalkSpeed;
}

// Hero 1개 생성
void APPlayer::Up()
{
	if (HeroNum < PlayerAndHeroStats.MaxHeroNum)
	{
		HeroNum++;
		SpringArm->TargetArmLength = 400 + PorterFloorArray[HeroNum] * AddCameraLength;
		
		ACharacter* TestHeroBox = GetWorld()->SpawnActor<ACharacter>(HeroBoxSpawner, HeroSpawnLocation->GetComponentLocation() + OffsetArr[HeroNum], HeroSpawnLocation->GetComponentRotation());

		if(TestHeroBox)
		{
			HeroBoxArray.Emplace(TestHeroBox);
		}
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(HeroNum));
		// MaxHp 업데이트
		MaxHp = MaxHeroHP * HeroNum;
		CurrentHP = MaxHeroHP * (HeroNum - 1) + LastHeroHP;
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHP));
	}
}

// Hero 1개 파괴
void APPlayer::Down()
{
	if (HeroNum > 0)
	{
		HeroNum--;
		SpringArm->TargetArmLength = 400 + PorterFloorArray[HeroNum] * AddCameraLength;

		if(HeroBoxArray.Num() > 0)
		{
			ACharacter* LastHeroBox = HeroBoxArray.Last();

			if(LastHeroBox)
			{
				LastHeroBox->Destroy();
				HeroBoxArray.Pop();
			}
		}
		// MaxHp 업데이트
		MaxHp = MaxHeroHP * HeroNum;
		CurrentHP = MaxHeroHP * (HeroNum - 1) + LastHeroHP;
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::FromInt(CurrentHP));
	}
}

// 생성자에서 미리 배열 만들어두기
void APPlayer::MakeArrays()
{
	int PorterFloor = 0;
	for(int TempHeroNum=0; TempHeroNum<15+1; TempHeroNum++)
	{
		// Hero 수에 따른 카메라 레벨 == 쌓은 층 수
		PorterFloor = 0;
		while (true)
		{
			if (TempHeroNum <= PorterFloor * (PorterFloor + 1) / 2) break;
			else PorterFloor++;
		}
		// 배열 추가
		PorterFloorArray.Emplace(PorterFloor);
		
		// 층수로 계산하는 스폰 위치 Offset
		float OffsetY = 0;
		float CurrentSpawnNum = TempHeroNum - (PorterFloor-1)*PorterFloor / 2;
		int ForCount = round(PorterFloor);
		for(int i = 0; i<round(CurrentSpawnNum); i++)
		{
			if (i%2 == 1) OffsetY = -1*OffsetY - 2*PorterWidth;
			else OffsetY *= -1;
		}
		if (ForCount%2 == 0) OffsetY += PorterWidth;
		OffsetArr.Emplace(FVector(-100, OffsetY, PorterFloor*PorterHeight));
	}
}

void APPlayer::FObjectFinderInputManager()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>InputMappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Porter/Develop/Inputs/IMC_Player.IMC_Player'"));
	if (InputMappingContext.Object != nullptr)
	{
		IMC = InputMappingContext.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputMove(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Move.IA_Move'"));
	if (InputMove.Object != nullptr)
	{
		MoveAction = InputMove.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputLook(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Look.IA_Look'"));
	if (InputLook.Object != nullptr)
	{
		LookAction = InputLook.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_TestUp.IA_TestUp'"));
	if (InputLook.Object != nullptr)
	{
		TestUpAction = InputUp.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputDown(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_TestDown.IA_TestDown'"));
	if (InputLook.Object != nullptr)
	{
		TestDownAction = InputDown.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputRun(TEXT("/Script/EnhancedInput.InputAction'/Game/Porter/Develop/Inputs/IA_Run.IA_Run'"));
	if (InputLook.Object != nullptr)
	{
		RunAction = InputRun.Object;
	}
}

void APPlayer::Die()
{
	// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("YOU DIE"));
}

