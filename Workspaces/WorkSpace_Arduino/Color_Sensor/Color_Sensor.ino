#include <Arduino_FreeRTOS.h>

#define RS485_Master_Pin  2


// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );

long Nb_Messages_recus = 0;
long Nb_Erreurs_com = 0;


/**************************************************
Definition des commandes de communication
 **************************************************/
enum Com_Instruction
{
  ACKNOWLEDGE = 0,

  //Destination Robot
  DESTINATION_ROBOT = 10,
  DESTINATION_ROBOT_SPLINE_CUBIQUE,

  //Position Robot
  SET_ROBOT_POSITION = 20,
  POSITION_AUTRE_ROBOT,

  //Servos et AX12
  DESTINATION_SERVOS_AND_AX12 = 30,

  //Info sur la carte
  DEFINITION_ID_ROBOT = 40,       //Permet à la carte ID de donner l'ID du robot sur les autres cartes
  DEMANDE_INFO,         //Demande les infos d'une carte
  REPONSE_INFO_Multi_FCT,         //Toutes les infos d'une carte dans un seul message
  REPONSE_INFO_IA,                //Toutes les infos de la carte IA

  //Power
  DEMANDE_MOTEURS_POWER = 50,
  DEMANDE_MOTEURS_AUX_POWER,
  DEMANDE_SERVO_POWER,
  DEMANDE_AX_12_POWER,
  DEMANDE_SIMULATION_MOTEURS,
  DEMANDE_SIMULATION_MOTEURS_AUX,

  //Parametres
  PARAMETRES_ODOMETRIE = 60,
  PARAMETRES_PID,

  //Vitesse du robot
  VITESSE_ROBOT = 70,
  VITESSE_MOTEURS_AUX,

  //Bloc 70 reserve pour la strategie
  STRATEGIE_CHANGEMENT_ETAT = 80,
  //STRATEGIE_ENSEMBLE_ACTIONS,
  //STRATEGIE_DIALOGUE_ENTREROBOTS,
  //STRATEGIE_ETAT_INTERNE,

  //Logger
  LOGGER_DEBUG = 90,

  //ASTAR
  ASTAR_CONTENU = 100,
  ASTAR_VECTEURS,

  //Graphiques
  GRAPHIQUES_ADD_DATA = 110,
  GRAPHIQUES_CLEAR,

  //Balises
  BALISE_START = 200,         //Start moteur et lasers
  BALISE_POSITION,            //Position du robot donnant la position centrale de la balise
  BALISE_POSITION_ADVERSAIRES,       //Retour sur la position des adversaires detectée par la balise

  //Bootloader
  BOOTLOADER = 240,
  BOOTLOADER_ACK,

  //Lecture en Flash
  FLASH_CHANGE_FILE = 250,    //Demande un changement de fichier à l'IHM lors de l'enregistrement

  PING = 252,
  PONG,

  //End Communication
  END_COMMUNICATION = 254,

  //Test
  TEST_COMMUNICATION = 255,
};


/**************************************************
Definition du nombre maximum de donnees dans une communication
 **************************************************/
#define COMMUNICATION_TRAME_MAX_DATA  80

/**************************************************
Definition des adresses des modules Xbee
 **************************************************/
enum Adress_Xbee
{
  Xbee_address_Gros_Robot = 0x01,
  Xbee_address_Petit_Robot,
  Xbee_address_Balise_Gros_Robot,
  Xbee_address_Balise_Petit_Robot,
  XBee_PC = 0x0A,
  ALL_XBEE = 0xFFFF
};


/**************************************************
Definition des adresses des escalves
 **************************************************/
enum Slave_Adresses
{
  INTERFACE_PC = -4,
  BALISE_BOARD = -3,
  PC = -2,
  ALL_CARDS = -1,
  IA_BOARD = 0
};


/**************************************************
Declaration de la definition de la Structure de communication contenant un message
 **************************************************/
struct Communication_Trame
{
  enum Com_Instruction Instruction;                           //Instruction a echanger

  enum Slave_Adresses Slave_Adresse;                          //Adresse de la carte ciblée //ALL_CARDS (-1) pour toutes les cartes

  byte Length;                                              //Nombre d'octets dans le tableau de data

  byte Data[COMMUNICATION_TRAME_MAX_DATA];

  enum Adress_Xbee XBEE_DEST_ADDR;              //Adresse de la cible Xbee en mode API
};

struct Communication_Trame received_trame;

void setup() 
{  
  Serial.begin(2000000);

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(RS485_Master_Pin, OUTPUT);
  digitalWrite(RS485_Master_Pin, LOW);    // turn RS485 Slave Mode
  
  /*xTaskCreate(
    TaskBlink
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );*/

    xTaskCreate(
    TaskRxData
    ,  "RXdata"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );
}


void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;  

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskRxData(void *pvParameters)  // This is a task.
{
  (void) pvParameters;  

  struct Communication_Trame trame_RX;

  for (;;) // A Task shall never return or exit.
  {
    if(Serial.available() > 60)
    {
      Serial.println("Rx buffer full");  // print as an ASCII-encoded decimal
    }

    
    if(Serial.available() > 11)
    {
      _1_Communication_Create_Trame_From_Buffer();
      Serial.print("Messages recus: ");
      Serial.print(Nb_Messages_recus, DEC);  // print as an ASCII-encoded decimal
      Serial.print(" Erreurs recues: ");
      Serial.println(Nb_Erreurs_com, DEC);  // print as an ASCII-encoded decimal

      vTaskDelay( 16 / portTICK_PERIOD_MS ); // wait for one second
    }
  }
}

/*****************************************************************************
 ** Function name:    _1_Communication_Create_Trame_From_Buffer
 **
 ** Descriptions:   Fonction de lecture d'un Buffer afin d'en refaire une trame
 **
 ** parameters:     Pointeur vers le Buffer
 **           Queue à la quelle ajouter le message
 ** Returned value:   None
 **
 *****************************************************************************/
byte Data_rx[COMMUNICATION_TRAME_MAX_DATA + 11];
BaseType_t _1_Communication_Create_Trame_From_Buffer(void)
{
  byte API_start = 0;
  static short API_LENGTH = 0;
  short crc = 0;
  byte index = 0, rx_crc = 0;

  //Reception En-tête API
  static byte boucle = 0;
  API_start = Serial.read();

  boucle = 0;
  while (API_start != 0x7E)
  {    
    API_start = Serial.read();
    boucle++;
    if(boucle > 10)
    {
      Nb_Erreurs_com++;
      Serial.println("Erreur_Step 1");  
      return false;
    }
  }

  //Le start byte est recu, on demarre la lecture de la trame
  Serial.readBytes(Data_rx, 3);

  API_LENGTH = Data_rx[0];
  API_LENGTH <<= 8;
  API_LENGTH += Data_rx[1];

  if (API_LENGTH > COMMUNICATION_TRAME_MAX_DATA + 11)
  {
    Nb_Erreurs_com++;
    Serial.println("Erreur_Step 3");
    return false;
  }

  //Packet Type
  crc += Data_rx[2];
  if (Data_rx[2] == 0x89)
  {
    Serial.readBytes(Data_rx, 3);
    //Dummy Read 3 bytes
    return false;
  }

  boucle = 0;
  while (Serial.available() < 6)
  {
    boucle++;
    if (boucle > 10)
    {
      Nb_Erreurs_com++;
      Serial.print("Erreur_Step 4:");
      Serial.print(Serial.available(), DEC);  // print as an ASCII-encoded decimal
      Serial.print("/");
      Serial.print(6, DEC);  // print as an ASCII-encoded decimal
      Serial.println(" octets dispo");
      return false;
    }
    vTaskDelay( 16 / portTICK_PERIOD_MS );
  }

  Serial.readBytes(Data_rx, 6);
  for(int i = 0; i < 4; i++)
  {
    crc += Data_rx[i];
  }

  /*//Tx Address
  FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
  crc += API_DUMMY;

  FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
  crc += API_DUMMY;

  //RSSI
  FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
  crc += API_DUMMY;

  //Option
  FIFO_READ_ELEMENT(Fifo, &API_DUMMY);
  crc += API_DUMMY;*/

  //Reception datas
  received_trame.Instruction = Data_rx[4];
  crc += (byte)received_trame.Instruction;

  received_trame.Slave_Adresse = Data_rx[5];
  crc += (byte)received_trame.Slave_Adresse;

  received_trame.Length = (byte)(API_LENGTH - 7);

  if (received_trame.Length > COMMUNICATION_TRAME_MAX_DATA)
  {
    Nb_Erreurs_com++;  
    Serial.println("Erreur_Step 5");
    return false;
  }
  
  boucle = 0;
  while (Serial.available() < received_trame.Length + 1)
  {
    boucle++;

    if (boucle > 10)
    {
      Nb_Erreurs_com++;
      Serial.println("Erreur_Step 6");
      return false;
    }
    vTaskDelay( 16 / portTICK_PERIOD_MS );
  }

  Serial.readBytes(received_trame.Data, received_trame.Length);

  //Reception des data
  for (index = 0; index < received_trame.Length; index++)
  {
    crc += (byte)(received_trame.Data[index]);
  }

  //API CRC
  while (Serial.available() == 0)
  { 
    boucle++;
    if (boucle > 10)
    {
      Nb_Erreurs_com++;
      Serial.println("Erreur_Step 7");
      return false;
    }
    vTaskDelay( 16 / portTICK_PERIOD_MS );
  }
  rx_crc = Serial.read();

  //Contrôle CRC
  crc &= 0xFF;
  crc = (byte)(0xFF - crc);

  //Vérifie le CRC
  if (crc == rx_crc)
  {
    Nb_Messages_recus++;
    //Vérifie l'adressage du message
    //_1_Communication_Check_Rx_Adresse(&received_trame);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   // turn the LED on (HIGH is the voltage level)
    return true;
  }else
  {
    Nb_Erreurs_com++;
    Serial.print("Erreur_Step 8/ ");
    Serial.print(crc, DEC);  // print as an ASCII-encoded decimal
    Serial.print("/");
    Serial.print(rx_crc, DEC);  // print as an ASCII-encoded decimal
    Serial.println(" crc/rx_crc");
    return false;
  }    
}

void loop() {
  // put your main code here, to run repeatedly:

}
