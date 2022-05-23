using System;
using System.Runtime.InteropServices;

namespace TextureTransfer
{
    public class Timer
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Portability", "CA1901:PInvokeDeclarationsShouldBePortable", MessageId = "return"), System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1060:MovePInvokesToNativeMethodsClass"), DllImport("kernel32.dll")]
        extern static short QueryPerformanceCounter(ref long x);

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Portability", "CA1901:PInvokeDeclarationsShouldBePortable", MessageId = "return"), System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Design", "CA1060:MovePInvokesToNativeMethodsClass"), DllImport("kernel32.dll")]
        extern static short QueryPerformanceFrequency(ref long x);



        private long m_lStart = 0, m_lStop = 0, m_lFreq = 0;
        private double m_fTicksPerMicrosecond = 0;

        /// <summary>
        /// Make a new timer object
        /// </summary>
        public Timer()
        {
            QueryPerformanceFrequency(ref m_lFreq);
            m_fTicksPerMicrosecond = m_lFreq / 1e6;
        }

        /// <summary>
        /// Start the timer
        /// </summary>
        public void Start()
        {
            QueryPerformanceCounter(ref m_lStart);
        }

        /// <summary>
        /// Stop the timer
        /// </summary>
        public void Stop()
        {
            QueryPerformanceCounter(ref m_lStop);
        }

        /// <summary>
        /// Report the elapsed time in microseconds
        /// </summary>
        public double Microseconds
        {
            get { return (m_lStop - m_lStart) / m_fTicksPerMicrosecond; }
        }

        /// <summary>
        /// Report the elapsed time in milliseconds
        /// </summary>
        public double Milliseconds
        {
            get { return Microseconds / 1000; }
        }

        /// <summary>
        /// Report the elapsed time in seconds
        /// </summary>
        public double Seconds
        {
            get { return Milliseconds / 1000; }
        }
    }
}
