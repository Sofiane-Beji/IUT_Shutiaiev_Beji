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
        reception reception = new reception();

        public void textReceivedAdd(string textReceived)
        {
            TextBoxReception.Text += textReceived;
        }

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
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
                //temp[i] = robot.byteListReceived.Dequeue();
                byte receivedByte = robot.byteListReceived.Dequeue();
                textReceivedAdd(receivedByte.ToString("X2"));
                reception.CallDecodeMessage(receivedByte);
                //TextBoxReception.Text += robot.byteListReceived.Dequeue()
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
            byte[] tram = new byte[6 + msgPayloadLength];
            tram[0] = 0xFE;
            tram[1] = (byte)(msgFunction >> 8);
            tram[2] = (byte)(msgFunction);
            tram[3] = (byte)(msgPayloadLength >> 8);
            tram[4] = (byte)(msgPayloadLength);

            for (int i = 5; i < msgPayload.Length + 5; i++)
            {
               
                tram[i] = (byte)(msgPayload[i - 5]);
            }
            byte checkSum = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            tram[5 + msgPayloadLength] = checkSum;
            serialPort1.Write(tram, 0, 6 + msgPayloadLength);
            
            

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
            bool textFormat = (bool)textCheckBox.IsChecked;
            bool LEDFormat = (bool)ledCheckBox.IsChecked;
            bool speedtFormat = (bool)speedCheckBox.IsChecked;
            bool distFormat = (bool)distCheckBox.IsChecked;

            byte[] bytesToSend = Encoding.ASCII.GetBytes(TextBoxEmission.Text);
            

            if (textFormat && LEDFormat && speedtFormat && distFormat)
            { }
            else if (textFormat)
            {
                UartEncodeAndSendMessage(0x0080, bytesToSend.Length, bytesToSend);
            }
            else if (LEDFormat)
            {
                UartEncodeAndSendMessage(0x0020, bytesToSend.Length, bytesToSend);
            }
            else if (speedtFormat)
            {
                UartEncodeAndSendMessage(0x0040, bytesToSend.Length, bytesToSend);
            }
            else if (distFormat)
            {
                UartEncodeAndSendMessage(0x0030, bytesToSend.Length, bytesToSend);
            }
            else { TextBoxReception.Text += "At least one format has to be choosen\n"; }
            TextBoxEmission.Text = "";



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
            //byte[] array = Encoding.ASCII.GetBytes("Bonjour");
            //UartEncodeAndSendMessage(0x0080, array.Length, array);
            string text = "Bonjour";
            byte[] temp = Encoding.ASCII.GetBytes(text);
            UartEncodeAndSendMessage(0x0080, temp.Length, temp);

        }
      
        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void TextBoxReception_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {

        }
    }
}