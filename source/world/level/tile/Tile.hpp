/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include <string>
#include <vector>

#include "client/math/Random.hpp"
#include "world/phys/AABB.hpp"
#include "world/level/material/Material.hpp"
#include "world/level/storage/LevelSource.hpp"
#include "world/entity/Entity.hpp"

class Level;
class Entity;
class Mob;
class Player;
class LiquidTile;

class Tile
{
public: // structs
	struct SoundType
	{
		std::string name;
		float volume, pitch;

		SoundType(const std::string& _name, float _volume, float _pitch) : name(_name), volume(_volume), pitch(_pitch) {}

		std::string getBreakSound() const {
			return "step." + name;
		}
		std::string getStepSound() const {
			return "step." + name;
		}
	};

public: // virtual functions
	virtual ~Tile();
	virtual bool isCubeShaped();
	virtual int getRenderShape();
	virtual Tile* setShape(float, float, float, float, float, float);
	virtual void updateShape(LevelSource*, int, int, int);
	virtual void updateDefaultShape();
	virtual void addLights(Level*, int, int, int);
	virtual float getBrightness(LevelSource*, int, int, int);
	virtual bool shouldRenderFace(LevelSource*, int, int, int, int);
	virtual int getTexture(int);
	virtual int getTexture(int, int);
	virtual int getTexture(LevelSource*, int, int, int, int);
	virtual AABB* getAABB(Level*, int, int, int);
	virtual void addAABBs(Level*, int, int, int, const AABB*, std::vector<AABB>&);
	virtual AABB getTileAABB(Level*, int, int, int);
	virtual bool isSolidRender();
	virtual bool mayPick();
	virtual bool mayPick(int, bool);
	virtual bool mayPlace(Level*, int, int, int);
	virtual int getTickDelay();
	virtual void tick(Level*, int, int, int, Random*);
	virtual void animateTick(Level*, int, int, int, Random*);
	virtual void destroy(Level*, int, int, int, int dir);
	virtual void neighborChanged(Level*, int, int, int, int);
	virtual void onPlace(Level*, int, int, int);
	virtual void onRemove(Level*, int, int, int);
	virtual int getResource(int, Random*);
	virtual int getResourceCount(Random*);
	virtual float getDestroyProgress(Player*);
	virtual void spawnResources(Level*, int, int, int, int);
	virtual void spawnResources(Level*, int, int, int, int, float);
	virtual int spawnBurnResources(Level*, float, float, float);
	virtual float getExplosionResistance(Entity*);
	virtual HitResult clip(Level*, int, int, int, Vec3, Vec3);
	virtual void wasExploded(Level*, int, int, int);
	virtual int getRenderLayer();
	virtual int use(Level*, int, int, int, Player*);
	virtual void stepOn(Level*, int, int, int, Entity*);
	virtual void setPlacedOnFace(Level*, int, int, int, int);
	virtual void setPlacedBy(Level*, int, int, int, Mob*);
	virtual void prepareRender(Level*, int, int, int);
	virtual void attack(Level*, int, int, int, Player*);
	virtual void handleEntityInside(Level*, int, int, int, Entity*, Vec3&);
	virtual int getColor(LevelSource*, int, int, int);
	virtual bool isSignalSource();
	virtual int getSignal(LevelSource*, int, int, int);
	virtual int getSignal(LevelSource*, int, int, int, int);
	virtual int getDirectSignal(Level*, int, int, int, int);
	virtual void entityInside(Level*, int, int, int, Entity*);
	virtual void playerDestroy(Level*, Player*, int, int, int, int);
	virtual bool canSurvive(Level*, int, int, int);
	virtual std::string getName();
	virtual std::string getDescriptionId();
	virtual Tile* setDescriptionId(std::string const&);
	virtual void triggerEvent(Level*, int, int, int, int, int);
	virtual Tile* setSoundType(Tile::SoundType const&);
	virtual Tile* setLightBlock(int);
	virtual Tile* setLightEmission(float);
	virtual Tile* setExplodeable(float);
	virtual Tile* setDestroyTime(float);
	virtual Tile* setTicking(bool);
	virtual int getSpawnResourcesAuxValue(int);

public: // functions
	Tile(int ID, Material*);
	Tile(int ID, int texture, Material*);

	Tile* init();

	bool containsX(const Vec3&);
	bool containsY(const Vec3&);
	bool containsZ(const Vec3&);

public: // static functions
	static void initTiles();
	static void teardownTiles();

public: // static variables
	static std::string TILE_DESCRIPTION_PREFIX;
	static SoundType
		SOUND_NORMAL,
		SOUND_WOOD,
		SOUND_GRAVEL,
		SOUND_GRASS,
		SOUND_STONE,
		SOUND_METAL,
		SOUND_GLASS,
		SOUND_CLOTH,
		SOUND_SAND,
		SOUND_SILENT;
	static Tile* tiles        [C_MAX_TILES];
	static int   lightBlock   [C_MAX_TILES];
	static int   lightEmission[C_MAX_TILES];
	static bool  shouldTick   [C_MAX_TILES];
	static bool  solid        [C_MAX_TILES];
	static bool  translucent  [C_MAX_TILES];
	static bool  isEntityTile [C_MAX_TILES];

public:
	int tex = 1;
	int id;
	AABB aabb;
	const SoundType* soundType = nullptr;
	float gravity = 1.0f;
	Material* material;
	float friction = 0.6f;

protected:
	float destroySpeed = 0.0f;
	float explosionResistance = 0.0f;
	AABB aabbReturned;

	friend class StairTile;
	friend class FarmTile;

private:
	std::string descriptionId = "";

public:
	static Tile
		* sand,
		* sandStone,
		* stoneBrick,
		* redBrick,
		* wood,
		* glass,
		* calmWater,
		* calmLava,
		* gravel,
		* rock,
		* unbreakable,
		* dirt,
		* grass,
		* ice,
		* clay,
		* farmland,
		* stoneSlab,
		* stoneSlabHalf,
		* cloth,
		* cloth_00,
		* cloth_10,
		* cloth_20,
		* cloth_30,
		* cloth_40,
		* cloth_50,
		* cloth_60,
		* cloth_70,
		* cloth_01,
		* cloth_11,
		* cloth_21,
		* cloth_31,
		* cloth_41,
		* cloth_51,
		* cloth_61,
		* flower,
		* rose,
		* mushroom1,
		* mushroom2,
		* topSnow,
		* treeTrunk,
		* leaves,
		* emeraldOre, //! actually diamond ore
		* redStoneOre,
		* redStoneOre_lit,
		* goldOre,
		* ironOre,
		* coalOre,
		* lapisOre,
		* reeds,
		* ladder,
		* obsidian,
		* tnt,
		* torch,
		* water,
		* lava,
		* fire,
		* invisible_bedrock,
		* goldBlock,
		* ironBlock,
		* emeraldBlock, //! actually diamond block
		* stairs_wood,
		* stairs_stone,
		* door_wood,
		* door_iron;
};
