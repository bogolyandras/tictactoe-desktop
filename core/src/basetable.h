#pragma once

#include <iostream>

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
	inline size_t positionToIndex(int x, int y) {
		return (size_t)y * this->fieldsX + x;
	}

};
