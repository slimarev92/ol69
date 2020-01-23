using System;
using System.Collections.Generic;

namespace ilrd
{
    [Serializable]
    public class TodoItem
    {
        public int id;
        public string item;
        public bool done;

        public TodoItem(int id, string item, bool done)
        {
            this.id = id;
            this.item = item;
            this.done = done;
        }
    }

    public class Service : ITodoService
    {
        public List<TodoItem> GetItems()
        {
            var items = new List<TodoItem>()
            {
                new TodoItem(1, "Wash the dishes", false),
                new TodoItem(2, "Walk the dogs", false),
                new TodoItem(3, "Learn WCF", false)
            };

            return items;
        }
    }
}
