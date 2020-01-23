using System;
using System.Collections.Generic;
using System.Text;

namespace ilrd.WS03
{
    public class ChainConsole
    {
        private readonly LinkedList<Command> commandHandlers = new LinkedList<Command>();
        private bool isRunning = true;

        public ChainConsole() => RegisterHandler(message => message.Equals("exit"), message => isRunning = false);

        public void RegisterHandler(Predicate<string> condition, Action<string> handler) =>
            commandHandlers.AddLast(new Command(condition, handler));

        public void Run()
        {
            RegisterHandler(message => true,
                            message => Console.WriteLine($"The input:|{message}| doesn't match any known command"));

            while (isRunning)
            {
                Console.Write("> ");
                var command = Console.ReadLine();

                foreach (var handler in commandHandlers)
                {
                    if (true == handler.InvokeIfMatches(command))
                    {
                        break;
                    }
                }
            }
        }

        public static void ReverseText(string message)
        {
            var reversedBuilder = new StringBuilder();

            for (int i = (message.Length - 1); 0 <= i; --i)
            {
                reversedBuilder.Append(message[i]);
            }

            Console.WriteLine(reversedBuilder.ToString());
        }

        public static void CountChars(string message) =>
           Console.WriteLine($"The command |{message}| contains {message.Length} charactares");

        private class Command
        {
            private readonly Predicate<string> condition;
            private readonly Action<string> handler;

            public Command(Predicate<string> condition, Action<string> handler)
            {
                this.condition = condition;
                this.handler = handler;
            }

            public bool InvokeIfMatches(string command)
            {
                var retVal = false;

                if (true == condition(command))
                {
                    retVal = true;
                    handler(command);
                }

                return retVal;
            }
        }
    }
}