using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace serialtest
{
    class Program
    {
        static void Main(string[] args)
        {
            using (var sp = new SerialPort("COM11", 9600))
            {
                sp.Open();
                byte[] b = new byte[1];
                while (true)
                {
                    rnd.NextBytes(b);
                    sp.Write(b, 0, 1);
                    Thread.Sleep(50);
                }
            }
        }

        static Random rnd = new Random();

        static void Main1(string[] args)
        {
            Console.WriteLine("Type an integer 0-255 followed by enter. Ctrl-c to exit.");

            using (var sp = new SerialPort("COM11", 9600))
            {
                sp.Open();
                while (true)
                {
                    byte i;
                    string s = Console.ReadLine();
                    if (!byte.TryParse(s, out i) || i < 0 || i > 255)
                    {
                        Console.WriteLine("Invalid - enter 0 to 255");
                        continue;
                    }

                    sp.Write(new[] { i }, 0, 1);
                }
            }
        }
    }
}
