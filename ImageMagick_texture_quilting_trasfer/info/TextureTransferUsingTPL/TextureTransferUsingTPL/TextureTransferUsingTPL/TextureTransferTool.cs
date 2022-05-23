using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Linq;
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
        /// <summary>
        /// fraction of texture transfer that has been completed. range: [0,1]
        /// </summary>        
        public double FractionCompleted { get; private set; }
        /// <summary>
        /// the block of pixels that has been synthesized.
        /// </summary>        
        public Image Image { get; private set; }
        /// <summary>
        /// the rectangle where the block of pixels goes
        /// </summary>
        public Rectangle Rectangle { get; private set; }
        
        /// <summary>
        /// creates a progress report.
        /// </summary>
        /// <param name="fraction">fraction of texture transfer that has been completed. range: [0,1]</param>
        /// <param name="img">the synthesized texture transfer image.</param>
        public ProgressReport(double fraction, Image bmp, Rectangle r)
        {
            FractionCompleted = fraction;
            Image = bmp;
            Rectangle = r;
        }
    }
    
    /// <summary>
    /// Summary description for TextureTransfer.
    /// </summary>
    public partial class TextureTransferTool
    {
        /// <summary>
        /// a reference to a <c>Control</c> is need to marshal callbacks to the UI thread
        /// </summary>
        private Control owner;
        private object lockObj = new object();
        /// <summary>
        /// method used to compute difference between two rgb (i.e., color) values
        /// </summary>
        /// <param name="left"></param>
        /// <param name="right"></param>
        /// <returns></returns>
        private delegate double Difference(rgb left, rgb right);
        /// <summary>
        /// get/set whether tool is already running
        /// </summary>
        private bool isBusy;
        /// <summary>
        /// Event raised when texture transfer is complete
        /// </summary>
        public event RunWorkerCompletedEventHandler RunWorkerCompleted;
        /// <summary>
        /// Event raised to periodically report progres about the texture transfer job to UI
        /// </summary>
        public event ProgressChangedEventHandler ProgressChanged;
        /// <summary>
        /// starts a texture transfer job with given inputs
        /// </summary>
        /// <param name="args">inputs</param>
        public void RunWorkerAsync(TextureTransferArgs args)
        {
            lock (lockObj)
            {
                if (isBusy)
                {
                    throw new InvalidOperationException("worker is already running");
                }
                isBusy = true;
            }


            DoWork(args, (result, exception, cancelled) =>
            {                
                var handler = RunWorkerCompleted;
                if (handler != null)
                {
                    try
                    {
                        owner.BeginInvoke(handler, this, new RunWorkerCompletedEventArgs(result, exception, cancelled));
                    }
                    catch
                    {
                    }
                }
                isBusy = false;
            });
        }
     
        private void ReportProgress(int percentComplete, object userState)
        {                        
            var handler = ProgressChanged;
            if (handler != null)
            {
                try
                {
                    owner.BeginInvoke(handler, this, new ProgressChangedEventArgs(percentComplete, userState));
                }
                catch { }
            }            
        }

        private void DoWork(TextureTransferArgs args,
            Action<Bitmap, Exception, bool> callback)
        {
            BeginOuterLoop(new OuterLoopArgs(args), (result) =>
            {
                callback(result, null, false);
            });
        }

        /// <summary>
        /// one iteration of outer loop results in synthesis of an image
        /// </summary>
        /// <param name="args"></param>
        /// <param name="callback"></param>
        private void BeginOuterLoop(OuterLoopArgs args, Action<Bitmap> callback)
        {
            if (args.Completed)
            {
                callback(args.OutputBitmap);
                return;
            }

            int i = args.Iteration;
            double alpha = 0.5;
            if (args.NumberOfIterations > 1)
            {
                alpha = 0.8 * i / (args.NumberOfIterations - 1) + 0.1;     // from Alyosha's paper
            }

            BeginInnerLoop(new InnerLoopArgs(args, alpha), (bmp) =>
            {
                args.Next();
                args.OutputBitmap = bmp;
                BeginOuterLoop(args, callback);
            });
        }

        /// <summary>
        /// one iteration of inner loop results in synthesis of a block of pixels
        /// </summary>
        /// <param name="args"></param>
        /// <param name="Callback"></param>
        private void BeginInnerLoop(InnerLoopArgs args, Action<Bitmap> Callback)
        {
            if (args.Completed)
            {
                // we have finished synthesizing all the blocks of a pass
                // the callback will trigger another pass (by default we do 3 passes)
                Callback(rgb.ToBitmap(args.Output));
                return;
            }

            // get matching block from src which will be pasted at location q in img                    
            BestMatch bm = new BestMatch();
            
            var t = Task.Factory.ContinueWhenAll(FindBestMatch(args, bm), (tasks) =>
            {               
                var src = args.Source;
                var img = args.Output;
                var q = args.TopLeft;
                // the horizontal and vertical cuts will ensure that the patch from src will fit in
                // seamlessly when it is pasted onto img
                int[] Hcut = findHcut(bm.Eh);  // compute the horizontal cut
                int[] Vcut = findVcut(bm.Ev);  // compute the vertical cut
                // cut the block per the horizontal and vertical cuts and then paste it onto img
                cutandpaste(src, bm.TopLeft, img, q, Hcut, Vcut, args.BlockSize.Width,
                    args.BlockSize.Height, args.DisplayBoundaryCut);
                // calculate how much of texture transfer is complete; 1->fully completed
                double fractionComplete = CalculateFractionComplete(args.Iteration,
                    args.TotalNumberOfIterations,
                    args.TopLeft.X,
                    args.TopLeft.Y,
                    args.BlockSize.Width,
                    args.BlockSize.Height,
                    args.Output.GetLength(1),
                    args.Output.GetLength(0),
                    args.BlockReductionFactor);
                int percentComplete = (int)Math.Round(fractionComplete * 100);
                // send progress report to UI thread      
                Rectangle rect = new Rectangle(args.TopLeft, args.BlockSize);
                ReportProgress(percentComplete, new ProgressReport(fractionComplete, rgb.ToBitmap(img, rect), rect));
                // move to next block of pixels to be synthesized
                args.Next();
                // begin next iteration of inner loop
                BeginInnerLoop(args, Callback);                
            });            
        }

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
        private static Task[] FindBestMatch(InnerLoopArgs args,
            BestMatch result)
        {
            int srcWidth = args.Source.GetLength(1);
            int srcHeight = args.Source.GetLength(0);
            var blockWidth = args.BlockSize.Width;
            int blockHeight = args.BlockSize.Height;
            var overlapX = args.Overlap.Width;
            var overlapY = args.Overlap.Height;
            var isFirstIteration = args.Iteration == 0;
            var img = args.Output;
            var target = args.Target;
            var topLeft = args.TopLeft;
            var src = args.Source;
            var alpha = args.Alpha;            
            var probe = args.Probe;
            probe.Reset();
            List<Task> list = new List<Task>();

            object lockObj = new object();
            double dmin = double.MaxValue;
            
            while (!probe.IsCompleted)
            {
                // NOTES:
                // 1. The probe.Next() method is not thread-safe, but we call it on multiple threads, and yet are able to get away with it. Why?
                //    This is because although multiple threads call this method, they don't do so concurrently.                
                // 2. If you declare Point p outside while loop, you will introduce a bug. See toub's paper p. 52.
                Point p = probe.Next();

                list.Add(Task.Factory.StartNew(() =>
                {
                    int argsX = p.X, argsY = p.Y;
                    double d = 0.0, d1 = 0, d2 = 0;
                    double[,] eh = null, ev = null;
                    if (args.Overlap.Height > 0)
                    {
                        eh = new double[args.Overlap.Height, args.BlockSize.Width];
                    }
                    if (args.Overlap.Width > 0)
                    {
                        ev = new double[args.BlockSize.Height, args.Overlap.Width];
                    }

                    for (int j = 0; j < blockHeight; j++)
                        for (int i = 0; i < blockWidth; i++)
                        {
                            if (i < overlapX || j < overlapY || !isFirstIteration)
                            {   // either we are in overlapping region or this is not our first iteration                            
                                double diff = rgb.Diff(img[topLeft.Y + j, topLeft.X + i], src[argsY + j, argsX + i]);
                                if (j < overlapY && i < overlapX)
                                    eh[j, i] = ev[j, i] = diff;
                                else if (j < overlapY)
                                    eh[j, i] = diff;
                                else if (i < overlapX)
                                    ev[j, i] = diff;
                                d1 += diff;
                            }
                            d2 += args.DifferencingFunction(target[topLeft.Y + j, topLeft.X + i], src[argsY + j, argsX + i]);
                        }

                    d = alpha * d1 + (1.0 - alpha) * d2;

                    // dmin is shared by multiple threads, therefore we need to lock
                    lock (lockObj)
                    {
                        if (d < dmin)
                        {
                            dmin = d;
                            result.TopLeft = new Point(argsX, argsY);
                            result.Eh = eh;
                            result.Ev = ev;
                        }
                    }

                }));
            }

            return list.ToArray();
        } /* end fn */

        private static int[] findHcut(double[,] e)
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

        private static int[] findVcut(double[,] e)
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



        private static void cutandpaste(rgb[,] src,
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
        private static double CalculateFractionComplete(int iteration,
            int totalNumberOfIterations,
            int X, int Y,
            int blockWidth, int blockHeight,
            int imageWidth, int imageHeight,
            double blockReductionFactor)
        {
            System.Diagnostics.Debug.Assert(blockReductionFactor > 0 && blockReductionFactor < 1);
            double w = X + blockWidth;
            double area1 = w * (Y + blockHeight);
            double area2 = (imageWidth - w) * Y;
            double covered = area1 + area2;
            double imageArea = imageWidth * imageHeight;
            double fractionOfThisIterationThatHasBeenDone = covered / imageArea;
            double denominator = CalculateGeometricSum(1.0, blockReductionFactor, totalNumberOfIterations);
            double t1 = CalculateGeometricSum(Math.Pow(blockReductionFactor, totalNumberOfIterations - 1), 1.0 / blockReductionFactor, iteration);
            double t2 = fractionOfThisIterationThatHasBeenDone * Math.Pow(blockReductionFactor, totalNumberOfIterations - 1 - iteration);
            return (t1 + t2) / denominator;
        }

        private static double CalculateGeometricSum(double firstTerm, double ratio, int numberOfTerms)
        {
            if (numberOfTerms < 0) { throw new ArgumentOutOfRangeException("numberOfTerms"); }
            if (numberOfTerms == 0) { return 0; }
            return firstTerm * (1.0 - Math.Pow(ratio, numberOfTerms)) / (1.0 - ratio);
        }


        /// <summary>
        /// create texture transfer tool
        /// </summary>        
        public TextureTransferTool(Control owner)
        {
            this.owner = owner;         
        }
    }
}