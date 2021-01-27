#include "ai.h"

class RandomAi : public Ai {
public:
	Position calculateAnswer(TableView* tableView) {
		const int fieldsX = tableView->fieldsX();
		const int fieldsY = tableView->fieldsY();
		for (int i = 0; i < fieldsX; i++) {
			for (int j = 0; j < fieldsY; j++) {
				if (tableView->get(Position(i, j)) == FieldView::Empty) {
					return Position(i, j);
				}
			}
		}
		return Position(0, 0);
	};

};
