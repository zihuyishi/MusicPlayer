using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PlayerCore
{
    public class MusicList
    {
        private void _init() {
            m_list = new List<MusicFile>();
        }
        public MusicList() {
            this._init();
        }
        public MusicList(List<MusicFile> newlist) {
            this._init();
            m_list = newlist;
        }
        public void Add(params string[] filenames) {
            foreach (string filename in filenames) {
                Add(new MusicFile(filename));
            }
            
        }
        public void Add(params MusicFile[] files) {
            m_list.AddRange(files);
        }
        public bool Remove(int index) {
            if (index < 0 || index >= ListLength) {
                return false;
            }
            m_list.RemoveAt(index);
            return true;
        }
        public bool Remove(MusicFile file) {
            bool ret = false;
            
            return ret;
        }
        public string GetFilePath(int index) {
            if (index < 0)
                index = 0;
            if (index > ListLength)
                index = ListLength - 1;
            return m_list[index].FileName;
        }
        public MusicFile this[int index] {
            get {
                index %= m_list.Count;
                return m_list[index];
            }
            private set {
                m_list[index] = value;
            }
        }
        public int ListLength {
            get{
                return m_list.Count;
            }
            
        }

        private List<MusicFile> m_list;
    }
}
