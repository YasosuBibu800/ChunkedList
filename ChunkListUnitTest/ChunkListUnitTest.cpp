#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../ChunkList/ChunkList.h"

using namespace fefu_laboratory_two;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChunkListUnitTest
{
	TEST_CLASS(ChunkListTests)
	{
	public:
		using ChunkList = ChunkList<int, 5>;
		TEST_METHOD(ctor)
		{
			ChunkList list;
			Assert::IsTrue(list.size() == 0);
		}


		TEST_METHOD(assign)
		{
			ChunkList expected;
			for (int i = 0; i < 10; i++) expected.push_back(7);
			ChunkList actual;


			actual.assign(10, 7);


			Assert::IsTrue(expected == actual);
		}



		TEST_METHOD(assign_opearator)
		{
			ChunkList list1;
			ChunkList list2 = { 1,2,3,4,5 };
			list1 = { 1,2,3,4,5 };
			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(at)
		{
			ChunkList list;
			for (int i = 0; i < 15; i++) list.push_back(i);
			for (int i = 0; i < 15; i++) Assert::AreEqual(i, list.at(i));
		}

		TEST_METHOD(indexation)
		{
			ChunkList list;
			for (int i = 0; i < 15; i++) list.push_back(i);
			for (int i = 0; i < 15; i++) Assert::AreEqual(i, list.at(i));
		}

		TEST_METHOD(front)
		{
			ChunkList list = { 4, 1, 5, 7, 4, 1 };
			Assert::IsTrue(list.front() == 4);
		}

		TEST_METHOD(back)
		{
			ChunkList list = { 1, 1, 5, 8, 0, 2, 5, 7, 2 };
			Assert::IsTrue(list.back() == 2);
		}

		TEST_METHOD(empty) {
			ChunkList list;
			Assert::IsTrue(list.empty() == true);
			Assert::IsTrue(list.size() == 0);
			Assert::IsTrue(list.max_size() == 0);
		}

		TEST_METHOD(no_empty)
		{
			ChunkList list;
			list = { 1, 2, 3, 4, 5, 6, 7 };
			Assert::IsTrue(list.size() == 7);
			Assert::IsTrue(list.max_size() == 10);
		}

		TEST_METHOD(emplace)
		{
			ChunkList actual;
			actual = { 1,2,3,4,5 };
			ChunkList expected;
			expected = { 1,2,3,1,2,3,4,5 };
			actual.emplace(actual.cbegin() + 3, 1,2,3);
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(emplace_back)
		{
			ChunkList actual = { 1,2,3,4,5 };
			ChunkList expected = { 1,2,3,4,5,1,2 };
			actual.emplace_back(1, 2);
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(emplace_front)
		{
			ChunkList list = { 1,2,3,4,5 };
			ChunkList expected = { 1,2,3,1,2,3,4,5 };
			auto it2 = list.emplace_front(1,2,3);
			Assert::IsTrue(list == expected);
		}

		TEST_METHOD(Insert1) {
			ChunkList list;
			ChunkList list_should;
			list_should.push_back(100);
			list_should.push_back(0);
			list_should.push_back(1);
			list_should.push_back(2);

			for (int i = 0; i < 10; i++)
				list.push_back(i);

			list.clear();

			Assert::IsTrue(list.empty() == true);
			Assert::IsTrue(list.size() == 0);
			Assert::IsTrue(list.max_size() == 0);

			for (int i = 0; i < 3; i++)
				list.push_back(i);

			auto it = list.cbegin();
			list.insert(it, 100);

			Assert::IsTrue(list == list_should);
		}

		TEST_METHOD(Insert2) {
			std::vector<int> vec = { -1,22,-333 };
			ChunkList list = { 1,2,3,4,5 };
			ChunkList expected = { -1,22,-333,1,2,3,4,5 };

			list.insert(list.cbegin(), vec.begin(), vec.end());

			Assert::IsTrue(list == expected);
		}

		TEST_METHOD(Resize)
		{
			ChunkList list1 = { 1,2,3,4,5 };
			ChunkList list2 = { 1,2,3,4,5,-1,-1,-1,-1,-1};

			list1.resize(10, -1);

			Assert::IsTrue(list1 == list2);

			list1.resize(4);
			list2.resize(4);

			Assert::IsTrue(list1 == list2);
		}

		TEST_METHOD(erase) 
		{
			ChunkList actual = {1,2,3,4,5, 6};
			ChunkList expected = { 1,2,4,5,6 };
			actual.erase(actual.cbegin() + 1, actual.cbegin() + 2);
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(PushBack) {
			ChunkList list;

			for (int i = 0; i < 11; i++)
				list.push_back(i);

			Assert::IsTrue(list[0] == 0);
			Assert::IsTrue(list[10] == 10);
		}

		TEST_METHOD(PushFront) {
			ChunkList list;

			for (int i = 0; i < 11; i++)
				list.push_back(i);

			list.push_front(42);
			Assert::IsTrue(list[0] == 42);
		}

		TEST_METHOD(PopBack) {
			ChunkList list;

			for (int i = 0; i < 11; i++)
				list.push_back(i);

			list.pop_back();
			Assert::IsTrue(list.size() == 10);
		}

		TEST_METHOD(PopFront) {
			ChunkList list;

			for (int i = 0; i < 11; i++)
				list.push_back(i);

			list.pop_front();
			Assert::IsTrue(list[0] == 1);
		}

		TEST_METHOD(Comparisions) {
			ChunkList list1 = { 1,2,3,4,5 };
			ChunkList list2 = { 1,2,3,4,5 };
			ChunkList list3 = { 1,2,6,4,5 };
			ChunkList list4 = { 1,2,3,4,5,1 };


			Assert::IsTrue(list1 == list2);
			Assert::IsTrue(list1 <= list2);
			Assert::IsTrue(list1 >= list2);
			Assert::IsTrue(list3 > list2);
			Assert::IsTrue(list4 > list2);
			Assert::IsTrue(list1 < list4);
			Assert::IsFalse(list2 > list3);
		}
	};
}
