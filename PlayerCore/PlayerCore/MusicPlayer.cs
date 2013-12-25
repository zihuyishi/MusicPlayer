using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Timers;
using libZPlay;

namespace PlayerCore
{

    public class MusicPlayer
    {
        private void Init() {
            m_player = new ZPlay();
            CallbackFunc = new TCallbackFunc(MyCallbackFunc);
            m_player.SetCallbackFunc(CallbackFunc, (TCallbackMessage)TCallbackMessage.MsgStop, 0);
        }
        public MusicPlayer() {
            Init();
        }
        private void _Play(string filePath) {
            m_player.OpenFile(filePath, TStreamFormat.sfAutodetect);
            m_player.StartPlayback();
            bMusicPlaying = true;
        }
        public void Play(string filePath) {
            _Play(filePath);
        }
        public void Play(MusicFile musicFile) {
            _Play(musicFile.FileName);
        }
        public void Pause() {
            m_player.PausePlayback();
        }
        public void Resume() {
            m_player.ResumePlayback();
        }
        public void Stop() {
            m_player.StopPlayback();
        }
        public int Volume {
            set {
                m_volume = value;
                m_volume %= 101;
                m_player.SetPlayerVolume(value, value);
            }
            get {
                return m_volume;
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
        public int MyCallbackFunc(uint objptr, int user_data,
            TCallbackMessage msg, uint param1, uint param2) {
            switch (msg) {
                case TCallbackMessage.MsgStop:
                    bMusicPlaying = false;
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
        public bool bMusicPlaying {
            get;
            set;
        }
        // global holder for our callback function, need this to prevent garbage collector to destroy our callback function
        private TCallbackFunc CallbackFunc;
        private ZPlay m_player;
        private int m_volume;
        public EventHandler OnMusicEnd;
        public readonly string SUPPORTFORMAT = "*.MP3;*.FLAC;*.WAV;*.AAC;*.OGG";
    }
}
