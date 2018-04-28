/*
 * SBU Robot Design Team
 * 
 * Implements a way to control AX 12 servo.
 * 
 * Author: Sasha Sokolov
 * Date: 10/9/2017                                                PUT RIGHT TIME STAMP
 */
 
void setup() 
{
  Serial1.begin(1000000);                       //Preparing Serial1 pin with baud rate of 1000000
  Serial2.begin(1000000);                           
  Serial.begin(115200);
}
void loop() 
{
  Serial.flush();
  Serial.println("What ID do you want to communicate with? (254 for  Broadcast ID; don't use 253)");
  while(!Serial.available()){}
  int id = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  
  if(id < 255 && id > -1 && id != 253)
  {
    Serial.print("ID: ");
    Serial.println(id);
    
    Serial.println("\nWhat command would you like to send?");
    Serial.println("\t1: PING");
    Serial.println("\t2: READ DATA");
    Serial.println("\t3: WRITE DATA");
    Serial.println("\t4: REG WRITE");
    Serial.println("\t5: ACTION");
    Serial.println("\t6: RESET");
    Serial.println("\t83: SYNC WRITE");
    Serial.println("\tanything else: Cancel (this transmition)");
    while(!Serial.available()){}
    int comm = Serial.parseInt();                   //takes letters as 0, so don't type in letters
    if(comm == 1)
    {
      ping(id);
    }
    else if(comm == 2)
    {
      readD(id);
    }
    else if(comm == 3)
    {
      writeD(id);
    }
    else if(comm == 4)
    {
      regW(id);
    }
    else if(comm == 5)
    {
      action(id);
    }
    else if(comm == 6)
    {
      reset(id);
    }
    else if(comm == 83)
    {
      syncW(id);
    }
  }
  else
  {
    Serial.println("INVALID ID");
  }
}
void ping(int id)
{
  Serial.println("Pinging...");
  byte data[] = {255, 255, id, 2, 1};
  sendData(data, sizeof(data));
}
void readD(int id)
{
  Serial.println("\nWhat address would you like to start reading from? (0-49, don't use 10 or 45)");
  while(!Serial.available()){}
  int add = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Start address: ");
  Serial.println(add);
  
  Serial.println("How many addresses do you want to read?");
  while(!Serial.available()){}
  int num = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Number of addresses to read: ");
  Serial.println(num);
  Serial.println("Reading...");
  
  byte data[] = {255, 255, id, 4, 2, add, num};
  sendData(data, sizeof(data));
}
void writeD(int id)
{
  Serial.println("\nWhat address would you like to start writing to? (0-49, don't use 10 or 45)");
  while(!Serial.available()){}
  int add = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Start address: ");
  Serial.println(add);
  
  Serial.println("How many addresses do you want to write to? (0-49, don't use 0, 1, 2, 10, 20, 21, 22, 23, 36, 37, 38, 39, 40, 41, 42, 43, 45, or 46)");
  while(!Serial.available()){}
  int num = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Number of addresses to write to: ");
  Serial.println(num);
  byte data[num + 6] = {255, 255, id, num + 3, 3, add};
  Serial.println("Enter first number");
  for(int i = 0; i < num; i++)
  {
    while(!Serial.available()){}
    data[6 + i] = Serial.parseInt();            //takes letters as 0, so don't type in letters      
    Serial.println(data[6 + i]);
    if(i < num - 1)
    {
      Serial.println("next");
    }
  }
  
  Serial.println("Writing...");
  sendData(data, sizeof(data));
}
void regW(int id)
{
  Serial.println("\nWhat address would you like to start writing to? (0-49, don't use 10 or 45)");
  while(!Serial.available()){}
  int add = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Start address: ");
  Serial.println(add);
  
  Serial.println("How many addresses do you want to write to? (0-49, don't use 0, 1, 2, 10, 20, 21, 22, 23, 36, 37, 38, 39, 40, 41, 42, 43, 45, or 46)");
  while(!Serial.available()){}
  int num = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Number of addresses to write to: ");
  Serial.println(num);
  byte data[num + 6] = {255, 255, id, num + 3, 4, add};
  Serial.println("Enter first number");
  for(int i = 0; i < num; i++)
  {
    while(!Serial.available()){}
    data[6 + i] = Serial.parseInt();            //takes letters as 0, so don't type in letters      
    Serial.println(data[6 + i]);
    if(i < num - 1)
    {
      Serial.println("next");
    }
  }
  
  Serial.println("Reg Writing...");
  sendData(data, sizeof(data));
}
void action(int id)
{
  Serial.println("action!");
  byte data[] = {255, 255, id, 2, 5};
  sendData(data, sizeof(data));
}
void reset(int id)
{
  Serial.println("resetting...");
  byte data[] = {255, 255, id, 2, 6};
  sendData(data, sizeof(data));
}
void syncW(int id)
{
  Serial.println("\nHow many ID's do you want to write to?");
  
  while(!Serial.available()){}
  int ids = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Number of ID's to write to:");
  Serial.println(ids);
  
  Serial.println("What address would you like to start writing to? (0-49, don't use 10 or 45)");
  while(!Serial.available()){}
  int add = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Start address: ");
  Serial.println(add);
  
  Serial.println("How many addresses do you want to write to? (0-49, don't use 0, 1, 2, 10, 20, 21, 22, 23, 36, 37, 38, 39, 40, 41, 42, 43, 45, or 46)");
  
  while(!Serial.available()){}
  int num = Serial.parseInt();                   //takes letters as 0, so don't type in letters
  Serial.print("Number of addresses to write to: ");
  Serial.println(num);
  byte data[(num + 1) * ids + 7] = {255, 255, id, (num + 1) * ids + 4, 83, add, num};
  for(int i = 0; i < ids; i++)
  {
    Serial.println("Enter ID to write to");
    while(!Serial.available()){}
    data[7 + ((num + 1) * i)] = Serial.parseInt();            //takes letters as 0, so don't type in letters      
    Serial.println(data[7 + ((num + 1) * i)]);
    for(int j = 0; j < num; j++)
    {
      Serial.println("\tEnter data");
      while(!Serial.available()){}
      data[7 + ((num + 1) * i) + 1 + j] = Serial.parseInt();            //takes letters as 0, so don't type in letters      
      Serial.print("\t");
      Serial.println(data[7 + ((num + 1) * i) + 1 + j]);
    }
  }
  
  Serial.println("Sync Writing...");
  sendData(data, sizeof(data));
}
void sendData(byte data[], int len)
{
  byte sum = 0;
  for (int i = 2; i < len; i++)
  {
    sum += data[i];                             //Adding up the data values to get checksum
  }
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
    Serial.println(Serial2.read());             //Reading status packet
  }
  Serial.println();
  Serial1.begin(1000000);                       //Preparing Serial1 pin with baud rate of 1000000
}
