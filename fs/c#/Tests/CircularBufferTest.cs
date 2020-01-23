using NUnit.Framework;
using System;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Generic;
using ilrd.WS05;

namespace ilrd.Tests
{
    class CircularBufferTest
    {
        [Test]
        public void SingleThreadTest()
        {
            var buffer = new CircularBuffer<int>(1000);

            for(var i = 0; i < 1000; ++i)
            {
                buffer.Write(i);
            }

            for(var expected = 0; expected < 1000; ++expected)
            {
                int actual = buffer.Read();

                Assert.AreEqual(expected, actual);
            }
        }

        [Test]
        public void TwoThreadTest()
        {
            var buffer = new CircularBuffer<int>(10);
            var resultList = new List<int>();

            var writerThread = new Thread(() =>
            {
                for(var i = 0; i < 1000; ++i)
                {
                    buffer.Write(i);
                }
            });

            var readerThread = new Thread(() =>
            {
                for (var curr = 0; curr < 1000; ++curr)
                {
                    var currRead = buffer.Read();

                    resultList.Add(currRead);
                }
            });

            writerThread.Start();
            readerThread.Start();

            readerThread.Join();

            for(var expected = 0; expected < 1000; ++expected)
            {
                var actual = resultList[expected];
                Assert.AreEqual(expected, actual);
            }
        }

        [Test]
        public void ThreeThreadTest()
        {
            var buffer = new CircularBuffer<int>(1);
            var resultList = new int[1000];

            Array.Fill(resultList, 500);

            var writerThread = new Thread(() =>
            {
                for (var i = 0; i < 1000; ++i)
                {
                    buffer.Write(i);
                }
            });

            var readerThread1 = new Thread(() =>
            {
                for (var i = 0; i < 500; ++i)
                {
                    var currRead = buffer.Read();

                    resultList[currRead] = currRead;
                }
            });

            var readerThread2 = new Thread(() =>
            {
                for (var i = 0; i < 500; ++i)
                {
                    var currRead = buffer.Read();

                    resultList[currRead] = currRead;
                }
            });

            writerThread.Start();
            readerThread1.Start();
            readerThread2.Start();

            readerThread1.Join();
            readerThread2.Join();

            for (var expected = 999; expected > 0; --expected)
            {
                var actual = resultList[expected];
                Assert.AreEqual(expected, actual);
            }
        }

        [Test]
        public void TaskTest()
        {
            var buffer = new CircularBuffer<int>(10);

            var writerTask = new Task(() =>
            {
                for (var i = 0; i < 1000; ++i)
                {
                    buffer.Write(i);
                }
            });

            var readerTask = new Task(() =>
            {
                for (var curr = 0; curr < 1000; ++curr)
                {
                    var currRead = buffer.Read();

                    Assert.AreEqual(curr, currRead);
                }
            });

            writerTask.Start();
            readerTask.Start();

            try
            {
                readerTask.Wait();
            }
            catch(AggregateException e)
            {
                if(e.InnerException.GetType().Equals(typeof(NUnit.Framework.AssertionException)))
                {
                    throw e.InnerException;
                }
            }
        }
    }
}
