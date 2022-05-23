using System;

namespace TextureTransfer
{
    class SimpleProbe : Probe
    {
        int x, y, width, height, xIncrement, yIncrement;

        public SimpleProbe(int Width, int Height, double fractionToProbe)
        {
            x = y = 0;
            width = Width;
            height = Height;
            fractionToProbe = Math.Min(fractionToProbe, 1.0);
            fractionToProbe = Math.Max(fractionToProbe, 0.001); // probe at least .1%
            double d = 1.0 / fractionToProbe;
            xIncrement = yIncrement = (int) Math.Round(Math.Sqrt(d));
        }

        public override void Next(out int X, out int Y, out bool probeCompleted)
        {
            X = x;
            Y = y;
            probeCompleted = false;
            // calculate values that will be returned in next call to Next
            x += xIncrement;
            if (x >= width)
            {
                x = 0;
                y += yIncrement;
                if (y >= height)
                {
                    y = 0;
                    probeCompleted = true;
                }
            }
        }

        public override void Reset()
        {
            x = 0; y = 0;
        }

        public override void Reset(int Width, int Height)
        {
            x = 0; y = 0; width = Width; height = Height;
        }
    }
}
