using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Xml;
namespace PlayerCore
{
    public class MusicList
    {
        #region 构造函数初始化
        private void _init() {
            _list = new List<MusicFile>();
            ListName = "默认列表";
        }
        #endregion
        public MusicList() {
            _init();
        }
        public MusicList(List<MusicFile> newlist) {
            _init();
            _list = newlist;
        }
        /// <summary>
        /// 添加文件到列表
        /// </summary>
        /// <param name="filepaths"></param>
        public void Add(params string[] filepaths) {
            foreach (string filepath in filepaths) {
                Add(new MusicFile(filepath));
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
            int index = _list.FindIndex((musicFile) => musicFile.FilePath == file.FilePath);
            if (index == -1) return false;
            _list.RemoveAt(index);
            */
            return _list.Remove(file);
        }
        /// <summary>
        /// 将列表保存到文件
        /// </summary>
        /// <param name="xmlFilepath">目标文件</param>
        /// <returns>是否保存成功</returns>
        public bool SaveListToFile(string xmlFilepath) {
            XmlDocument xmlDoc = new XmlDocument();
            XmlDeclaration xmlDeclar = xmlDoc.CreateXmlDeclaration("1.0", "UTF-8", null);
            xmlDoc.AppendChild(xmlDeclar);
            XmlElement xmlElement = xmlDoc.CreateElement("", ListName, "");
            xmlDoc.AppendChild(xmlElement);
            foreach (var musicfile in _list) {
                XmlNode xmlRoot = xmlDoc.SelectSingleNode(ListName);
                xmlElement = xmlDoc.CreateElement("MusicFile");
                xmlElement.SetAttribute("musicname", musicfile.MusicName);
                XmlNode xmlnode1 = xmlDoc.CreateNode("element", "filepath", "");
                xmlnode1.InnerText = musicfile.FilePath;
                xmlElement.AppendChild(xmlnode1);
                xmlnode1 = xmlDoc.CreateNode("element", "filelength", "");
                xmlnode1.InnerText = Convert.ToString(musicfile.Minutes) +
                                     " : " + Convert.ToString(musicfile.Seconds);
                xmlElement.AppendChild(xmlnode1);
                //xmlElement.SetAttribute("filename", musicfile.FilePath);
                Debug.Assert(xmlRoot != null, "xmlRoot != null");
                xmlRoot.AppendChild(xmlElement);
            }
            try {
                xmlDoc.Save(xmlFilepath);
            }
            catch (Exception ex) {
                return false;
            }

            return true;
        }
        public void RemoveAll() {
            _list.Clear();
        }
        /// <summary>
        /// 获取歌曲文件位置
        /// </summary>
        /// <param name="index">要获取的项数</param>
        /// <returns>文件名</returns>
        public string GetFilePath(int index) {
            if (index < 0)
                index = 0;
            if (index > ListLength)
                index = ListLength - 1;
            return _list[index].FilePath;
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
        public string ListName { get; set; }
        private List<MusicFile> _list;
    }
}
