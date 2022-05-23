using System;
using System.Drawing;

namespace TextureTransfer
{
    public partial class TextureTransferTool
    {
        private class InnerLoopArgs
        {
            public double BlockReductionFactor;
            public int Iteration;
            public int TotalNumberOfIterations;
            public Point TopLeft;
            public bool Completed;
            /// <summary>
            /// what you want
            /// </summary>
            public Size BlockSizeWanted;
            public Size OverlapWanted;
            /// <summary>
            /// what you get
            /// </summary>
            public Size BlockSize;
            public Size Overlap;
            public rgb[,] Output;
            public rgb[,] Source;
            public rgb[,] Target;
            public double Alpha;
            public Probe Probe;
            public Difference DifferencingFunction;
            public bool DisplayBoundaryCut;            

            public InnerLoopArgs(OuterLoopArgs args, double alpha)
            {                            
                TotalNumberOfIterations = args.NumberOfIterations;
                TopLeft = new Point();
                BlockReductionFactor = args.BlockReductionFactor;
                BlockSizeWanted = args.BlockSize;
                BlockSize = BlockSizeWanted;
                Completed = false;
                DifferencingFunction = args.DifferencingFunction;
                DisplayBoundaryCut = args.DisplayBoundaryCut;
                Iteration = args.Iteration;
                OverlapWanted = args.Overlap;
                Overlap = new Size(0, 0); // first block does not overlap with anything
                int probeWidth = args.SourceBitmap.Width - args.BlockSize.Width;
                int probeHeight = args.SourceBitmap.Height - args.BlockSize.Height;
                Source = args.Source;
                Target = args.Target;
                Output = args.Output;
                if (args.AmountToProbeFraction >= 1)
                {
                    this.Probe = new ExhaustiveProbe(probeWidth, probeHeight);
                }
                else
                {
                    this.Probe = new RandomProbe(probeWidth, probeHeight, args.AmountToProbeFraction);
                }
                Alpha = alpha;
            }

            public void Next()
            {
                var X = TopLeft.X;
                var Y = TopLeft.Y;
                var blockWidth = BlockSizeWanted.Width;
                var blockHeight = BlockSizeWanted.Height;
                var overlapX = OverlapWanted.Width;
                var overlapY = OverlapWanted.Height;
                var imgWidth = Output.GetLength(1);
                var imgHeight = Output.GetLength(0);
                int nextX, nextY, nextBlockWidth, nextBlockHeight, nextOverlapX, nextOverlapY;
                bool completed;
                Next(X, Y, blockWidth, blockHeight, imgWidth, imgHeight, overlapX, overlapY,
                    out nextX, out nextY, out nextBlockWidth, out nextBlockHeight, out nextOverlapX, out nextOverlapY,
                    out completed);
                this.TopLeft = new Point(nextX, nextY);
                this.BlockSize = new Size(nextBlockWidth, nextBlockHeight);
                this.Overlap = new Size(nextOverlapX, nextOverlapY);
                this.Completed = completed;
            }

            private static void Next(int X, int Y,
                    int blockWidth, int blockHeight,
                    int imgWidth, int imgHeight,
                    int overlapX, int overlapY,
                    out int nextX, out int nextY,
                    out int nextBlockWidth, out int nextBlockHeight,
                    out int nextOverlapX, out int nextOverlapY,
                    out bool completed)
            {
                completed = false;
                nextOverlapX = overlapX; nextOverlapY = overlapY;
                nextX = X + blockWidth - overlapX;
                if (nextX >= imgWidth - overlapX)
                {
                    nextX = 0;
                    nextY = Y + blockHeight - overlapY;
                    if (nextY >= imgHeight - overlapY)
                        completed = true;
                }
                else
                    nextY = Y;
                nextBlockWidth = Math.Min(blockWidth, imgWidth - nextX);
                nextBlockHeight = Math.Min(blockHeight, imgHeight - nextY);
                if (nextX == 0) // only overlap in Y
                    nextOverlapX = 0;
                if (nextY == 0)
                    nextOverlapY = 0;
            }
        }
    }
}
