#include "SimulationInputStatusSet.hpp"

void SimulationInputStatusSet::toggleCondition(bool& b) { b = (b) ? false : true; }
void SimulationInputStatusSet::setConditionTrue(bool& b) { b = true; }
void SimulationInputStatusSet::setConditionFalse(bool& b) { b = false; }

