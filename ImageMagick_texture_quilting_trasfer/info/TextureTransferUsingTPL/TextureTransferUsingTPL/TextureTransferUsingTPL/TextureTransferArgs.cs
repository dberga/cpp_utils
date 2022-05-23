using System.Drawing;
namespace TextureTransfer
{
    /// <summary>
    /// Class encapsulating inputs needed to perform a texture transfer
    /// </summary>
    public class TextureTransferArgs
    {
        public Bitmap SourceBitmap { get; private set; }
        public Bitmap TargetBitmap { get; private set; }
        public int BlockWidth { get; private set; }
        public int BlockHeight { get; private set; }
        public int NumberOfIterations { get; private set; }
        public double OverlapXFraction { get; private set; }
        public double OverlapYFraction { get; private set; }
        public double BlockReductionFactor { get; private set; }
        public double AmountToProbeFraction { get; private set; }
        public bool DisplayBoundaryCut { get; private set; }
        public Metric metric { get; private set; }

        /// <param name="SourceBitmap">source image</param>
        /// <param name="TargetBitmap">target image</param>
        /// <param name="BlockWidth">block width of source texture; units: pixels; suggested value: 27</param>
        /// <param name="BlockHeight">block height of source texture; units: pixels; suggested value: 27</param>
        /// <param name="NumberOfIterations">number of iterations to do; suggested value: 3</param>
        /// <param name="OverlapXFraction">amount of overlap along X dimension; units: fraction (0,1); suggested value: 1/3</param>
        /// <param name="OverlapYFraction">amount of overlap along Y dimension; units: fraction (0,1); suggested value: 1/3</param>
        /// <param name="BlockReductionFactor">factor by which block size would be reduced in each successive iteration; units: fraction (0,1); suggested value: 1/3</param>
        /// <param name="AmountToProbeFraction">determines how exhaustively you want to search the source image for a matching block.
        /// The texture transfer process works by finding matching blocks in the source texture and weaving them together like
        /// jig-saw pieces to create the synthesized image. The main bottleneck in the process is to find the matching block
        /// at every iteration step. The weaving of the blocks is not expensive. By adjusting the <param name="AmountToProbeFraction">
        /// one can trade off quality for speed. The <param name="AmountToProbeFraction"> is a fraction ranging
        /// from (0,1]. Setting <param name="AmountToProbeFraction"> = 0.1 for example will probe only 10% of the
        /// source image to find a matching block. Values of 0.1 have been used with acceptable results.</param>
        /// <param name="DisplayBoundaryCut">if set to true the boundary cuts are displayed in red</param>
        public TextureTransferArgs(Bitmap src, Bitmap target,
            int blockWidth,
            int blockHeight,
            int iterations,
            double overlapX, double overlapY,
            double reduction,
            double amount,
            bool display,
            Metric metric)
        {
            SourceBitmap = src;
            TargetBitmap = target;
            BlockWidth = blockWidth;
            BlockHeight = blockHeight;
            NumberOfIterations = iterations;
            OverlapXFraction = overlapX;
            OverlapYFraction = overlapY;
            BlockReductionFactor = reduction;
            AmountToProbeFraction = amount;
            DisplayBoundaryCut = display;
            this.metric = metric;
        }
    }
}
