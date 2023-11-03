#pragma once

class Shape;
class Hitbox;

struct Vector2;
struct GameObjectRewindData;


namespace ObjectType
{
	enum ObjectType
	{
		Unknown =	0b0000,
		Actor =		0b0001,
		Prop =		0b0010,
		Camera =	0b0100,
	};
}

namespace ObjectId
{
	enum ObjectId
	{
		None =			0b0000'0000,
		Player =		0b0000'0001,
		Enemy =			0b0000'0010,
		PlayerAttack =	0b0000'0100,
		EnemyAttack =	0b0000'1000,
		Collectible =	0b0001'0000,
		Rope =			0b0010'0000,
		Trigger =		0b0100'0000,
	};
}


enum class ObjectClass
{
	Camera,
	Player,
	Knight,
	Archer,
	Coin,
	Soul,
	RockPile,
	Rock,
	Arrow,
	Rope,
	SavePoint,
	Exit,
	Particles,
	Unknown,
};


class GameObject
{
public:
	GameObject(const ObjectClass objectClass, const int objectType, int tileLayer = 0, int tileMask = 0, int objectLayer = 0, int objectMask = 0);

	virtual ~GameObject();

	// Manage rewind data.
	void StoreRewindData(GameObjectRewindData* rewindData);
	void Rewind(GameObjectRewindData* rewindData);
	void Resume(GameObjectRewindData* rewindData);


	virtual void Update(float deltaTime, float gravity = 0.0f);

	virtual void Draw(Surface* screen, int x, int y);

	virtual void ApplyFriction(float deltaTime);

	// These must exist so the collision managers can call these functions without worry about derived type.
	virtual void PrepareSprite();
	virtual void OnObjectCollision(GameObject* object);
	virtual void ResolveObstacleCollisions(const float2& contactNormals);
	virtual void ResolveObjectCollisions();

	// Collision boxes.
	Shape* shape_{ nullptr };
	Hitbox* hitboxes_{ nullptr };
	int hitboxCount_{ 0 };

	// Visuals.
	Sprite* sprite_{ nullptr };

	const ObjectClass objectClass_{ ObjectClass::Unknown };
	const int objectType_{ ObjectType::Unknown };

	// [Credit] First saw this pattern in Godot.
	int tileLayer_{ 0xb0 };	// I am on this layer.
	int tileMask_{ 0xb0 };	// I care about things on this layer.

	// [Credit] First saw this pattern in Godot.
	int objectLayer_{ 0xb0 };	// I am on this layer.
	int objectMask_{ 0xb0 };	// I care about things on this layer.

	// What I am touching after movement.
	int overlappingTileLayers_{ 0xb0 };

	// Movement.
	Vector2* velocity_;
	Vector2* frameVelocity_;
	float friction_{ 0.0f };

	bool isOnstage_{ false };
	bool isFlipped_{ false };	
	bool isActive_{ false };
};