using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

public class Send : MonoBehaviour {

    public string IP = "192.168.12.139";
    public int port = 5560;
    IPEndPoint endPoint;
    UdpClient client;

    // Use this for initialization
    void Start () {
        endPoint = new IPEndPoint(IPAddress.Parse(IP), port);
        client = new UdpClient();
    }
    
    public void sendData(string finger, bool onOrOff){
        Debug.Log("Name of finger" + finger);
        
        try{
            string message = finger + ":" + Convert.ToInt32(onOrOff);
            byte[] data = Encoding.UTF8.GetBytes(message);
            client.Send(data, data.Length, endPoint);
        }
        catch(Exception e){
            Debug.Log(e.ToString());
        }
    }

    void OnApplicatonQuit () {
        client.Close();
    }
}
