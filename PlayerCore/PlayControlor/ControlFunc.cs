using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using PlayerCore;

namespace PlayControlor
{
    public class MusicControlor//: Form
    {
        public MusicControlor() {
            _Init();
        }
        #region _Init()初始化
        private void _Init() {
            _musicPlayer = new PlayerCore.MusicPlayer();
            _musicList = new PlayerCore.MusicList();
            _currentIndex = 0;
            _musicPlayer.Volume = 100;
            _endtimer = new Timer();
            _endtimer.Tick += new EventHandler(end_timer_Tick);
            RandomOrder = false;
            _loopMode = LoopMode.LoopNo;
        }
        #endregion
        /// <summary>
        /// 显示文件浏览窗口并返回所选文件
        /// </summary>
        /// <param name="files">返回所选文件，外部不需要初始化</param>
        /// <returns>是否有返回文件</returns>
        private static bool _showDlgAndGetFile(out string[] files) {
            OpenFileDialog fileDlg = new OpenFileDialog {
                InitialDirectory = "c://",
                Filter = "音乐文件|" + MusicFile.SupportFormat,
                RestoreDirectory = true,
                Multiselect = true
            };
            if (fileDlg.ShowDialog() == DialogResult.OK) {
                files = fileDlg.FileNames;
                return true;
            }
            files = null;
            return false;
        }
        public int ShowDlgAndAdd() {
            string[] files;
            bool bRet = _showDlgAndGetFile(out files);
            if (!bRet) return -1;
            _musicList.Add(files);
            return files.Length;
        }
        public bool ShowDlgAndPlay() {
            string[] files;
            bool bRet = _showDlgAndGetFile(out files);
            if (!bRet) return false;
            _musicList.RemoveAll();
            _musicList.Add(files);
            _play(_musicList[_currentIndex].FilePath);
            return true;
        }
        public void Play(string fileName) {
            _play(fileName);
        }
        private void _play(string fileName) {
            _endtimer.Interval = 1000;
            _endtimer.Start();
            _musicPlayer.Play(fileName);
        }
        #region 下一个曲子索引_nextIndex
        /// <summary>
        /// 下个曲子
        /// </summary>
        /// <param name="force">主动播放下一首歌</param>
        /// <returns>-1表示没有获取正确曲子</returns>
        private int _nextIndex(bool force = false) {
            int index = _currentIndex;
            //主动播放下一首歌
            if (force) {
                if (RandomOrder) {
                    index = new Random().Next(_musicList.ListLength);
                }
                index++;
                index %= _musicList.ListLength;
                return index;
            }
            //被动播放下一首歌
            switch (_loopMode) {
                case LoopMode.LoopOnce:
                    break;
                case LoopMode.LoopNo:
                    if (RandomOrder) {
                        index = new Random().Next(_musicList.ListLength);
                        //随机的话，不管循环都会继续播放下一个曲子
                        break;
                    }
                    if (index == _musicList.ListLength - 1) {
                        return -1;
                    }
                    index++;
                    break;
                case LoopMode.LoopAll:
                    if (RandomOrder) {
                        index = new Random().Next(_musicList.ListLength);
                    }
                    index++;
                    break;
            }
            index %= _musicList.ListLength;
            return index;
        }
        #endregion
        public void PlayNext(bool force = false) {
            int index = _nextIndex(force);
            if (index == -1) {
                return;
            }
            _currentIndex = index;
            _play(_musicList[_currentIndex].FilePath);
        }
        public void Pause() {
            _musicPlayer.Pause();
        }
        public void Resume() {
            _musicPlayer.Resume();
        }
        public void Stop() {
            _musicPlayer.Stop();
        }

        public void SaveList() {
            _musicList.SaveListToFile("musiclist.xml");
        }

        public void LoadList() {
            _musicList.LoadListFromFile("musiclist.xml");
        }
        public int Volume {
            get {
                return _musicPlayer.Volume;
            }
            set {
                _musicPlayer.Volume = value;
            }
        }
        public bool HasMusicPlaying() {
            return _musicPlayer.MusicPlaying;
        }
        private void end_timer_Tick(object sender, EventArgs e) {
            if (HasMusicPlaying()) {
                return;
            }
            _endtimer.Stop();
            PlayNext();
        }
        public bool RandomOrder {
            set;
            get;
        }
        public void SetLoopMode(LoopMode loopMode) {
            _loopMode = loopMode;
        }
        public enum LoopMode {
            LoopOnce = 1,
            LoopAll = 2,
            LoopNo = 3,
        }
        private MusicPlayer _musicPlayer;
        private MusicList _musicList;
        private int _currentIndex;
        private LoopMode _loopMode;
        private Timer _endtimer;
    }
}