#ifndef TABLEVIEWIMPL_H
#define TABLEVIEWIMPL_H

#include "tableview.h"
#include "../table.h"

class TableViewImplementation : public TableView {
private:
	Table* table;
	FieldState myMark;
public:
	TableViewImplementation(Table* table, FieldState myMark) : table{ table }, myMark{ myMark } {};

	int fieldsX() {
		return table->fieldsX;
	}
	int fieldsY() {
		return table->fieldsY;
	}

	FieldView get(Position position) {
		FieldState state = table->get(position);
		if (state == FieldState::Empty) {
			return FieldView::Empty;
		}
		else if (state == FieldState::Cross && myMark == FieldState::Cross) {
			return FieldView::Mine;
		}
		else if (state == FieldState::Naught && myMark == FieldState::Naught) {
			return FieldView::Mine;
		}
		else {
			return FieldView::Opponent;
		}
	}

	PositionIndexCombinedContainer getPositionIndexCombinedContainer() {
		return table->getComboContainer();
	}

};

#endif // TABLEVIEWIMPL_H
