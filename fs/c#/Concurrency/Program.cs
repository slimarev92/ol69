using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

namespace ilrd.WS05
{
    public static class PrimeUtils
    {
        public static int FindNextPrime(int start)
        {
            var curr = start + 1;

            while (!IsPrime(curr))
            {
                ++curr;
            }

            return curr;
        }

        public static bool IsPrime(int num)
        {
            bool retVal = true;

            if (0 == (num % 2))
            {
                retVal = false;
            }
            else
            {
                for (int i = 3; i < ((num / 2) - 1); i += 2)
                {
                    if (0 == (num % i))
                    {
                        retVal = false;
                        break;
                    }
                }
            }

            return retVal;
        }
    }

    public class BusyWaitCP
    {
        private volatile bool canConsume = false;
        private readonly int maxNum;
        private int product = 0;

        public BusyWaitCP(int maxNum = 100) => this.maxNum = maxNum;

        private void ProducerMethod()
        {
            for (var i = 0; i <= maxNum; ++i)
            {
                if (PrimeUtils.IsPrime(i))
                {
                    while (canConsume) { }

                    product = i;
                    canConsume = true;
                }
            }
        }

        private void ConsumerMethod()
        {
            for (int i = 0, currNum = 0; i < 9593; ++i)
            {
                while (!canConsume) { }
                currNum = product;
                canConsume = false;
                //Console.WriteLine($"{i + 1}: {currNum}");
            }
        }

        public void Run()
        {
            var producerThread = new Thread(ProducerMethod);
            var consumerThread = new Thread(ConsumerMethod);
            var stopwatch = new Stopwatch();

            stopwatch.Start();

            consumerThread.Start();
            producerThread.Start();

            consumerThread.Join();
            producerThread.Join();

            stopwatch.Stop();

            Console.WriteLine($"Busy waiting - time elapsed: {stopwatch.ElapsedMilliseconds}ms");
        }
    }

    public class MonitorCP
    {
        private readonly WaitableListMonitor<int> primeList = new WaitableListMonitor<int>();
        private readonly int maxNum;
        private readonly int numProducers;

        public MonitorCP(int maxNum = 1000, int numProducers = 4)
        {
            this.maxNum = maxNum;
            this.numProducers = numProducers;
        }

        public void Producer(object rangeObj)
        {
            var numRange = (Range)rangeObj;


            for (var curr = numRange.Start; curr < numRange.End; ++curr)
            {
                if (PrimeUtils.IsPrime(curr))
                {
                    primeList.PushFront(curr);
                }
            }
        }

        public void Run()
        {
            var jump = maxNum / numProducers;
            var currStart = 0;
            var currEnd = 0;
            Thread[] producers = new Thread[numProducers];
            var stopwatch = new Stopwatch();

            stopwatch.Start();

            for (var i = 0; i < numProducers - 1; ++i)
            {
                currStart = currEnd;
                currEnd += jump;

                producers[i] = new Thread(Producer);

                producers[i].Start(new Range(currStart, currEnd));
            }

            producers[numProducers - 1] = new Thread(Producer);

            producers[numProducers - 1].Start(new Range(currEnd, maxNum + 1));

            for (var i = 0; i < numProducers; ++i)
            {
                producers[i].Join();
            }

            stopwatch.Stop();

            Console.WriteLine($"Monitor list - time elapsed: {stopwatch.ElapsedMilliseconds}ms");

            // List<int> regularList = primeList.GetRegularList();
            // regularList.Sort();

            //  var n = 0;

            // foreach (var num in regularList)
            //  {
            //      Console.WriteLine($"{n + 1}: {num}");
            //       ++n;
            //  }
        }
    }

    public class SemaphoreCP
    {
        private readonly WaitableListSemaphore<int> primeList = new WaitableListSemaphore<int>();
        private readonly int maxNum;
        private readonly int numProducers;

        public SemaphoreCP(int maxNum = 1000, int numProducers = 4)
        {
            this.maxNum = maxNum;
            this.numProducers = numProducers;
        }

        public void Producer(object rangeObj)
        {
            var numRange = (Range)rangeObj;

            for (var curr = numRange.Start; curr < numRange.End; ++curr)
            {
                if (PrimeUtils.IsPrime(curr))
                {
                    primeList.PushFront(curr);
                }
            }
        }

        public void Run()
        {
            var jump = maxNum / numProducers;
            var currStart = 0;
            var currEnd = 0;
            Thread[] producers = new Thread[numProducers];
            var stopwatch = new Stopwatch();

            stopwatch.Start();

            for (var i = 0; i < numProducers - 1; ++i)
            {
                currStart = currEnd;
                currEnd += jump;

                producers[i] = new Thread(Producer);

                producers[i].Start(new Range(currStart, currEnd));
            }

            producers[numProducers - 1] = new Thread(Producer);

            producers[numProducers - 1].Start(new Range(currEnd, maxNum + 1));

            for (var i = 0; i < numProducers; ++i)
            {
                producers[i].Join();
            }

            stopwatch.Stop();

            Console.WriteLine($"Semaphore list - time elapsed: {stopwatch.ElapsedMilliseconds}ms");

            //  List<int> regularList = primeList.GetRegularList();
            //  regularList.Sort();

            //  var n = 0;

            //  foreach (var num in regularList)
            //  {
            //      Console.WriteLine($"{n + 1}: {num}");
            //      ++n;
            //  }
        }
    }

    public struct Range
    {
        // Start is exclusive
        public int Start { get; }

        // End is enclusive
        public int End { get; }

        public Range(int start, int end)
        {
            Start = start;
            End = end;
        }

        public override bool Equals(object obj)
        {
            throw new NotImplementedException();
        }

        public override int GetHashCode()
        {
            throw new NotImplementedException();
        }

        public static bool operator ==(Range left, Range right)
        {
            return left.Equals(right);
        }

        public static bool operator !=(Range left, Range right)
        {
            return !(left == right);
        }
    }

    public class WaitableListMonitor<T>
    {
        private readonly ilrd.WS01.LinkedList<T> list = new ilrd.WS01.LinkedList<T>();

        public void PushFront(T element)
        {
            lock (list)
            {
                list.PushFront(element);
                Monitor.Pulse(list);
            }
        }

        public void PushBack(T element)
        {
            lock (list)
            {
                list.PushBack(element);
                Monitor.Pulse(list);
            }
        }

        public T PopFront()
        {
            T retVal;

            lock (list)
            {
                while (list.IsEmpty())
                {
                    Monitor.Wait(list);
                }

                retVal = list.PopFront();
            }

            return retVal;
        }

        public List<T> GetRegularList()
        {
            var regList = new List<T>();

            lock (list)
            {
                while (!list.IsEmpty())
                {
                    regList.Add(list.PopFront());
                }
            }

            regList.Sort();

            return regList;
        }
    }

    public class WaitableListSemaphore<T>
    {
        private readonly ilrd.WS01.LinkedList<T> list = new ilrd.WS01.LinkedList<T>();
        private readonly SemaphoreSlim numOfItems = new SemaphoreSlim(0);

        public void PushFront(T element)
        {
            lock (list)
            {
                list.PushFront(element);
                numOfItems.Release();
            }
        }

        public void PushBack(T element)
        {
            lock (list)
            {
                list.PushBack(element);
                numOfItems.Release();
            }
        }

        public T PopFront()
        {
            T retVal;

            numOfItems.Wait();

            lock (list)
            {
                retVal = list.PopFront();
            }

            return retVal;
        }

        public List<T> GetRegularList()
        {
            var regList = new List<T>();

            lock (list)
            {
                while (!list.IsEmpty())
                {
                    regList.Add(list.PopFront());
                }
            }

            regList.Sort();

            return regList;
        }
    }

    class Program
    {
        static void Main()
        {
            var numOfIters = 100000;
            var consProdBusy = new BusyWaitCP(numOfIters);
            var consProdMonitor = new MonitorCP(numOfIters, 1);
            var consProdSem = new SemaphoreCP(numOfIters, 1);

            consProdBusy.Run();
            consProdMonitor.Run();
            consProdSem.Run();
        }
    }
}
