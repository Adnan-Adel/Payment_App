#include <stdio.h>
#include <string.h>
#include "terminal.h"

#define TRANSACTION_DATE_LEN 10

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    EN_terminalError_t ret_val= WRONG_DATE;

    while(WRONG_DATE == ret_val)
    {
         printf("Enter transaction date: ");
        gets(termData->transactionDate);

        if(NULL == termData)
        {
            ret_val= WRONG_DATE;
            break;
        }
        else
        {
            if(strlen(termData->transactionDate)< TRANSACTION_DATE_LEN)
            {
                ret_val= WRONG_DATE;
                printf("Entered transaction date is less than transaction date length(10).\n");
            }
            else
            {
                if(((termData->transactionDate[0] <= '2') || ((termData->transactionDate[0] == '3') && (termData->transactionDate[4] <= '1'))) &&
                   (termData->transactionDate[2] == '/') &&
                   (((termData->transactionDate[3] == '0') && termData->transactionDate[4] <= '9') || ((termData->transactionDate[3] == '1') && termData->transactionDate[4] <= '2')) &&
                   (termData->transactionDate[5] == '/') &&
                   (termData->transactionDate[6] >= '0') && (termData->transactionDate[6] <= '9') &&
                   (termData->transactionDate[7] >= '0') && (termData->transactionDate[7] <= '9') &&
                   (termData->transactionDate[8] >= '0') && (termData->transactionDate[8] <= '9') &&
                   (termData->transactionDate[9] >= '0') && (termData->transactionDate[9] <= '9') )
                {
                    ret_val= TERMINAL_OK;
                    printf("Transaction date has been entered successfully.\n");
                }

               else if(((termData->transactionDate[0] >= '3') && (termData->transactionDate[1] > '1')) || (termData->transactionDate[0] > '3'))
                {
                    ret_val= WRONG_DATE;
                   printf("You are trying to enter a wrong day.\n");
                }
                else if(((termData->transactionDate[3] >= '1') && (termData->transactionDate[4] > '2')) || (termData->transactionDate[3] > '1'))
                {
                    ret_val= WRONG_DATE;
                    printf("You are trying to enter a wrong month.\n");
                }
                else
                {
                    ret_val= WRONG_DATE;
                    printf("Wrong format, try again.\n");
                }
            }
        }
    }

        return ret_val;

}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    EN_terminalError_t ret_val=EXPIRED_CARD;

    uint8_t expiry_date_year[3];
    uint8_t expiry_date_y;
    uint8_t expiry_date_month[3];
    uint8_t expiry_date_m;

    uint8_t trans_date_year[3];
    uint8_t trans_date_y;
    uint8_t trans_date_month[3];
    uint8_t trans_date_m;

    expiry_date_year[0]= cardData->cardExpirationDate[3];
    expiry_date_year[1]= cardData->cardExpirationDate[4];
    expiry_date_y=atoi(expiry_date_year);

    expiry_date_month[0]= cardData->cardExpirationDate[0];
    expiry_date_month[1]= cardData->cardExpirationDate[1];
    expiry_date_m=atoi(expiry_date_month);

    trans_date_year[0]= termData->transactionDate[8];
    trans_date_year[1]= termData->transactionDate[9];
    trans_date_y= atoi(trans_date_year);

    trans_date_month[0]= termData->transactionDate[3];
    trans_date_month[1]= termData->transactionDate[4];
    trans_date_m= atoi(trans_date_month);

        if(NULL == termData || NULL == cardData)
        {
            ret_val = EXPIRED_CARD;
        }
        else
        {
            if(trans_date_y > expiry_date_y)
            {
                ret_val=EXPIRED_CARD;
                printf("Card is expired.\n");
            }
            else if((trans_date_y == expiry_date_y) && (trans_date_m > expiry_date_m))
            {
                ret_val=EXPIRED_CARD;
                printf("Card is expired.\n");
            }
            else
            {

                ret_val= TERMINAL_OK;
                printf("Card is OK.\n");
            }
        }
    return ret_val;
}


EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    EN_terminalError_t ret_val= INVALID_AMOUNT;

    while(INVALID_AMOUNT == ret_val)
    {
        if(NULL == termData)
        {
            ret_val =INVALID_AMOUNT;
            break;
        }
        else
        {
            printf("Enter transaction amount: ");
            scanf("%f", &termData->transAmount);

            if(0 >= termData->transAmount)
            {
                ret_val= INVALID_AMOUNT;
                printf("Entered Transaction amount is less than or equal to (0).\n");
            }
            else
            {
                ret_val= TERMINAL_OK;
                printf("Transaction amount has been entered successfully.\n");
            }
        }
    }
    return ret_val;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    EN_terminalError_t ret_val= EXCEED_MAX_AMOUNT;
    if(NULL == termData)
    {
        ret_val= EXCEED_MAX_AMOUNT;
    }
    else
    {
        if(termData->transAmount > termData->maxTransAmount)
        {
            ret_val= EXCEED_MAX_AMOUNT;
            printf("Entered transaction amount exceeds maximum amount.\n");
        }
        else
        {
            ret_val=TERMINAL_OK;
            printf("Transaction amount is ok.\n");
        }
    }
    return ret_val;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
    EN_terminalError_t ret_val= INVALID_MAX_AMOUNT;
    while(INVALID_MAX_AMOUNT == ret_val)
    {
        if(NULL == termData)
        {
            ret_val= INVALID_MAX_AMOUNT;
        }
        else
        {
            printf("max amount= %0.2f\n", maxAmount);
            if(maxAmount <= 0)
            {
                ret_val= INVALID_MAX_AMOUNT;
                printf("Entered MAX amount is less than or equal to (0).\n");
                break;
            }
            else
            {
                ret_val= TERMINAL_OK;
                termData->maxTransAmount= maxAmount;
            }
        }
    }

    return ret_val;
}


void getTransactionDateTest(void)
{
        ST_terminalData_t term_test;
        uint8_t index_i=0;
        uint8_t transactionDate_test[5][11]={"06/11/200", "06/11*2002", "21/19/2021", "35/12/2001", "31/11/2002"};
        for(index_i= 0; index_i<5; index_i++)
        {
            strcpy(term_test.transactionDate, transactionDate_test[index_i]);

            printf("Entered transaction date: %s\n", transactionDate_test[index_i]);
            if(strlen(term_test.transactionDate)< TRANSACTION_DATE_LEN)
            {
                printf("Entered transaction date is less than transaction date length(10).\n");
            }
            else
            {
                if(((term_test.transactionDate[0] <= '2') || ((term_test.transactionDate[0] == '3') && (term_test.transactionDate[4] <= '1'))) &&
                   (term_test.transactionDate[2] == '/') &&
                   (((term_test.transactionDate[3] == '0') && term_test.transactionDate[4] <= '9') || ((term_test.transactionDate[3] == '1') && term_test.transactionDate[4] <= '2')) &&
                   (term_test.transactionDate[5] == '/') &&
                   (term_test.transactionDate[6] >= '0') && (term_test.transactionDate[6] <= '9') &&
                   (term_test.transactionDate[7] >= '0') && (term_test.transactionDate[7] <= '9') &&
                   (term_test.transactionDate[8] >= '0') && (term_test.transactionDate[8] <= '9') &&
                   (term_test.transactionDate[9] >= '0') && (term_test.transactionDate[9] <= '9') )
                {
                    printf("Transaction date has been entered successfully.\n");
                }

                else if(((term_test.transactionDate[0] >= '3') && (term_test.transactionDate[1] > '1')) || (term_test.transactionDate[0] > '3'))
               {
                   printf("You are trying to enter a wrong day.\n");
               }
                else if(((term_test.transactionDate[3] >= '1') && (term_test.transactionDate[4] > '2')) || (term_test.transactionDate[3] > '1'))
                {
                    printf("You are trying to enter a wrong month.\n");
                }
                else
                {
                    printf("Wrong format, try again.\n");
                }
            }
            printf("---------------------------------\n");
        }
}
void isCardExpriedTest(void)
{
    ST_terminalData_t termData;
    ST_cardData_t cardData;

    uint8_t index_i=0;

    uint8_t expiry_date_year[3];
    uint8_t expiry_date_y;
    uint8_t expiry_date_month[3];
    uint8_t expiry_date_m;

    uint8_t trans_date_year[3];
    uint8_t trans_date_y;
    uint8_t trans_date_month[3];
    uint8_t trans_date_m;

    uint8_t termDate_t[3][11]={"06/05/2023", "06/08/2022", "06/11/2022"};
    uint8_t expiryDate_t[3][11]={"11/19", "5/22", "11/24"};
    for(index_i=0; index_i<3; index_i++)
    {
        strcpy(termData.transactionDate, termDate_t[index_i]);
        strcpy(cardData.cardExpirationDate, expiryDate_t[index_i]);
        printf("transaction date: %s\n", termData.transactionDate);
        printf("Expiry date: %s\n", cardData.cardExpirationDate);

        expiry_date_year[0]= cardData.cardExpirationDate[3];
        expiry_date_year[1]= cardData.cardExpirationDate[4];
        expiry_date_y=atoi(expiry_date_year);

        expiry_date_month[0]= cardData.cardExpirationDate[0];
        expiry_date_month[1]= cardData.cardExpirationDate[1];
        expiry_date_m=atoi(expiry_date_month);

        trans_date_year[0]= termData.transactionDate[8];
        trans_date_year[1]= termData.transactionDate[9];
        trans_date_y= atoi(trans_date_year);

        trans_date_month[0]= termData.transactionDate[3];
        trans_date_month[1]= termData.transactionDate[4];
        trans_date_m= atoi(trans_date_month);
        if(trans_date_y > expiry_date_y)
        {
            printf("Card is expired.\n");
        }
        else if((trans_date_y == expiry_date_y) && (trans_date_m > expiry_date_m))
        {
            printf("Card is expired.\n");
        }
        else
        {
            printf("Card is OK.\n");
        }
        printf("---------------------------\n");

    }
}
void getTransactionAmountTest(void)
{
    ST_terminalData_t termData;
    uint8_t index_i;
    float32_t tr_amount_test[2]={-500, 2000};
    for(index_i= 0; index_i<2; index_i++)
    {
        termData.transAmount=tr_amount_test[index_i];
        printf("Transaction amount= %0.2f\n", termData.transAmount);
        if(0 >= termData.transAmount)
        {
            printf("Entered Transaction amount is less than or equal to (0).\n");
        }
        else
        {
            printf("Transaction amount has been entered successfully.\n");
        }

        printf("--------------------------------\n");
    }
}
void isBelowMaxAmountTest(void)
{
    ST_terminalData_t termData;
    uint8_t index_i;
    termData.maxTransAmount=7000;
    float32_t tr_amount_test[2]={9000, 2000};
    for(index_i= 0; index_i<2; index_i++)
    {
        termData.transAmount=tr_amount_test[index_i];
        printf("Transaction amount = %0.2f\n", termData.transAmount);
        printf("Maximum amount = %0.2f\n", termData.maxTransAmount);

        if(termData.transAmount > termData.maxTransAmount)
        {
            printf("Entered transaction amount exceeds maximum amount.\n");
        }
        else
        {
            printf("Transaction amount is ok.\n");
        }
        printf("--------------------------------\n");
    }
}
void setMaxAmountTest(void)
{
    ST_terminalData_t termData;
    uint8_t index_i;
    float32_t tr_max_amount_test[2]={-500, 9000};
    for(index_i= 0; index_i<2; index_i++)
    {
        termData.maxTransAmount= tr_max_amount_test[index_i];
        printf("Entered max amount: %0.2f\n", termData.maxTransAmount);
        if(termData.maxTransAmount <= 0)
        {
            printf("Entered MAX amount is less than or equal to (0).\n");
        }
        else
        {
            printf("MAX amount has been entered successfully.\n");
        }

        printf("------------------------------------\n");
    }
}
