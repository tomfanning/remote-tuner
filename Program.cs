using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace myApp
{
    class Program
    {
        static void Main(string[] args)
        {
            double[] bestSet = null;
            double lowestEvenness = double.MaxValue;
            double lowestDistance = double.MaxValue;

            //var values = new double[] {390,330,270,200,160,130,30,20};
            while (true)
            {
                var values = PickCaps();

                // 256 possible solutions for this set of 8 caps

                List<Solution> solutions = new List<Solution>();

                for (int i=1; i<Math.Pow(2, values.Length); i++)
                {
                    BitArray bits = new BitArray(new int[]{i});
                    // Console.WriteLine("{0} {1} {2} {3} {4} {5} {6} {7}", bits[0], bits[1], bits[2], bits[3], bits[4], bits[5], bits[6], bits[7]);

                    Solution s = new Solution();

                    for (int b = 0; b <values.Length; b++)
                    {
                        if (bits[b])
                        {

                            s.Caps.Add(values[b]);
                            
                        }
                    }

                    solutions.Add(s);
                }

                var distinctCapacitances = solutions.Select(s=>s.ResultingCapacitance).Distinct().OrderBy(v=>v).ToArray();

                //foreach (var value in distinctCapacitances)
                //{
                    // find the simplest solution for each
                    //var solutionsForValue = solutions.Where(s=>s.ResultingCapacitance == value).OrderBy(s=>s.Caps.Count);

                    //var simplestSolution = solutionsForValue.First();

                    //Console.WriteLine("{0} with {1} bits", simplestSolution.ResultingCapacitance, simplestSolution.Caps.Count);
                    //Console.Write("{0:0.0} ", simplestSolution.ResultingCapacitance);
                    //Console.WriteLine();
                //}

                /*foreach (var solution in solutions.Where(s=>s.ResultingCapacitance == double.PositiveInfinity))
                {
                    Console.Write("Inf: ");
                    foreach (var cap in solution.Caps)
                    {
                        Console.Write("{0} ", cap);
                    }
                    Console.WriteLine();
                }*/

                var solutionValues = solutions.Select(s=>s.ResultingCapacitance).OrderBy(s=>s).ToArray();

                // mean gap between values - minimise
                var meanGap = MeanGap(solutionValues);

                // standard deviation of gaps between values - minimise
                //double evenness = GapDeviation(solutionValues);

                double evenness = MaxGapSize(solutionValues);

                // measure of the range - minimise
                //var minDistanceFrom10 = DistanceFrom(solutionValues, 10);

                // measure of the range - minimise
                //var minDistanceFrom400 = DistanceFrom(solutionValues, 400);

                // use the whole range
                var minDistanceFrom10 = Math.Abs(10-solutionValues.Min());
                var minDistanceFrom400 = Math.Abs(400-solutionValues.Max());

                // these can just be combined
                var minDistances = minDistanceFrom10 + minDistanceFrom400;

                //if ((minDistances < lowestDistance && evenness <= lowestEvenness)
                //    || (minDistances <= lowestDistance && evenness < lowestEvenness))
                if (evenness < lowestEvenness && minDistances <= 100)
                {
                    bestSet = values;
                    lowestDistance = minDistances;
                    lowestEvenness = evenness;

                    Console.Write("Nice result: ");
                    foreach (var v in values.OrderBy(v=>v))
                    {
                        Console.Write(v + " ");
                    }
                    Console.WriteLine();
                    Console.WriteLine("Cap range: {0:0}-{1:0}  Distance from 10-400: {2:0}  max gap: {3:0}", 
                        solutions.Select(s=>s.ResultingCapacitance).Min(),
                        solutions.Select(s=>s.ResultingCapacitance).Max(),
                        minDistances,
                        evenness
                    );

                    Console.Write("Capacitances: ");
                    foreach (var value in distinctCapacitances)
                    {
                        // find the simplest solution for each
                        var solutionsForValue = solutions.Where(s=>s.ResultingCapacitance == value).OrderBy(s=>s.Caps.Count);

                        var simplestSolution = solutionsForValue.First();

                        //Console.WriteLine("{0} with {1} bits", simplestSolution.ResultingCapacitance, simplestSolution.Caps.Count);
                        Console.Write("{0:0} ", simplestSolution.ResultingCapacitance);
                        
                    }
                    Console.WriteLine();
                    Console.WriteLine();
                }

                //Console.ReadLine();
                //return;

            }
        }

        static Random rnd = new Random();
        static double[] PickCaps()
        {
            //return new double[]{18, 39, 130, 240, 240, 360, 360, 390};

            // return 8 standard capacitor values
            /*var pool = new double[]{8.2,9.1,10,11,12,13,15,16,18,20,22,24,27,30,33,36,
                39,43,47,51,56,62,68,75,82,91,100,110,120,130,150,160,180,200,
                220,240,270,300,330,360,390,430,470,510,560,620,680,750,820,910,1000
            };*/            

            var pool = new double[]{10,15,18,20,22,27,30,33,39,47,56,68,100,220,330,470};
            
            var set = new double[8];
            for (int i=0;i<8;i++)
            {
                var randOrd = rnd.Next(0, pool.Length);
                set[i] = pool[randOrd];
            }
            return set;
        }

        static double DistanceFrom(IEnumerable<double> values, double from){
            double result = double.MaxValue;

            foreach (double val in values)
            {
                var mag = Math.Abs(val - from);

                if (mag < result)
                {
                    result = mag;
                }

            }   
            return result;
        }
        static double MeanGap(IEnumerable<double> values)
        {
            var ordered = values.OrderBy(o=>o).ToArray();

            var distances = new List<Double>();
            for (int i =0; i< ordered.Length-1; i++){
                var distanceToNext = ordered[i+1] - ordered[i];
                distances.Add(distanceToNext);
            }

            return distances.Average();
        }

        static double MaxGapSize(IEnumerable<double> values)
        {
            var ordered = values.OrderBy(o=>o).ToArray();

            var distances = new List<Double>();
            for (int i =0; i< ordered.Length-1; i++){
                var distanceToNext = ordered[i+1] - ordered[i];
                distances.Add(distanceToNext);
            }

            return distances.Max();
        }
        static double GapDeviation(IEnumerable<double> values)
        {
            var ordered = values.OrderBy(o=>o).ToArray();

            var distances = new List<Double>();
            for (int i =0; i< ordered.Length-1; i++){
                var distanceToNext = ordered[i+1] - ordered[i];
                distances.Add(distanceToNext);
            }

            return StdDev(distances);
        }

        private static double StdDev(IEnumerable<double> values)
        {   
            double ret = 0;
            if (values.Count() > 0) 
            {      
                //Compute the Average      
                double avg = values.Average();
                //Perform the Sum of (value-avg)_2_2      
                double sum = values.Sum(d => Math.Pow(d - avg, 2));
                //Put it all together      
                ret = Math.Sqrt((sum) / (values.Count()-1));   
            }   
            return ret;
        }
    }

    class Solution{
        
        public double ResultingCapacitance {get{
            double inverse = 0;
            foreach (double val in Caps)
            {
                inverse += 1/val;
            }
            return Math.Round( 1/inverse, 0, MidpointRounding.AwayFromZero);
        }
        }

        public List<double> Caps {get;set;}
        
        public Solution(){
            Caps = new List<double>();
        }
    }
}
