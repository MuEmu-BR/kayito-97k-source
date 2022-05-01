#pragma once

class CWeaponView
{
public:

	CWeaponView();

	~CWeaponView();

	void Init();

	static void WeaponViewCheck();

	static void SecondWeaponViewCheck();

	static void SecondWeaponViewFix();
};

extern CWeaponView gWeaponView;