#pragma once

#include <iostream>
#include <memory>

struct Position {
public:
	int X = 0;
	int Y = 0;
	Position() : X{ 0 }, Y{ 0 } {}
	Position(int X, int Y) : X{ X }, Y{ Y } {}
	Position(const Position& p) {
		X = p.X;
		Y = p.Y;
	}

	Position& operator=(const Position& a)
	{
		X = a.X;
		Y = a.Y;
		return *this;
	}
};

constexpr int getNumberOfPositionsToCheck() {
	return 5;
};

struct PositionCombinationContainer {
	Position positions[getNumberOfPositionsToCheck()];
};

struct IndexCombinationContainer {
	size_t indexes[getNumberOfPositionsToCheck()];
};

struct PositionIndexCombinedContainer {

	size_t const positionAndIndexCombinationsSize;
	std::shared_ptr<const PositionCombinationContainer[]> positionCombinations;

	PositionIndexCombinedContainer(size_t const positionAndIndexCombinationsSize,
	std::shared_ptr<const PositionCombinationContainer[]> positionCombinations) :
		positionAndIndexCombinationsSize { positionAndIndexCombinationsSize },
		positionCombinations { positionCombinations }
	{}
};

template <class T>
class TableLayoutBase {

public:
	const int fieldsX;
	const int fieldsY;
	const size_t numberOfFields;
	std::unique_ptr<T[]> fields;

	TableLayoutBase(const int fieldsX, const int fieldsY):
		fieldsX { fieldsX },
		fieldsY{ fieldsY },
		numberOfFields{ (size_t)fieldsX * fieldsY },
		fields{ new T[numberOfFields] }
	{
		//Intentionally empty constructor body
	}

protected:
	inline size_t positionToIndex(Position position) {
		size_t index = (size_t)position.Y* this->fieldsX + position.X;
		if (index >= numberOfFields) {
			throw std::invalid_argument("index out of bounds!");
		}
		return index;
	}

};
