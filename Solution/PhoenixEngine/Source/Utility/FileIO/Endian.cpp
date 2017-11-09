#include "Stdafx.h"
#include "Utility/FileIO/Endian.h"

using namespace Phoenix;

FEndian::TSwapFunc<Int16> FEndian::SwapLittleInt16;
FEndian::TSwapFunc<Int32> FEndian::SwapLittleInt32;
FEndian::TSwapFunc<UInt16> FEndian::SwapLittleUInt16;
FEndian::TSwapFunc<UInt32> FEndian::SwapLittleUInt32;
FEndian::TSwapFunc<Float32> FEndian::SwapLittleFloat32;

FEndian::TSwapFunc<Int16> FEndian::SwapBigInt16;
FEndian::TSwapFunc<Int32> FEndian::SwapBigInt32;
FEndian::TSwapFunc<UInt16> FEndian::SwapBigUInt16;
FEndian::TSwapFunc<UInt32> FEndian::SwapBigUInt32;
FEndian::TSwapFunc<Float32> FEndian::SwapBigFloat32;

void FEndian::Init()
{
	union
	{
		UInt32 Byte4;
		UInt8 Byte1;
	} EndianTest;

	EndianTest.Byte4 = 0x01;
	const bool bIsLittleEndian = EndianTest.Byte1 == 0x01;

	if (bIsLittleEndian)
	{
		F_Log("Little endian system detected.");
		FEndian::SwapLittleInt16 = &FEndian::InternalNoSwap<Int16>;
		FEndian::SwapLittleInt32 = &FEndian::InternalNoSwap<Int32>;
		FEndian::SwapLittleUInt16 = &FEndian::InternalNoSwap<UInt16>;
		FEndian::SwapLittleUInt32 = &FEndian::InternalNoSwap<UInt32>;
		FEndian::SwapLittleFloat32 = &FEndian::InternalNoSwap<Float32>;

		FEndian::SwapBigInt16 = &FEndian::InternalSwap<Int16>;
		FEndian::SwapBigInt32 = &FEndian::InternalSwap<Int32>;
		FEndian::SwapBigUInt16 = &FEndian::InternalSwap<UInt16>;
		FEndian::SwapBigUInt32 = &FEndian::InternalSwap<UInt32>;
		FEndian::SwapBigFloat32 = &FEndian::InternalSwap<Float32>;
	}
	else
	{
		F_Log("Big endian system detected.");
		FEndian::SwapLittleInt16 = &FEndian::InternalSwap<Int16>;
		FEndian::SwapLittleInt32 = &FEndian::InternalSwap<Int32>;
		FEndian::SwapLittleUInt16 = &FEndian::InternalSwap<UInt16>;
		FEndian::SwapLittleUInt32 = &FEndian::InternalSwap<UInt32>;
		FEndian::SwapLittleFloat32 = &FEndian::InternalSwap<Float32>;

		FEndian::SwapBigInt16 = &FEndian::InternalNoSwap<Int16>;
		FEndian::SwapBigInt32 = &FEndian::InternalNoSwap<Int32>;
		FEndian::SwapBigUInt16 = &FEndian::InternalNoSwap<UInt16>;
		FEndian::SwapBigUInt32 = &FEndian::InternalNoSwap<UInt32>;
		FEndian::SwapBigFloat32 = &FEndian::InternalNoSwap<Float32>;
	}
}
