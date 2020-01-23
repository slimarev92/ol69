using System;
using System.IO;
using System.IO.Pipes;
using System.Threading;

namespace ilrd.WS04
{
    class PongProgram
    {
        private const string lineToSend = "Pong";

        public static void Run(string inPipeHandle, string outPipeHandle, string timesToRunStr)
        {
            int timesToRun = Int32.Parse(timesToRunStr);

            Console.WriteLine(outPipeHandle);

            using var clientPipeOut = new AnonymousPipeClientStream(PipeDirection.Out, outPipeHandle);
            using var clientPipeIn = new AnonymousPipeClientStream(PipeDirection.In, inPipeHandle);
            using var clientWriter = new StreamWriter(clientPipeOut) { AutoFlush = true };
            using var clientReader = new StreamReader(clientPipeIn);

            for (int i = 0; i < timesToRun; ++i)
            {
                var currLine = clientReader.ReadLine();

                Console.WriteLine(currLine);

                clientWriter.WriteLine(lineToSend);

                clientPipeOut.WaitForPipeDrain();

                Thread.Sleep(100);
            }
        }

        static void Main(string[] args)
        {
            if (args.Length < 3) { throw new System.ArgumentException("Missing client pipe handle", nameof(args)); }

            Run(args[0], args[1], args[2]);
        }
    }
}
