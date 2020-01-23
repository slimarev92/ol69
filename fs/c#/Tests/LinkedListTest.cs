using NUnit.Framework;

namespace ilrd.Tests
{
    class LinkedListTest
    {
        [Test]
        public void TestIndexer()
        {
            var list = new ilrd.WS01.LinkedList<int>();

            for (int i = 0; 100 > i; ++i)
            {
                list.PushFront(i);
            }

            for (var expected = 0; 100 > expected; ++expected)
            {
                int actual = list[99 - expected];

                Assert.AreEqual(expected, actual);
            }
        }
    }
}
