using System;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Threading;

namespace ilrd.WS04
{
    class PingProgram
    {
        private const string lineToSend = "Ping";
        private const int timesToRun = 100000;

        public static void Run()
        {
            using var clientProccess = new Process();
            using var outPipe = new AnonymousPipeServerStream(PipeDirection.Out, HandleInheritability.Inheritable);
            using var inPipe = new AnonymousPipeServerStream(PipeDirection.In, HandleInheritability.Inheritable);

            var clientHandleOut = outPipe.GetClientHandleAsString();
            var clientHandleIn = inPipe.GetClientHandleAsString();

            var clientArgs = new System.Text.StringBuilder();

            clientArgs.Append(clientHandleOut);
            clientArgs.Append(" ");
            clientArgs.Append(clientHandleIn);
            clientArgs.Append(" ");
            clientArgs.Append(timesToRun);

            clientProccess.StartInfo.Arguments = clientArgs.ToString();
            clientProccess.StartInfo.FileName = @"C:\Users\slima\source\Repos\sasha-limarev\fs\c#\AnonPipeClient\bin\Debug\netcoreapp3.0\AnonPipePong.exe";

            clientProccess.Start();

            outPipe.DisposeLocalCopyOfClientHandle();
            inPipe.DisposeLocalCopyOfClientHandle();

            using var serverReader = new StreamReader(inPipe);
            using var serverWriter = new StreamWriter(outPipe) { AutoFlush = true };

            for (int i = 0; i < timesToRun; ++i)
            {
                serverWriter.WriteLine(lineToSend);

                outPipe.WaitForPipeDrain();

                var currLine = serverReader.ReadLine();

                Console.WriteLine(currLine);

                Thread.Sleep(100);
            }
        }

        static void Main()
        {
            Run();
        }
    }
}
