#include "ms4525do.h"

/*
 * An MS4525DO object
 */
bfs::Ms4525do pres;

float velocity()
{
  // Read and Convert Velocity for pressure(pascal)
  float read_press;            // read press
  float conv_press;            // converted press
  float low_p_filter = 0.0005; // low cat filter factor
  float high_p_filter1 = 0.1;  // high filter factor
  float high_p_filter2 = 4;    // high filter factor
  float dose_point = 58;       // cat point
  float air = 1.15;            // air Density
  float velo;                  // result velosity

  // If you want to use the Sensor temperature, you can use below
  // float temp = pres.temp_c(); // temperature in celsius
  // air = 1.15 - (0.00367 * temp); // air density in kg/m^3

  read_press = abs(pres.pres_pa());

  if (read_press < dose_point)
  {
    conv_press = read_press * low_p_filter;
  }
  else
  {
    conv_press = (read_press * high_p_filter1) - high_p_filter2;
  }
  velo = (2 * conv_press) / air;
  velo = 0.97 * sqrt(velo);

  return velo;
}

void setup()
{
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial)
  {
  }
  Wire.begin();
  // Wire.setClock(400000);
  /*
   * I2C address of 0x28, on bus 0, with a -1 to +1 PSI range
   */
  pres.Config(&Wire, 0x28, 1.0f, -1.0f);
  /* Starting communication with the pressure transducer */
  if (!pres.Begin())
  {
    Serial.println("Error communicating with sensor");
    while (1)
    {
    }
  }
}

void loop()
{
  /* Display the data */
  if (pres.Read())
  {
    Serial.print("speed=");
    Serial.println(velocity(), 6);
  }

  delay(100);
}
