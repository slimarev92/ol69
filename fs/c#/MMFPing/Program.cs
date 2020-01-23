using System;
using System.Diagnostics;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Text;
using System.Threading;

namespace ilrd.WS04
{
    class Program
    {
        public static readonly string lineToSend = "Ping";
        public static readonly string mappedFileName = "pingpong";
        public static readonly string pingSemName = "ping_sem";
        public static readonly string pongSemName = "pong_sem";
        public const int numOfIters = 5;

        private static Process GetPongProccess()
        {
            var pongArgs = new System.Text.StringBuilder();

            pongArgs.Append(mappedFileName);
            pongArgs.Append(" ");
            pongArgs.Append(pingSemName);
            pongArgs.Append(" ");
            pongArgs.Append(pongSemName);
            pongArgs.Append(" ");
            pongArgs.Append(numOfIters);
            pongArgs.Append(" ");

            var pongProccess = new Process();

            pongProccess.StartInfo.Arguments = pongArgs.ToString();
            pongProccess.StartInfo.FileName = @"C:\Users\slima\source\repos\sasha-limarev\c#\MMFPong\bin\Debug\netcoreapp3.0\MMFPong.exe";

            return pongProccess;
        }

        private static void Run()
        {
            using var mappedFile = MemoryMappedFile.CreateNew(mappedFileName, 4096);
            using var streamView = mappedFile.CreateViewStream();
            using var pingWritten = new Semaphore(0, 1, pingSemName);
            using var pongWritten = new Semaphore(0, 1, pongSemName);

            using var pongProccess = GetPongProccess();

            pongProccess.Start();

            var buffer = new byte[256];

            for (int i = 0; i < numOfIters; ++i)
            {
                streamView.Seek(0, SeekOrigin.Begin);

                streamView.Write(Encoding.ASCII.GetBytes(lineToSend));

                pingWritten.Release();
                pongWritten.WaitOne();

                streamView.Seek(0, SeekOrigin.Begin);

                var bytesRead = streamView.Read(buffer, 0, 256);
                var currLine = Encoding.ASCII.GetString(buffer, 0, bytesRead);

                Console.WriteLine(currLine);
            }
        }

        static void Main()
        {
            Run();
        }
    }
}
