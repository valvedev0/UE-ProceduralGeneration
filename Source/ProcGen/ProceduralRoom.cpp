// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralRoom.h"
#include "DrawDebugHelpers.h"
#include "FloorNode.h"
#include "Floor.h"

// Sets default values
AProceduralRoom::AProceduralRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	SetRootComponent(FloorMesh);


	//declareing the debug variables

	RoomLength = 2000.f;
	RoomWidth = 2000.f;
	GridHeight = 1.f;
	SquareWidth = 400.f;
	Radius = 50.f;

	GSX = 5;
	GSY = 5;

	TopLeft = FVector(0.f);
	BottomRight = FVector(2000.f, 2000.f, 0.f);
}

// Called when the game starts or when spawned
void AProceduralRoom::BeginPlay()
{
	Super::BeginPlay();
	
	//SpawnItem(ChairClass);
	//CreateGrid();
	//PlacePointOnGrid();

	TSharedPtr<Floor> TheFloor(new Floor);
	TheFloor->Partition();
	TheFloor->DrawFloorNodes(GetWorld());
	UE_LOG(LogTemp, Warning, TEXT("No. of Partiotions in FloorNodeStack %d"), TheFloor->GetPartitionedFloor().Num());
	
	/*FloorNode* Node = new FloorNode();
	delete Node;*/
	//creating an instance of floornode class using shared ptr for effiecinet moemory usage
	/*{
		UE_LOG(LogTemp, Warning, TEXT("FlorNode Count: %d"), FloorNode::GetFloorNodeCount());
		TSharedPtr<FloorNode> UniqueNodePtr(new FloorNode());
		UE_LOG(LogTemp, Warning, TEXT("FlorNode Count: %d"), FloorNode::GetFloorNodeCount());
	}
	UE_LOG(LogTemp, Warning, TEXT("FlorNode Count: %d"), FloorNode::GetFloorNodeCount());
	UE_LOG(LogTemp, Warning, TEXT("Still in BP"));*/
}

// Called every frame
void AProceduralRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralRoom::SpawnItem(UClass* ItemToSpawn)
{

	float XC = FMath::FRandRange(500.f, 2000.f);
	float YC = FMath::FRandRange(600.f, 1600.f);

	float Yaw = FMath::FRandRange(0.f, 360.f);


	FVector Location(XC, YC, 0.f);
	FRotator Rotation(0.f, Yaw, 0.f);

	GetWorld()->SpawnActor<AActor>(ItemToSpawn, Location, Rotation);
}

void AProceduralRoom::CreateGrid()
{
	//looping throught the roomleth and width to create debug lines


	//first drawing vertical debug lines
	for (int32 i = 0; i <= GSX; i++)
	{
		FVector Start = TopLeft + FVector(i * SquareWidth, 0.f, GridHeight);
		FVector End = Start + FVector(0.f, RoomWidth, GridHeight);
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, true);
	}

	//drawing horizontal lines in 
	for (int32 i = 0; i <= GSY; i++)
	{
		FVector Start = TopLeft + FVector(0.f, i * SquareWidth, GridHeight);
		FVector End = Start + FVector(RoomLength, 0.f, GridHeight);
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, true);
	}
}



FVector AProceduralRoom::GetRandomPointInSquare(const FVector& UpperlLeft, const FVector& LowerRight)
{
	float RandomX = FMath::FRandRange(UpperlLeft.X, LowerRight.X);
	float RandomY = FMath::FRandRange(UpperlLeft.Y, LowerRight.Y);

	return FVector(RandomX, RandomY, 0.f);
}

void AProceduralRoom::PlacePointOnGrid()
{
	for (int32 i = 0; i < GSX; i++)
	{
		for (int32 j = 0; j < GSY; j++)
		{
			FVector UpperLeft(i * SquareWidth + Radius, j * SquareWidth + Radius, GridHeight);
			FVector LowerRight(i * SquareWidth + SquareWidth - Radius, j * SquareWidth + SquareWidth - Radius, GridHeight);
			FVector RandomPointInRange = GetRandomPointInSquare(UpperLeft, LowerRight);
		
			DrawDebugPoint(GetWorld(), RandomPointInRange, 5.0f, FColor::Red, true);

			float RandomYaw = FMath::FRandRange(0.f, 360.f);
			FRotator Rotation(0.f, RandomYaw, 0.f);

			GetWorld()->SpawnActor<AActor>(ChairClass, RandomPointInRange, Rotation);
		}
	}
}

