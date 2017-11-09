#ifndef PHOENIX_CLASS_TYPE_ID_H
#define PHOENIX_CLASS_TYPE_ID_H

namespace Phoenix
{
	typedef std::size_t TypeId;

	template<typename TBase>
	class ClassTypeId {
	public:
		template<typename T>
		static TypeId GetTypeId() {
			static const TypeId Id = NextTypeId++;
			return Id;
		}
	private:
		static TypeId NextTypeId;
	};

	template<typename TBase>
	TypeId ClassTypeId<TBase>::NextTypeId = 0;
}

#endif
