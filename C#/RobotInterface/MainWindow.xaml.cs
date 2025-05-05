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

    public partial class MainWindow : Window
    {
        ExtendedSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        
        Robot Robot = new Robot();
        String receptionWindowDspMode = "txt";
        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM11", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();
            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 1);
            timerAffichage.Tick += TimerAffichage_Tick1;
            timerAffichage.Start();
        }

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            for (int i = 0; i < e.Data.Length; i++)
            {
                Robot.byteListReceived.Enqueue(e.Data[i]);
            }
        }

        private void TimerAffichage_Tick1(object? sender, EventArgs e)
        {
            byte[] temp = new byte[64];
            
            for (int i = 0; i < Robot.byteListReceived.Count; i++) {
                
                byte receivedByte = Robot.byteListReceived.Dequeue();
                receptionWindowDisplay(receivedByte.ToString("X2"), "txt"); //affichage de tram
                Robot.CallDecodeMessage(receivedByte ); //traitement de tram
            }

        }
        public void receptionWindowDisplay(string textReceived, string dataToDspType)
        {
            
            switch (receptionWindowDspMode)
            {
                
                case "txt":
                    if (dataToDspType != "txt") { break; }
                        if (textReceived == "FE")
                        {
                            TextBoxReception.Text = ""; // On vide à chaque nouvelle tramme -> TODO faire un buffer circulaire 
                            
                        }
                        TextBoxReception.Text += textReceived;
                    break;
                case "dist":
                    if (Robot.sensor[0] != 0) 
                    {
                        TextBoxReception.Text = "Distance G: " + Robot.sensor[0].ToString() + "\n";
                        TextBoxReception.Text += "Distance C: " + Robot.sensor[2].ToString() + "\n";
                        TextBoxReception.Text += "Distance D: " + Robot.sensor[4].ToString();
                    }
                    else 
                    {
                        TextBoxReception.Text = "No data from robot";
                    }
                    break;
                case "pos":
                    TextBoxReception.Text = "";
                    TextBoxReception.Text = "Timestamp: " + (Robot.timestamp).ToString() + "\n";
                    TextBoxReception.Text += "xPosFromOdometry: " + (Robot.xPosFromOdometry).ToString() + "\n";
                    TextBoxReception.Text += "yPosFromOdometry: " + (Robot.yPosFromOdometry).ToString() + "\n";
                    TextBoxReception.Text += "angleRadianFromOdometry: " + (Robot.angleRadianFromOdometry).ToString() + "\n";
                    TextBoxReception.Text += "vitesseLineaireFromOdometry: " + (Robot.vitesseLineaireFromOdometry).ToString() + "\n";
                    TextBoxReception.Text += "vitesseAngulaireFromOdometry: " + (Robot.vitesseAngulaireFromOdometry).ToString();

                    break;
                default:
                    break;
            }
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


            for (int i = 0; i < msgPayload.Length - 1; i++) 
            {
                checksum ^= msgPayload[i];
            }
            return checksum;

        }

        private void BoutonEnvoyer_Click_1(object sender, RoutedEventArgs e)
        {
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
            /*bool textFormat = (bool)textCheckBox.IsChecked;
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
                String textToSend = TextBoxEmission.Text;
                String motor = textToSend;
                UartEncodeAndSendMessage(0x0040, bytesToSend.Length, bytesToSend);
            }
            else if (distFormat)
            {
                UartEncodeAndSendMessage(0x0030, bytesToSend.Length, bytesToSend);
            }
            else { TextBoxReception.Text += "At least one format has to be choosen\n"; }
            TextBoxEmission.Text = "";
            */


        }

      

        private void boutonClear_Click(object sender, RoutedEventArgs e)
        {
            TextBoxReception.Text = ""; 
        }

        private void boutonTest_Click(object sender, RoutedEventArgs e)
        {
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

        private void DisplayModeCheckBox_Checked(object sender, RoutedEventArgs e)
        {
            if (sender is CheckBox checkBox)
            {
                switch (checkBox.Name)
                {
                    case "textCheckBox":
                        receptionWindowDspMode = "txt";
                        break;
                    case "distCheckBox":
                        receptionWindowDspMode = "dist";
                        break;
                    case "posCheckBox":
                        receptionWindowDspMode = "pos";
                        break;
                }
            }
        }

        private void TextBox_TextChanged_1(object sender, TextChangedEventArgs e)
        {

        }
    }
}