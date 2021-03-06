﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;
using PlayController;
using VolumnBar;

namespace WPFAPlayer
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow() {
            InitializeComponent();
            this.Init();
        }
        #region 初始化函数
        private void Init() {
            _musicPlayer = new MusicController();
            _isPlaying = false;
            _musicPlayer.SetLoopMode(MusicController.LoopMode.LoopAll);
            _controlList = new List<Control>();
            //自己工作听歌用....
            //_musicPlayer.Volume = 15;
        }
        #endregion

        private void PlayButton_OnClick(object sender, RoutedEventArgs e) {
            if (_isPlaying) {
                _musicPlayer.Pause();
                ((Button) sender).Content = "Play";
            }
            else {
                if (_musicPlayer.HasPlayingMusic()) {
                    _musicPlayer.Resume();
                }
                else {
                    if (!_musicPlayer.Play()) {
                        if (!_musicPlayer.ShowDlgAndPlay()) return;
                    }
                }
                ((Button) sender).Content = "Pause";
            }
            _isPlaying = !_isPlaying;
        }

        private void AddMusicButton_OnClick(object sender, RoutedEventArgs e) {
            _musicPlayer.ShowDlgAndAdd();
        }
        #region 设置是否显示按钮
        private void SetControlVisibility(Visibility buttonVisible) {
            foreach (var control in _controlList) {
                control.Visibility = buttonVisible;
            }
        }
        #endregion
        private void MainGrid_OnMouseMove(object sender, MouseEventArgs e) {
            
        }

        private void MainGrid_OnMouseLeave(object sender, MouseEventArgs e) {
            SetControlVisibility(Visibility.Hidden);
        }

        private void MainGrid_Loaded(object sender, RoutedEventArgs e) {
            SetControlVisibility(Visibility.Hidden);
            
        }

        private void MainGrid_OnMouseEnter(object sender, MouseEventArgs e) {
            SetControlVisibility(Visibility.Visible);
        }

        private void NextButton_OnClick(object sender, RoutedEventArgs e) {
            if (_musicPlayer.PlayNext(true)) {
                PlayButton.Content = "Pause";
            }
        }

        private void AllFormControl_Loaded(object sender, RoutedEventArgs e) {
            _controlList.Add((Control)sender);
        }

        

        private void LoadListButton_OnClick(object sender, RoutedEventArgs e) {
            _musicPlayer.ShowDlgAndLoadList();
        }

        private void SaveListButton_OnClick(object sender, RoutedEventArgs e) {
            _musicPlayer.ShowDlgAndSaveList();
        }
        private void MainGrid_MouseRightButtonUp(object sender, MouseButtonEventArgs e) {
            OpenFileDialog fileDlg = new OpenFileDialog
            {
                RestoreDirectory = true,
                Filter = "图片文件|*.png;*.jpg"
            };
            if (fileDlg.ShowDialog() != true) return;
            string filepath = fileDlg.FileName;
            this.ChangeBackImage(filepath);
        }
        
        #region 界面效果方法s

        private void ChangeBackImage(string imagePath) {
            ImageSource imageSource = new BitmapImage(new Uri(imagePath));
            int width = Convert.ToInt32(imageSource.Width),
                height = Convert.ToInt32(imageSource.Height);
            width = width*800/height;
            height = 800;
            this.Width = width;
            this.Height = height;
            this.MainGrid.Background = new ImageBrush(imageSource);
        }

        #endregion
        private List<Control> _controlList;
        private MusicController _musicPlayer;
        private bool _isPlaying;
        


        private void VolumnProgressBar_OnValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e) {
            double value = ((ProgressBar) sender).Value;
            _musicPlayer.Volume = Convert.ToInt32(value);

        }
        private void VolumnBar_OnOnValueChanged(object sender, EventArgs e) {
            _musicPlayer.Volume = ((VolumnBarControl) sender).Value;
        }
    }
}
