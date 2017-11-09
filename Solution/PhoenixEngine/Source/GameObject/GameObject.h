#pragma once
#include "Components/Component.h"
#include "Utility/Misc/ClassTypeId.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <cstdint>

#ifdef _WIN32
#	define GAMEOBJECT_ID_INDEX_BIT_COUNT 20
#	define GAMEOBJECT_ID_COUNTER_BIT_COUNT 12
#else
#	define GAMEOBJECT_ID_INDEX_BIT_COUNT 48
#	define GAMEOBJECT_ID_COUNTER_BIT_COUNT 16
#endif

namespace Phoenix
{
	class FGameScene;

	class FGameObject
	{
	public:
		struct ID
		{
			typedef std::uint32_t IntType;

			IntType Index{ GAMEOBJECT_ID_INDEX_BIT_COUNT };
			IntType Counter{ GAMEOBJECT_ID_COUNTER_BIT_COUNT };

			ID() : Index(0), Counter(0) {};
			ID(IntType inIndex, IntType inCounter) : Index(inIndex), Counter(inCounter) {};

			inline operator IntType() const
			{
				return Value();
			}

			inline IntType Value() const
			{
				return (Counter << GAMEOBJECT_ID_COUNTER_BIT_COUNT) | Index;
			}

			void Clear()
			{
				Index = Counter = 0;
			}

			bool IsNull() const
			{
				return Value() == 0;
			}
		};

		FGameObject();
		FGameObject(FGameScene& inGameScene, FGameObject::ID inId);
		~FGameObject();
		FGameObject(const FGameObject&) = default;
		FGameObject& operator=(const FGameObject&) = default;
		bool operator==(const FGameObject& GameObject) const;
		bool operator!=(const FGameObject& GameObject) const { return !operator==(GameObject); }

		template <typename T>
		T& AddComponent(T* inComponent);

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template <typename T>
		T& GetComponent() const;

		template <typename T>
		void RemoveComponent();

		const ID& GetId() const;

		void SetActive(const bool InActive);

	protected:
	private:
		FGameScene* GameScene = nullptr;
		ID Id;

		void AddComponent(BaseComponent* inComponent, TypeId inComponentTypeId);
		BaseComponent& GetComponent(TypeId InTypeId) const;
		void RemoveComponent(TypeId InComponentTypeId);
	};

	template <typename T>
	T& FGameObject::AddComponent(T* inComponent)
	{
		static_assert(std::is_base_of<BaseComponent, T>::value, "T is not a component, cannot add T to GameObject");
		AddComponent(inComponent, T::GetTypeId());
		return *inComponent;
	}

	template <typename T>
	T& FGameObject::GetComponent() const
	{
		static_assert(std::is_base_of<BaseComponent, T>::value, "T is not a component, cannot get T from GameObject");
		return static_cast<T&>(GetComponent(T::GetTypeId()));
	}

	template <typename T, typename... Args>
	T& FGameObject::AddComponent(Args&&... args)
	{
		return AddComponent(new T{ std::forward<Args>(args)... });
	}

	template <typename T>
	void FGameObject::RemoveComponent()
	{
		RemoveComponent(T::GetTypeId());
	}
}
