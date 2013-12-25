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
    public class MusicControlor//: Form
    {
        public MusicControlor() {
            this._Init();
        }
        #region _Init()初始化
        private void _Init() {
            musicPlayer = new PlayerCore.MusicPlayer();
            musicList = new PlayerCore.MusicList();
            m_currentIndex = 0;
            musicPlayer.Volume = 100;
            end_timer = new Timer();
            end_timer.Tick += new EventHandler(end_timer_Tick);
            bRandom = false;
            m_Loop = LoopMode.LoopNo;
        }
        #endregion
        public bool ShowDlgAndPlay() {
            OpenFileDialog fileDlg = new OpenFileDialog();
            fileDlg.InitialDirectory = "c://";
            fileDlg.Filter = "音乐文件|" + musicPlayer.SUPPORTFORMAT;
            fileDlg.RestoreDirectory = true;
            fileDlg.Multiselect = true;
            if (fileDlg.ShowDialog() == DialogResult.OK) {
                musicList.Add(fileDlg.FileNames);
                m_currentIndex = 0;
                _play(musicList[m_currentIndex].FileName);
                return true;
            }
            return false;
        }
        public void Play(string fileName) {
            _play(fileName);
        }
        private void _play(string fileName) {
            end_timer.Interval = 1000;
            end_timer.Start();
            musicPlayer.Play(fileName);
        }
        #region 下一个曲子索引
        /// <summary>
        ///  返回下一个歌曲索引
        /// </summary>
        /// <returns>-1表示到末尾并且不循环</returns>
        private int _nextIndex() {
            int index = m_currentIndex;
            switch (m_Loop) {
                case LoopMode.LoopOnce:
                    break;
                case LoopMode.LoopNo:
                    if (bRandom) {
                        index = new Random().Next(musicList.ListLength);
                        //随机的话，不管循环都会继续播放下一个曲子
                        break;
                    }
                    if (index == musicList.ListLength - 1) {
                        return -1;
                    }
                    index++;
                    break;
                case LoopMode.LoopAll:
                    if (bRandom) {
                        index = new Random().Next(musicList.ListLength);
                    }
                    index++;
                    break;
            }
            index %= this.musicList.ListLength;
            return index;
        }
        #endregion
        public void PlayNext() {
            int index = _nextIndex();
            if (index == -1) {
                return;
            }
            m_currentIndex = index;
            _play(musicList[m_currentIndex].FileName);
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
        private void end_timer_Tick(object sender, EventArgs e) {
            if (HasMusicPlaying()) {
                return;
            }
            end_timer.Stop();
            PlayNext();
        }
        public bool bRandom {
            set;
            get;
        }
        public void SetLoopMode(LoopMode loopMode) {
            m_Loop = loopMode;
        }
        public enum LoopMode {
            LoopOnce = 1,
            LoopAll = 2,
            LoopNo = 3,
        }
        private PlayerCore.MusicPlayer musicPlayer;
        private PlayerCore.MusicList musicList;
        private int m_currentIndex;
        private LoopMode m_Loop;
        private Timer end_timer;
    }
}