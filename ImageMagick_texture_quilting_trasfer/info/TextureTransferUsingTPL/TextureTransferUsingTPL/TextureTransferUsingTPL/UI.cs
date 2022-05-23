using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Threading;
using System.IO;

namespace TextureTransfer
{
    /// <summary>
    /// Primary UI for the texture transfer application.
    /// </summary>
    public class StartMenuForm : System.Windows.Forms.Form
    {       
        private FlowLayoutPanel buttonsPanel;
        private Button      setSrcButton;
        private Button      setTargetButton;
        private Button      saveResultButton;
        private Button      settingsButton;
        private Button      performTextureTransferButton;
        private Button exitButton;
        private FlowLayoutPanel pictureBoxPanel;
        private PictureBox  srcPictureBox;
        private PictureBox  targetPictureBox;
        private PictureBox  synthesizedPictureBox;
        private FlowLayoutPanel bottomPanel;
        private ProgressBar progressBar;
        private Label progressLabel;
        private SettingsForm settingsForm;
        private Timer timer;
        private double timeElapsed = 0;
        private int numIterations = 3;
        private int blockWidth = 27;
        private int blockHeight = 27;
        private double overlapXFraction = 1.0 / 3;
        private double overlapYFraction = 1.0 / 3;
        private double blockSizeReductionFraction = 1.0 / 3;
        private double probeFraction = 0.1;
        private bool displayBoundaryCut = false;
        private Metric metric;
        private Image srcImage;

        public StartMenuForm()
        {         
            this.AutoScroll = true;
                   
            buttonsPanel = new FlowLayoutPanel();
            buttonsPanel.FlowDirection = FlowDirection.LeftToRight;
            buttonsPanel.Dock = DockStyle.Top;
            buttonsPanel.Parent = this;
            buttonsPanel.AutoSize = true;
            
            setSrcButton = new Button();
            setSrcButton.Text = "Set Source Image...";
            setSrcButton.Click += new EventHandler(setSrcButton_Click);
            setSrcButton.Parent = buttonsPanel;
            setSrcButton.Enabled = true;
            setSrcButton.AutoSize = true;

            setTargetButton = new Button();
            setTargetButton.Text = "Set Target Image...";
            setTargetButton.Click += new EventHandler(setTargetButton_Click);
            setTargetButton.Parent = buttonsPanel;
            setTargetButton.Enabled = true;
            setTargetButton.AutoSize = true;

            settingsButton = new Button();
            settingsButton.Text = "Settings...";
            settingsButton.Click += new EventHandler(settingsButton_Click);
            settingsButton.Parent = buttonsPanel;
            settingsButton.Enabled = false;
            settingsButton.AutoSize = true;

            performTextureTransferButton = new Button();
            performTextureTransferButton.Text = "Perform Texture Transfer...";
            performTextureTransferButton.Click += new EventHandler(performTextureTransferButton_Click);
            performTextureTransferButton.Parent = buttonsPanel;
            performTextureTransferButton.Enabled = false;
            performTextureTransferButton.AutoSize = true;

            saveResultButton = new Button();
            saveResultButton.Text = "Save Result As...";
            saveResultButton.Click += new EventHandler(saveResultButton_Click);
            saveResultButton.Parent = buttonsPanel;
            saveResultButton.Enabled = false;
            saveResultButton.AutoSize = true;

            exitButton = new Button();
            exitButton.Text = "Exit...";
            exitButton.Click += new EventHandler(exitButton_Click);
            exitButton.Parent = buttonsPanel;
            exitButton.Enabled = true;
            exitButton.AutoSize = true;

            pictureBoxPanel = new FlowLayoutPanel();
            pictureBoxPanel.FlowDirection = FlowDirection.LeftToRight;
            pictureBoxPanel.Parent = this;
            pictureBoxPanel.Location = new Point(0, buttonsPanel.Bottom);
            pictureBoxPanel.AutoSize = true;

            srcPictureBox = new PictureBox();
            srcPictureBox.SizeMode = PictureBoxSizeMode.AutoSize;
            srcPictureBox.LoadCompleted += new AsyncCompletedEventHandler(srcPictureBox_LoadCompleted);
            srcPictureBox.Parent = pictureBoxPanel;
            srcPictureBox.BorderStyle = BorderStyle.FixedSingle;
            
            targetPictureBox = new PictureBox();
            targetPictureBox.SizeMode = PictureBoxSizeMode.AutoSize;
            targetPictureBox.LoadCompleted += new AsyncCompletedEventHandler(targetPictureBox_LoadCompleted);
            targetPictureBox.Parent = pictureBoxPanel;
            targetPictureBox.BorderStyle = BorderStyle.FixedSingle;

            synthesizedPictureBox = new PictureBox();
            synthesizedPictureBox.SizeMode = PictureBoxSizeMode.AutoSize;
            synthesizedPictureBox.Parent = pictureBoxPanel;
            synthesizedPictureBox.BorderStyle = BorderStyle.FixedSingle;

            bottomPanel = new FlowLayoutPanel();
            bottomPanel.FlowDirection = FlowDirection.LeftToRight;
            bottomPanel.Dock = DockStyle.Bottom;
            bottomPanel.AutoSize = true;
            bottomPanel.Parent = this;

            progressBar = new ProgressBar();
            progressBar.Parent = bottomPanel;
            progressBar.Minimum = 0;
            progressBar.Maximum = 100;
            progressBar.Value = 0;                         
            
            progressLabel = new Label();
            progressLabel.Parent = bottomPanel;            
            progressLabel.AutoSize = true;

            timer = new Timer();
            this.Text = "Sid's Texture Transfer program";        
            // this.AutoSize = true;       
        }

        void exitButton_Click(object sender, EventArgs e)
        {            
            this.Close();            
        }

        void saveResultButton_Click(object sender, EventArgs e)
        {
            if (synthesizedPictureBox.Image != null)
            {
                Image image = (Image)synthesizedPictureBox.Image.Clone();
                SaveFileDialog dlg = new SaveFileDialog();
                dlg.Filter = "Jpeg(*.JPG)|*.JPG|Image Files(*.BMP;*.JPG;*.GIF)|*.BMP;*.JPG;*.GIF|All files (*.*)|*.*";
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                        try
                        {                            
                            image.Save(dlg.FileName);
                            image.Dispose();
                        }
                        catch
                        {
                            MessageBox.Show("Unable to Save File", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }             
                }
            }
        }

        private void UpdateTextureTransferButton()
        {
            if (srcPictureBox.Image != null && targetPictureBox.Image != null)
            {
                performTextureTransferButton.Enabled = true;
                settingsButton.Enabled = true;
            }
            else
            {
                performTextureTransferButton.Enabled = false;
                settingsButton.Enabled = false;
            }
        }

        void srcPictureBox_LoadCompleted(object sender, AsyncCompletedEventArgs e)
        {
            PictureBox picbox = sender as PictureBox;

            if (e.Error != null)
            {
                picbox.Image = null;
                MessageBox.Show("Unable to load file");
            }
            else
            {
                srcImage = (Image) picbox.Image.Clone();
                settingsForm = new SettingsForm(this.srcPictureBox.Image);
                UpdateTextureTransferButton();                
            }
        }

        void targetPictureBox_LoadCompleted(object sender, AsyncCompletedEventArgs e)
        {
            PictureBox picbox = sender as PictureBox;

            if (e.Error != null)
            {
                picbox.Image = null;
                MessageBox.Show("Unable to load file");
            }
            else
                UpdateTextureTransferButton();
        }

        void setSrcButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Image Files(*.BMP;*.JPG)|*.BMP;*.JPG|All files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                srcPictureBox.LoadAsync(openFileDialog.FileName);             
            }
        }

        void setTargetButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                targetPictureBox.LoadAsync(openFileDialog.FileName);                
            }
        }

        private void settingsButton_Click(object sender, EventArgs e)
        {            
            if (settingsForm.ShowDialog() == DialogResult.OK)
            {
                this.numIterations = settingsForm.NumberOfIterations;
                this.blockWidth = settingsForm.BlockWidth;
                this.blockHeight = settingsForm.BlockHeight;
                this.overlapXFraction = settingsForm.OverlapX;
                this.overlapYFraction = settingsForm.OverlapY;
                this.blockSizeReductionFraction = settingsForm.BlockSizeReduction;
                this.probeFraction = settingsForm.AmountToProbe;
                this.displayBoundaryCut = settingsForm.DisplayBoundaryCut;
                this.metric = settingsForm.DifferenceMetric;
                UpdateTextureTransferButton();
            }
        }

        private void DisableButtons()
        {
            // do not disable save result as button
            // you can save as the texture transfer is progressing
            setSrcButton.Enabled = false;
            setTargetButton.Enabled = false;
            settingsButton.Enabled = false;
            performTextureTransferButton.Enabled = false;
            exitButton.Enabled = false;
        }        
        
        private void performTextureTransferButton_Click(object sender, EventArgs e)
        {
            var x = this.targetPictureBox.Image;
            this.synthesizedPictureBox.Image = new Bitmap(x.Width, x.Height, x.PixelFormat);            
            DisableButtons();
            saveResultButton.Enabled = true;   
            if (srcPictureBox.Image == null || targetPictureBox.Image == null)
            {
                MessageBox.Show("Unable to perform texture transfer", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            TextureTransferArgs args = new TextureTransferArgs(new Bitmap(srcPictureBox.Image),
                new Bitmap(targetPictureBox.Image),
                this.blockWidth, this.blockHeight, this.numIterations, this.overlapXFraction,
                this.overlapYFraction, this.blockSizeReductionFraction, this.probeFraction,
                this.displayBoundaryCut, this.metric);
            TextureTransferTool tt = new TextureTransferTool(this);
            tt.RunWorkerCompleted += new RunWorkerCompletedEventHandler(TextureTransferCompleted);
            tt.ProgressChanged += new ProgressChangedEventHandler(SynthesizedImageUpdated);
            progressBar.Value = 0;
            tt.RunWorkerAsync(args);
            timeElapsed = 0;
            timer.Start();
        }

        void SynthesizedImageUpdated(object sender, ProgressChangedEventArgs e)
        {
            timer.Stop();
            ProgressReport report = (ProgressReport)e.UserState;            
            double sec = timer.Seconds;
            double min = sec / 60;
            timeElapsed += min;
            double estimatedTotalTime = timeElapsed / report.FractionCompleted;
            double timeRemaining = estimatedTotalTime - timeElapsed;
            int minutesRemaining = (int)Math.Round(timeRemaining);
            int secondsRemaining = (int)Math.Round(timeRemaining * 60);
            progressBar.Value = Math.Min(Math.Max(0,e.ProgressPercentage),100);
            /*
            if (minutesRemaining > 0)
                progressLabel.Text = progressBar.Value + "% completed; Estimated Time Left = " + minutesRemaining + " min";
            else
                progressLabel.Text = progressBar.Value + "% completed; Estimated Time Left = " + secondsRemaining + " sec";            
             */
            progressLabel.Text = progressBar.Value + "% completed" + " Time taken = " + timeElapsed.ToString("0.00") + " minutes";
            /* TODO: update and invalidate only the region that has changed, instead of the whole image */
            using (var g = Graphics.FromImage(this.synthesizedPictureBox.Image))
            {
                g.DrawImageUnscaled(report.Image, report.Rectangle);               
            }
            this.synthesizedPictureBox.Invalidate(report.Rectangle);
            timer.Start();
        }

        void TextureTransferCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            this.setSrcButton.Enabled = true;
            this.setTargetButton.Enabled = true;
            this.settingsButton.Enabled = true;
            this.saveResultButton.Enabled = true;
            this.performTextureTransferButton.Enabled = true;
            this.exitButton.Enabled = true;
        }
        
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.Run(new StartMenuForm());
        }
    }
}