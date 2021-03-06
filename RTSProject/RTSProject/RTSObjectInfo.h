/**
 * RTSObjectInfo
 * Object에 대한 다양한 정보들이 저장됨
 */

#pragma once

class RTSObjectInfo
{
public:
											RTSObjectInfo();
										   ~RTSObjectInfo();

	void									SetType(int t) { mType = t;}
	int										GetModel();
	
	void									SetHealth(int h) { health = h; }
	void									SetSpeed(double s) { speed = s; }
	void									SetDamege(double d) { damege = d; }
	void									SetDefense(double d) { damege = d; }
	void									SetAttackRange(double r) { range = r; }
	void									SetAttackSpeed(double s) { attackspeed = s; }
	void									SetSight(double s) { sight = s; }

	int										GetHealth() { return health; }
	double									GetDamege() { return damege; }
	double									GetSpeed() { return speed; }
	double									GetDefense() { return defense; }
	double									GetAttackRange() { return range; }
	double									GetAttackSpeed() { return attackspeed; }
	double									GetSight() { return sight; }

private:
	int										mType;
	int										health;
	double									attackspeed;
	double									sight;
	double									range;
	double									defense;
	double									speed;
	double									damege;
};

