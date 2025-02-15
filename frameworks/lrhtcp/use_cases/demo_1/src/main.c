//------------------------------------------------------------------------------
//
// File:        main.c
// Description: Definition of the lrhtcp demo #1
//
//------------------------------------------------------------------------------
//
// File generated on feb 2025 by Rin Baudelet
//------------------------------------------------------------------------------

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// LR/HT Control Protocol
#include <lrhtcp/lrhtcp.h>

// wl55jc1 HAL driver
#include <wl55jc1_nucleo_driver.h>

// C standard lib
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef enum demo1_error_id
{
    k_Demo1_Success = 0,

    // Error with the device
    k_Demo1_Error_DeviceNotFound    = 0x01,
    k_Demo1_Error_DeviceIO          = 0x02,

    // Error with LRHTCP
    k_Demo1_Error_CreateSocket      = 0x10,
    k_Demo1_Error_Bind2Network      = 0x11,
    k_Demo1_Error_SendingFailed     = 0x12,
    k_Demo1_Error_ReceivingFailed   = 0x13,

    // Error with hardware/internal software
    k_Demo1_Error_RpiCameraFailed   = 0x21,
    k_Demo1_Error_CompressionFailed = 0x22,
    k_Demo1_Error_FileNotFound      = 0x23,
    k_Demo1_Error_FileStream        = 0x24,

} demo1_error_id_t;

static const char* serial_port_list[] = {
        "/dev/ttyS0",
        "/dev/ttyS1",
        "/dev/ttyACM0",
        "/dev/ttyACM1",
        NULL
};

#define __unused __attribute__((unused)) // NOLINT
#define demo1_log(msg, ...) printf("[framework:demo1.l%03d] " msg "\n", \
    __LINE__, ##__VA_ARGS__)

#define DEMO1_PHOTO_DELAY   5
#define DEMO1_VERSION       "1.0.0"

lp_address_t demo1_server_addr = {.bytes = {65, 1, 1, 1}};
lp_address_t demo1_client_addr = {.bytes = {65, 1, 1, 2}};

int server_side(void);
int client_side(void);

int main(int argc __unused,
         const char *argv[] __unused)
{
    int status = 0;
#ifdef DEMO_SERVER_SIDE
    status = server_side();
#else
    status = client_side();
#endif 
    return status;
}

pWL55JC1_Serial find_device(const char** selected_port)
{
    const char** serial_port = serial_port_list;

    demo1_log("Start scanning serial ports to find device...");

    while (*serial_port != NULL)
    {
        pWL55JC1_Serial device = WL55JC1_Nucleo_Serial_Init(*serial_port);
        if (device != NULL)
        {
            *selected_port = *serial_port;
            return device;
        }
        serial_port++;
    }

    return NULL;
}

int server_side(void)
{
    printf("========================================\n");
    printf("Welcome to the LR/HT-CP server-side!\n"
        "\t_ demo #1 version " DEMO1_VERSION "\n"
        "\t_ made by Rin Baudelet\n"
        );
    printf("========================================\n\n");

    demo1_log("Setting up network with...");
    demo1_log("  _ %-15s: EU863_70", "frequency plan");
    demo1_log("  _ %-15s: 65.1.1.1", "lp address");

    const char* serial_port = NULL;

    // Initialize the NIC device and update driver
    pWL55JC1_Serial device = find_device(&serial_port);
    if (!device)
    {
        demo1_log("Failed to initialize wl55jc1 device");
        return k_Demo1_Error_DeviceNotFound;
    }
    WL55JC1_Nucleo_SetCurrentDevice(device);
    //
    demo1_log("Driver %s found and initialized...", serial_port);

    // Setting up the server address
    if (sizeof demo1_server_addr != WL55JC1_Nucleo_Serial_WriteRegister(device, k_wl55jc1_reg_lrhtcp_lpaddr0,
        (uint8_t*)&demo1_server_addr, sizeof demo1_server_addr))
    {
        demo1_log("Failed to update network lightwhale protocol address");
        return k_Demo1_Error_DeviceIO;
    }
    //
    demo1_log("LP address refreshed...");

    // Create a socket with EU863-870 MHz frequency plan
    pLwpSocket socket = LpCreateSocket(LP_FREQUENCY_PLAN_EU863_70);
    if (!socket)
    {
        demo1_log("Failed to create socket");
        return k_Demo1_Error_CreateSocket;
    }
    //
    demo1_log("Socket created...");

    // Listen for incoming communication(s)
    if (!LpListen(socket))
    {
        demo1_log("Failed to listen on socket");
        return k_Demo1_Error_Bind2Network;
    }

    // Accept communication
    LpAccept(socket);

    // Take a photo
    demo1_log("A communication has been established! Say cheese!");
    for (uint8_t i = DEMO1_PHOTO_DELAY; i > 0; i--)
    {
        demo1_log("%d...", i);
        usleep(1*1000*1000);
    }
    demo1_log("CHEESE!");
    if (0 != system("rpicam-still --width 1920 --height 1080 -o photo.jpg"))
    {
        demo1_log("Failed to take photo");
        demo1_log("Close communication...");
        LpClose(socket);
        return k_Demo1_Error_RpiCameraFailed;
    }

    // Convert/Compress to JPEG XL
    if (0 != system("ffmpeg -i photo.jpg photo.jxl"))
    {
        demo1_log("Compression failed!");
        demo1_log("Close communication...");
        LpClose(socket);
        return k_Demo1_Error_CompressionFailed;
    }

    // Read the photo buffer
    FILE* photo = fopen("photo.jxl", "r");
    if (NULL == photo)
    {
        demo1_log("Failed to open photo.jxl");
        demo1_log("Close communication...");
        LpClose(socket);
        return k_Demo1_Error_FileNotFound;
    }

    // Create timer for test benchmarking (timing profiling)
    clock_t begin = clock();

    // Send the photo
    uint8_t buffer[1024*1];
    size_t read_bytes = 0;
    while (0 < (read_bytes=fread(buffer, sizeof buffer, 1, photo)))
        // Read until EOF
    {
        if (read_bytes != LpSend(socket, buffer, read_bytes))
        {
            demo1_log("Failed to send photo");
            demo1_log("Close communication...");
            LpClose(socket);
            return k_Demo1_Error_SendingFailed;
        }
    }

    demo1_log("Photo sent in %.2f seconds!", (double)(clock() - begin) / CLOCKS_PER_SEC);
    demo1_log("Demo #1 terminated with success!");

    // Close socket and file
    LpClose(socket);
    fclose(photo);

    return k_Demo1_Success;
}

int client_side(void)
{
    printf("========================================\n");
    printf("Welcome to the LR/HT-CP client-side!\n"
        "\t_ demo #1 version " DEMO1_VERSION "\n"
        "\t_ made by Rin Baudelet\n"
        );
    printf("========================================\n");

    demo1_log("Setting up network with...");
    demo1_log("  _ %-15s: EU863_70", "frequency plan");
    demo1_log("  _ %-15s: 65.1.1.2", "lp address");

    const char* serial_port = NULL;

    // Initialize the NIC device and update driver
    pWL55JC1_Serial device = find_device(&serial_port);
    if (!device)
    {
        demo1_log("Failed to initialize wl55jc1 device");
        return k_Demo1_Error_DeviceNotFound;
    }
    WL55JC1_Nucleo_SetCurrentDevice(device);
    //
    demo1_log("Driver %s found and initialized...", serial_port);

    // Setting up the server address
    if (sizeof demo1_server_addr != WL55JC1_Nucleo_Serial_WriteRegister(device, k_wl55jc1_reg_lrhtcp_lpaddr0,
        (uint8_t*)&demo1_server_addr, sizeof demo1_client_addr))
    {
        demo1_log("Failed to update network lightwhale protocol address");
        return k_Demo1_Error_DeviceIO;
    }
    //
    demo1_log("LP address refreshed...");

    // Create a socket with EU863-870 MHz frequency plan
    pLwpSocket socket = LpCreateSocket(LP_FREQUENCY_PLAN_EU863_70);
    if (!socket)
    {
        demo1_log("Failed to create socket");
        return k_Demo1_Error_CreateSocket;
    }
    //
    demo1_log("Socket created...");

    // Bind communication to the distant host
    if (!LpBind(socket, demo1_server_addr))
    {
        demo1_log("Failed to bind socket");
        return k_Demo1_Error_Bind2Network;
    }
    //
    demo1_log("Socket bound to distant host (lp=%d.%d.%d.%d)...",
        demo1_server_addr.bytes[0], demo1_server_addr.bytes[1],
        demo1_server_addr.bytes[2], demo1_server_addr.bytes[3]);

    // Prepare to receive the photo by creating a new file
    FILE* photo = fopen("photo.jxl", "w+");
    if (NULL == photo)
    {
        demo1_log("Failed to open filestream");
        demo1_log("Close communication...");
        LpClose(socket);
        return k_Demo1_Error_FileStream;
    }

    // Create timer for test benchmarking (timing profiling)
    clock_t begin = clock();

    // Receive the photo from the network
    uint8_t buffer[256];
    size_t read_bytes = 0;

    while (0 < (read_bytes=LpRecv(socket, buffer, 256)))
        // Read from lora network until received buffer's size different from expected
    {
        if (read_bytes != fwrite(buffer, read_bytes, 1, photo))
        {
            demo1_log("Error while writing to the output file");
            demo1_log("Close communication...");
            LpClose(socket);
            return k_Demo1_Error_FileStream;
        }

        // If read bytes are less than the buffer size, it means than EOF has been
        // reached. File fully received, close communication.
        if (read_bytes < 256)
            break;
    }

    demo1_log("Photo received in %.2f seconds!", (double)(clock() - begin) / CLOCKS_PER_SEC);
    demo1_log("Demo #1 terminated with success!");

    // Close socket and file
    LpClose(socket);
    fclose(photo);

    return k_Demo1_Success;
}
