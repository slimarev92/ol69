using System;
using System.IO;
using System.IO.Pipes;

namespace ilrd.WS04
{
    class Program
    {
        public static readonly string lineToSend = "Pong";
        public static int numOfIters = 0;

        public static void Run(string pipeName, string numOfItersStr)
        {
            numOfIters = Int32.Parse(numOfItersStr);

            using var clientPipe = new NamedPipeClientStream(".", pipeName, PipeDirection.InOut);

            clientPipe.Connect();

            using var clientWriter = new StreamWriter(clientPipe) { AutoFlush = true };
#pragma warning disable IDE0067 // Dispose objects before losing scope
            var clientReader = new StreamReader(clientPipe);
#pragma warning restore IDE0067 // Dispose objects before losing scope

            for (int i = 0; i < numOfIters; ++i)
            {
                var currLine = clientReader.ReadLine();

                Console.WriteLine(currLine);

                clientWriter.WriteLine(lineToSend);

                clientPipe.WaitForPipeDrain();
            }
        }

        static void Main(string[] args)
        {
            Run(args[0], args[1]);
        }
    }
}
