#include <stdio.h>
#include <string.h>
#include "server.h"

uint32_t acc_num;

uint32_t seq_num=0;

ST_accountsDB_t accountsDB[255]={{2000.0, RUNNING, "8989374615436851"},
                                 {100000.0, BLOCKED, "5807007076043875"},
                                 {80000.0, BLOCKED, "4568419523678415"},
                                 {30000.0, RUNNING, "9621354709632058"},
                                 {44000000.0, RUNNING, "1254369875213008"},
                                 {550000.0, RUNNING, "1408709635124789"},
                                 {25000.0, RUNNING, "2543698740015236"},
                                 {975000.0, BLOCKED, "9856471239513247"},
                                 {35000.0, BLOCKED, "7852360014078096"},
                                 {62000.0, RUNNING, "8523694107809635"},
                                 };

ST_transaction_t Transactions[255]={0};


EN_transStat_t recieveTransactionData(ST_transaction_t *transData)
{
    EN_transStat_t ret_val= APPROVED;
    transData->transState= APPROVED;
    if(NULL == transData)
    {
        ret_val= INTERNAL_SERVER_ERROR;
    }
    else
    {

        if(SERVER_OK != (isValidAccount(&transData->cardHolderData, &accountsDB)))
        {
            transData->transState= FRAUD_CARD;
            ret_val= FRAUD_CARD;
            printf("PAN doesn't exist!!.\n");
        }

        else if(SERVER_OK != (isBlockedAccount(transData, &accountsDB)))
        {
            transData->transState= DECLINED_STOLEN_CARD;
            ret_val= DECLINED_STOLEN_CARD;
            printf("Stolen card!!.\n");
        }
        else if(SERVER_OK != (isAmountAvailable(&transData->terminalData, &accountsDB)))
        {

            transData->transState= DECLINED_INSUFFECIENT_FUND;
            ret_val= DECLINED_INSUFFECIENT_FUND;
            printf("Insufficient fund!!.\n");
        }
        else
        {
            printf("Transaction has been done successfully.\n");
        }
    }
    return ret_val;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
    EN_serverError_t ret_val= APPROVED;
    uint8_t index_i, index_j;
    accountRefrence= &accountsDB;
    if((NULL == cardData) || (NULL == accountRefrence))
    {
        ret_val= ACCOUNT_NOT_FOUND;
    }
    else
    {
        for(index_i=0; index_i<10; index_i++)
        {
            for(index_j=0; index_j< 20; index_j++)
            {
                if(cardData->primaryAccountNumber[index_j] == *(&accountRefrence[index_i].primaryAccountNumber[index_j]))
                {
                    ret_val= APPROVED;
                }
                else
                {
                    ret_val= ACCOUNT_NOT_FOUND;
                    break;
                }
            }

            if(APPROVED == ret_val)
            {
                printf("Account exists.\n");
                acc_num= index_i;
                return SERVER_OK;
                break;
            }
        }

        if(ACCOUNT_NOT_FOUND == ret_val)
        {
            printf("Account not found.\n");
        }
    }

    return ret_val;
}

EN_serverError_t isBlockedAccount(ST_transaction_t *transData, ST_accountsDB_t *accountRefrence)
{
    EN_serverError_t ret_val= ACCOUNT_NOT_FOUND;
    uint8_t index_i, index_j;
    uint8_t flag=0;

    if((NULL == accountRefrence))
    {
        ret_val= ACCOUNT_NOT_FOUND;
    }
    else
    {
        for(index_i=0; index_i<10; index_i++)
        {
            for(index_j=0; index_j< 20; index_j++)
            {
                if(transData->cardHolderData.primaryAccountNumber[index_j] == *(&accountRefrence[index_i].primaryAccountNumber[index_j]))
                {
                    flag++;
                }
                else
                {
                    ret_val= ACCOUNT_NOT_FOUND;
                    break;
                }
            }

            if(20 == flag)
            {
                ret_val= SERVER_OK;
                if((&accountRefrence[index_i])->state == RUNNING)
                {
                    ret_val= SERVER_OK;
                }
                else
                {
                    ret_val= BLOCKED_ACCOUNT;
                }
                break;
            }
            flag=0;
        }

        if(ACCOUNT_NOT_FOUND == ret_val)
        {
            printf("Account not found.\n");
        }

    }

    return ret_val;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
    EN_serverError_t ret_val;
    uint8_t index_i=0;

    if((NULL == termData) || (NULL == accountRefrence))
    {
        ret_val= LOW_BALANCE;
    }
    else
    {
        if(termData->transAmount > ((&accountRefrence[acc_num])->balance))
        {
            ret_val= LOW_BALANCE;
            printf("The transaction amount is greater than the balance.\n");
        }
        else
        {
            ret_val= SERVER_OK;
        }
    }

    return ret_val;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    EN_transStat_t trans_state;
    EN_serverError_t ret_val= SERVER_OK;

    if(NULL == transData)
    {
        ret_val= INTERNAL_SERVER_ERROR;
    }
    else
    {
        seq_num++;
        Transactions[seq_num].transactionSequenceNumber= seq_num;
        Transactions[seq_num].cardHolderData= transData->cardHolderData;
        Transactions[seq_num].terminalData=transData->terminalData;
        Transactions[seq_num].transState=transData->transState;
        accountsDB[acc_num].balance= accountsDB[acc_num].balance- transData->terminalData.transAmount;
        printf("Your new balance: %0.2f\n", accountsDB[acc_num].balance);
    }
    return ret_val;
}

void listSavedTransactions(void)
{
    uint8_t index_i=0;

    if(0 == seq_num)
    {
        printf("There's no transactions.\n");
    }
    else
    {
        printf("#########################\n");

        printf("Transaction Sequence Number: %i\n", Transactions[seq_num].transactionSequenceNumber-1);

        printf("Cardholder Name: %s\n", Transactions[seq_num].cardHolderData.cardHolderName);
        printf("PAN: %s\n", Transactions[seq_num].cardHolderData.primaryAccountNumber);
        printf("Card Expiration Date: %s\n", Transactions[seq_num].cardHolderData.cardExpirationDate);


        printf("Transaction Date: %s\n", Transactions[seq_num].terminalData.transactionDate);
        printf("Transaction Amount: %0.2f\n", Transactions[seq_num].terminalData.transAmount);
        printf("Terminal Max Amount: %0.2f\n", Transactions[seq_num].terminalData.maxTransAmount);
        switch (Transactions[seq_num].transState)
        {
        case 0:
            printf("Transaction State: APPROVED\n");
            break;
        case 1:
            printf("Transaction State: DECLINED_INSUFFECIENT_FUND\n");
            break;
        case 2:
            printf("Transaction State: DECLINED_STOLEN_CARD\n");
            break;
        case 3:
            printf("Transaction State: FRAUD_CARD\n");
            break;
        default:
            printf("Transaction State: INTERNAL_SERVER_ERROR\n");
            break;
        }




        printf("#########################\n");
    }
}

void recieveTransactionDataTest(void)
{
    ST_transaction_t transData;
    EN_transStat_t err1;
    EN_serverError_t err2;
    uint8_t index_i;
    uint8_t Test_Card_Pan[5][40]={"8989374615436851", "8989374615436851", "5807007076043875", "12345678901234567"};
    float32_t trans_amount_t[5]= {5000, 1000, 3000, 4000 ,6000};
    for(index_i=0; index_i<4; index_i++)
    {
        strcpy(transData.cardHolderData.primaryAccountNumber, Test_Card_Pan[index_i]);
        transData.terminalData.transAmount= trans_amount_t[index_i];
        printf("PAN: %s\n", Test_Card_Pan[index_i]);
        err1= recieveTransactionData(&transData);
        printf("----------------------------------\n");
    }

}
void isValidAccountTest(void)
{
    uint8_t index_i, index_j, index_k=0;
    uint8_t flag=0;
    uint8_t t=0;
    ST_cardData_t cardData;
    ST_accountsDB_t *accountRefrence;
    accountRefrence= &accountsDB;
    uint8_t Test_Card_Pan[3][40]={"8989374615436851", "12345678901234567"};
    for(index_k=0; index_k<2; index_k++)
    {
        strcpy(cardData.primaryAccountNumber, Test_Card_Pan[index_k]);
        printf("Entered PAN: %s\n", (cardData.primaryAccountNumber));

        for(index_i=0; index_i<10; index_i++)
            {
                for(index_j=0; index_j< 20; index_j++)
                {
                    if(cardData.primaryAccountNumber[index_j] == *(&accountRefrence[index_i].primaryAccountNumber[index_j]))
                    {
                        flag++;
                    }
                    else
                    {
                        break;
                    }
                }
                if(17 == flag)
                {

                    printf("Account exists.\n");
                    break;
                }

                else
                {
                   flag=0;
                   t++;
                   printf("%i\n", t);
                }

            }
            flag=0;
        if(flag == 0 && t == 10)
        {
            printf("Account not found.\n");
        }
    }
}

void isBlockedAccountTest(void)
{
    ST_accountsDB_t *accountRefrence;
    EN_accountState_t test_accstate[2]={RUNNING, BLOCKED};
    uint8_t index_i;
    printf("0: Acc is ok \t 1: Acc is blocked\n");
    for(index_i=0; index_i<2; index_i++)
    {
        printf("Account state: %i\n", test_accstate[index_i]);
        (&accountRefrence[acc_num])->state=test_accstate[index_i];
        if((&accountRefrence[acc_num])->state == RUNNING)
        {
            printf("Account is ok.\n");
        }
        else
        {
            printf("Blocked account.\n");
        }
    }

}

void isAmountAvailableTest(void)
{
    uint8_t index_i;
    float32_t trans_amount_t[2]={300000, 200};
    ST_terminalData_t *termData;
    ST_accountsDB_t *accountRefrence;
    accountRefrence=&accountsDB;
    printf("balance= %0.2f\n", ((&accountRefrence[acc_num])->balance));
    for(index_i=0; index_i<2; index_i++)
    {
        printf("Entered amount: %0.2f\t", trans_amount_t[index_i]);
        if(trans_amount_t[index_i]> ((&accountRefrence[acc_num])->balance))
        {
            printf("The transaction amount is greater than the balance(%0.2f).\n", ((&accountRefrence[acc_num])->balance));
        }
        else
        {
            printf("amount is available.\n");
        }
    }


}

void listSavedTransactionsTest(void)
{
    ST_transaction_t transData;
    EN_transStat_t err1;
    EN_serverError_t err2;

    uint8_t cardHolderName_t[25]="adnan adel ali moustafa";
    uint8_t primaryAccountNumber_t[20]="9621354709632058";
    uint8_t cardExpirationDate_t[6]="09/12";

    float transAmount_t= 5000;
    float maxTransAmount_t= 60000;
    uint8_t transactionDate_t[11]="06/11/2022";

   strcpy(transData.cardHolderData.cardHolderName, cardHolderName_t);
   strcpy(transData.cardHolderData.primaryAccountNumber, primaryAccountNumber_t);
   strcpy(transData.cardHolderData.cardExpirationDate, cardExpirationDate_t);

   strcpy(transData.terminalData.transactionDate, transactionDate_t);
   transData.terminalData.maxTransAmount= maxTransAmount_t;
   transData.terminalData.transAmount= transAmount_t;

   err1= recieveTransactionData(&transData);
   err2= saveTransaction(&transData);
   listSavedTransactions();

}

