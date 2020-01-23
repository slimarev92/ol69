using System;
using System.IO;

namespace ilrd.WS04
{
    public class DifferencePrinter : IDisposable
    {
        private readonly StreamReader reader1;
        private readonly StreamReader reader2;

        public DifferencePrinter(string path1, string path2)
        {
            reader1 = GetTextReader(path1);
            reader2 = GetTextReader(path2);
        }

        public void PrintDifferences()
        {
            string currLine1 = reader1.ReadLine();
            string currLine2 = reader2.ReadLine();
            var lineNum = 1;

            while (null != currLine1 || null != currLine2)
            {
                if (null == currLine1)
                {
                    Console.WriteLine($"File 1 is over");
                    Console.WriteLine($"File 2, line {lineNum}: {currLine2}");
                    Console.WriteLine("*******************************************************");
                }
                else if (null == currLine2)
                {
                    Console.WriteLine($"File 1, line {lineNum}: {currLine1}");
                    Console.WriteLine($"File 2 is over");
                    Console.WriteLine("*******************************************************");
                }
                else if (!currLine1.Equals(currLine2))
                {
                    Console.WriteLine($"File 1, line {lineNum}: {currLine1}");
                    Console.WriteLine($"File 2, line {lineNum}: {currLine2}");
                    Console.WriteLine("*******************************************************");
                }

                ++lineNum;

                currLine1 = reader1.ReadLine();
                currLine2 = reader2.ReadLine();
            }
        }

        private static StreamReader GetTextReader(string path)
        {
            var file = new FileInfo(path);

            if (!file.Exists) { throw new FileNotFoundException($"{path} doesn't exist or isn't a file."); }

            return file.OpenText();
        }

        public void Dispose()
        {
            reader1.Dispose();
            reader2.Dispose();
        }
    }


    class Program
    {
        static void Main(string[] args)
        {
            //if (2 > args.Length) { throw new System.ArgumentException("Missing one or two file paths", "args"); }

            using var diffPrinter = new DifferencePrinter(@"C:\Users\slima\Desktop\a.txt", @"C:\Users\slima\Desktop\b.txt");

            diffPrinter.PrintDifferences();
        }
    }
}
