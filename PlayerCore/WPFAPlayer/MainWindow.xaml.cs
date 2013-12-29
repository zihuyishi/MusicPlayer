using System;
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
using PlayControlor;

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
            _musicPlayer = new MusicControlor();
            _isPlaying = false;
            _musicPlayer.SetLoopMode(MusicControlor.LoopMode.LoopAll);
            _buttonList = new List<Button>();
        }
        #endregion
        private MusicControlor _musicPlayer;
        private bool _isPlaying;
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
        private void SetButtonVisibility(Visibility buttonVisible) {
            foreach (Button button in _buttonList) {
                button.Visibility = buttonVisible;
            }
        }
        #endregion
        private void MainGrid_OnMouseMove(object sender, MouseEventArgs e) {
            
        }

        private void MainGrid_OnMouseLeave(object sender, MouseEventArgs e) {
            SetButtonVisibility(Visibility.Hidden);
        }

        private void MainGrid_Loaded(object sender, RoutedEventArgs e) {
            SetButtonVisibility(Visibility.Hidden);
        }

        private void MainGrid_OnMouseEnter(object sender, MouseEventArgs e) {
            SetButtonVisibility(Visibility.Visible);
        }

        private void NextButton_OnClick(object sender, RoutedEventArgs e) {
            _musicPlayer.PlayNext(true);
        }

        private void AllFormButton_Loaded(object sender, RoutedEventArgs e) {
            _buttonList.Add((Button) sender);
        }

        private List<Button> _buttonList;

        private void LoadListButton_OnClick(object sender, RoutedEventArgs e) {
            _musicPlayer.ShowDlgAndLoadList();
        }

        private void SaveListButton_OnClick(object sender, RoutedEventArgs e) {
            _musicPlayer.ShowDlgAndSaveList();
        }
    }
}
