#pragma once

#include "basetable.h"

enum class FieldState { Empty, Cross, Naught };

struct FieldStateWithProperties {
	bool mouseOver = false;
	bool cursorOver = false;
	FieldState fieldState = FieldState::Empty;
};

class Table: public TableLayoutBase<FieldStateWithProperties> {
public:
	int positionX;
	int positionY;
	int positionAsIndex();

	Table(const int fieldsX, const int fieldsY);

	void keyPressUp();
	void keyPressDown();
	void keyPressLeft();
	void keyPressRight();

};
