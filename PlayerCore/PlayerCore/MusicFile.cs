using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PlayerCore
{
    public class MusicFile
    {
        public MusicFile(string filename) {
            FileName = filename;
            Format = filename.Substring(filename.LastIndexOf('.'));
        }

        public string FileName {
            set;
            get;
        }

        public string Format {
            get;
            set;
        }
    }
}
