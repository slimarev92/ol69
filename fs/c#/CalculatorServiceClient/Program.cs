using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalculatorServiceClient
{
    class Program
    {
        static void Main()
        {
            using (var calcClient = new CalculatorClient())
            {
                while (true)
                {
                    Console.WriteLine("Enter an operand:");

                    var operand1 = Int32.Parse(Console.ReadLine());

                    Console.WriteLine("Enter a second operand:");

                    var operand2 = Int32.Parse(Console.ReadLine());

                    Console.WriteLine("Enter an operation (+, -):");

                    var operation = Console.ReadLine()[0];

                    var result = 0;

                    switch (operation)
                    {
                        case '+':
                            result = calcClient.Add(operand1, operand2);
                            break;
                        case '-':
                            result = calcClient.Substract(operand1, operand2);
                            break;
                    }

                    Console.WriteLine($"{operand1} {operation} {operand2} = {result}");
                }
            }

        }
    }
}
