using System;
using System.Drawing;
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
        /// <param name="width">width of source image</param>
        /// <param name="height">height of source image</param>
        public ExhaustiveProbe(int width, int height)
        {
            counter = 0;
            this.width = width; this.height = height;
        }

        /// <summary>
        /// return the next (X,Y) location in the source which can be checked to see if it qualifies to be a matching block.
        /// </summary>
        /// <param name="X"></param>
        /// <param name="Y"></param>
        /// <param name="probeCompleted"></param>
        public override Point Next()
        {
            int Y = counter / width;
            int X = counter - width * Y;
            counter++;
            return new Point(X, Y);
        }

        public override bool IsCompleted
        {
            get { return counter >= width * height;  }
        }

        public override void Reset()
        {
            this.counter = 0;
        }
    }
}
