using System;
using System.Collections.Generic;
using System.Threading;

namespace ilrd.WS05
{
    public class CircularBuffer<T> 
    {
        private int Capacity { get; }
        private readonly T[] data;

        private readonly SemaphoreSlim readableElements;
        private readonly SemaphoreSlim writeableElements;
        private int readIndex = 0;
        private int writeIndex = 0;

        public CircularBuffer(int capacity = 10)
        {
            Capacity = capacity;
            data = new T[capacity];
            readableElements = new SemaphoreSlim(0, Capacity);
            writeableElements = new SemaphoreSlim(Capacity, Capacity);
        }

        public void Write(T element)
        {
           writeableElements.Wait();

           lock(writeableElements)
            {
                data[writeIndex] = element;

                writeIndex = (writeIndex == (Capacity - 1)) ? 0 : (writeIndex + 1);

                readableElements.Release();
            }
        }

        public T Read()
        {
            readableElements.Wait();

            lock(readableElements)
            {
                T retVal = data[readIndex];

                writeableElements.Release();
                readIndex = (readIndex == (Capacity - 1)) ? 0 : (readIndex + 1);

                return retVal;
            }
        }
    }
}
