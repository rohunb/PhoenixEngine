#ifndef PHOENIX_BINARY_DESERIALIZER_H
#define PHOENIX_BINARY_DESERIALIZER_H

#include "Utility/Containers/Array.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Debug/Assert.h"
#include "Utility/FileIO/FileStream.h"
#include "Utility/Serialization/ISerializer.h"

namespace Phoenix
{
	class FBinaryDeserializer : public ISerializer<FBinaryDeserializer>
	{
	public:
		explicit FBinaryDeserializer(FInputFileStream& InputStream);
		~FBinaryDeserializer() = default;

		//Primitive Types
		template<typename T
				, TEnableIf<TIsArithmetic<T>::value, Int32> = 0>
		void Load(T& Data);

		//Emums
		template<typename T
				, TEnableIf<std::is_enum<T>::value, Int32> = 0>
		void Load(T& EnumValue);

		//Vector of Non-bool Primitives
		template<typename T
				, TEnableIf<TIsArithmetic<T>::value
							&& !TIsSame<T, bool>::value, Int32> = 0>
		void Load(TVector<T>& VectorData);

		//Vectors of bools
		void Load(TVector<bool>& VectorData);

		//Vector of Class Types
		template<typename T
				, TDisableIf<TIsArithmetic<T>::value, Int32> = 0>
		void Load(TVector<T>& VectorData);

		//Array
		template<typename T, SizeT Size
				, TEnableIf<TIsArithmetic<T>::value, Int32> = 0>
		void Load(TArray<T, Size>& ArrayData);

		template<typename T, SizeT Size
				, TDisableIf<TIsArithmetic<T>::value, Int32> = 0>
		void Load(TArray<T, Size>& ArrayData);
		
		//String types
		template<typename CharT>
		void Load(std::basic_string<CharT>& StringData);

	private:
		FInputFileStream& InputStream;

		template<typename T>
		void LoadBinary(T* const Data, SizeT Size);
	};

	template<typename T
			, TEnableIf<TIsArithmetic<T>::value, Int32>>
	void FBinaryDeserializer::Load(T& Data)
	{
		LoadBinary(std::addressof(Data), sizeof(Data));
	}

	template<typename T
			, TEnableIf<std::is_enum<T>::value, Int32>>
	void FBinaryDeserializer::Load(T& EnumValue)
	{
		std::underlying_type_t<T> UnderlyingValue;
		Serialize(UnderlyingValue);

		EnumValue = static_cast<T>(UnderlyingValue);
	}

	template<typename T
			, TEnableIf<TIsArithmetic<T>::value
						&& !TIsSame<T, bool>::value, Int32>>
	void FBinaryDeserializer::Load(TVector<T>& VectorData)
	{
		SizeT VectorSize = 0;
		Serialize(VectorSize);

		VectorData.resize(VectorSize);

		LoadBinary(VectorData.data(), VectorSize * sizeof(T));
	}

	template<typename T
			, TDisableIf<TIsArithmetic<T>::value, Int32>>
	void FBinaryDeserializer::Load(TVector<T>& VectorData)
	{
		SizeT VectorSize = 0;
		Serialize(VectorSize);

		VectorData.resize(VectorSize);

		for (auto& Element : VectorData)
		{
			Serialize(Element);
		}
	}

	template<typename T, SizeT Size
			, TEnableIf<TIsArithmetic<T>::value, Int32>>
	void FBinaryDeserializer::Load(TArray<T, Size>& ArrayData)
	{
		LoadBinary(ArrayData.data(), sizeof(ArrayData));
	}

	template<typename T, SizeT Size
			, TDisableIf<TIsArithmetic<T>::value, Int32>>
	void FBinaryDeserializer::Load(TArray<T, Size>& ArrayData)
	{
		for (auto&& Element : ArrayData)
		{
			Serialize(Element);
		}
	}

	template<typename CharT>
	void FBinaryDeserializer::Load(std::basic_string<CharT>& StringData)
	{
		SizeT StringSize;
		Serialize(StringSize);
		StringData.resize(StringSize);

		LoadBinary(const_cast<CharT*>(StringData.data()), StringSize * sizeof(CharT));
	}

	template<typename T>
	void FBinaryDeserializer::LoadBinary(T* const Data, SizeT Size)
	{
		F_Assert(InputStream, "InputStream Failed");

		char* const Buffer = reinterpret_cast<char* const>(Data);
		InputStream.read(Buffer, Size);

		F_Assert(InputStream, "InputStream Failed");
	}
}
#endif
