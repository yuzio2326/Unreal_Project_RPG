// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AdvanceFloatingPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class KDT3D_API UAdvanceFloatingPawnMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()
	
public:
	UAdvanceFloatingPawnMovement();

	/** Returns how far to rotate character during the time interval DeltaTime. */
	virtual FRotator GetDeltaRotation(float DeltaTime) const;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PhysicsRotation(float DeltaTime);

	virtual bool CheckFalling(float DeltaTime);
	virtual bool IsFalling() const override;
	virtual bool IsMovingOnGround() const override;
	virtual void MoveToLocation(FVector NewTargetLocation, float Speed);
	virtual void MoveToLocation(float fTime, float Speed);

	virtual FVector NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const;
	
	/**
	  * Compute a target rotation based on current movement. Used by PhysicsRotation() when bOrientRotationToMovement is true.
	  * Default implementation targets a rotation based on Acceleration.
	  *
	  * @param CurrentRotation	- Current rotation of the Character
	  * @param DeltaTime		- Time slice for this movement
	  * @param DeltaRotation	- Proposed rotation change based simply on DeltaTime * RotationRate
	  *
	  * @return The target rotation given current movement.
	  */
	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const;
public:
	virtual void LaunchPawn(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);
	virtual FVector LaunchVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const;
protected:
	/**
	 * Current acceleration vector (with magnitude).
	 * This is calculated each update based on the input vector and the constraints of MaxAcceleration and the current movement mode.
	 */
	UPROPERTY()
	FVector AccelerationAdvance;

	/**
	 * If true, rotate the Character toward the direction of acceleration, using RotationRate as the rate of rotation change. Overrides UseControllerDesiredRotation.
	 * Normally you will want to make sure that other settings are cleared, such as bUseControllerRotationYaw on the Character.
	 */
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	uint8 bOrientRotationToMovement : 1;

	/** Change in rotation per second, used when UseControllerDesiredRotation or OrientRotationToMovement are true. Set a negative value for infinite rotation rate and instant turns. */
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	FRotator RotationRate;
public:
	bool bFalling = false;
	UPROPERTY(EditAnywhere)
	float FallingDeltaTime = 0.f;	//체크
	FVector TargetLocation = FVector(0, 0, 0);
	float fSpeed = 0;
	float MovingTime = 0;
	bool bIsMoving = false;
	bool bDetectLocation = false;
	bool bLaunch = false;
	FVector LaunchVel = FVector(0, 0, 0);

};
