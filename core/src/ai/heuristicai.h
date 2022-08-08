#include "ai.h"

#include <memory>
#include <list>

struct Threat {

	int values[6];

};

struct ThreatCombo {

	Threat threadPosedToOpponent;
	Threat threadDecreaseFromOpponent;

};

inline size_t positionToIndex(size_t fieldsX, size_t fieldsY, Position position) {
	return (size_t)position.Y * fieldsX + position.X;
}

ThreatCombo calculateThreats(size_t fieldsX, size_t fieldsY, std::shared_ptr<FieldView[]> table);

class HeuristicAi : public Ai {
public:
	Position calculateAnswer(TableView* tableView) {

		Position toReturn(0, 0);

		//Make a copy from the original table
		const int fieldsX = tableView->fieldsX();
		const int fieldsY = tableView->fieldsY();
		const std::shared_ptr<FieldView[]> originalTable(new FieldView[fieldsX * fieldsY]);
		for (int i = 0; i < fieldsX; i++) {
			for (int j = 0; j < fieldsY; j++) {
				originalTable[positionToIndex(fieldsX, fieldsY, Position(i, j))] = tableView->get(Position(i, j));
				if (originalTable[positionToIndex(fieldsX, fieldsY, Position(i, j))] == FieldView::Empty) {
					toReturn = Position(i, j);
				}
			}
		}

		const ThreatCombo originalThreatCombo = calculateThreats(fieldsX, fieldsY, originalTable);

		//Store all the evaluations here
		std::list<std::tuple<ThreatCombo, Position>> mylist(fieldsX * fieldsY);

		


		return toReturn;
	};

};

inline ThreatCombo calculateThreats(size_t fieldsX, size_t fieldsY, std::shared_ptr<FieldView[]> table) {

	for (int x = 0; x < fieldsX; x++) {
		for (int y = 0; y < fieldsY; y++) {
			//to the right
			for (int k = 0; k < 5; k++) {
				FieldView f = table[positionToIndex(fieldsX, fieldsY, Position(x + k, y))];
			}

			//to the left
			for (int k = 0; k < 5; k++) {
				table[positionToIndex(fieldsX, fieldsY, Position(x + k, y))];
			}
		}
	}

	return ThreatCombo();

}