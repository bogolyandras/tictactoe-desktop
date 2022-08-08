#include "ai.h"

#include <memory>
#include <list>

struct Threat {

	int values[getNumberOfPositionsToCheck() + 1];
	
	Threat() {
		for (int i = 0; i < getNumberOfPositionsToCheck() + 1; i++) {
			values[i] = 0;
		}
	}

	Threat& operator=(const Threat& a)
	{
		for (int i = 0; i < getNumberOfPositionsToCheck() + 1; i++) {
			values[i] = a.values[i];
		}
		return *this;
	}

	bool operator==(Threat a) const {
		for (int i = 0; i < getNumberOfPositionsToCheck() + 1; i++) {
			if (values[i] != a.values[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(Threat a) const {
		return !operator==(a);
	}

	Threat operator+(const Threat& a) const
	{
		Threat b;
		for (int i = 0; i < getNumberOfPositionsToCheck() + 1; i++) {
			b.values[i] = values[i] + a.values[i];
		}
		return b;
	}

	Threat operator-(const Threat& a) const
	{
		Threat b;
		for (int i = 0; i < getNumberOfPositionsToCheck() + 1; i++) {
			b.values[i] = values[i] - a.values[i];
		}
		return b;
	}

};

struct ThreatCombo {

	Threat threatPosedToOpponent;
	Threat threatPosedFromOpponent;

	ThreatCombo& operator=(const ThreatCombo& a)
	{
		threatPosedToOpponent = a.threatPosedToOpponent;
		threatPosedFromOpponent = a.threatPosedFromOpponent;
		return *this;
	}

	bool operator==(ThreatCombo a) const {
		if (threatPosedToOpponent != a.threatPosedToOpponent) {
			return false;
		}
		if (threatPosedFromOpponent != a.threatPosedFromOpponent) {
			return false;
		}
		return true;
	}

	bool operator!=(ThreatCombo a) const {
		return !operator==(a);
	}

	const ThreatCombo operator+(const ThreatCombo& a) const
	{
		ThreatCombo b;
		b.threatPosedToOpponent = threatPosedToOpponent + a.threatPosedToOpponent;
		b.threatPosedFromOpponent = threatPosedFromOpponent + a.threatPosedFromOpponent;
		return b;
	}

	ThreatCombo operator-(const ThreatCombo& a) const
	{
		ThreatCombo b;
		b.threatPosedToOpponent = threatPosedToOpponent - a.threatPosedToOpponent;
		b.threatPosedFromOpponent = threatPosedFromOpponent - a.threatPosedFromOpponent;
		return b;
	}

};

inline size_t positionToIndex(size_t fieldsX, size_t fieldsY, Position position) {
	return (size_t)position.Y * fieldsX + position.X;
}

ThreatCombo calculateThreats(const PositionIndexCombinedContainer c, const size_t fieldsX, const size_t fieldsY, std::shared_ptr<const FieldView[]> table);

class HeuristicAi : public Ai {
public:
	Position calculateAnswer(TableView* tableView) {

		const PositionIndexCombinedContainer c = tableView->getPositionIndexCombinedContainer();

		//Make a copy from the original table
		const int fieldsX = tableView->fieldsX();
		const int fieldsY = tableView->fieldsY();
		const std::shared_ptr<FieldView[]> originalTable(new FieldView[(size_t)fieldsX * fieldsY]);
		const std::shared_ptr<FieldView[]> demonstrativeTable(new FieldView[(size_t)fieldsX * fieldsY]);

		std::list<std::tuple<size_t, Position>> possibleMoves;
		for (int i = 0; i < fieldsX; i++) {
			for (int j = 0; j < fieldsY; j++) {
				const Position p(i, j);
				const size_t index = positionToIndex(fieldsX, fieldsY, p);
				const FieldView fw = tableView->get(p);
				originalTable[index] = fw;
				demonstrativeTable[index] = fw;
				if (fw == FieldView::Empty) {
					possibleMoves.push_back({ index, p });
				}
			}
		}

		std::list<std::tuple<ThreatCombo, Position>> threatList(fieldsX * fieldsY);

		const ThreatCombo originalThreatCombo = calculateThreats(c, fieldsX, fieldsY, originalTable);
		const size_t possibleMovesSize = possibleMoves.size();
		std::list<std::tuple<size_t, Position>>::iterator it;
		int i;
		for (i = 0, it = possibleMoves.begin(); it != possibleMoves.end(); it++, i++)
		{
			const size_t index = std::get<0>(*it);
			const Position position = std::get<1>(*it);

			demonstrativeTable[index] = FieldView::Mine;
			const ThreatCombo diffThreadCombo = calculateThreats(c, fieldsX, fieldsY, demonstrativeTable);
			demonstrativeTable[index] = FieldView::Empty;

			threatList.push_back({ (diffThreadCombo - originalThreatCombo), position });
		}

		return std::get<1>(possibleMoves.front());
	};

};

inline ThreatCombo calculateThreats(
	const PositionIndexCombinedContainer c,
	const size_t fieldsX, const size_t fieldsY,
	std::shared_ptr<const FieldView[]> table) {

	ThreatCombo aggregatedThreatCombo;

	for (int i = 0; i < c.positionAndIndexCombinationsSize; i++) {
		ThreatCombo tc;
		bool canPoseThreatToOpponent = true;
		bool canOpponentPoseThreat = true;
		int threatToOpponent = 0;
		int threatFromOpponent = 0;
		for (int j = 0; j < getNumberOfPositionsToCheck(); j++) {
			const size_t index = positionToIndex(fieldsX, fieldsY, c.positionCombinations[i].positions[j]);
			if (table[index] == FieldView::Mine) {
				canOpponentPoseThreat = false;
			}
			else if (table[index] == FieldView::Opponent) {
				canPoseThreatToOpponent = false;
			}
			else {
				threatToOpponent++;
				threatFromOpponent++;
			}
		}

		if (canPoseThreatToOpponent) {
			tc.threatPosedToOpponent.values[0]++;
		}
		if (canOpponentPoseThreat) {
			tc.threatPosedFromOpponent.values[0]++;
		}
		for (int j = 0; j < getNumberOfPositionsToCheck(); j++) {
			if (canPoseThreatToOpponent && j < threatToOpponent) {
				tc.threatPosedToOpponent.values[j + 1]++;
			}
			if (canOpponentPoseThreat && j < threatFromOpponent) {
				tc.threatPosedFromOpponent.values[j + 1]++;
			}
		}
		aggregatedThreatCombo = aggregatedThreatCombo + tc;
	}

	return aggregatedThreatCombo;
}
