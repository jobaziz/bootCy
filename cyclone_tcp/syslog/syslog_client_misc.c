/**
 * @file syslog_client_misc.c
 * @brief Helper functions for Syslog client
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
 *
 * This software is provided in source form for a short-term evaluation only. The
 * evaluation license expires 90 days after the date you first download the software.
 *
 * If you plan to use this software in a commercial product, you are required to
 * purchase a commercial license from Oryx Embedded SARL.
 *
 * After the 90-day evaluation period, you agree to either purchase a commercial
 * license or delete all copies of this software. If you wish to extend the
 * evaluation period, you must contact sales@oryx-embedded.com.
 *
 * This evaluation software is provided "as is" without warranty of any kind.
 * Technical support is available as an option during the evaluation period.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.3.2
 **/

//Switch to the appropriate trace level
#define TRACE_LEVEL SYSLOG_TRACE_LEVEL

//Dependencies
#include "syslog/syslog_client.h"
#include "syslog/syslog_client_misc.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (SYSLOG_CLIENT_SUPPORT == ENABLED)


/**
 * @brief Format timestamp
 * @param[in] time Unix timestamp
 * @param[out] buffer Buffer where to store the formatted string
 * @return Length of the formatted string
 **/

size_t syslogClientFormatTimestamp(time_t time, char_t *buffer)
{
   size_t n;
   DateTime dateTime;

   //Abbreviated months
   static const char_t months[13][4] =
   {
      "   ",
      "Jan",
      "Feb",
      "Mar",
      "Apr",
      "May",
      "Jun",
      "Jul",
      "Aug",
      "Sep",
      "Oct",
      "Nov",
      "Dec"
   };

   //Convert Unix timestamp to date
   convertUnixTimeToDate(time, &dateTime);

   //The format of the timestamp field is Mmm dd hh:mm:ss. If the day of the
   //month is less than 10, then it must be represented as a space and then the
   //number (refer to RFC 3164, section 4.1.2)
   n = osSprintf(buffer, "%s %2" PRIu8 " %02" PRIu8 ":%02" PRIu8 ":%02" PRIu8,
      months[dateTime.month], dateTime.day, dateTime.hours, dateTime.minutes,
      dateTime.seconds);

   //Return the length of the formatted string
   return n;
}

#endif
