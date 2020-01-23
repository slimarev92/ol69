using System;
using System.IO.MemoryMappedFiles;
using System.Threading;
using System.IO;
using System.Text;

namespace ilrd.WS04
{
    class Program
    {
        private static readonly string lineToSend = "Pong";
        private static int numOfIters = 0;

        private static void Run(string[] args)
        {
            using var mappedFile = MemoryMappedFile.OpenExisting(args[0]);
            using var streamView = mappedFile.CreateViewStream();
            using var pingWritten = Semaphore.OpenExisting(args[1]);
            using var pongWritten = Semaphore.OpenExisting(args[2]);

            numOfIters = Int32.Parse(args[3]);

            var buffer = new byte[256];

            for (int i = 0; i < numOfIters; ++i)
            {
                pingWritten.WaitOne();

                streamView.Seek(0, SeekOrigin.Begin);

                var bytesRead = streamView.Read(buffer, 0, 256);

                Console.WriteLine(Encoding.ASCII.GetString(buffer, 0, bytesRead));

                streamView.Seek(0, SeekOrigin.Begin);
                streamView.Write(Encoding.ASCII.GetBytes(lineToSend));

                pongWritten.Release();
            }
        }

        static void Main(string[] args)
        {
            if(args.Length < 4) { throw new System.ArgumentException("Missing arguments", "args");  }

            Run(args);
        }
    }
}
