using System;

namespace TextureTransfer
{
    /// <summary>
    /// This probe will cause an exhaustive search to be performed on the source image to find matching
    /// source texture blocks.
    /// </summary>
    public class ExhaustiveProbe : Probe
    {
        int counter, width, height;

        /// <summary>
        /// creates the probe
        /// </summary>
        /// <param name="Width">width of source image</param>
        /// <param name="Height">height of source image</param>
        public ExhaustiveProbe(int Width, int Height)
        {
            counter = 0;
            width = Width; height = Height;
        }

        /// <summary>
        /// return the next (X,Y) location in the source which can be checked to see if it qualifies to be a matching block.
        /// </summary>
        /// <param name="X"></param>
        /// <param name="Y"></param>
        /// <param name="probeCompleted"></param>
        public override void Next(out int X, out int Y, out bool probeCompleted)
        {
            Y = counter / width;
            X = counter - width * Y;
            if (counter >= width * height)
                probeCompleted = true;
            else
                probeCompleted = false;
            counter++;
        }

        public override void Reset()
        {
            this.counter = 0;
        }

        public override void Reset(int Width, int Height)
        {
            this.counter = 0;
            width = Width; Height = height;
        }
    }
}
