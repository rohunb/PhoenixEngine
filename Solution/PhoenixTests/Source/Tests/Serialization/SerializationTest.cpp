#include "Tests/Serialization/SerializationTest.h"

#include <chrono>

using namespace Phoenix;

void FSerializationTest::RunTests()
{
	F_Log("Testing Serialization...");

	TestPrimitives();
	TestEnums();

	TestCustomClassWithPrimitives();

	TestVectorOfPrimivites();
	TestVectorOfClasses();
	TestVectorOfVectors();

	TestArrayOfPrimitives();
	TestArrayOfClasses();

	TestStrings();
}

void FSerializationTest::TestPrimitives()
{
	F_Log("\tPrimitives...");
	TestSinglePrimitive<FByte>();
	TestSinglePrimitive<FChar>();
	TestSinglePrimitive<SizeT>();

	TestSinglePrimitive<Int8>();
	TestSinglePrimitive<Int16>();
	TestSinglePrimitive<Int32>();
	TestSinglePrimitive<Int64>();

	TestSinglePrimitive<UInt8>();
	TestSinglePrimitive<UInt16>();
	TestSinglePrimitive<UInt32>();
	TestSinglePrimitive<UInt64>();

	TestSinglePrimitive<Float32>();
	TestSinglePrimitive<Float64>();

	{
		bool OutValue { true };
		TestSerialize(OutValue);
		bool InValue { false };
		TestDeserialize(InValue);
		F_Assert(OutValue == InValue, "Bool Serialization Failed");
	}
	{
		bool OutValue { false };
		TestSerialize(OutValue);
		bool InValue { true };
		TestDeserialize(InValue);
		F_Assert(OutValue == InValue, "Bool Serialization Failed");
	}
}

void FSerializationTest::TestEnums()
{
	{
		FTestData OutData {};
		OutData.EnumValue = FTestData::TestEnum::Alpha;
		TestSerialize(OutData);

		FTestData InData {};
		TestDeserialize(InData);

		F_AssertEqual(OutData, InData, "Enum Serialization Failed");
	}
	{
		FTestData OutData {};
		OutData.EnumValue = FTestData::TestEnum::Beta;
		TestSerialize(OutData);

		FTestData InData {};
		TestDeserialize(InData);

		F_AssertEqual(OutData, InData, "Enum Serialization Failed");
	}
	{
		FTestData OutData {};
		OutData.EnumValue = FTestData::TestEnum::Gamma;
		TestSerialize(OutData);

		FTestData InData {};
		TestDeserialize(InData);

		F_AssertEqual(OutData, InData, "Enum Serialization Failed");
	}
}

void FSerializationTest::TestCustomClassWithPrimitives()
{
	F_Log("\tStruct of Primitives...");
	{
		FTestData OutData = { false
							, TNumericLimits<FChar>::min()
							, TNumericLimits<Int16>::min()
							, TNumericLimits<Int32>::min()
							, TNumericLimits<UInt32>::min()
							, TNumericLimits<UInt64>::min()
							, TNumericLimits<Float32>::min()
							, TNumericLimits<Float64>::min()
							, {"        "}
							, {false, false, false}
							, { TNumericLimits<Int32>::min(), TNumericLimits<Int32>::min(), TNumericLimits<Int32>::min() }
							, { "        ", "        ", "        ", "        " } };

		TestSerialize(OutData);
		FTestData InData {};
		TestDeserialize(InData);
		F_Assert(OutData == InData, "Custom Class Serialization Failed");
	}
	{
		FTestData OutData = { true
							, TNumericLimits<FChar>::max()
							, TNumericLimits<Int16>::max()
							, TNumericLimits<Int32>::max()
							, TNumericLimits<UInt32>::max()
							, TNumericLimits<UInt64>::max()
							, TNumericLimits<Float32>::max()
							, TNumericLimits<Float64>::max()
							, FString( 4, TNumericLimits<FChar>::max() )
							, { true, true, true }
							, { TNumericLimits<Int32>::max(), TNumericLimits<Int32>::max(), TNumericLimits<Int32>::max() } 
							, TVector<FString>(4, FString(4, TNumericLimits<FChar>::max()))};

		TestSerialize(OutData);
		FTestData InData {};
		TestDeserialize(InData);
		F_Assert(OutData == InData, "Custom Class Serialization Failed");
	}
	{
		FTestData OutData {};
		TestSerialize(OutData);
		FTestData InData;
		TestDeserialize(InData);
		F_Assert(OutData == InData, "Custom Class Serialization Failed");
	}
	{
		const UInt32 VectorMaxSize { 15 };
		FRandom Random;

		for (UInt32 I = 0; I < NumReps; ++I)
		{
			FTestData OutData = { Random.Bool()
								, Random.Range(TNumericLimits<FChar>::min(), TNumericLimits<FChar>::max())
								, Random.Range(TNumericLimits<Int16>::min(), TNumericLimits<Int16>::max())
								, Random.Range(TNumericLimits<Int32>::min(), TNumericLimits<Int32>::max())
								, Random.Range(TNumericLimits<UInt32>::min(), TNumericLimits<UInt32>::max())
								, Random.Range(TNumericLimits<UInt64>::min(), TNumericLimits<UInt64>::max())
								, Random.Range(TNumericLimits<Float32>::min(), TNumericLimits<Float32>::max())
								, Random.Range(TNumericLimits<Float64>::min(), TNumericLimits<Float64>::max())
								, Random.String(Random.Bounded(TNumericLimits<UInt8>::max()))
								, TVector<bool>(Random.Bounded(VectorMaxSize), Random.Bool())
								, TVector<Int32>(Random.Bounded(VectorMaxSize), Random.Range(TNumericLimits<Int32>::min(), TNumericLimits<Int32>::max()))
								, TVector<FString>(Random.Bounded(VectorMaxSize), Random.String(Random.Bounded(100)))};

			//F_Log("RandomData: " << OutData.Char << " " << OutData.Int16Num << " " << OutData.Int32Num << " " << OutData.UInt32Num << " " << OutData.Uint64Num << " " << OutData.FloatNum << " " << OutData.DoubleNum );
			TestSerialize(OutData);
			FTestData InData;
			TestDeserialize(InData);
			F_Assert(OutData == InData, "Custom Class Serialization Failed");
		}
	}
}

void FSerializationTest::TestVectorOfPrimivites()
{
	F_Log("\tVectors of Primitives...");

	const UInt32 LargeVectorSize = NumReps;
	F_Assert(LargeVectorSize <= TVector<UInt64>().max_size(), "Update size of large vector");

	const auto StartTime = std::chrono::high_resolution_clock::now();

	TestVectorOfBool(0);
	TestVectorOfBool(1);
	TestVectorOfBool(LargeVectorSize);

	TestVectorOfPrimitive<FChar>(0);
	TestVectorOfPrimitive<FChar>(1);
	TestVectorOfPrimitive<FChar>(LargeVectorSize);

	TestVectorOfPrimitive<Int8>(0);
	TestVectorOfPrimitive<Int8>(1);
	TestVectorOfPrimitive<Int8>(LargeVectorSize);

	TestVectorOfPrimitive<Int16>(0);
	TestVectorOfPrimitive<Int16>(1);
	TestVectorOfPrimitive<Int16>(LargeVectorSize);

	TestVectorOfPrimitive<Int32>(0);
	TestVectorOfPrimitive<Int32>(1);
	TestVectorOfPrimitive<Int32>(LargeVectorSize);

	TestVectorOfPrimitive<Int64>(0);
	TestVectorOfPrimitive<Int64>(1);
	TestVectorOfPrimitive<Int64>(LargeVectorSize);

	TestVectorOfPrimitive<FByte>(0);
	TestVectorOfPrimitive<FByte>(1);
	TestVectorOfPrimitive<FByte>(LargeVectorSize);

	TestVectorOfPrimitive<UInt8>(0);
	TestVectorOfPrimitive<UInt8>(1);
	TestVectorOfPrimitive<UInt8>(LargeVectorSize);

	TestVectorOfPrimitive<UInt16>(0);
	TestVectorOfPrimitive<UInt16>(1);
	TestVectorOfPrimitive<UInt16>(LargeVectorSize);

	TestVectorOfPrimitive<UInt32>(0);
	TestVectorOfPrimitive<UInt32>(1);
	TestVectorOfPrimitive<UInt32>(LargeVectorSize);

	TestVectorOfPrimitive<UInt64>(0);
	TestVectorOfPrimitive<UInt64>(1);
	TestVectorOfPrimitive<UInt64>(LargeVectorSize);

	TestVectorOfPrimitive<Float32>(0);
	TestVectorOfPrimitive<Float32>(1);
	TestVectorOfPrimitive<Float32>(LargeVectorSize);

	TestVectorOfPrimitive<Float64>(0);
	TestVectorOfPrimitive<Float64>(1);
	TestVectorOfPrimitive<Float64>(LargeVectorSize);

	TestVectorOfPrimitive<SizeT>(0);
	TestVectorOfPrimitive<SizeT>(1);
	TestVectorOfPrimitive<SizeT>(LargeVectorSize);

	const auto EndTime = std::chrono::high_resolution_clock::now();
	F_Log("Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime).count());
}

void FSerializationTest::TestVectorOfBool(UInt32 Size)
{
	{
		TVector<bool> OutData(Size, false);
		TestSerialize(OutData);
		TVector<bool> InData;
		TestDeserialize(InData);
		F_AssertEqual(OutData, InData, "Vector Of bools failed");
	}
	{
		TVector<bool> OutData(Size, true);
		TestSerialize(OutData);
		TVector<bool> InData;
		TestDeserialize(InData);
		F_AssertEqual(OutData, InData, "Vector Of bools failed");
	}
	{
		TVector<bool> OutData;
		OutData.reserve(Size);
		FRandom Random;
		for (UInt32 I = 0; I < Size; ++I)
		{
			OutData.push_back(Random.Bool());
		}

		TestSerialize(OutData);
		TVector<bool> InData;
		TestDeserialize(InData);
		F_AssertEqual(OutData, InData, "Vector Of Random bools failed");
	}
}

void FSerializationTest::TestVectorOfClasses()
{
	F_Log("\tVectors of Classes...");

	const UInt32 LargeVectorSize = NumReps;
	F_Assert(LargeVectorSize <= TVector<UInt64>().max_size(), "Update size of large vector");

	{
		FTestData TestData = { false
							, TNumericLimits<FChar>::min()
							, TNumericLimits<Int16>::min()
							, TNumericLimits<Int32>::min()
							, TNumericLimits<UInt32>::min()
							, TNumericLimits<UInt64>::min()
							, TNumericLimits<Float32>::min()
							, TNumericLimits<Float64>::min()
							, { "        " }
							, { false, false, false }
							, { TNumericLimits<Int32>::min(), TNumericLimits<Int32>::min(), TNumericLimits<Int32>::min() }
							, { "        ", "        ", "        ", "        " } };

		TestVectorOfClass(TestData, 0);
		TestVectorOfClass(TestData, 1);
		TestVectorOfClass(TestData, LargeVectorSize);
	}
	{
		FTestData TestData = { true
							, TNumericLimits<FChar>::max()
							, TNumericLimits<Int16>::max()
							, TNumericLimits<Int32>::max()
							, TNumericLimits<UInt32>::max()
							, TNumericLimits<UInt64>::max()
							, TNumericLimits<Float32>::max()
							, TNumericLimits<Float64>::max()
							, FString(4, TNumericLimits<FChar>::max())
							, { true, true, true }
							, { TNumericLimits<Int32>::max(), TNumericLimits<Int32>::max(), TNumericLimits<Int32>::max() }
							, TVector<FString>(4, FString(4, TNumericLimits<FChar>::max())) };

		TestVectorOfClass(TestData, 0);
		TestVectorOfClass(TestData, 1);
		TestVectorOfClass(TestData, LargeVectorSize);
	}
	{
		FTestData TestData = {};

		TestVectorOfClass(TestData, 0);
		TestVectorOfClass(TestData, 1);
		TestVectorOfClass(TestData, LargeVectorSize);
	}
	{
		//Vector of Random Elements
		const UInt32 VectorMaxSize { LargeVectorSize };
		FRandom Random;

		for (UInt32 I = 0; I < NumReps; ++I)
		{
			const UInt32 Size = Random.Bounded(LargeVectorSize);
			TVector<FTestData> OutData;
			OutData.reserve(Size);

			for (UInt32 I = 0; I < Size; ++I)
			{
				FTestData TestData = { Random.Bool()
									, Random.Range(TNumericLimits<FChar>::min(), TNumericLimits<FChar>::max())
									, Random.Range(TNumericLimits<Int16>::min(), TNumericLimits<Int16>::max())
									, Random.Range(TNumericLimits<Int32>::min(), TNumericLimits<Int32>::max())
									, Random.Range(TNumericLimits<UInt32>::min(), TNumericLimits<UInt32>::max())
									, Random.Range(TNumericLimits<UInt64>::min(), TNumericLimits<UInt64>::max())
									, Random.Range(TNumericLimits<Float32>::min(), TNumericLimits<Float32>::max())
									, Random.Range(TNumericLimits<Float64>::min(), TNumericLimits<Float64>::max())
									, Random.String(Random.Bounded(TNumericLimits<UInt8>::max()))
									, TVector<bool>(Random.Bounded(VectorMaxSize), Random.Bool())
									, TVector<Int32>(Random.Bounded(VectorMaxSize), Random.Range(TNumericLimits<Int32>::min(), TNumericLimits<Int32>::max()))
									, TVector<FString>(Random.Bounded(25), Random.String(Random.Bounded(100))) };

				OutData.emplace_back(std::move(TestData));
			}

			TestSerialize(OutData);
			TVector<FTestData> InData;
			TestDeserialize(InData);
			F_AssertEqual(OutData.size(), InData.size(), "Vector Sizes are not Equal");

			for (UInt32 I = 0; I < Size; ++I)
			{
				F_AssertEqual(OutData[I], InData[I], "Serialization of Vector Of Classes failed");
			}
		}
	}
}

void FSerializationTest::TestVectorOfClass(FTestData& Data, const UInt32 Size)
{
	TVector<FTestData> OutData(Size, Data);
	TestSerialize(OutData);
	TVector<FTestData> InData;
	TestDeserialize(InData);
	F_AssertEqual(OutData.size(), InData.size(), "Vector Sizes are not Equal");

	for (UInt32 I = 0; I < Size; ++I)
	{
		F_AssertEqual(OutData[I], InData[I], "Serialization of Vector Of Classes failed");
	}
}

void FSerializationTest::TestVectorOfVectors()
{
	F_Log("\tVectors of Vectors...");

	const UInt32 LargeVectorSize = NumReps;
	F_Assert(LargeVectorSize <= TVector<UInt64>().max_size(), "Update size of large vector");

	const auto StartTime = std::chrono::high_resolution_clock::now();

	TestVectorOfVector(0, 0);
	TestVectorOfVector(0, 1);
	TestVectorOfVector(0, LargeVectorSize);

	TestVectorOfVector(1, 0);
	TestVectorOfVector(1, 1);
	TestVectorOfVector(1, LargeVectorSize);

	TestVectorOfVector(LargeVectorSize, 0);
	TestVectorOfVector(LargeVectorSize, 1);
	TestVectorOfVector(LargeVectorSize, LargeVectorSize);

	const auto EndTime = std::chrono::high_resolution_clock::now();
	F_Log("Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime).count());
}

void Phoenix::FSerializationTest::TestVectorOfVector(UInt32 Size1, UInt32 Size2)
{
	TVector<TVector<FTestData>> OutVector { Size1 };
	F_AssertEqual(OutVector.size(), Size1, "Sizes not equal");

	const UInt32 VectorMaxSize { 15 };
	FRandom Random;

	for (auto&& InnerVector : OutVector)
	{
		InnerVector.reserve(Size2);
		for (UInt32 I = 0; I < Size2; ++I)
		{
			FTestData TestData = { Random.Bool()
								, Random.Range(TNumericLimits<FChar>::min(), TNumericLimits<FChar>::max())
								, Random.Range(TNumericLimits<Int16>::min(), TNumericLimits<Int16>::max())
								, Random.Range(TNumericLimits<Int32>::min(), TNumericLimits<Int32>::max())
								, Random.Range(TNumericLimits<UInt32>::min(), TNumericLimits<UInt32>::max())
								, Random.Range(TNumericLimits<UInt64>::min(), TNumericLimits<UInt64>::max())
								, Random.Range(TNumericLimits<Float32>::min(), TNumericLimits<Float32>::max())
								, Random.Range(TNumericLimits<Float64>::min(), TNumericLimits<Float64>::max())
								, Random.String(Random.Bounded(TNumericLimits<UInt8>::max()))
								, TVector<bool>(Random.Bounded(VectorMaxSize), Random.Bool())
								, TVector<Int32>(Random.Bounded(VectorMaxSize), Random.Range(TNumericLimits<Int32>::min(), TNumericLimits<Int32>::max()))
								, TVector<FString>(Random.Bounded(25), Random.String(Random.Bounded(100))) };

			InnerVector.emplace_back(std::move(TestData));
		}
	}

	TestSerialize(OutVector);
	TVector<TVector<FTestData>> InVector;
	TestDeserialize(InVector);
	F_AssertEqual(OutVector, InVector, "Vector of Vector Serialization failed");
}

void FSerializationTest::TestArrayOfPrimitives()
{
	F_Log("\tArray of Primitives...");

	const UInt32 LargeArraySize = 25;

	TestArrayOfBool<0>();
	TestArrayOfBool<1>();
	TestArrayOfBool<LargeArraySize>();
	
	TestArrayOfPrimitive<FChar, 0>();
	TestArrayOfPrimitive<FChar, 1>();
	TestArrayOfPrimitive<FChar, LargeArraySize>();
	
	TestArrayOfPrimitive<Int8, 0>();
	TestArrayOfPrimitive<Int8, 1>();
	TestArrayOfPrimitive<Int8, LargeArraySize>();
	
	TestArrayOfPrimitive<Int16, 0>();
	TestArrayOfPrimitive<Int16, 1>();
	TestArrayOfPrimitive<Int16, LargeArraySize>();
	
	TestArrayOfPrimitive<Int32, 0>();
	TestArrayOfPrimitive<Int32, 1>();
	TestArrayOfPrimitive<Int32, LargeArraySize>();
	
	TestArrayOfPrimitive<Int64, 0>();
	TestArrayOfPrimitive<Int64, 1>();
	TestArrayOfPrimitive<Int64, LargeArraySize>();
	
	TestArrayOfPrimitive<FByte, 0>();
	TestArrayOfPrimitive<FByte, 1>();
	TestArrayOfPrimitive<FByte, LargeArraySize>();
	 
	TestArrayOfPrimitive<UInt8, 0>();
	TestArrayOfPrimitive<UInt8, 1>();
	TestArrayOfPrimitive<UInt8, LargeArraySize>();
	
	TestArrayOfPrimitive<UInt16, 0>();
	TestArrayOfPrimitive<UInt16, 1>();
	TestArrayOfPrimitive<UInt16, LargeArraySize>();
	
	TestArrayOfPrimitive<UInt32, 0>();
	TestArrayOfPrimitive<UInt32, 1>();
	TestArrayOfPrimitive<UInt32, LargeArraySize>();
	
	TestArrayOfPrimitive<UInt64, 0>();
	TestArrayOfPrimitive<UInt64, 1>();
	TestArrayOfPrimitive<UInt64, LargeArraySize>();
	
	TestArrayOfPrimitive<Float32, 0>();
	TestArrayOfPrimitive<Float32, 1>();
	TestArrayOfPrimitive<Float32, LargeArraySize>();
	
	TestArrayOfPrimitive<Float64, 0>();
	TestArrayOfPrimitive<Float64, 1>();
	TestArrayOfPrimitive<Float64, LargeArraySize>();
	
	TestArrayOfPrimitive<SizeT, 0>();
	TestArrayOfPrimitive<SizeT, 1>();
	TestArrayOfPrimitive<SizeT, LargeArraySize>();
}

void FSerializationTest::TestArrayOfClasses()
{
	F_Log("\tVectors of Classes...");

	const SizeT LargeArraySize = 25;

	{
		const FTestData TestData = { false
									, TNumericLimits<FChar>::min()
									, TNumericLimits<Int16>::min()
									, TNumericLimits<Int32>::min()
									, TNumericLimits<UInt32>::min()
									, TNumericLimits<UInt64>::min()
									, TNumericLimits<Float32>::min()
									, TNumericLimits<Float64>::min()
									, { "        " }
									, { false, false, false }
									, { TNumericLimits<Int32>::min(), TNumericLimits<Int32>::min(), TNumericLimits<Int32>::min() }
									, { "        ", "        ", "        ", "        " } };

		TestArrayOfClass<0>(TestData);
		TestArrayOfClass<1>(TestData);
		TestArrayOfClass<LargeArraySize>(TestData);
	}
	{
		const FTestData TestData = { true
									, TNumericLimits<FChar>::max()
									, TNumericLimits<Int16>::max()
									, TNumericLimits<Int32>::max()
									, TNumericLimits<UInt32>::max()
									, TNumericLimits<UInt64>::max()
									, TNumericLimits<Float32>::max()
									, TNumericLimits<Float64>::max()
									, FString(4, TNumericLimits<FChar>::max())
									, { true, true, true }
									, { TNumericLimits<Int32>::max(), TNumericLimits<Int32>::max(), TNumericLimits<Int32>::max() }
									, TVector<FString>(4, FString(4, TNumericLimits<FChar>::max())) };

		TestArrayOfClass<0>(TestData);
		TestArrayOfClass<1>(TestData);
		TestArrayOfClass<LargeArraySize>(TestData);
	}
	{
		const FTestData TestData = {};
		TestArrayOfClass<0>(TestData);
		TestArrayOfClass<1>(TestData);
		TestArrayOfClass<LargeArraySize>(TestData);
	}
	{
		FRandom Random;
		const FTestData TestData = { Random.Bool()
									, Random.Range(TNumericLimits<FChar>::min(), TNumericLimits<FChar>::max())
									, Random.Range(TNumericLimits<Int16>::min(), TNumericLimits<Int16>::max())
									, Random.Range(TNumericLimits<Int32>::min(), TNumericLimits<Int32>::max())
									, Random.Range(TNumericLimits<UInt32>::min(), TNumericLimits<UInt32>::max())
									, Random.Range(TNumericLimits<UInt64>::min(), TNumericLimits<UInt64>::max())
									, Random.Range(TNumericLimits<Float32>::min(), TNumericLimits<Float32>::max())
									, Random.Range(TNumericLimits<Float64>::min(), TNumericLimits<Float64>::max())
									, Random.String(Random.Bounded(TNumericLimits<UInt8>::max()))
									, TVector<bool>(Random.Bounded(LargeArraySize), Random.Bool())
									, TVector<Int32>(Random.Bounded(LargeArraySize), Random.Range(TNumericLimits<Int32>::min(), TNumericLimits<Int32>::max()))
									, TVector<FString>(Random.Bounded(25), Random.String(Random.Bounded(100))) };

		TestArrayOfClass<0>(TestData);
		TestArrayOfClass<1>(TestData);
		TestArrayOfClass<LargeArraySize>(TestData);
	}
}

void FSerializationTest::TestStrings()
{
	F_Log("\tStrings...");

	{
		FString OutString;
		TestSerialize(OutString);
		FString InString;
		TestDeserialize(InString);
		F_AssertEqual(OutString, InString, "String Serialization failed");
	}
	//1
	{
		FString OutString(" ");
		TestSerialize(OutString);
		FString InString;
		TestDeserialize(InString);
		F_AssertEqual(OutString, InString, "String Serialization failed");
	}
	{
		FString OutString;
		const UInt32 Size = TNumericLimits<UInt16>::max();

		OutString.reserve(Size);

		FRandom Random;

		for (UInt32 I = 0; I < Size; ++I)
		{
			OutString.push_back(Random.Range(TNumericLimits<FChar>::min(), TNumericLimits<FChar>::max()));
		}

		TestSerialize(OutString);
		FString InString;
		TestDeserialize(InString);
		F_AssertEqual(OutString, InString, "String Serialization failed");
	}

}