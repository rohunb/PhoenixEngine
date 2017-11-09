#ifndef PHOENIX_ECS_TEST_H
#define PHOENIX_ECS_TEST_H

namespace Phoenix
{
	class FECSTest
	{
	public:
		void RunTests() const;

	private:
		void TestRequirementBitArrays() const;
		void TestComponentManager() const;

		void ManagerBasicTests() const;
		void ManagerKillTests() const;
		void ManagerComponentTests() const;
	};
}

#endif
