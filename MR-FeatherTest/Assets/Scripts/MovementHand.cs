using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using DG.Tweening;


public class MovementHand : MonoBehaviour {


    //the name of the connection, not required but better for overview if you have more than 1 connections running
    //public string conName = "Localhost";

    //ip/address of the server, 127.0.0.1 is for your own computer
    public string conHost = "10.0.0.15";

    //port for the server, make sure to unblock this in your router firewall if you want to allow external connections
    public int conPort = 80;

    //a true/false variable for connection status
    public bool socketReady = false;

    public Transform indexFinger;

    TcpClient mySocket;
    NetworkStream theStream;
    StreamWriter theWriter;
    StreamReader theReader;

    string contX = "x:";
    string contY = "y:";
    string contZ = "z:";

    float x;
    float y;
    float z;
    float w;
    float indexValue;

    void Start(){
        setupSocket();
    }

    void FixedUpdate()
    {
        while (theStream.DataAvailable)
        {
            string recieveData = readSocket();
            //string[] rotate = recieveData.Split(',');
            Debug.Log("Data " + recieveData);

            string[] splitePackage = recieveData.Split(Environment.NewLine.ToCharArray());

            //Debug.Log("SplitPackage " + splitePackage[splitePackage.Length - 1]);

            string[] spliteValues = splitePackage[splitePackage.Length - 1].Split(',');

            if (spliteValues.Length == 4)
            {
                x = float.Parse(spliteValues[0]);
                y = float.Parse(spliteValues[1]);
                z = float.Parse(spliteValues[2]);
                w = float.Parse(spliteValues[3]);

                //transform.eulerAngles = new Vector3(x, y, z);

                Quaternion quaternion = new Quaternion(-y, -z, x, w);
                transform.DORotateQuaternion(quaternion, 0f);
            }
        }
    }

    //try to initiate connection
    public void setupSocket()
    {
        try
        {
            mySocket = new TcpClient(conHost, conPort);
            theStream = mySocket.GetStream();
            theWriter = new StreamWriter(theStream);
            theReader = new StreamReader(theStream);
            socketReady = true;
        }
        catch (Exception e)
        {
            Debug.Log("Socket error:" + e);
        }
    }

    //send message to server
    public void writeSocket(string theLine)
    {
        if (!socketReady)
            return;
        String tmpString = theLine + "\r\n";
        theWriter.Write(tmpString);
        theWriter.Flush();
    }

    //read message from server
    public string readSocket()
    {
        String result = "";
        if (theStream.DataAvailable)
        {
            Byte[] inStream = new Byte[mySocket.SendBufferSize];
            theStream.Read(inStream, 0, inStream.Length);
            result = System.Text.Encoding.UTF8.GetString(inStream);
            //theStream.Flush(); //????
        }
        return result;
    }

    //disconnect from the socket
    public void closeSocket()
    {
        if (!socketReady)
            return;
        theWriter.Close();
        theReader.Close();
        mySocket.Close();
        socketReady = false;
    }

    //keep connection alive, reconnect if connection lost
    public void maintainConnection()
    {
        if (!theStream.CanRead)
        {
            setupSocket();
        }
    }


}