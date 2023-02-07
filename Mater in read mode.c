/* Master in Read Mode*/

#include<avr/io.h>

// To initalise the module in master mode do following steps : 1. Set TWI clock freq. by setting values of TWBR register and TWPS bits in TWSR register.
// 2. Eable TWI by setting TWEN bit in TWCR reg.

void i2c_init (void)
{
TWSR = 0X00; // set prescaler bits to zero
TWBR = 0x47; // SCL frequency is 50K for XTAL = 8Mhz
TWCR = 0x04; // enable TWI module
}


// Transmit START condition : to start data transfer in master operating mode, we must transmit START condition. This done by setting TWEN,TWSTA and TWINT bits in TWCR set to 1.
// setting TWEN means we are enabling the TWI,setting TWSTA means we tell TWI to initiate START when bus is free, Setting TWINT means it clears to interrupt flag to START condition
// we should  poll the TWINT flag to check whether START condition transmitted completely.

void i2c_start(void)
{
 TWCR = ( 1 << TWINT ) | ( 1 << TWSTA ) | ( 1 << TWEN );
 while ((TWCR & ( 1 << TWINT ) ) == 0) ;
}


// To send a byte of data after START we should do : 1. copy data to TWDR register 2. set TWINT and TWEN bits of TWCR to 1 to start sending the data
// 3. poll the TWINT flag of TWCR to see whether data biyte transmitted completely. 

void i2c_write(unsigned char data)
{
 TWDR = data ;
 TWCR = (1<<TWINT) | (1<<TWEN); // enable the transmission
 while ((TWCR & ( 1 << TWINT ) ) == 0); // polling e.g checking TWINT flag to check whether START condition transmitted succesfully
}
 

// To receive a byte of data after transmitting Slave address from above function , follow these steps
// 1. set TWEN and TWEA bits of TWCR register to 1 to receive the byte of data. if u want to return ACK after receiving data then set TWINT flag
// 2. poll the TWINT 3. copy the data from byte data from TWDR to some other local variable here i .

unsigned char i2c_read(unsigned char last)
{
 if ( last == 0 ) // If want to read more than 1 byte
     TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
 else             // else read only one byte
 TWCR = (1<<TWINT) | (1<<TWEN);
 while ((TWCR & ( 1 << TWINT ) ) == 0);
 return TWDR ; 
}

// To stop data transfer, we must tramitt STOP condition this is done by seeting TWINT,TWEN,TWSTO reg.We don't poll at this function as bus is being free after this condition.

void i2c_stop()
{
TWCR = ( 1 <<TWINT )|( 1 << TWEN) | ( 1<< TWSTO);
}


int main(void)
{
unsigned char i = 0 ;
DDRD = 0xFF; // PORTD is output
i2c_init(); // initialse TWI for master mode
i2c_start(); // transmit start condition
i2c_write(0b11010001); // transmitt Slave address + R(1) (SLA+R means to master is reading )
i = i2c_read(1); // read only 1 byte 
PORTD= i; // show byte in PORTD
i2c_stop(); // transmitt stop condition
while(1); 
return 0;  
}	
				
					
