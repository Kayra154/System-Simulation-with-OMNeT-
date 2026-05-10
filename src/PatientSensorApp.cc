#include "PatientSensorApp.h"

Define_Module(PatientSensorApp);
using namespace flora;

PatientSensorApp::PatientSensorApp() {
    fprintf(stderr, "===== PatientSensorApp CONSTRUCTOR =====\n");
}

PatientSensorApp::~PatientSensorApp() {

}
PatientSensorApp::PatientSensorApp(const PatientSensorApp &other) {

}

void PatientSensorApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == sendMeasurements) {
            sendJoinRequest();
            if (simTime() >= getSimulation()->getWarmupPeriod())
                sentPackets++;
            delete msg;
            if (numberOfPacketsToSend == 0 || sentPackets < numberOfPacketsToSend) {
                double time;
                int loRaSF = getSF();
                if(loRaSF == 7) time = 7.808;
                if(loRaSF == 8) time = 13.9776;
                if(loRaSF == 9) time = 24.6784;
                if(loRaSF == 10) time = 49.3568;
                if(loRaSF == 11) time = 85.6064;
                if(loRaSF == 12) time = 171.2128;
                do {
                    timeToNextPacket = par("timeToNextPacket");
                } while(timeToNextPacket <= time);
                sendMeasurements = new cMessage("sendMeasurements");
                scheduleAt(simTime() + timeToNextPacket, sendMeasurements);
            }
        }
    } else {
        handleMessageFromLowerLayer(msg);
        delete msg;
    }
}

void PatientSensorApp::initialize(int stage)
{
    SimpleLoRaApp::initialize(stage);
    fprintf(stderr, "===== PatientSensorApp IS RUNNING =====\n");
}

void PatientSensorApp::handleMessageFromLowerLayer(cMessage *msg)
{
    // Just delegate to parent — node doesn't need to decode downlink for now
    SimpleLoRaApp::handleMessageFromLowerLayer(msg);
}

void PatientSensorApp::sendJoinRequest()
{
    auto pktRequest = new Packet("PatientDataFrame");
    pktRequest->setKind(DATA);

    auto payload = makeShared<LoRaAppPacket>();
    payload->setChunkLength(B(par("dataSize").intValue()));

    // Encode: msgType = DATA, sampleMeasurement encodes urgency + patient id
    // Convention: sampleMeasurement > 0 = urgent, 0 = normal
    payload->setMsgType(DATA);
    payload->setSampleMeasurement(1);  // 1 = urgent (heart attack), 0 = normal

    auto loraTag = pktRequest->addTagIfAbsent<LoRaTag>();
    loraTag->setBandwidth(getBW());
    loraTag->setCenterFrequency(getCF());
    loraTag->setSpreadFactor(getSF());
    loraTag->setCodeRendundance(getCR());
    loraTag->setPower(mW(math::dBmW2mW(getTP())));

    sfVector.record(getSF());
    tpVector.record(getTP());
    EV << "Sending patient packet, TP: " << getTP() << ", SF: " << getSF() << endl;

    pktRequest->insertAtBack(payload);
    send(pktRequest, "socketOut");

    if (evaluateADRinNode) {
        ADR_ACK_CNT++;
        if (ADR_ACK_CNT == ADR_ACK_LIMIT) sendNextPacketWithADRACKReq = true;
        if (ADR_ACK_CNT >= ADR_ACK_LIMIT + ADR_ACK_DELAY) {
            ADR_ACK_CNT = 0;
            increaseSFIfPossible();
        }
    }
    emit(LoRa_AppPacketSent, getSF());
}
