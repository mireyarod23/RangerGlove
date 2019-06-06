using UnityEngine;

//ADD to the objects that the hand will be collide into
public class FingerCollision : MonoBehaviour
{


    //Include the object that includes the MovementFinger
    public MovementFinger SendObject;
    public GameObject[] fingers;

    void Start()
    {
        for (int i = 0; i < fingers.Length; i++){
            SendObject.writeSocket(fingers[i].transform.parent.name, 0);
        }
    }

    ////On collusion enter 
    void OnCollisionEnter(Collision col)
    {
        //Debug.Log(col.gameObject.name); 
        if (col.gameObject.tag == "finger")
        {
            SendObject.writeSocket(col.gameObject.transform.parent.name, 100);
        }
    }

    void OnCollisionExit(Collision col)
    {
        //Debug.Log(col.gameObject.name);
        if (col.gameObject.tag == "finger")
        {
            SendObject.writeSocket(col.gameObject.transform.parent.name, 0);
        }

    }
}
