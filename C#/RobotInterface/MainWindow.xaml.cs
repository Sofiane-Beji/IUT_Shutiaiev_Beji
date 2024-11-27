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
using System.Windows.Threading;



namespace RobotInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    

    public partial class MainWindow : Window
    {
        ExtendedSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();

        

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();
            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 1);
            timerAffichage.Tick += TimerAffichage_Tick1;
            timerAffichage.Start();

        }
   
        private void TimerAffichage_Tick1(object? sender, EventArgs e)
        {
            byte[] temp = new byte[129];
            for (int i = 0; i < robot.byteListReceived.Count; i++) {
                temp[i] = robot.byteListReceived.Dequeue();
                TextBoxReception.Text += robot.byteListReceived.Dequeue().ToString("X2") + " ";
            }
            //TextBoxReception.Text += Encoding.UTF8.GetString(temp);


        }

        private void TextBoxEmission_TextChanged()
        {

        }

        private void TextBoxEmission_TextChanged(object sender, TextChangedEventArgs e)
        {


        }

        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload) {
            byte tram = 0xFE;
            tram |= (byte)(msgFunction >> 16);
            tram |= (byte)(msgPayloadLength >> 32);
            int size = 32;
            for (int i = 0; i < msgPayload.Length; i++)
            {
                size = i * 8;
                tram |= (byte)(msgPayload[i] >> size);
            }
            byte checkSum = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload); 
            tram |= (byte)(checkSum >> size + 8);
            for (int i = 0; i < size + 8; i++)
            {
                serialPort1.Write(tram[i]);
            }

        }
        //bool a = false;
        byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;
            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);


            for (int i = 0; i < msgPayload.Length ; i++) 
            {
                checksum ^= msgPayload[i];
            }
            return checksum;

        }

        private void BoutonEnvoyer_Click_1(object sender, RoutedEventArgs e)
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
            string val = TextBoxEmission.Text;

            if (val != "" && val.Replace("\r\n", string.Empty) != "")
            {
                TextBoxEmission.Text = "";
            

                serialPort1.Write((val.Replace("\r\n", string.Empty)) + "\r\n");
            }
            else { TextBoxEmission.Text = ""; }
        }

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            for (int i = 0; i < e.Data.Length; i++)
            {
                robot.byteListReceived.Enqueue(e.Data[i]);
            }


            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
        }

        private void boutonClear_Click(object sender, RoutedEventArgs e)
        {
            TextBoxReception.Text = "";
        }

        private void boutonTest_Click(object sender, RoutedEventArgs e)
        {
            byte[] array = Encoding.ASCII.GetBytes("Bonjour");
            UartEncodeAndSendMessage(0x0080, array.Length, array);


        }
    }
}