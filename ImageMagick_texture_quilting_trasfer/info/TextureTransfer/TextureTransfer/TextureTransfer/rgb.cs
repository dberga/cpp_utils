using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace TextureTransfer
{
    /// <summary>
    /// Summary description for rgb.
    /// </summary>
    public struct rgb
    {
        private byte r, g, b;

        private static int GetBytesPerPixelFromPixelFormat(PixelFormat fmt)
        {
            if (fmt == PixelFormat.Format24bppRgb)
                return 3;
            else if (fmt == PixelFormat.Format32bppArgb
                || fmt == PixelFormat.Format32bppPArgb
                || fmt == PixelFormat.Format32bppRgb)
                return 4;
            else if (fmt == PixelFormat.Format48bppRgb)
                return 6;
            else if (fmt == PixelFormat.Format64bppArgb
                || fmt == PixelFormat.Format64bppPArgb)
                return 8;
            else if (fmt == PixelFormat.Format8bppIndexed)
                return 1;
            else
                throw new Exception("GetBytesPerPixelFromPixelFormat: invalid argument exception");
        }

        public byte R { get { return r; } set { r = value; } }

        public byte G { get { return g; } set { g = value; } }

        public byte B { get { return b; } set { b = value; } }

        
        public rgb(byte R, byte G, byte B)
        {
            r = R; g = G; b = B;
        }


        public static bool operator ==(rgb left, rgb right)
        {
            return (left.R == right.R && left.G == right.G && left.B == right.B);
        }

        public static bool operator !=(rgb left, rgb right)
        {
            return !(left.R == right.R && left.G == right.G && left.B == right.B);
        }

        public override bool Equals(object obj)
        {
            rgb arg = (rgb)obj;
            return (this.R == arg.R && this.G == arg.G && this.B == arg.B);
        }        

        public static double L2Norm(rgb X)
        {
            return Math.Sqrt(X.R * X.R + X.G * X.G + X.B * X.B);
        }

        public static double Diff(rgb A, rgb B)
        {
            double r = A.R - B.R;
            double g = A.G - B.G;
            double b = A.B - B.B;
            return r * r + b * b + g * g;
        }

        public static double Diffi(rgb left, rgb right)
        {
            double d1 = L2Norm(left);
            double d2 = L2Norm(right);
            double d3 = d1 - d2;
            return d3 * d3;
        }

        public static rgb Average(rgb left, rgb right)
        {
            byte r = (byte) ((left.R + right.R) / 2);
            byte g = (byte) ((left.G + right.G) / 2);
            byte b = (byte) ((left.B + right.B) / 2);
            return new rgb(r, g, b);
        }

        public static rgb[,] FromFilename(string filename)
        {
            Bitmap bmp = (Bitmap)Bitmap.FromFile(filename);
            return rgb.FromBitmap(bmp);
        }

        public static void Save(rgb[,] A, string filename)
        {
            Bitmap bmp = rgb.ToBitmap(A);
            bmp.Save(filename);
        }

        public static unsafe rgb[,] FromBitmap(Bitmap bmp)
        {
            int width = bmp.Width;
            int height = bmp.Height;
            PixelFormat fmt = bmp.PixelFormat;
            Rectangle rect = new Rectangle(0, 0, width, height);
            int bpp = GetBytesPerPixelFromPixelFormat(fmt);
            if (bpp < 3)
                throw new Exception("FromBitmap: invalid argument exception");
            BitmapData bmpData = bmp.LockBits(rect, ImageLockMode.ReadOnly, fmt);
            byte* ptr = (byte*)(void*)bmpData.Scan0;
            int stride = bmpData.Stride;
            int offset = stride - width * bpp;
            int offset2 = bpp - 3;

            rgb[,] A = new rgb[height, width];
            for (int y = 0; y < height; y++, ptr += offset)
                for (int x = 0; x < width; x++)
                {
                    A[y, x].b = *ptr; ptr++;
                    A[y, x].g = *ptr; ptr++;
                    A[y, x].r = *ptr; ptr++;
                    ptr += offset2;
                }

            bmp.UnlockBits(bmpData);

            return A;
        }

        public static unsafe Bitmap ToBitmap(rgb[,] A)
        {
            int width = A.GetLength(1);
            int height = A.GetLength(0);
            int bpp = 3;
            Rectangle rect = new Rectangle(0, 0, width, height);
            PixelFormat fmt = PixelFormat.Format24bppRgb;
            Bitmap bmp = new Bitmap(width, height, fmt);
            BitmapData bmpData = bmp.LockBits(rect, ImageLockMode.WriteOnly, fmt);
            byte* ptr = (byte*)(void*)bmpData.Scan0;
            int stride = bmpData.Stride;
            int offset = stride - bpp * width;
            for (int y = 0; y < height; y++, ptr += offset)
                for (int x = 0; x < width; x++)
                {
                    *ptr = A[y, x].b; ptr++;
                    *ptr = A[y, x].g; ptr++;
                    *ptr = A[y, x].r; ptr++;
                }

            bmp.UnlockBits(bmpData);

            return bmp;
        }
    }
}
