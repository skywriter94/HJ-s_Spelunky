#include "stdafx.h"
#include "Enemy.h"



Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

HRESULT Enemy::init(POINTFLOAT _pos, float _speed, int _EnemyKind, int _EnemyPosState, int _state, int _enemyDmg, int _maxHP, bool _dir)
{
	pos = { _pos.x, _pos.y };
	posID = { (int)pos.x / 64, (int)pos.y / 64 };
	rc = RectMakeCenter(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());
	speed = _speed;
	kind = (EnemyKind)_EnemyKind;
	posState = (EnemyPosState)_EnemyPosState;
	state = (EnemyAniState)_state;
	dir = _dir;
	dmg = _enemyDmg;
	maxHP = currentHP = _maxHP;
	gravity = jumpPower = 0;
	ani = new animation;
	hitSwitch = true;
	return S_OK;
}

void Enemy::release()
{
}

void Enemy::update()
{

}

void Enemy::render()
{
}



void Enemy::update_Idle()
{
}

void Enemy::update_Move()
{
}

void Enemy::update_Attack()
{
}

void Enemy::update_Hit()
{
}

void Enemy::update_Dead()
{
}



void Enemy::hitDamage(float damage)
{
	currentHP -= damage;
}