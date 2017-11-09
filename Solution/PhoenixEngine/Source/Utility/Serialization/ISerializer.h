#ifndef PHOENIX_ISERIALIZER_H
#define PHOENIX_ISERIALIZER_H

#include "Utility/MetaProgramming/HasMethod.h"

namespace Phoenix
{
	F_DefineTrait_HasMethod(Serialize);
	F_DefineTrait_HasMethod(Save);
	F_DefineTrait_HasMethod(Load);

	template<typename SerializerType>
	class ISerializer
	{
	public:
		explicit ISerializer(SerializerType* const ConcreteSerializer)
			: ConcreteSerializer(ConcreteSerializer)
		{}

		/*
		User defined Types should have a Serialize method in the following form:
		template<typename SerializerType>
		void Serialize(SerializerType& Serializer)
		{
			Serializer.Serialize(Arg1, Arg2, Arg3);
		}
		*/

		//Serialization API Entry Point
		template<typename... Types>
		void Serialize(Types&&... Args);

	private:
		SerializerType* ConcreteSerializer { nullptr };

		//The 2 following methods Strip and process each arg to serialize
		template<typename Type>
		void Process(Type&& Data);

		template<typename Type, typename... OtherTypes>
		void Process(Type&& Arg1, OtherTypes&&... OtherArgs);

		//The following few methods figure out the appropriate method to call to serialize the type

		//The Type has a custom Serialize Method - call it
		template<typename Type
				, TEnableIf<THasMethod_Serialize<Type, SerializerType>::Value, Int32> = 0>
		void ProcessImpl(Type& TypeObject);

		//The Serializer has a specialized Save method for the given type
		template<typename Type
				, TEnableIf<THasMethod_Save<SerializerType, Type>::Value, Int32> = 0>
		void ProcessImpl(Type&& TypeObject);

		//The Serializer has a specialized Load method for the given type
		template<typename Type
				, TEnableIf<THasMethod_Load<SerializerType, Type>::Value, Int32> = 0>
		void ProcessImpl(Type&& TypeObject);

		template<typename Type
				, TDisableIf<THasMethod_Serialize<Type, SerializerType>::Value
							|| THasMethod_Save<SerializerType, Type>::Value
							|| THasMethod_Load<SerializerType, Type>::Value
							, Int32> = 0>
		void ProcessImpl(Type&&);
	};


	template<typename SerializerType>
	template<typename... Types>
	void ISerializer<SerializerType>::Serialize(Types&&... Args)
	{
		Process(std::forward<Types>(Args)...);
	}

	template<typename SerializerType>
	template<typename Type>
	void ISerializer<SerializerType>::Process(Type&& Data)
	{
		ProcessImpl(std::forward<Type>(Data));
	}

	template<typename SerializerType>
	template<typename Type, typename... OtherTypes>
	void ISerializer<SerializerType>::Process(Type&& Arg1, OtherTypes&&... OtherArgs)
	{
		Process(std::forward<Type>(Arg1));
		Process(std::forward<OtherTypes>(OtherArgs)...);
	}

	template<typename SerializerType>
	template<typename Type
			, TEnableIf<THasMethod_Serialize<Type, SerializerType>::Value, Int32>>
	void ISerializer<SerializerType>::ProcessImpl(Type& TypeObject)
	{
		//Calls User Defined Serialize Method in Type
		TypeObject.Serialize(*ConcreteSerializer);
	}

	template<typename SerializerType>
	template<typename Type
			, TEnableIf<THasMethod_Save<SerializerType, Type>::Value, Int32>>
	void ISerializer<SerializerType>::ProcessImpl(Type&& TypeObject)
	{
		ConcreteSerializer->Save(std::forward<Type>(TypeObject));
	}

	template<typename SerializerType>
	template<typename Type
			, TEnableIf<THasMethod_Load<SerializerType, Type>::Value, Int32>>
	void ISerializer<SerializerType>::ProcessImpl(Type&& TypeObject)
	{
		ConcreteSerializer->Load(std::forward<Type>(TypeObject));
	}

	template<typename SerializerType>
	template<typename Type
			, TDisableIf < THasMethod_Serialize<Type, SerializerType>::Value
						|| THasMethod_Save<SerializerType, Type>::Value
						|| THasMethod_Load<SerializerType, Type>::Value
						, Int32 >>
	void ISerializer<SerializerType>::ProcessImpl(Type&&)
	{
		const bool SuccessCondition = THasMethod_Serialize<Type, SerializerType>::Value
									|| THasMethod_Save<SerializerType, Type>::Value
									|| THasMethod_Load<SerializerType, Type>::Value;

#define ERROR_MESSAGE "\nCould not find any appropriate Serialize or Save/Load methods for the provided Type and Serializer.\
					   \nUser defined Types should have a Serialize method in the following form:\n\n\
					   \ttemplate<typename SerializerType>\n\
					   \tvoid Serialize(SerializerType& Serializer)\n\
					   \t{\n\
					   \t\tSerializer.Serialize(Arg1, Arg2, Arg3);\n\
					   \t}\n\n"

		F_Assert(SuccessCondition, ERROR_MESSAGE);
		static_assert(SuccessCondition, ERROR_MESSAGE);

#undef ERROR_MESSAGE
	}


}

#endif
