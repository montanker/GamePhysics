/*#include "stdafx.h"
#include "CppUnitTest.h"
#include "../BlobGame/Vector3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestClass
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(VectorMathTest)
		{
			Vector3 testVector = Vector3(3, 2, 1);
			testVector = testVector + Vector3(1, 2, 3);
			Assert::AreEqual(testVector.x, (double)4);
			Assert::AreEqual(testVector.y, (double)4);
			Assert::AreEqual(testVector.z, (double)4);
		}

		TEST_METHOD(AddScaledVectorTest)
		{
			Vector3 testVector = Vector3(0, 0, 0);
			testVector.addScaledVector(Vector3(1, 2, 3), 2);
			Assert::AreEqual(testVector.x, (double)2);
			Assert::AreEqual(testVector.y, (double)4);
			Assert::AreEqual(testVector.z, (double)6);
		}

		TEST_METHOD(VectorDistanceTest)
		{
			Vector3 vector1 = Vector3(0, 0, 0);
			Vector3 vector2 = Vector3(3, 0, 0);
			double testDist = 3;
			Assert::AreEqual(vector1.distance(vector2), testDist);
		}
	};
}*/