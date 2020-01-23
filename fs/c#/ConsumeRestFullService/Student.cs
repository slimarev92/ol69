using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsumeRestFullService
{
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

    public class Student
    {
        public string ID { get; set; }
        public string Name { get; set; }
    }
}
