/* File:   I2C.h */
#ifndef I2C_H
#define	I2C_H
void I2C_Init(const unsigned long);
void I2C_Wait(void);
void I2C_Start(void);
void I2C_ReStart(void);
void I2C_Stop(void);
char I2C_Read(char);
void I2C_Write(unsigned char);
void I2C_Write_Cmd_Only(char, char);  
void I2C_Write_Cmd_Write_Data(char, char, char);
void I2C_Write_Address_Write_One_Byte(char, char, char);    
BYTE I2C_Write_Address_Read_One_Byte(char, char);
void i2cHighSda(void);
void i2cLowSda(void);
void i2cHighScl(void);
void i2cLowScl(void);
void i2cNack(void);
void i2cAck(void);

#define SCL_PIN PORTDbits.RD6   // configure portD bit 6 for SCL_PIN
#define SCL_DIR TRISDbits.RD6   // configure portD bit 6 for SCL_DIR
#define SDA_PIN PORTDbits.RD7   // configure portD bit 7 for SDA_PIN
#define SDA_DIR TRISDbits.RD7   // configure portD bit 7 for SDA_DIR
#endif	/* I2C_H */

