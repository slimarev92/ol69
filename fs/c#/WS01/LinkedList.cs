using System.Collections;
using System.Collections.Generic;

namespace ilrd.WS01
{
    /// <summary>
    /// A generic singly linked list.
    /// </summary>
    /// <typeparam name="T"> The type of the list.</typeparam>
    public class LinkedList<T> : IEnumerable<T>
    {
        private Node headNode = null;
        private long modCount = 0;
        private int _count = 0;

        /// <summary>
        /// The number of nodes in the list.
        /// </summary>
        public int Count { get => _count; }

        /// <summary>
        /// Returns an element from the given index.
        /// </summary>
        /// <param name="i"> The index of the element (starting from 0). </param>
        /// <returns> Return a reference to the element. </returns>
        public T this[int i]
        {
            get
            {
                if (_count <= i) { throw new System.ArgumentException("Index is larger than list size."); }

                using IEnumerator<T> iterator = GetEnumerator();

                iterator.MoveNext();

                for (int count = 0; count < i; ++count)
                {
                    iterator.MoveNext();
                }

                return iterator.Current;
            }
        }

        /// <summary>
        /// Adds the given element to the beginning list.
        /// </summary>
        /// <param name="element"> The element to be added. </param>
        public void PushFront(T element)
        {
            headNode = new Node(element, headNode);
            ++_count;
            ++modCount;
        }

        /// <summary>
        /// Adds the given element to the end of the list
        /// </summary>
        /// <param name="element"> The element to be added. </param>
        public void PushBack(T element)
        {
            var curr = headNode;
            var newNode = new Node(element, null); ;

            if (null == curr)
            {
                headNode = newNode;
            }
            else
            {
                while(null != curr.Next)
                {
                    curr = curr.Next;
                }

                curr.Next = newNode;
            }
        }

        /// <summary>
        /// Returns a reference to the first element in the list.
        /// </summary>
        /// <returns> Returns the first element of the list </returns>
        public T Head()
        {
            if (IsEmpty())
            {
                throw new System.InvalidOperationException("The list is empty");
            }

            return headNode.Data;
        }

        /// <summary>
        /// Removes the first element in the list and returns a reference to that element.
        /// </summary>
        /// <returns> Returns a reference to the first element of the list.</returns>
        public T PopFront()
        {
            T retVal = Head();

            headNode = headNode.Next;
            --_count;
            ++modCount;

            return retVal;
        }

        /// <summary>
        /// Clears all the nodes in the list.
        /// </summary>
        public void Clear()
        {
            headNode = null;
            _count = 0;
            ++modCount;
        }

        /// <summary>
        /// Checks wheteer the list is empty.
        /// </summary>
        /// <returns>Returns true if the list is empty, false otherwise.</returns>
        public bool IsEmpty()
        {
            return (0 == Count);
        }

        /// <summary>
        /// Gets an Enumerator for the list.
        /// </summary>
        /// <returns>Returns an Enumerator of the list's elements.</returns>
        public IEnumerator<T> GetEnumerator()
        {
            return new IEnumeratorImpl(this);
        }

        /// <summary>
        /// Gets an Enumerator for the list.
        /// </summary>
        /// <returns>Returns an Enumerator of the list's elements.</returns>
        IEnumerator IEnumerable.GetEnumerator() => this.GetEnumerator();

        private class Node
        {
            public Node Next { get; set; }
            public T Data { get; }

            public Node(T data, Node next)
            {
                Data = data;
                Next = next;
            }
        }

        private class IEnumeratorImpl : IEnumerator<T>
        {
            private readonly long initialModCount;
            private readonly LinkedList<T> list;
            private Node currentNode = null;
            private bool movedOnce = false;

            object IEnumerator.Current { get => (object)Current; }
            public T Current { get => currentNode.Data; }

            public IEnumeratorImpl(LinkedList<T> list)
            {
                this.list = list;
                initialModCount = list.modCount;
            }

            public bool MoveNext()
            {
                if (initialModCount != list.modCount)
                {
                    throw new System.InvalidOperationException("The list was modified after the creation of the enumerator.");
                }

                if (!movedOnce)
                {
                    movedOnce = true;
                    currentNode = list.headNode;
                }
                else
                {
                    if (null != currentNode)
                    {
                        currentNode = currentNode.Next;
                    }
                }

                return (null != currentNode);
            }

            public void Dispose()
            {
                /*  If there are no additional resources to dispose of, 
                 *  provide an empty Dispose() implementation */
            }

            public void Reset()
            {
                movedOnce = false;
            }
        }
    }
}