/* Slave in write mode*/

#include<avr/io.h>

// To work in slave mode , initialise TWI to send and receive data.
// In salve mode we cannot send START and STOP conditions and also slave needs to wait until master address it, it should listen to the bus.
// Set the slave address by setting the values for the TWAR, Enable TWI module and acknowledge generation by setting TWEN,TWINT,TWEA bit to 1 


void i2c_initslave (unsigned char slaveaddress)
{
TWCR = 0x04;  // enable TWI module by setting TWEN flag 
TWAR = slaveaddress;  // set slave address
TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // init TWI module 

}

// after initialisimg TWI module, slave should listen to bus to be addressed by master. When TWI module of receiver detects its address on line 
// it returns ACK and sets TWINT flag to 1. That is why we should poll the TWINT flag to see when the slave is addressed.

void i2c_listen()
{
 while ((TWCR & ( 1 << TWINT ) ) == 0); // wait to be addressed by master
}

// After being address by the master device for read , we should : 1.Copy the data to TWDR , 2.Set to TWINT,TWEN,TWEA to 1 to start sending the byte
// poll the TWINT to ensure  when byte is completely transmitted.

void i2c_send(unsigned char data)
{ 
TWDR = data ; // copy data to TWDR
TWCR = ( 1 << TWINT ) | ( 1 << TWEN ); // start transmission
 while ((TWCR & ( 1 << TWINT ) ) == 0) ; // wait to complete
}

int main(void)
{
i2c_initslave(0b11010001); // initiate  TWI module as slave with address = 0b11010001
i2c_listen(); // listen to be addressed 
i2c_send('E'); //transmitt letter 
while(1); // stay here forever
return 0; 
}