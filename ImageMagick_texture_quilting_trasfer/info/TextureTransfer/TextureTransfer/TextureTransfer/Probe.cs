using System;

namespace TextureTransfer
{
    /// <summary>
    /// Summary description for Probe.
    /// </summary>
    public abstract class Probe
    {
        public abstract void Next(out int X, out int Y, out bool probeCompleted);

        public abstract void Reset();

        public abstract void Reset(int Width, int Height);
    }
}
