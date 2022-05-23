using System;
using System.Drawing;
using System.ComponentModel;

namespace TextureTransfer
{
    /// <summary>
    /// SSD: sum of squared r, g, b differences
    /// IntensityDiffSquared: intensity difference squared
    /// </summary>
    public enum Metric { SSD, IntensityDiffSquared };
    
    /// <summary>
    /// The texture transfer tool will periodically send a progress report to the UI thread.
    /// </summary>
    public class ProgressReport
    {
        private double  fractionCompleted;
        private Image   image;

        /// <summary>
        /// fraction of texture transfer that has been completed. range: [0,1]
        /// </summary>
        public double FractionCompleted { get { return fractionCompleted; } }
        
        /// <summary>
        /// the synthesized texture transfer image.
        /// </summary>
        public Image Image { get { return image; } }

        /// <summary>
        /// creates a progress report.
        /// </summary>
        /// <param name="fraction">fraction of texture transfer that has been completed. range: [0,1]</param>
        /// <param name="img">the synthesized texture transfer image.</param>
        public ProgressReport(double fraction, Image img)
        {
            fractionCompleted = fraction;
            image = img;
        }
    }

    /// <summary>
    /// Summary description for TextureTransfer.
    /// </summary>
    public class TextureTransferTool : BackgroundWorker
    {
        private delegate double Difference(rgb left, rgb right);

        private int[] findHcut(double[,] e)
        {            
            if (e == null)
                return null;
            int overlapY = e.GetLength(0);
            if (overlapY < 3)   // too small an overlap; cut is not going to be of much use
                return null;
            int w1width = e.GetLength(1);
            int[] H = new int[w1width];
            int x, y;
            double[,] E = new double[overlapY, w1width];
            for (x = 0; x < w1width; x++)
                for (y = 0; y < overlapY; y++)
                {
                    if (x == 0)
                        E[y, x] = e[y, x];
                    else if (y == 0)
                        E[y, x] = e[y, x] + Math.Min(E[y, x - 1], E[y + 1, x - 1]);
                    else if (y == overlapY - 1)
                        E[y, x] = e[y, x] + Math.Min(E[y - 1, x - 1], E[y, x - 1]);
                    else
                        E[y, x] = e[y, x] + Math.Min(Math.Min(E[y - 1, x - 1], E[y, x - 1]), E[y + 1, x - 1]);
                }
            x = w1width - 1;
            H[x] = 0;
            for (y = 1; y < overlapY; y++)
                if (E[y, x] < E[H[x], x])
                    H[x] = y;
            for (x = w1width - 2; x >= 0; x--)
            {
                int v = H[x + 1];
                H[x] = v;
                if (v > 0)
                    if (E[v - 1, x] < E[H[x], x])
                        H[x] = v - 1;
                if (v < overlapY - 1)
                    if (E[v + 1, x] < E[H[x], x])
                        H[x] = v + 1;
            }
            return H;
        } /* end fn */

        private int[] findVcut(double[,] e)
        {
            if (e == null)
                return null;
            int x, y;
            int w1height = e.GetLength(0);
            int overlapX = e.GetLength(1);
            if (overlapX < 3)
                return null;
            int[] V = new int[w1height];
            double[,] E = new double[w1height, overlapX];
            for (y = 0; y < w1height; y++)
                for (x = 0; x < overlapX; x++)
                {                    
                    if (y == 0)
                        E[y, x] = e[y, x];
                    else if (x == 0)
                        E[y, x] = e[y, x] + Math.Min(E[y - 1, x], E[y - 1, x + 1]);
                    else if (x == overlapX - 1)
                        E[y, x] = e[y, x] + Math.Min(E[y - 1, x - 1], E[y - 1, x]);
                    else
                        E[y, x] = e[y, x] + Math.Min(Math.Min(E[y - 1, x - 1], E[y - 1, x]), E[y - 1, x + 1]);
                }
            y = w1height - 1;
            V[y] = 0;
            for (x = 1; x < overlapX; x++)
                if (E[y, x] < E[y, V[y]])
                    V[y] = x;
            for (y = w1height - 2; y >= 0; y--)
            {
                int v = V[y + 1];
                V[y] = v;
                if (v > 0)
                    if (E[y, v - 1] < E[y, V[y]])
                        V[y] = v - 1;
                if (v < overlapX - 1)
                    if (E[y, v + 1] < E[y, V[y]])
                        V[y] = v + 1;
            }
            return V;
        }

        

        private void cutandpaste(rgb[,] src,
            Point srcTopLeft,
            rgb[,] dest,
            Point destTopLeft,
            int[] Hcut,
            int[] Vcut,
            int blockWidth,
            int blockHeight,
            bool displayBoundaryCut)
        {
            rgb red = new rgb(255, 0, 0);
            for (int x = 0; x < blockWidth; x++)
                for (int y = 0; y < blockHeight; y++)
                {
                    bool flag1 = Vcut == null || (Vcut != null && x > Vcut[y]);
                    bool flag2 = Hcut == null || (Hcut != null && y > Hcut[x]);
                    if (flag1 && flag2)
                    {
                        dest[destTopLeft.Y + y, destTopLeft.X + x] = src[srcTopLeft.Y + y, srcTopLeft.X + x];
                        continue;
                    }
                    if (Vcut != null && x == Vcut[y] && displayBoundaryCut)
                    {
                        dest[destTopLeft.Y + y, destTopLeft.X + x] = red;
                        continue;
                    }
                    if (Hcut != null && y == Hcut[x] && displayBoundaryCut)
                    {
                        dest[destTopLeft.Y + y, destTopLeft.X + x] = red;
                        continue;
                    }
                }
        } /* end fn */               

        /// <summary>
        /// Get matching block from src which will be pasted at location topLeft of img.
        /// </summary>
        /// <param name="img">the image being synthesized</param>
        /// <param name="src">source image</param>
        /// <param name="target">target image</param>
        /// <param name="topLeft">coordinates of top left corner of the block being synthesized in img.
        /// the matching block from src will be pasted at this location in img units: pixels</param>
        /// <param name="overlapX">amount of overlap in X dimension units: pixels</param>
        /// <param name="overlapY">amount of overlap in Y dimension units: pixels</param>
        /// <param name="blockWidth">width of block units: pixels</param>
        /// <param name="blockHeight">height of block units: pixels</param>
        /// <param name="alpha">weighting factor range: (0,1)</param>
        /// <param name="isFirstIteration">is this the first iteration</param>
        /// <param name="probe"></param>
        /// <param name="Eh">error matrix for horizontal cut</param>
        /// <param name="Ev">error matrix for vertical cut</param>
        /// <returns>coordnates of top left corner of the matching block in src</returns>
        private Point GetMatchingBlock(rgb[,] img,	// image being synthesized			
            rgb[,] src,				// source image
            rgb[,] target,			// target image
            Point topLeft,			// topLeft corner of block to be synthesized in img
            int overlapX,
            int overlapY,
            int blockWidth,
            int blockHeight,
            double alpha,
            bool isFirstIteration,
            Probe probe,
            out double[,] Eh,   // error matrix for H cut
            out double[,] Ev)   // error matrix for V cut
        {
            Point p = new Point(0, 0);
            int srcWidth = src.GetLength(1);
            int srcHeight = src.GetLength(0);
            double d, d1, d2, dmin = double.MaxValue;
            Eh = null; Ev = null;
            double[,] eh = null, ev = null;
            if (overlapY > 0)            
                eh = new double[overlapY, blockWidth];                     
            if (overlapX > 0)
                ev = new double[blockHeight, overlapX];
            int x, y;
            bool probeCompleted = false;
            probe.Reset();
            while (!probeCompleted && !CancellationPending)
            {
                probe.Next(out x, out y, out probeCompleted);
                d = 0.0; d1 = 0; d2 = 0;
                for (int j = 0; j < blockHeight; j++)
                    for (int i = 0; i < blockWidth; i++)
                    {                        
                        if (i < overlapX || j < overlapY || !isFirstIteration)
                        {   // either we are in overlapping region or this is not our first iteration                            
                            double diff = rgb.Diff(img[topLeft.Y + j, topLeft.X + i], src[y + j, x + i]);
                            if (j < overlapY && i < overlapX)
                                eh[j, i] = ev[j, i] = diff;
                            else if (j < overlapY)
                                eh[j, i] = diff;
                            else if (i < overlapX)
                                ev[j, i] = diff;
                            d1 += diff;
                        }                                                
                        d2 += Diff(target[topLeft.Y + j, topLeft.X + i], src[y + j, x + i]);
                    }

                d = alpha * d1 + (1.0 - alpha) * d2;

                if (d < dmin)
                {
                    dmin = d;
                    p.X = x; p.Y = y;
                    if (eh != null)
                        Eh = (double[,]) eh.Clone();
                    if (ev != null)
                        Ev = (double[,]) ev.Clone();
                }
            }
            return p;
        } /* end fn */

        private void Next(int X, int Y,
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

        /// <summary>
        /// returns the fraction of texture transfer that has been completed. 
        /// </summary>
        /// <param name="iteration"></param>
        /// <param name="totalNumberOfIterations"></param>
        /// <param name="X"></param>
        /// <param name="Y"></param>
        /// <param name="blockWidth"></param>
        /// <param name="blockHeight"></param>
        /// <param name="imageWidth"></param>
        /// <param name="imageHeight"></param>
        /// <returns>returns the fraction of texture transfer that has been completed. range: [0,1]</returns>
        private double CalculateFractionComplete(int iteration,
            int totalNumberOfIterations,
            int X, int Y, int blockWidth, int blockHeight, int imageWidth, int imageHeight)
        {
            double w = X + blockWidth;
            double area1 = w * (Y + blockHeight);
            double area2 = (imageWidth - w) * Y;
            double covered = area1 + area2;
            double imageArea = imageWidth * imageHeight;
            return (iteration * imageArea + covered) / (totalNumberOfIterations * imageArea);
        }

        protected override void OnDoWork(DoWorkEventArgs e)
        {
            int X = 0; int Y = 0;
            int nextX, nextY, imgWidth = target.GetLength(1), imgHeight = target.GetLength(0),
                nextBlockWidth, nextBlockHeight, srcWidth = src.GetLength(1), srcHeight = src.GetLength(0);
            
            rgb[,] img = new rgb[imgHeight, imgWidth];
            int overlapX = (int)Math.Round(overlapXFraction * blockWidth);
            int overlapY = (int)Math.Round(overlapYFraction * blockHeight);
            int nextOverlapX, nextOverlapY;
            Probe probe;
            double[,] eh, ev;
            int probeWidth = srcWidth - blockWidth;
            int probeHeight = srcHeight - blockHeight;
            if (amountToProbeFraction >= 1)
                probe = new ExhaustiveProbe(probeWidth, probeHeight);
            else
                probe = new RandomProbe(probeWidth, probeHeight, amountToProbeFraction);
            Point p;
            for (int i = 0; i < numIterations; i++)
            {
                bool isFirstIteration = i == 0;
                double alpha = 0.8 * i / (numIterations - 1) + 0.1;     // from Alyosha's paper
                nextX = 0; nextY = 0; nextBlockWidth = blockWidth; nextBlockHeight = blockHeight; nextOverlapX = 0; nextOverlapY = 0;
                bool completed = false;
                while (!completed && !CancellationPending)
                {
                    Point q = new Point(nextX, nextY);  
                    // get matching block from src which will be pasted at location q in img
                    p = GetMatchingBlock(img, src, target, q, nextOverlapX, nextOverlapY,
                        nextBlockWidth, nextBlockHeight, alpha, isFirstIteration, probe, out eh, out ev);                    
                    // the horizontal and vertical cuts will ensure that the patch from src will fit in
                    // seamlessly when it is pasted onto img
                    int[] Hcut = findHcut(eh);  // compute the horizontal cut
                    int[] Vcut = findVcut(ev);  // compute the vertical cut
                    // cut the block per the horizontal and vertical cuts and then paste it onto img
                    cutandpaste(src, p, img, q, Hcut, Vcut, nextBlockWidth, nextBlockHeight, displayBoundaryCut);                                        
                    // calculate how much of texture transfer is complete; 1->fully completed
                    double fractionComplete = CalculateFractionComplete(i, numIterations, nextX, nextY, nextBlockWidth,
                        nextBlockHeight, imgWidth, imgHeight);
                    int percentComplete = (int)Math.Round(fractionComplete * 100);
                    // send progress report to UI thread
                    ReportProgress(percentComplete, new ProgressReport(fractionComplete, rgb.ToBitmap(img)));
                    // (nextX, nextY): coordinates of top left corner in img where next block is to be synthesized;
                    // (nextBlockWidth, nextBlockHeight): dimensions of block to be synthesized
                    // (nextOverlapX, nextOverlapY): the amount by which block will will overlap neighbors
                    // completed: is this iteration complete
                    Next(X, Y, blockWidth, blockHeight, imgWidth, imgHeight, overlapX, overlapY,
                        out nextX, out nextY, out nextBlockWidth, out nextBlockHeight, out nextOverlapX, out nextOverlapY,
                        out completed);                    
                    X = nextX; Y = nextY;                    
                }
                if (CancellationPending)
                {
                    e.Cancel = true;
                    return;
                }
                X = 0; Y = 0;
                blockWidth = (int)Math.Round(blockWidth * blockReductionFactor);
                blockHeight = (int)Math.Round(blockHeight * blockReductionFactor);
                probeWidth = srcWidth - blockWidth;
                probeHeight = srcHeight - blockHeight;
                probe.Reset(probeWidth, probeHeight);
                overlapX = (int)Math.Round(overlapX * blockReductionFactor);
                overlapY = (int)Math.Round(overlapY * blockReductionFactor);
                if (overlapX >= blockWidth || overlapY >= blockHeight || blockWidth == 0 || blockHeight == 0)
                    break;
            }
            e.Result = rgb.ToBitmap(img);
        }

        private rgb[,] src, target;
        private int blockWidth, blockHeight, numIterations;
        private double overlapXFraction, overlapYFraction, blockReductionFactor, amountToProbeFraction;
        private bool displayBoundaryCut;
        private Difference Diff;

        /// <summary>
        /// create texture transfer tool
        /// </summary>
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
        public TextureTransferTool(Bitmap SourceBitmap,
            Bitmap TargetBitmap,
            int BlockWidth,
            int BlockHeight,
            int NumberOfIterations,
            double OverlapXFraction,
            double OverlapYFraction,
            double BlockReductionFactor,
            double AmountToProbeFraction,
            bool    DisplayBoundaryCut,
            Metric  metric)
        {
            src = rgb.FromBitmap(SourceBitmap);
            target = rgb.FromBitmap(TargetBitmap);
            blockWidth = BlockWidth;
            blockHeight = BlockHeight;
            numIterations = NumberOfIterations;
            overlapXFraction = OverlapXFraction;
            overlapYFraction = OverlapYFraction;
            blockReductionFactor = BlockReductionFactor;
            amountToProbeFraction = AmountToProbeFraction;
            displayBoundaryCut = DisplayBoundaryCut;
            if (metric == Metric.SSD)
                Diff = new Difference(rgb.Diff);
            else
                Diff = new Difference(rgb.Diffi);
            WorkerReportsProgress = true;
            WorkerSupportsCancellation = true;
        }
    }
}