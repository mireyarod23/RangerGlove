using UnityEngine;
using System;
using System.Net.Sockets;
using System.IO;
using DG.Tweening;

public class MovementFinger : MonoBehaviour {

    public GameObject[] fingers;

    //the name of the connection, not required but better for overview if you have more than 1 connections running
    //public string conName = "Localhost";

    //ip/address of the server, 127.0.0.1 is for your own computer
    public string conHost = "10.0.0.15";

    //port for the server, make sure to unblock this in your router firewall if you want to allow external connections
    public int conPort = 80;

    //a true/false variable for connection status
    public bool socketReady = false;

    public Transform wristQuad;

    TcpClient mySocket;
    NetworkStream theStream;
    StreamWriter theWriter;
    StreamReader theReader;

    private string[] fingerMovement;
    private string[] handMovement;

    float x;
    float y;
    float z;
    float w;

    void Start()
    {
        setupSocket();
    }

    void FixedUpdate()
    {
        while (theStream.DataAvailable)
        {
            string recieveData = readSocket();
            //string[] rotate = recieveData.Split(',');
            //Debug.Log("Data " + recieveData)
            if (recieveData != null || recieveData != " ")
            {
                string[] splitePackage = recieveData.Split(Environment.NewLine.ToCharArray());

                //Debug.Log("SplitPackage " + splitePackage[splitePackage.Length - 1]);
                if (splitePackage != null || splitePackage.Length > 0)
                {
                    string[] spliteValues = splitePackage[splitePackage.Length - 1].Split('/');
                        
                    if (spliteValues.Length >= 2)
                    {
                        fingerMovement = spliteValues[0].Split(',');
                        handMovement = spliteValues[1].Split(',');
  
                        //Debug.Log(spliteValues[0] + "---" + spliteValues[1]);

                        for (int i = 0; i < fingers.Length; i++)
                        {
                            fingers[i].transform.DOLocalRotate(new Vector3(0, 0, int.Parse(fingerMovement[i])), 0);
                           
                            x = float.Parse(handMovement[0]);
                            y = float.Parse(handMovement[1]);
                            z = float.Parse(handMovement[2]);
                            w = float.Parse(handMovement[3]);

                 
                            Quaternion quaternion = new Quaternion(-y, -z, x, w); 
                         
                            wristQuad.DORotateQuaternion(quaternion, 0f);
                        }
                    }
                }
            }
        }
    }


                            //Quaternion quaternion = new Quaternion(y, -z, -x, w); 

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
    public void writeSocket(string finger, int onOrOff)
    {
        if (!socketReady)
            return;
        string message = finger + ":" + Convert.ToInt32(onOrOff) + "&";
        //String tmpString = theLine + "\r\n";
        Debug.Log(message);
        theWriter.Write(message);
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
            theStream.Flush(); //????
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