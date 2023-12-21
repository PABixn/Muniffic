using System;

namespace eg
{
    public class Main
    {
        public float FloatVar { get; set; }
        public Main()
        {
            Console.WriteLine("Main constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }

        public void PrintMessage(string message)
        {
            Console.WriteLine(message);
        }

        public void PrintInts(int a, int b)
        {
            Console.WriteLine(a + b);
        }
    }
}