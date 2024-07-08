#pragma once
//a floor node class that can be used as a data structure for storing the coordinates of the corners of ach square

//struct for storing the coordinates
struct FCornerCoordinates {
	int32 UpperLeftX;
	int32 UpperLeftY;
	int32 LowerRightX;
	int32 LowerRightY;
};


class FloorNode {

public:

	FloorNode();

	//assinging values to the cordinates fn
	FloorNode(const FCornerCoordinates& Coordinates );

	~FloorNode();

	//a getter and setter fn for coordinates
	FORCEINLINE FCornerCoordinates GetCornerCoordinates() const { return CornerCoordinates; }
	FORCEINLINE void SetCornerCoordinates(FCornerCoordinates Coordinates) {CornerCoordinates = Coordinates;}
	FORCEINLINE static int32 GetFloorNodeCount() { return FloorNodeCount; }
private:
	//creating a private variable for struct cornercoordinates
	FCornerCoordinates CornerCoordinates;

	static int32 FloorNodeCount;

	/*
	
	*/

};