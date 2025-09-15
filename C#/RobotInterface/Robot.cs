using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Text;
using System.Windows;
using System.Windows.Threading;

using WpfOscilloscopeControl;
using WpfWorldMap_NS;
using WpfAsservissementDisplay;
using RobotInterface;


public class Robot
{
    public Queue<byte> byteListReceived = new Queue<byte>();
    
    public float distanceTelemetreDroit;
    public float distanceTelemetreCentre;
    public float distanceTelemetreGauche;

    public float timestamp = 0.0f;
    public float xPosFromOdometry = 0.0f;
    public float yPosFromOdometry = 0.0f;
    public float angleRadianFromOdometry = 0.0f;
    public float angleDegre = 0.0f;
    public float vitesseLineaireFromOdometry = 0.0f;
    public float vitesseAngulaireFromOdometry = 0.0f;

    public float angleRadGhosto = 0.0f;
    public float vitesseLineaireGhosto = 0.0f;
    public float vitesseAngGhosto = 0.0f;
    public float positionXGhosto = 0.0f;
    public float positionYGhosto = 0.0f;


    public float kpX;
    public float kiX;
    public float kdX;
    public float proportionnelleMaxX;
    public float integralMaxX;
    public float deriveeMaxX;

    public float kpTheta;
    public float kiTheta;
    public float kdTheta;
    public float proportionnelleMaxTheta;
    public float integralMaxTheta;
    public float deriveeMaxTheta;

    public float consigneX;
    public float consigneTheta;
    public float consigneM1;
    public float consigneM2;

    public float valueX;
    public float valueTheta;
    public float valueM1;
    public float valueM2;

    public float errorX;
    public float errorTheta;
    public float errorM1;
    public float errorM2;

    public float commandX;
    public float commandtheta;
    public float commandM1;
    public float commandM2;

    public float corrPX;
    public float corrPTheta;

    public float corrIX;
    public float corrITheta;

    public float corrDX;
    public float corrDTheta;


    public Robot()
    {

    }
    public enum StateReception
    {
        Waiting,
        FunctionMSB,
        FunctionLSB,
        PayloadLengthMSB,
        PayloadLengthLSB,
        Payload,
        CheckSum
    }

    StateReception rcvState = StateReception.Waiting;

    int msgDecodedFunction = 0;
    int msgDecodedPayloadLength = 0;
    byte[] msgDecodedPayload = new byte[1];
    int msgDecodedPayloadIndex = 0;
    public int[] sensor = new int[5];

    public int[] getSensorValues() { return sensor; }

    public void CallDecodeMessage(byte c)
    {
        DecodeMessage(c);
    }

    private void SensorsPutData(byte[] payload)
    {
        for (int i = 0; i < payload.Length; i++)
        {
            sensor[i] = (int)payload[i];
        }
    }
    private void PositionReconstructData(byte[] c)
    {   
        timestamp = BitConverter.ToSingle(c, 0);
        xPosFromOdometry = BitConverter.ToSingle(c, 4);
        yPosFromOdometry = BitConverter.ToSingle(c, 8);
        angleRadianFromOdometry = BitConverter.ToSingle(c, 12);
        angleDegre = angleRadianFromOdometry * (-180) / 3.14159265359f;
        vitesseLineaireFromOdometry = BitConverter.ToSingle(c, 16);
        vitesseAngulaireFromOdometry = BitConverter.ToSingle(c, 20);
        Debug.WriteLine(timestamp);
    }
    private void DecodeMessage(byte c)
    {
        switch (rcvState)
        {
            case StateReception.Waiting:
                rcvState = StateReception.Waiting;
                if (c == 0XFE)
                    rcvState = StateReception.FunctionMSB;
                break;
            case StateReception.FunctionMSB:
                msgDecodedFunction = (c << 8);
                rcvState = StateReception.FunctionLSB;
                break;
            case StateReception.FunctionLSB:
                msgDecodedPayloadLength = 0;
                msgDecodedFunction |= c;
                rcvState = StateReception.PayloadLengthMSB;
                break;
            case StateReception.PayloadLengthMSB:
                msgDecodedPayloadLength = (c << 8);
                rcvState = StateReception.PayloadLengthLSB;
                break;
            case StateReception.PayloadLengthLSB:
                msgDecodedPayloadLength |= c;
                if (msgDecodedPayloadLength > 0)
                {
                    if(msgDecodedPayloadLength < 100) { //limit de la taille de payload
                        msgDecodedPayload = new byte[msgDecodedPayloadLength]; 
                        rcvState = StateReception.Payload;}
                    else { rcvState = StateReception.Waiting; }
                    
                }
                else
                {
                    rcvState = StateReception.CheckSum;
                }
                break;
            case StateReception.Payload:
                msgDecodedPayload[msgDecodedPayloadIndex++] = c;
                if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                {
                    rcvState = StateReception.CheckSum;
                    msgDecodedPayloadIndex = 0;
                }
                break;
            case StateReception.CheckSum:
                byte res = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                if (res == c)
                {
                    switch (msgDecodedFunction)
                    {
                        case 0x0061:
                            if(msgDecodedPayloadLength == 24) {PositionReconstructData(msgDecodedPayload); }
                            
                            break;
                        case 0x0030:
                            SensorsPutData(msgDecodedPayload);
                            break;
                        case 0x0063:
                            confPID(msgDecodedPayload);
                            break;
                        case 0x0091:
                            codeAsserv(msgDecodedPayload);
                            break;
                        //default:
                            //throw new Exception("Code non reconnu");
                    }
                    rcvState = StateReception.Waiting;
                }
                else
                {
                    rcvState = StateReception.Waiting;
                    Debug.WriteLine("CRC ERROR");
                }
                
                break;
            default:
                rcvState = StateReception.Waiting;
                break;
        }
    }
    byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
    {
        byte checksum = 0;
        checksum ^= 0xFE;
        checksum ^= (byte)(msgFunction >> 8);
        checksum ^= (byte)(msgFunction >> 0);
        checksum ^= (byte)(msgPayloadLength >> 8);
        checksum ^= (byte)(msgPayloadLength >> 0);


        for (int i = 0; i < msgPayload.Length; i++)
        {
            checksum ^= msgPayload[i];
        }
        return checksum;

    }

    public enum codeFunction
    {
        text = 0x0080,
        led1 = 0x0021,
        led2 = 0x0022,
        led3 = 0x0023,
        telemetreL = 0x0031,
        telemetreC = 0x0032,
        telemetreR = 0x0033,
        telemetreEL = 0x0034,
        telemetreER = 0x0035,
        vitesseL = 0x0041,
        vitesseC = 0x0042,
        positionData = 0x0062,
        confPID = 0x0063,
        vitLinRob = 0x0071,
        vitPolRob = 0x0072,
        codeAsserv = 0x0091,
        ghostData = 0x0010,
    }
    codeFunction rcvFunction;

    void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
    {
        rcvFunction = (codeFunction)msgFunction;
        switch (rcvFunction)
        {
            /*case codeFunction.text:
                textBoxReception.Text += "Texte reçu : " + Encoding.ASCII.GetString(msgPayload, 0, msgPayloadLength);
                break;
            case codeFunction.led1:
                led1.IsChecked = true;
                break;
            case codeFunction.led2:
                break;
            case codeFunction.led3:
                break;
            case codeFunction.telemetreL:
                IR_Gauche.Text = "IR Gauche : " + Encoding.ASCII.GetString(msgPayload, 0, msgPayloadLength) + " cm";
                break;
            case codeFunction.telemetreC:
                IR_Centre.Text = "IR Centre : " + Encoding.ASCII.GetString(msgPayload, 0, msgPayloadLength) + " cm";
                break;
            case codeFunction.telemetreR:
                IR_Droit.Text = "IR Droit : " + Encoding.ASCII.GetString(msgPayload, 0, msgPayloadLength) + " cm";
                break;
            case codeFunction.vitesseL:
                Vitesse_Gauche.Text = "Vitesse Gauche : " + Encoding.ASCII.GetString(msgPayload, 0, msgPayloadLength) + " %";
                break;
            case codeFunction.vitesseC:
                Vitesse_Centre.Text = "Vitesse Centre : " + Encoding.ASCII.GetString(msgPayload, 0, msgPayloadLength) + " %";
                break;*/
            //case codeFunction.RobotState: // so call me maybe, old code

            //    int timestamp = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16) + (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);
            //    string Text_Etat = "\nRobot State: " + ((StateRobot)msgPayload[0]).ToString() + " - " + timestamp.ToString() + " ms";
            //    robot.TextDebugAutoMode.Add(Text_Etat);
            //    while (robot.TextDebugAutoMode.Count() > 5)
            //    {
            //        robot.TextDebugAutoMode.RemoveAt(0);
            //    }
            //    break;


            /*case codeFunction.positionData:
                var Tab = msgPayload.Skip(0).Take(4).Reverse().ToArray();
                timestamp = BitConverter.ToUInt32(Tab, 0);
                positionXOdo = BitConverter.ToSingle(msgPayload, 4);
                positionYOdo = BitConverter.ToSingle(msgPayload, 8);
                angleRadOdo = BitConverter.ToSingle(msgPayload, 12);
                vitesseLineaire = BitConverter.ToSingle(msgPayload, 16);
                vitesseAngulaire = BitConverter.ToSingle(msgPayload, 20);

                // Affichage console
                //textBoxReception.Text = "PositionX: " + robot.positionXOdo.ToString();
                //textBoxReception.Text += "\nPositionY: " + robot.positionYOdo.ToString();
                //textBoxReception.Text += "\n" + robot.timestamp.ToString();

                // Affichage oscillo
                
                AddPointToLine(0, robot.positionXOdo, robot.positionYOdo);
                AddPointToLine(0, robot.timestamp, robot.vitesseLineaire);
                break;
*/
            case codeFunction.ghostData:
                var TabG = msgPayload.Skip(0).Take(4).Reverse().ToArray();
                timestamp = BitConverter.ToUInt32(TabG, 0);
                angleRadGhosto = BitConverter.ToSingle(msgPayload, 4);
                vitesseLineaireGhosto = BitConverter.ToSingle(msgPayload, 8);
                vitesseAngGhosto = BitConverter.ToSingle(msgPayload, 12);
                positionXGhosto = BitConverter.ToSingle(msgPayload, 16);
                positionYGhosto = BitConverter.ToSingle(msgPayload, 20);


                /*  // Affichage oscillo 
                  ghostOscilloPositiono.AddPointToLine(0, timestamp, angleRadGhosto);
                  ghostOscilloSpeed.AddPointToLine(0, timestamp, vitesseAngGhosto);
                */
                break;
              
            case codeFunction.confPID:
                kpX = BitConverter.ToSingle(msgPayload, 0);
                kiX = BitConverter.ToSingle(msgPayload, 4);
                kdX = BitConverter.ToSingle(msgPayload, 8);
                proportionnelleMaxX = BitConverter.ToSingle(msgPayload, 12);
                integralMaxX = BitConverter.ToSingle(msgPayload, 16);
                deriveeMaxX = BitConverter.ToSingle(msgPayload, 20);

                kpTheta = BitConverter.ToSingle(msgPayload, 24);
                kiTheta = BitConverter.ToSingle(msgPayload, 28);
                kdTheta = BitConverter.ToSingle(msgPayload, 32);
                proportionnelleMaxTheta = BitConverter.ToSingle(msgPayload, 36);
                integralMaxTheta = BitConverter.ToSingle(msgPayload, 40);
                deriveeMaxTheta = BitConverter.ToSingle(msgPayload, 44);

                /*
                textBoxReception.Text = "KpX = " + kpX + " kiX = " + kiX + " kdX = " + kdX + "\n" + "PropX = " +
                    proportionnelleMaxX + " IntX = " + integralMaxX + " DerivX = " + deriveeMaxX + "\n\n"

                    + "kpTheta = " + kpTheta + " kiTheta = " + kiTheta + " kdTheta = " + kdTheta + "\n" + "proportionnelleMaxTheta = " +
                    proportionnelleMaxTheta + " integralMaxTheta = " + integralMaxTheta + " deriveeMaxTheta = " + deriveeMaxTheta + "\n";
                */
                break;

            case codeFunction.codeAsserv:
                float consigneX = BitConverter.ToSingle(msgPayload, 0);
                float consigneTheta = BitConverter.ToSingle(msgPayload, 4);
                float consigneM1 = BitConverter.ToSingle(msgPayload, 8);
                float consigneM2 = BitConverter.ToSingle(msgPayload, 12);

                float valueX = BitConverter.ToSingle(msgPayload, 16);
                float valueTheta = BitConverter.ToSingle(msgPayload, 20);
                float valueM1 = BitConverter.ToSingle(msgPayload, 24);
                float valueM2 = BitConverter.ToSingle(msgPayload, 28);

                float errorX = BitConverter.ToSingle(msgPayload, 32);
                float errorTheta = BitConverter.ToSingle(msgPayload, 36);
                float errorM1 = BitConverter.ToSingle(msgPayload, 40);
                float errorM2 = BitConverter.ToSingle(msgPayload, 44);

                float commandX = BitConverter.ToSingle(msgPayload, 48);
                float commandtheta = BitConverter.ToSingle(msgPayload, 52);
                float commandM1 = BitConverter.ToSingle(msgPayload, 56);
                float commandM2 = BitConverter.ToSingle(msgPayload, 60);

                float corrPX = BitConverter.ToSingle(msgPayload, 64);
                float corrPTheta = BitConverter.ToSingle(msgPayload, 68);

                float corrIX = BitConverter.ToSingle(msgPayload, 72);
                float corrITheta = BitConverter.ToSingle(msgPayload, 76);

                float corrDX = BitConverter.ToSingle(msgPayload, 80);
                float corrDTheta = BitConverter.ToSingle(msgPayload, 84);
                break;
        }
    }

    void confPID(byte[] c)
    {
        kpX = BitConverter.ToSingle(c, 0);
        kiX = BitConverter.ToSingle(c, 4);
        kdX = BitConverter.ToSingle(c, 8);
        proportionnelleMaxX = BitConverter.ToSingle(c, 12);
        integralMaxX = BitConverter.ToSingle(c, 16);
        deriveeMaxX = BitConverter.ToSingle(c, 20);

        kpTheta = BitConverter.ToSingle(c, 24);
        kiTheta = BitConverter.ToSingle(c, 28);
        kdTheta = BitConverter.ToSingle(c, 32);
        proportionnelleMaxTheta = BitConverter.ToSingle(c, 36);
        integralMaxTheta = BitConverter.ToSingle(c, 40);
        deriveeMaxTheta = BitConverter.ToSingle(c, 44);
    }

    void codeAsserv(byte[] c)
    {
        consigneX = BitConverter.ToSingle(c, 0);
        consigneTheta = BitConverter.ToSingle(c, 4);
        consigneM1 = BitConverter.ToSingle(c, 8);
        consigneM2 = BitConverter.ToSingle(c, 12);

        valueX = BitConverter.ToSingle(c, 16);
        valueTheta = BitConverter.ToSingle(c, 20);
        valueM1 = BitConverter.ToSingle(c, 24);
        valueM2 = BitConverter.ToSingle(c, 28);

        errorX = BitConverter.ToSingle(c, 32);
        errorTheta = BitConverter.ToSingle(c, 36);
        errorM1 = BitConverter.ToSingle(c, 40);
        errorM2 = BitConverter.ToSingle(c, 44);

        commandX = BitConverter.ToSingle(c, 48);
        commandtheta = BitConverter.ToSingle(c, 52);
        commandM1 = BitConverter.ToSingle(c, 56);
        commandM2 = BitConverter.ToSingle(c, 60);

        corrPX = BitConverter.ToSingle(c, 64);
        corrPTheta = BitConverter.ToSingle(c, 68);

        corrIX = BitConverter.ToSingle(c, 72);
        corrITheta = BitConverter.ToSingle(c, 76);

        corrDX = BitConverter.ToSingle(c, 80);
        corrDTheta = BitConverter.ToSingle(c, 84);
    }            

}