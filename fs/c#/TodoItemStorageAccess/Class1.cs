using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Data.Entity;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

namespace ilrd.TodoItemStorageAccess
{
    [Serializable]
    public class TodoItem
    {
        public int Id { set; get; }
        public string content;
        public bool done;

        public TodoItem(int Id, string content, bool done)
        {
            this.Id = Id;
            this.content = content;
            this.done = done;
        }
    }

    public class TodoItemContext : DbContext
    {
        public TodoItemContext() : base("mysssdqlConnection")
        {

        }

        public DbSet<TodoItem> Items { get; set; }
    }

    public class Test
    {
        public static void Run()
        {
            using (var itemContext = new TodoItemContext())
            {
                itemContext.Items.Add(new TodoItem(1, "Wash the dishes", false));
                Console.WriteLine("here");


                itemContext.Items.Add(new TodoItem(1, "Walk the dogs", false));
                itemContext.Items.Add(new TodoItem(1, "Learn WCF", true));
            }
        }
    }
}
