using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;

namespace PlayerCore
{
    public class MusicList
    {
        private void _init() {
            _list = new List<MusicFile>();
        }
        public MusicList() {
            _init();
        }
        public MusicList(List<MusicFile> newlist) {
            _init();
            _list = newlist;
        }
        public void Add(params string[] filenames) {
            foreach (string filename in filenames) {
                Add(new MusicFile(filename));
            }
            
        }
        public void Add(params MusicFile[] files) {
            _list.AddRange(files);
        }
        public bool Remove(int index) {
            if (index < 0 || index >= ListLength) {
                return false;
            }
            _list.RemoveAt(index);
            return true;
        }
        public bool Remove(MusicFile file) {
            /*
            int index = _list.FindIndex((musicFile) => musicFile.FileName == file.FileName);
            if (index == -1) return false;
            _list.RemoveAt(index);
            */
            return _list.Remove(file);
        }

        public void RemoveAll() {
            _list.Clear();
        }
        public string GetFilePath(int index) {
            if (index < 0)
                index = 0;
            if (index > ListLength)
                index = ListLength - 1;
            return _list[index].FileName;
        }
        public MusicFile this[int index] {
            get {
                index %= _list.Count;
                return _list[index];
            }
            private set {
                _list[index] = value;
            }
        }
        public int ListLength {
            get{
                return _list.Count;
            }
            
        }

        private List<MusicFile> _list;
    }
}
