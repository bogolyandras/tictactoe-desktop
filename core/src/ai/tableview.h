#pragma once
#include "..\basetable.h"

enum class FieldView { Empty, Mine, Opponent };

class TableView {
public:
	virtual int fieldsX() = 0;
	virtual int fieldsY() = 0;
	virtual FieldView get(Position position) = 0;
};
