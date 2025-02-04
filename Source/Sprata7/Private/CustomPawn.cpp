#include "CustomPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"

ACustomPawn::ACustomPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetSimulatePhysics(false);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	//멤버 변수 초기화
	MovementSpeed = 600.0f;
	RotationSpeed = 100.0f;
	GravityAcceleration = -980.0f;
	FallingVelocity = 0.0f;
	AirControlFactor = 0.5f;
	bIsGrounded = true;
}

void ACustomPawn::BeginPlay()
{
	Super::BeginPlay();

	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ACustomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_MoveForward)
		{
			EnhancedComp->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ACustomPawn::MoveForward);
		}
		
		if (IA_MoveRight)
		{
			EnhancedComp->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ACustomPawn::MoveRight);
		}

		if (IA_LookAction)
		{
			EnhancedComp->BindAction(IA_LookAction, ETriggerEvent::Triggered, this, &ACustomPawn::Look);
		}

		if (IA_Jump)
		{
			EnhancedComp->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACustomPawn::JumpPressed);
		}
	}
}

void ACustomPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ACustomPawn::UpdateGroundedState(float DeltaTime)
{
	// 이미 위로 낙하 중인 경우, 지면에 있을 수 없음
	if (FallingVelocity > 0.f)
	{
		bIsGrounded = false;
		return;
	}

	// 캡슐 하단 위치
	const float CapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	const FVector CapsuleBottom = GetActorLocation() - FVector(0.f, 0.f, CapsuleHalfHeight);

	// 지면 감지를 위한 아래쪽 라인 트레이스
	const FVector TraceStart = CapsuleBottom;
	const FVector TraceEnd = CapsuleBottom - FVector(0.f, 0.f, GroundCheckOffset); // 하단에서 약간 아래로 오프셋

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this); // 트레이스에서 자신을 무시

	bIsGrounded = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
}

void ACustomPawn::ApplyGravity(float DeltaTime)
{
	if (!bIsGrounded)
	{
		FallingVelocity += GravityAcceleration * DeltaTime; // 낙하 속도에 중력 적용
	}
	else
	{
		FallingVelocity = 0.0f; // 지면에 닿으면 낙하 속도 리셋
	}
}

void ACustomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateGroundedState(DeltaTime);
	ApplyGravity(DeltaTime);

	// 중력 적용!
	const FVector GravityMovement = FVector(0.f, 0.f, FallingVelocity * DeltaTime);
	AddActorWorldOffset(GravityMovement, true);
}

void ACustomPawn::MoveForward(const FInputActionValue& Value)
{
	// 입력 값은 float (앞/뒤)
	const float ForwardInput = Value.Get<float>();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	// 카메라의 전방 벡터 (수평면으로 투영)
	FVector CameraForward = CameraComponent->GetForwardVector();
	CameraForward.Z = 0.f;
	CameraForward.Normalize();

	const float EffectiveSpeed = bIsGrounded ? MovementSpeed : MovementSpeed * AirControlFactor;
	FVector MovementDelta = CameraForward * ForwardInput * EffectiveSpeed * DeltaTime;
	AddActorWorldOffset(MovementDelta, true);
}
void ACustomPawn::MoveRight(const FInputActionValue& Value)
{
	// 입력 값은 float (좌/우)
	const float RightInput = Value.Get<float>();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	// 카메라의 우측 벡터 (수평면으로 투영)
	FVector CameraRight = CameraComponent->GetRightVector();
	CameraRight.Z = 0.f;
	CameraRight.Normalize();

	const float EffectiveSpeed = bIsGrounded ? MovementSpeed : MovementSpeed * AirControlFactor;
	FVector MovementDelta = CameraRight * RightInput * EffectiveSpeed * DeltaTime;
	AddActorWorldOffset(MovementDelta, true);
}

void ACustomPawn::Look(const FInputActionValue& Value)
{
	// 입력 값은 Vector2D (X=Yaw, Y=Pitch)
	const FVector2D LookInput = Value.Get<FVector2D>();
	if (!LookInput.IsNearlyZero())
	{
		const float DeltaTime = GetWorld()->GetDeltaSeconds();

		// 마우스 X 입력으로부터 Yaw 회전
		const float YawRotation = LookInput.X * RotationSpeed * DeltaTime;
		FRotator DeltaRotation(0.f, YawRotation, 0.f);
		AddActorLocalRotation(DeltaRotation);
	}
}

void ACustomPawn::JumpPressed()
{
	if (bIsGrounded)
	{
		FallingVelocity = JumpVelocity;
		bIsGrounded = false;
	}
}
