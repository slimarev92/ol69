using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Configuration;

namespace TestApp
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
        public TodoItemContext() : base()
        {
            foreach (var conString in ConfigurationManager.ConnectionStrings)
            {
                Console.WriteLine(conString);
            }
        }

        public DbSet<TodoItem> Items { get; set; }
    }

    class Program
    {
        static void Main()
        {
            try
            {
                using (var itemContext = new TodoItemContext())
                {
                    itemContext.Items.Add(new TodoItem(1, "Wash the dishes", false));
                    Console.WriteLine("here");


                    itemContext.Items.Add(new TodoItem(1, "Walk the dogs", false));
                    itemContext.Items.Add(new TodoItem(1, "Learn WCF", true));
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }

            Console.WriteLine("Done");
        }
    }
}
