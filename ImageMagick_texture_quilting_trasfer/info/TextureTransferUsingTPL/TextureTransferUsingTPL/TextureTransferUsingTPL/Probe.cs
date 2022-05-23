using System;
using System.Drawing;
namespace TextureTransfer
{
    /// <summary>
    /// Summary description for Probe.
    /// </summary>
    public abstract class Probe
    {
        public abstract bool IsCompleted { get; }

        public abstract Point Next();

        public abstract void Reset();        
    }
}
