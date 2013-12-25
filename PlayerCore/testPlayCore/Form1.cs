using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace APlayer
{
    public partial class Form1 : Form
    {
        public Form1() {
            InitializeComponent();
            musicPlayer = new MusicControlor();
        }

        private void Form1_Load(object sender, EventArgs e) {
            this.hScrollBar1.Value = 100;
        }

        private void button1_Click(object sender, EventArgs e) {
            musicPlayer.ShowDlgAndPlay();
        }
        private MusicControlor musicPlayer;
        private void button2_Click(object sender, EventArgs e) {
            musicPlayer.Pause();
        }

        private void button3_Click(object sender, EventArgs e) {
            musicPlayer.Resume();
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e) {
            this.musicPlayer.Volume = ((HScrollBar)sender).Value;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e) {
            bool ret = ((CheckBox)sender).Checked;
            this.musicPlayer.bRandom = ret;
        }

        private void LoopRadioButton_CheckedChanged(object sender, EventArgs e) {
            string text = ((RadioButton)sender).Text;
            if (text == "LoopNo") {
                this.musicPlayer.SetLoopMode(MusicControlor.LoopMode.LoopNo);
            }
            if (text == "LoopAll") {
                this.musicPlayer.SetLoopMode(MusicControlor.LoopMode.LoopAll);
            }
            if (text == "LoopOnce") {
                this.musicPlayer.SetLoopMode(MusicControlor.LoopMode.LoopOnce);
            }

        }
    }
}
