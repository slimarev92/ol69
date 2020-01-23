using System;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;

namespace ilrd.WS04
{
    class Program
    {
        private static readonly string lineToSend = "Ping";
        public static readonly string pipeName = "pingpong";
        private const int numOfIters = 10;

        public static void Run()
        {
            using var pongProccesss = new Process();
            using var serverPipe = new NamedPipeServerStream(pipeName, PipeDirection.InOut, 1);

            var clientArgs = new System.Text.StringBuilder();

            clientArgs.Append(pipeName);
            clientArgs.Append(" ");
            clientArgs.Append(numOfIters);

            pongProccesss.StartInfo.FileName = @"C:\Users\slima\source\repos\sasha-limarev\c#\NamedPipePong\bin\Debug\netcoreapp3.0\NamedPipePong.exe";
            pongProccesss.StartInfo.Arguments = clientArgs.ToString();

            pongProccesss.Start();

            serverPipe.WaitForConnection();

            var pipeWrite = new StreamWriter(serverPipe) { AutoFlush = true };
            var pipeRead = new StreamReader(serverPipe);

            for (int i = 0; i < numOfIters; ++i)
            {
                pipeWrite.WriteLine(lineToSend);

                serverPipe.WaitForPipeDrain();

                var currLine = pipeRead.ReadLine();

                Console.WriteLine(currLine);
            }
        }

        static void Main()
        {
            Run();
        }
    }
}