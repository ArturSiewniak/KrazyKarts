// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;
	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;
	UPROPERTY()
	float Time;
};

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FGoKartMove LastMove;
};

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SimulateMove(const FGoKartMove& Move);

	FGoKartMove CreateMove(float DeltaTime);
	void ClearAcknowledgeMoves(FGoKartMove LastMove);

	FVector GetAirResistance();
	FVector GetRollingResistance();
	void ApplyRotation(float DeltaTime, float SteeringThrow);
	void UpdateLocationFromVelocity(float DeltaTime);
	
	//Mass is in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	//The force applied to the car when the throttle is fully down (in N - Newtons - 1 N == 1*kg*m/s^-2)
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	//Minimum radius of the car turning circle at full lock (in m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	//Higher means more air drag
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	//Higher means more rolling resistance drag
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.010;

	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Move);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	FVector Velocity;

	UFUNCTION()
	void OnRep_ServerState();

	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;
	
	float Throttle;
	float SteeringThrow;

	TArray<FGoKartMove> UnacknowlegedMoves;
};
