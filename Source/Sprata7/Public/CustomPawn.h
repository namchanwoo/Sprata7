#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomPawn.generated.h"

struct FInputActionValue;

// 전방 선언
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;


UCLASS()
class SPRATA7_API ACustomPawn : public APawn
{
	GENERATED_BODY()

public:
	ACustomPawn();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

protected:
	void UpdateGroundedState(float DeltaTime);

	void ApplyGravity(float DeltaTime);

	void MoveForward(const FInputActionValue& Value);
	
	void MoveRight(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void JumpPressed();


	/* ----------------	   컴포넌트	   ---------------- */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	UCameraComponent* CameraComponent;

	/* ----------------	   Enhanced Input	   ---------------- */
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input")
	UInputAction* IA_MoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input")
	UInputAction* IA_MoveRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input")
	UInputAction* IA_LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input")
	UInputAction* IA_Jump;

	/* ----------------	   이동/회전/중력 파라미터	   ---------------- */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|Movement")
	float RotationSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Movement")
	float FallingVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|Movement")
	float AirControlFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|Movement")
	double GroundCheckOffset = 10.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Movement")
	bool bIsGrounded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|Gravity")
	float GravityAcceleration; // 중력 가속도 (예: -980.0 cm/s²)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|Gravity")
	float JumpVelocity = 550.0f;
};


