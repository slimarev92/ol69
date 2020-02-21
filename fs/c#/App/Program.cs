using System;
using System.Data.Entity;
using LearnEF;

namespace App
{
    class Program
    {
        public static void Main()
        {
            using (var storeContext = new StoreContext())
            {
                storeContext.Database.Log = Console.Write;

                var result = storeContext.Inventory.Include(item => item.Type);

                foreach (var item in result)
                {
                    Console.WriteLine($"{item.Type.Name}: {item.Price}");
                }

                try
                {
                    storeContext.SaveChanges();
                }
                catch(Exception e)
                {
                    System.Console.WriteLine(e.ToString());
                }
            }
        }
    }
}
