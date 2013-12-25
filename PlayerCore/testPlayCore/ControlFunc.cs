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
    public class MusicControlor
    {
        public MusicControlor() {
            this._Init();
        }
        private void _Init() {
            musicPlayer = new PlayerCore.MusicPlayer();
        }
        public bool ShowDlgAndPlay() {
            OpenFileDialog fileDlg = new OpenFileDialog();
            fileDlg.InitialDirectory = "c://";
            fileDlg.Filter = "音乐文件|" + musicPlayer.SUPPORTFORMAT;
            fileDlg.RestoreDirectory = true;

            if (fileDlg.ShowDialog() == DialogResult.OK) {
                musicPlayer.Volume = 15;
                musicPlayer.Play(fileDlg.FileName);
                return true;
            }
            return false;
        }
        public void Play(string fileName) {
            musicPlayer.Play(fileName);
        }
        public void Pause() {
            musicPlayer.Pause();
        }
        public void Resume() {
            musicPlayer.Resume();
        }
        public void Stop() {
            musicPlayer.Stop();
        }
        public bool HasMusicPlaying() {
            return musicPlayer.bMusicPlaying;
        }
        private PlayerCore.MusicPlayer musicPlayer;
    }
}