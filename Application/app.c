#include <stdio.h>
#include <string.h>
#include "app.h"
#include"../Server/server.h"

ST_cardData_t card_t;
ST_terminalData_t term_t;
ST_transaction_t trans_t;

EN_cardError_t card_err;
EN_terminalError_t ter_err;
EN_serverError_t ser_err;
EN_transStat_t trans_err;

void appStart(void)
{
    /* Getting card data */
    card_err= getCardHolderName(&card_t);
    card_err= getCardExpiryDate(&card_t);
    card_err= getCardPAN(&card_t);

    /* Getting terminal data */
    ter_err= getTransactionDate(&term_t);
    ter_err= isCardExpired(&card_t, &term_t);
    if(EXPIRED_CARD == ter_err)
    {
        printf("Declined Expired Card!!\n");
        return;
    }
    ter_err= setMaxAmount(&term_t, 10000);
    ter_err= getTransactionAmount(&term_t);
    ter_err= isBelowMaxAmount(&term_t);
    if(EXCEED_MAX_AMOUNT == ter_err)
    {
        printf("Declined, Amount Exceeding Limit");
        return;
    }

    /* Checking with server */
    strcpy(trans_t.cardHolderData.cardHolderName, card_t.cardHolderName);
    strcpy(trans_t.cardHolderData.cardExpirationDate, card_t.cardExpirationDate);
    strcpy(trans_t.cardHolderData.primaryAccountNumber, card_t.primaryAccountNumber);

    strcpy(trans_t.terminalData.transactionDate, term_t.transactionDate);
    trans_t.terminalData.maxTransAmount= term_t.maxTransAmount;
    trans_t.terminalData.transAmount= term_t.transAmount;


    trans_err= recieveTransactionData(&trans_t);
    if(DECLINED_INSUFFECIENT_FUND == trans_err || DECLINED_STOLEN_CARD == trans_err
       || FRAUD_CARD == trans_err || INTERNAL_SERVER_ERROR == trans_err)
    {
        return;
    }
    trans_err= saveTransaction(&trans_t);

}
