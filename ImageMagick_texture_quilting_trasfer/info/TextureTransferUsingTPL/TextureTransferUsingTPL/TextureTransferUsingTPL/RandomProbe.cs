/*
 * let N = Width * Height = # of pixels in image
 * let n = # of random probes 
 * all n probes may not be distinct. What is the expected # of double/triple/... hits?
 * n probes give C(n,2) combinations
 * let Yij = Xi == Xj
 * E[Yij] = p = 1/N
 * then # of non-unique hits Z = summation Yij
 * E[Z] = summation E[Yij] = C(n,2)*p = n(n-1)/(2*N) I have verifies through simulations that formula is correct c:/c#temp/temp12.cs
 * if E[Z] > a*n where a is in [0,1]; a is fraction of non-unique hits; go in for a complex scheme in which you maintain a book-keeping list and return unique probes
 * E[Z] > a*n gives n > 1+2*a*N with a=0.5 n > 1+N
 * for N large when n = N, E[Z] = N/2 or about 50% probes are non-unique
 * I don't think there is any need to go in for the complex scheme when we maintain a list and make sure we don't return non-unique hits
 */
using System;
using System.Drawing;
namespace TextureTransfer
{
    /// <summary>
    /// This probe will randomly probe the source image to find matching blocks. The amount to probe is set
    /// using the fractionToProbe parameter. Notes:
    /// 1. The Random() class is not threadsafe. See http://blogs.msdn.com/b/pfxteam/archive/2009/02/19/9434171.aspx
    /// 2. Even if it were, this class won't be threadsafe (see Next() method for example)    
    /// </summary>
    public class RandomProbe : Probe
    {
        private int counter, width, height, maxAttempts;
        private Random rand;        

        public RandomProbe(int Width, int Height, double fractionToProbe)
        {            
            rand = new Random();
            width = Width; height = Height; maxAttempts = (int) (fractionToProbe * width * height);
        }

        public override Point Next()
        {
            int i = rand.Next(width * height);
            int Y = i / width;
            int X = i - width * Y;
            counter++;
            return new Point(X, Y);
        }

        public override bool IsCompleted
        {
            get { return counter >= maxAttempts;  }
        }

        public override void Reset()
        {
            this.counter = 0;
        }        
    }
}
