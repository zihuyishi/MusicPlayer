using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PlayerCore
{
    internal class MusicFile
    {
        public string FileName {
            set;
            get;
        }
        public MusicFile(string filename) {
            FileName = filename;
        }
    }
}
