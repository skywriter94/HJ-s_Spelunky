#pragma once
#include "gameNode.h"

#define SPEED_R 0.1f

class Enemy : public gameNode
{
protected:
	enum EnemyKind
	{
		_Snake,
		_Bat,
		_Spider
	};
	enum EnemyPosState
	{
		_Air,
		_Ground
	};
	enum EnemyAniState
	{
		_Idle,
		_Move,
		_Attack,
		_Hit,
		_Dead
	};

protected:
	image* img;
	animation* ani;
	POINTFLOAT pos;
	POINT posID;
	RECT rc;
	float angle;
	float gravity;
	float jumpPower;
	float speed;
	EnemyKind kind;
	EnemyPosState posState;
	EnemyAniState state;
	int dmg;
	float currentHP, maxHP;
	bool dir;
	bool hitSwitch;

public:
	Enemy();
	~Enemy();
	HRESULT init(POINTFLOAT _pos, float _speed, int _EnemyKind, int _EnemyPosState, int _state, int _enemyDmg, int _maxHP, bool _dir);
	void release();
	virtual void update();
	void render();

public:
	virtual void update_Idle();
	virtual void update_Move();
	virtual void update_Attack();
	virtual void update_Hit();
	virtual void update_Dead();

public:
	inline image* getImage() { return img; }
	inline void setImage(const char* imgKeyName) { img = IMAGEMANAGER->findImage(imgKeyName); }

	inline void setPos(POINTFLOAT _pos) { pos = _pos; }
	inline POINTFLOAT getPos() { return pos; }
	inline POINTFLOAT& getPosPt() { return pos; }

	inline void setKind(int _setKind) { kind = (EnemyKind)_setKind; }
	inline EnemyKind getKind() { return kind; }

	inline EnemyPosState& getPosStatePt() { return posState; }
	inline int& getPosStateToIntPt() { return (int&)posState; }

	inline void setState(int _state) { state = (EnemyAniState)_state; }
	inline EnemyAniState getState() { return state; }

	inline void setDmg(int _dmg) { dmg = _dmg; }
	inline int getDmg() { return dmg; }

	inline void setMaxHP(float _maxHP) { maxHP = _maxHP; }
	inline void setCurrentHP(float _currentHP) { currentHP = _currentHP; }
	inline float getCurrentHP() { return currentHP; }

	inline void setRect(float y, float x) { rc = RectMakeCenter(x, y, img->getFrameX(), img->getFrameY()); }
	inline RECT getRect() { return rc; }
	inline RECT& getRectPt() { return rc; }

	inline void setDir(bool _dir) { dir = _dir; }
	inline bool getDir() { return dir; }

	inline void setPosID(POINT _posID) { posID = _posID; }
	inline POINT getPosID() { return posID; }

	inline void addAni() { ani = new animation; }
	inline void setAni(const char* aniKeyName) { ani = KEYANIMANAGER->findAnimation(aniKeyName); }
	inline animation* getAni() { return ani; }

	inline void setSpeed(float _speed) { speed = _speed; }
	inline float getSpeed() { return speed; }
	inline float& getSpeedPt() { return speed; }

	inline float& getGravityPt() { return gravity; }

	inline float& getJumpPowerPt() { return jumpPower; }

	inline void set_hitSwitch(bool _switch) { hitSwitch = _switch; }
	inline bool get_hitSwitch() { return hitSwitch; }

	void hitDamage(float damage);
};