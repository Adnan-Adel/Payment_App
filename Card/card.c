#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card.h"

#define CARD_HOLDER_NAME_MAX_LEN 24
#define CARD_HOLDER_NAME_MIN_LEN 20

#define CARD_EXPIRY_DATE_LEN 5

#define PAN_MAX_LEN 19
#define PAN_MIN_LEN 16


EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    EN_cardError_t ret_val= WRONG_NAME;
    uint8_t index_i=0;
    uint8_t is_num=0;

    while(WRONG_NAME == ret_val)
    {
        printf("Enter name: ");
        fflush(stdin);
        gets(cardData->cardHolderName);

        if ((NULL == cardData))
        {
            ret_val= WRONG_NAME;
            break;
        }

        else
        {
            if (strlen(cardData->cardHolderName)-1 < CARD_HOLDER_NAME_MIN_LEN)
            {
                ret_val= WRONG_NAME;
                printf("Entered name is less than minimum number of characters(20)!, try again.\n");
            }

           else if(strlen(cardData->cardHolderName)-1 > CARD_HOLDER_NAME_MAX_LEN)
            {
                printf("Entered name is more than maximum number of characters(24)!, try again.\n");
            }

            else
            {
                is_num= 0;
                for(index_i=0; index_i<strlen(cardData->cardHolderName)-1; index_i++)
                {
                    if(cardData->cardHolderName[index_i] >= '0' && cardData->cardHolderName[index_i] <= '9')
                    {
                        is_num=1;
                        break;
                    }
                }
                if(is_num == 1)
                {
                    ret_val= WRONG_NAME;
                    printf("Name must be alphabetic characters only.\n");
                }
                else
                {
                    ret_val= CARD_OK;
                    printf("CardHolderName has been Entered successfully.\n");
                }
            }
        }


    }

    return ret_val;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    EN_cardError_t ret_val= WRONG_EXP_DATE;

    while(WRONG_EXP_DATE == ret_val)
    {
        if(NULL == cardData)
        {
            ret_val= WRONG_EXP_DATE;
            break;
        }

        else
        {
            printf("Enter expiry date(MM/YY): ");
            fflush(stdin);
            gets(cardData->cardExpirationDate);

            if(strlen(cardData->cardExpirationDate) != CARD_EXPIRY_DATE_LEN)
               {
                   ret_val= WRONG_EXP_DATE;

                   if(((strlen(cardData->cardExpirationDate)) < CARD_EXPIRY_DATE_LEN))
                    {
                        printf("Entered expiry date is less than minimum number of characters(5)!, try again.\n");
                    }

                    else if(((strlen(cardData->cardExpirationDate)) > CARD_EXPIRY_DATE_LEN))
                    {
                        printf("Entered expiry date is more than maximum number of characters(5)!, try again.\n");
                    }
               }

            else
            {
                if(
                   ((cardData->cardExpirationDate[0] == '0' && cardData->cardExpirationDate[1]<='9') || (cardData->cardExpirationDate[0] == '1' && cardData->cardExpirationDate[1] <= '2')) &&
                   ((cardData->cardExpirationDate[1]>='0') && (cardData->cardExpirationDate[1]<='9')) &&
                   ('/' == cardData->cardExpirationDate[2]) &&
                   ((cardData->cardExpirationDate[3]>='0') && (cardData->cardExpirationDate[3]<='9')) &&
                   ((cardData->cardExpirationDate[4]>='0') && (cardData->cardExpirationDate[4]<='9'))
                   )
                {
                    ret_val=CARD_OK;
                    printf("ExpiryDate has been Entered successfully.\n");
                }

                else if((cardData->cardExpirationDate[0]>='1' && cardData->cardExpirationDate[1] > '2') || (cardData->cardExpirationDate[0] > '1'))
                {
                    printf("You are trying to enter a wrong month.\n");
                }
                else
                {
                    printf("Wrong format.\n");
                }
            }
        }
    }

    return ret_val;
}


EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    EN_cardError_t ret_val=WRONG_PAN;
    uint8_t index_i=0;
    uint8_t flag=0;

    while(WRONG_PAN == ret_val)
    {
        if(NULL == cardData)
        {
            ret_val= WRONG_PAN;
            break;
        }

        else
        {
            printf("Enter PAN: ");
            fflush(stdin);
            gets(cardData->primaryAccountNumber);

            if(strlen(cardData->primaryAccountNumber) < PAN_MIN_LEN)
            {
                ret_val=WRONG_PAN;
                printf("Entered PAN is less than minimum number of characters(16), try again.\n");
            }
            else if(strlen(cardData->primaryAccountNumber)-1 > PAN_MAX_LEN)
            {
                ret_val=WRONG_PAN;
                printf("Entered PAN is greater than number of characters(19), try again.\n");
            }
            else
            {
                for(index_i=0; index_i<strlen(cardData->primaryAccountNumber)-1; index_i++)
                {
                    if(cardData->primaryAccountNumber[index_i] >= '0' && cardData->primaryAccountNumber[index_i] <= '9')
                    {
                        flag=0;
                    }
                    else
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag == 0)
                {
                    ret_val=CARD_OK;
                    printf("CardPAN has been Entered successfully.\n");
                }
                else
                {
                    ret_val= WRONG_PAN;
                    printf("PAN must be numeric characters only.\n");
                }

            }
        }
    }


    return ret_val;
}


void getCardHolderNameTest(void)
{

    ST_cardData_t cardData_test;

    uint8_t index_i=0;
    uint8_t index_j=0;
    uint8_t is_num=0;
    uint8_t Test_Name[4][50]={"adnan", "adnan adel ali mohammed moustafa", "adnan adel 7li moustafa", "adnan adel ali moustafa"};
    for(index_i=0; index_i<4; index_i++)
    {is_num= 0;
        strcpy(cardData_test.cardHolderName, Test_Name[index_i]);
        printf("Name: %s\n", cardData_test.cardHolderName);
        if(strlen(cardData_test.cardHolderName)-1 < CARD_HOLDER_NAME_MIN_LEN)
        {
            printf("Entered name is less than the minimum number of characters(20).\n");
        }
        else if(strlen(cardData_test.cardHolderName)-1 > CARD_HOLDER_NAME_MAX_LEN)
        {
            printf("Entered name is larger than the maximum length of characters(24).\n");
        }
        else
        {
            for(index_j=0; index_j<strlen(cardData_test.cardHolderName)-1; index_j++)
            {
                if(cardData_test.cardHolderName[index_j] >= '0' && cardData_test.cardHolderName[index_j] <= '9')
                {
                    is_num= 1;
                    break;
                }
            }
            if(is_num == 0)
            {
            printf("Name is ok.\n");
            }
            else
            {
                printf("Name must be alphabetic characters only.\n");
            }
        }
        printf("-----------------------------------------------\n");
    }

}


void getCardExpiryDateTest (void)
{
    ST_cardData_t cardData_test;

    uint8_t index_i=0;

    uint8_t Test_Expiry_Date[5][25]={"1/5", "111/888", "10*96","22/22", "11/22"};

    for(index_i =0; index_i <5; index_i++)
    {
        strcpy(cardData_test.cardExpirationDate, Test_Expiry_Date[index_i]);
        printf("expiry date: %s\n", cardData_test.cardExpirationDate);

        if (strlen(cardData_test.cardExpirationDate)-1 < CARD_EXPIRY_DATE_LEN-1)
        {
            printf("Entered expiry date is less than minimum number of characters(5)!, try again.\n");
        }
        else if(strlen(cardData_test.cardExpirationDate)-1 > CARD_EXPIRY_DATE_LEN)
        {
            printf("Entered expiry date is more than maximum number of characters(5)!, try again.\n");
        }
        else
        {
            if(
                   (((cardData_test.cardExpirationDate[0] == '0') && (cardData_test.cardExpirationDate[1] <= '9')) || ((cardData_test.cardExpirationDate[0] == '1') && cardData_test.cardExpirationDate[1] <= '2')) &&
                   ('/' == cardData_test.cardExpirationDate[2]) &&
                   ((cardData_test.cardExpirationDate[3]>='0') && (cardData_test.cardExpirationDate[3]<='9')) &&
                   ((cardData_test.cardExpirationDate[4]>='0') && (cardData_test.cardExpirationDate[4]<='9'))
                   )
                {
                    printf("expiry date is ok.\n");
                }

                else if((cardData_test.cardExpirationDate[0]>='1' && cardData_test.cardExpirationDate[1] > '2') || (cardData_test.cardExpirationDate[0] > '1') )
                {
                    printf("You are trying to enter a wrong month.\n");
                }
                else
                {
                    printf("Wrong format.\n");
                }
        }

        printf("-------------------------\n");
    }
}
void getCardPANTest (void)
{
    ST_cardData_t cardData_test;

    uint8_t index_i=0,index_j=0;
    uint8_t flag=0;

    uint8_t Test_Card_Pan[4][40]={"123456", "12345678963241j874", "123456789012384194567", "12543698745321653"};

    for(index_i =0; index_i <4; index_i++)
    {
        strcpy(cardData_test.primaryAccountNumber, Test_Card_Pan[index_i]);
        printf("PAN: %s\n", cardData_test.primaryAccountNumber);
        if (strlen(cardData_test.primaryAccountNumber)-1 < PAN_MIN_LEN)
        {

            printf("Entered PAN is less than minimum number of characters(16)!.\n");
        }
        else if(strlen(cardData_test.primaryAccountNumber)-1 > PAN_MAX_LEN)
        {

            printf("Entered PAN is more than maximum number of characters(19)!.\n");
        }
        else
        {
            for(index_j=0; index_j<strlen(cardData_test.primaryAccountNumber)-1; index_j++)
            {
                if(Test_Card_Pan[index_i][index_j]>='0' && Test_Card_Pan[index_i][index_j] <= '9')
                {
                    flag=0;
                }
                else
                {
                    flag=1;
                    break;
                }
            }
            if(flag == 0)
            {
                printf("PAN is ok.\n");
            }
            else
            {
                printf("PAM must be numeric characters only.\n");
            }
        }

        printf("-------------------------\n");
    }
}
