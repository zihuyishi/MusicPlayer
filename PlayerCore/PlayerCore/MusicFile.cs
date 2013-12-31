using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using libZPlay;
namespace PlayerCore
{
    public sealed class MusicFile: IEquatable<MusicFile>
    {
        public static readonly MusicFile Empty;
        public MusicFile(string filename) {
            _loadImformation(filename);
        }
        public MusicFile(MusicFile file) {
            _loadImformation(file.FilePath);
        }

        
        private void _loadImformation(string filename) {
            IsEmpty = true;
            if (filename == string.Empty) {
                return;
            }
            FilePath = filename;
            #region 获取文件信息
            #region 设置文件时间
            ZPlay tempPlay = new ZPlay();
            //如果文件不存在，则退出构造..
            bool bret = tempPlay.OpenFile(filename, TStreamFormat.sfAutodetect);
            if (!bret) return;
            TStreamInfo streamInfo = new TStreamInfo();
            tempPlay.GetStreamInfo(ref streamInfo);
            Length = streamInfo.Length;
            Minutes = streamInfo.Length.sec / 60;
            Seconds = streamInfo.Length.sec % 60;

            tempPlay.Close();
            #endregion
            #region 获取歌曲名
            string musicName = filename.Substring(filename.LastIndexOf('\\') + 1);
            MusicName = musicName.Substring(0, musicName.LastIndexOf('.'));
            #endregion
            #region 设置文件格式
            string format = filename.Substring(filename.LastIndexOf('.'));
            switch (format.ToLower()) {
                case ".wav":
                    Format = TStreamFormat.sfWav;
                    break;
                case ".mp3":
                    Format = TStreamFormat.sfMp3;
                    break;
                case ".flac":
                    Format = TStreamFormat.sfFLAC;
                    break;
                default:
                    Format = TStreamFormat.sfAutodetect;
                    break;
            }
            #endregion
            IsEmpty = false;

            #endregion
        }
        #region 重载ToString Equals等

        public override bool Equals(object obj) {
            return Equals((MusicFile)obj);
        }

        public override string ToString() {
            return FilePath;
        }

        public override int GetHashCode() {
            return FilePath.GetHashCode() * 37 + Minutes.GetHashCode();
        }
        
        public bool Equals(MusicFile other) {
            return other.FilePath == FilePath;
        }
        #endregion
        public uint Minutes { set; get; }
        public uint Seconds { set; get; }
        public string FilePath { set; get; }
        public string MusicName { set; get; }
        public bool IsEmpty { set; get; }
        internal TStreamTime Length { get; set; }
        public TStreamFormat Format { get; set; }
        public static readonly string SupportFormat = "*.MP3;*.FLAC;*.WAV;*.AAC;*.OGG";
    }
}
