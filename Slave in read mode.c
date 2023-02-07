/* Slave in Read mode*/

#include<avr/io.h>



void i2c_initslave (unsigned char slaveaddress)
{
TWCR = 0x04;  // enable TWI module by setting TWEN flag 
TWAR = slaveaddress;  // set slave address
TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // init TWI module 

}



void i2c_listen()
{
 while ((TWCR & ( 1 << TWINT ) ) == 0); // wait to be addressed by master
}



unsigned char i2c_read(unsigned char last)
{
 if ( last == 0 ) // If want to read more than 1 byte
     TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
 else             // else read only one byte
 TWCR = (1<<TWINT) | (1<<TWEN);
 while ((TWCR & ( 1 << TWINT ) ) == 0);
 return TWDR ; 
}

int main(void)
{

DDRD = 0xFF; // PORTD is output
i2c_initslave(0b11010001); // initiate  TWI module as slave with address = 0b11010001
i2c_listen(); // listen to be addressed 
PORTD = i2c_read(1); // read only 1 byte 
while(1); 
return 0; 
}