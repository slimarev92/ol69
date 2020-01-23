using System;
using System.ServiceModel;
using System.ServiceModel.Description;
using ilrd.WCF;

namespace ilrd.WCF
{
    class Program
    {
        static void Main()
        {
            //the URI of the service, defined in the service's App.config file
            var baseAddress = new Uri("http://localhost:8000/ilrd.WCF");

            // the service host
            using (var selfServiceHost = new ServiceHost(typeof(CalculatorService), baseAddress))
            {
                try
                {
                    //this will allow clients to get the service's contract via http
                    var metadataBehavior = new ServiceMetadataBehavior() { HttpGetEnabled = true };

                    //expose an endpoint with the following contract, using an http binding, with the given address
                    selfServiceHost.AddServiceEndpoint(typeof(ICalculator), new WSHttpBinding(), "CalculatorService");
                    selfServiceHost.Description.Behaviors.Add(metadataBehavior);

                    //open service
                    selfServiceHost.Open();

                    Console.WriteLine("Opened calculator service");

                    //close service
                    Console.ReadLine();

                    selfServiceHost.Close();
                    Console.WriteLine("Closed calculator service");

                }
                catch (CommunicationException e)
                {
                    Console.WriteLine("an error occurred lol");
                    Console.WriteLine($"{e}");
                    selfServiceHost.Abort();
                }
            }
        }
    }
}
