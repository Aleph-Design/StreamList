//
//  main.c
//  StreamList
//
//  Created by Jan H. Kila on 10/01/2021.
//  Copyright © 2021 Aleph Design. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PATH "/Users/janhkila/Internet-Radio/Radio-Stations/StreamList/202012-TEST/"
#define INPUT_FILE "202012.radio.test"
#define OUTPUT_FILE "Output.txt"
#define READ_LEN 255


char *toLower(char *s)
{
  for(char *p=s; *p; p++) *p=tolower(*p);
  return s;
}


int main(int argc, const char * argv[])
{
  FILE* pInput;
  pInput = NULL;
  char inputFile[255] = PATH;
  strcat(inputFile, INPUT_FILE);
  
  FILE* pOutput;
  pOutput = NULL;
  char outputFile[255] = PATH;
  strcat (outputFile, OUTPUT_FILE);
  
  char readLine[READ_LEN] = "";
  char statForm[4] = "mp3";   // format of station stream
  char lastChar[4] = "";      // last 3 char's of station stream (mp3)
  
//  char statName[128] = "";    // station name
//  char statStrm[255] = "";    // station stream
    
  char *strchr(const char *s, int c);
  char *pStatName  = NULL;    // pointer to station name
  char *pFirstLine = NULL;    // pointer to the first line
  char *pLastThree = NULL;    // pointer to last three char's of readLine
  
  int readLength = 0;         // length of readLine
  
  pInput  = fopen(inputFile, "r");
  pOutput = fopen(outputFile, "w");
  
  if (!pInput)
  {
    perror("Error opening input file");
    return -1;
  }
  
  if (!pOutput)
  {
    perror("Error opening output file");
    return -1;
  }

  
  while (!feof(pInput))
  {
    /*
     * Read first line
     * ===============
     */
    if (fgets(readLine, READ_LEN, pInput) != NULL)
    {
      /*
       * Replace new line character with string terminator
       * =================================================
       */
      if (readLine[strlen(readLine) - 1] == '\n')
      {
        readLine[strlen(readLine) - 1] = '\0';
      }
      /*
       * Remove the first lines with '+' in them
       * Within this block all the work is done
       */
      if (strchr(readLine, '+') == NULL)
      {
        pFirstLine = strchr(readLine, '#');
        /*
         * First real valuable line, holds station name
         * ============================================
         */
        if (pFirstLine != NULL)
        {
          pStatName = strchr(readLine, ',');
          if (pStatName != NULL)
          {
            if (pStatName[0] == ',') pStatName++; // remove ','
            pStatName = strtok(pStatName, "(");   // remove (...)
            //strcpy(statName, pStatName);          // Not needed
            /*
             * Now we have pointer to station name, read the stream
             * ====================================================
             */
            if (fgets(readLine, READ_LEN, pInput) != NULL)
            {
              /*
               * Replace new line character with string terminator
               * =================================================
               */
              if (readLine[strlen(readLine) - 1] == '\n')
              {
                readLine[strlen(readLine) - 1] = '\0';
              }
              
              if (strchr(readLine, 'h'))
              {
                /*
                 * This second line contains the stream's URL
                 * ==========================================
                 */
                readLength = (int)strlen(readLine);
                pLastThree = &readLine[readLength - 3];
                
                // Make sure the format is lower case
                strcpy(lastChar, toLower(pLastThree));
                
                if (strcmp(lastChar, statForm) == 0)
                {
                  //strcpy(statStrm, readLine); // Not needed
                  printf("{\"%s\", \"%s\"},\n", readLine, pStatName); // test
                  fprintf(pOutput, "{\"%s\", \"%s\"},\n", readLine, pStatName);
                }
              } // if-strchr(readLine) second line
            } // if-fgets() second line
            
          } // if-pStatName
        } // if-pFirstLine
      } // if-strchr()
    } // if-fgets() first line
    
  } // while() =============================================
  
  fclose(pInput); pInput = NULL;
  fclose(pOutput); pOutput = NULL;
  
  printf("End Of File Demo\n");
  return 0;
}
