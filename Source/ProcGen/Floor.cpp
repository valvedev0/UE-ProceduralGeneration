#include "Floor.h"
#include "FloorNode.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

Floor::Floor()
{
	FloorGridSizeX = 25;
	FloorGridSizeY = 25;
	RoomMinX = 1;
	RoomMinY = 1;

	UnitLength = 200.f;
	SplitChance = 1.5f;
}

Floor::~Floor()
{
}

void Floor::Partition()
{
	FCornerCoordinates CornerCoordinatesA = { 0, 0, FloorGridSizeX, FloorGridSizeY };
	FloorNodeStack.Push(TSharedPtr<FloorNode>(new FloorNode(CornerCoordinatesA)));
	while (FloorNodeStack.Num() > 0)
	{
		TSharedPtr<FloorNode> A = FloorNodeStack.Pop();

		bool bNodeWasSplit = SplitAttempt(A);
		if (!bNodeWasSplit)
		{
			PartitionedFloor.Push(A);
			UE_LOG(LogTemp, Warning, TEXT("Pushing in  the Partion stack"));
		}

		/*if (ShouldSplitNode(A, ESplitOrientation::ESO_Horizontal))
		{
			UE_LOG(LogTemp, Warning, TEXT("Poping in the FloorNodestack"));
		}
		else
		{
			PartitionedFloor.Push(A);
			UE_LOG(LogTemp, Warning, TEXT("Pushing in  the Partion stack"));
		}
		UE_LOG(LogTemp, Warning, TEXT("Node in existence: %d"), FloorNode::GetFloorNodeCount());
		UE_LOG(LogTemp, Warning, TEXT("Popping the stack"));*/
	}
}

int32 Floor::CoinFlip()
{
	return FMath::RandRange(0, 1);
}

bool Floor::ShouldSplitNode(TSharedPtr<FloorNode> InNode, ESplitOrientation Orientation)
{
	FCornerCoordinates CornerCoordinates = InNode->GetCornerCoordinates();
	if (Orientation == ESplitOrientation::ESO_Horizontal)
	{
		int32 FloorLength = CornerCoordinates.LowerRightY - CornerCoordinates.UpperLeftY;
		float PercentageChanceOfSplit = (float)FloorLength / (float)FloorGridSizeY;
		PercentageChanceOfSplit *= SplitChance;
		float DiceRoll = FMath::FRandRange(0.f, 1.f);
		if (DiceRoll > PercentageChanceOfSplit)
		{
			return false;
		}
		if (FloorLength >= 2*RoomMinY)
		{
			return true;
		}
	}
	if (Orientation == ESplitOrientation::ESO_Vertical)
	{
		int32 FloorWidth = CornerCoordinates.LowerRightX - CornerCoordinates.UpperLeftX;
		float PercentageChanceOfSplit = (float)FloorWidth / (float)FloorGridSizeX;
		PercentageChanceOfSplit *= SplitChance;
		float DiceRoll = FMath::FRandRange(0.f, 1.f);
		if (DiceRoll > PercentageChanceOfSplit)
		{
			return false;
		}
		if (FloorWidth >= 2 * RoomMinY)
		{
			return true;
		}
	}
	return false;
}

bool Floor::SplitAttempt(TSharedPtr<FloorNode> InNode)
{
	int32 HOV = CoinFlip();
	if (HOV == 0)
	{
		//try horizontalyy first
		if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Horizontal))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitHorizontal(InNode, B, C);
			return true;
		}
		else if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Vertical))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitVertical(InNode, B, C);
			return true;
		}
	}
	else if (HOV == 1)
	{
		//try vertically first
		if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Vertical))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitVertical(InNode, B, C);
			
			return true;
		}
		else if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Horizontal))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitHorizontal(InNode, B, C);
			
			return true;
		}
	}
	return false;
} 

void Floor::SplitHorizontal(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC)
{
	//todo: set SplitPointY, and sect cc of b
	int32 SplitPointY = FMath::RandRange(InA->GetCornerCoordinates().UpperLeftY + RoomMinY, InA->GetCornerCoordinates().LowerRightY - RoomMinY);

	FCornerCoordinates CornerCoordinatesB;
	
	CornerCoordinatesB.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesB.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesB.LowerRightX = InA->GetCornerCoordinates().LowerRightX;
	CornerCoordinatesB.LowerRightY = SplitPointY;

	InB->SetCornerCoordinates(CornerCoordinatesB);
	FloorNodeStack.Push(InB);

	FCornerCoordinates CornerCoordinatesC;

	CornerCoordinatesC.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesC.UpperLeftY = SplitPointY;
	CornerCoordinatesC.LowerRightX = InA->GetCornerCoordinates().LowerRightX;
	CornerCoordinatesC.LowerRightY = InA->GetCornerCoordinates().LowerRightY;

	InC->SetCornerCoordinates(CornerCoordinatesC);
	FloorNodeStack.Push(InC);

}

void Floor::SplitVertical(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC)
{
	int32 SplitPointX = FMath::RandRange(InA->GetCornerCoordinates().UpperLeftX + RoomMinX, InA->GetCornerCoordinates().LowerRightX - RoomMinX);

	FCornerCoordinates CornerCoordinatesB;

	CornerCoordinatesB.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesB.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesB.LowerRightX = SplitPointX;
	CornerCoordinatesB.LowerRightY = InA->GetCornerCoordinates().LowerRightY;

	InB->SetCornerCoordinates(CornerCoordinatesB);
	FloorNodeStack.Push(InB);

	FCornerCoordinates CornerCoordinatesC;

	CornerCoordinatesC.UpperLeftX = SplitPointX;
	CornerCoordinatesC.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesC.LowerRightX = InA->GetCornerCoordinates().LowerRightX;
	CornerCoordinatesC.LowerRightY = InA->GetCornerCoordinates().LowerRightY;

	InC->SetCornerCoordinates(CornerCoordinatesC);
	FloorNodeStack.Push(InC);
}

void Floor::DrawFloorNodes(UWorld* World)
{
	for (int32 i = 0; i < PartitionedFloor.Num(); i++)
	{
		FCornerCoordinates Coordinates = PartitionedFloor[i]->GetCornerCoordinates();
		DrawFloorNode(World, Coordinates);
	}
}

void Floor::DrawFloorNode(UWorld* World, FCornerCoordinates Coordinates)
{
	/*define UL, UR, LL, LR
	UL = ULX-ULY
	UR = LrX-UlY
	LL = ULX-LrY
	LR = LRX- LRY
	DDL

	*/
	

	const FVector UpperLeft(Coordinates.UpperLeftX * UnitLength, Coordinates.UpperLeftY * UnitLength, 10.f);
	const FVector UpperRight(Coordinates.LowerRightX * UnitLength, Coordinates.UpperLeftY * UnitLength, 10.f);
	const FVector LowerLeft(Coordinates.UpperLeftX * UnitLength, Coordinates.LowerRightY * UnitLength, 10.f);
	const FVector LowerRight(Coordinates.LowerRightX * UnitLength, Coordinates.LowerRightY * UnitLength, 10.f);

	DrawDebugLine(World, UpperLeft, UpperRight, FColor::Blue, true, -1.0f, 2.5f);
	DrawDebugLine(World, UpperLeft, LowerLeft, FColor::Blue, true, -1.0f, 2.5f);
	DrawDebugLine(World, LowerLeft, LowerRight, FColor::Blue, true, -1.0f, 2.5f);
	DrawDebugLine(World, UpperRight, LowerRight, FColor::Blue, true, -1.0f, 2.5f);
	

}
