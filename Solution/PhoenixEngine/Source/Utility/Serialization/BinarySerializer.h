#ifndef PHOENIX_BINARY_SERIALIZER_H
#define PHOENIX_BINARY_SERIALIZER_H

#include "Utility/Containers/Array.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Debug/Assert.h"
#include "Utility/FileIO/FileStream.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Serialization/ISerializer.h"

namespace Phoenix
{
	class FBinarySerializer : public ISerializer<FBinarySerializer>
	{
	public:
		explicit FBinarySerializer(FOutputFileStream& OutputStream);
		~FBinarySerializer() = default;

		//Specialized methods to Serialize certain types
		//Primitive Types
		template<typename T
				, TEnableIf<TIsArithmetic<T>::value, Int32> = 0>
		void Save(const T& Data);

		//Emums
		template<typename T
				, TEnableIf<std::is_enum<T>::value, Int32> = 0>
		void Save(const T& EnumValue);

		//Vector of Non-bool Primitives
		template<typename T
				, TEnableIf<TIsArithmetic<T>::value
							&& !TIsSame<T, bool>::value, Int32> = 0>
		void Save(TVector<T>& VectorData);

		//Vectors of bools
		void Save(TVector<bool>& VectorData);

		//Vector of Class Types
		template<typename T
				, TDisableIf<TIsArithmetic<T>::value, Int32> = 0>
		void Save(TVector<T>& VectorData);

		//Array
		template<typename T, SizeT Size
				, TEnableIf<TIsArithmetic<T>::value, Int32> = 0>
		void Save(TArray<T, Size>& ArrayData);

		template<typename T, SizeT Size
				, TDisableIf<TIsArithmetic<T>::value, Int32> = 0>
		void Save(TArray<T, Size>& ArrayData);

		//string types
		template<typename CharT>
		void Save(std::basic_string<CharT>& StringData);

	private:
		//#FIXME Update to use Phoenix File system
		FOutputFileStream& OutputStream;

		template<typename T>
		void SaveBinary(const T* const Data, SizeT Size);
	};

	template<typename T
			, TEnableIf<TIsArithmetic<T>::value, Int32>>
	void FBinarySerializer::Save(const T& Data)
	{
		SaveBinary(std::addressof(Data), sizeof(Data));
	}

	template<typename T
			, TEnableIf<std::is_enum<T>::value, Int32>>
	void FBinarySerializer::Save(const T& EnumValue)
	{
		Serialize(static_cast<const std::underlying_type_t<T>>(EnumValue));
	}

	template<typename T
			, TEnableIf<TIsArithmetic<T>::value
						&& !TIsSame<T, bool>::value, Int32>>
	void FBinarySerializer::Save(TVector<T>& VectorData)
	{
		const SizeT VectorSize = VectorData.size();
		Serialize(VectorSize);

		SaveBinary(VectorData.data(), VectorSize * sizeof(T));
	}

	template<typename T
			, TDisableIf<TIsArithmetic<T>::value, Int32>>
	void FBinarySerializer::Save(TVector<T>& VectorData)
	{
		const SizeT VectorSize = VectorData.size();
		Serialize(VectorSize);

		for (auto&& Element : VectorData)
		{
			Serialize(Element);
		}
	}

	template<typename T, SizeT Size
			, TEnableIf<TIsArithmetic<T>::value, Int32>>
	void FBinarySerializer::Save(TArray<T, Size>& ArrayData)
	{
		SaveBinary(ArrayData.data(), sizeof(ArrayData));
	}

	template<typename T, SizeT Size
			, TDisableIf<TIsArithmetic<T>::value, Int32>>
	void FBinarySerializer::Save(TArray<T, Size>& ArrayData)
	{
		for (auto&& Element : ArrayData)
		{
			Serialize(Element);
		}
	}

	template<typename CharT>
	void FBinarySerializer::Save(std::basic_string<CharT>& StringData)
	{
		const SizeT StringSize = StringData.size();
		Serialize(StringSize);

		SaveBinary(StringData.data(), StringSize * sizeof(CharT));
	}

	template<typename T>
	void FBinarySerializer::SaveBinary(const T* const Data, SizeT Size)
	{
		F_Assert(OutputStream, "OutputStream Failed");

		const char* const Buffer = reinterpret_cast<const char* const>(Data);
		OutputStream.write(Buffer, Size);

		F_Assert(OutputStream, "OutputStream Failed");
	}
}
#endif
