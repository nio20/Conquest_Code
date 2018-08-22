// The database of all fix.___ things.
// https://github.com/SlashDevin/NeoGPS/blob/master/extras/doc/Data%20Model.md


#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>

static NMEAGPS  gps;
static gps_fix  fix;
static void doSomeWork()
{

  Serial.print("Speed:");
  Serial.println(fix.speed_kph());

  Serial.println("Lat and Long:");
  Serial.print( " Lat:");
  Serial.print(fix.latitude());
  Serial.print( " Long:");
  Serial.println(fix.longitude());
  Serial.println();

}

static void GPSloop()
{
  while (gps.available( gpsPort )) {
    fix = gps.read();
    doSomeWork();
  }

}

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.print( F("NMEA.INO: started\n") );
  Serial.print( F("  fix object size = ") );
  Serial.println( sizeof(gps.fix()) );
  Serial.print( F("  gps object size = ") );
  Serial.println( sizeof(gps) );
  Serial.println( F("Looking for GPS device on " GPS_PORT_NAME) );

#ifndef NMEAGPS_RECOGNIZE_ALL
#error You must define NMEAGPS_RECOGNIZE_ALL in NMEAGPS_cfg.h!
#endif

#ifdef NMEAGPS_INTERRUPT_PROCESSING
#error You must *NOT* define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

#if !defined( NMEAGPS_PARSE_GGA ) & !defined( NMEAGPS_PARSE_GLL ) & \
      !defined( NMEAGPS_PARSE_GSA ) & !defined( NMEAGPS_PARSE_GSV ) & \
      !defined( NMEAGPS_PARSE_RMC ) & !defined( NMEAGPS_PARSE_VTG ) & \
      !defined( NMEAGPS_PARSE_ZDA ) & !defined( NMEAGPS_PARSE_GST )

  Serial.println( F("\nWARNING: No NMEA sentences are enabled: no fix data will be displayed.") );

#else
  if (gps.merging == NMEAGPS::NO_MERGING) {
    Serial.print  ( F("\nWARNING: displaying data from ") );
    Serial.print  ( gps.string_for( LAST_SENTENCE_IN_INTERVAL ) );
    Serial.print  ( F(" sentences ONLY, and only if ") );
    Serial.print  ( gps.string_for( LAST_SENTENCE_IN_INTERVAL ) );
    Serial.println( F(" is enabled.\n"
                      "  Other sentences may be parsed, but their data will not be displayed.") );
  }
#endif

  Serial.print  ( F("\nGPS quiet time is assumed to begin after a ") );
  Serial.print  ( gps.string_for( LAST_SENTENCE_IN_INTERVAL ) );
  Serial.println( F(" sentence is received.\n"
                    "  You should confirm this with NMEAorder.ino\n") );

  trace_header( Serial );
  Serial.flush();

  gpsPort.begin( 9600 );

}

void loop() {
  GPSloop();

}
