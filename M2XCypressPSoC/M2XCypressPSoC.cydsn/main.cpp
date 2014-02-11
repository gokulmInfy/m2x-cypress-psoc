#ifdef __cplusplus
extern "C" {
#endif
#include <project.h>
#ifdef __cplusplus
}
#endif

#include <stdlib.h>

#include "m2x/Client.h"
#include "m2x/M2XStreamClient.h"

char feedId[] = "<feed id>"; // Feed you want to post to
char streamName[] = "<stream name>"; // Stream you want to post to
char m2xKey[] = "<m2x api key>"; // Your M2X API Key or Master API Key

int main()
{
  uint32 ip = 0;
  Client client;
  M2XStreamClient m2xClient(&client, m2xKey);

  /* Place your initialization/startup code here (e.g. MyInst_Start()) */
	SPI0_Start();
	ETH0_Start();
  SDCS_Write(1);

  while (true) {
    ip = ETH0_GetIP();
    if ((ip != 0) && (ip != 0xffffffff)) { break; }
  }

  /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
  for(;;)
  {
    /* Place your application code here. */
    int response = m2xClient.post(feedId, streamName, 42);
    if (response == -1) {
      /* Error occurs */
      response = response;
    } else {
      /* Post succeeds */
      response = response;
    }
    CyDelay(10 * 1000);
  }

  return 0;
}
