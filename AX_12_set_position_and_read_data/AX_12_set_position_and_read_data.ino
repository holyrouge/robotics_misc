/*
 * SBU Robot Design Team
 * 
 * Implements a way to control AX 12 servo by sending it 
 * Serial commands.
 * 
 * Author: Sasha Sokolov
 * Date: 9/28/2017
 */
 
void setup() 
{
  Serial1.begin(1000000);                       //Preparing Serial1, Serial2, and Serial pins with baud rate of 1000000
  Serial2.begin(1000000);                           
  Serial.begin(1000000);
}

void loop() 
{
  while(!Serial.available()){}
  int in = Serial.parseInt() % 1024;            //Reading desired position (making sure its 0-1023)
  byte a = lowByte(in);                         //Low byte of position
  byte b = highByte(in);                        //High byte of position
  byte data[] = {255, 255, 4, 5, 3, 30, a, b};  //Data to be sent
  
  /*
   *Values: 
   *1)Start byte
   *2)Start byte
   *3)ID
   *4)Length (includes everything to be sent after it (including checksum)
   *5)Instruction (3 is to write)
   *6)Start address to write to (30 and 31 are for postion)
   *7)First byte of position (written to 30)
   *8)Second byte of position (written to 31)
   */

  sendData(data, 8);                            //8 is the length of the data array
}

void sendData(byte data[], int len)
{
  byte sum = 0;
  for (int i = 2; i < len; i++)
  {
    sum += data[i];                             //Adding up the data values to get checksum
  }

  Serial1.begin(1000000);                       //Preparing Serial1 pin with baud rate of 1000000
  Serial1.write(data, len);                     //Sending the data
  Serial1.write(~sum);                          //Sending the checksum
  Serial1.end();                                //Disabling Serial1 so servo can respond
  getData();
}

void getData()
{
  while(Serial2.available())
  {
    Serial.println(Serial2.read());             //Reading echo
  }

  while(!Serial2.available()){}                 //Waiting for status packet

  while(Serial2.available())
  {
    Serial.println(Serial2.read());             //Reading status packet (sometimes has extra 0 at the end)
  }                                                                             //NEED TO FIGURE OUT WHY!!!
  Serial1.begin(1000000);                       //Reanabling Serial1 pin
}

