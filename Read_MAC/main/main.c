#include <stdio.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_mac.h"

void app_main(void) {
    uint8_t mac[6];  // Array para almacenar la dirección MAC

    esp_err_t ret = esp_read_mac(mac, ESP_MAC_WIFI_STA);

    // if (ret == ESP_OK) {
    //     // Imprimir la MAC en formato legible
    //     printf("MAC Address Wi-Fi STA: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // } else {
    //     printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    // }

    // ret = esp_read_mac(mac, ESP_MAC_WIFI_SOFTAP);

    // if (ret == ESP_OK) {
    //     // Imprimir la MAC en formato legible
    //     printf("MAC Address Wi-Fi AP: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // } else {
    //     printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    // }

    // ret = esp_read_mac(mac, ESP_MAC_BT);

    // if (ret == ESP_OK) {
    //     // Imprimir la MAC en formato legible
    //     printf("MAC Address Bluetooth: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // } else {
    //     printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    // }

    // ret = esp_read_mac(mac, ESP_MAC_ETH);

    // if (ret == ESP_OK) {
    //     // Imprimir la MAC en formato legible
    //     printf("MAC Address Ethernet: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // } else {
    //     printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    // }

    // uint8_t mac_8[8]={};  // Array para almacenar la dirección MAC

    // // Leer la dirección MAC para Wi-Fi STA (estación)
    // ret = esp_read_mac(mac_8, ESP_MAC_IEEE802154);

    // if (ret == ESP_OK) {
    //     // Imprimir la MAC en formato legible
    //     printf("MAC Address IEEE802154: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n",
    //            mac_8[0], mac_8[1], mac_8[2], mac_8[3], mac_8[4], mac_8[5], mac_8[6], mac_8[7]);
    // } else {
    //     printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    // }

    // ret = esp_read_mac(mac, ESP_MAC_BASE);

    // if (ret == ESP_OK) {
    //     // Imprimir la MAC en formato legible
    //     printf("MAC Address other MAC types: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // } else {
    //     printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    // }

    char mac_str[18];             // String para almacenar la MAC formateada (17 caracteres + '\0')
    ret = esp_read_mac(mac, ESP_MAC_EFUSE_FACTORY);

    if (ret == ESP_OK) {
        // Imprimir la MAC en formato legible
        printf("MAC Address MAC_FACTORY: %02X:%02X:%02X:%02X:%02X:%02X\n",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    }

    snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    printf("MAC Address as string: %s\n", mac_str);

    // // ret = esp_read_mac(mac, ESP_MAC_EFUSE_CUSTOM);

    // // if (ret == ESP_OK) {
    // //     // Imprimir la MAC en formato legible
    // //     printf("MAC Address MAC_CUSTOM: %02X:%02X:%02X:%02X:%02X:%02X\n",
    // //            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // // } else {
    // //     printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    // // }

    // uint8_t mac_2[2]={};  // Array para almacenar la dirección MAC

    // ret = esp_read_mac(mac_2, ESP_MAC_EFUSE_EXT);

    // if (ret == ESP_OK) {
    //     // Imprimir la MAC en formato legible
    //     printf("MAC Address ESP_MAC_EFUSE_EXT: %02X:%02X\n",
    //            mac_2[0], mac_2[1]);
    // } else {
    //     printf("Error reading MAC address: %s\n", esp_err_to_name(ret));
    // }
}
