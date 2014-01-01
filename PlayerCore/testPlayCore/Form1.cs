using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using PlayController;

namespace APlayer
{
    public partial class Form1 : Form
    {
        public Form1() {
            InitializeComponent();
            _musicPlayer = new MusicController();
        }

        protected override void OnPaint(PaintEventArgs e) {
            base.OnPaint(e);
        }
        private void Form1_Load(object sender, EventArgs e) {
            hScrollBar1.Value = 100;
        }

        private void button1_Click(object sender, EventArgs e) {
            _musicPlayer.ShowDlgAndPlay();
        }
        
        private void button2_Click(object sender, EventArgs e) {
            _musicPlayer.Pause();
        }

        private void button3_Click(object sender, EventArgs e) {
            _musicPlayer.Resume();
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e) {
            _musicPlayer.Volume = ((HScrollBar)sender).Value;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e) {
            bool ret = ((CheckBox)sender).Checked;
            _musicPlayer.RandomOrder = ret;
        }

        private void LoopRadioButton_CheckedChanged(object sender, EventArgs e) {
            string text = ((RadioButton)sender).Text;
            if (text == "LoopNo") {
                _musicPlayer.SetLoopMode(MusicController.LoopMode.LoopNo);
            }
            if (text == "LoopAll") {
                _musicPlayer.SetLoopMode(MusicController.LoopMode.LoopAll);
            }
            if (text == "LoopOnce") {
                _musicPlayer.SetLoopMode(MusicController.LoopMode.LoopOnce);
            }

        }

        private void button4_Click(object sender, EventArgs e) {
            _musicPlayer.PlayNext(true);
        }

        private void button5_Click(object sender, EventArgs e) {
            _musicPlayer.ShowDlgAndAdd();
        }

        private void button6_Click(object sender, EventArgs e) {
            _musicPlayer.SaveList("musiclist.xml");
        }

        private void button7_Click(object sender, EventArgs e) {
            _musicPlayer.LoadList("musiclist.xml");
        }
        private readonly MusicController _musicPlayer;
    }
}
