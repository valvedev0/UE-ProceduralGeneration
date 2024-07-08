// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralRoom.generated.h"

UCLASS()
class PROCGEN_API AProceduralRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room,  meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ChairClass;

	void SpawnItem(UClass* ItemToSpawn);

	//lets create a grid
	void CreateGrid();

	//variables for debugging
	float RoomLength;
	float RoomWidth;
	float GridHeight;
	float SquareWidth;
	float Radius;

	int32 GSX;
	int32 GSY;

	FVector TopLeft;
	FVector BottomRight;

	//lets find out a random point in sqaures and place the chair in grid
	FVector GetRandomPointInSquare(const FVector& UpperlLeft, const FVector& LowerRight);
	void PlacePointOnGrid();

};
