//Floor class for using the splitting algorithm 
#pragma once

#include "FloorNode.h" 

enum class ESplitOrientation
{
	ESO_Horizontal,
	ESO_Vertical
};

//class FloorNode;

class Floor
{
public:
	
	Floor();
	~Floor();

	void Partition();
	int32 CoinFlip();
	bool ShouldSplitNode(TSharedPtr<FloorNode> InNode, ESplitOrientation Orientation);
	bool SplitAttempt(TSharedPtr<FloorNode> InNode);
	void SplitHorizontal(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC);
	void SplitVertical(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC);

	FORCEINLINE TArray<TSharedPtr<FloorNode>> GetPartitionedFloor() const { return PartitionedFloor; }
	
	void DrawFloorNodes(class UWorld* World);
	void DrawFloorNode(class UWorld* World, FCornerCoordinates Coordinates);

	float UnitLength;
	float SplitChance;

private:
	TArray<TSharedPtr<FloorNode>> FloorNodeStack;
	TArray<TSharedPtr<FloorNode>> PartitionedFloor;
	
	int32 FloorGridSizeX;
	int32 FloorGridSizeY;
	int32 RoomMinX;
	int32 RoomMinY;


};