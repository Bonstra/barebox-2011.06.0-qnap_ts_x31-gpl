#include "legerity_event_hdlr.h"

extern struct legerity_data *legerity;

/**
* legerity_slicline_to_pots_id -
*
*/
int legerity_slicline_to_pots_id(int slic_id, int line_id)
{
    int pots = 0;

        struct legerity_slic_desc *slic = legerity->slic_list;

        pots += slic->slicMaxLines;
    pots += line_id;

    return pots;
}

/**
* legerity_pots_to_slic_id -
*
*/
int legerity_pots_to_slic_id(int pots)
{
    int slic_id=0;

        struct legerity_slic_desc *slic = legerity->slic_list;

        if (pots < slic->slicMaxLines)
            return slic_id;

        pots -= slic->slicMaxLines;

    return -1;
}

/**
* legerity_pots_to_line_id -
*
*/
int legerity_pots_to_line_id(int pots)
{
        struct legerity_slic_desc *slic = legerity->slic_list;

        if (pots < slic->slicMaxLines)
            return pots;

        pots -= slic->slicMaxLines;

    return -1;
}

/*
 *  legerity_eventcat_siganling_handler - handler for signaling type events
*/
void legerity_eventcat_signaling_handler(VpEventType pEvent)
{
    struct legerity_slic_desc* slic = legerity->slic_list;
    int res;

    switch (pEvent.eventId) {
        case VP_LINE_EVID_HOOK_OFF:
            printf("LEGERITY DEVICE %d LINE %d OFFHOOK\n", pEvent.deviceId, pEvent.channelId);

            /* forward event to wan module */
            if (legerity->itf) {
                switch (slic->line_list[pEvent.channelId].state) {
                    case POTS_STATE_ONHOOK:
                        legerity->itf->incoming_call(legerity->itf, legerity_slicline_to_pots_id(pEvent.deviceId, pEvent.channelId));
                        break;

                    case POTS_STATE_OFFHOOK:
                        printf("Offhook detection on already offhooked line on device %d line %d\n", pEvent.deviceId, pEvent.channelId);
                        break;
                }
            }

            /* have slic driver line */
            res = VpSetLineState(pEvent.pLineCtx, VP_LINE_TALK);
            if (res != VP_STATUS_SUCCESS) printf("VpSetLineState returned %d\n", res);

            if (pEvent.pDevCtx->deviceType == VP_DEV_880_SERIES) {
                /* turn led on */
                slic->io.accessMask_31_0 = 1<<((pEvent.channelId*2)+1);
                slic->io.deviceIOData_31_0 = 0;
                res = VpDeviceIoAccess(pEvent.pDevCtx, &slic->io);
                if (res != VP_STATUS_SUCCESS) ;//PRINT(KERN_ERR "VpDeviceIoAccess returned %d\n", res);
            }

            /* update line state */
            slic->line_list[pEvent.channelId].state = POTS_STATE_OFFHOOK;
            break;

        case VP_LINE_EVID_HOOK_ON:
            printf("LEGERITY DEVICE %d LINE %d ONHOOK\n", pEvent.deviceId, pEvent.channelId);

            /* forward event to wan module */
            if (legerity->itf) {
                switch (slic->line_list[pEvent.channelId].state) {
                    case POTS_STATE_ONHOOK:
                        printf("Onhook detection on already onhooked line on device %d line %d\n", pEvent.deviceId, pEvent.channelId);
                        break;

                    case POTS_STATE_OFFHOOK:
                        legerity->itf->disconnect(legerity->itf, legerity_slicline_to_pots_id(pEvent.deviceId, pEvent.channelId));
                        break;
                }
            }

            /* have slic stop driving the line */
            res = VpSetLineState(pEvent.pLineCtx, VP_LINE_STANDBY);
            if (res != VP_STATUS_SUCCESS) ;//PRINT(KERN_ERR "VpSetLineState returned %d\n", res);

            if (pEvent.pDevCtx->deviceType == VP_DEV_880_SERIES) {
                /* turn led off */
                slic->io.accessMask_31_0 = 1<<((pEvent.channelId*2)+1);
                slic->io.deviceIOData_31_0 = 1<<((pEvent.channelId*2)+1);
                res = VpDeviceIoAccess(pEvent.pDevCtx, &slic->io);
                if (res != VP_STATUS_SUCCESS) ;//PRINT(KERN_ERR "VpDeviceIoAccess returned %d\n", res);
            }

            /* update line state */
            slic->line_list[pEvent.channelId].state = POTS_STATE_ONHOOK;
            break;

        case VP_LINE_EVID_FLASH :
#ifdef LEGERITY_FLASH
            if (legerity->itf)
                legerity->itf->flash(legerity->itf, legerity_slicline_to_pots_id(pEvent.deviceId, pEvent.channelId));
#endif
            break;

        case VP_LINE_EVID_STARTPULSE :
        case VP_LINE_EVID_DTMF_DIG :
        case VP_LINE_EVID_PULSE_DIG :
        case VP_LINE_EVID_MTONE :
            break;

        default:
            printf("legerity_eventcat_signaling_handler() - Nothing done with event %d !!!!\n",pEvent.eventId);
            break;
    }
    return;
}

/*
 *  legerity_eventcat_response_handler - handler for response type events
 */

void legerity_eventcat_response_handler(VpEventType pEvent)
{

    struct legerity_slic_desc *slic = legerity->slic_list;
    VpDevCtxType *pDevCtx = pEvent.pDevCtx;
    VpOptionEventMaskType eventMask;
    int i, res;
    VpOptionTimeslotType timeslot;
#ifdef LEGERITY_FLASH
    VpOptionPulseType pulseparam;
    int pulsemode=VP_OPTION_PULSE_DECODE_ON;
#endif

    switch (pEvent.eventId) {
        case VP_DEV_EVID_DEV_INIT_CMP:
            printf("LEGERITY DEVICE %d INIT COMPLETE\n", pEvent.deviceId);

            /* slic finished initializing, apply our custom configuration */
            res = VpSetOption (VP_NULL, pDevCtx, VP_OPTION_ID_CODEC, &legerity->tdm_coding);

#ifdef LEGERITY_FLASH
            /*Activate PULSE decode to have flash detection*/
            if ((legerity->flash_time != 0) && ((slic->termType == VP_TERM_FXS_GENERIC) || (slic->termType == VP_TERM_FXS_ISOLATE)|| (slic->termType == VP_TERM_FXS_LOW_PWR))) {
                res |= VpSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_PULSE_MODE, &pulsemode);
                pulseparam.breakMin = 8* 33;
                pulseparam.breakMax = 8* 100;
                pulseparam.makeMin = 8* 17;
                pulseparam.makeMax = 8* 75;
                pulseparam.interDigitMin = 8* 250;
                pulseparam.flashMin = 8* 1;
                pulseparam.flashMax = 8* legerity->flash_time;
                res |= VpSetOption(VP_NULL, pDevCtx, VP_DEVICE_OPTION_ID_PULSE, &pulseparam);
            }
#endif
            if (res != VP_STATUS_SUCCESS) printf("VpSetOption returned %d\n", res);

            for (i = 0; i < slic->slicMaxLines; i++) {
                /* FXO settings */
                if ( slic->termType == VP_TERM_FXO_GENERIC )  {
                    /* define default ring cadence */
                    if (res != VP_STATUS_SUCCESS) printf("VpInitRing returned %d\n", res);

                    /* have slic not driver the line */
                    res = VpSetLineState(&slic->line_list[i].context, VP_LINE_FXO_LOOP_OPEN);
                    if (res != VP_STATUS_SUCCESS) PRINT("VpSetLineState returned %d\n", res);
                    /* define timeslot the line should drive */
                    timeslot.tx = timeslot.rx =4;
                    res = VpSetOption(&slic->line_list[i].context, VP_NULL, VP_OPTION_ID_TIMESLOT, &timeslot);
                    if (res != VP_STATUS_SUCCESS) PRINT("VpSetOption returned %d\n", res);

                    #ifdef LEGERITY_DEBUG
                    PRINT(KERN_INFO "********************************\n");
                    PRINT(KERN_INFO "Device-ID =%d\n",pEvent.deviceId);
                    PRINT(KERN_INFO "MAX-NO-SLIC-LINES =%d\n",slic->slicMaxLines);
                    PRINT(KERN_INFO "LINE %d TX-TIME-SLOT =%d RX-TIME-SLOT %d\n",i,timeslot.tx,timeslot.rx);
                    PRINT(KERN_INFO "********************************\n");
                    #endif

                    /* define initial line state */
                    slic->line_list[i].state = POTS_STATE_ONHOOK;
                    if(i==0) {
                        VpGetOption(&slic->line_list[0].context, VP_NULL, VP_OPTION_ID_EVENT_MASK, 0);
                    }
                } else {
                    /* define default ring cadence */
                    res = VpInitRing(&slic->line_list[i].context, slic->ringCadence, VP_PTABLE_NULL);
                    if (res != VP_STATUS_SUCCESS) ;//PRINT(KERN_ERR "VpInitRing returned %d\n", res);

                    /* have slic not driver the line */
                    res = VpSetLineState(&slic->line_list[i].context, VP_LINE_STANDBY);
                    if (res != VP_STATUS_SUCCESS) ;//PRINT(KERN_ERR "VpSetLineState returned %d\n", res);

                    res = VpCalLine(&slic->line_list[i].context);

                    /* define timeslot the line should drive */
                    timeslot.tx = timeslot.rx = (pEvent.deviceId*slic->slicMaxLines) + i;    /* Seperate by 2 for WB Mode */
                    res = VpSetOption(&slic->line_list[i].context, VP_NULL, VP_OPTION_ID_TIMESLOT, &timeslot);
                    if (res != VP_STATUS_SUCCESS) ;//PRINT(KERN_ERR "VpSetOption returned %d\n", res);

                    #ifdef LEGERITY_DEBUG
                    PRINT(KERN_INFO "********************************\n");
                    PRINT(KERN_INFO "Device-ID =%d\n",pEvent.deviceId);
                    PRINT(KERN_INFO "MAX-NO-SLIC-LINES =%d\n",slic->slicMaxLines);
                    PRINT(KERN_INFO "LINE %d TX-TIME-SLOT =%d RX-TIME-SLOT %d\n",i,timeslot.tx,timeslot.rx);
                    PRINT(KERN_INFO "********************************\n");
                    #endif

                    /* define initial line state */
                    slic->line_list[i].state = POTS_STATE_ONHOOK;

                    if(i==0) {
                        VpGetOption(&slic->line_list[0].context, VP_NULL, VP_OPTION_ID_EVENT_MASK, 0);
                    }
                }
            }
            break;

        case VP_LINE_EVID_RD_OPTION:
            /* this is the last step to a device initialization phase */
            #ifdef LEGERITY_DEBUG
            PRINT(KERN_INFO "LEGERITY DEVICE %d READ OPTION\n", pEvent.deviceId);
            #endif

            VpGetResults(&pEvent, &eventMask);
            eventMask.signaling &= ~(VP_LINE_EVID_HOOK_OFF | VP_LINE_EVID_HOOK_ON);
            eventMask.fxo = VP_EVCAT_FXO_UNMASK_ALL;// fXo implementation FXO

            if( (VpSetOption(VP_NULL, pDevCtx, VP_OPTION_ID_EVENT_MASK, &eventMask)) == VP_STATUS_SUCCESS) {
                PRINT("All FXO Event are UnMASKED -LEGERITY DEVICE %d READ OPTION\n", pEvent.deviceId);
            } else {
                PRINT("FAIED to amke All FXO Event are UnMASKED -LEGERITY DEVICE %d READ OPTION\n", pEvent.deviceId);
            }

#ifdef LINE_TESTING_ENABLED
            switch (legerity->lt_status) {
                case LT_INIT_SLIC:
                    legerity->lt_status = LT_INIT_SLIC_COMPLETE;
                    break;

                case LT_REBOOT_SLIC:
                    legerity->lt_status = LT_REBOOT_SLIC_COMPLETE;
                    break;

                default:
                    break;
            }
#endif
            break;

        case VP_LINE_EVID_LINE_INIT_CMP:
            PRINT("LEGERITY DEVICE %d LINE %d INIT COMPLETE\n", pEvent.deviceId, pEvent.channelId);
            break;

        case VP_EVID_CAL_CMP:
            PRINT("LEGERITY DEVICE %d CALIBRATION COMPLETE\n", pEvent.deviceId);
            break;

        case VP_DEV_EVID_IO_ACCESS_CMP:
            PRINT("LEGERITY DEVICE %d IO ACCESS COMPLETE\n", pEvent.deviceId);
            break;

        default:
            PRINT ("legerity_eventcat_response_handler() ID(%x) - Nothing done with event !!!!\n", pEvent.eventId);
            break;
    }
    return;
}

/*
 *  legerity_event_handler - dispatcher for slic events
 */
void legerity_event_handler(VpEventType pEvent)
{
    switch (pEvent.eventCategory)
    {
        case VP_EVCAT_SIGNALING :
            legerity_eventcat_signaling_handler(pEvent);
            break;

        case VP_EVCAT_RESPONSE :
            legerity_eventcat_response_handler(pEvent);
            break;

        default:

            printf ("legerity_event_handler() CAT(%x) ID(%x) - Nothing done with event !!!!\n", pEvent.eventCategory, pEvent.eventId);
            break;
    }
    return;
}

