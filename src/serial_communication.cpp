#include "ros2serial/serial_communication.hpp"


SerialCommunication::SerialCommunication(string serial_port, int baudrate)
{
    openSerial(serial_port.c_str(), baudrate);
}

SerialCommunication::~SerialCommunication()
{
    if (fd_ >= 0) {
        close(fd_);
    }
}

void SerialCommunication::openSerial(const char *device_name, const int baudrate)
{
    fd_ = open(device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
        cout << "fd_: " << fd_ << endl;

        fcntl(fd_, F_SETFL, 0);
        struct termios conf_tio;
        tcgetattr(fd_, &conf_tio);

        speed_t BAUDRATE;
        switch (baudrate) {
            case 9600:    BAUDRATE = B9600; break;
            case 19200:   BAUDRATE = B19200; break;
            case 38400:   BAUDRATE = B38400; break;
            case 57600:   BAUDRATE = B57600; break;
            case 115200:  BAUDRATE = B115200; break;
            default:      BAUDRATE = B115200; break; // デフォルト値
        }
        cfsetispeed(&conf_tio, BAUDRATE);
        cfsetospeed(&conf_tio, BAUDRATE);

        conf_tio.c_lflag &= ~(ECHO | ICANON);

        // 読み取り条件: バイト未到着でもタイムアウトで復帰（無期限ブロックを回避）
        // VTIME は 0.1 秒単位。VMIN=0, VTIME=2 なら最大 ~0.2 秒で復帰
        conf_tio.c_cc[VMIN] = 0;
        conf_tio.c_cc[VTIME] = 2;

        tcsetattr(fd_, TCSANOW, &conf_tio);
}

void SerialCommunication::sendSerialData(const string& message)
{
    int bytes_written = write(fd_, message.c_str(), message.length());

    if (bytes_written < 0)
    {
        cout << "Serial Faild: could not write" << endl;
    }
    else
    {
        cout << "serial sended" << endl;
    }
}

string SerialCommunication::receiveSerialData(const string& init_object)
{
    string recv_serial_data;
    char buf[1024] = {0};

    int recv_data = read(fd_, buf, sizeof(buf));
    
    if (recv_data > 0)
    {
        recv_serial_data += string(buf, recv_data);
        cout << "recv_serial_data: " << recv_serial_data << endl;
        return recv_serial_data;
    }
    // データ未到着または一時的エラーの場合は空文字を返す
    return string();
}

json SerialCommunication::decodeSerialData(const string& serial_data, const string& init_data)
{
    json json_document;
    string convert_serial_data;

    // raw data をカット
    string input = serial_data;
    regex re("\\{[^{}]*\\}");
    smatch match;
    if (regex_search(input, match, re)) {
        convert_serial_data = match.str();
    }
    cout << "convert_serial_data: " << convert_serial_data << endl;

    try
    {
        json json_document = json::parse(convert_serial_data);
        cout << "===================== parse success ! ======================" << endl;
        return json_document;
    }
    catch (const nlohmann::detail::parse_error& e)
    {
        cout << "===== parse_error です" << e.what() << endl;
        json json_document = json::parse(init_data);
        return json_document;

    }
    catch (const nlohmann::detail::type_error& e)
    {
        cout << "===== parse_error です" << e.what() << endl;
        json json_document = json::parse(init_data);
        return json_document;
    }
    catch (...)
    {
        cout << "===== 未定義エラーです" << endl;
        json json_document = json::parse(init_data);
        return json_document;
    }
}