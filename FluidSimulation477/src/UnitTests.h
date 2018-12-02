#pragma once
#include "Quaternion.h"
#include <assert.h>
class UnitTests
{
private:

	static void assertEqual(double a, double b)
	{
		std::cout << "\t\tTesting: is " << a << " equal to " << b << "?" << std::endl;
		assert(a == b);
	}

	static void assertAlmostEqual(double a, double b)
	{
		std::cout << "\t\tTesting: is " << a << " almost equal to " << b << "?" << std::endl;
		assert(a <= b + 0.0001 && a >= b - 0.0001);
	}

	static void TestAxisAngle()
	{
		std::cout << "TESTING AXIS-ANGLE..." << std::endl;
		std::cout << "Conversion from axis-angle to quaternion" << std::endl;
		std::cout << "\tTesting with axis (1, 1, 0)" << std::endl;
		Vec3 axis(1, 1, 0);
		double angle = 90;
		Quaternion quatFromAA(axis, angle);
		assertAlmostEqual(quatFromAA.getC(), sqrt(2) / 2);
		assertEqual(quatFromAA.getX(), 0.5);
		assertEqual(quatFromAA.getY(), 0.5);
		assertEqual(quatFromAA.getZ(), 0);

		std::cout << std::endl << "\tTesting with axis (0, 0, 1)" << std::endl;
		Vec3 axis2(0, 0, 1);
		Quaternion quatFromAA2(axis2, angle);
		assertAlmostEqual(quatFromAA2.getC(), sqrt(2) / 2);
		assertEqual(quatFromAA2.getX(), 0);
		assertEqual(quatFromAA2.getY(), 0);
		assertAlmostEqual(quatFromAA2.getZ(), sqrt(2) / 2);

		std::cout << std::endl << "Conversion back to axis-angle from quaternion" << std::endl;
		std::cout << "\tTesting with axis (1, 1, 0)" << std::endl;
		// Normalize original axis:
		axis.normalize();

		Vec4 axisAngle = quatFromAA.conversionAxisAngle();
		assertAlmostEqual(axisAngle.w, angle);
		assertAlmostEqual(axisAngle.x, axis.x);
		assertAlmostEqual(axisAngle.y, axis.y);
		assertEqual(axisAngle.z, axis.z);

		std::cout << std::endl << "\tTesting with axis (0, 0, 1)" << std::endl;
		Vec4 axisAngle2 = quatFromAA2.conversionAxisAngle();
		assertAlmostEqual(axisAngle2.w, angle);
		assertEqual(axisAngle2.x, axis2.x);
		assertEqual(axisAngle2.y, axis2.y);
		assertAlmostEqual(axisAngle2.z, axis2.z);
		std::cout << std::endl;
	}

	static void checkEulerAngles(const Vec3& eulerAngles, Quaternion& expectedQuaternion)
	{
		std::cout << "Testing euler angle (" << eulerAngles.x << ", " << eulerAngles.y << ", " << eulerAngles.z << ")" << std::endl;
		std::cout << " (roll = " << eulerAngles.x << " pitch = " << eulerAngles.y << " yaw = " << eulerAngles.z << ")" << std::endl;
		std::cout << "\tConversion from euler-angle to quaternion" << std::endl;
		Quaternion quatFromEuler(eulerAngles.x, eulerAngles.y, eulerAngles.z);
		assertAlmostEqual(quatFromEuler.getC(), expectedQuaternion.getC());
		assertAlmostEqual(quatFromEuler.getX(), expectedQuaternion.getX());
		assertAlmostEqual(quatFromEuler.getY(), expectedQuaternion.getY());
		assertAlmostEqual(quatFromEuler.getZ(), expectedQuaternion.getZ());

		std::cout << std::endl << "\tConversion back to euler angles from quaternion" << std::endl;
		Vec3 resEulerAngles = quatFromEuler.conversionEulerAngle();
		assertAlmostEqual(eulerAngles.x, convertAngleToDegreesFromRadian(resEulerAngles.x));
		assertAlmostEqual(eulerAngles.y, convertAngleToDegreesFromRadian(resEulerAngles.y));
		assertAlmostEqual(eulerAngles.z, convertAngleToDegreesFromRadian(resEulerAngles.z));

		std::cout << std::endl << std::endl;
	}

	static void TestEulerAngles()
	{
		//NOTE: We are using the ZYX order and (roll, pitch, yaw)
		// Tests can be done on this website: https://quaternions.online/
		std::cout << "TESTING EULER-ANGLE..." << std::endl;
		
		checkEulerAngles(Vec3(0, 0, 0), Quaternion(1, 0, 0, 0));
		checkEulerAngles(Vec3(90, 0, 0), Quaternion(sqrt(2) / 2, sqrt(2) / 2, 0, 0));
		checkEulerAngles(Vec3(180, 0, 0), Quaternion(1.0f, 0.0f, 0.0f));
		checkEulerAngles(Vec3(90, 40, 45), Quaternion(0.7064, 0.5213, 0.4777, 0.0308));
	}

	static void assertIsUnitQuaternion(Quaternion& quat)
	{
		assertAlmostEqual(1.0, quat.getNorm());
	}

	static void checkConjugateNorm(Quaternion& test, double expectedNorm)
	{
		std::cout << "TESTING CONJUGATE AND NORM..." << std::endl;
		Quaternion conjugateOfTest = test.conjugate();
		std::cout << "\tVerifying the value of the conjugate" << std::endl;
		assertEqual(test.getC(), conjugateOfTest.getC());
		assertEqual(-test.getX(), conjugateOfTest.getX());
		assertEqual(-test.getY(), conjugateOfTest.getY());
		assertEqual(-test.getZ(), conjugateOfTest.getZ());

		std::cout << "\tVerifying the value of the norm" << std::endl;
		assertAlmostEqual(expectedNorm, test.getNorm());
		std::cout << "\tThe normalized quaternion should be a unit quaternion" << std::endl;
		test.normalize();
		assertIsUnitQuaternion(test);
		std::cout << "\tNormalizing a unit quaternion should still work and yield a unit quaternion" << std::endl;
		test.normalize();
		assertIsUnitQuaternion(test);
	}

	static void TestConjugateNorm()
	{
		checkConjugateNorm(Quaternion(10, 11, 12, 13), 23.1084);
		checkConjugateNorm(Quaternion(1.0f, 1.0f, 0.0f), sqrt(2));
		checkConjugateNorm(Quaternion(1, 1, 1, 1), 2);
	}

	static void testMultiply(Quaternion& q1, Quaternion& q2, Quaternion& qP)
	{
		std::cout << "\ttesting q1 * q2 with q1 = " << q1.getC() << " + " << q1.getX() << "i + "
			<< q1.getY() << "j + " << q1.getZ() << "k and q2 = " << q2.getC() << " + " << q2.getX()
			<< "i + " << q2.getY() << "j + " << q2.getZ() << "k" << std::endl;
		std::cout << "\t\tqP = q1 * q2, where qP should be: q' = " << qP;
		assert(q1*q2 == qP);
	}

	static void TestArithmeticOperations()
	{
		std::cout << "TESTING ARITHMETIC OPERATIONS..." << std::endl;
		std::cout << "TESTING MULTIPLICATION" << std::endl;
		testMultiply(Quaternion(3, 1, 0, 0), Quaternion(6, 0, 0, 0), Quaternion(18, 6, 0, 0));
		testMultiply(Quaternion(7, 5, 0, 0), Quaternion(6, 0, 0, 1), Quaternion(42, 30, -5, 7));
	}

public:
	static void runUnitTests()	
	{
		TestAxisAngle();
		TestEulerAngles();
		TestConjugateNorm();
		TestArithmeticOperations();

		std::cout << "\nUNIT TEST STATUS: OK => ALL TESTS PASSED" << std::endl << std::endl;
	}
};