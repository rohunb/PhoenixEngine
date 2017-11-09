#include "Stdafx.h"
#include "EngineComponents/CInput.h"

using namespace Phoenix;

void CInput::ClearAllCallbacks()
{
	KeyCallback = nullptr;
	MovementAxisCallback = nullptr;
}
