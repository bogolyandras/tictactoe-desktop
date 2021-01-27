#pragma once

#include "tableview.h"

class Ai {
public:
	virtual Position calculateAnswer(TableView* tableView) = 0;

};
