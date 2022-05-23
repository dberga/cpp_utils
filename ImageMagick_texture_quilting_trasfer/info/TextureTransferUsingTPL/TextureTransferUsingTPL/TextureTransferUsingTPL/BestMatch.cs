using System.Drawing;

namespace TextureTransfer
{
    public partial class TextureTransferTool
    {
        private class BestMatch
        {
            /// <summary>
            /// top-left corner of bestmatch
            /// </summary>
            public Point TopLeft { get; set; }
            /// <summary>
            /// error matrix for H cut
            /// </summary>
            public double[,] Eh { get; set; }
            /// <summary>
            /// error matrix for V cut
            /// </summary>
            public double[,] Ev { get; set; }
        }
    }    
}
