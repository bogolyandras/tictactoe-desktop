#ifndef AI_H
#define AI_H

#include "tableview.h"

class Ai {
public:
	virtual Position calculateAnswer(TableView* tableView) = 0;

};

#endif // AI_H
