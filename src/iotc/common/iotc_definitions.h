// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license.

#ifndef AZURE_IOTC_LITE_DEFINITIONS_H
#define AZURE_IOTC_LITE_DEFINITIONS_H

#define IOTC_MALLOC malloc
#define IOTC_FREE free

#define AZURE_MQTT_SERVER_PORT 8883
#define AZURE_HTTPS_SERVER_PORT 443

#define IOTC_SERVER_RESPONSE_TIMEOUT 20  // seconds

#define SSL_CLIENT_CERT_PEM NULL
#define SSL_CLIENT_PRIVATE_KEY_PEM NULL

/* DigiCert Global Root G2 (https://www.digicert.com/kb/digicert-root-certificates.htm) */
#define SSL_CA_PEM_DEF                                                   \
  "-----BEGIN CERTIFICATE-----\r\n"                                      \
  "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\r\n" \
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n" \
  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\r\n" \
  "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\r\n" \
  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n" \
  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\r\n" \
  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\r\n" \
  "2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\r\n" \
  "1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\r\n" \
  "q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\r\n" \
  "tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\r\n" \
  "vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\r\n" \
  "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\r\n" \
  "5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\r\n" \
  "1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\r\n" \
  "NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\r\n" \
  "Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\r\n" \
  "8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\r\n" \
  "pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\r\n" \
  "MrY=\r\n"                                                             \
  "-----END CERTIFICATE-----\r\n"

// GENERAL
#define TO_STRING_(s) #s
#define TO_STRING(s) TO_STRING_(s)
#define iotc_max(a, b) (a > b ? a : b)
#define iotc_min(a, b) (a < b ? a : b)

#define STRING_BUFFER_16 16
#define STRING_BUFFER_32 32
#define STRING_BUFFER_64 64
#define STRING_BUFFER_128 128
#define STRING_BUFFER_256 256
#define STRING_BUFFER_512 512
#define STRING_BUFFER_1024 1024
#define STRING_BUFFER_4096 4096

#define NTP_SYNC_PERIOD (6 * 60 * 60 * 1000)

#define AZIOTC_API_MAJOR_VERSION 0.
#define AZIOTC_API_MINOR_VERSION 3.
#define AZIOTC_API_PATCH_VERSION 0
#define AZIOTC_API_VERSION                                    \
  TO_STRING(AZIOTC_API_MAJOR_VERSION AZIOTC_API_MINOR_VERSION \
                AZIOTC_API_PATCH_VERSION)                     \
  "-esp8266iotc"

#define AZURE_IOT_CENTRAL_CLIENT_SIGNATURE \
  "user-agent: iot-central-client/" AZIOTC_API_VERSION

#if defined(_DEBUG) || defined(DEBUG)
#define ASSERT_OR_FAIL_FAST(x) assert(x)
#else  // defined(_DEBUG) || defined(DEBUG)
#define ASSERT_OR_FAIL_FAST(x)                      \
  if (!(x)) {                                       \
    LOG_ERROR(TO_STRING(x) "condition has failed"); \
  }
#endif  // defined(_DEBUG) || defined(DEBUG)

#ifdef MBED_STATIC_ASSERT
#define ASSERT_STATIC MBED_STATIC_ASSERT
#else
#define ASSERT_STATIC static_assert
#endif

#endif  // AZURE_IOTC_LITE_DEFINITIONS_H