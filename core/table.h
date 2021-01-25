#pragma once

#include "basetable.h"

enum FieldState { Empty, Cross, Naught };

class Table: public TableLayoutBase<FieldState> {
public:
	Table(const int fieldsX, const int fieldsY);



};