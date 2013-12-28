using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Xml;
using System.Xml.Linq;

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
            return _list.Remove(file);
        }
        /// <summary>
        /// 将列表保存到文件
        /// </summary>
        /// <param name="xmlFilepath">目标文件</param>
        /// <returns>是否保存成功</returns>
        public bool SaveListToFile(string xmlFilepath) {
            XDocument xDoc = new XDocument
            {
                Declaration = new XDeclaration("1.0", "UTF-8", null)
            };
            xDoc.Add(new XElement(ListName));
            XElement xRoot = xDoc.Element(ListName);
            foreach (var musicfile in _list) {
                string filelength = Convert.ToString(musicfile.Minutes) +
                                     " : " + Convert.ToString(musicfile.Seconds);
                XElement xElement =
                    new XElement("MusicFile",
                        new XElement("filepath", musicfile.FilePath),
                        new XElement("filelength", filelength),
                        new XAttribute("musicname", musicfile.MusicName));
                Debug.Assert(xRoot != null, "xRoot != null");
                xRoot.Add(xElement);
            }
            try {
                xDoc.Save(xmlFilepath);
            }
            catch (Exception) {
                return false;
            }

            return true;
        }
        /// <summary>
        /// 加载音乐列表
        /// </summary>
        /// <param name="filepath">文件路径</param>
        /// <returns>是否加载成功</returns>
        public bool LoadListFromFile(string filepath) {
            XmlDocument xmlDoc = new XmlDocument();
            try {
                xmlDoc.Load(filepath);
            }
            catch (Exception) {
                return false;
            }
            XmlElement xmlRoot = xmlDoc.DocumentElement;
            if (xmlRoot == null) return false;
            ListName = xmlRoot.Name;
            foreach (XmlNode xmlNode in xmlRoot) {
                string listpath = (from XmlNode childNode in xmlNode
                                   where childNode.Name.ToLower() == "filepath" 
                                   select childNode.InnerText).FirstOrDefault();
                if (listpath == null) continue;
                MusicFile musicFile = new MusicFile(listpath);
                if (musicFile.IsEmpty) continue;
                _list.Add(musicFile);
            }
            return _list.Count() != 0;
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
