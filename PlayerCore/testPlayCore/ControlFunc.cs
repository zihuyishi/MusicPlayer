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
            musicList = new PlayerCore.MusicList();
            m_currentIndex = 0;
            musicPlayer.Volume = 15;
        }
        public bool ShowDlgAndPlay() {
            OpenFileDialog fileDlg = new OpenFileDialog();
            fileDlg.InitialDirectory = "c://";
            fileDlg.Filter = "音乐文件|" + musicPlayer.SUPPORTFORMAT;
            fileDlg.RestoreDirectory = true;
            fileDlg.Multiselect = true;
            if (fileDlg.ShowDialog() == DialogResult.OK) {
                musicList.Add(fileDlg.FileNames);
                m_currentIndex = 0;
                musicPlayer.Play(musicList[m_currentIndex].FileName);
                return true;
            }
            return false;
        }
        public void Play(string fileName) {
            musicPlayer.Play(fileName);
        }
        public void PlayNext() {
            m_currentIndex++;
            m_currentIndex %= this.musicList.ListLength;
            musicPlayer.Play(musicList[m_currentIndex]);
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
        public int Volume {
            get {
                return musicPlayer.Volume;
            }
            set {
                musicPlayer.Volume = value;
            }
        }
        public bool HasMusicPlaying() {
            return musicPlayer.bMusicPlaying;
        }
        private PlayerCore.MusicPlayer musicPlayer;
        private PlayerCore.MusicList musicList;
        private int m_currentIndex;
    }
}