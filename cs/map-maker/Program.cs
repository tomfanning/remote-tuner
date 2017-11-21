using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace map_maker
{
    class Program
    {
        static void Main(string[] args)
        {
            int[] caps = { 330, 220, 100, 100, 33, 27, 20, 18 };

            var distinctVals = new Dictionary<double, byte>();

            for (int i = 0; i < Math.Pow(2, caps.Length); i++)
            {
                var bits = getBits((byte)i);

                double inv = 0;

                for (int c = 0; c < caps.Length; c++)
                {
                    if (bits[c])
                    {
                        inv += 1 / (double)caps[c];
                    }
                }

                double sum = Math.Round(1 / inv, 0);

                if (!distinctVals.ContainsKey(sum))
                {
                    distinctVals.Add(sum, (byte)i);
                }
            }

            var ordered = distinctVals.OrderBy(k => k.Key);

            Console.WriteLine("int possibleCapacitances[] = {{{0}}};", String.Join(", ", ordered.Select(o => o.Key)));
            Console.WriteLine("int obtainedBySetting[] = {{{0}}};", String.Join(", ", ordered.Select(o => o.Value)));

            /*foreach (var kvp in ordered)
            {
                double forCap = kvp.Key;
                byte setTo = kvp.Value;

                Console.WriteLine("{0} {1}", forCap, setTo);
            }*/

            Console.ReadLine();
        }

        private static bool[] getBits(byte val)
        {
            var ba = new BitArray(new[] { val });
            var result = new bool[8];
            ba.CopyTo(result, 0);
            return result.Reverse().ToArray();
        }
    }
}