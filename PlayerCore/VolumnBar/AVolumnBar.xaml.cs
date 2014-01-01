using System;
using System.Collections.Generic;
using System.Diagnostics;
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

namespace VolumnBar
{
    /// <summary>
    /// UserControl1.xaml 的交互逻辑
    /// </summary>
    public partial class VolumnBarControl : UserControl
    {
        public VolumnBarControl()
        {
            InitializeComponent();
            Maximum = 100;
            Minimum = 0;
        }

        public VolumnBarControl(int max, int min) {
            InitializeComponent();
            Maximum = max;
            Minimum = min;
        }
        public int Value {
            set {
                if (value > Maximum || value < Minimum) {
                    Debug.Fail("无效的值");
                    return;
                }
                _value = value;

                if (OnValueChanged != null) {
                    OnValueChanged(this, new EventArgs());
                }
            }
            get { return _value; }
        }
        public int Maximum { get; set; }
        public int Minimum { get; set; }
        public event EventHandler OnValueChanged;
        private int _value;


        private void MainGrid_MouseMove(object sender, MouseEventArgs e) {
            if (e.LeftButton != MouseButtonState.Pressed) return;
            Point point = e.GetPosition(this.MainGrid);
            double right = this.Width - (point.X + OneEllipse.Height / 2);
            if (right < 0) right = 0;
            OneEllipse.Margin = new Thickness(0, 0, right, 0);
            double percent = point.X / this.Width;
            double value = percent * (Maximum - Minimum) + Minimum;
            Value = Convert.ToInt32(value);
        }

        private void MainGrid_SizeChanged(object sender, SizeChangedEventArgs e) {
            this.OneEllipse.Height = this.Height;
            this.OneEllipse.Width = this.Height;
        }

        private void MainGrid_MouseLeftButtonUp(object sender, MouseButtonEventArgs e) {

        }
        private void MainGrid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e) {
            MainGrid_MouseMove(sender, e);
        }
    }
}
