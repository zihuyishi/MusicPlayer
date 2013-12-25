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
        
        public MusicPlayer() {
            m_player = new ZPlay();
        }

        public void Play(string filePath) {

        }
        public void Pause() {

        }
        public void Resume() {
        }

        private Timer m_Timer;
        ZPlay m_player;
    }
}
