using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Imaging;

namespace TextureTransfer
{
    class SettingsForm : Form
    {
        private Label numIterationsLabel;
        private Label blockWidthLabel;
        private Label blockWidthValueLabel;
        private Label blockHeightLabel;
        private Label blockHeightValueLabel;
        private Label overlapXLabel;
        private Label overlapXValueLabel;
        private Label overlapYLabel;
        private Label overlapYValueLabel;
        private Label blockSizeReductionLabel;
        private Label blockSizeReductionValueLabel;
        private Label probeLabel;        
        private Label probeValueLabel;
        private NumericUpDown numIterationsUpDown;        
        private TrackBar blockWidthTrackBar;
        private TrackBar blockHeightTrackBar;
        private TrackBar overlapXTrackBar;
        private TrackBar overlapYTrackBar;
        private TrackBar blockSizeReductionTrackBar;
        private TrackBar probeTrackBar;
        private CheckBox displayBoundaryCut;
        private ComboBox metricComboBox;    // combo box to select difference metric - SSD or intensity squared
        private Label metricLabel;  
        private PictureBox pictureBox;
        private Image pictureBoxImage;
        private Button okButton;
        private Button cancelButton;
        private TableLayoutPanel tablePanel;
        private float defaultFontWidthPixels = 6F;
        private float defaultFontHeightPixels = 13F;
        private string[] comboBoxStrings = new string[]{"Sum of squared R,G,B differences",
            "Intensity difference squared"};
        private int numIterations = 3;
        private int blockWidth = 27;
        private int blockHeight = 27;
        private double overlapXFraction = 1.0/3;
        private double overlapYFraction = 1.0/3;
        private double blockSizeReductionFraction = 1.0/3;
        private double probeFraction = 0.1;
        private Metric differenceMetric;

        public int NumberOfIterations { get { return numIterations; } }

        public int BlockWidth { get { return blockWidth; } }

        public int BlockHeight { get { return blockHeight; } }

        public double OverlapX { get { return overlapXFraction; } }

        public double OverlapY { get { return overlapYFraction; } }

        public double BlockSizeReduction { get { return blockSizeReductionFraction; } }

        public double AmountToProbe { get { return probeFraction; } }

        public bool DisplayBoundaryCut { get { return displayBoundaryCut.Checked; } }

        public Metric DifferenceMetric { get { return differenceMetric; } }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Reliability", "CA2000:Dispose objects before losing scope")]
        private void UpdatePictureBoxImage()
        {
            pictureBox.Image = (Image) pictureBoxImage.Clone(); // need to clone as we want to preserve the original
            try
            {
                Graphics g = Graphics.FromImage(pictureBox.Image);
                Size sz = pictureBoxImage.Size;
                int centerX = sz.Width / 2;
                int centerY = sz.Height / 2;
                Rectangle rect = new Rectangle(centerX - blockWidth / 2, centerY - blockHeight / 2,
                    blockWidth, blockHeight);
                Pen pen = new Pen(Brushes.Red, g.DpiX / 50);
                g.DrawRectangle(pen, rect);
                g.Dispose();
                pen.Dispose();
            }
            catch // do nothing
            { /* an exception can occur for example if source has an indexed pixel format*/}
        }
        
        /// <summary>
        /// Used to create a new settings form that will allow user to adjust texture transfer parameters
        /// </summary>        
        public SettingsForm(Image image)
        {
            Text = "Texture Transfer Settings";
            FormBorderStyle = FormBorderStyle.FixedDialog;
            ControlBox = false;
            MinimizeBox = false;
            MaximizeBox = false;
            ShowInTaskbar = false;
            AutoSize = true;
            StartPosition = FormStartPosition.Manual;
            if (ActiveForm != null)
                Location = ActiveForm.Location + SystemInformation.CaptionButtonSize + SystemInformation.FrameBorderSize;        
            numIterationsLabel = new Label();
            numIterationsLabel.AutoSize = true;
            numIterationsLabel.Parent = this;
            numIterationsLabel.Text = "Number of iterations:";
            numIterationsLabel.Padding = new Padding(5);

            numIterationsUpDown = new NumericUpDown();
            numIterationsUpDown.Location = new System.Drawing.Point(numIterationsLabel.Right, 0);
            numIterationsUpDown.AutoSize = true;
            numIterationsUpDown.Parent = this;
            numIterationsUpDown.Value = 3;
            numIterationsUpDown.Minimum = 1;
            numIterationsUpDown.Maximum = 5;
            numIterationsUpDown.Increment = 1;
            numIterationsUpDown.ValueChanged += new EventHandler(numIterationsUpDown_ValueChanged);

            tablePanel = new TableLayoutPanel();
            tablePanel.Parent = this;
            tablePanel.AutoSize = true;
            tablePanel.ColumnCount = 3;
            tablePanel.Location = new System.Drawing.Point(0, numIterationsLabel.Bottom);

            blockWidthLabel = new Label();
            blockWidthLabel.AutoSize = true;
            blockWidthLabel.Parent = tablePanel;
            blockWidthLabel.Text = "Block Width (pixels):";

            blockWidthTrackBar = new TrackBar();
            blockWidthTrackBar.AutoSize = true;
            blockWidthTrackBar.Parent = tablePanel;
            blockWidthTrackBar.Minimum = 3;
            blockWidthTrackBar.Maximum = 50;            
            blockWidthTrackBar.Value = 27;
            blockWidthTrackBar.TickFrequency = 5;
            blockWidthTrackBar.ValueChanged += new EventHandler(blockWidthTrackBar_ValueChanged);

            blockWidthValueLabel = new Label();
            blockWidthValueLabel.AutoSize = true;
            blockWidthValueLabel.Parent = tablePanel;
            blockWidthValueLabel.Text = blockWidthTrackBar.Value.ToString();

            blockHeightLabel = new Label();
            blockHeightLabel.AutoSize = true;
            blockHeightLabel.Parent = tablePanel;
            blockHeightLabel.Text = "Block Height (pixels):";

            blockHeightTrackBar = new TrackBar();
            blockHeightTrackBar.AutoSize = true;
            blockHeightTrackBar.Parent = tablePanel;
            blockHeightTrackBar.Minimum = 3;
            blockHeightTrackBar.Maximum = 50;
            blockHeightTrackBar.Value = 27;
            blockHeightTrackBar.TickFrequency = 5;
            blockHeightTrackBar.ValueChanged += new EventHandler(blockHeightTrackBar_ValueChanged);

            blockHeightValueLabel = new Label();
            blockHeightValueLabel.AutoSize = true;
            blockHeightValueLabel.Parent = tablePanel;
            blockHeightValueLabel.Text = blockHeightTrackBar.Value.ToString();

            overlapXLabel = new Label();
            overlapXLabel.AutoSize = true;
            overlapXLabel.Parent = tablePanel;
            overlapXLabel.Text = "Overlap in X (%):";

            overlapXTrackBar = new TrackBar();
            overlapXTrackBar.AutoSize = true;
            overlapXTrackBar.Parent = tablePanel;
            overlapXTrackBar.Minimum = 1;
            overlapXTrackBar.Maximum = 50;
            overlapXTrackBar.Value = 33;
            overlapXTrackBar.TickFrequency = 5;
            overlapXTrackBar.ValueChanged += new EventHandler(overlapXTrackBar_ValueChanged);

            overlapXValueLabel = new Label();
            overlapXValueLabel.AutoSize = true;
            overlapXValueLabel.Parent = tablePanel;
            overlapXValueLabel.Text = overlapXTrackBar.Value.ToString();

            overlapYLabel = new Label();
            overlapYLabel.AutoSize = true;
            overlapYLabel.Parent = tablePanel;
            overlapYLabel.Text = "Overlap in Y (%):";

            overlapYTrackBar = new TrackBar();
            overlapYTrackBar.AutoSize = true;
            overlapYTrackBar.Parent = tablePanel;
            overlapYTrackBar.Minimum = 1;
            overlapYTrackBar.Maximum = 50;
            overlapYTrackBar.Value = 33;
            overlapYTrackBar.TickFrequency = 5;
            overlapYTrackBar.ValueChanged += new EventHandler(overlapYTrackBar_ValueChanged);

            overlapYValueLabel = new Label();
            overlapYValueLabel.AutoSize = true;
            overlapYValueLabel.Parent = tablePanel;
            overlapYValueLabel.Text = overlapYTrackBar.Value.ToString();

            blockSizeReductionLabel = new Label();
            blockSizeReductionLabel.AutoSize = true;
            blockSizeReductionLabel.Parent = tablePanel;
            blockSizeReductionLabel.Text = "Block Size Reduction (%):";

            blockSizeReductionTrackBar = new TrackBar();
            blockSizeReductionTrackBar.AutoSize = true;
            blockSizeReductionTrackBar.Parent = tablePanel;
            blockSizeReductionTrackBar.Minimum = 1;
            blockSizeReductionTrackBar.Maximum = 50;
            blockSizeReductionTrackBar.Value = 33;
            blockSizeReductionTrackBar.TickFrequency = 5;
            blockSizeReductionTrackBar.ValueChanged += new EventHandler(blockSizeReductionTrackBar_ValueChanged);

            blockSizeReductionValueLabel = new Label();
            blockSizeReductionValueLabel.AutoSize = true;
            blockSizeReductionValueLabel.Parent = tablePanel;
            blockSizeReductionValueLabel.Text = blockSizeReductionTrackBar.Value.ToString();

            probeLabel = new Label();
            probeLabel.AutoSize = true;
            probeLabel.Parent = tablePanel;
            probeLabel.Text = "Fraction of source image to probe (%):";            

            probeTrackBar = new TrackBar();
            probeTrackBar.AutoSize = true;
            probeTrackBar.Parent = tablePanel;
            probeTrackBar.Minimum = 10;
            probeTrackBar.Maximum = 100;
            probeTrackBar.Value = 10;
            probeTrackBar.TickFrequency = 10;
            probeTrackBar.ValueChanged += new EventHandler(probeTrackBar_ValueChanged);

            probeValueLabel = new Label();
            probeValueLabel.AutoSize = true;
            probeValueLabel.Parent = tablePanel;
            probeValueLabel.Text = probeTrackBar.Value.ToString();

            displayBoundaryCut = new CheckBox();
            displayBoundaryCut.AutoSize = true;
            displayBoundaryCut.Parent = this;
            displayBoundaryCut.Checked = false;
            displayBoundaryCut.Text = "Display boundary cut";
            displayBoundaryCut.Location = new Point(0, tablePanel.Bottom);
            displayBoundaryCut.Padding = new Padding(5);

            metricLabel = new Label();
            metricLabel.AutoSize = true;
            metricLabel.Parent = this;
            metricLabel.Text = "Difference metric (source, target)";
            metricLabel.Location = new Point(0, displayBoundaryCut.Bottom);

            metricComboBox = new ComboBox();
            metricComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            metricComboBox.Parent = this;            
            metricComboBox.Location = new Point(metricLabel.Right, metricLabel.Location.Y);
            metricComboBox.Items.Add("Sum of Squared R,G,B differences");
            metricComboBox.Items.Add("Intensity difference squared");
            metricComboBox.Text = "Difference metric";
            metricComboBox.SelectedIndexChanged += new EventHandler(metricComboBox_SelectedIndexChanged);
            metricComboBox.SelectedIndex = 0;
            int maxLength = comboBoxStrings[0].Length;
            for (int ctr = 1; ctr < comboBoxStrings.Length; ctr++)
                maxLength = Math.Max(maxLength, comboBoxStrings[ctr].Length);
            metricComboBox.Size = new Size((int)(maxLength * defaultFontWidthPixels),
                (int) defaultFontHeightPixels);   

            differenceMetric = Metric.SSD;

            pictureBox = new PictureBox();
            pictureBox.Location = new Point(metricComboBox.Right, 0);
            pictureBoxImage = image;            
            pictureBox.Size = image.Size;
            pictureBox.BorderStyle = BorderStyle.FixedSingle;
            pictureBox.Parent = this;
            UpdatePictureBoxImage();

            okButton = new Button();
            okButton.Text = "OK";            
            okButton.Location = new System.Drawing.Point((tablePanel.Left + tablePanel.Right)/3, metricComboBox.Bottom);
            okButton.Parent = this;
            okButton.DialogResult = DialogResult.OK;
            okButton.Margin = new Padding(Font.Height);

            cancelButton = new Button();
            cancelButton.Text = "Cancel";
            cancelButton.Location = new System.Drawing.Point(okButton.Right, metricComboBox.Bottom);
            cancelButton.Parent = this;
            cancelButton.DialogResult = DialogResult.Cancel;
            cancelButton.Margin = new Padding(Font.Height);

            AcceptButton = okButton;
            CancelButton = cancelButton;

            this.AutoScaleDimensions = new SizeF(defaultFontWidthPixels, defaultFontHeightPixels);
            this.AutoScaleMode = AutoScaleMode.Font;
        }

        void metricComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox box = sender as ComboBox;
            if (box.SelectedIndex == 0)
                differenceMetric = Metric.SSD;
            else
                differenceMetric = Metric.IntensityDiffSquared;
        }
        
        void numIterationsUpDown_ValueChanged(object sender, EventArgs e)
        {
            numIterations = (int) ((NumericUpDown)sender).Value;
        }

        void blockWidthTrackBar_ValueChanged(object sender, EventArgs e)
        {
            blockWidth = ((TrackBar)sender).Value;
            blockWidthValueLabel.Text = blockWidth.ToString();
            UpdatePictureBoxImage();
            pictureBox.Invalidate();
        }

        void blockHeightTrackBar_ValueChanged(object sender, EventArgs e)
        {
            blockHeight = ((TrackBar)sender).Value;
            blockHeightValueLabel.Text = blockHeight.ToString();
            UpdatePictureBoxImage();
            pictureBox.Invalidate();
        }

        void overlapXTrackBar_ValueChanged(object sender, EventArgs e)
        {
            overlapXFraction = (double)((TrackBar)sender).Value/100;
            overlapXValueLabel.Text = ((TrackBar)sender).Value.ToString();
        }

        void overlapYTrackBar_ValueChanged(object sender, EventArgs e)
        {
            overlapYFraction = (double)((TrackBar)sender).Value / 100;
            overlapYValueLabel.Text = ((TrackBar)sender).Value.ToString();
        }

        void blockSizeReductionTrackBar_ValueChanged(object sender, EventArgs e)
        {
            blockSizeReductionFraction = (double)((TrackBar)sender).Value / 100;
            blockSizeReductionValueLabel.Text = ((TrackBar)sender).Value.ToString();
        }

        void probeTrackBar_ValueChanged(object sender, EventArgs e)
        {
            TrackBar bar = sender as TrackBar;
            probeFraction = (double) bar.Value / 100;
            probeValueLabel.Text = bar.Value.ToString();
        }
    }
}
