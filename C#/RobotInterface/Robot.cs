using System;


public class Robot
{
    public Queue<byte> byteListReceived = new Queue<byte>();
    public string receivedText = "";
    public float distanceTelemetreDroit;
    public float distanceTelemetreCentre;
    public float distanceTelemetreGauche;

    public float timestamp = 0.0f;
    public float xPosFromOdometry = 0.0f;
    public float yPosFromOdometry = 0.0f;
    public float angleRadianFromOdometry;
    public float vitesseLineaireFromOdometry;
    public float vitesseAngulaireFromOdometry;
    public Robot()
    {

    }
}