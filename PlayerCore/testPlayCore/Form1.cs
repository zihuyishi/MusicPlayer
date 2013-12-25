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
        }

        private void button1_Click(object sender, EventArgs e) {
            if (musicPlayer.ShowDlgAndPlay()) {
                this.timer1.Interval = 1000;
                this.timer1.Start();
            }
        }
        private MusicControlor musicPlayer;
        private void button2_Click(object sender, EventArgs e) {
            musicPlayer.Pause();
        }

        private void button3_Click(object sender, EventArgs e) {
            musicPlayer.Resume();
        }

        private void timer1_Tick(object sender, EventArgs e) {
            if (musicPlayer.HasMusicPlaying()) {
                return;
            }
            this.timer1.Stop();
            this.button1_Click(sender, e);
        }
    }
}
