using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace ilrd.WCF
{
    public class CalculatorService : ICalculator
    {
        public int Add(int operand1, int operand2)
        {
            var result = operand1 + operand2;

            Console.WriteLine($"{operand1} + {operand2} = {result}");

            return result;
        }

        public int Substract(int operand1, int operand2)
        {
            var result = operand1 - operand2;

            Console.WriteLine($"{operand1} - {operand2} = {result}");

            return result;
        }
    }
}