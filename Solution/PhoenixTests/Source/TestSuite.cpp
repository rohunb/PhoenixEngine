#include "TestSuite.h"
#include "Tests/ECS/ECSTest.h"
#include "Tests/MetaProgramming/MetaProgrammingTest.h"
#include "Tests/Serialization/SerializationTest.h"

void Phoenix::FTestSuite::RunTests() const
{
	FSerializationTest SerializationTest;
	SerializationTest.RunTests();

	FMetaProgrammingTest MPTest;
	MPTest.RunTests();

	FECSTest ECSTest;
	ECSTest.RunTests();
}
