#pragma once

#include <iostream>

struct Position {
public:
	int X = 0;
	int Y = 0;
	Position() : X{ 0 }, Y{ 0 } {}
	Position(int X, int Y) : X{ X }, Y{ Y } {}
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
		return (size_t)position.Y * this->fieldsX + position.X;
	}

};
