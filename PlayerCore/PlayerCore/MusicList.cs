using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PlayerCore
{
    internal class MusicList
    {
        public MusicList() {
            this._init();
        }
        public MusicList(List<MusicFile> newlist) {
            m_list = newlist;
        }
        public MusicFile nextMusic {
            set;
            get;
        }
        public int ListLength() {
            return m_list.Count;
        }
        private void _init() {
            m_list = new List<MusicFile>();
        }
        private List<MusicFile> m_list;
    }
}
