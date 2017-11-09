#pragma once
#include "GameObject/GameObject.h"

namespace Phoenix
{
	class GameObjectIdPool
	{
	public:
		GameObjectIdPool() = default;
		GameObjectIdPool(std::size_t InPoolSize);

		GameObjectIdPool(const GameObjectIdPool&) = delete;
		GameObjectIdPool& operator=(const GameObjectIdPool&) = delete;

		GameObjectIdPool(GameObjectIdPool&&) = delete;
		GameObjectIdPool& operator=(GameObjectIdPool&&) = delete;

		FGameObject::ID Create();

		std::size_t GetSize() const;

		void Resize(std::size_t InAmount);

		void Remove(FGameObject::ID InGameObjectId);
	protected:
	private:
		std::size_t DefaultPoolSize;

		std::size_t NextId;

		std::vector<FGameObject::ID> FreeList;

		std::vector<FGameObject::ID::IntType> Entities;
	};

};