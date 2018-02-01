
int8_t open_port(uint8_t * SerialPort);
void close_port();
ssize_t write_port(uint8_t * CommandLine);
ssize_t read_port(uint8_t * Buffer, size_t Count);

