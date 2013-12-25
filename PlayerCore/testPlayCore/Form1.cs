using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace testPlayCore
{
    public partial class Form1 : Form
    {
        public Form1() {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e) {
            musicPlayer = new PlayerCore.MusicPlayer();
        }

        private void button1_Click(object sender, EventArgs e) {
            OpenFileDialog fileDlg = new OpenFileDialog();
            fileDlg.InitialDirectory = @"c:\";
            fileDlg.Filter = "mp3音乐|*.mp3";
            fileDlg.RestoreDirectory = true;
            /*
            if (fileDlg.ShowDialog() == DialogResult.OK) {
                musicPlayer.Play(fileDlg.FileName);
            }
            */
            musicPlayer.Play(@"d:\1.mp3");
        }
        private PlayerCore.MusicPlayer musicPlayer;

        private void button2_Click(object sender, EventArgs e) {
            musicPlayer.Pause();
        }

        private void button3_Click(object sender, EventArgs e) {
            musicPlayer.Resume();
        }
    }
}
