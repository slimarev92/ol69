using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Configuration;
using System.Data.SqlClient;
using TodoItems;

namespace TestApp
{
    class Program
    {
        static void Main()
        {
            try
            {
                var items = new ItemAccess();

                var newItems = new List<TodoItem>();

                newItems.Add(new TodoItem(1, "Wash the dishes", false));
                newItems.Add(new TodoItem(2, "Walk the dogs", false));
                newItems.Add(new TodoItem(3, "Buy Food", true));

                items.AddItems(newItems);

                items.Save();

                /*
                foreach (var item in items.GetUserItems())
                {
                    Console.WriteLine(item.Id);
                    Console.WriteLine(item.Content);
                    Console.WriteLine(item.Done);
                }
                */

            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }

            Console.WriteLine("Finished");
        }
    }
}