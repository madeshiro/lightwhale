/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "wl55jc1_nucleo_driver.h"
#include "wl55jc1_nucleo_driver-defs.h"

// C standard lib
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// POSIX serial i/o stream
#include <termios.h>

#define check_device() if (NULL == WL55JC1_Device) {\
    driver_log("device not defined"); return 0; }

struct wl55jc1_nucleo_serial_driver
{
    int fd;
    struct termios tty;
} *WL55JC1_Device = NULL;

pWL55JC1_Serial WL55JC1_Nucleo_SetCurrentDevice(pWL55JC1_Serial serial)
{
    pWL55JC1_Serial old_device = WL55JC1_Device;
    WL55JC1_Device = serial;
    return old_device;
}

pWL55JC1_Serial WL55JC1_Nucleo_GetCurrentDevice(void)
{
    return WL55JC1_Device;
}

//
// LRHTCP device driver
//

static size_t WL55JC1_Driver_Ping   (pLwpSocket socket, lp_ping_result_t* addrBuffer, size_t maxSize);
static bool   WL55JC1_Driver_Bind   (pLwpSocket socket);
static bool   WL55JC1_Driver_Listen (pLwpSocket socket);
static bool   WL55JC1_Driver_Accept (pLwpSocket socket);
static size_t WL55JC1_Driver_Send   (pLwpSocket socket, const void* data, size_t size);
static size_t WL55JC1_Driver_Receive(pLwpSocket socket, void* data, size_t size);
static void   WL55JC1_Driver_Close  (pLwpSocket socket);

LpDeviceDriver_s LpDeviceDriver = (LpDeviceDriver_s){
    .Ping = WL55JC1_Driver_Ping,
    .Bind = WL55JC1_Driver_Bind,
    .Listen = WL55JC1_Driver_Listen,
    .Accept = WL55JC1_Driver_Accept,
    .Send = WL55JC1_Driver_Send,
    .Receive = WL55JC1_Driver_Receive,
    .Close = WL55JC1_Driver_Close
};

size_t WL55JC1_Driver_Ping(pLwpSocket socket, lp_ping_result_t* addrBuffer, size_t maxSize)
{
    check_device();

    if (!WL55JC1_Nucleo_Serial_SetSocket(WL55JC1_Device, socket))
    {
        return 0;
    }



    return 0;
}

bool WL55JC1_Driver_Bind(pLwpSocket socket)
{
    check_device();

    return false;
}

bool WL55JC1_Driver_Listen(pLwpSocket socket)
{
    check_device();

    return false;
}

bool WL55JC1_Driver_Accept(pLwpSocket socket)
{
    check_device();
    //
    // Accept the communication by sending the selected channel to use.
    // Once this is done, the modem will switch to the com channel
    // and synchronize communication with the distant host.
    //

    return false;
}

size_t WL55JC1_Driver_Send(pLwpSocket socket, const void* data, size_t size)
{
    check_device();

    return 0;
}

size_t WL55JC1_Driver_Receive(pLwpSocket socket, void* data, size_t size)
{
    check_device();

    return 0;
}

void WL55JC1_Driver_Close(pLwpSocket socket)
{
    struct wl55jc1_nucleo_serial_header header;

    if (NULL != WL55JC1_Device)
    {
        // Switch current communication to the requested socket
        if (!WL55JC1_Nucleo_Serial_SetSocket(WL55JC1_Device, socket))
        {
            driver_log("WL55JC1_Nucleo_Serial_SetSocket failed");
            return;
        }

        // Close communication
        header.actionId = WL55JC1_ACTION_CLOSE;
        if (sizeof header != WL55JC1_Nucleo_Serial_Write(WL55JC1_Device, &header, sizeof header))
        {
            driver_log("WL55JC1_Nucleo_Serial_Write failed");
        }
    }
}

//
// Device serial com
//

bool WL55JC1_Nucleo_Serial_Detect(const char* serial_port)
{
    const int fd = open(serial_port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        return false;
    }
    close(fd);
    return true;
}

pWL55JC1_Serial WL55JC1_Nucleo_Serial_Init(const char* serial_port)
{
    int fd = open(serial_port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        driver_log("Unable to open serial port %s", serial_port);
        return NULL;
    }

    pWL55JC1_Serial serial = malloc(sizeof(struct wl55jc1_nucleo_serial_driver));
    memset(&serial->tty, 0, sizeof serial->tty);

    // set file descriptor
    serial->fd = fd;

    // set tty attributes
    if (tcgetattr(fd, &serial->tty) != 0)
    {
        free(serial);
        return NULL;
    }

    // Set-up serial i/o baudrate
    cfsetospeed(&serial->tty, WL55JC1_NUCLEO_SERIAL_BAUDRATE);
    cfsetispeed(&serial->tty, WL55JC1_NUCLEO_SERIAL_BAUDRATE);

    serial->tty.c_cflag = serial->tty.c_cflag & ~CSIZE | CS8 | CRTSCTS;
    serial->tty.c_iflag &= ~IGNBRK;
    serial->tty.c_lflag &= ~ICANON;
    serial->tty.c_oflag = 0;

    serial->tty.c_cc[VMIN] = 0;     // timed-out read
    serial->tty.c_cc[VTIME] = 50;   // 5000 ms timeout

    serial->tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off material control
    serial->tty.c_cflag |= CLOCAL | CREAD;          // ignore modem control
    serial->tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    serial->tty.c_cflag &= ~CSTOPB;                 // 1 stop bit
    serial->tty.c_cflag &= ~CRTSCTS;                // Disable flow control

    if (tcsetattr(fd, TCSANOW, &serial->tty) != 0)
    {
        driver_log("Unable to set tty attributes for %s", serial_port);
        free(serial);
        return NULL;
    }

    uint8_t id = 0;
    WL55JC1_Nucleo_Serial_ReadRegister(serial, k_wl55jc1_reg_id, &id, sizeof id);
    if (id != WL55JC1_NUCLEO_SERIAL_ID)
    {
        driver_log("Invalid serial port @ %s", serial_port);
        free(serial);
        return NULL;
    }

    return serial;
}

void WL55JC1_Nucleo_Serial_Close(pWL55JC1_Serial fd)
{
    if (fd != NULL)
    {
        close(fd->fd);
        free(fd);
    }
}

size_t WL55JC1_Nucleo_Serial_WriteRegister(pWL55JC1_Serial fd,
            wl55jc1_nucleo_register_t reg, const void* value, size_t len)
{
    if (fd == NULL)
    {
        driver_log("Invalid device descriptor");
        return -1;
    }

    struct wl55jc1_nucleo_serial_register_header header;
    header.actionId.actionId = WL55JC1_ACTION_REGISTER_WRITE;
    header.registerId = reg;
    header.length = len;

    if (write(fd->fd, &header, sizeof header) != sizeof header)
    {
        driver_log("WL55JC1_Nucleo_Serial_WriteRegister failed");
        return -1;
    }

    return write(fd->fd, value, len);
}

size_t WL55JC1_Nucleo_Serial_ReadRegister(pWL55JC1_Serial fd,
            wl55jc1_nucleo_register_t reg, void* inValue, size_t len)
{
    if (fd == NULL)
    {
        driver_log("Invalid device descriptor");
        return -1;
    }

    struct wl55jc1_nucleo_serial_register_header header;
    header.actionId.actionId = WL55JC1_ACTION_REGISTER_READ;
    header.registerId = reg;
    header.length = len;

    if (write(fd->fd, &header, sizeof header) != sizeof header)
    {
        driver_log("WL55JC1_Nucleo_Serial_ReadRegister failed");
        return -1;
    }

    return read(fd->fd, inValue, len);
}

size_t WL55JC1_Nucleo_Serial_Read(pWL55JC1_Serial fd, void* value, size_t len)
{
    if (NULL == fd || fd->fd < 0)
    {
        return -1;
    }

    return read(fd->fd, value, len);
}

size_t WL55JC1_Nucleo_Serial_Write(pWL55JC1_Serial fd, const void* value, size_t len)
{
    if (NULL == fd || fd->fd < 0)
    {
        return -1;
    }

    return write(fd->fd, value, len);
}

bool WL55JC1_Nucleo_Serial_SetSocket(pWL55JC1_Serial fd, pLwpSocket socket)
{
    if (fd == NULL || fd->fd < 0)
    {
        return false;
    }

    return sizeof(LwpSocket) == WL55JC1_Nucleo_Serial_WriteRegister(
                                    fd,
                                    k_wl55jc1_reg_lrhtcp_socket,
                                    socket, sizeof(LwpSocket));
}

bool WL55JC1_Nucleo_Serial_GetSocket(pWL55JC1_Serial fd, pLwpSocket socket)
{
    if (!socket || fd == NULL || fd->fd < 0)
    {
        return false;
    }

    LwpSocket read;
    if (sizeof(LwpSocket) != WL55JC1_Nucleo_Serial_ReadRegister(fd,
        k_wl55jc1_reg_lrhtcp_socket, &read, sizeof(read)))
    {
        driver_log("Unable to read socket");
        return false;
    }

    *socket = read;
    return true;
}

size_t WL55JC1_Nucleo_Serial_ReadLog(pWL55JC1_Serial fd, void* inBuffer, uint8_t* logRemainingLines)
{
    uint8_t logsQueueLength = 0;
    size_t inRead = 0;

    if (NULL == fd || fd->fd < 0 || inBuffer == NULL)
    {
        return -1;
    }

    if (sizeof logsQueueLength != WL55JC1_Nucleo_Serial_ReadRegister(fd,
        k_wl55jc1_reg_logs_queue, &logsQueueLength, sizeof logsQueueLength))
    {
        driver_log("Unable to read logs queue length");
        return -1;
    }

    if (logsQueueLength > 0)
    {
        struct wl55jc1_nucleo_serial_header header;
        header.actionId = WL55JC1_ACTION_READLOG;

        if (sizeof header != WL55JC1_Nucleo_Serial_Write(fd, &header, sizeof(header)))
        {
            driver_log("WL55JC1_Nucleo_Serial_Write failed");
            return -1;
        }

        inRead = WL55JC1_Nucleo_Serial_Read(fd, inBuffer, WL55JC1_NUCLEO_SERIAL_LOG_LINE_LENGTH);
    }

    if (logRemainingLines)
    {
        *logRemainingLines = logsQueueLength == 0 ? 0 : logsQueueLength - 1;
    }

    return inRead;
}
