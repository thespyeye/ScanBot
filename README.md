# ScanBot
Repository containing the source code of the currently WIP ScanBot project

The code is currently being rewritten from the ground up to:
- Be compatible with Microchip Studio 7 as opposed to the Arduino IDE.
-  Resolve egregious memory leaks and logic errors in the original code.
- Refactor the code to increase readability and ease of debugging.
- Finish the code this time.

Parts list: (May change in the future)
- Arduino Mega
- Arduino Motor Shield Rev3
- RPLIDAR A1M8 Scanner
- [Half of a 1/24th scale RC tank I had lying around](https://www.amazon.com/dp/B08249TCZC/ref=sspa_dk_detail_5?pd_rd_i=B08249TCZC&pd_rd_w=pWtmN&content-id=amzn1.sym.eb7c1ac5-7c51-4df5-ba34-ca810f1f119a&pf_rd_p=eb7c1ac5-7c51-4df5-ba34-ca810f1f119a&pf_rd_r=JHPA20VEVBCVT81HBAV3&pd_rd_wg=PQXf2&pd_rd_r=ed9c835d-13f1-4b1b-8f6f-86c6a81a16d6&s=toys-and-games&sp_csd=d2lkZ2V0TmFtZT1zcF9kZXRhaWw&th=1) This will likely be changed if issues arise with the current chassis


As of now, all code is written in C and is configured specifically for the ATmega 2560 microcontroller. In practice, that means any register access is being done directly by the address, and not by any header files, such as <avr/io.h>
