#pragma once

#include "ai.h"

#include <memory>
#include <list>
#include <algorithm>

struct Threat {

	int values[getNumberOfPositionsToCheck() + 1];
	
	Threat() {
		for (int i = 0; i < getNumberOfPositionsToCheck() + 1; i++) {
			values[i] = 0;
		}
	}

	Threat(const Threat& t) {
		for (int i = 0; i < getNumberOfPositionsToCheck() + 1; i++) {
			values[i] = t.values[i];
		}
	}

	Threat& operator=(const Threat& a)
	{
		for (int i = 0; i < getNumberOfPositionsToCheck() + 1; i++) {
			values[i] = a.values[i];
		}
		return *this;
	}

	bool operator==(const Threat a) const {
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

	ThreatCombo() {
		threatPosedToOpponent = Threat();
		threatPosedFromOpponent = Threat();
	}

	ThreatCombo(const ThreatCombo& a)
	{
		threatPosedToOpponent = a.threatPosedToOpponent;
		threatPosedFromOpponent = a.threatPosedFromOpponent;
	}

	ThreatCombo& operator=(const ThreatCombo& a)
	{
		threatPosedToOpponent = a.threatPosedToOpponent;
		threatPosedFromOpponent = a.threatPosedFromOpponent;
		return *this;
	}

	bool operator==(const ThreatCombo a) const {
		if (threatPosedToOpponent != a.threatPosedToOpponent) {
			return false;
		}
		if (threatPosedFromOpponent != a.threatPosedFromOpponent) {
			return false;
		}
		return true;
	}

	bool operator<(const ThreatCombo a) const {
		for (int i = getNumberOfPositionsToCheck() + 1; i > 0; i--) {
			if (threatPosedFromOpponent.values[i - 1] > a.threatPosedFromOpponent.values[i - 1]) {
				return true;
			}
			else if (threatPosedFromOpponent.values[i - 1] < a.threatPosedFromOpponent.values[i - 1]) {
				return false;
			} else if (threatPosedToOpponent.values[i - 1] < a.threatPosedToOpponent.values[i - 1]) {
				return true;
			}
			else if (threatPosedToOpponent.values[i - 1] > a.threatPosedToOpponent.values[i - 1]) {
				return false;
			}
		}
		return false;
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

struct ThreatComboWithPosition {

	ThreatCombo threatCombo;
	Position position;

	ThreatComboWithPosition()
		: threatCombo{ ThreatCombo() }, position{ Position() } {}

	ThreatComboWithPosition(ThreatCombo threatCombo, Position position)
		: threatCombo{ threatCombo }, position{ position } {}

	ThreatComboWithPosition(const ThreatComboWithPosition& a)
	{
		threatCombo = a.threatCombo;
		position = a.position;
	}

	bool operator<(const ThreatComboWithPosition a) const {
		return threatCombo < a.threatCombo;
	}

	ThreatComboWithPosition& operator=(const ThreatComboWithPosition& a)
	{
		threatCombo = a.threatCombo;
		position = a.position;
		return *this;
	}

};

inline size_t positionToIndex(size_t fieldsX, size_t fieldsY, Position position) {
	return (size_t)position.Y * fieldsX + position.X;
}

class HeuristicAi : public Ai {
public:
	Position calculateAnswer(TableView* tableView);
private:
	ThreatCombo calculateThreats(const PositionIndexCombinedContainer c, const size_t fieldsX, const size_t fieldsY, std::shared_ptr<const FieldView[]> table);
};
