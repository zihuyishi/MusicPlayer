using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Timers;
using System.Windows.Forms;
using libZPlay;

namespace PlayerCore
{

    public class MusicPlayer
    {
        private void Init() {
            _zPlayer = new ZPlay();
            _callbackFunc = new TCallbackFunc(MyCallbackFunc);
            _zPlayer.SetCallbackFunc(_callbackFunc, TCallbackMessage.MsgStop, 0);
        }
        public MusicPlayer() {
            Init();
        }
        private void _play(string filePath, 
            TStreamFormat format = TStreamFormat.sfAutodetect)
        {
            bool ret = _zPlayer.OpenFile(filePath, format);
            if (!ret) {
                MessageBox.Show(_zPlayer.GetError());
            }
            ret = _zPlayer.StartPlayback();
            if (!ret) {
                MessageBox.Show(_zPlayer.GetError());
            }
            MusicPlaying = true;
        }
        public void Play(string filePath) {
            _play(filePath);
        }
        public void Play(MusicFile musicFile) {
            _play(musicFile.FileName, musicFile.Format);
        }
        public void Pause() {
            _zPlayer.PausePlayback();
        }
        public void Resume() {
            _zPlayer.ResumePlayback();
        }
        public void Stop() {
            _zPlayer.StopPlayback();
        }
        public int Volume {
            set {
                _volume = value;
                _volume %= 101;
                _zPlayer.SetPlayerVolume(value, value);
            }
            get {
                return _volume;
            }
        }
        private int leftVolume {
            set;
            get;
        }
        private int rightVolume {
            set;
            get;
        }
        public int MyCallbackFunc(uint objptr, int userdata,
            TCallbackMessage msg, uint param1, uint param2) {
            switch (msg) {
                //播放完
                case TCallbackMessage.MsgStop:
                    MusicPlaying = false;
                    if (OnMusicEnd != null) {
                        OnMusicEnd(new object(), EventArgs.Empty);
                    }
                    break;
            }
            return 0;
        }
        /// <summary>
        /// 是否在播放音乐
        /// </summary>
        public bool MusicPlaying {
            get;
            set;
        }
        // global holder for our callback function, need this to prevent garbage collector to destroy our callback function
        private TCallbackFunc _callbackFunc;
        private ZPlay _zPlayer;
        private int _volume;
        public EventHandler OnMusicEnd;
    }
}
