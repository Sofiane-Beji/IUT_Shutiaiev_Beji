using RobotInterface;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;





public class reception
    {


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
    byte[] msgDecodedPayload;
    int msgDecodedPayloadIndex = 0;

    public void CallDecodeMessage(byte c) { DecodeMessage(c); }

    private void DecodeMessage(byte c)
    {
        switch (rcvState)
        {
            case StateReception.Waiting:
                rcvState = StateReception.Waiting;
                if(c == 0XFE)
                    rcvState = StateReception.FunctionMSB;
                break;
            case StateReception.FunctionMSB:
                msgDecodedFunction |= (c << 8);
                rcvState = StateReception.FunctionLSB;
                break;
            case StateReception.FunctionLSB:
                msgDecodedFunction |= c;
                rcvState = StateReception.PayloadLengthMSB;
                break;
            case StateReception.PayloadLengthMSB:
                msgDecodedPayloadLength |= (c << 8);
                rcvState = StateReception.PayloadLengthLSB;
                break;
            case StateReception.PayloadLengthLSB:
                msgDecodedPayloadLength |= c;
                if (msgDecodedPayloadLength > 0)
                {
                    msgDecodedPayload = new byte[msgDecodedPayloadLength];
                    rcvState = StateReception.Payload;
                }
                else 
                {
                    rcvState = StateReception.CheckSum;
                }
                break;
            case StateReception.Payload:
                msgDecodedPayload[msgDecodedPayloadIndex++] = c;
                if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                    rcvState = StateReception.CheckSum;
                msgDecodedPayloadIndex = 0;
                break;
            case StateReception.CheckSum:
                
                if (CalculateChecksum(msgDecodedFunction, msgDecodedPayloadIndex, msgDecodedPayload) == c)
                {
                    
                    rcvState = StateReception.Waiting;
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

}

