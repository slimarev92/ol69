using ilrd.WS01;
using NUnit.Framework;

namespace ilrd.Tests
{
    public class StackTest
    {
        [Test]
        public void TestIsEmpty()
        {
            Stack stack = new Stack();

            bool expected = true;
            bool actual = stack.IsEmpty();

            /* stack is empty */
            Assert.AreEqual(expected, actual);

            /* push num to stack */
            stack.Push("23");

            expected = false;
            actual = stack.IsEmpty();

            /* stack is not empty */
            Assert.AreEqual(expected, actual);
        }

        [Test]
        public void TestPeek()
        {
            Stack stack = new Stack();
            int expected = 23;

            /* push number to stack */
            stack.Push(expected);

            /* stack is not empty */
            Assert.AreEqual(expected, stack.Peek());
        }

        [Test]
        public void TestPushPop()
        {
            Stack stack = new Stack();
            int[] nums = { 1, 2, 3, 4, 5, 6 };
            int[] expectedNums = { 6, 5, 4, 3, 2, 1 };

            /* push nums to stack */
            foreach (int num in nums)
            {
                stack.Push(num);
            }

            /* pop nums from stack and check against array of expected values */
            foreach (int num in expectedNums)
            {
                Assert.AreEqual(num, stack.Pop());
            }
        }

        [Test]
        public void TestClear()
        {
            Stack stack = new Stack();
            int[] nums = { 1, 2, 3, 4, 5, 6, 7 };

            /* push nums to stack */
            foreach (int num in nums)
            {
                stack.Push(num);
            }


            bool expected = false;
            bool actual = stack.IsEmpty();

            /* stack is not empty */
            Assert.AreEqual(expected, actual);

            stack.Clear();

            expected = true;
            actual = stack.IsEmpty();

            /* stack is empty */
            Assert.AreEqual(expected, actual);
        }

        [Test]
        public void TestPeekEmpty()
        {
            Stack stack = new Stack();

            /* should throw an exception since stack is empty */
            Assert.Throws<System.InvalidOperationException>(() => stack.Peek());
        }

        [Test]
        public void TestPopEmpty()
        {
            Stack stack = new Stack();

            /* should throw an exception since stack is empty */
            Assert.Throws<System.InvalidOperationException>(() => stack.Pop());
        }

        [Test]
        public void TestPushFull()
        {
            Stack stack = new Stack(1);

            stack.Push(23);

            /* should throw an expcetion since stack is full */
            Assert.Throws<System.InvalidOperationException>(() => stack.Push(23));
        }
    }
}