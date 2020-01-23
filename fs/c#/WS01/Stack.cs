namespace ilrd.WS01
{
    public class Stack
    {
        private const int DEFAULT_CAPACITY = 10;

        private readonly object[] data;
        private readonly int capacity;
        private int currIndex = 0;

        public Stack(int capacity = DEFAULT_CAPACITY)
        {
            this.capacity = capacity;
            data = new object[capacity];
        }

        public void Push(object element)
        {
            if (capacity <= currIndex)
            {
                throw new System.InvalidOperationException("Can't push to full a stack.");
            }

            data[currIndex] = element;
            ++currIndex;
        }

        public bool IsEmpty()
        {
            return (0 == currIndex);
        }

        public void Clear()
        {
            currIndex = 0;
        }

        public object Peek()
        {
            if (IsEmpty())
            {
                throw new System.InvalidOperationException("The stack is empty.");
            }

            return data[currIndex - 1];
        }

        public object Pop()
        {
            object retVal = Peek();

            --currIndex;

            return retVal;
        }
    }
}