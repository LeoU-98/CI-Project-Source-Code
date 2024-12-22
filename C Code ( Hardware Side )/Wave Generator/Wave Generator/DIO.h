
#ifndef DIO_H_
#define DIO_H_

void DIO_vsetPINDir(char portname,char pin,char direction);
void DIO_write_pin(char portname,char pin,char value);
unsigned char DIO_u8read(char portname,char pinnumber);
void DIO_toggle_pin(unsigned char portname,unsigned char pinnumber);
void DIO_set_port_direction(unsigned char portname,unsigned char direction);
void DIO_write_port(unsigned char portname,unsigned char portvalue);
unsigned char DIO_read_port(unsigned char portname);
void DIO_vDisable_internal_Pull_up();
void DIO_write_low_nibble(unsigned char portname,unsigned char value);
void DIO_write_high_nibble(unsigned char portname,unsigned char value);
void DIO_vconnectpullup(unsigned char portname,unsigned char pinnumber);

#endif /* DIO_H_ */