#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "temp_functions.h"

void help_output()
{
  printf("Description of the application functionality: sensordata [parameters]\n"
         "List of keys that this application:\n"
         "\t-h        Displaying a help prompt on the screen and exit\n"
         "\t-f FILE   Csv input file for processing\n"
         "\t-m MONTH  Statistics only for the specified month are displayed\n");
}

// Parses arguments to command
int command_line_handler(int argc, char *argv[], FILE **input, uint8_t *month)
{
  bool filepath_defined = false;
  bool month_defined = false;
  int opt = 0;
  while ((opt = getopt(argc, argv, "hf:m:")) != -1)
  {
    switch (opt)
    {
    case 'h':
      help_output();
      exit(0);
    case 'f':
    {
      FILE *file;
      file = fopen(optarg, "r");
      if (file == NULL)
      {
        printf("Error: I can't open the file %s\n", optarg);
        exit(1);
      }
      *input = file;
      filepath_defined = true;
      break;
    }
    case 'm':
    {
      char *ptr;
      long m = strtol(optarg, &ptr, 10);
      if (1 <= m && m <= 12)
      {
        *month = m;
      }
      else
      {
        printf("Error: Month must be between 1 and 12\n");
        exit(1);
      }
      break;
    }
    }
  }
  if (!filepath_defined)
  {
    if (!month_defined)
      help_output();
    else
      printf("Error: -f is required\n");
    exit(1);
  }
  return 0;
}

bool is_max(int8_t x, int8_t y)
{
  return x > y;
};

bool is_min(int8_t x, int8_t y)
{
  return x < y;
};

// Number of entries for month
uint64_t count_month(uint8_t month, sensor data[], uint32_t data_len)
{
  uint64_t count = 0;
  for (size_t i = 0; i < data_len; i++)
  {
    if (data[i].month == month)
    {
      count++;
    }
  }
  return count;
}

// Average temperature for the month
float avg_month(uint8_t month, sensor data[], uint32_t data_len)
{
  int sum = 0;
  for (size_t i = 0; i < data_len; i++)
  {
    if (data[i].month == month)
    {
      sum += data[i].temperature;
    }
  }
  return sum * 1.0 / data_len;
}

// Returns the temperature that meets the `find_cond` condition for the month
int8_t stat_month(uint8_t month, bool (*find_cond)(int8_t, int8_t), sensor data[], uint32_t data_len)
{
  int8_t stat;
  bool found_month = false;
  for (size_t i = 0; i < data_len; i++)
  {
    if (data[i].month == month)
    {
      if (!found_month)
      {
        stat = data[i].temperature;
        found_month = true;
      }
      else if ((*find_cond)(data[i].temperature, stat))
      {
        stat = data[i].temperature;
      }
    }
  }
  return stat;
}

// Mimimum temperature for the month
int8_t min_month(uint8_t month, sensor data[], uint32_t data_len)
{
  return stat_month(month, is_min, data, data_len);
}

// Maximum temperature for the month
int8_t max_month(uint8_t month, sensor data[], uint32_t data_len)
{
  return stat_month(month, is_max, data, data_len);
}

// Average temperature for the year
float avg_year(uint16_t year, sensor data[], uint32_t data_len)
{
  int sum = 0;
  for (size_t i = 0; i < data_len; i++)
  {
    if (data[i].year == year)
    {
      sum += data[i].temperature;
    }
  }
  return sum * 1.0 / data_len;
}

// Returns the temperature that meets the `find_cond` condition for the year
int8_t stat_year(uint16_t year, bool (*find_cond)(int8_t, int8_t), sensor data[], uint32_t data_len)
{
  int8_t stat;
  bool found_year = false;
  for (size_t i = 0; i < data_len; i++)
  {
    if (data[i].year == year)
    {
      if (!found_year)
      {
        stat = data[i].temperature;
        found_year = true;
      }
      else if ((*find_cond)(data[i].temperature, stat))
      {
        stat = data[i].temperature;
      }
    }
  }
  return stat;
}

// Mimimum temperature for the year
int8_t min_year(uint16_t year, sensor data[], uint32_t data_len)
{
  return stat_year(year, is_min, data, data_len);
}

// Maximum temperature for the year
int8_t max_year(uint16_t year, sensor data[], uint32_t data_len)
{
  return stat_year(year, is_max, data, data_len);
}

long get_num(char **ch, bool *ok)
{
  if (**ch != '\n' && **ch != '\r' && **ch != '\0')
  {
    char *ptr = NULL;
    errno = 0;
    long i = strtol(*ch, &ptr, 10);
    if ((errno == 0 && **ch != '\n' && !*ptr) || (*ptr == '\n') || (*ptr == '\r') || (*ptr == '\0'))
    {
      *ok = true;
    }
    else
    {
      *ok = false;
    }
    *ch = strtok(NULL, ";");
    return i;
  }
  else
  {
    *ok = false;
    return -1;
  }
}

// Parse sensor struct from CSV line
sensor from_csv_line(char *line, bool *ok)
{
  sensor data;
  char *ch = strtok(line, ";");
  data.year = get_num(&ch, ok);
  if (!*ok)
  {
    return data;
  }
  data.month = get_num(&ch, ok);
  if (!*ok)
  {
    return data;
  }
  data.day = get_num(&ch, ok);
  if (!*ok)
  {
    return data;
  }
  data.hour = get_num(&ch, ok);
  if (!*ok)
  {
    return data;
  }
  data.minute = get_num(&ch, ok);
  if (!*ok)
  {
    return data;
  }
  data.temperature = get_num(&ch, ok);
  return data;
}

// Create array from CSV file. Returns array length
uint64_t from_csv(sensor arr[], uint64_t max_arr_size, FILE *input, uint64_t max_line_size, uint64_t *number_of_err)
{
  bool ok = true;
  uint64_t rav = 0;
  uint64_t row = 1;
  char line[max_line_size];
  while (fgets(line, max_line_size, input))
  {
    if (rav >= max_arr_size)
    {
      
      printf("Error: Maximum array size exceded - file has >%llu lines\n", max_arr_size);
      return rav;
    }
    sensor parsed = from_csv_line(line, &ok);
    if (!ok)
    {
      printf("Error: The csv file format does not match the specified one in row %llu \n", row);
      (*number_of_err)++;
    }
    else
    {
      arr[rav] = parsed;
      rav++;
    }
    row++;
  }
  return rav;
}
