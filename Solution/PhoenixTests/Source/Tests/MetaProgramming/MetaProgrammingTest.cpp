#include "Tests/MetaProgramming/MetaProgrammingTest.h"

#include "ECS/ComponentManager.h"
#include "ECS/ComponentManagerConfig.h"
#include "ECS/RequirementBitArrayStorage.h"

#include "Utility/Debug/Assert.h"
#include "Utility/MetaProgramming/Filter.h"
#include "Utility/MetaProgramming/For.h"
#include "Utility/MetaProgramming/IndexOf.h"
#include "Utility/MetaProgramming/Rename.h"
#include "Utility/MetaProgramming/Repeat.h"
#include "Utility/MetaProgramming/Transform.h"
#include "Utility/MetaProgramming/TypeList.h"

using namespace Phoenix;

namespace MetaProgrammingTestStructs
{
	struct Comp1 {};
	struct Comp2 {};
	struct Comp3 {};

	using ComponentList = TTypeList<Comp1, Comp2, Comp3>;

	struct Tag1 {};
	struct Tag2 {};

	using TagList = TTypeList<Tag1, Tag2>;

	using Requirement0 = TTypeList<>;
	using Requirement1 = TTypeList<Tag1, Comp1, Comp2>;
	using Requirement2 = TTypeList<Tag2, Comp3, Comp1>;
	using Requirement3 = TTypeList<Comp1, Comp2>;

	using RequirementList = TTypeList<Requirement0, Requirement1, Requirement2, Requirement3>;

	template<typename TRequirement>
	struct TSystem0 {};
	
	using System0 = TSystem0<Requirement1>;

	using SystemList = TTypeList<System0>;

	using Config = TComponentManagerConfig<ComponentList, TagList, RequirementList, SystemList>;

	struct DummyType1 {};
	struct DummyType2 {};

	static_assert(ComponentList::Size == 3, "TypeList size failed");
	static_assert(TagList::Size == 2, "TypeList size failed");
	static_assert(RequirementList::Size == 4, "TypeList size failed");

	static_assert(TCount<Comp1, ComponentList>::value == 1, "TCount Failed");
	static_assert(TCount<Comp1, TTypeList<Comp1, Comp1, Comp2>>::value == 2, "TCount Failed");
	static_assert(TCount<Tag1, TagList>::value == 1, "TCount Failed");
	static_assert(TCount<DummyType1, ComponentList>::value == 0, "TCount Failed");
	static_assert(TCount<TTypeList<Tag1, Comp1, Comp2>, RequirementList>::value == 1, "TCount Failed");
	static_assert(TCount<Requirement1, RequirementList>::value == 1, "TCount Failed");
	static_assert(TCount<TTypeList<Tag1, Comp1>, RequirementList>::value == 0, "TCount Failed");

	static_assert(TContains<Comp1, ComponentList>::value, "TContains Failed");
	static_assert(!TContains<DummyType2, ComponentList>::value, "TContains Failed");
	static_assert(TContains<TTypeList<Tag1, Comp1, Comp2>, RequirementList>::value, "TContains Failed");

	static_assert(TIndexOf<Comp1, ComponentList>::value == 0, "IndexOf Failed");
	static_assert(TIndexOf<Comp3, ComponentList>::value == 2, "IndexOf Failed");
	static_assert(TIndexOf<Requirement1, RequirementList>::value == 1, "IndexOf Failed");
	//Compiler Error: static_assert(TIndexOf<Tag1, ComponentList>::value == 2, "IndexOf Failed"); 

	static_assert(Config::IsComponent<Comp1>(), "IsComponent failed");
	static_assert(!Config::IsComponent<Tag1>(), "IsComponent failed");
	static_assert(Config::IsTag<Tag1>(), "IsTag failed");
	static_assert(!Config::IsTag<Comp1>(), "IsTag failed");
	static_assert(Config::IsRequirement<Requirement0>(), "IsRequirement failed");
	static_assert(Config::IsRequirement<Requirement1>(), "IsRequirement failed");
	static_assert(!Config::IsRequirement<Comp1>(), "IsRequirement failed");

	static_assert(Config::GetComponentCount() == 3, "Config Count failed");
	static_assert(Config::GetTagCount() == 2, "Config Count failed");
	static_assert(Config::GetRequirementCount() == 4, "Config Count failed");

	static_assert(Config::GetComponentID<Comp2>() == 1, "Config GetID failed");
	static_assert(Config::GetTagID<Tag1>() == 0, "Config GetID failed");
	static_assert(Config::GetRequirementID<Requirement3>() == 3, "Config GetID failed");

	static_assert(Config::GetComponentBit<Comp2>() == 1, "Config GetBit failed");
	static_assert(Config::GetTagBit<Tag1>() == 3, "Config GetBit failed");

	static_assert(TIsSame< TConcat<>, TTypeList<> >::value, "Concat failed");
	static_assert(TIsSame< TConcat<TTypeList<>>, TTypeList<> >::value, "Concat failed");
	static_assert(TIsSame< TConcat<TTypeList<Comp1>>, TTypeList<Comp1> >::value, "Concat failed");
	static_assert(TIsSame< TConcat<TTypeList<Comp1>>, TTypeList<Comp1> >::value, "Concat failed");
	static_assert(TIsSame< TConcat<TTypeList<Comp1>, TTypeList<>>, TTypeList<Comp1>>::value, "Concat failed");
	static_assert(TIsSame< TConcat<TTypeList<Comp1>, TTypeList<Comp2>>, TTypeList<Comp1, Comp2>>::value, "Concat failed");
	static_assert(TIsSame< TConcat<TTypeList<Comp1, Comp2>, TTypeList<>, TTypeList<Tag1, Tag2>>, TTypeList<Comp1, Comp2, Tag1, Tag2>>::value, "Concat failed");
	
	using PushedBackList = TPushBack<TagList, DummyType1>;
	static_assert(TIsSame<PushedBackList, TTypeList<Tag1, Tag2, DummyType1>>::value, "PushBack failed");
	using PushedFrontList = TPushFront<TagList, DummyType1>;
	static_assert(TIsSame<PushedFrontList, TTypeList<DummyType1, Tag1, Tag2>>::value, "PushFront failed");

	static_assert(TIsSame<TRepeat<Comp1, 0>, TTypeList<>>::value, "Repeat Failed");
	static_assert(TIsSame<TRepeat<Comp1, 1>, TTypeList<Comp1>>::value, "Repeat Failed");
	static_assert(TIsSame<TRepeat<Comp1, 4>, TTypeList<Comp1, Comp1, Comp1, Comp1>>::value, "Repeat Failed");

	static_assert(TIsSame<TRename<TTypeList<>, TTuple>, TTuple<>>::value, "Rename failed");
	static_assert(TIsSame<TRename<TTypeList<Comp1, Comp2>, TTuple>, TTuple<Comp1, Comp2>>::value, "Rename failed");
	static_assert(TIsSame<TRename<TTypeList<Comp1, Comp2, Tag1, Requirement1>, TTuple>, TTuple<Comp1, Comp2, Tag1, Requirement1>>::value, "Rename failed");

	static_assert(TIsSame<TFilter<TTypeList<>, TIsIntegral>, TTypeList<>>::value, "Filter failed");
	static_assert(TIsSame<TFilter<TTypeList<Int32, UInt8, Float32>, TIsIntegral>, TTypeList<Int32, UInt8>>::value, "Filter failed");

	static_assert(TIsSame<TTransform<TTypeList<>, std::add_const_t>, TTypeList<>>::value, "Transform failed");
	static_assert(TIsSame<TTransform<TTypeList<Int32, Float32>, std::add_const_t>, TTypeList<const Int32, const Float32>>::value, "Transform failed");

}

void FMetaProgrammingTest::RunTests() const
{
}
