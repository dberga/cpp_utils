using System;
using System.Drawing;

namespace TextureTransfer
{
    public partial class TextureTransferTool
    {
        private class OuterLoopArgs
        {
            public int NumberOfIterations { get; private set; }
            public Size BlockSize;
            public Size Overlap;
            public Bitmap SourceBitmap;
            public Bitmap TargetBitmap;
            public Bitmap OutputBitmap;
            public rgb[,] Source { get; private set; }
            public rgb[,] Target { get; private set; }
            public rgb[,] Output { get; private set; }
            public double BlockReductionFactor;
            public double AmountToProbeFraction;
            public bool DisplayBoundaryCut;
            public Difference DifferencingFunction;
            private int iteration;

            public OuterLoopArgs(TextureTransferArgs args)
            {
                NumberOfIterations = args.NumberOfIterations;
                BlockSize = new Size(args.BlockWidth, args.BlockHeight);
                Overlap = new Size((int)Math.Round(args.OverlapXFraction * args.BlockWidth),
                    (int)Math.Round(args.OverlapYFraction * args.BlockHeight));
                SourceBitmap = args.SourceBitmap;
                TargetBitmap = args.TargetBitmap;
                Source = rgb.FromBitmap(SourceBitmap);
                Target = rgb.FromBitmap(TargetBitmap);
                Output = new rgb[TargetBitmap.Height, TargetBitmap.Width];
                BlockReductionFactor = args.BlockReductionFactor;
                AmountToProbeFraction = args.AmountToProbeFraction;
                DisplayBoundaryCut = args.DisplayBoundaryCut;
                if (args.metric == Metric.SSD)
                {
                    DifferencingFunction = rgb.Diff;
                }
                else
                {
                    DifferencingFunction = rgb.Diffi;
                }
            }

            /// <summary>
            /// return iteration #, starting from 0
            /// </summary>
            public int Iteration
            {
                get
                {
                    return iteration;
                }
            }

            private void Signal()
            {
                iteration++;
            }

            public bool Completed
            {
                get
                {
                    return iteration >= NumberOfIterations ||
                        this.Overlap.Width >= this.BlockSize.Width ||
                        this.Overlap.Height >= this.BlockSize.Height ||
                        this.BlockSize.Width == 0 ||
                        this.BlockSize.Height == 0;
                }
            }

            public void Next()
            {
                this.Signal();
                this.BlockSize = new Size((int)Math.Round(this.BlockSize.Width * this.BlockReductionFactor),
                    (int)Math.Round(this.BlockSize.Height * this.BlockReductionFactor));
                this.Overlap = new Size((int)Math.Round(this.Overlap.Width * this.BlockReductionFactor),
                    (int)Math.Round(this.Overlap.Height * this.BlockReductionFactor));
            }
        }
    }
}
