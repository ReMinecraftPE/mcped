/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "world/phys/AABB.hpp"
#include "world/phys/Vec3.hpp"
#include "world/level/material/Material.hpp"
#include "world/level/tile/Tile.hpp"
#include "world/item/ItemInstance.hpp"

class Level;
class Player;
class ItemInstance;

enum eEntityRenderType
{
	RENDER_NONE,
	RENDER_TNT,
	RENDER_HUMANOID,
	RENDER_ITEM,
	RENDER_CAMERA,

	// custom
	RENDER_FALLING_TILE = 50,
};

struct EntityPos
{
	Vec3 pos;
	float m_yaw = 0, m_pitch = 0;
	bool m_bHasRot = false, m_bHasPos = false;

	EntityPos() {};

	EntityPos(const Vec3& pos)
	{
		this->pos = pos;
		m_bHasPos = true;
	}

	EntityPos(float yaw, float pitch)
	{
		m_yaw = yaw;
		m_pitch = pitch;
		m_bHasRot = true;
	}

	EntityPos(const Vec3& pos, float yaw, float pitch)
	{
		this->pos = pos;
		m_yaw = yaw;
		m_pitch = pitch;
		m_bHasPos = true;
		m_bHasRot = true;
	}
};

class Entity
{
public:
	Entity(Level*);
	virtual ~Entity();
	virtual void setLevel(Level*);

	virtual void remove();
	virtual void setPos(float x, float y, float z);
	virtual int move(float x, float y, float z);
	virtual void moveTo(float x, float y, float z, float yaw, float pitch);
	virtual void absMoveTo(float x, float y, float z, float yaw, float pitch);
	virtual void moveRelative(float x, float z, float y);
	virtual void lerpTo(float x, float y, float z, float yaw, float pitch, int i);
	virtual void lerpMotion(float x, float y, float z);
	virtual void turn(float yaw, float pitch);
	virtual void interpolateTurn(float yaw, float pitch);
	virtual void tick();
	virtual void baseTick();
	virtual bool intersects(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	virtual bool isFree(float offX, float offY, float offZ);
	virtual bool isFree(float offX, float offY, float offZ, float expand);
	virtual bool isInWall();
	virtual bool isInWater();
	virtual bool isInLava();
	virtual bool isUnderLiquid(Material*);
	virtual float getHeadHeight();
	virtual float getShadowHeightOffs();
	virtual float getBrightness(float f);
	virtual float distanceTo(Entity*);
	virtual float distanceToSqr(float x, float y, float z);
	virtual float distanceTo(float x, float y, float z);
	virtual float distanceToSqr(Entity*);
	virtual int interactPreventDefault();
	virtual bool interact(Player*);
	virtual void playerTouch(Player*);
	virtual void push(Entity*);
	virtual void push(float x, float y, float z);
	virtual bool isPickable();
	virtual bool isPushable();
	virtual bool isShootable();
	virtual bool isSneaking();
	virtual bool isAlive();
	virtual bool isOnFire();
	virtual bool isPlayer();
	virtual bool isCreativeModeAllowed();
	virtual bool shouldRender(Vec3& camPos);
	virtual bool shouldRenderAtSqrDistance(float distSqr);
	virtual bool hurt(Entity*, int);
	virtual void animateHurt();
	virtual float getPickRadius();
	// virtual void spawnAtLocation(ItemInstance*, float);
	// virtual void spawnAtLocation(int, int);
	// virtual void spawnAtLocation(int, int, float);
	virtual void awardKillScore(Entity* pKilled, int score);
	virtual void setEquippedSlot(int, int, int);
	virtual void setRot(float yaw, float pitch);
	virtual void setSize(float rad, float height);
	virtual void setPos(EntityPos*);
	virtual void resetPos();
	virtual void outOfWorld();
	virtual void checkFallDamage(float f, bool b);
	virtual void causeFallDamage(float f);
	virtual void markHurt();
	virtual void burn(int);
	virtual void lavaHurt();

	int hashCode();

	bool operator==(const Entity& other) const;

	float distanceToSqr_inline(float x, float y, float z) const
	{
		return
			(pos.x - x) * (pos.x - x) +
			(pos.y - y) * (pos.y - y) +
			(pos.z - z) * (pos.z - z);
	}

public:
	static int entityCounter;
	static Random sharedRandom;

	Vec3 pos;
	bool inChunk = false;
	int xChunk = 0;
	int yChunk = 0;
	int zChunk = 0;
	int xp = 0;
	int yp = 0;
	int zp = 0;
	int entityId;
	float viewScale = 1.0f;
	uint8_t field_34 = 0;
	Level* level = nullptr;
	Vec3 posO;
	Vec3 vel;
	float yRot = 0.0f;
	float xRot = 0.0f;
	//maybe these are the actual m_yaw and m_pitch, and
	//the one I annotated are the destination yaw and pitch.
	//interpolateTurn doesn't modify them, so I highly suspect
	//this to be the case.
	float yRotO = 0.0f;
	float xRotO = 0.0f;
	AABB bb;
	bool onGround = false;
	bool horizontalCollision = false;
	bool verticalCollision = false;
	bool collision = false;
	bool hurtMarked = false;
	bool slide = true;
	bool removed = false;
	float heightOffset = 0.0f;
	float bbWidth = 0.6f;
	float bbHeight = 1.8f;
	float walkDistO = 0.0f;
	float walkDist = 0.0f;
	Vec3 posOld;
	float ySlideOffset = 0.0f;
	float footSize = 0.0f;
	bool noPhysics = false;
	float pushthrough = 0.0f;
	int tickCount = 0;
	int field_B8 = 0;
	int airSupply = 300;
	int onFire = 0;
	int flameTime;
	int field_C8 = 0;  // @NOTE: Render type? (eEntityRenderType)
	float fallDistance = 0.0f;
	int airCapacity = 300;
	uint8_t field_D4 = 0;
	bool field_D5 = false;
	bool field_D6 = true;
	int field_D8 = 1;
};
