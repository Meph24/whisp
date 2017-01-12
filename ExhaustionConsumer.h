#pragma once
class ExhaustionConsumer
{
private:

	float capacity;
	float filled;
	float power;

public:
	ExhaustionConsumer(float capacity,float filled);
	~ExhaustionConsumer();

	float getPower();
	
	static float filledToPower(float filledValue);
};
