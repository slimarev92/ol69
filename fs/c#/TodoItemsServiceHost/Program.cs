using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ServiceModel;
using ilrd.WCF;

namespace TodoItemsServiceHost
{
    class Program
    {
        static void Main()
        {
            var serviceUri = new Uri("http://localhost:8080/");

            using (var service = new ServiceHost(typeof(TodoItemsService), serviceUri))
            {
                service.AddServiceEndpoint(typeof(ITodoItemsService), new WebHttpBinding(), "");

                service.Open();

                Console.WriteLine("Started todo items service");

                Console.ReadLine();

                service.Close();
            }
        }
    }
}
