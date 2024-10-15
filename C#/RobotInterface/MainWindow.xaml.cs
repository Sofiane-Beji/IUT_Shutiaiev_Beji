using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ExtendedSerialPort_NS;
using System.IO.Ports;
using System.Diagnostics.Tracing;
using System.Runtime.Serialization;


namespace RobotInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    

    public partial class MainWindow : Window
    {
        ExtendedSerialPort serialPort1;

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM6", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();
        }

        private void TextBoxEmission_TextChanged()
        {

        }

        private void TextBoxEmission_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        //bool a = false;

        private void BoutonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            /*if (a == false)
            {
                boutonEnvoyer.Background = Brushes.RoyalBlue;
                a = true;
            }
            else
            {
                boutonEnvoyer.Background = Brushes.Beige;
                a = false;
            }*/

            SendMessage();


        }

        private void TextBoxEmission_KeyUp_1(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }

        private void SendMessage()
        {
            String val = TextBoxEmission.Text;
            TextBoxReception.Text = TextBoxReception.Text + "Reçu : " + val + "\n";
            TextBoxEmission.Text = " ";

            serialPort1.WriteLine(val);
        }

    }
}