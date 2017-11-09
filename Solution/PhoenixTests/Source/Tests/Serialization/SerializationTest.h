#ifndef PHOENIX_SERIALIZATION_TEST_H
#define PHOENIX_SERIALIZATION_TEST_H

#include "Math/Math.h"
#include "Utility/Containers/Array.h"
#include "Utility/Containers/Vector.h"
#include "Utility/Debug/Assert.h"
#include "Utility/Debug/Debug.h"
#include "Utility/FileIO/File.h"
#include "Utility/FileIO/FileStream.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/Random.h"
#include "Utility/Misc/String.h"
#include "Utility/Serialization/BinarySerializer.h"
#include "Utility/Serialization/BinaryDeserializer.h"

namespace Phoenix
{
	class FSerializationTest
	{
	public:
		FSerializationTest() = default;

		void RunTests();

	private:
		UInt32 NumReps { 10 };
		FString TestFileName { "TestSerialize.dat" };

		struct FTestData
		{
			bool BoolValue;
			FChar Char;
			Int16 Int16Num;
			Int32 Int32Num;
			UInt32 UInt32Num;
			UInt64 Uint64Num;
			Float32 FloatNum;
			Float64 DoubleNum;
			FString String;
			TVector<bool> BoolVector;
			TVector<Int32> IntVector;
			TVector<FString> StringVector;

			enum class TestEnum {Alpha, Beta, Gamma};
			TestEnum EnumValue;

			bool operator == (const FTestData& RHS) const
			{
				return BoolValue == RHS.BoolValue
					&& Char == RHS.Char
					&& Int16Num == RHS.Int16Num
					&& Int32Num == RHS.Int32Num
					&& UInt32Num == RHS.UInt32Num
					&& Uint64Num == RHS.Uint64Num
					&& FloatNum == RHS.FloatNum
					&& DoubleNum == RHS.DoubleNum
					&& IntVector == RHS.IntVector
					&& BoolVector == RHS.BoolVector
					&& String == RHS.String
					&& StringVector == RHS.StringVector
					&& EnumValue == RHS.EnumValue;
			}

			template<typename SerializerType>
			void Serialize(SerializerType& Serializer)
			{
				Serializer.Serialize(BoolValue, Char, Int16Num, Int32Num, UInt32Num, Uint64Num, FloatNum, DoubleNum, String, BoolVector, IntVector, StringVector, EnumValue);
			}
		};

		void TestPrimitives();
		template<typename T>
		void TestSinglePrimitive();

		void TestEnums();

		void TestCustomClassWithPrimitives();

		void TestVectorOfPrimivites();
		
		template<typename T>
		void TestVectorOfPrimitive(UInt32 Size);
		void TestVectorOfBool(UInt32 Size);

		void TestVectorOfClasses();
		void TestVectorOfClass(FTestData& Data, const UInt32 Size);

		void TestVectorOfVectors();
		void TestVectorOfVector(UInt32 Size1, UInt32 Size2);
		
		void TestArrayOfPrimitives();
		
		template<typename T, SizeT Size>
		void TestArrayOfPrimitive();
		template<SizeT Size>
		void TestArrayOfBool();

		void TestArrayOfClasses();
		template<SizeT Size>
		void TestArrayOfClass(const FTestData& Data);

		void TestStrings();

		template<typename T>
		void TestSerialize(T& Data);

		template<typename T>
		void TestDeserialize(T& Data);
	};

	template<typename T>
	void FSerializationTest::TestSerialize(T& Data)
	{
		FOutputFileStream OutputStream { TestFileName, FFile::EFileOpenMode::Binary };
		FBinarySerializer BinarySerializer { OutputStream };
		BinarySerializer.Serialize(Data);
	}

	template<typename T>
	void FSerializationTest::TestDeserialize(T& Data)
	{
		FInputFileStream InputStream { TestFileName, FFile::EFileOpenMode::Binary };
		FBinaryDeserializer BinaryDeserializer { InputStream };
		BinaryDeserializer.Serialize(Data);
	}

	template<typename T>
	void FSerializationTest::TestSinglePrimitive()
	{
		//Test Zero
		{
			T OutPrimitive { 0 };
			T InPrimitive { TNumericLimits<T>::max() };
			TestSerialize(OutPrimitive);
			TestDeserialize(InPrimitive);
			F_AssertEqual(OutPrimitive, InPrimitive, "Zero Serialization Failed");
		}

		//Test Min
		{
			T OutPrimitive { TNumericLimits<T>::min() };
			T InPrimitive {};
			TestSerialize(OutPrimitive);
			TestDeserialize(InPrimitive);
			F_AssertEqual(OutPrimitive, InPrimitive, "Min Primitive Serialization Failed");
		}

		//Test Max
		{
			T OutPrimitive { TNumericLimits<T>::max() };
			T InPrimitive {};
			TestSerialize(OutPrimitive);
			TestDeserialize(InPrimitive);
			F_AssertEqual(OutPrimitive, InPrimitive, "Max Primitive Serialization Failed");
		}

		//Some Random numbers
		{
			FRandom Random;

			for (UInt32 I = 0; I < NumReps; ++I)
			{
				T RandomPrimitive { Random.Range(TNumericLimits<T>::min(), TNumericLimits<T>::max()) };
				//F_Log(typeid(T).name() << " Rand: " << RandomPrimitive);
				T InPrimitive {};
				TestSerialize(RandomPrimitive);
				TestDeserialize(InPrimitive);
				F_AssertEqual(RandomPrimitive, InPrimitive, "Random Primitive Serialization Failed");
			}
		}
	}

	template<typename T>
	void FSerializationTest::TestVectorOfPrimitive(UInt32 Size)
	{
		{
			TVector<T> OutData(Size, TNumericLimits<T>::min());
			TestSerialize(OutData);
			TVector<T> InData;
			TestDeserialize(InData);
			F_AssertEqual(OutData, InData, "Vector Of Min Primitives failed");
		}
		{
			TVector<T> OutData(Size, TNumericLimits<T>::max());
			TestSerialize(OutData);
			TVector<T> InData;
			TestDeserialize(InData);
			F_AssertEqual(OutData, InData, "Vector Of Max Primitives failed");
		}
		{
			TVector<T> OutData(Size);
			TestSerialize(OutData);
			TVector<T> InData;
			TestDeserialize(InData);
			F_AssertEqual(OutData, InData, "Vector Of Default Primitives failed");
		}
		//Random Elements
		{
			TVector<T> OutData;
			OutData.reserve(Size);
			FRandom Random;
			for (UInt32 I = 0; I < Size; ++I)
			{
				OutData.push_back(Random.Range(TNumericLimits<T>::min(), TNumericLimits<T>::max()));
			}

			TestSerialize(OutData);
			TVector<T> InData;
			TestDeserialize(InData);
			F_AssertEqual(OutData, InData, "Vector Of Random Primitives failed");
		}
	}

	template<typename T, SizeT Size>
	void FSerializationTest::TestArrayOfPrimitive()
	{
		{
			TArray<T, Size> OutArray;
			OutArray.fill(TNumericLimits<T>::min());
			TestSerialize(OutArray);

			TArray<T, Size> InArray;
			TestDeserialize(InArray);
			F_AssertEqual(OutArray, InArray, "Array of Primitives Serialization failed")
		}
		{
			TArray<T, Size> OutArray;
			OutArray.fill(TNumericLimits<T>::max());
			TestSerialize(OutArray);
			TArray<T, Size> InArray;
			TestDeserialize(InArray);
			F_AssertEqual(OutArray, InArray, "Array of Primitives Serialization failed")
		}
		{
			FRandom Random;
			TArray<T, Size> OutArray;
			for (auto& Element : OutArray)
			{
				Element = Random.Range(TNumericLimits<T>::min(), TNumericLimits<T>::max());
			}
			TestSerialize(OutArray);

			TArray<T, Size> InArray;
			TestDeserialize(InArray);
			F_AssertEqual(OutArray, InArray, "Array of Primitives Serialization failed")
		}
	}

	template<SizeT Size>
	void FSerializationTest::TestArrayOfBool()
	{
		{
			TArray<bool, Size> OutArray;
			OutArray.fill(false);
			TestSerialize(OutArray);

			TArray<bool, Size> InArray;
			TestDeserialize(InArray);

			F_AssertEqual(OutArray, InArray, "Array of Bools Serialization failed");
		}
		{
			TArray<bool, Size> OutArray;
			OutArray.fill(true);
			TestSerialize(OutArray);

			TArray<bool, Size> InArray;
			TestDeserialize(InArray);

			F_AssertEqual(OutArray, InArray, "Array of Bools Serialization failed");
		}
		{
			FRandom Random;
			TArray<bool, Size> OutArray;
			for (auto& Element : OutArray)
			{
				Element = Random.Bool();
			}
			TestSerialize(OutArray);

			TArray<bool, Size> InArray;
			TestDeserialize(InArray);

			F_AssertEqual(OutArray, InArray, "Array of Bools Serialization failed");
		}
	}

	template<SizeT Size>
	void FSerializationTest::TestArrayOfClass(const FTestData& Data)
	{
		TArray<FTestData, Size> OutData;
		OutData.fill(Data);
		TestSerialize(OutData);

		TArray<FTestData, Size> InData;
		TestDeserialize(InData);

		F_AssertEqual(OutData, InData, "Array of Class Serialization Failed");
	}

}



#endif
