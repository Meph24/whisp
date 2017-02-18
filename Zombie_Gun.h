#pragma once
class Zombie_Gun
{
	float rld;
	float timer;
public:
	float baseDmg;
	float projectileSpeed;
	Zombie_Gun(float ProjectileSpeed, float BaseDmg,float ReloadTime);
	~Zombie_Gun();
	bool tryShoot();
	void tick(float sec);
};

