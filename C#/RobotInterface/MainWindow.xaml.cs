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
using static SciChart.Drawing.Utility.PointUtil;
using SciChart.Data.Model;
using SciChart.Charting.Visuals;

using System.Windows.Media;
using WpfOscilloscopeControl;
using WpfWorldMap_NS;
using WpfAsservissementDisplay;
using Newtonsoft.Json.Linq;
using System.ComponentModel.Design;
using System.Diagnostics;

namespace RobotInterface
{

    public partial class MainWindow : Window
    {
        bool rawTramDspMode = true;
        bool posDspMode = false;
        bool speedDspMode = false;
        bool distDspMode = false;

        ExtendedSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        
        Robot Robot = new Robot();
        String receptionWindowDspMode = "txt";


        public MainWindow()
        {
            // Set this code once in App.xaml.cs or application startup
            SciChartSurface.SetRuntimeLicenseKey("rawGYdZucXOANEX0TnQ0wSvPHXM3trkTCdRqc9VgfXhPE3bF05t7I6j41xW49IvBviM4ep3kbiO/Gj9qQ3rNDcq89Lm8hXfqu/0rMuj5hoTrcn1knJIGGB85+GaoUQP4ZV+mMFPnL3b2erT/NXobKdwi9SmlNJHfoesS4uM7AvLOSUymT8FknehVb1Ur9rqr1jxMn37sDcBql2nBHuDDSXiX1Fl0EB7OiMKyPZf+bEdqKE+j4+oOyZIGvuXbIAnffl4b1jv5J9vW2LmlDptXKLPQ42oywJwL+GmejhqV6VMSWB/9wtPfHKrPxRAI+7ViMMoKyIwR0Fch+PoLLRvA2ACLjXza3JCp3+SGXEGnajE8g+Wyey6gqIbxz1dGhEq8QXQa3X/QPfrZb4G/B+SFgo/Q8FJGn4sxCV+n469Wr92YF+d23giEOt49UFn2IL6czW19nFUQF09pRX6buAq0ULeFFeQs0vB0uxH+aosgIr5SuR6+W+9ptFSxJf+XZiA44wbhney7");

            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM11", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();
            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 1);
            timerAffichage.Tick += TimerAffichage_Tick1;
            timerAffichage.Start();


            oscilloSpeed.isDisplayActivated = true;
            oscilloSpeed.AddOrUpdateLine(0, 200, "Vitesse");
            oscilloSpeed.ChangeLineColor("Vitesse", Colors.LightBlue);
            oscilloSpeed.AddOrUpdateLine(1, 200, "AngleRadian");
            oscilloSpeed.ChangeLineColor("AngleRadian", Colors.Yellow);

            //oscilloDirection.isDisplayActivated = true;
            //oscilloDistance.isDisplayActivated = true;
            //oscilloDirection.AddOrUpdateLine(0, 200, "Direct. & Speed");
            //oscilloDistance.AddOrUpdateLine(lineId, 200, "Distance");





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

            //oscilloDirection.AddPointToLine(0, Double.Parse((Robot.angleRadianFromOdometry* 57.2958).ToString()), Double.Parse((Robot.vitesseAngulaireFromOdometry).ToString()));
            oscilloSpeed.AddPointToLine(0, Double.Parse((Robot.timestamp).ToString()), Double.Parse((Robot.vitesseLineaireFromOdometry).ToString()));
            oscilloSpeed.AddPointToLine(1, Double.Parse((Robot.timestamp).ToString()), Double.Parse((Robot.angleRadianFromOdometry).ToString()));

            //oscilloDistance.AddPointToLine(lineId, Double.Parse(0.ToString()), Double.Parse((Robot.sensor[2]/ 100.0).ToString()));
            //Console.WriteLine((Robot.distanceTelemetreCentre / 100.0).ToString());
            //oscilloDistance.AddPointToLine(lineId, Double.Parse(30.ToString()), Double.Parse(((Robot.sensor[4] / 100.0).ToString() ).ToString()));

            worldMapDisplay.UpdatePosRobot(Robot.xPosFromOdometry, Robot.yPosFromOdometry);
            worldMapDisplay.UpdateOrientationRobot(Robot.angleDegre);

            //asservSpeedDisplay.UpdateIndependantOdometry(Robot.xPosFromOdometry, Robot.yPosFromOdometry);
            asservSpeedDisplay.UpdatePolarCorrectionGains(Robot.kpX, Robot.kpTheta, Robot.kiX, Robot.kiTheta, Robot.kdX, Robot.kdTheta);
            asservSpeedDisplay.UpdatePolarCorrectionLimits(Robot.proportionnelleMaxX, Robot.proportionnelleMaxTheta, Robot.integralMaxX, Robot.integralMaxTheta, Robot.deriveeMaxX, Robot.deriveeMaxTheta);

            asservSpeedDisplay.UpdatePolarConsigneValues(Robot.consigneX, Robot.consigneTheta);
            asservSpeedDisplay.UpdateIndependantConsigneValues(Robot.consigneM1, Robot.consigneM2);
            asservSpeedDisplay.UpdatePolarCommandValues(Robot.commandX, Robot.commandtheta);
            asservSpeedDisplay.UpdateIndependantSpeedCommandValues(Robot.commandM1, Robot.commandM2);
            asservSpeedDisplay.UpdatePolarOdometry(Robot.valueX, Robot.valueTheta);
            asservSpeedDisplay.UpdateIndependantOdometry(Robot.valueM1, Robot.valueM2);
            asservSpeedDisplay.UpdatePolarErrorValues(Robot.errorX, Robot.errorTheta);
            asservSpeedDisplay.UpdateIndependantErrorValues(Robot.errorM1, Robot.errorM2);
            asservSpeedDisplay.UpdatePolarCorrectionValues(Robot.corrPX, Robot.corrPTheta, Robot.corrIX, Robot.corrITheta, Robot.corrDX, Robot.corrDTheta);

        }
        public void receptionWindowDisplay(string textReceived, string dataToDspType)
        {
            if (textReceived == "FE")
            {
                TextBoxReception.Text = ""; // On vide à chaque nouvelle tramme -> TODO faire un buffer circulaire 
                if (distDspMode)
                {
                    if (Robot.sensor[0] != 0)
                    {
                        TextBoxReception.Text += "------------Distance\n";
                        TextBoxReception.Text += "Distance G: " + Robot.sensor[0].ToString() + "\n";
                        TextBoxReception.Text += "Distance C: " + Robot.sensor[2].ToString() + "\n";
                        TextBoxReception.Text += "Distance D: " + Robot.sensor[4].ToString() + "\n\n";
                    }
                    else
                    {
                        TextBoxReception.Text += "------------Distance\n";
                        TextBoxReception.Text += "No data from robot" + "\n\n";
                    }
                }
                if (posDspMode)
                {
                    TextBoxReception.Text += "------------Position\n";
                    TextBoxReception.Text += "Timestamp: " + (Robot.timestamp).ToString() + "\n";
                    TextBoxReception.Text += "xPosFromOdometry: " + (Robot.xPosFromOdometry).ToString() + "\n";
                    TextBoxReception.Text += "yPosFromOdometry: " + (Robot.yPosFromOdometry).ToString() + "\n";
                    TextBoxReception.Text += "angleRadianFromOdometry: " + (Robot.angleRadianFromOdometry).ToString() + "\n";
                    TextBoxReception.Text += "angleDegre: " + (Robot.angleDegre).ToString() + "\n";
                    TextBoxReception.Text += "vitesseLineaireFromOdometry: " + (Robot.vitesseLineaireFromOdometry).ToString() + "\n";
                    TextBoxReception.Text += "vitesseAngulaireFromOdometry: " + (Robot.vitesseAngulaireFromOdometry).ToString() + "\n\n";

                    TextBoxReception.Text += "Ghost_Xpos : " + (Robot.positionXGhosto).ToString() + "\n";
                    TextBoxReception.Text += "Ghost_Xpos : " + (Robot.positionYGhosto).ToString() + "\n";
                    TextBoxReception.Text += "Angle_RadGhost : " + (Robot.angleRadGhosto).ToString() + "\n";

                    TextBoxReception.Text += "KpX = " + Robot.kpX + " kiX = " + Robot.kiX + " kdX = " + Robot.kdX + "\n" + "PropX = " +
                    Robot.proportionnelleMaxX + " IntX = " + Robot.integralMaxX + " DerivX = " + Robot.deriveeMaxX + "\n\n" + "kpTheta = " + Robot.kpTheta + " kiTheta = " + Robot.kiTheta + " kdTheta = " + Robot.kdTheta + "\n" + "proportionnelleMaxTheta = " +
                    Robot.proportionnelleMaxTheta + " integralMaxTheta = " + Robot.integralMaxTheta + " deriveeMaxTheta = " + Robot.deriveeMaxTheta + "\n";

                    TextBoxReception.Text += debg;
                }
                if (rawTramDspMode)
                {
                    TextBoxReception.Text += "------------Raw data\n";
                }
                }
            if (rawTramDspMode) {
                
                TextBoxReception.Text += textReceived; }
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

        string debg;

        private void boutonClear_Click(object sender, RoutedEventArgs e)
        {
            TextBoxReception.Text = ""; 
        }

        private void boutonTest_Click(object sender, RoutedEventArgs e)
        {
            float kpX = 0f;
            float kiX = 0f;
            float kdX = 0f;
            float limitPX = 0f;
            float limitIX = 0f;
            float limitDX = 0f;
            float kpTheta = 0f;
            float kiTheta = 0f;
            float kdTheta = 0f;
            float limitPTheta = 0f;
            float limitITheta = 0f;
            float limitDTheta = 0f;
            byte[] pidPayload = new byte[48];
            BitConverter.GetBytes(kpX).CopyTo(pidPayload, 0);
            BitConverter.GetBytes(kiX).CopyTo(pidPayload, 4);
            BitConverter.GetBytes(kdX).CopyTo(pidPayload, 8);
            BitConverter.GetBytes(limitPX).CopyTo(pidPayload, 12);
            BitConverter.GetBytes(limitIX).CopyTo(pidPayload, 16);
            BitConverter.GetBytes(limitDX).CopyTo(pidPayload, 20);
            BitConverter.GetBytes(kpTheta).CopyTo(pidPayload, 24);
            BitConverter.GetBytes(kiTheta).CopyTo(pidPayload, 28);
            BitConverter.GetBytes(kdTheta).CopyTo(pidPayload, 32);
            BitConverter.GetBytes(limitPTheta).CopyTo(pidPayload, 36);
            BitConverter.GetBytes(limitITheta).CopyTo(pidPayload, 40);
            BitConverter.GetBytes(limitDTheta).CopyTo(pidPayload, 44);

            debg = null;
            for (int i = 0; i < pidPayload.Length; i++)
            {
                debg += pidPayload[i] + "/";
                Console.Write($"{pidPayload[i]:X2} ");
            }
            Console.WriteLine();


            UartEncodeAndSendMessage(0x0061, pidPayload.Length, pidPayload);

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
                        rawTramDspMode = true;
                        break;
                    case "distCheckBox":
                        distDspMode = true;
                        break;
                    case "posCheckBox":
                        posDspMode = true;
                        break;
                }
            }
        }
        private void DisplayModeCheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            if (sender is CheckBox checkBox)
            {
                switch (checkBox.Name)
                {
                    case "textCheckBox":
                        rawTramDspMode = false;
                        break;
                    case "distCheckBox":
                        distDspMode = false;
                        break;
                    case "posCheckBox":
                        posDspMode = false;
                        break;
                }
            }
        }

        private void TextBox_TextChanged_1(object sender, TextChangedEventArgs e)
        {

        }

       
    }
}